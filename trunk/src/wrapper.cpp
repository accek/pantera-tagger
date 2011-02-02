/*
 * PanteraWrapper.cpp
 *
 *  Created on: 30-11-2010
 *      Author: lennyn
 */

#include "wrapper.h"

#include <iostream>
#include <fstream>

#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/mpi.hpp>

#include <nlpcommon/lexer.h>
#include <nlpcommon/plaintextlexer.h>
#include <nlpcommon/lexeme.h>
#include <nlpcommon/lexemesfilter.h>
#include <nlpcommon/spejdtagsetloader.h>
#include <nlpcommon/libsegmentsentencer.h>
#include <nlpcommon/morfeusz.h>
#include <nlpcommon/tagset.h>
#include <nlpcommon/polish_segm_disamb.h>
#include <nlpcommon/util.h>
#include <nlpcommon/ipipanwriter.h>

#include "pantera.h"
#include "pantera_rules.h"

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
		BTagger::BrillEngine<MyLexeme, MyScorer>& engine) {
	fs::path engine_path = find_with_path(ENGINES_PATH, DEFAULT_ENGINE);
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

static void morphAnalyze(vector<MyLexeme>& lexems, const Tagset* tagset) {
	static MorfeuszAnalyzer<MyLexeme> morfeusz(tagset, true);
	static PolishSegmDisambiguator<MyLexeme> segm_disamb;
	static bool first_time = true;

	if (first_time) {
		first_time = false;

		morfeusz.setQuiet(true);
	}
	lexems = morfeusz.analyzeText(lexems);

	segm_disamb.disambiguateSegmentation(lexems);
}

static void debug(vector<MyLexeme>& lexems, const Tagset* tagset) {
	stringstream out;
	IpiPanWriter<MyLexeme> writer(out);
	writer.writeVectorToStream(tagset, lexems);
	cerr << out.str() << endl;
}

PanteraWrapper::PanteraWrapper() {
	tagsets = load_tagsets(DEFAULT_LIB_TAGSET);
	tagset = const_cast<Tagset*> (tagsets[tagsets.size() - 1]);
}

PanteraWrapper::~PanteraWrapper() {

}

PanteraWrapper* PanteraWrapper::getInstance() {
    static PanteraWrapper pantera;
    return &pantera;
}

vector<DefaultLexeme> PanteraWrapper::tag(const string& text) {

	static bool initialized = false;
	static BTagger::BrillEngine<MyLexeme, MyScorer> engine;
	static std::vector<BTagger::RulesGenerator<MyLexeme>*> rule_generators;

	if (!initialized) {
		add_phases_to_engine(engine, tagsets, rule_generators);
		load_engine_from_archive(engine);
		engine.setQuiet(true);

		initialized = true;
	}

	vector < MyLexeme > lexems = tokenize(text, tagset);

	//cerr << "tokenized" << endl;
	splitIntoSents(lexems);
	//cerr << "split" << endl;
	morphAnalyze(lexems, tagset);
	//cerr << "morphed" << endl;

	LexemesFilter < MyLexeme > segments_filter(MyLexeme::SEGMENT);
	lexems = segments_filter.filterText(lexems);
	//cerr << "filtered" << endl;
	engine.tagText(lexems);
	//cerr << "tagged" << endl;
	lexems = segments_filter.unfilterText(lexems);
	//cerr << "unfiltered" << endl;
	//debug(lexems, tagset);

	vector<DefaultLexeme> res(lexems.begin(), lexems.end());
	return res;
}

Tagset* PanteraWrapper::getTagset() {
	return tagset;
}

} // namespace Pantera
