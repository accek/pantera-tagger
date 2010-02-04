/*
 ============================================================================
 Name        : xces2corpus.cpp
 Author      : Szymon Acedanski
 Version     :
 Copyright   : Copyright (C) Institute of CS, Polish Academy of Sciences
 Description : Hello World in C++,
 ============================================================================
 */

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <nlpcommon/corpuslexer.h>
#include <nlpcommon/corpuswriter.h>
#include <nlpcommon/spejdtagsetloader.h>

using namespace std;
using namespace NLPCommon;

void lexing_progress(int token) {
    cerr << "\rLexing...  " << token;
}

void writing_progress(int token) {
    cerr << "\rWriting...  " << token;
}

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);

    const Tagset* tagset = NULL;

    ifstream data_stream(argv[1]);
    CorpusLexer<> lexer(data_stream);
    vector<DefaultLexeme> text;
    lexer.setProgressHandler(lexing_progress, 1000);
    lexer.parseStreamToVector(text, &tagset);
    cerr << "\rLexing ...  done.       " << endl;

    cerr << "Loaded " << text.size() << " lexemes." << endl;
    cerr << "Tagset has " << tagset->getCategories().size()
        << " categories and " << tagset->getPartsOfSpeech().size() << " parts of speech."
        << endl;

    ofstream out_stream(argv[2]);
    CorpusWriter<DefaultLexeme> writer(out_stream);
    writer.setProgressHandler(writing_progress, 1000);
    writer.writeVectorToStream(tagset, text);
    cerr << "\rWriting ...  done.       " << endl;
}
