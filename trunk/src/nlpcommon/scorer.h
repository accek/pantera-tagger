#ifndef SCORER_H_
#define SCORER_H_

#include <boost/concept_check.hpp>
#include <boost/foreach.hpp>
#include <limits>
#include <nlpcommon/tag.h>

namespace NLPCommon {

/* Valid type of score is anything with + and -, comparisons, assignment,
 * default constructor, overloads for std::numeric_limits, conversion to
 * double, comparison with 0.
 *
 * A Scorer is any class meeting the following interface.
 *
 * - it has types score_type and tag_type defined.
 *
 * - score_type must have overloads for std::numeric_limits.
 *
 * - it has the following public methods:
 *
 *   constructor(const Tagset* tagset);
 *   score_t maxScore();
 *     // score for exact match
 *   score_t score(const tag_type& assigned, const tag_type& golden);
 *     // should return a value between score_type() and maxScore().
 *
 * A MultiGoldenScorer additionaly has the following method:
 *
 *   score_t score(const tag_type& assigned, const vector<tag_type>& golden);
 *
 * A MultiScorer additionaly has the following method:
 *
 *   score_t score(const vector<tag_type>& assigned, const vector<tag_type>& golden);
 *
 *
 * There are concept checking classes defined below. For more information
 * about concepts and concept checking in C++, see
 * http://www.boost.org/doc/libs/1_42_0/libs/concept_check/concept_check.htm
 */

/* Concept checking classes. */

template <class X>
struct ScoreConcept : boost::DefaultConstructible<X>, boost::Assignable<X>
{
public:
    BOOST_CONCEPT_USAGE(ScoreConcept) {
        x == y;
        x != y;
        x < y;
        x <= y;
        x > y;
        x >= y;
        x == 0;
        x != 0;
        x < 0;
        x <= 0;
        x > 0;
        x >= 0;
        x + y;
        x - y;
        -x;
        std::numeric_limits<X>::min();
        std::numeric_limits<X>::max();
        (double)x;
    }

private:
    X x, y;
};

template <class X>
struct ScorerConcept
{
public:
    typedef typename X::score_type score_type;
    typedef typename X::tag_type tag_type;

    BOOST_CONCEPT_ASSERT((ScoreConcept<score_type>));

    BOOST_CONCEPT_USAGE(ScorerConcept) {
        X x(tagset);
        score_type s = x.score(tag, tag);
        s = x.maxScore();
    }

private:
    const Tagset* tagset;
    tag_type tag;
};

template <class X>
struct MultiGoldenScorerConcept : ScorerConcept<X>
{
public:
    typedef typename ScorerConcept<X>::tag_type tag_type;
    typedef typename ScorerConcept<X>::score_type score_type;

    BOOST_CONCEPT_USAGE(MultiGoldenScorerConcept) {
        score_type s = x.score(tag, tags);
    }

private:
    X x;
    tag_type tag;
    vector<tag_type>& tags;
};

template <class X>
struct MultiScorerConcept : MultiGoldenScorerConcept<X>
{
    typedef typename MultiGoldenScorerConcept<X>::tag_type tag_type;
    typedef typename MultiGoldenScorerConcept<X>::score_type score_type;

    BOOST_CONCEPT_USAGE(MultiScorerConcept) {
        score_type s = x.score(tags, tags);
    }

private:
    X x;
    vector<tag_type>& tags;
};

/* Implementations. */

template <class Tag = Tag>
class BinaryScorer
{
public:
    typedef int score_type;
    typedef Tag tag_type;

    BinaryScorer(const Tagset* tagset) { }

    score_type maxScore() {
        return 1;
    }

    score_type score(const tag_type& assigned, const tag_type& golden) {
        return assigned == golden;
    }
};

template <class SingleScorer = BinaryScorer<> >
class BestScoreMultiGoldenScorer
{
    BOOST_CONCEPT_ASSERT((ScorerConcept<SingleScorer>));

private:
    SingleScorer _single_scorer;

public:
    typedef typename SingleScorer::score_type score_type;
    typedef typename SingleScorer::tag_type tag_type;

    BestScoreMultiGoldenScorer(const Tagset* tagset)
        : _single_scorer(tagset) { }

    score_type maxScore() {
        return _single_scorer.maxScore();
    }

    score_type score(const tag_type& assigned, const tag_type& golden) {
        return _single_scorer.score(assigned, golden);
    }

    score_type score(const tag_type& assigned, const vector<tag_type>& golden) {
        score_type best_s = std::numeric_limits<score_type>::min();
        BOOST_FOREACH(const tag_type& g, golden) {
            score_type s = score(assigned, g);
            if (s > best_s)
                best_s = s;
        }
        return best_s;
    }
};

} // namespace NLPCommon

#endif /* SCORER_H_ */
