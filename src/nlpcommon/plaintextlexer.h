/*
 * ipipanlexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef PLAINTEXTLEXER_H_
#define PLAINTEXTLEXER_H_

#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <locale>
#include <stack>

#include <nlpcommon/lexer.h>
#include <nlpcommon/util.h>

namespace NLPCommon {

using std::string;

template<class Lexeme = DefaultLexeme>
class PlainTextLexer : public Lexer<Lexeme>
{
private:
    LexerCollector<Lexeme>* collector;
    bool new_paragraph;

    void handleOrth(const wstring& orth, bool preceded_by_space) {
        if (new_paragraph) {
            Lexeme lex(Lexeme::START_OF_PARAGRAPH);
            collector->collectLexeme(lex);
        }

        if (!preceded_by_space && !new_paragraph)
            collector->collectLexeme(Lexeme(Lexeme::NO_SPACE));
        Lexeme current_lex = Lexeme(Lexeme::SEGMENT);
        current_lex.setOrth(orth);
        collector->collectLexeme(current_lex);
        this->advanceProgress();
        new_paragraph = false;
    }

    void handleNewParagraph() {
        if (!new_paragraph)
            endParagraph();
        new_paragraph = true;
    }

    void endParagraph() {
        Lexeme lex(Lexeme::END_OF_PARAGRAPH);
        collector->collectLexeme(lex);
    }

public:
    PlainTextLexer(std::istream& stream)
            : Lexer<Lexeme>(stream) { }

    virtual void parseStream(LexerCollector<Lexeme>& collector)
    {
        this->collector = &collector;
        this->new_paragraph = true;

        boost::wregex parsing_regex = boost::wregex(
				L"(\\s*?\n\\s*?\\n\\s*)|"  // New paragraph
				L"(\\s)?([^\\s]+)",  // Word may be preceded by space
                boost::regex::perl);

        enum {
			MATCH_NEWPAR = 1,
			MATCH_PRECEDING_SPACE = 2,
			MATCH_ORTH = 3
        };

        // This code is heavily based on example from Boost.Regex
        // (http://www.boost.org/doc/libs/1_41_0/libs/regex/doc/html/boost_regex/partial_matches.html)

        string raw_text_s = string(std::istreambuf_iterator<char>(this->stream),
                std::istreambuf_iterator<char>());
        wstring raw_text = utf8_to_wstring(raw_text_s);
        boost::wsregex_iterator i(raw_text.begin(), raw_text.end(),
                parsing_regex);
        boost::wsregex_iterator end;
        for (; i != end; ++i) {
            if ((*i)[MATCH_NEWPAR].matched) {
                handleNewParagraph();
            } else {
                handleOrth(i->str(MATCH_ORTH), (*i)[MATCH_PRECEDING_SPACE].matched);
            }
        }

        if (!new_paragraph)
            endParagraph();
    }
};

} // namespace NLPCommon

#endif /* PLAINTEXTLEXER_H_ */
