/*
 * lexeme.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LEXEME_H_
#define LEXEME_H_

#include <boost/program_options/detail/convert.hpp>
#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <vector>
#include <ostream>
#include <iomanip>

#include <nlpcommon/tag.h>

namespace NLPCommon {

using std::wstring;
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
    wstring _orth;
    vector<Tag> _allowed_tags;
    vector<Tag> _golden_tags;
    vector<Tag> _autoselected_tags;
    Type _type;

public:
    static const int ORTH_DISPLAY_WIDTH = 25;

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

    const wstring& getOrth() const {
        return _orth;
    }

    string getUtf8Orth() const {
        boost::program_options::detail::utf8_codecvt_facet utf8_facet;
        return boost::to_8_bit(_orth, utf8_facet);
    }

    void setOrth(const wstring& orth) {
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
        return std::find(_autoselected_tags.begin(), _autoselected_tags.end(),
                tag) != _autoselected_tags.end();
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

    void setAutoselectedTag(const Tag& tag) {
        _autoselected_tags.clear();
        _autoselected_tags.push_back(tag);
    }

    void writeToStreamWithTags(std::wostream& stream, const Tagset* tagset) const {
        stream << std::setw(ORTH_DISPLAY_WIDTH) << getOrth();
        writeTagsDifference(stream, tagset,
                getAutoselectedTags(), getGoldenTags());
        stream << std::endl;
    }
};

typedef Lexeme<Tag> DefaultLexeme;

} // namespace NLPCommon

#endif /* LEXEME_H_ */
