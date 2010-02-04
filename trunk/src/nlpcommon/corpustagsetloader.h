/*
 * spejdtagsetloader.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef CORPUSTAGSETLOADER_H_
#define CORPUSTAGSETLOADER_H_

#include <nlpcommon/tagsetloader.h>
#include <nlpcommon/corpus.pb.h>

namespace NLPCommon {

class CorpusTagsetLoader : public TagsetLoader
{
public:
    virtual const Tagset* loadTagset(std::istream& stream);

    static const Tagset* protoTagset2Tagset(
            const CorpusProto::Tagset& proto_tagset);
    static CorpusProto::Tagset tagset2ProtoTagset(const Tagset* tagset);
};

} // namespace NLPCommon

#endif /* CORPUSTAGSETLOADER_H_ */
