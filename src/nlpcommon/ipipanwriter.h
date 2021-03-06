/*
 * corpuswriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef IPIPANWRITER_H_
#define IPIPANWRITER_H_

#include <vector>
#include <utility>
#include <string>
#include <boost/foreach.hpp>

#include <nlpcommon/writer.h>
#include <nlpcommon/util.h>

namespace NLPCommon {

using std::wstring;

template<class Lexeme = DefaultLexeme>
class IpiPanWriter : public Writer<Lexeme>
{
private:
    bool use_disamb_sh;

	void writeLexeme(const Lexeme& lex, const Tagset* tagset)
    {
        switch (lex.getType()) {
            case Lexeme::SEGMENT:
            {
                this->stream <<
                    "<tok>\n"
                    "<orth>" << wstring_to_xml(lex.getOrth()) << "</orth>\n";

                typedef typename Lexeme::tag_type tag_type;
				typedef std::pair<tag_type, wstring> tag_base_type;
                BOOST_FOREACH(const tag_type& tag, lex.getAutoselectedTags()) {
                    this->stream << "<!-- autoselected: " <<
                        tag.asString(tagset) << " -->\n";
                }

                bool tag_selected = false;
                BOOST_FOREACH(const tag_base_type& tb, lex.getTagBases()) {
					const tag_type& tag = tb.first;
					const wstring& base = tb.second;

					this->stream << "<lex";
                    if (this->use_disamb_sh) {
                        if (lex.isGoldenTag(tag))
                            this->stream << " disamb=\"1\"";
                        if (!lex.isAutoselectedTag(tag))
                            this->stream << " disamb_sh=\"0\"";
                    } else {
                        if (lex.isAutoselectedTag(tag) && !tag_selected) {
                            this->stream << " disamb=\"1\"";
                            tag_selected = true;
                        }
                    }
					this->stream << "><base>" << wstring_to_xml(base)
						<< "</base><ctag>" << tag.asString(tagset)
						<< "</ctag></lex>\n";
                }
                this->stream << "</tok>\n";
                break;
            }

            case Lexeme::NO_SPACE:
				this->stream << "<ns/>\n";
                break;

            case Lexeme::START_OF_PARAGRAPH:
                this->stream << "<chunk type=\"p\">\n";
                break;

            case Lexeme::START_OF_SENTENCE:
                this->stream << "<chunk type=\"s\">\n";
                break;

            case Lexeme::END_OF_SENTENCE:
            case Lexeme::END_OF_PARAGRAPH:
				this->stream << "</chunk>\n";
                break;
        }
    }

public:
    IpiPanWriter(std::ostream& stream, bool use_disamb_sh = true)
        : Writer<Lexeme>(stream), use_disamb_sh(use_disamb_sh) { }

    virtual void writeToStream(WriterDataSource<Lexeme>& data_source) {
        this->stream.imbue(get_utf8_locale());

        this->stream <<
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE cesAna SYSTEM \"xcesAnaIPI.dtd\">\n"
            "<cesAna version=\"1.0\" type=\"lex disamb\">\n"
            "<chunkList>\n";

		const Tagset* tagset = data_source.getTagset();
        while (!data_source.eof()) {
            writeLexeme(data_source.nextLexeme(), tagset);
            this->advanceProgress();
        }

        this->stream <<
            "</chunkList>\n"
            "</cesAna>\n";
    }
};

} // namespace NLPCommon

#endif /* IPIPANWRITER_H_ */
