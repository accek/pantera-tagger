#ifndef _RULES_H_
#define _RULES_H_

#include <vector>
#include <cstdlib>
#include <functional>

#include "lexeme.h"

namespace BTagger {

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
		typename Lexeme::tag_type segment_tag;
		typename Lexeme::tag_type segment_subtag;
		union {
			typename Lexeme::tag_type tag;
			typename Lexeme::tag_type tags[2];
			char chars[4];
		};
	} params;

	Predicate() :
		tpl(NULL) {
	}

	Predicate(PredicateTemplate<Lexeme>* tpl) :
		tpl(tpl) {
		std::memset(&params, 0xff, sizeof(params));
	}

	bool operator==(const Predicate<Lexeme>& r) const {
		return !std::memcmp(this, &r, sizeof(Predicate<Lexeme>));
	}

	bool operator<(const Predicate<Lexeme>& r) const {
		return std::memcmp(this, &r, sizeof(Predicate<Lexeme>)) < 0;
	}
};

template<class Lexeme>
std::size_t hash_value(const Predicate<Lexeme>& pred) {
    std::size_t seed = 0;
    boost::hash_combine(seed, pred.tpl);
    boost::hash_combine(seed, pred.params.segment_tag);
    boost::hash_combine(seed, pred.params.segment_subtag);
    boost::hash_combine(seed, pred.params.tags[0]);
    boost::hash_combine(seed, pred.params.tags[1]);
    return seed;
}

template<class Lexeme>
class Rule {
public:
	Predicate<Lexeme> predicate;
	typename Lexeme::tag_type tag;

	Rule() { }

	Rule(const Predicate<Lexeme>& p, typename Lexeme::tag_type tag) :
		predicate(p), tag(tag) { }

	bool operator==(const Rule<Lexeme>& r) const {
		return r.tag == tag && r.predicate == predicate;
	}

	bool operator<(const Rule<Lexeme>& r) const {
		if (tag < r.tag)
			return true;
		if (tag > r.tag)
			return false;
		return predicate < r.predicate;
	}

	string asString() const {
		PredicateTemplate<Lexeme>* tpl = predicate.tpl;
		const Tagset* tagset = tpl->tagsets[tpl->tagsets.size() - 1];
		return string("(") + predicate.tpl->predicateAsString(predicate)
				+ string(") -> ") + tag.asString(tagset);
	}

	operator string() {
		return asString();
	}
};

template<class Lexeme>
std::size_t hash_value(const Rule<Lexeme>& rule) {
    std::size_t seed = 0;
    boost::hash_combine(seed, rule.predicate);
    boost::hash_combine(seed, rule.tag);
    return seed;
}

} // namespace BTagger

#endif
