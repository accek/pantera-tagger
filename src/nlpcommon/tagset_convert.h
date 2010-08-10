/*
 * lexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef TAGSET_CONVERT_H_
#define TAGSET_CONVERT_H_

#include <nlpcommon/tagset.h>
#include <nlpcommon/lexeme.h>

namespace NLPCommon {

template<class Tag>
class TagsetConverter
{
protected:
    const Tagset* from_tagset;
    const Tagset* to_tagset;

public:
    TagsetConverter(const Tagset* from_tagset, const Tagset* to_tagset)
        : from_tagset(from_tagset), to_tagset(to_tagset) { }

    virtual Tag convert(const Lexeme<Tag>& lex, const Tag& tag) = 0;
};

} // namespace NLPCommon

#endif /* TAGSET_CONVERT_H_ */
