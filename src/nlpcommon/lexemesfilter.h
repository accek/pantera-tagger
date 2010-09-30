/*
 * writer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LEXEMESFILTER_H_
#define LEXEMESFILTER_H_

#include <istream>
#include <vector>

#include <nlpcommon/lexeme.h>
#include <nlpcommon/progress.h>
#include <nlpcommon/exception.h>

namespace NLPCommon {

using std::vector;

class LexemesFilterException : public Exception
{
public:
    LexemesFilterException(const string& msg) : Exception(msg) { }
    virtual ~LexemesFilterException() throw () { }
};

template<class Lexeme = DefaultLexeme>
class LexemesFilter
{
protected:
	vector<Lexeme> _original_text;
	int _type_mask;
    bool _only_accepted_segments;

public:
	LexemesFilter(int type_mask, bool only_accepted_segments = true)
        : _type_mask(type_mask),
          _only_accepted_segments(only_accepted_segments)
    {
        if (only_accepted_segments) {
            _type_mask &= ~(Lexeme::START_OF_AMBIGUITY|
                    Lexeme::UNRESOLVED_FRAGMENT|
                    Lexeme::ACCEPTED_FRAGMENT|
                    Lexeme::REJECTED_FRAGMENT|
                    Lexeme::END_OF_AMBIGUITY);
        }
    }

	vector<Lexeme> filterText(vector<Lexeme>& original) {
		this->_original_text = original;
		vector<Lexeme> ret;

        bool in_rejected_fragment = false;
		BOOST_FOREACH(const Lexeme& lex, this->_original_text) {
            if (lex.getType() == Lexeme::REJECTED_FRAGMENT) {
                in_rejected_fragment = true;
            } else if (lex.getType() == Lexeme::ACCEPTED_FRAGMENT ||
                    lex.getType() == Lexeme::END_OF_AMBIGUITY) {
                in_rejected_fragment = false;
            } else if (lex.getType() == Lexeme::UNRESOLVED_FRAGMENT
                    && _only_accepted_segments) {
                throw Exception("Unresolved segmentation cannot be handled "
                        "by LexemesFilter. Text must be segmentation-"
                        "disambiguated before filtering with "
                        "only_accepted_segments.");
            }
			if (((int)lex.getType() & this->_type_mask) != 0
                    && (!_only_accepted_segments || !in_rejected_fragment)) {
				ret.push_back(lex);
			}
		}
		
		return ret;
	}

	vector<Lexeme> unfilterText(const vector<Lexeme>& text) {
		int i = 0;
		vector<Lexeme> ret;

        bool in_rejected_fragment = false;
		BOOST_FOREACH(const Lexeme& lex, this->_original_text) {
            if (lex.getType() == Lexeme::REJECTED_FRAGMENT) {
                in_rejected_fragment = true;
            } else if (lex.getType() == Lexeme::ACCEPTED_FRAGMENT ||
                    lex.getType() == Lexeme::END_OF_AMBIGUITY) {
                in_rejected_fragment = false;
            }
			if (((int)lex.getType() & this->_type_mask) != 0
                    && (!_only_accepted_segments || !in_rejected_fragment)) {
				const Lexeme& tlex = text[i++];
				if (lex.getType() != tlex.getType()) {
					throw LexemesFilterException(boost::str(
								boost::format("Lexeme type mismatch at "
									"position %1% while unfiltering text.")
								% i));
				}
				if (lex.getOrth() != tlex.getOrth()) {
					throw LexemesFilterException(boost::str(
								boost::format("Lexeme orth mismatch at "
									"position %1% while unfiltering text.")
								% i));
				}
				ret.push_back(tlex);
			} else {
				ret.push_back(lex);
			}
		}

		return ret;
	}
};


} // namespace NLPCommon

#endif /* LEXEMESFILTER_H_ */
