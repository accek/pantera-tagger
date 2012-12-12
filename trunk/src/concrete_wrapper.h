/*
 * ConcretePanteraWrapper.h
 *
 *  Created on: 26-05-2011
 *      Author: bz
 */

#ifndef CONCRETEPANTERAWRAPPER_H_
#define CONCRETEPANTERAWRAPPER_H_

#include <vector>
#include <string>
#include <nlpcommon/tag.h>
#include <nlpcommon/lexeme.h>
#include "wrapper.h"

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

namespace Pantera {

using namespace std;
using namespace NLPCommon;

typedef BTagger::BrillLexeme<Tag> MyLexeme;
typedef BestScoreMultiGoldenScorer<BinaryScorer<MyLexeme::tag_type> > MyScorer;

class ConcretePanteraWrapper : public PanteraWrapper {
public:
    ConcretePanteraWrapper(string enginePath, string tagsetName);
    virtual ~ConcretePanteraWrapper();
    
    virtual void tag(
        std::vector<NLPCommon::DefaultLexeme>& lexems,
        const TaggingOptions& options);
    
    virtual void tag(
        const std::string& text,
        const TaggingOptions& options,
        std::vector<NLPCommon::DefaultLexeme>& result);
    
    // DEPRECATED
    virtual std::vector<NLPCommon::DefaultLexeme> tag(
        const std::vector<NLPCommon::DefaultLexeme>& lexems, 
        bool doSentSplit, bool doMorphAnalysis, bool doSegmentDisamb, bool doTagging);

    // DEPRECATED
    virtual std::vector<NLPCommon::DefaultLexeme> tag(const std::string& text);

private:
  void morphAnalyze(vector<MyLexeme>& lexems, const string& morphDict, const bool useGuesser);
  void segmentDisamb(vector<MyLexeme>& lexems);
  
  void doTag(vector<MyLexeme>& lexems, const TaggingOptions& opts);
  
  MorfeuszAnalyzer<MyLexeme> morfeusz;
  PolishSegmDisambiguator<MyLexeme> segm_disamb;
  BTagger::BrillEngine<MyLexeme, MyScorer> engine;
  std::vector<BTagger::RulesGenerator<MyLexeme>*> rule_generators;

};
}

#endif /* CONCRETEPANTERAWRAPPER_H_ */
