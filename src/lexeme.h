/*
 * lexeme.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LEXEME_H_
#define LEXEME_H_

#include <string>
#include <vector>

namespace BTagger {

using std::string;
using std::vector;

template<class Tag>
class Lexeme
{
private:
    string _orth;
    vector<Tag> _allowed_tags;
    vector<Tag> _correct_tags;

public:
    typedef Tag tag_type;

    Lexeme() { }

    /*static Lexeme<Tag> getNullLexeme() {
        Lexeme<Tag> lexeme;
        lexeme.setOrth("[null]");
        lexeme.addAllowedTag(Tag::getNullTag());
        lexeme.addCorrectTag(Tag::getNullTag());
        return lexeme;
    }*/

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
        for (typename vector<Tag>::iterator i = tags.begin();
                i != tags.end(); ++i)
            if (isAllowedTag(*i))
                return true;
        return false;
    }

    const vector<Tag>& getAllowedTags() const {
        return _allowed_tags;
    }

    void addCorrectTag(const Tag& tag) {
        if (!isCorrectTag(tag))
            _correct_tags.push_back(tag);
    }

    bool isCorrectTag(const Tag& tag) const {
        return std::find(_correct_tags.begin(), _correct_tags.end(), tag)
                != _correct_tags.end();
    }

    void containsCorrectTag(const vector<Tag>& tags) const {
        for (typename vector<Tag>::iterator i = tags.begin();
                i != tags.end(); ++i)
            if (isCorrectTag(*i))
                return true;
        return false;
    }

    const vector<Tag>& getCorrectTags() const {
        return _correct_tags;
    }

};

} // namespace BTagger

#endif /* LEXEME_H_ */
