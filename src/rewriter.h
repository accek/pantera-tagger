/*
 * rewriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef REWRITER_H_
#define REWRITER_H_

#include <istream>
#include "lexeme.h"
#include "tagset.h"

namespace BTagger {

// Class responsible for rewriting data files with added disambiguation
// information.
template<class Lexeme>
class Rewriter
{
public:
    virtual void rewriteStream(int phase, const Tagset* tagset,
            const vector<Lexeme>& lexemes,
            std::istream& in, std::ostream& out) = 0;
};


} // namespace BTagger

#endif /* REWRITER_H_ */
