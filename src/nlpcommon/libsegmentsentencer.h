/*
 * libsegmentsentencer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LIBSEGMENTSENTENCER_H_
#define LIBSEGMENTSENTENCER_H_

#include <istream>
#include <vector>

#include <nlpcommon/sentencer.h>
#include <nlpcommon/lexeme.h>
#include <nlpcommon/progress.h>
#include <nlpcommon/exception.h>

#include <nlpcommon/_pstream.h>

#ifndef SEGMENT_EXECUTABLE
#error SEGMENT_EXECUTABLE should be defined automatically by build system
#endif

namespace NLPCommon {

using namespace redi;

template<class Lexeme = DefaultLexeme>
class LibSegmentSentencer : public Sentencer<Lexeme>
{
private:
    pstream ps;

public:
    LibSegmentSentencer(const string& segment_cmdline = SEGMENT_EXECUTABLE) 
        : Sentencer<Lexeme>(),
          ps(segment_cmdline,
             pstreams::pstdin|pstreams::pstdout|pstreams::pstderr)
    {
        if (!ps.is_open()) {
            throw Exception("Cannot execute 'segment_batch' excutable ("
                    SEGMENT_EXECUTABLE ")");
        }
    }

    virtual ~LibSegmentSentencer() { }

    vector<Lexeme> addSentenceDelimiters(const vector<Lexeme>& text) {

        int start_of_para;
        int len = text.size();
        vector<Lexeme> out;

        int i = -1, j;
        for (;;) {
            // Rewrite everything which is out of paragraphs.
            for (i++; i < len; i++) {
                const Lexeme& lex = text[i];
                out.push_back(lex);
                this->advanceProgress();
                if (lex.getType() == Lexeme::START_OF_PARAGRAPH)
                    break;
            }

            if (i == len)
                break;

            // Feed the sentencer with the paragraph.
            bool no_space = true;
            for (j = i; j < len; j++) {
                const Lexeme& lex = text[j];
                if (lex.getType() == Lexeme::END_OF_PARAGRAPH) {
                    break;
                } else if (lex.getType() == Lexeme::NO_SPACE) {
                    no_space = true;
                } else if (lex.getType() == Lexeme::SEGMENT) {
                    if (!no_space)
                        ps << ' ';
                    ps << lex.getUtf8Orth();
                    no_space = false;
                }
            }
            ps << '\n';

            // Read response.
            ps.flush();
            std::string response;
            if (!getline(ps.out(), response)) {
                std::cerr << "RESP: " << response << std::endl;
                throw Exception("No response returned by 'segment_batch'"
                        " executable");
            }

            // Read back and insert sentence delimiters.
            bool first_sentence = true;
            int response_position = 0;
            no_space = true;
            for (j = i + 1; j < len; j++) {
                const Lexeme& lex = text[j];
                if (lex.getType() == Lexeme::END_OF_PARAGRAPH) {
                    break;
                } else if (lex.getType() == Lexeme::NO_SPACE) {
                    out.push_back(lex);
                    no_space = true;
                } else if (lex.getType() == Lexeme::SEGMENT) {
                    if (response[response_position] == '|') {
                        // New sentence starts here.
                        if (first_sentence) {
                            first_sentence = false;
                        } else {
                            out.push_back(Lexeme(Lexeme::END_OF_SENTENCE));
                        }
                        out.push_back(Lexeme(Lexeme::START_OF_SENTENCE));

                        response_position++;
                    }

                    if (!no_space) {
                        if (response[response_position] != ' ') {
                            throw Exception("Expected space not found in output "
                                    "of segment_batch.");
                        }
                        response_position++;
                    }
                    const std::string& utf8_orth = lex.getUtf8Orth();
                    if (response.substr(response_position, utf8_orth.length())
                            != utf8_orth) {
                        throw Exception("Expected word not found in output "
                                "of segment_batch.");
                    }
                    response_position += utf8_orth.length();
                    out.push_back(lex);
                    no_space = false;
                } else {
                    out.push_back(lex);
                }
                this->advanceProgress();
            }

            out.push_back(Lexeme(Lexeme::END_OF_SENTENCE));
            out.push_back(Lexeme(Lexeme::END_OF_PARAGRAPH));

            i = j;
        }

        return out;
    }
};

} // namespace NLPCommon

#endif /* LIBSEGMENTSENTENCER_H_ */
