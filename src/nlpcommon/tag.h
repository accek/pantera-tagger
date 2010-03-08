#ifndef TAG_H_
#define TAG_H_

#include <boost/format.hpp>
#include <boost/unordered_map.hpp>
#include <boost/cstdint.hpp>
#include <boost/functional/hash.hpp>
#include <boost/serialization/access.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <nlpcommon/tagset.h>
#include <nlpcommon/pos.h>
#include <nlpcommon/util.h>
#include <nlpcommon/exception.h>

// TODO: comment why implementation in headers

namespace NLPCommon {

using std::string;

// This is an interface specification. You should inherit from this
// class to implement tags.
//
// This class is not polymorphic (i.e. it has no virtual methods) for
// performance reasons. Instead to implement a tag class, derive
// from BaseTag<YourClass> and override provided methods.
//
// All classes, which use tags are templates parameterized by specific
// implementation of BaseTag.

template<class Final>
class BaseTag
{
private:
    static Final parseStringImpl(const Tagset* tagset, const string& tag_spec) {
        string str = tag_spec;
        string pos_name = find_prefix(str, ':');
        const PartOfSpeech* pos = tagset->getPartOfSpeech(pos_name);
        Final tag;
        tag.clear();
        tag.setPos(tagset->getPartOfSpeechIndex(pos));

        const vector<const Category*>& cats = pos->getCategories();
        string value = find_prefix(str, ':');
        for (unsigned int i = 0; i < cats.size(); i++) {
            const Category* cat = cats[i];
            bool valid_value = cat->hasValue(value);
            if (!valid_value && pos->isRequiredCategory(i))
                throw Exception(boost::str(
                        boost::format("Missing specification for "
                                "category '%1%' in tag specification "
                                "'%2%'") % cat->getName() % tag_spec));
            if (!valid_value)
                continue;
            tag.setValue(tagset->getCategoryIndex(cat), cat->getIndex(value));
            value = find_prefix(str, ':');
        }
        if (!value.empty())
            throw Exception(boost::str(
                    boost::format("Unrecognized element "
                            "'%1%' in tag specification "
                            "'%2%'") % value % tag_spec));

        assert(tag_spec == tag.asString(tagset));
        return tag;
    }

public:
    static const int INVALID_POS = 255;

    // TODO: comment -- no destructor, please!

    // TODO: comment
    static Final parseString(const Tagset* tagset, const string& tag_spec) {
        typedef typename std::pair<const Tagset*, string> key_type;
        key_type key = std::make_pair(tagset, tag_spec);
        typedef boost::unordered_map<key_type, Final> cache_type;
        static cache_type cache;
        typename cache_type::const_iterator i = cache.find(key);
        if (i != cache.end())
            return i->second;
        Final tag = parseStringImpl(tagset, tag_spec);
        cache.insert(std::make_pair(key, tag));
        return tag;
    }

    // TODO: comment
    Final project(const Tagset* tagset) const {
        Final* fthis = (Final*)this;
        if (tagset == NULL)
            return *fthis;
        if (*fthis == Final::getNullTag())
            return *fthis;

        // TODO: check tagsets compatibility
        Final projected;
        projected.clear();
        projected.setPos(fthis->getPos());
        const PartOfSpeech* pos = tagset->getPartOfSpeech(fthis->getPos());
        const vector<const Category*>& cats = pos->getCategories();
        for (unsigned int i = 0; i < cats.size(); i++) {
            const Category* cat = cats[i];
            unsigned int cat_index = tagset->getCategoryIndex(cat);
            projected.setValue(cat_index, fthis->getValue(cat_index));
        }
        return projected;
    }

    void setValue(unsigned int category_index, unsigned int value_index)
            { assert(false); }
    unsigned int getValue(unsigned int category_index) const
            { assert(false); }
    void setPos(unsigned int pos_index)
            { assert(false); }
    unsigned int getPos() const
            { assert(false); }
    void clear()
            { assert(false); }

    bool operator==(const Final& other) const
            { assert(false); }
    bool operator!=(const Final& other) const
            { assert(false); }

    static const Final& getNullTag() {
        static Final tag;
        static bool initialized = false;
        if (!initialized) {
            tag.clear();
            tag.setPos(INVALID_POS);
            initialized = true;
        }
        return tag;
    }

    const string asString(const Tagset* tagset) const {
        Final* fthis = (Final*)this;

        if (*fthis == Final::getNullTag())
            return "[null]";

        std::ostringstream stream;
        const PartOfSpeech* pos = tagset->getPartOfSpeech(fthis->getPos());
        stream << pos->getName();
        vector<const Category*> cats = pos->getCategories();
        unsigned int n = cats.size();
        for (unsigned int i = 0; i < n; i++) {
            const Category* cat = cats[i];
            unsigned int value = fthis->getValue(tagset->getCategoryIndex(cat));
            if (value == 0 && !pos->isRequiredCategory(i))
                continue;
            stream << ':' << cat->getValue(value);
        }
        return stream.str();
    }

    const wstring asWString(const Tagset* tagset) const {
        return ascii_to_wstring(this->asString(tagset));
    }
};

class Tag : public BaseTag<Tag>
{
private:
    static const int max_num_categories = 15;

    // First max_num_categories entries store values for categories.
    // The last entry is part of speech.
    boost::uint8_t _data[max_num_categories + 1];

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & _data;
    }

public:
    void clear() {
        memset(_data, 0, sizeof(_data));
    }

    void setValue(unsigned int category_index, unsigned int value_index) {
        _data[category_index] = value_index;
    }

    unsigned int getValue(unsigned int category_index) const {
        return _data[category_index];
    }

    void setPos(unsigned int pos_index) {
        _data[max_num_categories] = pos_index;
    }

    unsigned int getPos() const {
        return _data[max_num_categories];
    }

    bool operator==(const Tag& other) const {
        return !memcmp(_data, other._data, sizeof(_data));
    }

    bool operator!=(const Tag& other) const {
        return memcmp(_data, other._data, sizeof(_data));
    }

    bool operator<(const Tag& other) const {
        return memcmp(_data, other._data, sizeof(_data)) < 0;
    }

    bool operator<=(const Tag& other) const {
        return memcmp(_data, other._data, sizeof(_data)) <= 0;
    }

    bool operator>(const Tag& other) const {
        return memcmp(_data, other._data, sizeof(_data)) > 0;
    }

    bool operator>=(const Tag& other) const {
        return memcmp(_data, other._data, sizeof(_data)) >= 0;
    }

    std::size_t hash() const {
        return boost::hash_range(_data, _data + max_num_categories + 1);
    }
} __attribute__((packed));

std::size_t hash_value(const Tag& tag) {
    return tag.hash();
}

} // namespace NLPCommon

#endif /* TAG_H_ */
