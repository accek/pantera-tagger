/*
 * lexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <istream>
#include <vector>

#include <nlpcommon/corpus.pb.h>
#include <nlpcommon/progress.h>

namespace NLPCommon {

class Lexer : public HasProgress
{
public:
    virtual void parseStream(std::istream& stream, std::vector<Token>& tokens)
        = 0;
};


} // namespace NLPCommon

#endif /* LEXER_H_ */
