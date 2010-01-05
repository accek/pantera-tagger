/*
 * category.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef CATEGORY_H_
#define CATEGORY_H_

#include <boost/unordered_map.hpp>
#include <string>
#include <vector>
#include "exception.h"

namespace BTagger {

using std::string;
using std::vector;
using boost::unordered_map;

class Category;

class CategoryValueNotFoundException : public Exception
{
public:
    CategoryValueNotFoundException(const string& msg) : Exception(msg) { }
    virtual ~CategoryValueNotFoundException() throw () { }

    static CategoryValueNotFoundException ById(const Category& cat,
            unsigned int id);
    static CategoryValueNotFoundException ByValue(const Category& cat,
            const string& name);
};

class Category
{
private:
    string _name;
    vector<string> _valuesByIndex;
    unordered_map<string, unsigned int> _indexesByValue;

    void init() {
        addValue("[none]");
    }

public:
    Category(const string& name) : _name(name) { init(); }

    Category(const string& name, const vector<string>& values)
        : _name(name) {
        init();
        for (vector<string>::const_iterator i = values.begin();
                i != values.end(); ++i)
            addValue(*i);
    }

    const string& getName() const { return _name; }

    int getIndex(const string& value) const {
        unordered_map<string, unsigned int>::const_iterator i =
                _indexesByValue.find(value);
        if (i == _indexesByValue.end())
            throw CategoryValueNotFoundException::ByValue(*this, value);
        return i->second;
    }

    bool hasValue(const string& value) const {
        return _indexesByValue.find(value) != _indexesByValue.end();
    }

    string getValue(unsigned int id) const {
        if (id < 0 || id >= _valuesByIndex.size())
            throw CategoryValueNotFoundException::ById(*this, id);
        return _valuesByIndex[id];
    }

    unsigned int addValue(const string& value) {
        unsigned int index = _valuesByIndex.size();
        _valuesByIndex.push_back(value);
        _indexesByValue[value] = index;
        return index;
    }
};

} // namespace BTagger

#endif /* CATEGORY_H_ */
