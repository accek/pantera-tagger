/*
 * brillengine.h
 *
 *  Created on: Jan 3, 2010
 *      Author: accek
 */

#ifndef BRILLENGINE_H_
#define BRILLENGINE_H_

#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <cassert>
#include <limits>
#include <algorithm>
#include "unigram.h"
#include "rules.h"

namespace BTagger {

using std::cerr;
using std::cerr;
using std::endl;
using std::vector;
using std::map;

template<class Lexeme>
class BrillEngine
{
public:
    typedef typename Lexeme::tag_type tag_type;

private:
    typedef boost::unordered_map<Rule<Lexeme>, std::pair<int, int> > scores_map_type;

    UnigramTagger<Lexeme> unigram_tagger;
    const Tagset* full_tagset;
    vector<Lexeme> text;
    vector<Lexeme> next_text;
    int phase;
    vector<const Tagset*> phase_tagsets;
    const Tagset* phase_tagset;
    vector<PredicateTemplate<Lexeme>*> rule_templates;
    scores_map_type scores;
    vector<Rule<Lexeme> > generated_rules;
    vector<vector<Rule<Lexeme> > > phase_generated_rules;
    bool cancelled;

    static const int INDEX_OFFSET = 4;
    static const int DBG = 0;
    static const int VICINITY = 3;


    static void applyRule(int phase, Rule<Lexeme>& b, vector<Lexeme>& text1,
            vector<Lexeme>& text2,
            int start_index = 0, int end_index = std::numeric_limits<int>::max(),
            int vicinity_value = 0, int vicinity_radius = 0) {
        assert(text1.size() == text2.size());
        start_index = std::min(start_index, (int)text1.size());
        end_index = std::min(end_index, (int)text1.size());
        #pragma omp parallel for default(shared)
        for (int i = start_index; i < end_index; ++i) {
            if (text1[i].isConsideredTag(b.tag)
                    && b.predicate.tpl
                    && b.predicate.tpl->predicateMatches(b.predicate, text1, i)) {
                text2[i].chosen_tag[phase] = b.tag;
                for (int j = std::max(start_index, i - vicinity_radius),
                         z = std::min(end_index - 1, i + vicinity_radius); j <= z; ++j) {
                    text2[j].vicinity = vicinity_value;
                }
            } else {
                text2[i].chosen_tag[phase] = text1[i].chosen_tag[phase];
            }
        }
    }

    static void calculateStats(int phase, vector<Lexeme>& text, double* prec,
            double* recall, double* f_measure, int* errors,
            int start_index = 0, int end_index = std::numeric_limits<int>::max()) {
        start_index = std::min(start_index, (int)text.size());
        end_index = std::min(end_index, (int)text.size());

        int tp = 0, tn = 0, fp = 0, fn = 0;
        int e = 0;

        #pragma omp parallel for \
            default(shared) \
            reduction(+: tp, tn, fp, fn, e)
        for (int i = start_index; i < end_index; ++i) {
            const Lexeme& lex = text[i];
            if (lex.chosen_tag[phase] == lex.expected_tag) {
                tp++;
                tn += lex.considered_tags.size() - 1;
            } else {
                fp++;
                fn++;
                tn += lex.considered_tags.size() - 2;
                e++;
            }
        }

        if (prec)
            *prec = tp / double(tp + fp);
        if (recall)
            *recall = tp / double(tp + fn);
        if (prec && recall && f_measure)
            *f_measure = 2 * *prec * *recall / (*prec + *recall);

        if (errors) *errors = e;
    }

    void calculateTaggingStats(vector<Lexeme>& text,
            double* prec,
            double* recall, double* f_measure, int* errors,
            int start_index = 0, int end_index = std::numeric_limits<int>::max()) {
        start_index = std::min(start_index, (int)text.size());
        end_index = std::min(end_index, (int)text.size());

        int tp = 0, tn = 0, fp = 0, fn = 0;
        int e = 0;

        for (int i = start_index; i < end_index; ++i) {
            const Lexeme& lex = text[i];
            int cs = lex.getCorrectTags().size();
            int as = lex.getAllowedTags().size();
            if (lex.isCorrectTag(lex.chosen_tag[phase])) {
                tp++;
                fn += cs - 1;
                tn += as - cs;
            } else {
                fp++;
                fn += cs;
                tn += as - cs - 1;
                e++;
            }
        }

        if (prec)
            *prec = tp / double(tp + fp);
        if (recall)
            *recall = tp / double(tp + fn);
        if (prec && recall && f_measure)
            *f_measure = 2 * *prec * *recall / (*prec + *recall);

        if (errors) *errors = e;
    }

    tag_type findBestInitialTag(Lexeme& lexeme,
            const Tagset* new_tagset, const Tagset* previous_tagset,
             const tag_type& previous_tag = tag_type::getNullTag())
    {
        const string& orth = lexeme.getOrth();

        tag_type best_tag;
        int best_freq = -1;
        BOOST_FOREACH(const tag_type& tag, lexeme.getAllowedTags()) {
            tag_type pprojected = tag.project(previous_tagset);
            if (pprojected == previous_tag || previous_tagset == NULL) {
                tag_type nprojected = tag.project(new_tagset);
                int freq = unigram_tagger.getFrequency(orth, nprojected);
                if (freq > best_freq) {
                    best_freq = freq;
                    best_tag = nprojected;
                }
            }
        }

        return best_tag;
    }

    vector<tag_type> calculateConsideredTags(Lexeme& lexeme,
            const Tagset* new_tagset, const Tagset* previous_tagset,
             const tag_type& previous_tag) {
        vector<tag_type> new_ctags;
        BOOST_FOREACH(const tag_type& tag, lexeme.getAllowedTags()) {
            tag_type projected = tag.project(previous_tagset);
            if (projected == previous_tag || previous_tagset == NULL)
                new_ctags.push_back(tag.project(new_tagset));
        }
        std::sort(new_ctags.begin(), new_ctags.end());
        new_ctags.resize(std::unique(new_ctags.begin(), new_ctags.end())
                - new_ctags.begin());
        return new_ctags;
    }

    tag_type findCorrectTag(Lexeme& lexeme,
            const Tagset* new_tagset, const Tagset* previous_tagset,
             const tag_type& previous_tag) {
        BOOST_FOREACH(const tag_type& tag, lexeme.getCorrectTags()) {
            tag_type projected = tag.project(previous_tagset);
            if (projected == previous_tag || previous_tagset == NULL)
                return tag.project(new_tagset);
        }
        return tag_type::getNullTag();
    }

    static void addSentinels(vector<Lexeme>& src, vector<Lexeme>& dst) {
        dst.clear();
        for (int i = 0; i < INDEX_OFFSET; i++)
            dst.push_back(Lexeme::getNullLexeme());
        dst.insert(dst.end(), src.begin(), src.end());
        for (int i = 0; i < INDEX_OFFSET; i++)
            dst.push_back(Lexeme::getNullLexeme());
    }
public:

    void init(vector<Lexeme>& text, const Tagset* full_tagset) {
        // Add sentinel segments.
        addSentinels(text, this->text);

        this->full_tagset = full_tagset;
        this->phase = -1;

    }

    void generateInitialTags(int phase, vector<Lexeme>& text_to_tag) {
        const Tagset* tagset = phase_tagsets[phase];

        cerr << "Training unigram tagger ...";
        unigram_tagger.clear();
        unigram_tagger.train(text, tagset == full_tagset ? NULL : tagset);
        cerr << "  done." << endl;

        cerr << "Preparing initial tagging for phase " << phase
                << " ...  ";
        const Tagset* previous_tagset =
                phase == 0 ? NULL : phase_tagsets[phase - 1];
        int n = text_to_tag.size();
        #pragma omp parallel for default(shared)
        for (int i = 0; i < n; i++) {
            Lexeme& lexeme = text_to_tag[i];
            tag_type previous_tag = phase == 0 ? tag_type::getNullTag()
                    : lexeme.chosen_tag[phase - 1];
            lexeme.chosen_tag[phase] =
                    findBestInitialTag(lexeme, tagset, previous_tagset,
                            previous_tag);
            lexeme.considered_tags = calculateConsideredTags(lexeme, tagset,
                    previous_tagset, previous_tag);
            lexeme.expected_tag = findCorrectTag(lexeme, tagset, previous_tagset,
                    previous_tag);
        }
        cerr << "done." << endl;

        /*int cnt = 0;
        BOOST_FOREACH(Lexeme& lexeme, text_to_tag) {
            if (lexeme.chosen_tag[phase] != lexeme.expected_tag) {
                cerr << "Mismatch " << lexeme.getOrth() << " chosen " <<
                        lexeme.chosen_tag[phase].asString(tagset) <<
                        ", expected " << lexeme.expected_tag.asString(tagset) <<
                        " (freqs: " <<
                        unigram_tagger.getFrequency(lexeme.getOrth(), lexeme.chosen_tag[phase]) <<
                        ' ' <<
                        unigram_tagger.getFrequency(lexeme.getOrth(), lexeme.expected_tag) << ')' <<
                        " num_allowed: " << lexeme.getAllowedTags().size() << endl;
                cnt++;

                if (cnt > 300) break;
            }
        }*/
    }

    void generatePredicates(vector<Lexeme>& v, int i, vector<Predicate<Lexeme> >& preds) {
        BOOST_FOREACH(PredicateTemplate<Lexeme>* tpl, rule_templates)
            tpl->findMatchingPredicates(preds, v, i);
    }

    void generateInitialScores()
    {
        scores.clear();

        cerr << "Generating initial rules for phase " << phase << " ...  ";
        if (DBG)
            cerr << endl;

        int n = text.size() - INDEX_OFFSET;
        for (int i = INDEX_OFFSET; i < n; ++i) {
            Lexeme& lex = text[i];
            vector<Predicate<Lexeme> > preds;
            generatePredicates(text, i, preds);

            if (DBG) {
                fprintf(stderr, "Init i: %d (%s) num_preds: %d ", i, lex.getOrth().c_str(), (int)preds.size());
                if (preds.size() > 0) fprintf(stderr, "%s\n", preds[0].tpl->predicateAsString(preds[0]).c_str());
                else fprintf(stderr, "\n");
            }

            //fprintf(stderr, "%s %s\n", lex.orth, tag_name(lex.chosen_tag[phase], phase).c_str());
            if (DBG) {
                if (lex.considered_tags.empty())
                    throw Exception(boost::str(
                            boost::format("lexeme '%1%' at %2% has no "
                                    "considered_tags") % lex.getOrth() % i));
                if (!lex.isConsideredTag(lex.chosen_tag[phase]))
                    throw Exception(boost::str(
                            boost::format("chosen_tag not in considered_tags "
                                "(lex: %1%, pos: %2%, chosen_tag: %3%)") %
                                lex.getOrth() % i %
                                lex.chosen_tag[phase].asString(phase_tagset)));
            }

            if (lex.chosen_tag[phase] == lex.expected_tag) {
                BOOST_FOREACH(const Predicate<Lexeme>& pred, preds) {
                    BOOST_FOREACH(const tag_type& ctag, lex.considered_tags) {
                        if (ctag == lex.expected_tag) continue;
                        Rule<Lexeme> r = Rule<Lexeme>(pred, ctag);
                        scores[r].second++;
                    }
                }
            } else {
                if (lex.expected_tag != tag_type::getNullTag()) {
                    BOOST_FOREACH(const Predicate<Lexeme>& pred, preds) {
                        Rule<Lexeme> r = Rule<Lexeme>(pred, lex.expected_tag);
                        scores[r].first++;
                    }
                }
            }
        }
        cerr << "done." << endl;
    }

    bool matchPredicate(Predicate<Lexeme>& p, vector<Lexeme>& v, int i) {
        return p.tpl->predicateMatches(p, v, i);
    }

    void process(int threshold) {
        int errors = 0;

        // TODO: Wlepic napis, skad pochodzi ten algorytm.

        if (scores.size() == 0) return;

        for (int round = 1; ; round++) {
            int correct_tags = 0, incorrect_tags = 0;
            int f;
            Rule<Lexeme> b = this->findBestRule(&f);

            fprintf(stderr, "(%d) CHOSEN RULE (good=%d, bad=%d): %s\n",
                            round, scores[b].first, scores[b].second,
                            b.asString().c_str());

            if (f < threshold || cancelled) break;

            assert(countScores(b)
                    == make_pair(scores[b].first, scores[b].second));

            double P, R, F;
            applyRule(phase, b, text, next_text, INDEX_OFFSET,
                    text.size() - INDEX_OFFSET, round, VICINITY);
            calculateStats(phase, next_text, &P, &R, &F, &errors, INDEX_OFFSET,
                    text.size() - INDEX_OFFSET);
            fprintf(stderr, "Applied. P=%lf R=%lf F=%lf errors: %d\n",
                    P, R, F, errors);

            generated_rules.push_back(b);

            int n = text.size() - INDEX_OFFSET;
            #pragma omp parallel default(shared)
            {
                vector<Rule<Lexeme> > first_dec;
                vector<Rule<Lexeme> > second_dec;
                vector<Rule<Lexeme> > first_inc;
                vector<Rule<Lexeme> > second_inc;

                #pragma omp for
                for (int i = INDEX_OFFSET; i < n; ++i) {
                    Lexeme& lex1 = text[i];
                    Lexeme& lex2 = next_text[i];
                    if (lex2.chosen_tag[phase] == lex1.expected_tag)
                        correct_tags++;
                    else
                        incorrect_tags++;

                    if (lex2.vicinity != round) continue;

                    if (lex2.chosen_tag[phase] == lex1.chosen_tag[phase]) {
                        vector<Predicate<Lexeme> > preds;
                        generatePredicates(text, i, preds);
                        BOOST_FOREACH(Predicate<Lexeme>& pred, preds) {
                            if (lex1.chosen_tag[phase] != lex1.expected_tag) {
                                if (!matchPredicate(pred, next_text, i)
                                        && lex1.expected_tag != tag_type::getNullTag()) {
                                    first_dec.push_back(Rule<Lexeme>(pred, lex1.expected_tag));
                                }
                            } else {
                                if (!matchPredicate(pred, next_text, i))
                                    BOOST_FOREACH(const tag_type& ctag,
                                            lex1.considered_tags) {
                                        if (ctag != lex1.chosen_tag[phase]) {
                                            second_dec.push_back(Rule<Lexeme>(pred, ctag));
                                        }
                                    }
                            }
                        }
                        preds.clear();
                        generatePredicates(next_text, i, preds);

                        BOOST_FOREACH(Predicate<Lexeme>& pred, preds) {
                            if (lex2.chosen_tag[phase] != lex1.expected_tag) {
                                if (!matchPredicate(pred, text, i)
                                        && lex1.expected_tag != tag_type::getNullTag()) {
                                   first_inc.push_back(Rule<Lexeme>(pred, lex1.expected_tag));
                                }
                            } else {
                                if (!matchPredicate(pred, text, i)) {
                                    BOOST_FOREACH(const tag_type& ctag,
                                            lex1.considered_tags) {
                                        if (ctag != lex2.chosen_tag[phase]) {
                                            second_inc.push_back(Rule<Lexeme>(pred, ctag));
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        vector<Predicate<Lexeme> > preds;
                        generatePredicates(text, i, preds);
                        BOOST_FOREACH(Predicate<Lexeme>& pred, preds) {
                            if (lex1.chosen_tag[phase] != lex1.expected_tag) {
                                if (lex1.expected_tag != tag_type::getNullTag() &&
                                        (!matchPredicate(pred, next_text, i) ||
                                         lex2.chosen_tag[phase] == lex1.expected_tag)) {
                                    first_dec.push_back(Rule<Lexeme>(pred, lex1.expected_tag));
                                }
                            } else {
                                BOOST_FOREACH(const tag_type& ctag,
                                        lex1.considered_tags) {
                                    if (ctag != lex1.chosen_tag[phase]) {
                                        second_dec.push_back(Rule<Lexeme>(pred, ctag));
                                    }
                                }
                            }
                        }
                        preds.clear();
                        generatePredicates(next_text, i, preds);
                        BOOST_FOREACH(Predicate<Lexeme>& pred, preds) {
                            if (lex2.chosen_tag[phase] != lex1.expected_tag) {
                                if (lex1.expected_tag != tag_type::getNullTag() &&
                                        (!matchPredicate(pred, text, i) ||
                                         lex1.chosen_tag[phase] == lex1.expected_tag)) {
                                    first_inc.push_back(Rule<Lexeme>(pred, lex1.expected_tag));
                                }
                            } else {
                                BOOST_FOREACH(const tag_type& ctag,
                                        lex1.considered_tags) {
                                    if (ctag != lex2.chosen_tag[phase]) {
                                        second_inc.push_back(Rule<Lexeme>(pred, ctag));
                                    }
                                }
                            }
                        }
                    }
                }

                #pragma omp critical (scores)
                {
                    BOOST_FOREACH(const Rule<Lexeme>& r, first_inc) scores[r].first++;
                    BOOST_FOREACH(const Rule<Lexeme>& r, first_dec) scores[r].first--;
                    BOOST_FOREACH(const Rule<Lexeme>& r, second_inc) scores[r].second++;
                    BOOST_FOREACH(const Rule<Lexeme>& r, second_dec) scores[r].second--;
                }

            }
            std::swap(text, next_text);

            if (DBG) {
                fprintf(stderr, "Correct tags: %d, incorrect tags: %d\n",
                        correct_tags, incorrect_tags);

                fprintf(stderr, "\nCURRENT TAGGING:\n\n");
                //print_text();
                fprintf(stderr, "\n");
            }

        }
    }

    Rule<Lexeme> findBestRule(int* max) {
        Rule<Lexeme> maxR = scores.begin()->first;
        *max = scores[maxR].first - scores[maxR].second;

        if (DBG) {
            cerr << "Rules: " << endl;
        }

        BOOST_FOREACH(const typename scores_map_type::value_type& i, scores) {
            if (DBG) {
                cerr << "\t" << i.first.asString()
                    << " (good: " << i.second.first
                    << ", bad: " << i.second.second << ")" << endl;
                assert(countScores(i.first)
                        == std::make_pair(i.second.first, i.second.second));
            }

            int val = i.second.first - i.second.second;
            if (val > *max) {
                *max = val;
                maxR = i.first;
            }
        }

        return maxR;
    }

    void runPhase(const Tagset* tagset,
            const vector<PredicateTemplate<Lexeme>*>& rule_templates,
            int threshold) {
        phase++;
        phase_tagsets.push_back(tagset);
        phase_tagset = tagset;
        this->rule_templates = rule_templates;
        generated_rules.clear();
        cancelled = false;

        try {
            generateInitialTags(phase, text);
            generateInitialScores();
            next_text = text;
            process(threshold);

            if (!cancelled)
                phase_generated_rules.push_back(generated_rules);
        } catch (...) {
            phase--;
            throw;
        }

    }

    void reportTaggingStats(vector<Lexeme>& tagged_text) {
        double prec = 0, recall = 0, f_measure = 0;

        calculateTaggingStats(tagged_text, &prec, &recall, &f_measure,
                NULL, INDEX_OFFSET, tagged_text.size() - INDEX_OFFSET);

        fprintf(stderr, "Stats -- P: %lf, R: %lf, F: %lf\n",
                prec, recall, f_measure);
    }

    std::pair<int, int> countScores(const Rule<Lexeme>& b) {
        int countGood = 0, countBad = 0;
        for (int i = INDEX_OFFSET, n = text.size() - INDEX_OFFSET; i < n; ++i) {
            const Lexeme& lex = text[i];
            if (b.predicate.tpl->predicateMatches(b.predicate, text, i)
                    && lex.isConsideredTag(b.tag)) {
                if (lex.expected_tag == b.tag
                        && lex.expected_tag != lex.chosen_tag[phase])
                    countGood++;
                else if (lex.expected_tag != b.tag
                        && lex.expected_tag == lex.chosen_tag[phase])
                    countBad++;
            }
        }
        return std::make_pair(countGood, countBad);
    }

    void cancel() {
        cancelled = true;
    }

    const vector<Lexeme>& getText() const {
        return text;
    }

    int getPhase() const {
        return phase;
    }

    void tagText(vector<Lexeme>& text_to_tag) {
        vector<Lexeme> our_text_to_tag;
        addSentinels(text_to_tag, our_text_to_tag);

        for (int i = 0; i <= phase; i++) {
            generateInitialTags(i, our_text_to_tag);
            applyAllRules(our_text_to_tag, i);
        }
        reportTaggingStats(our_text_to_tag);

        text_to_tag.clear();
        text_to_tag.insert(text_to_tag.end(),
                our_text_to_tag.begin() + INDEX_OFFSET,
                our_text_to_tag.end() - INDEX_OFFSET);
    }

    void applyAllRules(vector<Lexeme>& text, int phase) {
        vector<Lexeme> next_text = text;
        double prec = 0, recall = 0, f_measure = 0;
        int last_errors = text.size();
        BOOST_FOREACH(Rule<Lexeme>& rule, phase_generated_rules[phase]) {
            int errors;
            applyRule(phase, rule, text, next_text, INDEX_OFFSET,
                    text.size() - INDEX_OFFSET);
            calculateStats(phase, next_text, &prec, &recall, &f_measure,
                    &errors, INDEX_OFFSET, text.size() - INDEX_OFFSET);
            fprintf(stderr, "%d. Applied rule: %s (P: %lf, R: %lf, F: %lf, "
                    "errors: %d)\n", phase, rule.asString().c_str(), prec,
                    recall, f_measure, errors);

            if (errors > last_errors) {
                fprintf(stderr, "    Bad rule :(  errors was %d, now %d\n",
                        last_errors, errors);
            }

            swap(text, next_text);
            last_errors = errors;
        }
    }

};

} // namespace BTagger

#endif /* BRILLENGINE_H_ */
