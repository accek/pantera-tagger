/*
 * rules_impl.h
 *
 *  Created on: Jan 3, 2010
 *      Author: accek
 */

#ifndef RULES_IMPL_H_
#define RULES_IMPL_H_

#include <vector>
#include <cstdlib>

#include "rules.h"

namespace BTagger {
namespace Rules {

using namespace std;


#define STR_SIZE 250

#define T(a) (p.params.a.asString(p.tpl->tagsets[Phase]).c_str())

#include "rules_p1.cpp"

#undef T
#define T(a) (p.params.a.asString(p.tpl->tagsets[Phase - 1]).c_str())
#define S(a) (p.params.a.asString(p.tpl->tagsets[Phase]).c_str())

#include "rules_p2.cpp"

#undef T
#undef S
#undef STR_SIZE

}} // namespace BTagger::Rules

#endif /* RULES_IMPL_H_ */
