/*
 * lexeme.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LEXEME_H_
#define LEXEME_H_

#include <boost/foreach.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <ostream>
#include <iomanip>

#include <nlpcommon/tag.h>

namespace NLPCommon {

using std::wstring;
using std::vector;

class LexerData {
protected:
    LexerData() { }
public:
    virtual ~LexerData() { }
};

template<class Tag>
class Lexeme
{
public:
    enum Type {
        SEGMENT = 0x01,
        NO_SPACE = 0x02,
        START_OF_SENTENCE = 0x04,
        START_OF_PARAGRAPH = 0x08,
        START_OF_FILE = 0x10,
        END_OF_FILE = 0x20,
        END_OF_PARAGRAPH = 0x40,
        END_OF_SENTENCE = 0x80,

        // Example of ambiguous segmentation coding.
        //
        // [START_OF_AMBIGUITY]
        //   [ACCEPTED_FRAGMENT]
        //     [SEGMENT gdzie]
        //     [NO_SPACE]
        //     [SEGMENT ś]
        //   [REJECTED_FRAGMENT]
        //     [SEGMENT gdzieś]
        // [END_OF_AMBIGUITY]

        START_OF_AMBIGUITY = 0x100,
        UNRESOLVED_FRAGMENT = 0x200,
        ACCEPTED_FRAGMENT = 0x400,
        REJECTED_FRAGMENT = 0x800,
        END_OF_AMBIGUITY = 0x1000,
    };

private:
    wstring _orth;
    vector<Tag> _allowed_tags;
    vector<Tag> _golden_tags;
    vector<Tag> _autoselected_tags;
    vector<std::pair<Tag, wstring> > _tag_bases;
    Type _type;
    boost::shared_ptr<LexerData> _lexer_data;

    friend class boost::serialization::access;

    template <class Archive>
    void save(Archive& ar, const unsigned int version) const {
        string orth = this->getUtf8Orth();
        ar & orth;
        ar & _allowed_tags;
        ar & _golden_tags;
        ar & _autoselected_tags;
        ar & _type;
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int version) {
        string utf8_orth;
        ar & utf8_orth;
        this->setUtf8Orth(utf8_orth);
        ar & _allowed_tags;
        ar & _golden_tags;
        ar & _autoselected_tags;
        ar & _type;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

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
        return wstring_to_utf8(_orth);
    }

    void setUtf8Orth(const string& orth) {
        setOrth(utf8_to_wstring(orth));
    }

    void setOrth(const wstring& orth) {
        _orth = orth;
    }

    void addAllowedTag(const Tag& tag) {
        if (!isAllowedTag(tag))
            _allowed_tags.push_back(tag);
    }

    void setAllowedTags(const vector<Tag>& tags) {
        _allowed_tags = tags;
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

    void setGoldenTags(const vector<Tag>& tags) {
        _golden_tags = tags;
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

    void setAutoselectedTags(const vector<Tag>& tags) {
        _autoselected_tags = tags;
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

    const vector<std::pair<Tag, wstring> >& getTagBases() const {
       return _tag_bases;
    }

    void addTagBase(const Tag& tag, const wstring& base) {
        if (std::find(_tag_bases.begin(), _tag_bases.end(),
                    std::make_pair(tag, base)) == _tag_bases.end())
            _tag_bases.push_back(std::make_pair(tag, base));
    }

    void writeToStreamWithTags(std::wostream& stream, const Tagset* tagset) const {
        stream << std::setw(ORTH_DISPLAY_WIDTH) << getOrth();
        writeTagsDifference(stream, tagset,
                getAutoselectedTags(), getGoldenTags());
        stream << std::endl;
    }

    LexerData* getLexerData() const {
        return _lexer_data.get();
    }

    void setLexerData(LexerData* data) {
        _lexer_data.reset(data);
    }
};

typedef Lexeme<Tag> DefaultLexeme;

class StringLexerData : public LexerData {
protected:
    string _str;

public:
    StringLexerData(const string& str)
        : LexerData(), _str(str)
    {
    }

    const string& getString() const { return _str; }
};

} // namespace NLPCommon

#endif /* LEXEME_H_ */
