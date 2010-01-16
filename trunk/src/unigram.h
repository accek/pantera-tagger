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
#include "lexeme.h"

namespace BTagger {

using boost::unordered_map;

template<class Lexeme>
class UnigramTagger
{
public:
    typedef typename Lexeme::tag_type tag_type;

private:
    unordered_map<string, unordered_map<tag_type, int> > _freq;

public:
    void clear() {
        _freq.clear();
    }

    void train(const Lexeme& lexeme, const Tagset* projection_tagset = NULL) {
        BOOST_FOREACH(const tag_type& tag, lexeme.getCorrectTags())
            _freq[lexeme.getOrth()][tag.project(projection_tagset)]++;
    }

    void train(const vector<Lexeme>& lexemes,
            const Tagset* projection_tagset = NULL) {
        BOOST_FOREACH(const Lexeme& lexeme, lexemes)
            train(lexeme, projection_tagset);
    }

    int getFrequency(const string& str, const tag_type& tag) const {
        typename unordered_map<string, unordered_map<tag_type, int> >::const_iterator i
            = _freq.find(str);
        if (i == _freq.end())
            return 0;
        typename unordered_map<tag_type, int>::const_iterator j = i->second.find(tag);
        if (j == i->second.end())
            return 0;
        return j->second;
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
