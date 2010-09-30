#ifndef _RULES_H_
#define _RULES_H_

#include <boost/functional/hash.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/binary_object.hpp>
#include <vector>
#include <cstdlib>
#include <functional>
#include <algorithm>
#include <locale>

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
    int id;

    typedef Lexeme lexeme_type;

    PredicateTemplate(const std::vector<const Tagset*>& tagsets)
            : tagsets(tagsets), id(-1) { }
    virtual ~PredicateTemplate() { }

    void setId(int new_id) {
        assert(id == -1);
        id = new_id;
    }

    virtual bool predicateMatches(const Predicate<Lexeme>& p,
            std::vector<Lexeme>& text, int index) = 0;
    virtual void findMatchingPredicates(std::vector<Predicate<Lexeme> >& v,
            std::vector<Lexeme>& text, int index) = 0;

    virtual wstring predicateAsWString(const Predicate<Lexeme>& p) = 0;

    virtual bool usesCategory0() {
        return false;
    }
};

template<class Lexeme>
class Predicate {
public:
    int tpl_id;
    struct {
        union {
            typename Lexeme::tag_type tags[4];
            struct {
                uint8_t categories[4];
                uint8_t values[4];
                uint8_t pos[4];
            };
        };
        wchar_t chars[4];
        uint16_t rule_number;
    } params;

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & tpl_id;
        boost::serialization::binary_object bo(&params, sizeof(params));
        ar & bo;
    }

public:
    Predicate() :
        tpl_id(-1) {
        std::memset(&params, 0xff, sizeof(params));
    }

    Predicate(PredicateTemplate<Lexeme>* tpl) :
        tpl_id(tpl->id) {
        std::memset(&params, 0xff, sizeof(params));
    }

    bool operator==(const Predicate<Lexeme>& r) const {
        return tpl_id == r.tpl_id
            && !std::memcmp(&params, &r.params, sizeof(params));
    }

    bool operator<(const Predicate<Lexeme>& r) const {
        if (tpl_id == r.tpl_id)
            return std::memcmp(&params, &r.params, sizeof(params)) < 0;
        return tpl_id < r.tpl_id;
    }
};

template<class Lexeme>
std::size_t hash_value(const Predicate<Lexeme>& pred) {
    std::size_t seed = 0;
    boost::hash_combine(seed, pred.tpl_id);
    boost::hash_range(seed, (char*)&pred.params, (char*)(&pred.params) + sizeof(pred.params));
    return seed;
}

// --- Actions ---

template<class Lexeme>
class Action;

template<class Lexeme>
class Rule;

template<class Lexeme>
class TemplatesStore;

template<class Lexeme>
class ActionTemplate {
public:
    std::vector<const Tagset*> tagsets;
    int id;

    typedef Lexeme lexeme_type;
    typedef typename Lexeme::tag_type tag_type;

    ActionTemplate(const std::vector<const Tagset*>& tagsets)
            : tagsets(tagsets), id(-1) { }
    virtual ~ActionTemplate() { }

    void setId(int new_id) {
        assert(id == -1);
        id = new_id;
    }

    virtual bool actionApplicable(const Action<Lexeme>& a,
            std::vector<Lexeme>& text, int index) = 0;
    virtual void findMatchingRules(const TemplatesStore<Lexeme>* tstore,
            const Predicate<Lexeme>& p,
            std::vector<Rule<Lexeme> >& rules,
            std::vector<Lexeme>& text, int index) = 0;
    virtual tag_type changedTag(const Action<Lexeme>& a,
            std::vector<Lexeme>& text, int index) const = 0;

    virtual wstring actionAsWString(const Action<Lexeme>& a) = 0;
};

template<class Lexeme>
class Action {
public:
    //ActionTemplate<Lexeme>* tpl;
    int tpl_id;
    struct {
        typename Lexeme::tag_type tag;
        uint8_t category;
        uint8_t value;
        uint8_t pos;
    } params;

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & tpl_id;
        ar & params.tag;
        ar & params.category;
        ar & params.value;
        ar & params.pos;
    }

public:
    Action() :
        tpl_id(-1) {
        std::memset(&params, 0xff, sizeof(params));
    }

    Action(ActionTemplate<Lexeme>* tpl) :
        tpl_id(tpl->id) {
        std::memset(&params, 0xff, sizeof(params));
    }

    bool operator==(const Action<Lexeme>& r) const {
        return tpl_id == r.tpl_id
            && !std::memcmp(&params, &r.params, sizeof(params));
    }

    bool operator<(const Action<Lexeme>& r) const {
        if (tpl_id == r.tpl_id)
            return std::memcmp(&params, &r.params, sizeof(params)) < 0;
        return tpl_id < r.tpl_id;
    }
};

template<class Lexeme>
std::size_t hash_value(const Action<Lexeme>& act) {
    std::size_t seed = 0;
    boost::hash_combine(seed, act.tpl_id);
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

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & predicate;
        ar & action;
    }

public:
    Rule() { }

    Rule(const Predicate<Lexeme>& p, const Action<Lexeme>& a) :
        predicate(p), action(a) { }

    bool isApplicable(const TemplatesStore<Lexeme>* tstore,
            vector<Lexeme>& text, int index) const {
        return tstore->getPTemplate(predicate.tpl_id)
                ->predicateMatches(predicate, text, index) &&
            tstore->getATemplate(action.tpl_id)
                ->actionApplicable(action, text, index);
    }

    tag_type changedTag(const TemplatesStore<Lexeme>* tstore,
            std::vector<Lexeme>& text, int index) const {
        return tstore->getATemplate(action.tpl_id)
            ->changedTag(action, text, index);
    }

    bool operator==(const Rule<Lexeme>& r) const {
        return r.action == action && r.predicate == predicate;
    }

    bool operator<(const Rule<Lexeme>& r) const {
        if (action < r.action)
            return true;
        if (r.action < action)
            return false;
        return predicate < r.predicate;
    }

    wstring asWString(const TemplatesStore<Lexeme>* tstore) const {
        PredicateTemplate<Lexeme>* ptpl = tstore->getPTemplate(predicate.tpl_id);
        ActionTemplate<Lexeme>* atpl = tstore->getATemplate(action.tpl_id);
        const Tagset* tagset = ptpl->tagsets[ptpl->tagsets.size() - 1];
        return wstring(L"(") + ptpl->predicateAsWString(predicate)
                + wstring(L") -> ") + atpl->actionAsWString(action);
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
class TemplatesStore {
private:
    vector<PredicateTemplate<Lexeme>*> _ptemplates;
    vector<ActionTemplate<Lexeme>*> _atemplates;
    vector<PredicateTemplate<Lexeme>*> _enabled_ptemplates;
    vector<ActionTemplate<Lexeme>*> _enabled_atemplates;
    vector<int> _enabled_aids;
    vector<int> _enabled_pids;

    void updateEnabledTemplates() {
        _enabled_atemplates.clear();
        _enabled_ptemplates.clear();
        BOOST_FOREACH(int i, _enabled_aids)
            _enabled_atemplates.push_back(_atemplates[i]);
        BOOST_FOREACH(int i, _enabled_pids)
            _enabled_ptemplates.push_back(_ptemplates[i]);
    }

public:
    TemplatesStore(const vector<PredicateTemplate<Lexeme>*>& ptemplates,
            const vector<ActionTemplate<Lexeme>*> atemplates) :
        _ptemplates(ptemplates), _atemplates(atemplates)
    {
        for (int i = 0; i < ptemplates.size(); i++) {
            ptemplates[i]->setId(i);
            _enabled_pids.push_back(i);
        }
        for (int i = 0; i < atemplates.size(); i++) {
            atemplates[i]->setId(i);
            _enabled_aids.push_back(i);
        }
        updateEnabledTemplates();
    }

    ~TemplatesStore() {
        BOOST_FOREACH(PredicateTemplate<Lexeme>* ptmpl, _ptemplates)
            delete ptmpl;
        BOOST_FOREACH(ActionTemplate<Lexeme>* atmpl, _atemplates)
            delete atmpl;
    }

    void setEnabledPTemplates(const vector<int> ids) {
        _enabled_pids = ids;
        updateEnabledTemplates();
    }

    void setEnabledATemplates(const vector<int> ids) {
        _enabled_aids = ids;
        updateEnabledTemplates();
    }

    PredicateTemplate<Lexeme>* getPTemplate(int id) const {
        return _ptemplates[id];
    }

    ActionTemplate<Lexeme>* getATemplate(int id) const {
        return _atemplates[id];
    }

    const vector<PredicateTemplate<Lexeme>*> getPTemplates() const {
        return _enabled_ptemplates;
    }

    const vector<ActionTemplate<Lexeme>*> getATemplates() const {
        return _enabled_atemplates;
    }

    const vector<PredicateTemplate<Lexeme>*> getAllPTemplates() const {
        return _ptemplates;
    }

    const vector<ActionTemplate<Lexeme>*> getAllATemplates() const {
        return _atemplates;
    }
};

template<class Lexeme>
class RulesGenerator {
protected:
    TemplatesStore<Lexeme>* tstore;

public:
    RulesGenerator(TemplatesStore<Lexeme>* tstore)
        : tstore(tstore) { }

    TemplatesStore<Lexeme>* getTStore() const { return tstore; }

    virtual void generateRules(vector<Lexeme>& text, int index,
            vector<Rule<Lexeme> >& rules) const = 0;

    virtual void generateUniqueRules(vector<Lexeme>& text, int index,
            vector<Rule<Lexeme> >& rules) const {
        generateRules(text, index, rules);
        std::sort(rules.begin(), rules.end());
        rules.resize(std::unique(rules.begin(), rules.end()) - rules.begin());
    }

};

} // namespace BTagger

#endif
