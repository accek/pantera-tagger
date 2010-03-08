/*
 * tagset.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef TAGSET_H_
#define TAGSET_H_

#include <vector>
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/unordered_map.hpp>
#include <string>
#include <locale>
#include <nlpcommon/exception.h>
#include <nlpcommon/category.h>
#include <nlpcommon/pos.h>

namespace NLPCommon {

using std::vector;
using std::string;
using boost::unordered_map;

class CategoryNotFoundException : public Exception
{
public:
    CategoryNotFoundException(const string& msg) : Exception(msg) { }
    virtual ~CategoryNotFoundException() throw () { }

    static CategoryNotFoundException ById(unsigned int id);
    static CategoryNotFoundException ByName(const string& name);
};

class PartOfSpeechNotFoundException : public Exception
{
public:
    PartOfSpeechNotFoundException(const string& msg) : Exception(msg) { }
    virtual ~PartOfSpeechNotFoundException() throw () { }

    static PartOfSpeechNotFoundException ById(unsigned int id);
    static PartOfSpeechNotFoundException ByName(const string& name);
};


class Tagset {
private:
    std::locale _locale;
    vector<const Category*> _categoriesByIndex;
    unordered_map<string, const Category*> _categoriesByName;
    unordered_map<const Category*, unsigned int> _categoriesIndex;
    vector<const PartOfSpeech*> _posByIndex;
    unordered_map<string, const PartOfSpeech*> _posByName;

    friend class boost::serialization::access;

    template <class Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar << _locale.name();
        ar << _categoriesByIndex;
        ar << _posByIndex;
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int version) {
        string locale_name;
        ar >> locale_name;
        _locale = std::locale(locale_name);

        vector<const Category*> categories;
        ar >> categories;
        _categoriesByIndex.clear();
        _categoriesByName.clear();
        _categoriesIndex.clear();
        BOOST_FOREACH(const Category* cat, categories)
            addCategory(cat);

        vector<const PartOfSpeech*> poss;
        ar >> poss;
        _posByIndex.clear();
        _posByName.clear();
        BOOST_FOREACH(const PartOfSpeech* pos, poss)
            addPartOfSpeech(pos);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
    const vector<const Category*>& getCategories() const {
        return _categoriesByIndex;
    }

    const Category* getCategory(unsigned int index) const {
        if (index < 0 || index >= _categoriesByIndex.size())
            throw CategoryNotFoundException::ById(index);
        return _categoriesByIndex[index];
    }

    const Category* getCategory(const string& name) const {
        unordered_map<string, const Category*>::const_iterator i =
                _categoriesByName.find(name);
        if (i == _categoriesByName.end())
            throw CategoryNotFoundException::ByName(name);
        return i->second;
    }

    unsigned int addCategory(const Category* category) {
        unsigned int index = _categoriesByIndex.size();
        _categoriesByName[category->getName()] = category;
        _categoriesByIndex.push_back(category);
        _categoriesIndex[category] = index;
        return index;
    }

    unsigned int getCategoryIndex(const Category* category) const {
        return _categoriesIndex.find(category)->second;
    }

    const vector<const PartOfSpeech*>& getPartsOfSpeech() const {
        return _posByIndex;
    }

    const PartOfSpeech* getPartOfSpeech(unsigned int index) const {
        if (index < 0 || index >= _posByIndex.size())
            throw PartOfSpeechNotFoundException::ById(index);
        return _posByIndex[index];
    }

    const PartOfSpeech* getPartOfSpeech(const string& name) const {
        unordered_map<string, const PartOfSpeech*>::const_iterator i =
                _posByName.find(name);
        if (i == _posByName.end())
            throw PartOfSpeechNotFoundException::ByName(name);
        return i->second;
    }

    unsigned int getPartOfSpeechIndex(const PartOfSpeech* pos) const {
        vector<const PartOfSpeech*>::const_iterator i =
                std::find(_posByIndex.begin(), _posByIndex.end(), pos);
        if (i == _posByIndex.end())
            throw CategoryValueNotFoundException("Part of speech does not "
                    "belong to the tagset.");
        return i - _posByIndex.begin();
    }

    unsigned int addPartOfSpeech(const PartOfSpeech* pos) {
        unsigned int index = _posByIndex.size();
        _posByName[pos->getName()] = pos;
        _posByIndex.push_back(pos);
        return index;
    }

    void setLocale(const std::locale& locale) {
        _locale = locale;
    }

    const std::locale& getLocale() const {
        return _locale;
    }

};

} // namespace NLPCommon

#endif /* TAGSET_H_ */
