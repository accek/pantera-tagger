#ifndef STATS_H_
#define STATS_H_

#include <nlpcommon/scorer.h>

namespace NLPCommon {

template <class Lexeme>
class BaseLexemeWeighter
{
private:
	const Tagset* tagset;

public:
	BaseLexemeWeighter(const Tagset* tagset) : tagset(tagset) { }

	const Tagset* getTagset() { return tagset; }
	
	virtual double lexemeWeight(const Lexeme& lex) = 0;
};

template <class Lexeme>
class UniformLexemeWeighter : public BaseLexemeWeighter<Lexeme>
{
public:
	UniformLexemeWeighter(const Tagset* tagset)
		: BaseLexemeWeighter<Lexeme>(tagset) { }
	
	double lexemeWeight(const Lexeme& lex) {
		return 1.0;
	}
};

template <class Lexeme>
class AmbiguousOnlyLexemeWeighter : public BaseLexemeWeighter<Lexeme>
{
public:
	AmbiguousOnlyLexemeWeighter(const Tagset* tagset)
		: BaseLexemeWeighter<Lexeme>(tagset) { }
	
	double lexemeWeight(const Lexeme& lex) {
		return lex.getAllowedTags().size() > 1 ? 1.0 : 0.0;
	}
};

template <class Lexeme, class _CounterType = int>
class BaseTaggingStatistician
{
private:
	const Tagset* tagset;

protected:
	_CounterType _tpg, _tps, _sc, _wc, _num_golden, _num_selected, _num_tokens;

	void reset() {
		_tpg = _tps = _sc = _wc = _num_golden = _num_selected = _num_tokens = 0;
	}

	virtual void processLexeme(const Lexeme& lex) = 0;

public:
    typedef int score_type;
    typedef Tag tag_type;

    BaseTaggingStatistician(const Tagset* tagset) 
			: tagset(tagset) {
		reset();
	}

	const Tagset* getTagset() { return tagset; }

	void calculateStatistics(const vector<Lexeme>& text) {
		reset();
		addStatistics(text);
	}

	void addStatistics(const vector<Lexeme>& text) {
		BOOST_FOREACH(const Lexeme& lex, text)
			processLexeme(lex);
	}

	double getPrecision() {
		if (_tps == 0) return 0.0;
		return _tps / double(_num_selected);
	}

	double getRecall() {
		if (_tpg == 0) return 0.0;
		return _tpg / double(_num_golden);
	}

	double getFMeasure() {
		double p = getPrecision();
		double r = getRecall();
		if (p == 0 && r == 0) return 0.0;
		return 2 * p * r / (p + r);
	}

	double getStrongCorrectness() {
		if (_num_tokens == 0) return 0.0;
		return _sc / double(_num_tokens);
	}

	double getWeakCorrectness() {
		if (_num_tokens == 0) return 0.0;
		return _wc / double(_num_tokens);
	}
};

template <class Lexeme, class Scorer, class LexemeWeighter = UniformLexemeWeighter<Lexeme> >
class TaggingStatistician
		: public BaseTaggingStatistician<Lexeme, double> {

    BOOST_CONCEPT_ASSERT((MultiGoldenScorerConcept<Scorer>));

protected:
	Scorer scorer;
	LexemeWeighter lexeme_weighter;

	void processLexeme(const Lexeme& lex) {
		typedef typename Lexeme::tag_type tag_type;
		
		double wc = 0.0;
		BOOST_FOREACH(const tag_type& tag, lex.getAutoselectedTags())
			wc = std::max<double>(wc, scorer.score(tag, lex.getGoldenTags()));
		
		double sc = 1.0;
		BOOST_FOREACH(const tag_type& tag, lex.getAutoselectedTags())
			sc = std::min<double>(sc, scorer.score(tag, lex.getGoldenTags()));
		BOOST_FOREACH(const tag_type& tag, lex.getGoldenTags())
			sc = std::min<double>(sc, scorer.score(tag, lex.getAutoselectedTags()));

		BOOST_FOREACH(const tag_type& tag, lex.getAutoselectedTags())
			this->_tps += scorer.score(tag, lex.getGoldenTags());

		BOOST_FOREACH(const tag_type& tag, lex.getGoldenTags())
			this->_tpg += scorer.score(tag, lex.getAutoselectedTags());

		double weight = lexeme_weighter.lexemeWeight(lex);
		
		this->_wc += wc * weight;
		this->_sc += sc * weight;
		this->_num_tokens += weight;
		this->_num_selected += lex.getAutoselectedTags().size() * weight;
		this->_num_golden += lex.getGoldenTags().size() * weight;
	}

public:
    TaggingStatistician(const Tagset* tagset)
		: BaseTaggingStatistician<Lexeme, double>(tagset),
   		  scorer(tagset), lexeme_weighter(tagset) {
	}
};

} // namespace NLPCommon

#endif /* STATS_H_ */
