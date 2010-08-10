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
#include <boost/program_options.hpp>

#include <nlpcommon/nkjpwriter.h>
#include <nlpcommon/nkjptextlexer.h>
#include <nlpcommon/spejdtagsetloader.h>
#include <nlpcommon/morfeusz.h>
#include <nlpcommon/libsegmentsentencer.h>
#include <nlpcommon/tagset.h>

#ifndef DEFAULT_TAGSET
#define DEFAULT_TAGSET "ipipan"
#endif

using namespace std;
using namespace NLPCommon;

po::variables_map options;

void lexing_progress(int token) {
    cerr << "\rLexing...  " << token;
}

void writing_progress(int token) {
    cerr << "\rWriting...  " << token;
}

void parse_command_line(int argc, char** argv) {
    // Declare a group of options that will be 
    // allowed only on command line
    po::options_description generic("Generic options");
    generic.add_options()
        ("version,V", "print version string")
        ("help", "produce help message")
        ("config,c", po::value<string>(), "name of a configuration file")
        ("tagset,t", po::value<string>()->default_value("ipipan"),
         "name of tagset to use (or a Spejd-compatible tagset config file, "
         "or a multitier tagset folder containing 0.cfg, 1.cfg etc.)")
        ;
    generic.add(tagset_options());

    // Declare a group of options that will be 
    // allowed both on command line and in
    // config file
    po::options_description config("Configuration");
    config.add_options()
        ("verbose,v", "be verbose")
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
}

template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " "));
    return os;
}

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);

    parse_command_line(argc, argv);
    if (!options.count("input-file")) {
        std::cerr << "No input file specified" << std::endl;
        return 1;
    }

    const vector<string>& files = options["input-file"].as< vector<string> >();
    std::cerr << "Files: " << files << std::endl;

    cerr << "Loading tagset ... ";
    const Tagset* tagset = load_tagset(options["tagset"].as<string>());
    cerr << "done." << endl;

    ifstream data_stream(files[0].c_str());
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

    ofstream out_stream(files[1].c_str());
    NKJPSegmWriter<DefaultLexeme> writer(out_stream);
    writer.setProgressHandler(writing_progress, 1000);
    writer.writeVectorToStream(tagset, text);
    cerr << "\rWriting ...  done.       " << endl;

    ofstream out_stream2(files[2].c_str());
    NKJPMorphWriter<DefaultLexeme> writer2(out_stream2);
    writer.setProgressHandler(writing_progress, 1000);
    writer2.writeVectorToStream(tagset, text);
    cerr << "\rWriting ...  done.       " << endl;
}
