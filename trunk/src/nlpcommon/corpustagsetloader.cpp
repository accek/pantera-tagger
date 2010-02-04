/*
 * corpustagsetloader.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <boost/foreach.hpp>

#include <nlpcommon/corpustagsetloader.h>

using namespace google::protobuf::io;

namespace NLPCommon {

const Tagset* CorpusTagsetLoader::protoTagset2Tagset(
        const CorpusProto::Tagset& proto_tagset)
{
    Tagset* tagset = new Tagset();

    BOOST_FOREACH(const CorpusProto::Tagset_Category& proto_category,
            proto_tagset.categories()) {
        Category* category = new Category(proto_category.name());
        BOOST_FOREACH(const string& value, proto_category.values())
            category->addValue(value);
        tagset->addCategory(category);
    }

    BOOST_FOREACH(const CorpusProto::Tagset_PartOfSpeech& proto_pos,
            proto_tagset.pos()) {
        PartOfSpeech* pos = new PartOfSpeech(proto_pos.name());
        for (int i = 0; i < proto_pos.categories_size(); i++)
            pos->addCategory(tagset->getCategory(
                        proto_pos.categories(i)),
                    proto_pos.required_categories_mask(i));
        tagset->addPartOfSpeech(pos);
    }

    return tagset;
}

CorpusProto::Tagset CorpusTagsetLoader::tagset2ProtoTagset(
        const Tagset* tagset) {
    CorpusProto::Tagset proto_tagset;

    BOOST_FOREACH(const Category* category, tagset->getCategories()) {
        CorpusProto::Tagset_Category* proto_category =
            proto_tagset.add_categories();
        proto_category->set_name(category->getName());
        BOOST_FOREACH(const string& value, category->getValues())
            proto_category->add_values(value);
    }

    BOOST_FOREACH(const PartOfSpeech* pos, tagset->getPartsOfSpeech()) {
        CorpusProto::Tagset_PartOfSpeech* proto_pos =
            proto_tagset.add_pos();
        proto_pos->set_name(pos->getName());
        const vector<const Category*> categories = pos->getCategories();
        for (int i = 0; i < categories.size(); i++) {
            proto_pos->add_categories(categories[i]->getName());
            proto_pos->add_required_categories_mask(pos->isRequiredCategory(i));
        }
    }

    return proto_tagset;
}

const Tagset* CorpusTagsetLoader::loadTagset(std::istream& stream) {
    IstreamInputStream proto_stream(&stream);
    CorpusProto::Tagset proto_tagset;
    proto_tagset.ParseFromZeroCopyStream(&proto_stream);
    return protoTagset2Tagset(proto_tagset);
}

}
