/*
 * lexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <istream>
#include "lexeme.h"
#include "tagset.h"

namespace BTagger {

template<class Lexeme>
class Lexer
{
public:
	virtual void parseStream(const Tagset* tagset, std::istream& stream,
			vector<Lexeme>& lexemes) = 0;
};


} // namespace BTagger

#endif /* LEXER_H_ */
