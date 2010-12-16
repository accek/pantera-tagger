#ifndef PANTERA_RULES_H
#define PANTERA_RULES_H

#include <vector>
#include "pantera.h"

void add_phases_to_engine(BTagger::BrillEngine<MyLexeme, MyScorer>& engine,
        const std::vector<const NLPCommon::Tagset*>& tagsets,
        std::vector<BTagger::RulesGenerator<MyLexeme>*>& rule_generators);

#endif
