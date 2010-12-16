#ifndef PANTERA_H
#define PANTERA_H

#include <nlpcommon/scorer.h>

#include "brilllexeme.h"
#include "brillengine.h"

typedef BTagger::BrillLexeme<NLPCommon::Tag> MyLexeme;
typedef NLPCommon::BestScoreMultiGoldenScorer<
    NLPCommon::BinaryScorer<MyLexeme::tag_type> > MyScorer;

#endif
