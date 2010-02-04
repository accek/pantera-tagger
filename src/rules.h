#ifndef _RULES_H_
#define _RULES_H_

#include <vector>
#include <cstdlib>
#include <functional>

#include <nlpcommon/lexeme.h>

namespace BTagger {

using namespace NLPCommon;

// --- Predicates ---

template<class Lexeme>
class Predicate;

template<class Lexeme>
class PredicateTemplate {
public:
    std::vector<const Tagset*> tagsets;

    typedef Lexeme lexeme_type;

    PredicateTemplate(const std::vector<const Tagset*>& tagsets)
            : tagsets(tagsets) { }
    virtual ~PredicateTemplate() { }

    virtual bool predicateMatches(const Predicate<Lexeme>& p,
            std::vector<Lexeme>& text, int index) = 0;
    virtual void findMatchingPredicates(std::vector<Predicate<Lexeme> >& v,
            std::vector<Lexeme>& text, int index) = 0;

    virtual string predicateAsString(const Predicate<Lexeme>& p) = 0;
};

template<class Lexeme>
class Predicate {
public:
    PredicateTemplate<Lexeme>* tpl;
    struct {
        typename Lexeme::tag_type tags[3];
        char chars[4];
        uint8_t categories[3];
        uint8_t values[3];
    } params;

    Predicate() :
        tpl(NULL) {
        std::memset(&params, 0xff, sizeof(params));
    }

    Predicate(PredicateTemplate<Lexeme>* tpl) :
        tpl(tpl) {
        std::memset(&params, 0xff, sizeof(params));
    }

    bool operator==(const Predicate<Lexeme>& r) const {
        return tpl == r.tpl && !std::memcmp(&params, &r.params, sizeof(params));
    }

    bool operator<(const Predicate<Lexeme>& r) const {
        if (tpl == r.tpl)
            return std::memcmp(&params, &r.params, sizeof(params)) < 0;
        return tpl < r.tpl;
    }
};

template<class Lexeme>
std::size_t hash_value(const Predicate<Lexeme>& pred) {
    std::size_t seed = 0;
    boost::hash_combine(seed, pred.tpl);
    boost::hash_range(seed, (char*)&pred.params, (char*)(&pred.params) + sizeof(pred.params));
    return seed;
}

// --- Actions ---

template<class Lexeme>
class Action;

template<class Lexeme>
class Rule;

template<class Lexeme>
class ActionTemplate {
public:
    std::vector<const Tagset*> tagsets;

    typedef Lexeme lexeme_type;
    typedef typename Lexeme::tag_type tag_type;

    ActionTemplate(const std::vector<const Tagset*>& tagsets)
            : tagsets(tagsets) { }
    virtual ~ActionTemplate() { }

    virtual bool actionApplicable(const Action<Lexeme>& a,
            std::vector<Lexeme>& text, int index) = 0;
    virtual void findMatchingRules(const Predicate<Lexeme>& p,
            std::vector<Rule<Lexeme> >& rules,
            std::vector<Lexeme>& text, int index) = 0;
    virtual const tag_type& changedTag(const Action<Lexeme>& a,
            const tag_type& original_tag) const = 0;

    virtual string actionAsString(const Action<Lexeme>& a) = 0;
};

template<class Lexeme>
class Action {
public:
    ActionTemplate<Lexeme>* tpl;
    struct {
        typename Lexeme::tag_type tag;
        uint8_t category;
    } params;

    Action() :
        tpl(NULL) {
        std::memset(&params, 0xff, sizeof(params));
    }

    Action(ActionTemplate<Lexeme>* tpl) :
        tpl(tpl) {
        std::memset(&params, 0xff, sizeof(params));
    }

    bool operator==(const Action<Lexeme>& r) const {
        return tpl == r.tpl && !std::memcmp(&params, &r.params, sizeof(params));
    }

    bool operator<(const Action<Lexeme>& r) const {
        if (tpl == r.tpl)
            return std::memcmp(&params, &r.params, sizeof(params)) < 0;
        return tpl < r.tpl;
    }
};

template<class Lexeme>
std::size_t hash_value(const Action<Lexeme>& act) {
    std::size_t seed = 0;
    boost::hash_combine(seed, act.tpl);
    boost::hash_range(seed, (char*)&act.params, (char*)(&act.params) + sizeof(act.params));
    return seed;
}

// --- Rules ---

template<class Lexeme>
class Rule {
public:
    Predicate<Lexeme> predicate;
    Action<Lexeme> action;

    typedef typename Lexeme::tag_type tag_type;

    Rule() { }

    Rule(const Predicate<Lexeme>& p, const Action<Lexeme>& a) :
        predicate(p), action(a) { }

    bool isApplicable(vector<Lexeme>& text, int index) const {
        return predicate.tpl->predicateMatches(predicate, text, index) &&
            action.tpl->actionApplicable(action, text, index);
    }

    const tag_type& changedTag(const Tag& original_tag) const {
        return action.tpl->changedTag(action, original_tag);
    }

    bool operator==(const Rule<Lexeme>& r) const {
        return r.action == action && r.predicate == predicate;
    }

    bool operator<(const Rule<Lexeme>& r) const {
        if (action < r.action)
            return true;
        if (action > r.action)
            return false;
        return predicate < r.predicate;
    }

    string asString() const {
        PredicateTemplate<Lexeme>* ptpl = predicate.tpl;
        ActionTemplate<Lexeme>* atpl = action.tpl;
        const Tagset* tagset = ptpl->tagsets[ptpl->tagsets.size() - 1];
        return string("(") + ptpl->predicateAsString(predicate)
                + string(") -> ") + atpl->actionAsString(action);
    }

    operator string() {
        return asString();
    }
};

template<class Lexeme>
std::size_t hash_value(const Rule<Lexeme>& rule) {
    std::size_t seed = 0;
    boost::hash_combine(seed, rule.predicate);
    boost::hash_combine(seed, rule.action);
    return seed;
}

template<class Lexeme>
class RulesGenerator {
public:
    virtual void generateRules(vector<Lexeme>& text, int index,
            vector<Rule<Lexeme> >& rules) = 0;
};

} // namespace BTagger

#endif
