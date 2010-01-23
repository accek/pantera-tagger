/*
 * corpuswriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef CORPUSWRITER_H_
#define CORPUSWRITER_H_

#include <nlpcommon/writer.h>

namespace NLPCommon {

class CorpusWriter : public Writer
{
public:
    virtual void writeToStream(std::ostream& stream,
            std::vector<Token>& tokens);
};

} // namespace NLPCommon

#endif /* CORPUSWRITER_H_ */
