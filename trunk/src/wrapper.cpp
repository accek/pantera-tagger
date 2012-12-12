/*
 * PanteraWrapper.cpp
 *
 *  Created on: 30-11-2010
 *      Author: mlenart
 *
 *  Modified by: bz
 */

#include "concrete_wrapper.h"
#include <taggingoptions.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/mpi.hpp>

#ifndef DEFAULT_LIB_TAGSET
#define DEFAULT_LIB_TAGSET "nkjp"
#endif

#ifndef DEFAULT_ENGINE
#define DEFAULT_ENGINE "ultimarum-tertia-np0-6.btengine"
//#define DEFAULT_ENGINE "nkjp-wypluwka20091216-t6-np4.btengine"
#endif

// XXX bo jest jako extern w nlpcommon/util.h
boost::program_options::variables_map options;

// XXX bo nie chcemy, by sie wywalało przez MPI
boost::mpi::environment env;
boost::mpi::communicator world;

namespace Pantera {

using namespace std;
using namespace NLPCommon;

typedef BTagger::BrillLexeme<Tag> MyLexeme;
typedef BestScoreMultiGoldenScorer<BinaryScorer<MyLexeme::tag_type> > MyScorer;

static boost::scoped_ptr<Lexer<MyLexeme> > lexer;

static void load_engine_from_archive(
        BTagger::BrillEngine<MyLexeme, MyScorer>& engine, string enginePath) {
    fs::path engine_path = find_with_path(ENGINES_PATH, enginePath);
    fs::ifstream data_stream(engine_path, ios::in);
    boost::archive::text_iarchive engine_archive(data_stream);
    engine_archive >> engine;
}

static Lexer<MyLexeme>* make_lexer(istream& stream) {
    return new PlainTextLexer<MyLexeme>(stream);
}

static vector<MyLexeme> tokenize(const string& text, const Tagset* tagset) {
    vector<MyLexeme> lexems;

    istringstream stream(text);
    lexer.reset(make_lexer(stream));
    lexer->setQuiet(true);
    lexer->parseStreamToVector(lexems, &tagset);

    return lexems;
}

static void writeToFile(const string& fname, const string& data) {
    ofstream out(fname.c_str());
    out.exceptions(ofstream::badbit);
    out << data << endl;
}

static void splitIntoSents(vector<MyLexeme>& lexems, const string& rules) {
    static LibSegmentSentencer<MyLexeme> sentencer;
    string tmpNameString = "";
    char tmpName[L_tmpnam];
    if (!rules.empty()) {
        tmpnam(tmpName);
        tmpNameString = tmpName;
        writeToFile(tmpNameString, rules);
    }
    try {
        lexems = sentencer.addSentenceDelimiters(lexems, tmpNameString);
        if (!rules.empty())
            remove(tmpName);
    } catch (...) {
        if (!rules.empty())
            remove(tmpName);
    }
}

void ConcretePanteraWrapper::morphAnalyze(vector<MyLexeme>& lexems,
        const string& morphDict, const bool useGuesser) {
    morfeusz.clearMorphDict();
    if (!morphDict.empty()) {
        istringstream stream(morphDict);
        morfeusz.loadMorphDict(stream);
    }
    lexems = morfeusz.analyzeText(lexems, useGuesser);
}

void ConcretePanteraWrapper::segmentDisamb(vector<MyLexeme>& lexems) {
    segm_disamb.disambiguateSegmentation(lexems);
}

static void debug(vector<MyLexeme>& lexems, const Tagset* tagset) {
    stringstream out;
    IpiPanWriter<MyLexeme> writer(out);
    writer.writeVectorToStream(tagset, lexems);
    cerr << out.str() << endl;
}

PanteraWrapper::PanteraWrapper(string tagsetName) {
    tagsets = load_tagsets(tagsetName);
    tagset = const_cast<Tagset*>(tagsets[tagsets.size() - 1]);
}

ConcretePanteraWrapper::ConcretePanteraWrapper(string engineName,
        string tagsetName) :
        PanteraWrapper(tagsetName), morfeusz(tagset) {

    morfeusz.setQuiet(true);

    add_phases_to_engine(engine, tagsets, rule_generators);
    load_engine_from_archive(engine, engineName);
    engine.setQuiet(true);

}

ConcretePanteraWrapper::~ConcretePanteraWrapper() {
}

PanteraWrapper* PanteraWrapper::getInstance() {
    static ConcretePanteraWrapper pantera(DEFAULT_ENGINE, DEFAULT_LIB_TAGSET);
    return &pantera;
}

PanteraWrapper* PanteraWrapper::createInstance(string enginePath,
        string tagsetName) {
    return new ConcretePanteraWrapper(
            enginePath.empty() ? DEFAULT_ENGINE : enginePath,
            tagsetName.empty() ? DEFAULT_LIB_TAGSET : tagsetName);
}

void ConcretePanteraWrapper::tag(const string& text,
        const TaggingOptions& options,
        vector<DefaultLexeme>& result) {
    result.clear();
    vector<MyLexeme> lexems = tokenize(text, tagset);
    doTag(lexems, options);
    result.insert(result.begin(), lexems.begin(), lexems.end());
}

void ConcretePanteraWrapper::tag(
        std::vector<DefaultLexeme> &lexems,
        const TaggingOptions& options) {

    vector<MyLexeme> myLexems(lexems.begin(), lexems.end());

    //  debug(lexems, tagset);
    this->doTag(myLexems, options);
    //~ cerr << "done tagging" << endl; // XXX
    lexems.clear();
    lexems.insert(lexems.begin(), lexems.begin(), lexems.end());
}

void ConcretePanteraWrapper::doTag(
        vector<MyLexeme>& lexems,
        const TaggingOptions& options) {
    if (options.doSentSplit())
        splitIntoSents(lexems, options.sentencerRules());

    if (options.doMorphAnalysis())
        morphAnalyze(lexems, options.morphDict(), options.useGuesser());

    if (options.doSegmentDisamb())
        this->segmentDisamb(lexems);

    if (options.doTagging()) {
        LexemesFilter<MyLexeme> segments_filter(MyLexeme::SEGMENT);
        if (options.doSegmentDisamb()) {
            lexems = segments_filter.filterText(lexems);
        }
        engine.tagText(lexems);
        if (options.doSegmentDisamb()) {
            lexems = segments_filter.unfilterText(lexems);
        }
    }
}

Tagset* PanteraWrapper::getTagset() {
    return tagset;
}

// DEPRECATED
std::vector<NLPCommon::DefaultLexeme> ConcretePanteraWrapper::tag(
        const std::vector<NLPCommon::DefaultLexeme>& lexems, 
        bool doSentSplit, bool doMorphAnalysis, 
        bool doSegmentDisamb, bool doTagging) {
    TaggingOptions opts;
    opts.doSentSplit(doSentSplit)
        .doMorphAnalysis(doMorphAnalysis)
        .doSegmentDisamb(doSegmentDisamb)
        .doTagging(doTagging);
    std::vector<NLPCommon::DefaultLexeme> result(lexems.begin(), lexems.end());
    tag(result, opts);
    return result;
}

// DEPRECATED
std::vector<NLPCommon::DefaultLexeme> ConcretePanteraWrapper::tag(const std::string& text) {
    TaggingOptions opts;
    opts.doSentSplit(true)
        .doMorphAnalysis(true)
        .doSegmentDisamb(true)
        .doTagging(true);
    std::vector<NLPCommon::DefaultLexeme> result;
    tag(text, opts, result);
    return result;
}
} // namespace Pantera

