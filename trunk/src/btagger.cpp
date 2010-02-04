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

#if HAVE_OPENMP
#include <omp.h>
#endif

#include <nlpcommon/tag.h>
#include <nlpcommon/tagset.h>
#include <nlpcommon/spejdtagsetloader.h>
#include <nlpcommon/ipipanlexer.h>

#include "rules_impl.h"
#include "brilllexeme.h"
#include "brillengine.h"
#include "ipipanrewriter.h"

using namespace std;
using namespace BTagger;

typedef BrillLexeme<Tag> MyLexeme;
typedef BestScoreMultiGoldenScorer<BinaryScorer<MyLexeme::tag_type> > MyScorer;

void lexing_progress(int token) {
    cerr << "\rLexing...  " << token;
}

void rewriting_progress(int token) {
    cerr << "\rRewriting...  " << token;
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
                    cerr << "  NOT CONSTANT: " << cat->getName() << " (" <<
                        cat->getValue(values[0]) << ' ' <<
                        cat->getValue(values[1]) << ") " << lex.getOrth() <<
                        endl;
                }
            }
        }
    }

    return cats;
}

int main(int argc, char** argv) {
    //ios_base::sync_with_stdio(false);
#if HAVE_OPENMP
    cerr << "OpenMP parallelism enabled (processors: " <<
        omp_get_num_procs() << ", dynamic thread allocation: "
        << omp_get_dynamic() << ")" << endl;

    if (!omp_get_dynamic() && getenv("OMP_NUM_THREADS") == NULL) {
        omp_set_num_threads(min(omp_get_num_procs(), 8));
    }
#endif

    SpejdTagsetLoader tagset_loader;
    ifstream config_stream("tagset.cfg");
    const Tagset* tagset = tagset_loader.loadTagset(config_stream);

    cout << "Loaded " << tagset->getCategories().size() << " categories." << endl;
    cout << "Loaded " << tagset->getPartsOfSpeech().size() << " parts of speech." << endl;

    cerr << endl << "Lexing ...";
    ifstream data_stream(argv[1]);
    IpiPanLexer<MyLexeme> lexer(data_stream);
    vector<MyLexeme> text;
    lexer.setProgressHandler(lexing_progress, 1000);
    lexer.parseStreamToVector(text, &tagset);
    cerr << "\rLexing ...  done.       " << endl;

    for (int i=0; i < 20; i++) {
        MyLexeme& lex = text[i];
        cout << "  " << lex.getOrth() << " " << lex.getAllowedTags()[0].asString(tagset) << endl;
    }

//    cout << "Looking for constant categories ..." << endl;
//    BOOST_FOREACH(const Category* cat, findConstantCategories(tagset, text))
//        cout << "  " << cat->getName() << endl;

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
        cout << "  " << lex.getOrth() << " " << lex.chosen_tag[2].asString(tagset) << endl;
    }

    string rewrite_filename = argv[1];
    if (argc >= 3) {
        cerr << endl << "Lexing ...";
        text.clear();
        ifstream data_stream(argv[2]);
        IpiPanLexer<MyLexeme> lexer(data_stream);
        lexer.setProgressHandler(lexing_progress, 1000);
        lexer.parseStreamToVector(text, &tagset);
        cerr << "\rLexing ...  done.       " << endl;
        engine.tagText(text);

        rewrite_filename = argv[2];
    } else {
        text = engine.getText();
    }

    cerr << endl << "Rewriting ...";
    ifstream rewrite_in(rewrite_filename.c_str());
    ofstream rewrite_out((rewrite_filename + ".disamb").c_str());
    IpiPanRewriter<MyLexeme> rewriter;
    rewriter.setProgressHandler(rewriting_progress, 1000);
    rewriter.rewriteStream(engine.getPhase(), tagset, text,
            rewrite_in, rewrite_out);
    cerr << "\rRewriting ...  done.       " << endl;
}
