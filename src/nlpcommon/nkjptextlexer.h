/*
 * nkjptextlexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef NKJPTEXTLEXER_H_
#define NKJPTEXTLEXER_H_

#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <locale>
#include <stack>

#include <nlpcommon/lexer.h>
#include <nlpcommon/util.h>
#include <nlpcommon/nkjplexerdata.h>

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
        std::cerr << id << std::endl;
        Lexeme lex(Lexeme::START_OF_PARAGRAPH);
        lex.setLexerData(new NKJPParagraphData(id));
        collector->collectLexeme(lex);
        new_paragraph = true;
        inside_paragraph = true;
    }

    void handleEndOfParagraph() {
        Lexeme lex(Lexeme::END_OF_PARAGRAPH);
        collector->collectLexeme(lex);
        inside_paragraph = false;
    }

public:
    NKJPTextLexer(std::istream& stream)
            : Lexer<Lexeme>(stream)
    {
        parsing_regex = boost::wregex(
				L"(?:(<p|<ab|<u)[^>]*?(?:xml:id=[\"'](.*?)[\"']).*?>|(/gap>|lb/>))"
                  "(.*?)<(/p>|/ab>|/u>)?"
                );

        text_regex = boost::wregex(
				L"(\\s)?([^\\s]+)"  // Word may be preceded by space
                );
    }

    enum {
        MATCH_NEWPAR = 1,
        MATCH_NEWPAR_ID = 2,
        MATCH_CONTPAR = 3,
        MATCH_TEXT = 4,
        MATCH_ENDPAR = 5,

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
        boost::wsregex_iterator i(raw_text.begin(), raw_text.end(), parsing_regex);
        boost::wsregex_iterator end;
        for (; i != end; ++i) {
            if ((*i)[MATCH_NEWPAR].matched)
                handleNewParagraph(wstring_to_utf8(i->str(MATCH_NEWPAR_ID)));
            boost::wsregex_iterator j((*i)[MATCH_TEXT].first, (*i)[MATCH_TEXT].second, text_regex);
            for (; j != end; ++j) {
                handleOrth(j->str(MATCH_ORTH), (*j)[MATCH_PRECEDING_SPACE].matched);
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

        int text_len = text.size();
        int i;
        for (i = 0; i < text_len; i++) {
            Lexeme& lex = text[i];
            switch (lex.getType()) {
                case Lexeme::START_OF_PARAGRAPH:
                {
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

                    block = raw_i->str(MATCH_TEXT);
                    block_cursor = 0;
                    block_offset = 0;
                    sentence_num = 0;
                    break;
                }

                case Lexeme::START_OF_SENTENCE:
                {
                    sentence_num++;
                    string sent_id = boost::str(
                            boost::format("%1%.%2%-s")
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

                        size_t pos = block.find(needle, block_cursor);
                        if (pos != string::npos) {
                            segment_num++;
                            string segm_id = boost::str(
                                    boost::format("%1%.%2%-seg")
                                    % para_id % segment_num);
                            size_t start = pos + block_offset;
                            size_t end = start + needle.length();
                            lex.setLexerData(new NKJPSegmentData(segm_id,
                                        start, end));
                            block_cursor = pos + 1;
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
                        block = raw_i->str(MATCH_TEXT);
                    }
                    break;
                }

                default:
                    // do nothing
                    ;
            }
        }
    }
};

} // namespace NLPCommon

#endif /* NKJPTEXTLEXER_H_ */