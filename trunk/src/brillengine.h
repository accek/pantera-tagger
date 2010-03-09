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
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <cassert>
#include <limits>
#include <algorithm>

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
        MSG_PROGRESS = 1
    };

    UnigramTagger<Lexeme> unigram_tagger;
    const Tagset* full_tagset;
    vector<Lexeme> text;
    vector<Lexeme> next_text;
    int phase;
    const Tagset* phase_tagset;
    RulesGenerator<Lexeme>* rules_generator;
    TemplatesStore<Lexeme>* tstore;
    scores_map_type scores;
    vector<Rule<Lexeme> > generated_rules;
    bool cancelled;
    boost::scoped_ptr<Scorer> scorer;
    vector<float> progress;

    vector<const Tagset*> phase_tagsets;
    vector<TemplatesStore<Lexeme>*> phase_tstores;
    vector<vector<Rule<Lexeme> > > phase_generated_rules;

    mpi::communicator mpi_world;

    static const int INDEX_OFFSET = 4;
    static const int DBG = 0;
    static const int VICINITY = 3;

    void applyRule(int phase, Rule<Lexeme>& b, vector<Lexeme>& text1,
            vector<Lexeme>& text2,
            int start_index = 0,
            int end_index = std::numeric_limits<int>::max(),
            int vicinity_value = 0, int vicinity_radius = 0) {
        assert(text1.size() == text2.size());
        start_index = std::min(start_index, (int)text1.size());
        end_index = std::min(end_index, (int)text1.size());
        #pragma omp parallel for default(shared)
        for (int i = start_index; i < end_index; ++i) {
            if (b.isApplicable(tstore, text1, i)) {
                text2[i].chosen_tag[phase] = b.changedTag(tstore, text1, i);
                for (int j = std::max(start_index, i - vicinity_radius),
                         z = std::min(end_index - 1, i + vicinity_radius);
                         j <= z;
                         ++j) {
                    text2[j].vicinity = vicinity_value;
                }
            } else {
                text2[i].chosen_tag[phase] = text1[i].chosen_tag[phase];
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

    tag_type findBestInitialTag(Lexeme& lexeme,
            const Tagset* new_tagset, const Tagset* previous_tagset,
             const tag_type& previous_tag = tag_type::getNullTag())
    {
        const wstring& orth = lexeme.getOrth();

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

    void init(vector<Lexeme>& text, const Tagset* full_tagset) {
        // Add sentinel segments.
        addSentinels(text, this->text);

        this->full_tagset = full_tagset;
        this->phase = -1;
    }

    void resetProgress() {
        this->progress.clear();
        this->progress.resize(mpi_world.size());
    }

    void communicateProgress(const wstring& msg, float p) {
        if (mpi_world.rank() == 0) {
            boost::optional<mpi::status> status;
            while ((status = mpi_world.iprobe(mpi::any_source, MSG_PROGRESS))) {
                float recv_p;
                mpi_world.recv(status.get().source(), MSG_PROGRESS, recv_p);
                progress[status.get().source()] = recv_p;
            }

            progress[0] = p;
            p = std::accumulate(progress.begin(), progress.end(), 0.0)
                / progress.size();

            wcerr << '\r' << msg << boost::wformat(L"  %.2f%%") % (p*100.0);
        } else {
            mpi_world.isend(0, MSG_PROGRESS, p);
        }
    }

    void finishProgress() {
        mpi_world.barrier();
        if (mpi_world.rank() == 0) {
            boost::optional<mpi::status> status;
            float p;
            while ((status = mpi_world.iprobe(mpi::any_source, MSG_PROGRESS))) {
                mpi_world.recv(status.get().source(), MSG_PROGRESS, p);
            }

            wcerr << "  done." << endl;
        }
    }

    void generateInitialTags(int phase, vector<Lexeme>& text_to_tag) {
        const Tagset* tagset = phase_tagsets[phase];

        if (mpi_world.rank() == 0)
            wcerr << "Training unigram tagger ...";
        unigram_tagger.clear();
        unigram_tagger.train(text, tagset == full_tagset ? NULL : tagset);
        mpi_world.barrier();
        if (mpi_world.rank() == 0)
            wcerr << "  done." << endl;

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
                    findBestInitialTag(lexeme, tagset, previous_tagset,
                            previous_tag);
            lexeme.considered_tags = calculateConsideredTags(lexeme, tagset,
                    previous_tagset, previous_tag);
            lexeme.expected_tag = findGoldenTag(lexeme, tagset, previous_tagset,
                    previous_tag);
        }
        mpi_world.barrier();
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

    void generateInitialScores()
    {
        scores.clear();

        if (mpi_world.rank() == 0) {
            wcerr << "Generating initial rules for phase " << phase << " ...  ";
            resetProgress();
        }

        int n = text.size() - INDEX_OFFSET;
        int num = 0;

        // TODO: move somewhere
        size_t num_buckets = 16 * (1 << 20);
        int min_rule_count = 3;

        vector<uint32_t> bucket_counts;
        bucket_counts.resize(num_buckets);
        int num_rules = 0;

        #pragma omp parallel for default(shared) \
                reduction(+: num_rules)
        for (int i = INDEX_OFFSET; i < n; ++i) {
            if (((++num) % 1000) == 0) {
                #pragma omp critical (generateInitialScores_progress)
                communicateProgress(boost::str(boost::wformat(
                    L"Counting initial rules for phase %d ...") % phase),
                        num/(float)n);
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

        num = 0;
        resetProgress();
        #pragma omp parallel default(shared)
        {
            typedef std::pair<Rule<Lexeme>, score_type> score_changes_type;
            vector<score_changes_type> score_adds, score_subs;

            #pragma omp for
            for (int i = INDEX_OFFSET; i < n; ++i) {
                if (((++num) % 1000) == 0) {
                    #pragma omp critical (generateInitialScores_progress)
                    communicateProgress(boost::str(boost::wformat(
                        L"Generating initial rules for phase %d ...") % phase),
                            num/float(n));
                }
                Lexeme& lex = text[i];

                vector<Rule<Lexeme> > rules;
                rules_generator->generateUniqueRules(text, i, rules);

                if (DBG) {
                    wcerr << "Init i: " << i << '(' << lex.getOrth() <<
                        ") num_rules: " << rules.size();
                    if (rules.size() > 0)
                        wcerr << ascii_to_wstring(rules[0].asString(tstore));
                    wcerr << endl;
                }

                if (DBG) {
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
                }

                BOOST_FOREACH(const Rule<Lexeme>& r, rules) {
                    if (!r.isApplicable(tstore, text, i)) {
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

                    if (!buckets_map[hash_value(r) % num_buckets])
                        continue;

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

    void updateScores(int round) {
        int n = text.size() - INDEX_OFFSET;
        #pragma omp parallel default(shared)
        {
            typedef std::pair<Rule<Lexeme>, score_type> score_changes_type;
            vector<score_changes_type> score_adds, score_subs;

            #pragma omp for
            for (int i = INDEX_OFFSET; i < n; ++i) {
                Lexeme& lex1 = text[i];
                Lexeme& lex2 = next_text[i];

                if (lex2.vicinity != round) continue;

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
                    if (s != 0)
                        score_subs.push_back(make_pair(r, s));
                }

                // For each predicate which matches now, we add the scores.
                rules.clear();
                rules_generator->generateUniqueRules(next_text, i, rules);
                BOOST_FOREACH(const Rule<Lexeme>& r, rules) {
                    score_type s = scoreDelta(chosen_tag2,
                            r.changedTag(tstore, next_text, i), expected_tag);
                    if (s != 0)
                        score_adds.push_back(make_pair(r, s));
                }

                if (score_adds.size() + score_subs.size() > 10000)
                    applyScoreChanges(score_adds, score_subs);
            }

            applyScoreChanges(score_adds, score_subs);
        }
        mpi_world.barrier();
    }

    void process(int threshold) {

        // TODO: Wlepic napis, skad pochodzi ten algorytm.

        if (scores.size() == 0) return;

        for (int round = 1; ; round++) {
            score_type f;
            Rule<Lexeme> b = this->findBestRule(&f);

            if (f < threshold || cancelled) break;

            if (mpi_world.rank() == 0) {
                wcerr << boost::wformat(
                        L"(%d) CHOSEN RULE (good=%.1lf, bad=%.1lf, candidates=%d): %s") %
                                round %
                                double(scores[b].first) %
                                double(scores[b].second) %
                                int(scores.size()) %
                                b.asString(tstore).c_str() << endl;
            }

            if (scores.count(b) > 0) {
                std::pair<score_type, score_type> good_scores = countScores(b);
                if (good_scores != make_pair(scores[b].first, scores[b].second)) {
                    wcerr << boost::wformat(L"\n*** WRONG SCORES ***"
                            "\n good is %.1lf, should be %.1lf"
                            "\n bad is %.1lf, should be %.1lf\n\n") %
                            double(scores[b].first) %
                            double(good_scores.first) %
                            double(scores[b].second) %
                            double(good_scores.second);
                }
            }

            int errors = 0;
            double P, R, F, avg_score;
            applyRule(phase, b, text, next_text, INDEX_OFFSET,
                    text.size() - INDEX_OFFSET, round, VICINITY);

            if ((round % 10) == 1 && mpi_world.rank() == 0) {
                calculatePhaseStats(phase_tagset, phase, next_text, &P, &R, &F,
                        &errors, &avg_score,
                        INDEX_OFFSET, text.size() - INDEX_OFFSET);
                wcerr << boost::wformat(L"Applied. P=%lf R=%lf F=%lf avgScore=%lf errors: %d\n")
                        % P % R % F % avg_score % errors;
            }

            generated_rules.push_back(b);

            updateScores(round);
            std::swap(text, next_text);
        }
    }

    Rule<Lexeme> findBestRule(score_type* max) {
        typedef std::pair<score_type, Rule<Lexeme> > max_type;

        max_type gmax = std::make_pair(
                scores.begin()->second.first - scores.begin()->second.second,
                scores.begin()->first);

        if (DBG) {
            wcerr << "Rules: " << endl;
        }

        int n = scores.bucket_count();

        #pragma omp parallel default(shared)
        {
            max_type max = gmax;

            #pragma omp for
            for (int i = 0; i < n; i++) {
                typename scores_map_type::const_local_iterator end = scores.end(i);
                for (typename scores_map_type::const_local_iterator j = scores.begin(i);
                        j != end; j++) {
                    if (DBG) {
                        wcerr << "\t" << ascii_to_wstring(j->first.asString(tstore))
                            << " (good: " << j->second.first
                            << ", bad: " << j->second.second << ")" << endl;
                        assert(countScores(j->first)
                                == std::make_pair(j->second.first, j->second.second));
                    }

                    max_type val = std::make_pair(
                            j->second.first - j->second.second,
                            j->first);
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

        *max = gmax.first;
        return gmax.second;
    }

    void addPhase(const Tagset* tagset, TemplatesStore<Lexeme>* tstore) {
        phase_tagsets.push_back(tagset);
        phase_tstores.push_back(tstore);
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

        this->phase = phase;
        this->rules_generator = rules_generator;
        this->phase_tagset = phase_tagsets[phase];
        this->tstore = phase_tstores[phase];

        scorer.reset(new Scorer(phase_tagset));
        assert(this->tstore == rules_generator->getTStore());

        generated_rules.clear();
        cancelled = false;

        try {
            distributePTemplates();
            generateInitialTags(phase, text);
            generateInitialScores();
            next_text = text;
            process(threshold);

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

    std::pair<score_type, score_type> countScores(const Rule<Lexeme>& b) {
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

    int getPhase() const {
        return phase;
    }

    void setAutoselectedTags(vector<Lexeme>& text) {
        BOOST_FOREACH(Lexeme& lex, text)
            lex.setAutoselectedTag(lex.chosen_tag[phase]);
    }

    void tagText(vector<Lexeme>& text_to_tag) {
        vector<Lexeme> our_text_to_tag;
        addSentinels(text_to_tag, our_text_to_tag);

        for (int i = 0; i <= phase; i++) {
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
                    text.size() - INDEX_OFFSET);
            calculatePhaseStats(phase_tagset, phase, next_text, &prec, &recall,
                    &f_measure, &errors, &avg_score, INDEX_OFFSET, text.size() -
                    INDEX_OFFSET);
            wcerr << boost::wformat(L"%d. Applied rule: %s (P: %lf, R: %lf, F: %lf, "
                    "score: %lf, errors: %d)\n") % phase % ascii_to_wstring(rule.asString(tstore)) %
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
