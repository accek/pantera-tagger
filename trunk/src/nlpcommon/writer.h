/*
 * lexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef WRITER_H_
#define WRITER_H_

#include <istream>
#include <vector>

#include <nlpcommon/progress.h>
#include <nlpcommon/corpus.pb.h>

namespace NLPCommon {

class Writer : public HasProgress
{
public:
    virtual void writeToStream(std::ostream& stream, std::vector<Token>& tokens)
        = 0;
};


} // namespace NLPCommon

#endif /* WRITER_H_ */
