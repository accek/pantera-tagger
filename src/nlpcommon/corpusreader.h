/*
 * corpuswriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef CORPUSREADER_H_
#define CORPUSREADER_H_

#include <nlpcommon/lexer.h>

namespace NLPCommon {

class CorpusReader : public Lexer
{
public:
    virtual void parseStream(std::istream& stream, std::vector<Token>& tokens);
};

} // namespace NLPCommon

#endif /* CORPUSREADER_H_ */
