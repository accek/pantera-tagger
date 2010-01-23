/*
 * ipipanlexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef IPIPANLEXER_H_
#define IPIPANLEXER_H_

#include <string>
#include <iostream>
#include <nlpcommon/lexer.h>

namespace NLPCommon {

using std::string;

class IpiPanLexer : public Lexer
{
private:
    std::vector<Token>* out;
    Token* current_token;

    inline void newToken(Token_Type token_type);

    void handleChunkStart(const string& type);
    void handleNoSpace();
    void handleOrth(const string& orth);
    void handleCtag(const string& ctag, bool disamb);

public:
    virtual void parseStream(std::istream& stream, std::vector<Token>& tokens);
};

} // namespace NLPCommon

#endif /* IPIPANLEXER_H_ */
