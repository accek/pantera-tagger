/*
 * brilllexeme.h
 *
 *  Created on: Jan 3, 2010
 *      Author: accek
 */

#ifndef BRILLLEXEME_H_
#define BRILLLEXEME_H_

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/foreach.hpp>
#include <nlpcommon/lexeme.h>

namespace BTagger {

using namespace NLPCommon;

template<class Tag>
class BrillLexeme : public Lexeme<Tag>
{
public:
    static const int num_phases = 4;

    // Tags considered by Brill algorithm
    vector<Tag> considered_tags;
    // Tag chosen by Brill
    Tag chosen_tag[num_phases];
    // Correct tag for current phase of the algorithm.
    Tag expected_tag;
    // Vicinity parameter for marking nearby changes
    int vicinity;

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::base_object<Lexeme<Tag> >(*this);
        ar & considered_tags;
        ar & chosen_tag;
        ar & expected_tag;
        ar & vicinity;
    }

public:
    BrillLexeme(typename Lexeme<Tag>::Type type = Lexeme<Tag>::SEGMENT)
        : Lexeme<Tag>(type) { }

    static BrillLexeme<Tag> getNullLexeme() {
        BrillLexeme<Tag> lexeme;
        lexeme.setOrth(L"[null]");
        lexeme.addAllowedTag(Tag::getNullTag());
        lexeme.addGoldenTag(Tag::getNullTag());
        lexeme.considered_tags.push_back(Tag::getNullTag());
        lexeme.expected_tag = Tag::getNullTag();
        lexeme.vicinity = 0;
        BOOST_FOREACH(Tag& t, lexeme.chosen_tag)
            t = Tag::getNullTag();
        return lexeme;
    }

    bool isConsideredTag(const Tag& tag) const {
        return std::find(considered_tags.begin(), considered_tags.end(), tag) !=
                considered_tags.end();
    }

};

} // namespace BTagger

#endif /* BRILLLEXEME_H_ */
