/*
 * corpusreader.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <boost/cstdint.hpp>
#include <vector>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>
#include <google/protobuf/io/coded_stream.h>

#include <nlpcommon/corpusreader.h>

using namespace google::protobuf::io;

namespace NLPCommon {

void CorpusReader::parseStream(std::istream& stream, std::vector<Token>& tokens)
{
    IstreamInputStream proto_raw_stream(&stream);
    GzipInputStream proto_gzip_stream(&proto_raw_stream);
    CodedInputStream proto_stream(&proto_gzip_stream);
    proto_stream.SetTotalBytesLimit(512 << 20, 512 << 19);

    uint32_t num_tokens;
    proto_stream.ReadVarint32(&num_tokens);
    tokens.reserve(num_tokens);

    Token token;
    uint32_t size;
    for (uint32_t i = 0; i < num_tokens; i++) {
        proto_stream.ReadVarint32(&size);
        CodedInputStream::Limit limit = proto_stream.PushLimit(size);
        token.ParseFromCodedStream(&proto_stream);
        proto_stream.PopLimit(limit);
        tokens.push_back(token);
        advanceProgress();
    }
}

}
