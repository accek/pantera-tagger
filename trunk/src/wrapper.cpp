/*
 * PanteraWrapper.cpp
 *
 *  Created on: 30-11-2010
 *      Author: lennyn
 *
 *  Modified by: bz
 */

#include "concrete_wrapper.h"

#include <iostream>
#include <fstream>

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
	return new PlainTextLexer<MyLexeme > (stream);
}

static vector<MyLexeme> tokenize(const string& text, const Tagset* tagset) {
	vector<MyLexeme> lexems;

	istringstream stream(text);
	lexer.reset(make_lexer(stream));
	lexer->setQuiet(true);
	lexer->parseStreamToVector(lexems, &tagset);

	return lexems;
}

static void splitIntoSents(vector<MyLexeme>& lexems) {
	static LibSegmentSentencer<MyLexeme> sentencer;
	lexems = sentencer.addSentenceDelimiters(lexems);
}

void ConcretePanteraWrapper::morphAnalyze(vector<MyLexeme>& lexems) {
	lexems = morfeusz.analyzeText(lexems);
}

void ConcretePanteraWrapper::segmentDisamb(vector<MyLexeme>& lexems){
	segm_disamb.disambiguateSegmentation(lexems);
}

static void debug(vector<MyLexeme>& lexems, const Tagset* tagset) {
	stringstream out;
	IpiPanWriter<MyLexeme> writer(out);
	writer.writeVectorToStream(tagset, lexems);
	cerr << out.str() << endl;
}

PanteraWrapper::PanteraWrapper(string tagsetName){
  tagsets = load_tagsets(tagsetName);
	tagset = const_cast<Tagset*> (tagsets[tagsets.size() - 1]);
}

ConcretePanteraWrapper::ConcretePanteraWrapper(string engineName, string tagsetName) :
  PanteraWrapper(tagsetName), morfeusz(tagset, true) {

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

PanteraWrapper* PanteraWrapper::createInstance(string enginePath, string tagsetName) {
  return new ConcretePanteraWrapper(enginePath.empty() ? DEFAULT_ENGINE : enginePath,
      tagsetName.empty() ? DEFAULT_LIB_TAGSET : tagsetName);
}

vector<DefaultLexeme> ConcretePanteraWrapper::tag(const string& text) {

	vector < MyLexeme > lexems = tokenize(text, tagset);

//	debug(lexems, tagset);

	//cerr << "tokenized" << endl;
	splitIntoSents(lexems);
	//cerr << "split" << endl;
	morphAnalyze(lexems);

  segmentDisamb(lexems);
	//cerr << "morphed" << endl;

	LexemesFilter < MyLexeme > segments_filter(MyLexeme::SEGMENT);
	lexems = segments_filter.filterText(lexems);
	//cerr << "filtered" << endl;
	engine.tagText(lexems);
	//cerr << "tagged" << endl;
	lexems = segments_filter.unfilterText(lexems);
	//cerr << "unfiltered" << endl;
//	debug(lexems, tagset);

	vector<DefaultLexeme> res(lexems.begin(), lexems.end());
	return res;
}

vector<DefaultLexeme> ConcretePanteraWrapper::tag(
    const std::vector<DefaultLexeme> &lexems_,
    bool doSentSplit, bool doMorphAnalysis, bool doSegmentDisamb, bool doTagging){

  vector<MyLexeme> lexems(lexems_.begin(), lexems_.end());

//	debug(lexems, tagset);

  if (doSentSplit)
    splitIntoSents(lexems);

  if (doMorphAnalysis)
    morphAnalyze(lexems);

  if (doSegmentDisamb)
    segmentDisamb(lexems);

  if (!doTagging)
    return vector<DefaultLexeme>(lexems.begin(), lexems.end());

	LexemesFilter < MyLexeme > segments_filter(MyLexeme::SEGMENT);
	lexems = segments_filter.filterText(lexems);
	engine.tagText(lexems);
	lexems = segments_filter.unfilterText(lexems);

//	debug(lexems, tagset);

	return vector<DefaultLexeme>(lexems.begin(), lexems.end());
}

Tagset* PanteraWrapper::getTagset() {
	return tagset;
}

} // namespace Pantera

