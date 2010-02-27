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

#include "rules/impl.h"
#include "brilllexeme.h"
#include "brillengine.h"
#include "ipipanrewriter.h"

using namespace std;
using namespace BTagger;

typedef BrillLexeme<Tag> MyLexeme;
//typedef BestScoreMultiGoldenScorer<CAScorer<MyLexeme::tag_type> > MyScorer;
//typedef BestScoreMultiGoldenScorer<BinaryScorer<MyLexeme::tag_type> > MyScorer;

class MySingleScorer : public CAScorer<MyLexeme::tag_type>{
    public:
        MySingleScorer(const Tagset* tagset) :
            CAScorer<MyLexeme::tag_type>(tagset) { }

        score_type getPosRelativeWeight() {
            return 0.3;
        }

        score_type getNonExactMatchFactor() {
            return 0.3;
        }
};

typedef BestScoreMultiGoldenScorer<MySingleScorer> MyScorer;

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
    TaggingErrorsCollector<MyLexeme> errors_collector(tagset);
    errors_collector.addTaggingErrors(text);

    const vector<TaggingErrorsCollector<MyLexeme>::error_type>& errors =
        errors_collector.getErrors();
    wcerr << "found " << errors.size() << " errors." << endl;

    const vector<TaggingErrorsCollector<MyLexeme>::group_type>& groups =
        errors_collector.getGroups();
    wcerr << "found " << groups.size() << " groups." << endl;

    for (int i = 0; i < groups.size(); i++) {
        stream << "=== ERROR GROUP " << i << " ===" << endl;
        stream << "  " << groups[i].second.size() << ' ' << groups[i].first
            << endl;
        for (int j = 0; j < groups[i].second.size(); j++)
            stream << groups[i].second[j] << endl;
        stream << endl;
    }
}

int main(int argc, char** argv) {
    // This is needed for correct wcin/wcout behaviour.
    setlocale(LC_CTYPE, "");

#if HAVE_OPENMP
    wcerr << "OpenMP parallelism enabled (processors: " <<
        omp_get_num_procs() << ", dynamic thread allocation: "
        << omp_get_dynamic() << ")" << endl;

    if (!omp_get_dynamic() && getenv("OMP_NUM_THREADS") == NULL) {
        omp_set_num_threads(min(omp_get_num_procs(), 8));
    }
#endif

    SpejdTagsetLoader tagset_loader;
    ifstream config_stream("tagset.cfg");
    const Tagset* tagset = tagset_loader.loadTagset(config_stream);

    wcout << "Loaded " << tagset->getCategories().size() << " categories." << endl;
    wcout << "Loaded " << tagset->getPartsOfSpeech().size() << " parts of speech." << endl;

    wcerr << endl << "Lexing ...";
    ifstream data_stream(argv[1]);
    IpiPanLexer<MyLexeme> lexer(data_stream);
    vector<MyLexeme> text;
    lexer.setProgressHandler(lexing_progress, 1000);
    lexer.parseStreamToVector(text, &tagset);
    wcerr << "\rLexing ...  done.       " << endl;

    for (int i=0; i < 20; i++) {
        MyLexeme& lex = text[i];
        wcout << "  " << lex.getOrth() << " " << lex.getAllowedTags()[0].asWString(tagset) << endl;
    }

//    wcout << "Looking for constant categories ..." << endl;
//    BOOST_FOREACH(const Category* cat, findConstantCategories(tagset, text))
//        wcout << "  " << cat->getName() << endl;

    BrillEngine<MyLexeme, MyScorer> engine;
    engine.init(text, tagset);

    ifstream config_stream2("tagset2.cfg");
    Tagset* tagset2 = tagset_loader.loadTagset(config_stream2);
    ifstream config_stream3("tagset3.cfg");
    Tagset* tagset3 = tagset_loader.loadTagset(config_stream3);

    vector<const Tagset*> tagsets;
    tagsets.push_back(tagset2);
    RulesGenerator<MyLexeme>* g1 = Rules::make_p1_rules_generator<MyLexeme, 0>(tagsets);
    tagsets.push_back(tagset3);
    RulesGenerator<MyLexeme>* g2 = Rules::make_p2_rules_generator<MyLexeme, 1>(tagsets);
    tagsets.push_back(tagset);
    RulesGenerator<MyLexeme>* g3 = Rules::make_p2_rules_generator<MyLexeme, 2>(tagsets);

    int threshold = 50;
    if (argc >= 4) {
        threshold = atoi(argv[3]);
    }

    engine.runPhase(tagset2, g1, threshold);
    engine.runPhase(tagset3, g2, threshold);
    engine.runPhase(tagset, g3, threshold);

    for (int i=0; i < 20; i++) {
        const MyLexeme& lex = engine.getText()[i];
        wcout << "  " << lex.getOrth() << " "
            << lex.chosen_tag[2].asWString(tagset) << endl;
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
        engine.tagText(text);

        rewrite_filename = argv[2];
    } else {
        text = engine.getText();
    }

    wofstream errors_report((rewrite_filename + ".errors.txt").c_str());
    analyzeErrors(errors_report, tagset, text);

    wcerr << endl << "Rewriting ...";
    ifstream rewrite_in(rewrite_filename.c_str());
    ofstream rewrite_out((rewrite_filename + ".disamb").c_str());
    IpiPanRewriter<MyLexeme> rewriter;
    rewriter.setProgressHandler(rewriting_progress, 1000);
    rewriter.rewriteStream(engine.getPhase(), tagset, text,
            rewrite_in, rewrite_out);
    wcerr << "\rRewriting ...  done.       " << endl;
}
