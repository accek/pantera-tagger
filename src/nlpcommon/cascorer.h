#ifndef CASCORER_H_
#define CASCORER_H_

// Category-aware scorer

#include <boost/foreach.hpp>
#include <vector>
#include <nlpcommon/scorer.h>

namespace NLPCommon {

using std::vector;

template <class Tag = Tag>
class CAScorer
{
public:
    typedef float score_type;
    typedef Tag tag_type;

private:
    vector<score_type> pos_weights;
    vector<vector<score_type> > cat_weights;
    const Tagset* tagset;

protected:
    score_type getPosRelativeWeight() {
        return 0.5;
    }

    score_type getCategoryRelativeWeight(const PartOfSpeech* pos,
            const Category* category) {
        return 1.0;
    }

public:
    CAScorer(const Tagset* tagset)
            : tagset(tagset)
    {
        cat_weights.resize(tagset->getPartsOfSpeech().size());
        int i = -1;
        BOOST_FOREACH(const PartOfSpeech* pos, tagset->getPartsOfSpeech()) {
            i++;

            const vector<const Category*> cats = pos->getCategories();
            score_type pw = this->getPosRelativeWeight();
            pos_weights.push_back(pw);

            score_type sum_cw = 0.0;
            BOOST_FOREACH(const Category* cat, cats)
                sum_cw += this->getCategoryRelativeWeight(pos, cat);

            if (sum_cw == 0.0) {
                pos_weights.push_back(1.0);
                BOOST_FOREACH(const Category* cat, cats)
                    cat_weights[i].push_back(0.0);
                continue;
            }

            score_type cw_mult = (1.0 - pw) / sum_cw;
            vector<score_type> cw_vect;
            BOOST_FOREACH(const Category* cat, cats)
                cat_weights[i].push_back(
                        this->getCategoryRelativeWeight(pos, cat) * cw_mult);
            pos_weights.push_back(pw);
        }
    }

    score_type maxScore() {
        return 1.0;
    }

    score_type score(const tag_type& assigned, const tag_type& golden) {
        if (assigned == golden)
            return 1.0;
        if (golden == tag_type::getNullTag())
            return 0.0;

        int gpos_idx = golden.getPos();
        int apos_idx = assigned.getPos();
        const PartOfSpeech* gpos = tagset->getPartOfSpeech(gpos_idx);
        const PartOfSpeech* apos = tagset->getPartOfSpeech(apos_idx);
        score_type result = (apos_idx == gpos_idx) * pos_weights[gpos_idx];
        const vector<score_type>& cw_vect = cat_weights[gpos_idx];
        const vector<const Category*>& cats = gpos->getCategories();

        int i = 0;
        BOOST_FOREACH(const Category* cat, cats) {
            int cat_idx = tagset->getCategoryIndex(cat);
            if (apos == gpos || apos->hasCategory(cat))
                result += (assigned.getValue(cat_idx) ==
                        golden.getValue(cat_idx)) * cw_vect[i];
            i++;
        }

        return result;
    }
};

} // namespace NLPCommon

#endif /* CASCORER_H_ */
