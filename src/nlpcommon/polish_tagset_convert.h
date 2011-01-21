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
            if (from_pos->getName() == "xxs") {
                // Convert 'xxs' to 'subst'.
                to_pos = this->to_tagset->getPartOfSpeech("subst");
            } else {
                to_pos = this->to_tagset->getPartOfSpeech("ign");
            }
        }

        Tag ret;
        ret.clear();
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
                // 
                // We also consider the case of reduced gender (no
                // n1,n2,n3,p1,p2,p3) and two possibilities for
                // degree com/comp.

                int to_index = this->to_tagset->getCategoryIndex(to_cat);
                int value = tag.getValue(
                                this->from_tagset->getCategoryIndex(from_cat));
                if (to_cat->getValues().size() <= value) {
                    string value_str = from_cat->getValue(value);
                    if (from_cat->getName() == "gender") {
                        if (value_str == "p1") {
                            value_str = "m1";
                            value = to_cat->getIndex(value_str);
                        } else if (value_str == "p2" || value_str == "p3") {
                            value_str = "n";
                            value = to_cat->getIndex(value_str);
                        } else if (value_str[0] == 'n') {
                            value = to_cat->getIndex("n");
                        } else {
                            throw Exception(boost::str(boost::format(
                                            "Unexpected gender '%1%' when converting "
                                            "between Polish tagsets.")
                                        % value_str));
                        }
                    } else if (from_cat->getName() == "degree") {
                        if (value_str == "comp" || value_str == "com") {
                            try {
                                value = to_cat->getIndex("comp");
                            } catch (const CategoryValueNotFoundException& e) {
                                value = to_cat->getIndex("com");
                            }
                        }
                    } else {
                        throw Exception(boost::str(boost::format(
                                        "Don't know how to use reduced category "
                                        "'%1%' (value to convert: '%2%')")
                                    % to_cat->getName() % value_str));
                    }
                }
                ret.setValue(to_index, value);
            }
            to_cat_index++;
        }

        cache.insert(std::make_pair(tag, ret));

        return ret;
    }
};

} // namespace NLPCommon

#endif /* POLISH_TAGSET_CONVERT_H_ */
