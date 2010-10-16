/*
 * ipipanwriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef NKJPWRITER_H_
#define NKJPWRITER_H_

#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <locale>
#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include <nlpcommon/writer.h>
#include <nlpcommon/util.h>
#include <nlpcommon/nkjplexerdata.h>

namespace NLPCommon {

using std::wstring;

template<class Lexeme = DefaultLexeme>
class NKJPSegmWriter : public Writer<Lexeme>
{
private:
    string para_id;
    bool no_space;
    int id_generator;
    bool in_rejected;
    bool in_paren;

    void openParen() {
        assert(!in_paren);
        this->stream << "      <nkjp:paren>\n";
        in_paren = true;
    }

    void closeParen() {
        if (in_paren) {
            this->stream << "      </nkjp:paren>\n";
            in_paren = false;
        }
    }

    void writeLexeme(const Lexeme& lex, const Tagset* tagset) {
        switch (lex.getType()) {
            case Lexeme::SEGMENT:
            {
                int start, end;
                string id;

                NKJPSegmentData* ld = dynamic_cast<NKJPSegmentData*>(lex.getLexerData());
                if (ld) {
                    start = ld->getStart();
                    end = ld->getEnd();
                    id = ld->getId();
                } else {
                    start = end = 0;
                    id = boost::str(boost::format("seg%d",
                                std::locale::classic()) % (++id_generator));
                }

                this->stream << "      <!-- " << lex.getUtf8Orth() << " -->\n";
                this->stream << "      <seg corresp=\"text_structure.xml#string-range("
                    << para_id << ',' << start << ',' << end - start << ")\"";
                if (no_space)
                    this->stream << " nkjp:nps=\"true\"";
                if (in_rejected)
                    this->stream << " nkjp:rejected=\"true\"";
                this->stream << " xml:id=\"segm_" << id << "\"/>\n";
                no_space = false;
                break;
            }

            case Lexeme::NO_SPACE:
                no_space = true;
                break;

            case Lexeme::START_OF_PARAGRAPH:
            {
                NKJPParagraphData* ld = dynamic_cast<NKJPParagraphData*>(lex.getLexerData());
                if (ld) {
                    para_id = ld->getId();
                } else {
                    para_id = boost::str(boost::format("p%d",
                                std::locale::classic()) % ++id_generator);
                }
                this->stream << "    <p corresp=\"text_structure.xml#" << para_id << "\""
                   " xml:id=\"segm_" << para_id << "\">\n";
                break;
            }

            case Lexeme::END_OF_PARAGRAPH:
                this->stream << "    </p>\n";
                break;

            case Lexeme::START_OF_SENTENCE:
            {
                NKJPSentenceData* ld = dynamic_cast<NKJPSentenceData*>(lex.getLexerData());
                string id;
                if (ld) {
                    id = ld->getId();
                } else {
                    id = boost::str(boost::format("s%d", std::locale::classic()) % ++id_generator);
                }
                this->stream << "     <s xml:id=\"segm_" << id << "\">\n";
                break;
            }

            case Lexeme::END_OF_SENTENCE:
                this->stream << "     </s>\n";
                break;

            case Lexeme::START_OF_AMBIGUITY:
                this->stream << "      <choice>\n";
                break;

            case Lexeme::UNRESOLVED_FRAGMENT:
                this->closeParen();
                this->openParen();
                in_rejected = false;
                break;

            case Lexeme::ACCEPTED_FRAGMENT:
                this->closeParen();
                this->openParen();
                in_rejected = false;
                break;

            case Lexeme::REJECTED_FRAGMENT:
                this->closeParen();
                this->openParen();
                in_rejected = true;
                break;

            case Lexeme::END_OF_AMBIGUITY:
                this->closeParen();
                in_rejected = false;
                this->stream << "      </choice>\n";
                break;
        }
    }

public:
    NKJPSegmWriter(std::ostream& stream)
        : Writer<Lexeme>(stream), no_space(false) { }

    virtual void writeToStream(WriterDataSource<Lexeme>& data_source) {
        id_generator = 0;
        in_rejected = false;
        in_paren = false;

        this->stream <<
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<?oxygen RNGSchema=\"NKJP_segmentation.rng\" type=\"xml\"?>\n"
            "<teiCorpus xmlns=\"http://www.tei-c.org/ns/1.0\" xmlns:nkjp=\"http://www.nkjp.pl/ns/1.0\" xmlns:xi=\"http://www.w3.org/2001/XInclude\">\n"
            " <xi:include href=\"NKJP_header.xml\"/>\n"
            " <TEI>\n"
            "  <xi:include href=\"header.xml\"/>\n"
            "  <text xml:lang=\"pl\" xml:id=\"segm_text\">\n"
            "   <body xml:id=\"segm_body\">\n";

        const Tagset* tagset = data_source.getTagset();
        while (!data_source.eof()) {
            writeLexeme(data_source.nextLexeme(), tagset);
            this->advanceProgress();
        }

        this->stream <<
            "   </body>\n"
            "  </text>\n"
            " </TEI>\n"
            "</teiCorpus>\n";
    }
};

template<class Lexeme = DefaultLexeme>
class NKJPMorphWriter : public Writer<Lexeme>
{
private:
    string para_id;
    bool no_space;
    string time_str;
    int id_generator;
    bool in_rejected;

    typedef typename Lexeme::tag_type tag_type;

    // [[base, ctag], tag]
    typedef std::pair<std::pair<wstring, string>, tag_type> bct_type;

    vector<bct_type> extractBcts(
            const Lexeme& lex, const Tagset* tagset) {
        vector<bct_type> ret;
        typedef std::pair<tag_type, wstring> tag_base_type;
        BOOST_FOREACH(const tag_base_type& tb, lex.getTagBases()) {
            const tag_type& tag = tb.first;
            const wstring& base = tb.second;
            ret.push_back(std::make_pair(
                        std::make_pair(base, tag.asStringCtag(tagset)),
                        tag));
        }
        std::sort(ret.begin(), ret.end());
        ret.push_back(bct_type());
        return ret;
    }

    void writeLexeme(const Lexeme& lex, const Tagset* tagset)
    {
        switch (lex.getType()) {
            case Lexeme::SEGMENT:
            {
                int start, end;
                string id;

                if (in_rejected)
                    break;

                NKJPSegmentData* ld = dynamic_cast<NKJPSegmentData*>(lex.getLexerData());
                if (ld) {
                    start = ld->getStart();
                    end = ld->getEnd();
                    id = ld->getId();
                } else {
                    start = end = 0;
                    id = boost::str(boost::format("seg%d",
                                std::locale::classic()) % (++id_generator));
                }

                this->stream << "      <seg corresp=\"ann_segmentation.xml#segm_" << id << "\" xml:id=\"morph_" << id << "\">\n";
                this->stream << "       <fs type=\"morph\">\n";
                this->stream << "        <f name=\"orth\">\n";
                this->stream << "         <string>" << wstring_to_xml(lex.getOrth()) << "</string>\n";
                this->stream << "        </f>\n";
                this->stream << "        <!-- " << lex.getUtf8Orth() << " ["
                    << start << ',' << end - start << "] -->\n";

                std::vector<bct_type> all_bcts = extractBcts(lex, tagset);
                assert(all_bcts.size() > 0);
                std::vector<std::vector<bct_type> > bct_groups;
                std::pair<wstring, string> key = all_bcts[0].first;
                std::vector<bct_type> bct_group;
                BOOST_FOREACH(const bct_type& bct, all_bcts) {
                    if (bct.first != key) {
                        key = bct.first;
                        bct_groups.push_back(bct_group);
                        bct_group.clear();
                    }
                    if (bct.first.first.empty())
                        break;
                    bct_group.push_back(bct);
                }

                this->stream << "        <f name=\"interps\">\n";
                if (bct_groups.size() > 1)
                    this->stream << "         <vAlt>\n";

                int i = 0;
                int chosen_tag = -1;
                BOOST_FOREACH(const std::vector<bct_type>& bct_group, bct_groups) {
                    key = bct_group[0].first;
                    this->stream << "          <fs type=\"lex\" xml:id=\"morph_"
                        << id << '_' << i << "-lex\">\n";
                    this->stream << "           <f name=\"base\">\n";
                    this->stream << "            <string>" << wstring_to_xml(key.first)
                        << "</string>\n";
                    this->stream << "           </f>\n";
                    this->stream << "           <f name=\"ctag\">\n";
                    this->stream << "            <symbol value=\"" << key.second << "\"/>\n";
                    this->stream << "           </f>\n";
                    this->stream << "           <f name=\"msd\">\n";
                    if (bct_group.size() > 1)
                        this->stream << "            <vAlt>\n";

                    BOOST_FOREACH(const bct_type& bct, bct_group) {
                        this->stream << "             <symbol value=\"" <<
                            bct.second.asStringMsd(tagset) << "\" xml:id=\"morph_"
                                << id << '_' << i << "-msd\"/>\n";

                        if (lex.isAutoselectedTag(bct.second))
                            chosen_tag = i;

                        i++;
                    }

                    if (bct_group.size() > 1)
                        this->stream << "            </vAlt>\n";
                    this->stream << "           </f>\n";
                    this->stream << "          </fs>\n";
                }

                if (bct_groups.size() > 1)
                    this->stream << "         </vAlt>\n";
                this->stream << "        </f>\n";
                this->stream << "        <f name=\"disamb\">\n";
                this->stream << "         <fs feats=\"#pantera\" type=\"tool_report\">\n";
                this->stream << "          <f name=\"date\">\n";
                this->stream << "           <string>" << time_str << "</string>\n";
                this->stream << "          </f>\n";
                this->stream << "          <f name=\"resp\">\n";
                this->stream << "           <string>PANTERA Tagger (" PACKAGE_STRING ")</string>\n";
                this->stream << "          </f>\n";
                if (chosen_tag != -1) {
                    this->stream << "          <f fVal=\"#morph_"
                        << id << '_' << chosen_tag << "-msd\" name=\"choice\"/>\n";
                }
                this->stream << "         </fs>\n";
                this->stream << "        </f>\n";
                this->stream << "       </fs>\n";
                this->stream << "      </seg>\n";
                no_space = false;
                break;
            }

            case Lexeme::NO_SPACE:
                if (!in_rejected)
                    no_space = true;
                break;

            case Lexeme::START_OF_PARAGRAPH:
            {
                NKJPParagraphData* ld = dynamic_cast<NKJPParagraphData*>(lex.getLexerData());
                if (ld) {
                    para_id = ld->getId();
                } else {
                    para_id = boost::str(boost::format("p%d",
                                std::locale::classic()) % ++id_generator);
                }
                this->stream << "    <p xml:id=\"" << para_id << "\">\n";
                break;
            }

            case Lexeme::END_OF_PARAGRAPH:
                this->stream << "    </p>\n";
                break;

            case Lexeme::START_OF_SENTENCE:
            {
                NKJPSentenceData* ld = dynamic_cast<NKJPSentenceData*>(lex.getLexerData());
                string id;
                if (ld) {
                    id = ld->getId();
                } else {
                    id = boost::str(boost::format("s%d", std::locale::classic()) % ++id_generator);
                }
                this->stream << "     <s corresp=\"ann_segmentation.xml#segm_" << id << "\" xml:id=\"" << id << "\">\n";
                break;
            }

            case Lexeme::END_OF_SENTENCE:
                this->stream << "     </s>\n";
                break;

            case Lexeme::REJECTED_FRAGMENT:
                in_rejected = true;
                break;

            case Lexeme::ACCEPTED_FRAGMENT:
            case Lexeme::UNRESOLVED_FRAGMENT:
            case Lexeme::END_OF_AMBIGUITY:
                in_rejected = false;
                break;
        }
    }

public:
    NKJPMorphWriter(std::ostream& stream)
        : Writer<Lexeme>(stream), no_space(false)
    {
        namespace ptime = boost::posix_time;
        ptime::ptime now = ptime::second_clock::local_time();
        ptime::time_facet* output_facet = new ptime::time_facet();
        output_facet->format("%Y-%m-%d %H:%M:%S");
        std::ostringstream ss;
        ss.imbue(std::locale(std::locale::classic(), output_facet));
        ss << now;
        time_str = ss.str();
    }

    virtual void writeToStream(WriterDataSource<Lexeme>& data_source) {
        this->stream.imbue(get_utf8_locale());

        id_generator = 0;
        in_rejected = false;

        this->stream <<
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<?oxygen RNGSchema=\"NKJP_morphosyntax.rng\" type=\"xml\"?>\n"
            "<teiCorpus xmlns=\"http://www.tei-c.org/ns/1.0\" xmlns:nkjp=\"http://www.nkjp.pl/ns/1.0\" xmlns:xi=\"http://www.w3.org/2001/XInclude\">\n"
            " <xi:include href=\"NKJP_header.xml\"/>\n"
            " <TEI>\n"
            "  <xi:include href=\"header.xml\"/>\n"
            "  <text>\n"
            "   <body>\n";

        const Tagset* tagset = data_source.getTagset();
        while (!data_source.eof()) {
            writeLexeme(data_source.nextLexeme(), tagset);
            this->advanceProgress();
        }

        this->stream <<
            "   </body>\n"
            "  </text>\n"
            " </TEI>\n"
            "</teiCorpus>\n";
    }
};

} // namespace NLPCommon

#endif /* NKJPWRITER_H_ */
