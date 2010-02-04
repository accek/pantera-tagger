/*
 * writer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef WRITER_H_
#define WRITER_H_

#include <istream>
#include <vector>

#include <nlpcommon/lexeme.h>
#include <nlpcommon/progress.h>
#include <nlpcommon/corpus.pb.h>

namespace NLPCommon {

template<class Lexeme = DefaultLexeme>
class WriterDataSource
{
public:
    virtual const Lexeme& nextLexeme() = 0;
    virtual size_t getSize() = 0;
    virtual bool eof() = 0;
    virtual const Tagset* getTagset()  = 0;
};

template<class Lexeme = DefaultLexeme>
class VectorDataSource : public WriterDataSource<Lexeme>
{
private:
    const Tagset* tagset;
    size_t size;
    typename vector<Lexeme>::const_iterator iterator;
    typename vector<Lexeme>::const_iterator end_iterator;

public:
    VectorDataSource(const Tagset* tagset, vector<Lexeme>& vec)
        : tagset(tagset), size(vec.size()), iterator(vec.begin()),
          end_iterator(vec.end()) { }

    size_t getSize() {
        return size;
    }

    bool eof() {
        return iterator == end_iterator;
    }

    const Lexeme& nextLexeme() {
        return *(iterator++);
    }

    const Tagset* getTagset() {
        return tagset;
    }
};

template<class Lexeme = DefaultLexeme>
class Writer : public HasProgress
{
protected:
    std::ostream& stream;

public:
    Writer(std::ostream& stream) : stream(stream) { }

    virtual void writeToStream(WriterDataSource<Lexeme>& data_source) = 0;

    void writeVectorToStream(const Tagset* tagset, std::vector<Lexeme>& text)
    {
        VectorDataSource<Lexeme> data_source(tagset, text);
        writeToStream(data_source);
    }
};


} // namespace NLPCommon

#endif /* WRITER_H_ */
