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

#include <nlpcommon/corpusreader.h>
#include <nlpcommon/corpuswriter.h>

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

    CorpusReader lexer;
    vector<Token> text;
    ifstream data_stream(argv[1]);
    lexer.setProgressHandler(lexing_progress, 1000);
    lexer.parseStream(data_stream, text);
    cerr << "\rLexing ...  done.       " << endl;


    CorpusWriter writer;
    ofstream out_stream(argv[2]);
    writer.setProgressHandler(writing_progress, 1000);
    writer.writeToStream(out_stream, text);
    cerr << "\rWriting ...  done.       " << endl;
}
