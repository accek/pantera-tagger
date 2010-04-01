/*
 * unigram.h
 *
 *  Created on: Jan 3, 2010
 *      Author: accek
 */

#ifndef UNIGRAM_H_
#define UNIGRAM_H_

#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>
#include "../third_party/serialization/unordered_map.hpp"
#include <string>

#include <nlpcommon/lexeme.h>

namespace BTagger {

using namespace NLPCommon;

using std::string;
using std::wstring;
using boost::unordered_map;

template<class Lexeme>
class UnigramTagger
{
public:
    typedef typename Lexeme::tag_type tag_type;

private:
    unordered_map<string, unordered_map<tag_type, int> > _freq;
    unordered_map<string, int> _count;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & _freq;
        ar & _count;
    }

    static const int MIN_COUNT = 5;

public:
    void clear() {
        _freq.clear();
        _count.clear();
    }

    void train(const Lexeme& lexeme, const Tagset* projection_tagset = NULL) {
        const string key = lexeme.getUtf8Orth();
        _count[key]++;
        BOOST_FOREACH(const tag_type& tag, lexeme.getGoldenTags()) {
            _freq[key][tag.project(projection_tagset)]++;
            _freq["[all_tags]"][tag.project(projection_tagset)]++;
        }
    }

    void train(const vector<Lexeme>& lexemes,
            const Tagset* projection_tagset = NULL) {
        BOOST_FOREACH(const Lexeme& lexeme, lexemes)
            train(lexeme, projection_tagset);
    }

    int getFrequency(const wstring& str, const tag_type& tag) const {
        return getFrequency(wstring_to_utf8(str), tag);
    }

    int getFrequency(const string& str, const tag_type& tag) const {
        typename unordered_map<string, unordered_map<tag_type, int> >::const_iterator i
            = _freq.find(str);
        typename unordered_map<string, int>::const_iterator c
            = _count.find(str);
        if (i == _freq.end() || c == _count.end() || c->second < MIN_COUNT)
            i = _freq.find("[all_tags]");
        typename unordered_map<tag_type, int>::const_iterator j = i->second.find(tag);
        if (j == i->second.end())
            return 0;
        return j->second;
    }

    tag_type getBestTag(const wstring& string, const tag_type& default_tag) {
        return getBestTag(wstring_to_utf8(string), default_tag);
    }

    tag_type getBestTag(const string& string, const tag_type& default_tag) {
        typedef std::pair<tag_type, int> entry_type;
        entry_type best = std::make_pair(tag_type(), -1);
        unordered_map<tag_type, int>& freqs = _freq[string];
        if (freqs.empty())
            return default_tag;
        BOOST_FOREACH(const entry_type& entry, freqs) {
            if (entry.second > best.second)
                best = entry;
        }
        return best.first;
    }
};

} // namespace BTagger

#endif /* UNIGRAM_H_ */
