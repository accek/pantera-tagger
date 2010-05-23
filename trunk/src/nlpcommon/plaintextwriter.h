/*
 * corpuswriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef PLAINTEXTWRITER_H_
#define PLAINTEXTWRITER_H_

#include <vector>
#include <utility>
#include <string>
#include <boost/foreach.hpp>

#include <nlpcommon/writer.h>
#include <nlpcommon/util.h>

namespace NLPCommon {

using std::wstring;

template<class Lexeme = DefaultLexeme>
class PlainTextWriter : public Writer<Lexeme>
{
private:
    bool no_space;
    int current_pos;
    int wrap_at;

	void writeLexeme(const Lexeme& lex)
    {
        switch (lex.getType()) {
            case Lexeme::SEGMENT:
                if (!no_space) {
                    this->stream << ' ';
                    current_pos++;
                }

                {
                    int len = lex.getOrth().length();
                    if (wrap_at != -1 && !no_space && current_pos + len > wrap_at) {
                        this->stream << std::endl;
                        current_pos = 0;
                    }
                    this->stream << lex.getUtf8Orth();
                    current_pos += len;
                    no_space = false;
                }

                break;

            case Lexeme::NO_SPACE:
                no_space = true;
                break;

            case Lexeme::START_OF_CHUNK:
                if (lex.getUtf8Orth() == "p") {
                    // Start of paragraph.
                    this->stream << std::endl << std::endl;
                    no_space = true;
                    current_pos = 0;
                }
                break;
        }
    }

public:
    PlainTextWriter(std::ostream& stream, int wrap_at = -1)
        : Writer<Lexeme>(stream), wrap_at(wrap_at) { }

    virtual void writeToStream(WriterDataSource<Lexeme>& data_source) {
        no_space = true;
        current_pos = 0;
        while (!data_source.eof()) {
            writeLexeme(data_source.nextLexeme());
            this->advanceProgress();
        }
        this->stream << std::endl;
    }
};

} // namespace NLPCommon

#endif /* PLAINTEXTWRITER_H_ */
