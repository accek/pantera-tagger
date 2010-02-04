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
                proto_token.set_type(CorpusProto::Token::SEGMENT);
                proto_token.set_orth(lex.getOrth());
                proto_token.clear_interp();
                typedef typename Lexeme::tag_type tag_type;
                BOOST_FOREACH(const tag_type& tag, lex.getAllowedTags()) {
                    CorpusProto::Interpretation* interp = proto_token.add_interp();
                    string stag = tag.asString(tagset);
                    interp->set_tag(stag);
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
                proto_token.set_type(CorpusProto::Token::START_OF_SENTENCE);
                break;

            case Lexeme::START_OF_PARAGRAPH:
                proto_token.set_type(CorpusProto::Token::START_OF_PARAGRAPH);
                break;

            case Lexeme::END_OF_SENTENCE:
                proto_token.set_type(CorpusProto::Token::END_OF_SENTENCE);
                break;

            case Lexeme::END_OF_PARAGRAPH:
                proto_token.set_type(CorpusProto::Token::END_OF_PARAGRAPH);
                break;
        }

        return proto_token;
    }

public:
    CorpusWriter(std::ostream& stream) : Writer<Lexeme>(stream) { }

    virtual void writeToStream(WriterDataSource<Lexeme>& data_source) {
        GzipOutputStream::Options opt;
        opt.compression_level = 3;

        OstreamOutputStream proto_raw_stream(&this->stream);
        GzipOutputStream proto_gzip_stream(&proto_raw_stream, opt);
        CodedOutputStream proto_stream(&proto_gzip_stream);

        const Tagset* tagset = data_source.getTagset();
        CorpusProto::Tagset proto_tagset =
            CorpusTagsetLoader::tagset2ProtoTagset(tagset);
        proto_stream.WriteVarint32(proto_tagset.ByteSize());
        proto_tagset.SerializeToCodedStream(&proto_stream);

        size_t n = data_source.getSize();
        proto_stream.WriteVarint32(n);
        for (size_t i = 0; i < n; i++) {
            const Lexeme& lex = data_source.nextLexeme();
            CorpusProto::Token token = lexeme2ProtoToken(lex, tagset);
            proto_stream.WriteVarint32(token.ByteSize());
            token.SerializeToCodedStream(&proto_stream);
            this->advanceProgress();
        }
    }
};

} // namespace NLPCommon

#endif /* CORPUSWRITER_H_ */
