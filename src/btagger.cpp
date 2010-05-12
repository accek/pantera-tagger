/*
 ============================================================================
 Name        : btagger.cpp
 Author      : Szymon Acedanski
 Version     :
 Copyright   : Copyright (C) Institute of CS, Polish Academy of Sciences
 Description : Hello World in C++,
 ============================================================================
 */

#include <boost/foreach.hpp>
#include <boost/mpi.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <clocale>

#if HAVE_OPENMP
#include <omp.h>
#endif

#include <nlpcommon/tag.h>
#include <nlpcommon/tagset.h>
#include <nlpcommon/spejdtagsetloader.h>
#include <nlpcommon/ipipanlexer.h>
#include <nlpcommon/cascorer.h>
#include <nlpcommon/finderrors.h>
#include <nlpcommon/lexemesfilter.h>
#include <nlpcommon/ipipanwriter.h>

#include "rules/impl.h"
#include "brilllexeme.h"
#include "brillengine.h"
#include "ipipanrewriter.h"

using namespace std;
using namespace NLPCommon;
namespace mpi = boost::mpi;

typedef BTagger::BrillLexeme<Tag> MyLexeme;

class MySingleScorer : public CAScorer<false, MyLexeme::tag_type>{
    private:
        vector<const Category*> important_cats;
    public:
        MySingleScorer(const Tagset* tagset) :
                CAScorer<false, MyLexeme::tag_type>(tagset) {
            important_cats.push_back(tagset->getCategory("case", false));
            important_cats.push_back(tagset->getCategory("gender", false));
        }

        score_type getPosRelativeWeight() {
            return 0.5;
        }

        score_type getCategoryRelativeWeight(const PartOfSpeech* pos,
                const Category* category) {
            if (std::find(important_cats.begin(), important_cats.end(),
                        category) != important_cats.end())
                return 1.0;
            return 0.5;
        }

        score_type getNonExactMatchFactor() {
            return 0.5;
        }
};

//typedef BestScoreMultiGoldenScorer<MySingleScorer> MyScorer;
//typedef BestScoreMultiGoldenScorer<CAScorer<MyLexeme::tag_type> > MyScorer;
typedef BestScoreMultiGoldenScorer<BinaryScorer<MyLexeme::tag_type> > MyScorer;

void lexing_progress(int token) {
    wcerr << "\rLexing...  " << token;
}

void rewriting_progress(int token) {
    wcerr << "\rRewriting...  " << token;
}

template<class Lexeme>
vector<const Category*> findConstantCategories(const Tagset* tagset, vector<Lexeme>& text)
{
    typedef typename Lexeme::tag_type tag_type;

    vector<const Category*> cats(tagset->getCategories().begin(),
            tagset->getCategories().end());

    BOOST_FOREACH(Lexeme& lex, text) {
        BOOST_FOREACH(const Category* cat, tagset->getCategories()) {
            vector<int> values;
            int num_values = 0;
            int category_index = tagset->getCategoryIndex(cat);
            BOOST_FOREACH(const tag_type& tag, lex.getAllowedTags()) {
                const PartOfSpeech* pos = tagset->getPartOfSpeech(tag.getPos());
                if (pos->hasCategory(cat))
                    values.push_back(tag.getValue(category_index));
            }
            sort(values.begin(), values.end());
            values.resize(unique(values.begin(), values.end()) -
                    values.begin());
            if (values.size() > 1) {
                vector<const Category*>::iterator it = find(cats.begin(),
                        cats.end(), cat);
                if (it != cats.end()) {
                    cats.erase(it);
                    wcerr << "  NOT CONSTANT: " << cat->getWName() << " (" <<
                        cat->getWValue(values[0]) << ' ' <<
                        cat->getWValue(values[1]) << ") " << lex.getOrth() <<
                        endl;
                }
            }
        }
    }

    return cats;
}

template<class Lexeme>
void analyzeErrors(wostream& stream, const Tagset* tagset,
        const vector<Lexeme>& text) {
    wcerr << endl << "Analyzing errors ..." << endl;
    TaggingErrorsCollector<Lexeme> errors_collector(tagset);
    errors_collector.addTaggingErrors(text);

    const vector<typename TaggingErrorsCollector<Lexeme>::error_type>& errors =
        errors_collector.getErrors();
    wcerr << "found " << errors.size() << " errors." << endl;

    const vector<typename TaggingErrorsCollector<Lexeme>::group_type>& groups =
        errors_collector.getGroups();
    wcerr << "found " << groups.size() << " groups." << endl;

    errors_collector.printErrors(stream);
}

bool ends_with(const string& fullString, const string& ending)
{
    if (fullString.length() > ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(),
            ending.length(), ending));
    } else {
        return false;
    }
}

int real_main(mpi::communicator& world, int argc, char** argv) {
    vector<MyLexeme> text;
    const Tagset* tagset;
    const Tagset* tagset2;
    const Tagset* tagset3;

    SpejdTagsetLoader tagset_loader;
    ifstream config_stream("tagset.cfg");
    tagset = tagset_loader.loadTagset(config_stream);
    ifstream config_stream2("tagset2.cfg");
    tagset2 = tagset_loader.loadTagset(config_stream2);
    ifstream config_stream3("tagset3.cfg");
    tagset3 = tagset_loader.loadTagset(config_stream3);

    if (world.rank() == 0) {
        wcout << "Loaded " << tagset->getCategories().size() << " categories." << endl;
        wcout << "Loaded " << tagset->getPartsOfSpeech().size() << " parts of speech." << endl;
    }

    vector<const Tagset*> tagsets;
    tagsets.push_back(tagset2);
    BTagger::RulesGenerator<MyLexeme>* g1 = BTagger::Rules::make_p1_rules_generator<MyLexeme, 0>(tagsets);
    tagsets.push_back(tagset3);
    BTagger::RulesGenerator<MyLexeme>* g2 = BTagger::Rules::make_p2_rules_generator<MyLexeme, 1>(tagsets);
    tagsets.push_back(tagset);
    BTagger::RulesGenerator<MyLexeme>* g3 = BTagger::Rules::make_p2_rules_generator<MyLexeme, 2>(tagsets);

    BTagger::BrillEngine<MyLexeme, MyScorer> engine;
    engine.addPhase(tagset2, g1->getTStore());
    engine.addPhase(tagset3, g2->getTStore());
    engine.addPhase(tagset, g3->getTStore());

    LexemesFilter<MyLexeme> segments_filter(MyLexeme::SEGMENT);

    string input_filename(argv[1]);
    bool loaded_engine = false;
    if (world.rank() == 0) {
        ifstream data_stream(input_filename.c_str(), ios::in | ios::binary);
        if (ends_with(input_filename, ".btengine")) {
            wcerr << endl << "Loading engine ..." << endl;
            boost::archive::binary_iarchive engine_archive(data_stream);
            engine_archive >> engine;
            loaded_engine = true;
        } else {
            wcerr << endl << "Lexing ...";
            IpiPanLexer<MyLexeme> lexer(data_stream);
            lexer.setProgressHandler(lexing_progress, 1000);
            lexer.parseStreamToVector(text, &tagset);
            wcerr << "\rLexing ...  done.       " << endl;
            text = segments_filter.filterText(text);

            for (int i=0; i < std::min<int>(text.size(), 20); i++) {
                MyLexeme& lex = text[i];
                wcout << "  " << lex.getOrth()
                    << " " << lex.getAllowedTags()[0].asWString(tagset) << endl;
            }

            // Check if golden \subset allowed
            BOOST_FOREACH(MyLexeme& lex, text)
                BOOST_FOREACH(const Tag& tag, lex.getGoldenTags())
                    assert(lex.isAllowedTag(tag));

        //    wcout << "Looking for constant categories ..." << endl;
        //    BOOST_FOREACH(const Category* cat, findConstantCategories(tagset, text))
        //        wcout << "  " << cat->getName() << endl;

            engine.setText(text);
        }

        wcerr << "Sending data to all worker processes ..." << endl;
    }

    mpi::broadcast(world, engine, 0);


    int threshold = 50;
    if (argc >= 4) {
        threshold = atoi(argv[3]);
    }

    if (!engine.isTrained()) {
        engine.runPhase(0, g1, threshold);
        engine.runPhase(1, g2, threshold);
        engine.runPhase(2, g3, threshold);
    }

    if (world.rank() == 0) {
        /*
        for (int i=0; i < 20; i++) {
            const MyLexeme& lex = engine.getText()[i];
            wcout << "  " << lex.getOrth() << " "
                << lex.chosen_tag[2].asWString(tagset) << endl;
        }
        */

        if (!loaded_engine) {
            ofstream engine_stream((input_filename + ".btengine").c_str(),
                    ios::out | ios::binary);
            wcerr << endl << "Saving engine to " << ascii_to_wstring(input_filename + ".btengine") << " ..." << endl;
            boost::archive::binary_oarchive engine_archive(engine_stream);
            engine_archive << engine;
        }

        string rewrite_filename = argv[1];
        if (argc >= 3) {
            wcerr << endl << "Lexing ...";
            text.clear();
            ifstream data_stream(argv[2]);
            IpiPanLexer<MyLexeme> lexer(data_stream);
            lexer.setProgressHandler(lexing_progress, 1000);
            lexer.parseStreamToVector(text, &tagset);
            wcerr << "\rLexing ...  done.       " << endl;

            text = segments_filter.filterText(text);
            engine.tagText(text);

            rewrite_filename = argv[2];
        } else {
            text = engine.getText();
        }

        wofstream errors_report((rewrite_filename + ".errors.txt").c_str());
        std::locale errors_locale;
        try {
            errors_locale = std::locale("");
            if (errors_locale.name() == "*")
                errors_locale = std::locale("pl_PL.UTF-8");
        } catch (...) { }
        errors_report.imbue(errors_locale);
        analyzeErrors(errors_report, tagset, text);
        errors_report.close();

        wcerr << endl << "Rewriting ...";
        text = segments_filter.unfilterText(text);
        ofstream rewrite_out((rewrite_filename + ".disamb").c_str());
        IpiPanWriter<MyLexeme> writer(rewrite_out);
        writer.setProgressHandler(rewriting_progress, 1000);
        writer.writeVectorToStream(tagset, text);
        wcerr << "\rRewriting ...  done.       " << endl;
    }
}

int main(int argc, char** argv) {
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
