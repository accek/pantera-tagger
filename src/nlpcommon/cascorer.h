#ifndef CASCORER_H_
#define CASCORER_H_

// Category-aware scorer

#include <iostream>

#include <boost/foreach.hpp>
#include <string>
#include <vector>

#include <nlpcommon/scorer.h>
#include <nlpcommon/category-weights.h>

namespace NLPCommon {

using std::string;
using std::vector;

using std::wcerr;
using std::endl;

template <bool MatchAllCategories = false, class Tag = Tag>
class CAScorer
{
public:
    typedef double score_type;
    typedef Tag tag_type;

private:
    bool initialized;

protected:
    vector<double> pos_weights;
    vector<vector<double> > cat_weights;
    const Tagset* tagset;

    void init(bool normalize_cw = true) {
        if (initialized)
            return;

        cat_weights.resize(tagset->getPartsOfSpeech().size());
        int i = -1;
        BOOST_FOREACH(const PartOfSpeech* pos, tagset->getPartsOfSpeech()) {
            i++;

            const vector<const Category*> cats =
                MatchAllCategories ?
                    tagset->getCategories() : pos->getCategories();
            double pw = this->getPosRelativeWeight();

            double sum_cw = 0.0;
            BOOST_FOREACH(const Category* cat, cats)
                sum_cw += this->getCategoryRelativeWeight(pos, cat);

            if (sum_cw == 0.0) {
                pos_weights.push_back(1.0);
                BOOST_FOREACH(const Category* cat, cats)
                    cat_weights[i].push_back(0.0);
                continue;
            }

            double cw_mult = normalize_cw ? (1.0 - pw) / sum_cw : 1.0;
            vector<double> cw_vect;
            BOOST_FOREACH(const Category* cat, cats)
                cat_weights[i].push_back(
                        this->getCategoryRelativeWeight(pos, cat) * cw_mult);
            pos_weights.push_back(pw);
        }
        initialized = true;
    }

    virtual double getPosRelativeWeight() {
        return 0.5;
    }

    virtual double getCategoryRelativeWeight(const PartOfSpeech* pos,
            const Category* category) {
        return 1.0;
    }

    virtual double getNonExactMatchFactor() {
        return 0.5;
    }

public:
    CAScorer(const Tagset* tagset)
            : tagset(tagset), initialized(false) { }

    double maxScore() {
        return 1.0;
    }

    bool isLessThanEpsilon(double value) {
        return value < 1.0e-3;
    }

    double score(const tag_type& assigned, const tag_type& golden) {
        this->init();

        if (assigned == golden)
            return 1.0;
        if (golden == tag_type::getNullTag()) {
            return 0.0;
        }

        int gpos_idx = golden.getPos();
        int apos_idx = assigned.getPos();
        const PartOfSpeech* gpos = tagset->getPartOfSpeech(gpos_idx);
        const PartOfSpeech* apos = tagset->getPartOfSpeech(apos_idx);
        double result = (apos_idx == gpos_idx) * pos_weights[gpos_idx];
        const vector<double>& cw_vect = cat_weights[gpos_idx];
        const vector<const Category*> cats =
            MatchAllCategories ?
                tagset->getCategories() : gpos->getCategories();

        int i = 0;
        BOOST_FOREACH(const Category* cat, cats) {
            int cat_idx = tagset->getCategoryIndex(cat);
            if (apos == gpos || apos->hasCategory(cat) || MatchAllCategories) {
                result += (assigned.getValue(cat_idx) ==
                        golden.getValue(cat_idx)) * cw_vect[i];
            }
            i++;
        }

        return result * getNonExactMatchFactor();
    }
};

template <class CategoryWeights, bool MatchAllCategories = true,
         class Tag = Tag>
class APScorer : public CAScorer<MatchAllCategories, Tag>,
                 CategoryWeightsStore<CategoryWeights>
{
protected:
    double getPosRelativeWeight() {
        return this->getCategoryWeight("pos") / this->getSumOfCategoryWeights();
    }

    double getCategoryRelativeWeight(const PartOfSpeech* pos,
            const Category* category) {
        return this->getCategoryWeight(category->getName());
    }

    double getNonExactMatchFactor() {
        return 1.0;
    }

public:
    APScorer(const Tagset* tagset)
        : CAScorer<MatchAllCategories, Tag>(tagset),
          CategoryWeightsStore<CategoryWeights>() { }
};

template <class Tag = Tag>
class PAScorer : public CAScorer<true, Tag>
{
protected:
    double getPosRelativeWeight() {
        return 1.0;
    }

    double getCategoryRelativeWeight(const PartOfSpeech* pos,
            const Category* category) {
        return 1.0;
    }

    double getNonExactMatchFactor() {
        return 1.0;
    }

public:
    typedef typename CAScorer<true, Tag>::tag_type tag_type;

    PAScorer(const Tagset* tagset)
        : CAScorer<true, Tag>(tagset) { }

    double score(const tag_type& assigned, const tag_type& golden) {
        this->init(false);

        if (assigned == golden)
            return 1.0;
        if (golden == tag_type::getNullTag()) {
            return 0.0;
        }

        int gpos_idx = golden.getPos();
        int apos_idx = assigned.getPos();
        const PartOfSpeech* gpos = this->tagset->getPartOfSpeech(gpos_idx);
        const PartOfSpeech* apos = this->tagset->getPartOfSpeech(apos_idx);
        double pos_weight = this->pos_weights[gpos_idx];
        const vector<double>& cw_vect = this->cat_weights[gpos_idx];
        const vector<const Category*> cats = this->tagset->getCategories();

        double na = pos_weight, ng = pos_weight, tp = 0.0;

        if (gpos_idx == apos_idx)
            tp += pos_weight;

        int i = 0;
        BOOST_FOREACH(const Category* cat, cats) {
            int cat_idx = this->tagset->getCategoryIndex(cat);
            double weight = cw_vect[i];
            assert(weight != 0.0);

            bool match = true;
            if (apos->hasCategory(cat) != gpos->hasCategory(cat))
                match = false;
            else if (apos->hasCategory(cat)
                    && assigned.getValue(cat_idx) != golden.getValue(cat_idx))
                match = false;

            if (apos->hasCategory(cat)) {
                if (match)
                    tp += weight;
                na += weight;
            }
            if (gpos->hasCategory(cat))
                ng += weight;

            i++;
        }

        double p = tp / na;
        double r = tp / ng;
        double f = p + r == 0.0 ? 0.0 : 2 * p * r / (p + r);

        return f * this->getNonExactMatchFactor();
    }
};

template <class CategoryWeights, class Tag = Tag>
class WPAScorer : public PAScorer<Tag>,
                 CategoryWeightsStore<CategoryWeights>
{
protected:
    double getPosRelativeWeight() {
        return this->getCategoryWeight("pos") / this->getSumOfCategoryWeights();
    }

    double getCategoryRelativeWeight(const PartOfSpeech* pos,
            const Category* category) {
        return this->getCategoryWeight(category->getName())
            / this->getSumOfCategoryWeights();
    }

    double getNonExactMatchFactor() {
        return 1.0;
    }

public:
    WPAScorer(const Tagset* tagset)
        : PAScorer<Tag>(tagset),
          CategoryWeightsStore<CategoryWeights>() { }
};

} // namespace NLPCommon

#endif /* CASCORER_H_ */
