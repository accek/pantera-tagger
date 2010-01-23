/*
 * corpuswriter.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <vector>
#include <boost/foreach.hpp>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>
#include <google/protobuf/io/coded_stream.h>

#include <nlpcommon/corpuswriter.h>

using namespace google::protobuf::io;

namespace NLPCommon {

void CorpusWriter::writeToStream(std::ostream& stream,
        std::vector<Token>& tokens) {
    GzipOutputStream::Options opt;
    opt.compression_level = 3;

    OstreamOutputStream proto_raw_stream(&stream);
    GzipOutputStream proto_gzip_stream(&proto_raw_stream, opt);
    CodedOutputStream proto_stream(&proto_gzip_stream);

    proto_stream.WriteVarint32(tokens.size());
    BOOST_FOREACH(Token& token, tokens) {
        proto_stream.WriteVarint32(token.ByteSize());
        token.SerializeToCodedStream(&proto_stream);
        advanceProgress();
    }
}

}
