/*
 * lexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef POLISH_TAGSET_CONVERT_H_
#define POLISH_TAGSET_CONVERT_H_

#include <boost/unordered_map.hpp>

#include <nlpcommon/tagset_convert.h>

namespace NLPCommon {

template<class Tag>
class PolishTagsetConverter : public TagsetConverter<Tag>
{
private:
    boost::unordered_map<Tag, Tag> cache;

public:
    PolishTagsetConverter(const Tagset* from_tagset, const Tagset* to_tagset)
        : TagsetConverter<Tag>(from_tagset, to_tagset) { }

    static PolishTagsetConverter<Tag>* getSharedInstance(
            const Tagset* from_tagset, const Tagset* to_tagset) {
        typedef typename std::map<std::pair<const Tagset*, const Tagset*>,
            PolishTagsetConverter<Tag>*> cache_type;
        
        cache_type instances;
        std::pair<const Tagset*, const Tagset*> key = std::make_pair(from_tagset, to_tagset);
        typename cache_type::const_iterator i = instances.find(key);
        if (i != instances.end())
            return i->second;

        PolishTagsetConverter<Tag>* instance =
            new PolishTagsetConverter<Tag>(from_tagset, to_tagset);
        instances.insert(std::make_pair(key, instance));
        return instance;
    }

    virtual Tag convert(const Lexeme<Tag>& lex, const Tag& tag) {
        if (this->from_tagset == this->to_tagset)
            return tag;

        typename boost::unordered_map<Tag, Tag>::const_iterator i
            = cache.find(tag);
        if (i != cache.end())
            return i->second;

        const PartOfSpeech* from_pos = this->from_tagset->getPartOfSpeech(
                tag.getPos());
        const PartOfSpeech* to_pos = this->to_tagset->getPartOfSpeech(
                from_pos->getName(), false);

        // Remap unknown tag to "ign".
        if (to_pos == NULL) {
            to_pos = this->to_tagset->getPartOfSpeech("ign");
        }

        Tag ret;
        ret.setPos(this->to_tagset->getPartOfSpeechIndex(to_pos));

        int to_cat_index = 0;
        BOOST_FOREACH(const Category* to_cat, to_pos->getCategories()) {
            const Category* from_cat = this->from_tagset->getCategory(
                    to_cat->getName(), false);
            if (from_cat == NULL && to_pos->isRequiredCategory(to_cat_index)) {
                throw Exception(boost::str(boost::format(
                                "Cannot convert between tagsets: could not "
                                "determine value for required category '%1%' "
                                "for part of speech '%2%'")
                            % to_cat->getName() % to_pos->getName()));
            }
            if (from_cat != NULL) {
                // We assume the values between categories match.
                ret.setValue(this->to_tagset->getCategoryIndex(to_cat),
                        tag.getValue(
                            this->from_tagset->getCategoryIndex(from_cat)));
            }
            to_cat_index++;
        }

        // TODO: remap known different categories

        cache.insert(std::make_pair(tag, ret));

        return ret;
    }
};

} // namespace NLPCommon

#endif /* POLISH_TAGSET_CONVERT_H_ */
