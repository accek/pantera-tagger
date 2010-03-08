/*
 * datwriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef DATWRITER_H_
#define DATWRITER_H_

#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <nlpcommon/writer.h>

namespace NLPCommon {

template<class Lexeme = DefaultLexeme>
class DatWriter : public Writer<Lexeme>
{
private:
public:
    DatWriter(std::ostream& stream) : Writer<Lexeme>(stream) { }

    virtual void writeToStream(WriterDataSource<Lexeme>& data_source) {
        size_t n = data_source.getSize();
        boost::archive::binary_oarchive oa(this->stream);
        oa << n;
        for (size_t i = 0; i < n; i++) {
            oa << data_source.nextLexeme();
            this->advanceProgress();
        }
    }
};

} // namespace NLPCommon

#endif /* DATWRITER_H_ */
