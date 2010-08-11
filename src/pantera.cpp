#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/mpi.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <clocale>

#if HAVE_OPENMP
#include <omp.h>
#endif

#include <nlpcommon/tag.h>
#include <nlpcommon/tagset.h>
#include <nlpcommon/ipipanlexer.h>
#include <nlpcommon/nkjptextlexer.h>
#include <nlpcommon/lexemesfilter.h>
#include <nlpcommon/ipipanwriter.h>
#include <nlpcommon/nkjpwriter.h>
#include <nlpcommon/finderrors.h>
#include <nlpcommon/morfeusz.h>
#include <nlpcommon/libsegmentsentencer.h>

#include "rules/impl.h"
#include "brilllexeme.h"
#include "brillengine.h"

#ifndef DEFAULT_TAGSET
#define DEFAULT_TAGSET "ipipan"
#endif

#ifndef DEFAULT_ENGINE
#define DEFAULT_ENGINE "nkjp-wypluwka20091216-t6-np4.btengine"
#endif

using namespace std;
using namespace NLPCommon;

po::variables_map options;

typedef BTagger::BrillLexeme<Tag> MyLexeme;
typedef BestScoreMultiGoldenScorer<BinaryScorer<MyLexeme::tag_type> > MyScorer;

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
    } else {
        assert(false && "Unexpected file type in make_lexer.");
    }
}

static void find_input_files(const fs::path& path,
        vector<pair<string, string> >& found_files) {
    // The vector is of (file_path, file_type).

    if (fs::is_regular_file(path)) {
        if (path.filename() == "text_structure.xml") {
            found_files.push_back(make_pair(path.file_string(), "nkjp-text"));
        } else if (path.filename() == "morph.xml") {
            found_files.push_back(make_pair(path.file_string(),
                        "ipipan-morph"));
        } else if (path.extension() == ".xml") {
            fs::ifstream stream(path);
            const char* pattern = "<tok>";
            for (int i = 0; i < 40; i++) {
                char line[1024];
                stream.getline(line, sizeof(line));
                if (!strncmp(pattern, line, strlen(pattern))) {
                    found_files.push_back(make_pair(path.file_string(),
                                "ipipan-morph"));
                    break;
                }
            }
        }
    } else if (fs::is_directory(path)) {
        fs::directory_iterator begin(path), end;
        BOOST_FOREACH(fs::path p, make_pair(begin, end))
            find_input_files(p, found_files);
    }
}

void setup_engine(BTagger::BrillEngine<MyLexeme, MyScorer>& engine,
        const vector<const Tagset*>& tagsets,
        vector<BTagger::RulesGenerator<MyLexeme>*>& rule_generators) {
    if (tagsets.size() > 3) {
        throw Exception("Tagsets with more than 3 phases are not supported.");
    }

    BTagger::RulesGenerator<MyLexeme>* g1 =
        BTagger::Rules::make_p1_rules_generator<MyLexeme, 0>(tagsets);
    engine.addPhase(tagsets[0], g1->getTStore());
    rule_generators.push_back(g1);

    if (tagsets.size() >= 2) {
        BTagger::RulesGenerator<MyLexeme>* g2 =
            BTagger::Rules::make_p2_rules_generator<MyLexeme, 1>(tagsets);
        engine.addPhase(tagsets[1], g2->getTStore());
        rule_generators.push_back(g2);
    }

    if (tagsets.size() >= 3) {
        BTagger::RulesGenerator<MyLexeme>* g3 =
            BTagger::Rules::make_p2_rules_generator<MyLexeme, 2>(tagsets);
        engine.addPhase(tagsets[2], g3->getTStore());
        rule_generators.push_back(g3);
    }
}

static void print_status(const char* status, const string& path) {
    cerr << "\x1b[30G\x1b[?7l" << status << ' ' << path << "\x1b[?7h\x1b[K\r";
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
        print_status("MORPH", path.file_string());
        static MorfeuszAnalyzer<MyLexeme> morfeusz(tagset,
                options.count("no-guesser") == 0);
        morfeusz.setQuiet(options.count("verbose") == 0);

        if (options.count("morph-dict")) {
            BOOST_FOREACH(const string& filename,
                    options["morph-dict"].as<vector<string> >())
                morfeusz.loadMorphDict(filename);
        }

        text = morfeusz.analyzeText(text);
    }
}

static void postprocess_file(const fs::path& path, const string& type,
        const Tagset* tagset, vector<MyLexeme>& text) {
    print_status("WRITER", path.file_string());

    if (type == "ipipan-morph") {
        ofstream rewrite_out((path.file_string() + ".disamb").c_str());
        IpiPanWriter<MyLexeme> writer(rewrite_out);
        writer.writeVectorToStream(tagset, text);
    } else if (type == "nkjp-text") {
        dynamic_cast<NKJPTextLexer<MyLexeme>*>(lexer.get())->alignText(text);

        fs::path p(path);
        p.remove_filename();
        fs::ofstream out_stream(p / "ann_segmentation.xml");
        NKJPSegmWriter<MyLexeme> writer(out_stream);
        writer.writeVectorToStream(tagset, text);

        fs::ofstream out_stream2(p / "ann_morphosyntax.xml");
        NKJPMorphWriter<MyLexeme> writer2(out_stream2);
        writer2.writeVectorToStream(tagset, text);
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
    std::locale errors_locale;
    try {
        errors_locale = std::locale("");
        if (errors_locale.name() == "*")
            errors_locale = std::locale("pl_PL.UTF-8");
    } catch (...) { }
    stream.imbue(errors_locale);

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
        ("tagset,t", po::value<string>()->default_value(DEFAULT_TAGSET),
         "name of tagset to use (or a Spejd-compatible tagset config file, "
         "or a multitier tagset folder containing 0.cfg, 1.cfg etc.)")
        ("no-sentencer", "disable sentencer")
        ("no-morph", "disable morphological analyzer (and guesser)")
        ("no-guesser", "disable guesser")
        ("engine", po::value<string>()->default_value(DEFAULT_ENGINE),
         "Brill engine to use (file ending with .btengine)")
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

    if (options.count("help")) {
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
    /*if (!options.count("input-file")) {
        std::cerr << "No input file specified" << std::endl;
        return 1;
    }*/

    // 1. Setup engine.
    vector<const Tagset*> tagsets = load_tagsets(
            options["tagset"].as<string>());
    const Tagset* tagset = tagsets[tagsets.size() - 1];

    BTagger::BrillEngine<MyLexeme, MyScorer> engine;
    vector<BTagger::RulesGenerator<MyLexeme>*> rule_generators;
    setup_engine(engine, tagsets, rule_generators);

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

        } else {
            // No training mode.
            cerr << endl << "Loading engine ..." << endl;
            fs::path engine_path = find_with_path(ENGINES_PATH,
                    options["engine"].as<string>());
            fs::ifstream data_stream(engine_path, ios::in | ios::binary);
            boost::archive::binary_iarchive engine_archive(data_stream);
            engine_archive >> engine;
        }

        cerr << "Sending data to all worker processes ..." << endl;
    }

    mpi::broadcast(world, engine, 0);

    // Run training if needed.
    int threshold = options["threshold"].as<int>();
    if (!engine.isTrained())
        for (int i = 0; i < engine.numPhases(); i++)
            engine.runPhase(i, rule_generators[i], threshold);

    // Save engine if needed.
    if (world.rank() == 0 && options.count("create-engine")) {
        string engine_filename = options["create-engine"].as<string>();
        ofstream engine_stream(engine_filename.c_str(), ios::out |
                ios::binary);
        cerr << endl << "Saving engine to " << engine_filename << " ...  ";
        boost::archive::binary_oarchive engine_archive(engine_stream);
        engine_archive << engine;
        cerr << "done." << endl;
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

    engine.setQuiet(options.count("verbose") == 0);

    MPIProgressController progress(world);
    progress.init("Processing ...", input_files.size());
    float done = 0;
    for (int i = world.rank(); i < input_files.size(); i += world.size()) {
        const input_pair_type& input_pair = input_files[i];
        const string& path = input_pair.first;

        vector<MyLexeme> text;
        preprocess_file(input_pair.first, input_pair.second, tagset, text);
        progress.set(done += 0.3);

        print_status("PRE-TAGGER", path);
        LexemesFilter<MyLexeme> segments_filter(MyLexeme::SEGMENT);
        text = segments_filter.filterText(text);
        print_status("TAGGER", path);
        engine.tagText(text);
        progress.set(done += 0.4);

        print_status("POST-TAGGER", path);
        analyze_errors(string(input_pair.first) + ".errors.txt", tagset, text);
        text = segments_filter.unfilterText(text);
        postprocess_file(input_pair.first, input_pair.second, tagset, text);
        progress.set(done += 0.3);
    }
    progress.finish();

    if (world.rank() == 0)
        cerr << "All done." << endl;
}

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);

    // This is needed for correct wcin/wcout behaviour.
    setlocale(LC_CTYPE, "");

    mpi::environment env(argc, argv);
    mpi::communicator world;

#if HAVE_OPENMP
    wcerr << "OpenMP parallelism enabled (processors: " <<
        omp_get_num_procs() << ", dynamic thread allocation: "
        << omp_get_dynamic() << ")" << endl;

    if (!omp_get_dynamic() && getenv("OMP_NUM_THREADS") == NULL) {
        omp_set_num_threads(min(omp_get_num_procs(), 8));
    }
#endif

    try {
        return real_main(world, argc, argv);
    } catch (std::exception const& e) {
        wcerr << "***FATAL***: Caught " << typeid(e).name() << " with message:" << endl;
        wcerr << e.what() << endl;
        throw;
    } catch (...) {
        wcerr << "***FATAL***: Caught unknown exception." << endl;
        throw;
    }
}
