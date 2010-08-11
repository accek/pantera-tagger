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
#include <boost/foreach.hpp>

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

	void writeLexeme(const Lexeme& lex, const Tagset* tagset)
    {
        switch (lex.getType()) {
            case Lexeme::SEGMENT:
            {
                NKJPSegmentData* ld = dynamic_cast<NKJPSegmentData*>(lex.getLexerData());
                this->stream << "      <seg ";
                if (no_space)
                    this->stream << "nkjp:nps=\"true\" ";
                this->stream << "xml:id=\"segm_" << ld->getId() << "\">\n";
                this->stream << "       <xi:include href=\"text.xml\" "
                    "xpointer=\"string-range(" << para_id << ','
                    << ld->getStart() << ','
                    << ld->getEnd() - ld->getStart() << ")\"/>\n";
                this->stream << "      </seg>\n";
                this->stream << "      <!-- " << lex.getUtf8Orth() << " -->\n";
                no_space = false;
                break;
            }

            case Lexeme::NO_SPACE:
                no_space = true;
                break;

            case Lexeme::START_OF_PARAGRAPH:
            {
                NKJPParagraphData* ld = dynamic_cast<NKJPParagraphData*>(lex.getLexerData());
                para_id = ld->getId();
                this->stream << "    <p xml:id=\"segm_" << para_id << "\">\n";
                break;
            }

            case Lexeme::END_OF_PARAGRAPH:
				this->stream << "    </p>\n";
                break;

            case Lexeme::START_OF_SENTENCE:
            {
                NKJPSentenceData* ld = dynamic_cast<NKJPSentenceData*>(lex.getLexerData());
                this->stream << "     <s xml:id=\"segm_" << ld->getId() << "\">\n";
                break;
            }

            case Lexeme::END_OF_SENTENCE:
				this->stream << "     </s>\n";
                break;
        }
    }

public:
    NKJPSegmWriter(std::ostream& stream)
        : Writer<Lexeme>(stream), no_space(false) { }

    virtual void writeToStream(WriterDataSource<Lexeme>& data_source) {
        this->stream <<
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<?oxygen RNGSchema=\"NKJP_segmentation.rng\" type=\"xml\"?>\n"
            "<teiCorpus xmlns=\"http://www.tei-c.org/ns/1.0\" xmlns:nkjp=\"http://www.nkjp.pl/ns/1.0\" xmlns:xi=\"http://www.w3.org/2001/XInclude\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
            " <xi:include href=\"NKJP_1M_header.xml\"/>\n"
            "  <TEI>\n"
            "   <xi:include href=\"header.xml\"/>\n"
            "    <text xml:lang=\"pl\" xml:id=\"segm_text\">\n"
            "     <body xml:id=\"segm_body\">\n";

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
        return ret;
    }

	void writeLexeme(const Lexeme& lex, const Tagset* tagset)
    {
        switch (lex.getType()) {
            case Lexeme::SEGMENT:
            {
                NKJPSegmentData* ld = dynamic_cast<NKJPSegmentData*>(lex.getLexerData());
                this->stream << "      <seg xlink:href=\"ann_segmentation.xml#segm_" << ld->getId() << "\" xml:id=\"morph_" << ld->getId() << "\">\n";
                this->stream << "       <fs type=\"morph\">\n";
                this->stream << "        <f name=\"orth\">\n";
                this->stream << "         <string>" << lex.getUtf8Orth() << "</string>\n";
                this->stream << "        </f>\n";
                this->stream << "        <f name=\"interps\">\n";

                std::pair<wstring, string> key;
                bool first = true;
                int chosen_tag = -1;
                int i = 0;
                BOOST_FOREACH(const bct_type& bct, extractBcts(lex, tagset)) {
                    if (bct.first != key) {
                        key = bct.first;
                        if (first) {
                            first = false;
                        } else {
                            this->stream << "           </vAlt>\n";
                            this->stream << "          </f>\n";
                            this->stream << "         </fs>\n";
                        }
                        this->stream << "         <fs type=\"lex\" xml:id=\"morph_"
                            << ld->getId() << '_' << i << "-lex\">\n";
                        this->stream << "          <f name=\"base\">\n";
                        this->stream << "           <string>" << wstring_to_utf8(key.first)
                            << "</string>\n";
                        this->stream << "          </f>\n";
                        this->stream << "          <f name=\"ctag\">\n";
                        this->stream << "           <symbol value=\"" << key.second << "\"/>\n";
                        this->stream << "          </f>\n";
                        this->stream << "          <f name=\"msd\">\n";
                        this->stream << "           <vAlt>\n";
                    }

                    this->stream << "            <symbol value=\"" <<
                        bct.second.asStringMsd(tagset) << "\" xml:id=\"morph_"
                            << ld->getId() << '_' << i << "-msd\">\n";

                    if (lex.isAutoselectedTag(bct.second))
                        chosen_tag = i;

                    i++;
                }

                this->stream << "           </vAlt>\n";
                this->stream << "          </f>\n";
                this->stream << "         </fs>\n";
                this->stream << "        </f>\n";
                this->stream << "        <f name=\"disamb\">\n";
                this->stream << "         <fs type=\"tool_report\">\n";
                this->stream << "          <f name=\"tool\">\n";
                this->stream << "           <string>PANTERA Tagger (" PACKAGE_STRING ")</string>\n";
                this->stream << "          </f>\n";
                if (chosen_tag != -1) {
                    this->stream << "          <f fVal=\"#morph_"
                        << ld->getId() << '_' << chosen_tag << "-msd\" name=\"choice\"/>\n";
                }
                this->stream << "         </fs>\n";
                this->stream << "        </f>\n";
                this->stream << "       </fs>\n";
                this->stream << "      </seg>\n";
                this->stream << "      <!-- " << lex.getUtf8Orth() << " -->\n";
                no_space = false;
                break;
            }

            case Lexeme::NO_SPACE:
                no_space = true;
                break;

            case Lexeme::START_OF_PARAGRAPH:
            {
                NKJPParagraphData* ld = dynamic_cast<NKJPParagraphData*>(lex.getLexerData());
                para_id = ld->getId();
                this->stream << "    <p xlink:href=\"ann_segmentation.xml#segm_" << para_id << "\" xml:id=\"" << para_id << "\">\n";
                break;
            }

            case Lexeme::END_OF_PARAGRAPH:
				this->stream << "    </p>\n";
                break;

            case Lexeme::START_OF_SENTENCE:
            {
                NKJPSentenceData* ld = dynamic_cast<NKJPSentenceData*>(lex.getLexerData());
                this->stream << "     <s xlink:href=\"ann_segmentation.xml#segm_" << ld->getId() << "\" xml:id=\"" << ld->getId() << "\">\n";
                break;
            }

            case Lexeme::END_OF_SENTENCE:
				this->stream << "     </s>\n";
                break;
        }
    }

public:
    NKJPMorphWriter(std::ostream& stream)
        : Writer<Lexeme>(stream), no_space(false) { }

    virtual void writeToStream(WriterDataSource<Lexeme>& data_source) {
        this->stream <<
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<?oxygen RNGSchema=\"NKJP_segmentation.rng\" type=\"xml\"?>\n"
            "<teiCorpus xmlns=\"http://www.tei-c.org/ns/1.0\" xmlns:nkjp=\"http://www.nkjp.pl/ns/1.0\" xmlns:xi=\"http://www.w3.org/2001/XInclude\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
            " <xi:include href=\"NKJP_1M_header.xml\"/>\n"
            "  <TEI>\n"
            "   <xi:include href=\"header.xml\"/>\n"
            "    <text xml:lang=\"pl\" xml:id=\"segm_text\">\n"
            "     <body xml:id=\"segm_body\">\n";

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
