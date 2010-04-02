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

    enum {
        MSG_PROGRESS = 1,
        MSG_DONE = 2
    };

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
    vector<float> progress;
    int progress_done;
    wstring progress_msg;
    vector<int> in_vicinity;

    vector<const Tagset*> phase_tagsets;
    vector<TemplatesStore<Lexeme>*> phase_tstores;
    vector<UnigramTagger<Lexeme> > phase_unigram_taggers;
    vector<vector<Rule<Lexeme> > > phase_generated_rules;

    mpi::communicator mpi_world;

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
            int vicinity_estimate = 0, bool use_mpi = true) {
        assert(text1.size() == text2.size());
        start_index = std::min(start_index, (int)text1.size());
        end_index = std::min(end_index, (int)text1.size());

        BOOST_FOREACH(int i, in_vicinity)
            text2[i].chosen_tag[phase] = text1[i].chosen_tag[phase];

        in_vicinity.clear();
        if (vicinity_estimate)
            in_vicinity.reserve(vicinity_estimate);

        if (mpi_world.rank() == 0) {
            wcerr << "APP>";
            int j, z = -1;
            for (int i = start_index; i < end_index; ++i) {
                assert(text2[i].chosen_tag[phase] == text1[i].chosen_tag[phase]);
                if (b.isApplicable(phase_tstores[phase], text1, i)) {
                    text2[i].chosen_tag[phase] = b.changedTag(phase_tstores[phase], text1, i);
                    in_vicinity.push_back(i);
                }
            }
            wcerr << "<LY";
        }
        if (use_mpi) {
            mpi::broadcast(mpi_world, in_vicinity, 0);
            if (mpi_world.rank() != 0) {
                BOOST_FOREACH(int i, in_vicinity) {
                    assert(b.isApplicable(phase_tstores[phase], text1, i));
                    text2[i].chosen_tag[phase] = b.changedTag(phase_tstores[phase], text1, i);
                }
            } else {
                wcerr << ">CAST" << endl;
            }
        }
    }

    static void calculatePhaseStats(const Tagset* tagset, int phase,
            vector<Lexeme>& text, double* prec,
            double* recall, double* f_measure, int* errors,
            double* avg_score,
            int start_index = 0,
            int end_index = std::numeric_limits<int>::max()) {
        start_index = std::min(start_index, (int)text.size());
        end_index = std::min(end_index, (int)text.size());

        int tp = 0, tn = 0, fp = 0, fn = 0;
        int e = 0;
        score_type ss = score_type();
        Scorer scorer(tagset);

        #pragma omp parallel for \
            default(shared) \
            reduction(+: tp, tn, fp, fn, e, ss)
        for (int i = start_index; i < end_index; ++i) {
            const Lexeme& lex = text[i];
            ss += scorer.score(lex.chosen_tag[phase], lex.expected_tag);
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
        if (avg_score)
            *avg_score = double(ss) / double(scorer.maxScore())
                / (end_index - start_index);
        if (errors)
            *errors = e;
    }

    void calculateTaggingStats(vector<Lexeme>& text,
            double* prec,
            double* recall, double* f_measure, int* errors,
            double* avg_score,
            int start_index = 0,
            int end_index = std::numeric_limits<int>::max()) {
        start_index = std::min(start_index, (int)text.size());
        end_index = std::min(end_index, (int)text.size());

        int phase = numPhases() - 1;
        int tp = 0, tn = 0, fp = 0, fn = 0;
        int e = 0;
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
        if (avg_score)
            *avg_score = double(ss) / double(scorer->maxScore())
                / (end_index - start_index);
        if (errors)
            *errors = e;
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

    void setText(vector<Lexeme>& text) {
        // Add sentinel segments.
        addSentinels(text, this->text);
    }

    bool isTrained() {
        return phase_generated_rules.size() == phase_tstores.size();
    }

    void initProgress(const wstring& msg) {
        mpi_world.barrier();
        if (mpi_world.rank() == 0) {
            this->progress.clear();
            this->progress.resize(mpi_world.size());
            this->progress_done = 0;
            this->progress_msg = msg;
        }
    }

    void printProgress() {
        float p = std::accumulate(progress.begin(), progress.end(), 0.0)
            / progress.size();
        wcerr << '\r' << this->progress_msg << boost::wformat(L"  %.2f%%") % (p*100.0);
    }

    void communicateProgress(float p) {
        if (mpi_world.rank() == 0) {
            boost::optional<mpi::status> status;
            while ((status = mpi_world.iprobe(mpi::any_source, MSG_PROGRESS))) {
                float recv_p;
                mpi_world.recv(status.get().source(), MSG_PROGRESS, recv_p);
                assert(progress[status.get().source()] <= recv_p);
                progress[status.get().source()] = recv_p;
            }

            progress[0] = p;
            printProgress();
        } else {
            mpi_world.isend(0, MSG_PROGRESS, p);
        }
    }

    void finishProgress() {
        if (mpi_world.rank() == 0) {
            this->progress_done++;
            progress[0] = 1.0;
            while (this->progress_done < mpi_world.size()) {
                mpi::status status = mpi_world.probe(mpi::any_source, mpi::any_tag);
                if (status.tag() == MSG_PROGRESS) {
                    float p;
                    mpi_world.recv(status.source(), MSG_PROGRESS, p);
                    assert(progress[status.source()] <= p);
                    progress[status.source()] = p;
                    printProgress();
                } else if (status.tag() == MSG_DONE) {
                    mpi_world.recv(status.source(), MSG_DONE);
                    progress[status.source()] = 1.0;
                    this->progress_done++;
                    printProgress();
                } else {
                    assert(false);
                }
            }

            wcerr << "  done." << endl;
        } else {
            mpi_world.send(0, MSG_DONE);
        }
        mpi_world.barrier();
    }

    void trainUnigramTaggers() {
        if (!phase_unigram_taggers.empty()) {
            assert(phase_unigram_taggers.size() == numPhases());
            return;
        }

        phase_unigram_taggers.resize(numPhases());
        for (int p = 0; p < numPhases(); p++) {
            if (mpi_world.rank() == 0)
                wcerr << "Training unigram tagger for phase " << p << "..." << endl;
            const Tagset* tagset = phase_tagsets[p];
            phase_unigram_taggers[p].train(text,
                    tagset == full_tagset ? NULL : tagset);
        }
    }

    void generateInitialTags(int phase, vector<Lexeme>& text_to_tag) {
        const Tagset* tagset = phase_tagsets[phase];

        trainUnigramTaggers();

        if (mpi_world.rank() == 0)
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
        if (mpi_world.rank() == 0)
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

        initProgress(boost::str(boost::wformat(
            L"Counting initial rules for phase %d ...") % phase));

        #pragma omp parallel for default(shared) \
                reduction(+: num_rules)
        for (int i = INDEX_OFFSET; i < n; ++i) {
            if (((++num) % 1000) == 0) {
                #pragma omp critical (generateInitialScores_progress)
                communicateProgress(num/(float)n);
            }
            Lexeme& lex = text[i];

            vector<Rule<Lexeme> > rules;
            rules_generator->generateUniqueRules(text, i, rules);
            BOOST_FOREACH(const Rule<Lexeme>& r, rules) {
                num_rules++;
                bucket_counts[hash_value(r) % num_buckets]++;
            }
        }

        finishProgress();
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
        initProgress(boost::str(boost::wformat(
            L"Generating initial rules for phase %d ...") % phase));

        #pragma omp parallel default(shared)
        {
            typedef std::pair<Rule<Lexeme>, score_type> score_changes_type;
            vector<score_changes_type> score_adds, score_subs;

            #pragma omp for
            for (int i = INDEX_OFFSET; i < n; ++i) {
                if (((++num) % 1000) == 0) {
                    #pragma omp critical (generateInitialScores_progress)
                    communicateProgress(num/(float)n);
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
                            wcerr << "RULE: " << ascii_to_wstring(r.asString(tstore)) << endl;
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
                    score_adds.push_back(make_pair(r, delta));
                }

                if (score_adds.size() + score_subs.size() > 10000)
                    applyScoreChanges(score_adds, score_subs);
            }

            applyScoreChanges(score_adds, score_subs);
        }

        finishProgress();
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
        if (mpi_world.rank() == 0)
            wcerr << "Gathering rules with score >= threshold ...  ";

        good_scores.clear();
        BOOST_FOREACH(const typename scores_map_type::value_type& j, this->scores) {
            if (ruleScore(j.second) >= threshold)
                good_scores[j.first] = j.second;
        }

        size_t total_good_rules;
        mpi::reduce(mpi_world, good_scores.size(), total_good_rules,
                std::plus<size_t>(), 0);
        if (mpi_world.rank() == 0)
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

            if (mpi_world.rank() == 0) {
                wcerr << boost::wformat(
                        L"(%d) CHOSEN RULE (good=%.1lf, bad=%.1lf, candidates=%d, good_candidates=%d): %s") %
                                round %
                                double(good) %
                                double(bad) %
                                int(scores.size()) %
                                int(good_scores.size()) %
                                b.asString(tstore).c_str() << endl;
            }

            if (mpi_world.rank() == 0 && DBG) {
                std::pair<score_type, score_type> good_scores = countScores(phase, b);
                if (good_scores != make_pair(good, bad)) {
                    wcerr << boost::wformat(L"\n*** WRONG SCORES ***"
                            "\n good is %.1lf, should be %.1lf"
                            "\n bad is %.1lf, should be %.1lf\n\n") %
                            double(good) %
                            double(good_scores.first) %
                            double(bad) %
                            double(good_scores.second);
                }
            }

            int errors = 0;
            double P, R, F, avg_score;
            applyRule(phase, b, text, next_text, INDEX_OFFSET,
                    text.size() - INDEX_OFFSET,
                    2 * (good + bad));

            if ((round % 10) == 1 && mpi_world.rank() == 0) {
                calculatePhaseStats(phase_tagset, phase, next_text, &P, &R, &F,
                        &errors, &avg_score,
                        INDEX_OFFSET, text.size() - INDEX_OFFSET);
                wcerr << boost::wformat(L"Applied. P=%lf R=%lf F=%lf avgScore=%lf errors: %d\n")
                        % P % R % F % avg_score % errors;
            }

            generated_rules.push_back(b);

            updateScores(phase, round, threshold, VICINITY);
            std::swap(text, next_text);
        }
    }

    score_type calculateRulePriority(score_type good, score_type bad) {
        //return (good + 1)/(bad + 1);
		return good - bad;
    }

    Rule<Lexeme> findBestRule(score_type threshold, bool* found, score_type* good, score_type* bad) {
        // [score, [rule, [good, bad]]]
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
        double prec = 0, recall = 0, f_measure = 0;

        calculateTaggingStats(tagged_text, &prec, &recall, &f_measure,
                NULL, NULL, INDEX_OFFSET, tagged_text.size() - INDEX_OFFSET);

        wcerr << boost::wformat(L"Stats -- P: %lf, R: %lf, F: %lf\n")
                % prec % recall % f_measure;
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
                        wcerr << "RULE: " << ascii_to_wstring(b.asString(tstore)) << endl;
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

    const vector<Lexeme>& getText() const {
        return text;
    }

    int numPhases() const {
        return phase_tstores.size();
    }

    void setAutoselectedTags(vector<Lexeme>& text) {
        int p = numPhases() - 1;
        BOOST_FOREACH(Lexeme& lex, text)
            lex.setAutoselectedTag(lex.chosen_tag[p]);
    }

    void tagText(vector<Lexeme>& text_to_tag) {
        vector<Lexeme> our_text_to_tag;
        addSentinels(text_to_tag, our_text_to_tag);

        scorer.reset(new Scorer(phase_tagsets[numPhases() - 1]));

        for (int i = 0; i < numPhases(); i++) {
            generateInitialTags(i, our_text_to_tag);
            applyAllRules(our_text_to_tag, i);
        }
        reportTaggingStats(our_text_to_tag);
        setAutoselectedTags(our_text_to_tag);

        text_to_tag.clear();
        text_to_tag.insert(text_to_tag.end(),
                our_text_to_tag.begin() + INDEX_OFFSET,
                our_text_to_tag.end() - INDEX_OFFSET);
    }

    void applyAllRules(vector<Lexeme>& text, int phase) {
        vector<Lexeme> next_text = text;
        double prec = 0, recall = 0, f_measure = 0, avg_score = 0;
        int last_errors = text.size();
        BOOST_FOREACH(Rule<Lexeme>& rule, phase_generated_rules[phase]) {
            int errors;
            applyRule(phase, rule, text, next_text, INDEX_OFFSET,
                    text.size() - INDEX_OFFSET, 0, false);
            calculatePhaseStats(phase_tagsets[phase], phase, next_text, &prec, &recall,
                    &f_measure, &errors, &avg_score, INDEX_OFFSET, text.size() -
                    INDEX_OFFSET);
            wcerr << boost::wformat(L"%d. Applied rule: %s (P: %lf, R: %lf, F: %lf, "
                    "score: %lf, errors: %d)\n") % phase %
                    ascii_to_wstring(rule.asString(phase_tstores[phase])) %
                    prec % recall % f_measure % avg_score % errors;

            if (errors > last_errors) {
                wcerr << boost::wformat(L"    Bad rule :(  errors was %d, now %d\n")
                        % last_errors % errors;
            }

            swap(text, next_text);
            last_errors = errors;
        }
    }

};

} // namespace BTagger

#endif /* BRILLENGINE_H_ */
