/*
 * brillengine.h
 *
 *  Created on: Jan 3, 2010
 *      Author: accek
 */

#ifndef BRILLENGINE_H_
#define BRILLENGINE_H_

#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/mpi.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <cassert>
#include <limits>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>

#include "unigram.h"
#include "rules.h"
#include <nlpcommon/scorer.h>

namespace BTagger {

using namespace NLPCommon;

using std::wcerr;
using std::endl;
using std::vector;
using std::map;

namespace mpi = boost::mpi;

static const int INDEX_OFFSET = 4;

template<class Lexeme, class Scorer>
class BrillEngine
{
    BOOST_CONCEPT_ASSERT((MultiGoldenScorerConcept<Scorer>));

public:
    typedef typename Lexeme::tag_type tag_type;
    //typedef typename boost::same_type<Lexeme::tag_type, Scorer::tag_type>
    //    tag_type;

private:
    typedef typename Scorer::score_type score_type;
    typedef boost::unordered_map<Rule<Lexeme>,
            std::pair<score_type, score_type> > scores_map_type;

    const Tagset* full_tagset;
    vector<Lexeme> text;
    vector<Lexeme> next_text;
    RulesGenerator<Lexeme>* rules_generator;
    TemplatesStore<Lexeme>* tstore;
    scores_map_type scores;
    scores_map_type good_scores;
    vector<Rule<Lexeme> > generated_rules;
    bool cancelled;
    boost::scoped_ptr<Scorer> scorer;
    vector<int> in_vicinity;

    vector<const Tagset*> phase_tagsets;
    vector<TemplatesStore<Lexeme>*> phase_tstores;
    vector<UnigramTagger<Lexeme> > phase_unigram_taggers;
    vector<vector<Rule<Lexeme> > > phase_generated_rules;

    mpi::communicator mpi_world;
    MPIProgressController progress;
    bool quiet;

    static const int DBG = 0;
    static const int VICINITY = 3;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        bool is_trained = isTrained();
        ar & is_trained;
        if (is_trained) {
            ar & phase_unigram_taggers;
            ar & phase_generated_rules;
        } else {
            ar & text;
        }
    }

    score_type ruleScore(const std::pair<score_type, score_type>& s) {
        return s.first - s.second;
    }

    score_type ruleScore(const Rule<Lexeme>& r) {
        std::pair<score_type, score_type> s = scores[r];
        return ruleScore(s);
    }

    void applyRule(int phase, Rule<Lexeme>& b, vector<Lexeme>& text1,
            vector<Lexeme>& text2,
            int start_index = 0,
            int end_index = std::numeric_limits<int>::max(),
            int vicinity_estimate = 0, bool use_mpi = true,
            int rule_number = 0) {
        assert(text1.size() == text2.size());
        start_index = std::min(start_index, (int)text1.size());
        end_index = std::min(end_index, (int)text1.size());

        BOOST_FOREACH(int i, in_vicinity) {
            text2[i].chosen_tag[phase] = text1[i].chosen_tag[phase];
            text2[i].last_matched_rule = text1[i].last_matched_rule;
        }

        in_vicinity.clear();
        if (vicinity_estimate)
            in_vicinity.reserve(vicinity_estimate);

        if (mpi_world.rank() == 0) {
            int j, z = -1;
            for (int i = start_index; i < end_index; ++i) {
                assert(text2[i].chosen_tag[phase] == text1[i].chosen_tag[phase]);
                if (b.isApplicable(phase_tstores[phase], text1, i)) {
                    text2[i].chosen_tag[phase] = b.changedTag(phase_tstores[phase], text1, i);
                    text2[i].last_matched_rule = rule_number + phase * 10000;
                    in_vicinity.push_back(i);
                }
            }
        }
        if (use_mpi) {
            mpi::broadcast(mpi_world, in_vicinity, 0);
            if (mpi_world.rank() != 0) {
                BOOST_FOREACH(int i, in_vicinity) {
                    assert(b.isApplicable(phase_tstores[phase], text1, i));
                    text2[i].chosen_tag[phase] = b.changedTag(phase_tstores[phase], text1, i);
                    text2[i].last_matched_rule = rule_number + phase * 10000;
                }
            }
        }
    }

    static void calculatePhaseStats(const Tagset* tagset, int phase,
            vector<Lexeme>& text, double* accuracy, int* errors,
            double* avg_score,
            int start_index = 0,
            int end_index = std::numeric_limits<int>::max()) {
        start_index = std::min(start_index, (int)text.size());
        end_index = std::min(end_index, (int)text.size());

        int e = 0;
        score_type ss = score_type();
        Scorer scorer(tagset);

        #pragma omp parallel for \
            default(shared) \
            reduction(+: e, ss)
        for (int i = start_index; i < end_index; ++i) {
            const Lexeme& lex = text[i];
            ss += scorer.score(lex.chosen_tag[phase], lex.expected_tag);
            if (lex.chosen_tag[phase] != lex.expected_tag)
                e++;
        }

		if (accuracy)
			*accuracy = 1.0 - e/double(end_index - start_index);
        if (avg_score)
            *avg_score = double(ss) / double(scorer.maxScore())
                / (end_index - start_index);
        if (errors)
            *errors = e;
    }

	vector<int> mapToPos(const vector<tag_type>& tags) {
		vector<int> pos;
		BOOST_FOREACH(const tag_type& tag, tags)
			pos.push_back(tag.getPos());
		std::sort(pos.begin(), pos.end());
		pos.resize(std::unique(pos.begin(), pos.end()) - pos.begin());
		return pos;
	}

    void calculateTaggingStats(vector<Lexeme>& text,
            double* prec,
            double* recall,
			double* f_measure,
			double* strong_correctness,
            double* avg_score,
            double* prec_pos = NULL,
            double* recall_pos = NULL,
			double* f_measure_pos = NULL,
			double* strong_correctness_pos = NULL,
            int start_index = 0,
            int end_index = std::numeric_limits<int>::max()) {
        start_index = std::min(start_index, (int)text.size());
        end_index = std::min(end_index, (int)text.size());

        int phase = numPhases() - 1;
        int tp = 0, tn = 0, fp = 0, fn = 0;
        int sc = 0; /* strongly correct */
        int tppos = 0, tnpos = 0, fppos = 0, fnpos = 0;
        int scpos = 0;
        score_type ss = score_type();

        for (int i = start_index; i < end_index; ++i) {
            const Lexeme& lex = text[i];
            int cs = lex.getGoldenTags().size();
            int as = lex.getAllowedTags().size();
            ss += scorer->score(lex.chosen_tag[phase], lex.getAllowedTags());
            if (lex.isGoldenTag(lex.chosen_tag[phase])) {
                tp++;
                fn += cs - 1;
                tn += as - cs;
				if (cs == 1)
					sc++;
            } else {
                fp++;
                fn += cs;
                tn += as - cs - 1;
            }

			vector<int> golden_pos = mapToPos(lex.getGoldenTags());
			vector<int> allowed_pos = mapToPos(lex.getAllowedTags());
			bool good_pos = std::find(golden_pos.begin(), golden_pos.end(),
					lex.chosen_tag[phase].getPos()) != golden_pos.end();
            int cspos = golden_pos.size();
            int aspos = allowed_pos.size();

            if (good_pos) {
                tppos++;
                fnpos += cspos - 1;
                tnpos += aspos - cspos;
				if (cspos == 1)
					scpos++;
            } else {
                fppos++;
                fnpos += cspos;
                tnpos += aspos - cspos - 1;
            }
        }

        if (prec)
            *prec = tp / double(tp + fp);
        if (recall)
            *recall = tp / double(tp + fn);
        if (prec && recall && f_measure)
            *f_measure = 2 * *prec * *recall / (*prec + *recall);
		if (strong_correctness)
			*strong_correctness = sc / double(tp + fp);

        if (avg_score)
            *avg_score = double(ss) / double(scorer->maxScore())
                / (end_index - start_index);

        if (prec_pos)
            *prec_pos = tppos / double(tppos + fppos);
        if (recall_pos)
            *recall_pos = tppos / double(tppos + fnpos);
        if (prec_pos && recall_pos && f_measure_pos)
            *f_measure_pos = 2 * *prec_pos * *recall_pos / (*prec_pos + *recall_pos);
		if (strong_correctness_pos)
			*strong_correctness_pos = scpos / double(tppos + fppos);
    }

    tag_type findBestInitialTag(int phase, Lexeme& lexeme,
            const Tagset* new_tagset, const Tagset* previous_tagset,
             const tag_type& previous_tag = tag_type::getNullTag())
    {
        const wstring& orth = lexeme.getOrth();
        const UnigramTagger<Lexeme>& unigram_tagger =
            phase_unigram_taggers[phase];

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

    tag_type findGoldenTag(Lexeme& lexeme,
            const Tagset* new_tagset, const Tagset* previous_tagset,
             const tag_type& previous_tag) {
        BOOST_FOREACH(const tag_type& tag, lexeme.getGoldenTags()) {
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
    BrillEngine()
        : progress(mpi_world), quiet(false) { }

    void setText(vector<Lexeme>& text) {
        // Add sentinel segments.
        addSentinels(text, this->text);
    }

    bool isTrained() {
        return phase_generated_rules.size() == phase_tstores.size();
    }

    void trainUnigramTaggers() {
        if (!phase_unigram_taggers.empty()) {
            assert(phase_unigram_taggers.size() == numPhases());
            return;
        }

        phase_unigram_taggers.resize(numPhases());
        for (int p = 0; p < numPhases(); p++) {
            if (mpi_world.rank() == 0 && !quiet)
                wcerr << "Training unigram tagger for phase " << p << "..." << endl;
            const Tagset* tagset = phase_tagsets[p];
            phase_unigram_taggers[p].train(text,
                    tagset == full_tagset ? NULL : tagset);
        }
    }

    void generateInitialTags(int phase, vector<Lexeme>& text_to_tag) {
        const Tagset* tagset = phase_tagsets[phase];

        trainUnigramTaggers();

        if (mpi_world.rank() == 0 && !quiet)
            wcerr << "Preparing initial tagging for phase " << phase
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
                    findBestInitialTag(phase, lexeme, tagset, previous_tagset,
                            previous_tag);
            lexeme.considered_tags = calculateConsideredTags(lexeme, tagset,
                    previous_tagset, previous_tag);
            lexeme.expected_tag = findGoldenTag(lexeme, tagset, previous_tagset,
                    previous_tag);
        }
        if (mpi_world.rank() == 0 && !quiet)
            wcerr << "done." << endl;

        /*int cnt = 0;
        BOOST_FOREACH(Lexeme& lexeme, text_to_tag) {
            if (lexeme.chosen_tag[phase] != lexeme.expected_tag) {
                wcerr << "Mismatch " << lexeme.getUtf8Orth() << " chosen " <<
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

    score_type scoreDelta(const Tag& chosen_tag, const Tag& new_tag, const Tag&
            expected_tag) {
        return scorer->score(new_tag, expected_tag)
            - scorer->score(chosen_tag, expected_tag);
    }

    void applyScoreChanges(
            vector<std::pair<Rule<Lexeme>, score_type> >& score_adds,
            vector<std::pair<Rule<Lexeme>, score_type> >& score_subs) {
        typedef std::pair<Rule<Lexeme>, score_type> score_changes_type;
        #pragma omp critical (applyScoreChanges)
        {
            BOOST_FOREACH(const score_changes_type& ch, score_adds) {
                if (ch.second == 0)
                    continue;
                if (ch.second > 0)
                    scores[ch.first].first += ch.second;
                else
                    scores[ch.first].second += -ch.second;
                typename scores_map_type::iterator i = scores.find(ch.first);
                if (scorer->isLessThanEpsilon(i->second.first) &&
                        scorer->isLessThanEpsilon(i->second.second))
                    scores.erase(i);
            }
            BOOST_FOREACH(const score_changes_type& ch, score_subs) {
                if (ch.second == 0)
                    continue;
                if (ch.second > 0)
                    scores[ch.first].first -= ch.second;
                else
                    scores[ch.first].second -= -ch.second;
                typename scores_map_type::iterator i = scores.find(ch.first);
                if (scorer->isLessThanEpsilon(i->second.first) &&
                        scorer->isLessThanEpsilon(i->second.second))
                    scores.erase(i);
            }
        }
        score_adds.clear();
        score_subs.clear();
    }

    void generateInitialScores(int phase)
    {
        const Tagset* phase_tagset = phase_tagsets[phase];

        scores.clear();

        int n = text.size() - INDEX_OFFSET;
        int num = 0;

        // TODO: move somewhere
        /*size_t num_buckets = 16 * (1 << 20);
        int min_rule_count = 3;

        vector<uint32_t> bucket_counts;
        bucket_counts.resize(num_buckets);
        int num_rules = 0;

        progress.init(boost::str(boost::wformat(
            L"Counting initial rules for phase %d ...") % phase));

        #pragma omp parallel for default(shared) \
                reduction(+: num_rules)
        for (int i = INDEX_OFFSET; i < n; ++i) {
            if (((++num) % 1000) == 0) {
                #pragma omp critical (generateInitialScores_progress)
                progress.set(num/(float)n);
            }
            Lexeme& lex = text[i];

            vector<Rule<Lexeme> > rules;
            rules_generator->generateUniqueRules(text, i, rules);
            BOOST_FOREACH(const Rule<Lexeme>& r, rules) {
                num_rules++;
                bucket_counts[hash_value(r) % num_buckets]++;
            }
        }

        progress.finish();
        mpi_world.barrier();

        vector<bool> buckets_map;
        buckets_map.resize(num_buckets);

        int single_buckets = 0;
        for (int i = 0; i < num_buckets; i++) {
            if (bucket_counts[i] > 0 && bucket_counts[i] < min_rule_count)
                single_buckets++;
            else
                buckets_map[i] = true;
        }

        bucket_counts.clear();

        int total_buckets;
        mpi::reduce(mpi_world, single_buckets, total_buckets, std::plus<int>(),
                0);
        if (mpi_world.rank() == 0)
            wcerr << single_buckets << " rules matching less than "
                << min_rule_count << " times eliminated." << endl;

        */
        num = 0;
        progress.init(boost::str(boost::format(
            "Generating initial rules for phase %d ...") % phase));

        #pragma omp parallel default(shared)
        {
            typedef std::pair<Rule<Lexeme>, score_type> score_changes_type;
            vector<score_changes_type> score_adds, score_subs;

            #pragma omp for
            for (int i = INDEX_OFFSET; i < n; ++i) {
                if (((++num) % 1000) == 0) {
                    #pragma omp critical (generateInitialScores_progress)
                    progress.set(num/(float)n);
                }
                Lexeme& lex = text[i];

                vector<Rule<Lexeme> > rules;
                rules_generator->generateUniqueRules(text, i, rules);

                /*wcerr << "GEN: " << lex.getOrth() << ' ' <<
                    rules.size() << " rules; chosen: " <<
                    lex.chosen_tag[phase].asWString(phase_tagset) <<
                    ", expected: " <<
                    lex.expected_tag.asWString(phase_tagset) <<
                    endl;*/

                if (DBG && mpi_world.rank() == 0) {
                    if (lex.considered_tags.empty())
                        throw Exception(boost::str(
                                boost::format("lexeme '%1%' at %2% has no "
                                        "considered_tags") % lex.getUtf8Orth() % i));
                    if (!lex.isConsideredTag(lex.chosen_tag[phase]))
                        throw Exception(boost::str(
                                boost::format("chosen_tag not in considered_tags "
                                    "(lex: %1%, pos: %2%, chosen_tag: %3%)") %
                                    lex.getUtf8Orth() % i %
                                    lex.chosen_tag[phase].asString(phase_tagset)));
                    if (lex.expected_tag != tag_type::getNullTag()) {
                        if (!lex.isConsideredTag(lex.expected_tag))
                            throw Exception(boost::str(
                                    boost::format("expected_tag not in considered_tags "
                                        "(lex: %1%, pos: %2%, expected_tag: %3%)") %
                                        lex.getUtf8Orth() % i %
                                        lex.expected_tag.asString(phase_tagset)));
                        if (phase_tagset->getPartOfSpeech(
                                    lex.expected_tag.getPos())->getName() == "ign")
                            throw Exception(boost::str(
                                    boost::format("expected_tag is ign "
                                        "(lex: %1%, pos: %2%)") %
                                        lex.getUtf8Orth() % i));
                    }
                }

                BOOST_FOREACH(const Rule<Lexeme>& r, rules) {
                    if (DBG && !r.isApplicable(tstore, text, i)) {
                        #pragma omp critical
                        {
                            wcerr << endl << "*** Inconsistency detected" << endl;
                            wcerr << "RULE: " << r.asWString(tstore) << endl;
                            wcerr << "Is not applicable at offset " << i
                                << ", but reported by rules generator.";
                            throw new Exception("Internal inconsistency: "
                                    "broken rule templates.");
                        }
                    }

                    /*if (!buckets_map[hash_value(r) % num_buckets])
                        continue;*/

                    const Tag& chosen_tag = lex.chosen_tag[phase];
                    score_type delta = scoreDelta(chosen_tag,
                            r.changedTag(tstore, text, i), lex.expected_tag);
                    score_adds.push_back(std::make_pair(r, delta));
                }

                if (score_adds.size() + score_subs.size() > 10000)
                    applyScoreChanges(score_adds, score_subs);
            }

            applyScoreChanges(score_adds, score_subs);
        }

        progress.finish();
    }

    void updateScores(int phase, int round, int threshold, int vicinity_radius) {
        int vsize = in_vicinity.size();
        int i = INDEX_OFFSET;
        int n = text.size() - INDEX_OFFSET;

        //#pragma omp for
        for (int j = 0; j < vsize; j++) {
            i = std::max(i, in_vicinity[j] - vicinity_radius);
            int z = std::min(n - 1, in_vicinity[j] + vicinity_radius);
            for (; i<=z; i++) {
                Lexeme& lex1 = text[i];
                Lexeme& lex2 = next_text[i];

                //if (lex2.vicinity != round) continue;

                const Tag& expected_tag = lex1.expected_tag;
                const Tag& chosen_tag1 = lex1.chosen_tag[phase];
                const Tag& chosen_tag2 = lex2.chosen_tag[phase];

                // For each predicate which matched here before, we retract
                // the scores.
                vector<Rule<Lexeme> > rules;
                rules_generator->generateUniqueRules(text, i, rules);
                BOOST_FOREACH(const Rule<Lexeme>& r, rules) {
                    score_type s = scoreDelta(chosen_tag1,
                            r.changedTag(tstore, text, i), expected_tag);
                    if (s == 0) continue;
                    std::pair<score_type, score_type>& rscore = scores[r];
                    if (s > 0)
                        rscore.first -= s;
                    else
                        rscore.second += s;
                    if (ruleScore(rscore) >= threshold)
                        good_scores[r] = rscore;
                    else
                        good_scores.erase(r);
                }

                // For each predicate which matches now, we add the scores.
                rules.clear();
                rules_generator->generateUniqueRules(next_text, i, rules);
                BOOST_FOREACH(const Rule<Lexeme>& r, rules) {
                    score_type s = scoreDelta(chosen_tag2,
                            r.changedTag(tstore, next_text, i), expected_tag);
                    if (s == 0) continue;
                    std::pair<score_type, score_type>& rscore = scores[r];
                    if (s > 0)
                        rscore.first += s;
                    else
                        rscore.second -= s;
                    if (ruleScore(rscore) >= threshold)
                        good_scores[r] = rscore;
                    else
                        good_scores.erase(r);
                }
            }
        }
    }

    void initializeGoodScores(int threshold) {
        if (mpi_world.rank() == 0 && !quiet)
            wcerr << "Gathering rules with score >= threshold ...  ";

        good_scores.clear();
        BOOST_FOREACH(const typename scores_map_type::value_type& j, this->scores) {
            if (ruleScore(j.second) >= threshold)
                good_scores[j.first] = j.second;
        }

        size_t total_good_rules;
        mpi::reduce(mpi_world, good_scores.size(), total_good_rules,
                std::plus<size_t>(), 0);
        if (mpi_world.rank() == 0 && !quiet)
            wcerr << total_good_rules << " rules" << endl;
    }

    void process(int phase, int threshold) {
        const Tagset* phase_tagset = phase_tagsets[phase];

        // TODO: Wlepic napis, skad pochodzi ten algorytm.

        if (scores.size() == 0) return;

        for (int round = 1; ; round++) {
            score_type good, bad;
            bool found;
            Rule<Lexeme> b = this->findBestRule(threshold, &found, &good, &bad);

            if (!found || cancelled) break;

            if (mpi_world.rank() == 0 && !quiet) {
                wstring rule_str = b.asWString(tstore);
                wcerr << boost::wformat(
                        L"(%d) CHOSEN RULE (good=%.1lf, bad=%.1lf, candidates=%d, good_candidates=%d): %ls") %
                                round %
                                double(good) %
                                double(bad) %
                                int(scores.size()) %
                                int(good_scores.size()) %
                                rule_str
                      << endl;;
            }

            if (mpi_world.rank() == 0 && DBG && !quiet) {
                std::pair<score_type, score_type> good_scores = countScores(phase, b);
                if (good_scores != std::make_pair(good, bad)) {
                    wcerr << boost::wformat(L"\n*** WRONG SCORES ***"
                            "\n good is %.1lf, should be %.1lf"
                            "\n bad is %.1lf, should be %.1lf\n\n") %
                            double(good) %
                            double(good_scores.first) %
                            double(bad) %
                            double(good_scores.second);
                }
            }

            applyRule(phase, b, text, next_text, INDEX_OFFSET,
                    text.size() - INDEX_OFFSET,
                    2 * (good + bad), true, round);

            if ((round % 10) == 1 && mpi_world.rank() == 0 && !quiet) {
				int errors = 0;
				double accuracy, avg_score;
                calculatePhaseStats(phase_tagset, phase, next_text, &accuracy,
                        &errors, &avg_score,
                        INDEX_OFFSET, text.size() - INDEX_OFFSET);
                wcerr << boost::wformat(L"Applied. accuracy=%lf avgScore=%lf errors=%d\n")
                        % accuracy % avg_score % errors;
            }

            generated_rules.push_back(b);

            updateScores(phase, round, threshold, VICINITY);
            std::swap(text, next_text);
        }
    }

    score_type calculateRulePriority(score_type good, score_type bad) {
		return good - bad;
    }

    Rule<Lexeme> findBestRule(score_type threshold, bool* found, score_type* good, score_type* bad) {
		// The rule with highest priority (as calculated by calculateRulePriority) is chosen,
		// among the rules having good - bad >= threshold.

        // [priority, [rule, [good, bad]]]
        typedef std::pair<score_type,
                    std::pair<Rule<Lexeme>, std::pair<score_type, score_type> > >
                        max_type;

        max_type gmax = std::make_pair(0, std::make_pair(
                    Rule<Lexeme>(), std::make_pair(0, 0)));

        int n = good_scores.bucket_count();

        #pragma omp parallel default(shared)
        {
            max_type max = gmax;

            #pragma omp for
            for (int i = 0; i < n; i++) {
                typename scores_map_type::const_local_iterator end = good_scores.end(i);
                for (typename scores_map_type::const_local_iterator j = good_scores.begin(i);
                        j != end; j++) {
                    score_type quality = j->second.first - j->second.second;
                    if (quality < threshold) continue;
                    max_type val = std::make_pair(
                            calculateRulePriority(j->second.first, j->second.second),
                            std::make_pair(j->first, j->second));
                    if (val > max) {
                        max = val;
                    }
                }
            }

            #pragma omp critical (findBestRuleMax)
            if (max > gmax)
                gmax = max;
        }

        mpi::reduce(mpi_world, gmax, gmax, mpi::maximum<max_type>(), 0);
        mpi::broadcast(mpi_world, gmax, 0);

        *found = (gmax.first > 0);
        *good = gmax.second.second.first;
        *bad = gmax.second.second.second;
        return gmax.second.first;
    }

    void addPhase(const Tagset* tagset, TemplatesStore<Lexeme>* tstore) {
        phase_tagsets.push_back(tagset);
        phase_tstores.push_back(tstore);
        this->full_tagset = tagset;
    }

    void distributePTemplates() {
        int num_pt = tstore->getAllPTemplates().size();
        vector<int> my_ids;
        for (int i = mpi_world.rank(); i < num_pt; i += mpi_world.size())
            my_ids.push_back(i);
        tstore->setEnabledPTemplates(my_ids);
    }

    void runPhase(int phase,
            RulesGenerator<Lexeme>* rules_generator,
            int threshold) {
        int original_phase = phase;

        this->rules_generator = rules_generator;
        this->tstore = phase_tstores[phase];

        scorer.reset(new Scorer(phase_tagsets[phase]));
        assert(this->tstore == rules_generator->getTStore());

        generated_rules.clear();
        in_vicinity.clear();

        cancelled = false;

        try {
            distributePTemplates();
            generateInitialTags(phase, text);
            generateInitialScores(phase);
            initializeGoodScores(threshold);
            next_text = text;
            process(phase, threshold);

            if (!cancelled)
                phase_generated_rules.push_back(generated_rules);
        } catch (...) {
            phase = original_phase;
            throw;
        }

    }

    void reportTaggingStats(vector<Lexeme>& tagged_text) {
        double prec, recall, f_measure, strong_correctness,
			   avg_score, prec_pos, recall_pos, f_measure_pos,
			   strong_correctness_pos;

        calculateTaggingStats(tagged_text, &prec, &recall, &f_measure,
				&strong_correctness, &avg_score, &prec_pos, &recall_pos,
				&f_measure_pos, &strong_correctness_pos,
                INDEX_OFFSET, tagged_text.size() - INDEX_OFFSET);

        wcerr << boost::wformat(L"Stats -- P %lf  R %lf  F %lf  SC %lf  AScore %lf\n")
                % prec % recall % f_measure % strong_correctness % avg_score;
        wcerr << boost::wformat(L"POSstats -- P %lf  R %lf  F %lf  SC %lf\n")
                % prec_pos % recall_pos % f_measure_pos % strong_correctness_pos;
    }

    static score_type goodScore(score_type s) {
        return s > 0 ? s : score_type();
    }

    static score_type badScore(score_type s) {
        return s < 0 ? -s : score_type();
    }

    std::pair<score_type, score_type> countScores(int phase, const Rule<Lexeme>& b) {
        score_type countGood = score_type();
        score_type countBad = score_type();

        int n = text.size() - INDEX_OFFSET;
        #pragma omp parallel for default(shared) \
                reduction(+: countGood, countBad)
        for (int i = INDEX_OFFSET; i < n; ++i) {
            const Lexeme& lex = text[i];
            if (b.isApplicable(tstore, text, i)) {
                vector<Rule<Lexeme> > rules;
                rules_generator->generateUniqueRules(text, i, rules);
                if (std::find(rules.begin(), rules.end(), b) == rules.end() &&
                        mpi_world.size() == 1) {
                    #pragma omp critical
                    {
                        wcerr << endl << "*** Inconsistency detected" << endl;
                        wcerr << "RULE: " << b.asWString(tstore) << endl;
                        wcerr << "Is applicable at offset " << i
                            << ", but not reported by rules generator.";
                        throw new Exception("Internal inconsistency: "
                                "broken rule templates.");
                    }
                }

                const Tag& chosen_tag = lex.chosen_tag[phase];
                score_type delta = scoreDelta(chosen_tag,
                        b.changedTag(tstore, text, i), lex.expected_tag);
                countGood += goodScore(delta);
                countBad += badScore(delta);
            }
        }
        return std::make_pair(countGood, countBad);
    }

    void cancel() {
        cancelled = true;
    }

    void setQuiet(bool value = true) {
        quiet = value;
    }

    const vector<Lexeme>& getText() const {
        return text;
    }

    int numPhases() const {
        return phase_tstores.size();
    }

    void tagText(vector<Lexeme>& text_to_tag) {
        vector<Lexeme> our_text_to_tag;
        addSentinels(text_to_tag, our_text_to_tag);

        scorer.reset(new Scorer(phase_tagsets[numPhases() - 1]));

        for (int i = 0; i < numPhases(); i++) {
            generateInitialTags(i, our_text_to_tag);
            applyAllRules(our_text_to_tag, i);
        }
        if (!quiet)
            reportTaggingStats(our_text_to_tag);

        int p = numPhases() - 1;
        for (int i = 0; i < text_to_tag.size(); i++) {
            text_to_tag[i].setAutoselectedTag(
                    our_text_to_tag[i + INDEX_OFFSET].chosen_tag[p]);
        }
    }

    void applyAllRules(vector<Lexeme>& text, int phase) {
        vector<Lexeme> next_text = text;
		int rule_num = 0;
		int num_rules = phase_generated_rules[phase].size();
        BOOST_FOREACH(Rule<Lexeme>& rule, phase_generated_rules[phase]) {
			rule_num++;

            applyRule(phase, rule, text, next_text, INDEX_OFFSET,
                    text.size() - INDEX_OFFSET, 0, false, rule_num);
            if (!quiet) {
                wcerr << boost::wformat(L"Phase %d. Rule %d/%d applied: %ls\n")
                        % phase % rule_num % num_rules
                        % rule.asWString(phase_tstores[phase]);
            }

			if (rule_num % 10 == 0 && !quiet) {
				double accuracy, avg_score;
				int errors;
				calculatePhaseStats(phase_tagsets[phase], phase, next_text, &accuracy,
						&errors, &avg_score, INDEX_OFFSET, text.size() -
						INDEX_OFFSET);
				wcerr << boost::wformat(L"Current stats -- accuracy: %lf, "
						"avg_score: %lf, errors: %d)\n") %
						accuracy % avg_score % errors;
			}

            swap(text, next_text);
        }
    }

};

} // namespace BTagger

#endif /* BRILLENGINE_H_ */
