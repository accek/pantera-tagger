/*
 * impl.h
 *
 *  Created on: Jan 3, 2010
 *      Author: accek
 */

#ifndef RULES_IMPL_H_
#define RULES_IMPL_H_

#include <boost/format.hpp>
#include <vector>
#include <cstdlib>
#include <cassert>

#include "../rules.h"

namespace BTagger {
namespace Rules {

using namespace std;
using namespace NLPCommon;

template<class Lexeme>
class AllPredicatesAllActionsGenerator : public RulesGenerator<Lexeme> {
protected:
    vector<PredicateTemplate<Lexeme>*> ptemplates;
    vector<ActionTemplate<Lexeme>*> atemplates;

public:
    AllPredicatesAllActionsGenerator(
            const vector<PredicateTemplate<Lexeme>*>& ptemplates,
            const vector<ActionTemplate<Lexeme>*>& atemplates)
        : ptemplates(ptemplates), atemplates(atemplates) { }

    virtual void generateRules(vector<Lexeme>& text, int index,
            vector<Rule<Lexeme> >& rules)
    {
        vector<Predicate<Lexeme> > predicates;
        BOOST_FOREACH(PredicateTemplate<Lexeme>* ptpl, ptemplates) {
            ptpl->findMatchingPredicates(predicates, text, index);
        }
        BOOST_FOREACH(const Predicate<Lexeme>& p, predicates) {
            BOOST_FOREACH(ActionTemplate<Lexeme>* atpl, atemplates) {
                atpl->findMatchingRules(p, rules, text, index);
            }
        }
    }
};

#include "a.h"

#define STR_SIZE 250

#define T(a) (p.params.a.asString(p.tpl->tagsets[Phase]).c_str())

#include "p1.h"
#include "c1.m4h"

#undef T
#define T(a) (p.params.a.asString(p.tpl->tagsets[Phase - 1]).c_str())
#define S(a) (p.params.a.asString(p.tpl->tagsets[Phase]).c_str())

#include "p2.h"

#undef T
#undef S
#undef STR_SIZE

#include "make.h"

}} // namespace BTagger::Rules

#endif /* RULES_IMPL_H_ */
