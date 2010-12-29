/*
 * nkjptextlexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef NKJPTEXTLEXER_H_
#define NKJPTEXTLEXER_H_

#include <boost/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include <locale>
#include <stack>
#include <algorithm>

#include <nlpcommon/lexer.h>
#include <nlpcommon/util.h>
#include <nlpcommon/nkjplexerdata.h>
#include <nlpcommon/exception.h>

namespace NLPCommon {

using std::string;

template<class Lexeme = DefaultLexeme>
class NKJPTextLexer : public Lexer<Lexeme>
{
private:
    LexerCollector<Lexeme>* collector;
    bool new_paragraph;
    bool inside_paragraph;
    boost::wregex parsing_regex, text_regex;
    wstring raw_text; // Saved most recently parsed file

    typedef typename Lexeme::tag_type tag_type;

    void handleOrth(const wstring& orth, bool preceded_by_space) {
        if (!inside_paragraph)
            return;
        if (!preceded_by_space && !new_paragraph)
            collector->collectLexeme(Lexeme(Lexeme::NO_SPACE));
        Lexeme current_lex = Lexeme(Lexeme::SEGMENT);
        current_lex.setOrth(orth);
        collector->collectLexeme(current_lex);
        this->advanceProgress();
        new_paragraph = false;
    }

    void handleNewParagraph(const string& id) {
        Lexeme lex(Lexeme::START_OF_PARAGRAPH);
        lex.setLexerData(new NKJPParagraphData(id));
        collector->collectLexeme(lex);
        new_paragraph = true;
        inside_paragraph = true;
    }

    void handleEndOfParagraph() {
        if (!inside_paragraph) {
            throw Exception("NKJP lexer found end-of-para before "
                    "start-of-para.");
        }
        Lexeme lex(Lexeme::END_OF_PARAGRAPH);
        collector->collectLexeme(lex);
        inside_paragraph = false;
    }

#define SPACE_CHARACTERS \
    L"\x00a0\x2000\x2001\x2002\x2003\x2004\x2005" \
    L"\x2006\x2007\x2008\x2009\x200a\x200b\x202f" \
    L"\x205f\x3000\x2028\x2029\xfeff \t\r\n\x0085"

    static bool is_space(wchar_t c) {
        static const wstring space_string(SPACE_CHARACTERS);
        return std::find(space_string.begin(), space_string.end(), c)
            != space_string.end();
    }

    static bool is_not_space(wchar_t c) {
        return !is_space(c);
    }

public:
    NKJPTextLexer(std::istream& stream)
            : Lexer<Lexeme>(stream)
    {
        parsing_regex = boost::wregex(
                L"(?:(?:(<p|<ab|<u)[^>]*?(?:xml:id=[\"']([^\"']*)[\"'])[^/>]*?>)|"
                 "(?<!<desc|.<gap)>)([^<>]*)<(?!p\\>|ab\\>|u\\>)(/p>|/ab>|/u>)?",
                  boost::regex::perl);

        text_regex = boost::wregex(
                L"([" SPACE_CHARACTERS L"])?([^" SPACE_CHARACTERS L"]+)"  // Word may be preceded by space
                );
    }

    enum {
        MATCH_NEWPAR = 1,
        MATCH_NEWPAR_ID = 2,
        MATCH_TEXT = 3,
        MATCH_ENDPAR = 4,

        MATCH_PRECEDING_SPACE = 1,
        MATCH_ORTH = 2
    };

    virtual void parseStream(LexerCollector<Lexeme>& collector)
    {
        this->collector = &collector;
        this->new_paragraph = true;
        this->inside_paragraph = false;

        // This code is heavily based on example from Boost.Regex
        // (http://www.boost.org/doc/libs/1_41_0/libs/regex/doc/html/boost_regex/partial_matches.html)

        string raw_text_s = string(std::istreambuf_iterator<char>(this->stream),
                std::istreambuf_iterator<char>());
        raw_text = utf8_to_wstring(raw_text_s);
        boost::wsregex_iterator i(raw_text.begin(), raw_text.end(),
                parsing_regex);
        boost::wsregex_iterator end;
        for (; i != end; ++i) {
            if ((*i)[MATCH_NEWPAR].matched)
                handleNewParagraph(wstring_to_utf8(i->str(MATCH_NEWPAR_ID)));
            boost::wsregex_iterator j((*i)[MATCH_TEXT].first,
                    (*i)[MATCH_TEXT].second, text_regex);
            for (; j != end; ++j) {
                handleOrth(wxml_to_wstring(j->str(MATCH_ORTH)),
                        (*j)[MATCH_PRECEDING_SPACE].matched);
            }
            if ((*i)[MATCH_ENDPAR].matched)
                handleEndOfParagraph();
        }
    }

    virtual void alignText(vector<Lexeme>& text)
    {
        // This code is heavily based on example from Boost.Regex
        // (http://www.boost.org/doc/libs/1_41_0/libs/regex/doc/html/boost_regex/partial_matches.html)

        boost::wsregex_iterator raw_i(raw_text.begin(), raw_text.end(), parsing_regex);
        boost::wsregex_iterator end;

        size_t block_offset; // Sum of lengths of all blocks
                             // before current in the paragraph.
        size_t block_cursor;
        wstring block;
        string para_id;
        int segment_num = 0;
        int sentence_num = 0;

        bool in_ambiguity = false;
        boost::wsregex_iterator saved_raw_i;
        size_t saved_block_cursor;
        size_t saved_block_offset;


        int text_len = text.size();
        int i;
        for (i = 0; i < text_len; i++) {
            Lexeme& lex = text[i];
            switch (lex.getType()) {
                case Lexeme::START_OF_PARAGRAPH:
                {
                    assert(!in_ambiguity);

                    para_id = dynamic_cast<NKJPParagraphData*>(
                            lex.getLexerData())->getId();

                    while (raw_i != end && (!(*raw_i)[MATCH_NEWPAR].matched
                                || wstring_to_utf8(raw_i->str(MATCH_NEWPAR_ID)) != para_id))
                        ++raw_i;

                    if (raw_i == end) {
                        throw Exception(boost::str(
                                    boost::format("NKJP aligner could not find"
                                       " paragraph with id '%1%'") % para_id));
                    }

                    block = wxml_to_wstring(raw_i->str(MATCH_TEXT));
                    block_cursor = 0;
                    block_offset = 0;
                    sentence_num = 0;
                    break;
                }

                case Lexeme::START_OF_SENTENCE:
                {
                    sentence_num++;
                    string sent_id = boost::str(
                            boost::format("%1%.%2%-s", std::locale::classic())
                            % para_id % sentence_num);
                    lex.setLexerData(new NKJPSentenceData(sent_id));
                    break;
                }

                case Lexeme::SEGMENT:
                {
                    const wstring& needle = lex.getOrth();
                    for (;;) {
                        /*
                        std::cerr << "Looking: " << wstring_to_utf8(needle) << ' ' << needle.length() << std::endl;
                        std::cerr << "Cursor: " << block_cursor << std::endl;
                        std::cerr << "Offset: " << block_offset << std::endl;
                        std::cerr << "Block near start: " << wstring_to_utf8(block.substr(block_cursor, 30)) << std::endl;
                        std::cerr << "Block: " << wstring_to_utf8(block) << std::endl;
                        std::cerr << std::endl;
                        */

                        const wstring::iterator i = std::find_if(block.begin() + block_cursor,
                                block.end(), is_not_space);
                        if (i != block.end()) {
                            size_t pos = i - block.begin();
                            const wstring& block_substr = block.substr(pos, needle.size());
                            if (block_substr != needle) {
                                throw Exception(boost::str(
                                            boost::format("Unexpetced '%2%' "
                                                "looking for '%1%' (context: '%3%')")
                                            % wstring_to_utf8(needle) % wstring_to_utf8(block_substr)
                                            % wstring_to_utf8(block)));
                            }
                            segment_num++;
                            string segm_id = boost::str(
                                    boost::format("%1%.%2%-seg",
                                        std::locale::classic())
                                    % para_id % segment_num);
                            size_t start = pos + block_offset;
                            size_t end = start + needle.length();
                            lex.setLexerData(new NKJPSegmentData(segm_id,
                                        start, end));
                            block_cursor = pos + needle.length();
                            break;
                        }

                        ++raw_i;
                        if (raw_i == end) {
                            throw Exception(boost::str(
                                        boost::format("Unexpetced end of file "
                                            "looking for '%1%'")
                                        % wstring_to_utf8(needle)));
                        }

                        block_offset += block.length();
                        block_cursor = 0;
                        block = wxml_to_wstring(raw_i->str(MATCH_TEXT));
                    }
                    break;
                }

                case Lexeme::START_OF_AMBIGUITY:
                    assert(!in_ambiguity);
                    in_ambiguity = true;
                    saved_raw_i = raw_i;
                    saved_block_cursor = block_cursor;
                    saved_block_offset = block_offset;
                    break;

                case Lexeme::UNRESOLVED_FRAGMENT:
                    throw Exception("NKJPTextLexer cannot align text with "
                            "unresolved segmentation.");
                    
                case Lexeme::ACCEPTED_FRAGMENT:
                case Lexeme::REJECTED_FRAGMENT:
                    assert(in_ambiguity);
                    raw_i = saved_raw_i;
                    block_cursor = saved_block_cursor;
                    block_offset = saved_block_offset;
                    block = wxml_to_wstring(raw_i->str(MATCH_TEXT));
                    break;

                case Lexeme::END_OF_AMBIGUITY:
                    assert(in_ambiguity);
                    in_ambiguity = false;
                    break;

                default:
                    // do nothing
                    ;
            }
        }

        // Make sure the text is not truncated and left inside an ambiguity.
        assert(!in_ambiguity);
    }
};

} // namespace NLPCommon

#endif /* NKJPTEXTLEXER_H_ */
