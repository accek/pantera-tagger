/*
 * writer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef MORFEUSZANALYZER_H_
#define MORFEUSZANALYZER_H_

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/range/iterator_range.hpp>

#include <iostream>
#include <vector>
#include <morfeusz.h>
#include <guesser_api.h>

#include <nlpcommon/lexeme.h>
#include <nlpcommon/progress.h>
#include <nlpcommon/exception.h>
#include <nlpcommon/util.h>

namespace NLPCommon {

using std::vector;

class MorfeuszAnalyzerException : public Exception
{
public:
    MorfeuszAnalyzerException(const string& msg) : Exception(msg) { }
    virtual ~MorfeuszAnalyzerException() throw () { }
};

template<class Lexeme = DefaultLexeme>
class MorfeuszAnalyzer
{
public:
    typedef typename Lexeme::tag_type tag_type;

protected:
    typedef boost::split_iterator<string::const_iterator> string_split_iterator;

    const Tagset* tagset;

    void parseMorfeuszTagSuffix(
            tag_type& tag,
            const PartOfSpeech* pos,
            int category_offset,
            const string_split_iterator& part,
            const wstring& base,
            Lexeme& lex) {
        if (part.eof()) {
            int num_cats = pos->getCategories().size();
            while (category_offset < num_cats) {
                const Category* cat = pos->getCategories()[category_offset];
                int cindex = tagset->getCategoryIndex(cat);
                tag.setValue(cindex, cat->getIndex("[none]"));
                category_offset++;
            }

            //std::cerr << "adding " << tag.asString(tagset) << " " <<
            //    wstring_to_utf8(base) << std::endl;
            lex.addTagBase(tag, base);
            lex.addAllowedTag(tag);
            return;
        }

        const Category* cat = pos->getCategories()[category_offset];
        int cindex = tagset->getCategoryIndex(cat);
        string_split_iterator new_part = part;
        ++new_part;
        /*
        std::cerr << "parsing " << boost::copy_range<string>(*part) <<
            " with category " << cat->getName() << " offset is "
            << category_offset << " new part eof " << new_part.eof() << ' ' <<
            part.eof() << std::endl;
        */

        if (part->front() == '_') {
            int num_values = cat->getValues().size();
            for (int i = 0; i < num_values; i++) {
                tag.setValue(cindex, i);
                parseMorfeuszTagSuffix(tag, pos, category_offset + 1,
                        new_part, base, lex);
            }
        } else {
            for (string_split_iterator value_it =
                    boost::make_split_iterator(*part, boost::token_finder(
                            boost::is_any_of(".")));
                    value_it != string_split_iterator(); ++value_it) {
                tag.setValue(cindex,
                        cat->getIndex(boost::copy_range<string>(*value_it)));
                parseMorfeuszTagSuffix(tag, pos, category_offset + 1,
                        new_part, base, lex);
            }
        }
    }

public:
	MorfeuszAnalyzer(const Tagset* tagset) : tagset(tagset) { }

    void parseMorfeuszTag(const string_split_iterator& mtag, const wstring& base,
            Lexeme& lex) {
        string_split_iterator cat_it =
                boost::make_split_iterator(*mtag, boost::token_finder(
                            boost::is_any_of(":")));
        const PartOfSpeech* pos = tagset->getPartOfSpeech(
                boost::copy_range<string>(*cat_it));
        int pos_idx = tagset->getPartOfSpeechIndex(pos);
        tag_type tag;
        tag.setPos(pos_idx);
        parseMorfeuszTagSuffix(tag, pos, 0, ++cat_it, base, lex);
    }

    void parseMorfeuszTags(const string& mtags, const wstring& base,
            Lexeme& lex) {
        for (string_split_iterator tag_it =
                boost::make_split_iterator(mtags, boost::token_finder(
                        boost::is_any_of("|")));
                tag_it != string_split_iterator(); ++tag_it) {
            parseMorfeuszTag(tag_it, base, lex);
        }
    }

	void parseOdgadywaczResponse(const string& forms, Lexeme& lex) {
        for (string_split_iterator segment_it =
                boost::make_split_iterator(forms, boost::token_finder(
                        boost::is_any_of("\n")));
                segment_it != string_split_iterator(); ++segment_it) {
            if (segment_it->begin() == segment_it->end())
                break;

            string_split_iterator interp_it = boost::make_split_iterator(
                    *segment_it, boost::token_finder(boost::is_any_of("\t")));
            lex.setUtf8Orth(boost::copy_range<string>(*interp_it));
            
            for (++interp_it; interp_it != string_split_iterator(); ++interp_it)
            {
                vector<boost::iterator_range<string::const_iterator> > parts;
                boost::split(parts, *interp_it, boost::is_any_of(" "));
                tag_type tag = tag_type::parseString(tagset,
                        boost::copy_range<string>(parts[1]));
                lex.addAllowedTag(tag);
                lex.addTagBase(tag,
                        utf8_to_wstring(boost::copy_range<string>(parts[0])));
            }
        }
    }

	vector<Lexeme> analyzeText(const vector<Lexeme>& text) {
        vector<Lexeme> ret;
        int tidx = -1;
		BOOST_FOREACH(const Lexeme& lex, text) {
            tidx++;

            if (lex.getType() != Lexeme::SEGMENT) {
                ret.push_back(lex);
                continue;
            }

            morfeusz_set_option(MORFOPT_ENCODING, MORFEUSZ_UTF_8);
            InterpMorf* interps = morfeusz_analyse(
                    const_cast<char*>(lex.getUtf8Orth().c_str()));

            Lexeme current_lex;
            int segm = -1;
            for (int i = 0; ; i++) {
                InterpMorf& interp = interps[i];
                if (interp.p == -1)
                    break;
                if (interp.p + 1 != interp.k || interp.p < segm
                        || interp.p > segm + 1) {
					/*throw MorfeuszAnalyzerException(boost::str(
								boost::format("Ambiguous interpretation "
                                    "returned by Morfeusz for word '%1%' "
                                    "(edge %2% -> %3%, expected %4% -> %5%, "
                                    "forma='%6%')")
                                % lex.getUtf8Orth() % interp.p % interp.k
                                % i % (i + 1) % interp.forma));*/
                    std::cerr << 
								boost::format("Ambiguous interpretation "
                                    "returned by Morfeusz for word '%1%' "
                                    "(edge %2% -> %3%, expected %4% -> %5%, "
                                    "forma='%6%')")
                                % lex.getUtf8Orth() % interp.p % interp.k
                                % i % (i + 1) % interp.forma << std::endl
                                << "     ";
                    for (int i = std::max<int>(0, tidx - 3);
                            i <= std::min<int>(text.size(), tidx + 3);
                            i++)
                        std::cerr << text[i].getUtf8Orth() << ' ';
                    std::cerr << std::endl << std::endl;
                    continue;
                }

                if (interp.p > segm) {
                    if (segm != -1) {
                        ret.push_back(current_lex);
                        ret.push_back(Lexeme(Lexeme::NO_SPACE));
                    }
                    current_lex = Lexeme(Lexeme::SEGMENT);
                    current_lex.setUtf8Orth(interp.forma);
                    segm = interp.p;
                }

                //std::cerr << boost::format("morfeusz: %1% %2% %3% %4% %5%\n") %
                //    interp.p % interp.k % interp.forma % interp.haslo % interp.interp;

                if (interp.interp == NULL) {
                    SetCorpusEncoding(GUESSER_UTF8);
                    string forms = GuessForm(interp.forma);
                    //std::cerr << "Odg:" << forms << std::endl;
                    parseOdgadywaczResponse(forms, current_lex);

                    // Add "ign".
                    tag_type tag = tag_type::parseString(tagset, string("ign"));
                    current_lex.addAllowedTag(tag);
                    current_lex.addTagBase(tag, utf8_to_wstring(interp.forma));

                    interps = morfeusz_analyse(
                            const_cast<char*>(lex.getUtf8Orth().c_str()));
                } else {
                    //std::cerr << "Morf:" << interp.forma << ' ' << interp.haslo << ' ' << interp.interp << std::endl;
                    parseMorfeuszTags(interp.interp,
                            utf8_to_wstring(interp.haslo == NULL ?
                                interp.forma : interp.haslo), current_lex);
                }
            }
            if (segm >= 0)
                ret.push_back(current_lex);
		}

        return ret;
	}
};


} // namespace NLPCommon

#endif /* MORFEUSZANALYZER_H_ */
