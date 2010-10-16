/*
 * ipipanlexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef IPIPANLEXER_H_
#define IPIPANLEXER_H_

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
class IpiPanLexer : public Lexer<Lexeme>
{
private:
    LexerCollector<Lexeme>* collector;
    Lexeme current_lex;
	bool consider_disamb_sh;
	std::stack<typename Lexeme::Type> chunks_to_close;

    inline void newLexeme(typename Lexeme::Type lexeme_type) {
        current_lex = Lexeme(lexeme_type);
    }

    void handleChunkStart(const string& type) {
        typename Lexeme::Type openType, closeType;
        if (type == "p") {
            openType = Lexeme::START_OF_PARAGRAPH;
            closeType = Lexeme::END_OF_PARAGRAPH;
        } else if (type == "s") {
            openType = Lexeme::START_OF_SENTENCE;
            closeType = Lexeme::END_OF_SENTENCE;
        } else {
            // Ignore unknown chunks. Better to ignore than to report error probably.
            return;
        }

		Lexeme lex(openType);
        collector->collectLexeme(lex);
		chunks_to_close.push(closeType);
    }

	void handleChunkEnd() {
        if (chunks_to_close.empty()) {
            if (!this->quiet) {
                std::cerr << "IpiPanLexer: Ignoring incorrect chunk nesting. "
                    "Skipping unexpected end of chunk." << std::endl;
            }
        } else {
            Lexeme lex(chunks_to_close.top());
            collector->collectLexeme(lex);
            chunks_to_close.pop();
        }
	}

    void handleNoSpace() {
		collector->collectLexeme(Lexeme(Lexeme::NO_SPACE));
    }

    void handleOrth(const string& orth) {
        newLexeme(Lexeme::SEGMENT);
        current_lex.setOrth(xml_to_wstring(orth));
    }

    void handleCtag(const string& base, const string& ctag,
			bool disamb, bool disamb_sh) {
        typename Lexeme::tag_type tag =
                Lexeme::tag_type::parseString(collector->getTagset(), ctag);
		current_lex.addTagBase(tag, xml_to_wstring(base));
        current_lex.addAllowedTag(tag);
        if (disamb)
            current_lex.addGoldenTag(tag);
		if (consider_disamb_sh && !disamb_sh)
			current_lex.addAutoselectedTag(tag);
    }

    void handleEndOfTok() {
        collector->collectLexeme(current_lex);
        this->advanceProgress();
    }

public:
    IpiPanLexer(std::istream& stream, bool consider_disamb_sh = false)
            : Lexer<Lexeme>(stream), consider_disamb_sh(consider_disamb_sh) { }

    virtual void parseStream(LexerCollector<Lexeme>& collector)
    {
        this->collector = &collector;

        boost::regex parsing_regex = boost::regex(
				"(<ns\\>)|"
				"(?:<chunk[^>]*\\<type=[\"']?([a-zA-Z]*)[\"']?)|"
				"(</chunk\\>)|"
                "(?:<orth>\\s*(.*?)\\s*</orth>)|"
                "(?:<lex\\>([^>]*\\<disamb=[\"']?1[\"']?)?([^>]*\\<disamb_sh=[\"']?0[\"']?)?[^>]*>\\s*"
                "<base>\\s*(.*?)\\s*</base>\\s*<ctag>\\s*(.*?)\\s*</ctag>"
                "\\s*</lex>)|"
                "(</tok>)");

        enum {
			MATCH_NS = 1,
			MATCH_CHUNK_START = 2,
			MATCH_CHUNK_END = 3,
            MATCH_ORTH = 4,
            MATCH_DISAMB = 5,
            MATCH_DISAMB_SH = 6,
			MATCH_BASE = 7,
            MATCH_CTAG = 8,
            MATCH_ETOK = 9
        };

        // This code is heavily based on example from Boost.Regex
        // (http://www.boost.org/doc/libs/1_41_0/libs/regex/doc/html/boost_regex/partial_matches.html)

        char buf[4096];
        const char* next_pos = buf + sizeof(buf);
        while (!this->stream.eof()) {
            std::streamsize leftover = (buf + sizeof(buf)) - next_pos;
            std::streamsize size = next_pos - buf;
            memcpy(buf, next_pos, leftover);
            this->stream.read(buf + leftover, size);
            std::streamsize read = this->stream.gcount();
            next_pos = buf + sizeof(buf);

            boost::cregex_iterator i(buf, buf + read + leftover, parsing_regex,
                    boost::match_default | boost::match_partial);
            boost::cregex_iterator end;

            for (; i != end; ++i) {
                if ((*i)[0].matched == false) {
                    // Partial match, save position and break:
                    next_pos = (*i)[0].first;
                    break;
                }

                if ((*i)[MATCH_ORTH].matched) {
                    handleOrth(i->str(MATCH_ORTH));
                } else if ((*i)[MATCH_CTAG].matched) {
					handleCtag(i->str(MATCH_BASE),
							i->str(MATCH_CTAG),
							(*i)[MATCH_DISAMB].matched,
							(*i)[MATCH_DISAMB_SH].matched);
                } else if ((*i)[MATCH_ETOK].matched) {
                    handleEndOfTok();
                } else if ((*i)[MATCH_NS].matched) {
					handleNoSpace();
				} else if ((*i)[MATCH_CHUNK_START].matched) {
					handleChunkStart(i->str(MATCH_CHUNK_START));
				} else if ((*i)[MATCH_CHUNK_END].matched) {
					handleChunkEnd();
				}
            }
        }

		// Finally close all remaining chunks.
		while (!chunks_to_close.empty())
			handleChunkEnd();
    }
};

} // namespace NLPCommon

#endif /* IPIPANLEXER_H_ */
