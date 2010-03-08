/*
 ============================================================================
 Name        : dat2datcpp
 Author      : Szymon Acedanski
 Version     :
 Copyright   : Copyright (C) Institute of CS, Polish Academy of Sciences
 Description : Hello World in C++,
 ============================================================================
 */

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <nlpcommon/datlexer.h>
#include <nlpcommon/datwriter.h>
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

    SpejdTagsetLoader tagset_loader;
    ifstream config_stream("tagset.cfg");
    const Tagset* tagset = tagset_loader.loadTagset(config_stream);

    ifstream data_stream(argv[1]);
    DatLexer<DefaultLexeme> lexer(data_stream);
    vector<DefaultLexeme> text;
    lexer.setProgressHandler(lexing_progress, 1000);
    lexer.parseStreamToVector(text, &tagset);
    cerr << "\rLexing ...  done.       " << endl;

    ofstream out_stream(argv[2]);
    DatWriter<DefaultLexeme> writer(out_stream);
    writer.setProgressHandler(writing_progress, 1000);
    writer.writeVectorToStream(tagset, text);
    cerr << "\rWriting ...  done.       " << endl;
}
