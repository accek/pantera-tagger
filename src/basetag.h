/*
 * tag.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef BASETAG_H_
#define BASETAG_H_

#include <boost/format.hpp>
#include <boost/unordered_map.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include "tagset.h"
#include "pos.h"
#include "util.h"
#include "exception.h"

namespace BTagger {

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

    static const Final& getNullTag()
            { assert(false); }

    string asString(const Tagset* tagset) const {
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
};

} // namespace BTagger

#endif /* TAG_H_ */
