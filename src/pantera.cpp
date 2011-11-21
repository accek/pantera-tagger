#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/mpi.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <clocale>
#include <sstream>
#include <unistd.h>

#if HAVE_OPENMP
#include <omp.h>
#endif

#include <nlpcommon/tag.h>
#include <nlpcommon/tagset.h>
#include <nlpcommon/plaintextlexer.h>
#include <nlpcommon/ipipanlexer.h>
#include <nlpcommon/nkjptextlexer.h>
#include <nlpcommon/lexemesfilter.h>
#include <nlpcommon/ipipanwriter.h>
#include <nlpcommon/nkjpwriter.h>
#include <nlpcommon/finderrors.h>
#include <nlpcommon/morfeusz.h>
#include <nlpcommon/libsegmentsentencer.h>
#include <nlpcommon/polish_segm_disamb.h>

#include "pantera.h"
#include "pantera_rules.h"

#ifndef DEFAULT_TAGSET
#define DEFAULT_TAGSET "ipipan"
#endif

#ifndef DEFAULT_ENGINE
#define DEFAULT_ENGINE "ultimarum-tertia-np0-6.btengine"
#endif

using namespace std;
using namespace NLPCommon;

po::variables_map options;

static vector<fs::path> gen_output_paths(const fs::path& input_path, 
        const string& type);

static void lexing_progress(int token) {
    cerr << "\rLexing...  " << token;
}

static void writing_progress(int token) {
    cerr << "\rWriting...  " << token;
}

static Lexer<MyLexeme>* make_lexer(const string& type,
        istream& stream) {
    if (type == "nkjp-text") {
        return new NKJPTextLexer<MyLexeme>(stream);
    } else if (type == "ipipan-morph") {
        return new IpiPanLexer<MyLexeme>(stream);
    } else if (type == "plaintext") {
        return new PlainTextLexer<MyLexeme>(stream);
    } else {
        assert(false && "Unexpected file type in make_lexer.");
    }
}

static void find_input_files(const fs::path& path,
        vector<pair<string, string> >& found_files,
        bool specified_directly = true) {
    // The vector is of (file_path, file_type).

    vector<pair<string, string> > candidates;

    if (fs::is_regular_file(path)) {
        if (path.filename() == "text_structure.xml") {
            candidates.push_back(make_pair(path.file_string(), "nkjp-text"));
        } else if (path.filename() == "morph.xml") {
            candidates.push_back(make_pair(path.file_string(),
                        "ipipan-morph"));
        } else if (path.extension() == ".xml") {
            fs::ifstream stream(path);
            const char* pattern = "<tok>";
            for (int i = 0; i < 40; i++) {
                char line[1024];
                stream.getline(line, sizeof(line));
                if (!strncmp(pattern, line, strlen(pattern))) {
                    candidates.push_back(make_pair(path.file_string(),
                                "ipipan-morph"));
                    break;
                }
            }
        } else if (path.extension() == ".txt" && specified_directly) {
            candidates.push_back(make_pair(path.file_string(),
                        "plaintext"));
        }
    } else if (fs::is_directory(path)) {
        fs::directory_iterator begin(path), end;
        BOOST_FOREACH(fs::path p, make_pair(begin, end))
            find_input_files(p, found_files, false);
    }

    typedef pair<string, string> input_pair_type;
    BOOST_FOREACH(const input_pair_type& p, candidates) {
        if (options.count("skip-done")) {
            vector<fs::path> output_paths = gen_output_paths(p.first, p.second);
            bool all_exist = true;
            BOOST_FOREACH(const fs::path& path, output_paths) {
                if (!fs::exists(path)) {
                    all_exist = false;
                    break;
                }
            }
            if (all_exist)
                continue;
        }
        found_files.push_back(p);
    }
}

static void print_status(const char* status, const string& path) {
    //cerr << "\x1b[30G\x1b[?7l" << status << ' ' << path << "\x1b[?7h\x1b[K\r" <<
    //    std::flush;
    std::ostringstream s;
    s.imbue(std::locale::classic());
    s << '[' << getpid() << "] " << status << ' ' << path << std::endl;
    cerr << s.str();
}

static vector<fs::path> gen_output_paths(const fs::path& input_path, const string&
        type) {
    vector<fs::path> ret;
    if (type == "ipipan-morph") {
        ret.push_back(input_path.file_string() + ".disamb");
    } else if (type == "nkjp-text" || type == "plaintext") {
        string segm_filename = "ann_segmentation.xml";
        string morph_filename = "ann_morphosyntax.xml";
        if (options.count("compress")) {
            segm_filename += ".gz";
            morph_filename += ".gz";
        }
        fs::path p(input_path);
        if (type == "nkjp-text" && options.count("txt-output-to-dir")) {
            p.replace_extension();
        } else {
            p.remove_filename();
        }
        ret.push_back(p / segm_filename);
        ret.push_back(p / morph_filename);
    }
    return ret;
}

boost::scoped_ptr<Lexer<MyLexeme> > lexer;

static void preprocess_file(const fs::path& path, const string& type,
        const Tagset* tagset, vector<MyLexeme>& text, bool only_lex = false) {
    fs::ifstream data_stream(path);
    lexer.reset(make_lexer(type, data_stream));

    // 1. Parser.
    print_status("LEXER", path.file_string());
    text.clear();
    lexer->setQuiet(options.count("verbose") == 0);
    lexer->parseStreamToVector(text, &tagset);

    if (only_lex)
        return;

    // 2. Sentencer.
    if (options.count("no-sentencer") == 0) {
        print_status("SENTENCER", path.file_string());
        static LibSegmentSentencer<MyLexeme> sentencer;
        text = sentencer.addSentenceDelimiters(text);
    }

    // 3. Morphological analyzer.
    if (options.count("no-morph") == 0) {

        static MorfeuszAnalyzer<MyLexeme> morfeusz(tagset,
                options.count("no-guesser") == 0);
        static PolishSegmDisambiguator<MyLexeme> segm_disamb;
        static bool first_time = true;

        if (first_time) {
            first_time = false;

            morfeusz.setQuiet(options.count("verbose") == 0);

            if (options.count("morph-dict")) {
                BOOST_FOREACH(const string& filename,
                        options["morph-dict"].as<vector<string> >())
                    morfeusz.loadMorphDict(filename);
            }
        }

        print_status("MORPH", path.file_string());
        text = morfeusz.analyzeText(text);

        print_status("SEGM-DISAMB", path.file_string());
        segm_disamb.disambiguateSegmentation(text);
    }
}

static void postprocess_file(const fs::path& path, string type,
        const vector<fs::path> output_paths, const Tagset* tagset,
        vector<MyLexeme>& text) {
    print_status("WRITER", path.file_string());

    string output_format = options["output-format"].as<string>();
    if (output_format == "xces")
        type = "ipipan-morph";
    else if (output_format == "xces-disamb")
        type = "ipipan-disamb";
    else if (output_format == "nkjp")
        type = "nkjp-text";
    else if (!output_format.empty())
        throw Exception(boost::str(
                    boost::format("unknown output format '%1%'")
                    % output_format));

    if (type == "ipipan-morph" || type == "ipipan-disamb") {
        ofstream rewrite_out((path.file_string() + ".disamb").c_str());
        IpiPanWriter<MyLexeme> writer(rewrite_out, type == "ipipan-morph");
        writer.writeVectorToStream(tagset, text);
    } else if (type == "nkjp-text" || type == "plaintext") {
        NKJPTextLexer<MyLexeme>* nkjp_lexer = dynamic_cast<NKJPTextLexer<MyLexeme>*>(lexer.get());
        if (nkjp_lexer)
            nkjp_lexer->alignText(text);

        vector<fs::path> tmp_paths;
        BOOST_FOREACH(fs::path output_path, output_paths) {
            fs::path dir(output_path);
            dir.remove_filename();
            fs::create_directories(dir);
            tmp_paths.push_back(output_path.replace_extension(".tmp"));
        }

        fs::ofstream segm_file(tmp_paths[0]);
        fs::ofstream morph_file(tmp_paths[1]);

        boost::iostreams::filtering_stream<boost::iostreams::output> segm_stream;
        boost::iostreams::filtering_stream<boost::iostreams::output> morph_stream;
        if (options.count("compress")) {
            segm_stream.push(boost::iostreams::gzip_compressor(options.count("compress"), 1024*1024));
            morph_stream.push(boost::iostreams::gzip_compressor(options.count("compress"), 1024*1024));
        }
        segm_stream.push(segm_file);
        morph_stream.push(morph_file);

        NKJPSegmWriter<MyLexeme> segm_writer(segm_stream);
        segm_writer.writeVectorToStream(tagset, text);
        NKJPMorphWriter<MyLexeme> morph_writer(morph_stream,
                options["nkjp-tool-name"].as<string>());
        morph_writer.writeVectorToStream(tagset, text);

        BOOST_FOREACH(const fs::path& output_path, output_paths) {
            if (fs::exists(output_path))
                fs::remove(output_path);
        }

        for (int i = 0; i < output_paths.size(); i++) {
            fs::rename(tmp_paths[i], output_paths[i]);
        }
    }
}

void analyze_errors(const string& filename, const Tagset* tagset,
        const vector<MyLexeme>& text) {
    bool has_golden = false;
    BOOST_FOREACH(const MyLexeme& lex, text) {
        if (!lex.getGoldenTags().empty()) {
            has_golden = true;
            break;
        }
    }
    if (!has_golden)
        return;

    wofstream stream(filename.c_str());
    stream.imbue(get_utf8_locale());
    print_status("ERRORS", filename);
    TaggingErrorsCollector<MyLexeme> errors_collector(tagset);
    errors_collector.addTaggingErrors(text);
    errors_collector.printErrors(stream);
}

void parse_command_line(int argc, char** argv) {
    // Declare a group of options that will be 
    // allowed only on command line
    po::options_description generic("Generic options");
    generic.add_options()
        ("version,V", "print version string")
        ("help", "produce help message")
        ("config,c", po::value<string>(), "name of a configuration file")
        ;
    generic.add(tagset_options());

    // Declare a group of options that will be 
    // allowed both on command line and in
    // config file
    po::options_description config("Configuration");
    config.add_options()
        ("verbose,v", "be verbose")
        ("compress,z", "compress output, if possible; use this option "
         "multiple times for higher compression levels.")
        ("skip-done", "skip processing if output files exist")
        ("tagset,t", po::value<string>()->default_value(DEFAULT_TAGSET),
         "name of tagset to use (or a Spejd-compatible tagset config file, "
         "or a multitier tagset folder containing 0.cfg, 1.cfg etc.)")
        ("no-sentencer", "disable sentencer")
        ("no-morph", "disable morphological analyzer (and guesser)")
        ("no-guesser", "disable guesser")
        ("no-tagger", "disable tagger")
        ("engine", po::value<string>()->default_value(DEFAULT_ENGINE),
         "Brill engine to use (file ending with .btengine)")
        ("output-format,o", po::value<string>()->default_value(""),
         "Output format ('nkjp', 'xces' or 'xces-disamb')")
        ("create-engine,T", po::value<string>(),
         "Enter training mode. The argument to this option is the output "
         ".btengine file to produce. Training files should be provided "
         "using --training-data options.")
        ("training-data", po::value<vector<string> >(),
         "training data (files or folders)")
        ("threshold", po::value<int>()->default_value(50),
         "training threshold")
        ("morph-dict,d", po::value<vector<string> >(),
         "extra morphological dictionaries, see documentation "
         "on the project page for format examples.")
        ("nkjp-tool-name", po::value<string>()->default_value("pantera"),
         "the name of the tool to use when producing NKJP XML output "
         "(default is 'pantera').")
        ("txt-output-to-dir", "create a subfolder for every .txt file parsed")
        ;

    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("input-file", po::value< vector<string> >(), "input file")
        ;

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    po::options_description config_file_options;
    config_file_options.add(config).add(hidden);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    po::positional_options_description p;
    p.add("input-file", -1);

    store(po::command_line_parser(argc, argv).
          options(cmdline_options).positional(p).run(), options);
    notify(options);

    ifstream config_ifs(PACKAGE ".cfg");
    if (config_ifs) {
        store(po::parse_config_file(config_ifs, config_file_options), options);
        notify(options);
    }

    if (options.count("config")) {
        string config_file = options["config"].as<string>();
        ifstream ifs(config_file.c_str());
        if (!ifs) {
            throw Exception(boost::str(
                        boost::format("cannot open config file '%1%'")
                        % config_file));
        }
        store(po::parse_config_file(ifs, config_file_options), options);
        notify(options);
    }

    bool show_help = false;

    if (options.count("help")) {
        show_help = true;
    } else {
        if (options.count("version")) {
            cerr << PACKAGE_STRING << endl;
            exit(0);
        }

        if (!options.count("input-file") && !options.count("create-engine")) {
            std::cerr << "error: No input file specified" << std::endl;
            show_help = true;
        }

        if (options.count("create-engine") && !options.count("training-data")) {
            std::cerr << "error: No training data specified, use --training-data"
                << std::endl;
            show_help = true;
        }

        if (options.count("create-engine")
                && options["engine"].as<string>() != DEFAULT_ENGINE) {
            std::cerr << "error: Options --create-engine and --engine are "
                "mutually exclusive" << std::endl;
            show_help = true;
        }
    }

    if (show_help) {
        cerr << visible << endl;
        exit(1);
    }
}

template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " "));
    return os;
}

int real_main(mpi::communicator& world, int argc, char** argv) {
    parse_command_line(argc, argv);

    // 1. Setup engine.
    vector<const Tagset*> tagsets = load_tagsets(
            options["tagset"].as<string>());
    const Tagset* tagset = tagsets[tagsets.size() - 1];

    BTagger::BrillEngine<MyLexeme, MyScorer> engine;
    vector<BTagger::RulesGenerator<MyLexeme>*> rule_generators;
    add_phases_to_engine(engine, tagsets, rule_generators);

    typedef pair<string, string> input_pair_type;

    if (world.rank() == 0) {
        if (options.count("create-engine")) {
            // Training mode.

            vector<input_pair_type> training_files;
            cerr << "Scanning for training files ... ";
            BOOST_FOREACH(const string& file,
                    options["training-data"].as< vector<string> >())
                find_input_files(file, training_files);
            cerr << training_files.size() << " found" << endl;

            vector<MyLexeme> all_text;
            BOOST_FOREACH(const input_pair_type& input_pair, training_files) {
                vector<MyLexeme> text;
                preprocess_file(input_pair.first, input_pair.second, tagset,
                        text, true);
                all_text.insert(all_text.end(), text.begin(), text.end());
            }

            LexemesFilter<MyLexeme> segments_filter(MyLexeme::SEGMENT);
            all_text = segments_filter.filterText(all_text);
            
            engine.setText(all_text);

        } else if (!options.count("no-tagger")) {
            // No training mode.
            cerr << endl << "Loading engine ..." << endl;
            fs::path engine_path = find_with_path(ENGINES_PATH,
                    options["engine"].as<string>());

            try {
                fs::ifstream data_stream(engine_path, ios::in);
                boost::archive::text_iarchive engine_archive(data_stream);
                engine_archive >> engine;
            } catch (...) {
                fs::ifstream data_stream(engine_path, ios::in | ios::binary);
                boost::archive::binary_iarchive engine_archive(data_stream);
                engine_archive >> engine;
                data_stream.close();

                cerr << "Converting engine to newer file format ..." << endl;

                fs::path backup_path(engine_path);
                backup_path.replace_extension(".bak");
                fs::rename(engine_path, backup_path);

                fs::ofstream data_ostream(engine_path, ios::out);
                boost::archive::text_oarchive engine_oarchive(data_ostream);
                engine_oarchive << engine;
                data_stream.close();

                cerr << endl;
                cerr << "The engine has been converted to a newer file format."
                    << endl;
                cerr << "The original engine was saved as " << endl;
                cerr << backup_path << endl << endl;

                cerr << "Please run the tagger again." << endl;

                return 2;
            }

        }

        cerr << "Sending data to all worker processes ..." << endl;
    }

    if (options.count("create-engine") || !options.count("no-tagger")) {
        mpi::broadcast(world, engine, 0);
    }

    // Run training if needed.
    if (options.count("create-engine")) {
        int threshold = options["threshold"].as<int>();
        for (int i = 0; i < engine.numPhases(); i++)
            engine.runPhase(i, rule_generators[i], threshold);

        // Save engine if needed.
        if (world.rank() == 0) {
            string engine_filename = options["create-engine"].as<string>();
            ofstream engine_stream(engine_filename.c_str(), ios::out);
            cerr << endl << "Saving engine to " << engine_filename << " ...  ";
            boost::archive::text_oarchive engine_archive(engine_stream);
            engine_archive << engine;
            cerr << "done." << endl;
        }
    }

    if (options.count("create-engine") || !options.count("no-tagger")) {
        engine.setQuiet(options.count("verbose") == 0);
    }

    // Scan for input files.
    vector<input_pair_type> input_files;
    if (world.rank() == 0 && options.count("input-file")) {
        vector<string> option_values = options["input-file"].as< vector<string> >();
        cerr << "Scanning for input files ... ";
        BOOST_FOREACH(const string& file, option_values)
            find_input_files(file, input_files);
        cerr << input_files.size() << " found" << endl;
    }
    mpi::broadcast(world, input_files, 0);

    MPIProgressController progress(world);
    progress.init("Processing ...", input_files.size());
    float done = 0;
    int num_errors = 0;
    for (int i = world.rank(); i < input_files.size(); i += world.size()) {
        const input_pair_type& input_pair = input_files[i];
        const string& path = input_pair.first;
        float progress_left = 1.0;

        try {
            vector<fs::path> output_paths = gen_output_paths(input_pair.first,
                    input_pair.second);

            vector<MyLexeme> text;
            preprocess_file(input_pair.first, input_pair.second, tagset, text);
            progress.set(done += 0.3);
            progress_left -= 0.3;

            if (!options.count("no-tagger")) {
                print_status("PRE-TAGGER", path);
                LexemesFilter<MyLexeme> segments_filter(MyLexeme::SEGMENT);
                text = segments_filter.filterText(text);
                print_status("TAGGER", path);
                engine.tagText(text);
                progress.set(done += 0.4);
                progress_left -= 0.4;

                print_status("POST-TAGGER", path);
                analyze_errors(string(input_pair.first) + ".errors.txt", tagset, text);
                text = segments_filter.unfilterText(text);
            }

            postprocess_file(input_pair.first, input_pair.second, output_paths, tagset, text);
            print_status("DONE", path);
        } catch (std::exception const& e) {
            cerr << "[" << path << "] ERROR: Caught " << typeid(e).name() << " with message: " << e.what() << endl;
            num_errors++;
        } catch (...) {
            cerr << "[" << path << "] ERROR: Caught unknown exception." << endl;
            num_errors++;
        }

        progress.set(done += progress_left);
    }
    progress.finish();

    int all_errors;
    mpi::reduce(world, num_errors, all_errors, std::plus<int>(), 0);

    if (world.rank() == 0) {
        cerr << "All done.";
        if (all_errors > 0) {
            cerr << " Encountered " << all_errors << " error(s).";
        }
        cerr << endl;
        return all_errors > 0 ? 1 : 0;
    }

    return 0;
}

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);

    // This was needed for correct wcin/wcout behaviour on MacOS.
    // As we do not support it anymore and it seems to interfere
    // with std::locale::global(std::locale("")), we leave it
    // commented out.
    //setlocale(LC_CTYPE, "");
    
    // Use the locale from the environment.
    try {
        std::locale::global(std::locale(""));
    } catch (...) {
        std::cerr << "warning: terminal-compatible locale is "
            "not supported; this may cause incorrect handling "
            "of non-ASCII characters, you take the risk!" << std::endl;
    }

    wcerr.imbue(std::locale());

    mpi::environment env(argc, argv);
    mpi::communicator world;

#if HAVE_OPENMP
    cerr << "OpenMP parallelism enabled (processors: " <<
        omp_get_num_procs() << ", dynamic thread allocation: "
        << omp_get_dynamic() << ")" << endl;

    if (!omp_get_dynamic() && getenv("OMP_NUM_THREADS") == NULL) {
        omp_set_num_threads(min(omp_get_num_procs(), 8));
    }
#endif

    try {
        return real_main(world, argc, argv);
    } catch (std::exception const& e) {
        cerr << "***FATAL***: Caught " << typeid(e).name() << " with message:" << endl;
        cerr << e.what() << endl;
        throw;
    } catch (...) {
        cerr << "***FATAL***: Caught unknown exception." << endl;
        throw;
    }
}
