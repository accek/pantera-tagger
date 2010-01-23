/*
 * category.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <vector>
#include <boost/regex.hpp>
#include <nlpcommon/exception.h>
#include <nlpcommon/ipipanlexer.h>

namespace NLPCommon {

void IpiPanLexer::newToken(Token_Type token_type) {
    out->push_back(Token());
    current_token = &(*out)[out->size() - 1];
    current_token->set_type(token_type);
}

void IpiPanLexer::handleChunkStart(const string& type) {
}

void IpiPanLexer::handleNoSpace() {
}

void IpiPanLexer::handleOrth(const string& orth) {
    newToken(Token::SEGMENT);
    current_token->set_orth(orth);
    advanceProgress();
}

void IpiPanLexer::handleCtag(const string& ctag, bool disamb) {
    if (!current_token)
        throw Exception("Found <ctag> without preceding <tok>/<orth>.");
    Interpretation* interp = current_token->add_interp();
    interp->set_tag(ctag);
    interp->set_chosen_in_golden(disamb);
}

void IpiPanLexer::parseStream(std::istream& stream, std::vector<Token>& tokens)
{
    out = &tokens;
    current_token = NULL;

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
    while (!stream.eof()) {
        std::streamsize leftover = (buf + sizeof(buf)) - next_pos;
        std::streamsize size = next_pos - buf;
        memcpy(buf, next_pos, leftover);
        stream.read(buf + leftover, size);
        std::streamsize read = stream.gcount();
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
                handleCtag(i->str(MATCH_CTAG), (*i)[MATCH_DISAMB].matched);
            }
        }
    }
}

}
