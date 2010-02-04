/*
 * lexeme.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LEXEME_H_
#define LEXEME_H_

#include <boost/foreach.hpp>
#include <string>
#include <vector>

#include <nlpcommon/tag.h>

namespace NLPCommon {

using std::string;
using std::vector;

template<class Tag>
class Lexeme
{
public:
    enum Type {
        SEGMENT = 0x01,
        NO_SPACE = 0x02,
        START_OF_SENTENCE = 0x04,
        END_OF_SENTENCE = 0x08,
        START_OF_PARAGRAPH = 0x0a,
        END_OF_PARAGRAPH = 0x10
    };

private:
    string _orth;
    vector<Tag> _allowed_tags;
    vector<Tag> _golden_tags;
    vector<Tag> _autoselected_tags;
    Type _type;

public:
    typedef Tag tag_type;

    Lexeme(Type type = SEGMENT) : _type(type) { }

    static Lexeme<Tag> getNullLexeme() {
        Lexeme<Tag> lexeme;
        lexeme.setOrth("[null]");
        lexeme.addAllowedTag(Tag::getNullTag());
        lexeme.addGoldenTag(Tag::getNullTag());
        return lexeme;
    }

    Type getType() const {
        return _type;
    }

    void setType(Type type) {
        _type = type;
    }

    const string& getOrth() const {
        return _orth;
    }

    void setOrth(const string& orth) {
        assert(_orth.empty());
        _orth = orth;
    }

    void addAllowedTag(const Tag& tag) {
        if (!isAllowedTag(tag))
            _allowed_tags.push_back(tag);
    }

    bool isAllowedTag(const Tag& tag) const {
        return std::find(_allowed_tags.begin(), _allowed_tags.end(), tag)
                != _allowed_tags.end();
    }

    void containsAllowedTag(const vector<Tag>& tags) const {
        BOOST_FOREACH(const Tag& tag, tags)
            if (isAllowedTag(tag))
                return true;
        return false;
    }

    const vector<Tag>& getAllowedTags() const {
        return _allowed_tags;
    }

    void addGoldenTag(const Tag& tag) {
        if (!isGoldenTag(tag))
            _golden_tags.push_back(tag);
    }

    bool isGoldenTag(const Tag& tag) const {
        return std::find(_golden_tags.begin(), _golden_tags.end(), tag)
                != _golden_tags.end();
    }

    void containsGoldenTag(const vector<Tag>& tags) const {
        BOOST_FOREACH(const Tag& tag, tags)
            if (isGoldenTag(tag))
                return true;
        return false;
    }

    const vector<Tag>& getGoldenTags() const {
        return _golden_tags;
    }

    void addAutoselectedTag(const Tag& tag) {
        if (!isAutoselectedTag(tag))
            _autoselected_tags.push_back(tag);
    }

    bool isAutoselectedTag(const Tag& tag) const {
        return std::find(_autoselected_tags.begin(), _autoselected_tags.end(), tag)
                != _autoselected_tags.end();
    }

    void containsAutoselectedTag(const vector<Tag>& tags) const {
        BOOST_FOREACH(const Tag& tag, tags)
            if (isAutoselectedTag(tag))
                return true;
        return false;
    }

    const vector<Tag>& getAutoselectedTags() const {
        return _autoselected_tags;
    }

};

typedef Lexeme<Tag> DefaultLexeme;

} // namespace NLPCommon

#endif /* LEXEME_H_ */
