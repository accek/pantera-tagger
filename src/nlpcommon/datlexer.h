/*
 * datwriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef DATLEXER_H_
#define DATLEXER_H_

#include <vector>
#include <boost/foreach.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <nlpcommon/lexer.h>

namespace NLPCommon {

template<class Lexeme = DefaultLexeme>
class DatLexer : public Lexer<Lexeme>
{
public:
    DatLexer(std::istream& stream) : Lexer<Lexeme>(stream) { }

    virtual void parseStream(LexerCollector<Lexeme>& collector) {
        size_t len;
        Lexeme lex;
        boost::archive::binary_iarchive ia(this->stream);
        ia >> len;

        collector.setSize(len);
        for (size_t i = 0; i < len; i++) {
            ia >> lex;
            collector.collectLexeme(lex);
            this->advanceProgress();
        }
    }
};

} // namespace NLPCommon

#endif /* DATLEXER_H_ */
