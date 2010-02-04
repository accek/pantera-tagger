/*
 * corpuswriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef CORPUSLEXER_H_
#define CORPUSLEXER_H_

#include <boost/cstdint.hpp>
#include <vector>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>
#include <google/protobuf/io/coded_stream.h>

#include <nlpcommon/lexer.h>
#include <nlpcommon/corpus.pb.h>
#include <nlpcommon/corpustagsetloader.h>

namespace NLPCommon {

using namespace google::protobuf::io;

template<class Lexeme>
class CorpusWriter;

template<class Lexeme = DefaultLexeme>
class CorpusLexer : public Lexer<Lexeme>
{
    friend class CorpusWriter<Lexeme>;

private:
    const Tagset* tagset;

    void parseTagset(CodedInputStream& proto_stream) {
        uint32_t size;
        CorpusProto::Tagset proto_tagset;
        proto_stream.ReadVarint32(&size);
        CodedInputStream::Limit limit = proto_stream.PushLimit(size);
        proto_tagset.ParseFromCodedStream(&proto_stream);
        proto_stream.PopLimit(limit);

        assert(tagset == NULL);
        tagset = CorpusTagsetLoader::protoTagset2Tagset(proto_tagset);
    }

    Lexeme corpusTokenToLexeme(const Tagset* tagset,
            const CorpusProto::Token& token) {
        Lexeme lex;

        switch (token.type()) {
            case CorpusProto::Token::SEGMENT:
                lex.setOrth(token.orth());
                BOOST_FOREACH(const CorpusProto::Interpretation& interp,
                        token.interp()) {
                    typename Lexeme::tag_type tag =
                        Lexeme::tag_type::parseString(tagset, interp.tag());
                    lex.addAllowedTag(tag);
                    if (interp.is_golden())
                        lex.addGoldenTag(tag);
                    if (interp.is_autoselected())
                        lex.addAutoselectedTag(tag);
                }
                break;

            case CorpusProto::Token::NO_SPACE:
                lex.setType(Lexeme::NO_SPACE);
                break;

            case CorpusProto::Token::START_OF_SENTENCE:
                lex.setType(Lexeme::START_OF_SENTENCE);
                break;

            case CorpusProto::Token::START_OF_PARAGRAPH:
                lex.setType(Lexeme::START_OF_PARAGRAPH);
                break;

            case CorpusProto::Token::END_OF_SENTENCE:
                lex.setType(Lexeme::END_OF_SENTENCE);
                break;

            case CorpusProto::Token::END_OF_PARAGRAPH:
                lex.setType(Lexeme::END_OF_PARAGRAPH);
                break;
        }

        return lex;
    }

public:
    CorpusLexer(std::istream& stream) : Lexer<Lexeme>(stream) { }

    virtual void parseStream(LexerCollector<Lexeme>& collector) {
        IstreamInputStream proto_raw_stream(&this->stream);
        GzipInputStream proto_gzip_stream(&proto_raw_stream);
        CodedInputStream proto_stream(&proto_gzip_stream);
        proto_stream.SetTotalBytesLimit(512 << 20, 512 << 19);

        parseTagset(proto_stream);
        collector.setTagset(tagset);

        uint32_t num_tokens;
        proto_stream.ReadVarint32(&num_tokens);
        collector.setSize(num_tokens);

        CorpusProto::Token token;
        uint32_t size;
        for (uint32_t i = 0; i < num_tokens; i++) {
            proto_stream.ReadVarint32(&size);
            CodedInputStream::Limit limit = proto_stream.PushLimit(size);
            token.ParseFromCodedStream(&proto_stream);
            proto_stream.PopLimit(limit);
            Lexeme lex = corpusTokenToLexeme(tagset, token);
            collector.collectLexeme(lex);
            this->advanceProgress();
        }
    }
};

} // namespace NLPCommon

#endif /* CORPUSLEXER_H_ */
