/*
 * sentencer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef SENTENCER_H_
#define SENTENCER_H_

#include <istream>
#include <vector>

#include <nlpcommon/lexeme.h>
#include <nlpcommon/progress.h>

namespace NLPCommon {

template<class Lexeme = DefaultLexeme>
class Sentencer : public HasProgress
{
public:
    Sentencer() { }
    virtual ~Sentencer() { }

    virtual vector<Lexeme> addSentenceDelimiters(const vector<Lexeme>& text) = 0;
};

} // namespace NLPCommon

#endif /* SENTENCER_H_ */
