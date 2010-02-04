/*
 * corpuswriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef CORPUSWRITER_H_
#define CORPUSWRITER_H_

#include <vector>
#include <boost/foreach.hpp>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>
#include <google/protobuf/io/coded_stream.h>

#include <nlpcommon/writer.h>
#include <nlpcommon/corpustagsetloader.h>

namespace NLPCommon {

using namespace google::protobuf::io;

template<class Lexeme = DefaultLexeme>
class CorpusWriter : public Writer<Lexeme>
{
private:
    CorpusProto::Token lexeme2ProtoToken(const Lexeme& lex,
            const Tagset* tagset)
    {
        CorpusProto::Token proto_token;
        switch (lex.getType()) {
            case Lexeme::SEGMENT:
                stream <<
                    "<tok>\n"
                    "<orth>" << lex.getOrth() << "</orth>\n";

                typedef typename Lexeme::tag_type tag_type;
                BOOST_FOREACH(const tag_type& tag, lex.getAllowedTags()) {
                    CorpusProto::Interpretation* interp =
                        proto_token.add_interp();
                    interp->set_tag(tag.asString(tagset));
                    if (lex.isGoldenTag(tag))
                        interp->set_is_golden(true);
                    if (lex.isAutoselectedTag(tag))
                        interp->set_is_autoselected(true);
                }
                break;

            case Lexeme::NO_SPACE:
                proto_token.set_type(CorpusProto::Token::NO_SPACE);
                break;

            case Lexeme::START_OF_SENTENCE:
                stream << "<chunk type=\"s\">\n";
                break;

            case Lexeme::START_OF_PARAGRAPH:
                stream << "<chunk type=\"p\">\n";
                break;

            case Lexeme::END_OF_SENTENCE:
            case Lexeme::END_OF_PARAGRAPH:
                stream << "</chunk>\n";
                break;
        }

        return proto_token;
    }

public:
    CorpusWriter(std::ostream& stream) : Writer<Lexeme>(stream) { }

    virtual void writeToStream(WriterDataSource<Lexeme>& data_source) {
        stream <<
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE cesAna SYSTEM \"xcesAnaIPI.dtd\">\n"
            "<cesAna version=\"1.0\" type=\"lex disamb\">\n"
            "<chunkList>\n";

        while (!data_source.eof())
            writeLexeme(data_source.nextLexeme());

        stream <<
            "</chunkList>\n"
            "</cesAna>\n";
    }
};

} // namespace NLPCommon

#endif /* CORPUSWRITER_H_ */
