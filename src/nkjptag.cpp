/*
 ============================================================================
 Name        : dat2dat.cpp
 Author      : Szymon Acedanski
 Version     :
 Copyright   : Copyright (C) Institute of CS, Polish Academy of Sciences
 Description : Hello World in C++,
 ============================================================================
 */

#include <iostream>
#include <fstream>

#include <nlpcommon/nkjpwriter.h>
#include <nlpcommon/nkjptextlexer.h>
#include <nlpcommon/spejdtagsetloader.h>
#include <nlpcommon/morfeusz.h>
#include <nlpcommon/libsegmentsentencer.h>

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
    NKJPTextLexer<DefaultLexeme> lexer(data_stream);
    vector<DefaultLexeme> text;
    lexer.setProgressHandler(lexing_progress, 1000);
    lexer.parseStreamToVector(text, &tagset);
    cerr << "\rLexing ...  done.       " << endl;

    cerr << "Running sentencer ... ";
    LibSegmentSentencer<DefaultLexeme> sentencer;
    text = sentencer.addSentenceDelimiters(text);
    cerr << "done." << endl;

    cerr << "Running morphological analyzer ... ";
    MorfeuszAnalyzer<DefaultLexeme> morfeusz(tagset);
    text = morfeusz.analyzeText(text);
    cerr << "done." << endl;

    cerr << "Assigning segment IDs ... ";
    lexer.alignText(text);
    cerr << "done." << endl;

    ofstream out_stream(argv[2]);
    NKJPSegmWriter<DefaultLexeme> writer(out_stream);
    writer.setProgressHandler(writing_progress, 1000);
    writer.writeVectorToStream(tagset, text);
    cerr << "\rWriting ...  done.       " << endl;

    ofstream out_stream2(argv[3]);
    NKJPMorphWriter<DefaultLexeme> writer2(out_stream2);
    writer.setProgressHandler(writing_progress, 1000);
    writer2.writeVectorToStream(tagset, text);
    cerr << "\rWriting ...  done.       " << endl;
}
