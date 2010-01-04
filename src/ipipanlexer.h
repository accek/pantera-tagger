/*
 * ipipanlexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef IPIPANLEXER_H_
#define IPIPANLEXER_H_

#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <iostream>
#include "lexer.h"
#include "exception.h"

namespace BTagger {

template<class Lexeme>
class IpiPanLexer : Lexer<Lexeme>
{
private:
	const Tagset* tagset;
	vector<Lexeme>* out;
	Lexeme* current_lexeme;

	int token;
	void (*progress_handler)(int);
	int progress_every;

	void handleOrth(const string& orth) {
		out->push_back(Lexeme());
		current_lexeme = &(*out)[out->size() - 1];
		current_lexeme->setOrth(orth);

		token++;
		if (progress_handler && progress_every &&
				(token % progress_every) == 0)
			progress_handler(token);
	}

	void handleCtag(const string& ctag, bool disamb) {
		if (!current_lexeme)
			throw Exception("Found <ctag> without preceding <tok>/<orth>.");
		typename Lexeme::tag_type tag =
				Lexeme::tag_type::parseString(tagset, ctag);
		current_lexeme->addAllowedTag(tag);
		if (disamb)
			current_lexeme->addCorrectTag(tag);
	}

public:
	void setProgressHandler(void (*progress)(int token), int every) {
		progress_handler = progress;
		progress_every = every;
	}

	virtual void parseStream(const Tagset* tagset, std::istream& stream,
			vector<Lexeme>& lexemes) {
		out = &lexemes;
		this->tagset = tagset;
		current_lexeme = NULL;
		token = 0;

		boost::regex parsing_regex = boost::regex(
				"(?:<tok>\\s*<orth>\\s*(.*?)\\s*</orth>)|"
				"(?:<lex\\>([^>]*\\<disamb=[\"']?1[\"']?)?[^>]*>\\s*"
				"<base>\\s*(?:.*?)\\s*</base>\\s*<ctag>\\s*(.*?)\\s*</ctag>"
				"\\s*</lex>)");

		enum {
			MATCH_ORTH = 1,
			MATCH_DISAMB = 2,
			MATCH_CTAG = 3
		};

		// This code is heavily based on example from Boost.Regex
		// (http://www.boost.org/doc/libs/1_41_0/libs/regex/doc/html/boost_regex/partial_matches.html)

	    char buf[4096];
		const char* next_pos = buf + sizeof(buf);
		bool have_more = true;
		while (have_more) {
			unsigned leftover = (buf + sizeof(buf)) - next_pos;
			unsigned size = next_pos - buf;
			memcpy(buf, next_pos, leftover);
			unsigned read = stream.readsome(buf + leftover, size);
			have_more = read == size;
			next_pos = buf + sizeof(buf);

			boost::cregex_iterator i(buf, buf + read + leftover, parsing_regex,
					boost::match_default | boost::match_partial |
					boost::match_extra);
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
					handleCtag(i->str(MATCH_CTAG), (*i)[MATCH_DISAMB].matched);
				}
			}
		}
	}
};
} // namespace BTagger

#endif /* IPIPANLEXER_H_ */
