#include <nlpcommon/exception.h>

#include "rules/impl.h"
#include "pantera_rules.h"

void add_phases_to_engine(BTagger::BrillEngine<MyLexeme, MyScorer>& engine,
        const std::vector<const NLPCommon::Tagset*>& tagsets,
        std::vector<BTagger::RulesGenerator<MyLexeme>*>& rule_generators) {
    if (tagsets.size() > 3) {
        throw NLPCommon::Exception("Tagsets with more than 3 phases are not supported.");
    }

    BTagger::RulesGenerator<MyLexeme>* g1 =
        BTagger::Rules::make_p1_rules_generator<MyLexeme, 0>(tagsets);
    engine.addPhase(tagsets[0], g1->getTStore());
    rule_generators.push_back(g1);

    if (tagsets.size() >= 2) {
        BTagger::RulesGenerator<MyLexeme>* g2 =
            BTagger::Rules::make_p2_rules_generator<MyLexeme, 1>(tagsets);
        engine.addPhase(tagsets[1], g2->getTStore());
        rule_generators.push_back(g2);
    }

    if (tagsets.size() >= 3) {
        BTagger::RulesGenerator<MyLexeme>* g3 =
            BTagger::Rules::make_p2_rules_generator<MyLexeme, 2>(tagsets);
        engine.addPhase(tagsets[2], g3->getTStore());
        rule_generators.push_back(g3);
    }
}

