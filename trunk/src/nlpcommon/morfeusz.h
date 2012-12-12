/*
 * writer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef MORFEUSZANALYZER_H_
#define MORFEUSZANALYZER_H_

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/range/iterator_range.hpp>

#include <iostream>
#include <istream>
#include <vector>
#include <morfeusz.h>
#include <guesser_api.h>
#include <algorithm>

#include <nlpcommon/lexeme.h>
#include <nlpcommon/progress.h>
#include <nlpcommon/exception.h>
#include <nlpcommon/util.h>
#include <nlpcommon/tagset.h>
#include <nlpcommon/polish_tagset_convert.h>

//#define MORFEUSZ_IPI //uncomment if using old Morfeusz

#ifndef MORFEUSZ_TAGSET
#ifdef MORFEUSZ_IPI
#define MORFEUSZ_TAGSET "ipipan"
#else
#define MORFEUSZ_TAGSET "nkjp-morfeusz"
#endif
#endif

#ifndef ODGADYWACZ_TAGSET
#define ODGADYWACZ_TAGSET "ipipan"
#endif

namespace NLPCommon {

using std::vector;
using std::wstring;
using std::string;
using std::ifstream;

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

private:
    typedef boost::split_iterator<string::const_iterator> string_split_iterator;

    const Tagset* out_tagset;
    const Tagset* morf_tagset;
    TagsetConverter<tag_type>* morf_converter;
    const Tagset* odg_tagset;
    TagsetConverter<tag_type>* odg_converter;

    bool quiet;

    // lowercase form -> vector of (base, tag)
    std::map<wstring, std::vector<std::pair<wstring, string> > > morph_dict;

    bool filterMorfeuszTag(wstring orth, const wstring& base,
            const tag_type& tag) {
        // 5. Inne
        //
        // Jeżeli formą ortograficzną jest "ona", "ją", "nią", "jej", "niej",
        // "nią", "on", "nim", "ono", "je", "oni", "ich", "nich", "im", "nim",
        // "one", "je", "nie" (z dowolną kasztowością), to usuń wszystkie
        // interpretacje "...:nakc...".
        boost::to_lower(orth, get_locale("pl_PL"));
        if (orth == L"ona" || orth == L"ją" || orth == L"nią" ||
                orth == L"jej" || orth == L"niej" || orth == L"on" ||
                orth == L"nim" || orth == L"ono" || orth == L"je" ||
                orth == L"oni" || orth == L"ich" || orth == L"nich" ||
                orth == L"im" || orth == L"one" || orth == L"nie") {
            const PartOfSpeech* pos = out_tagset->getPartOfSpeech(
                    tag.getPos());
            const Category* acc_cat = out_tagset->getCategory("accentability");
            int cat_index = out_tagset->getCategoryIndex(acc_cat);
            if (pos->hasCategory(acc_cat)
                    && acc_cat->getValue(tag.getValue(cat_index)) == "nakc")
                return false;
        }
        return true;
    }

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
                int cindex = morf_tagset->getCategoryIndex(cat);
                tag.setValue(cindex, cat->getIndex("[none]"));
                category_offset++;
            }

            tag_type out_tag = morf_converter->convert(lex, tag);
            if (filterMorfeuszTag(lex.getOrth(), base, out_tag)) {
                lex.addTagBase(out_tag, base);
                lex.addAllowedTag(out_tag);
            }
            return;
        }

        const Category* cat = pos->getCategories()[category_offset];
        int cindex = morf_tagset->getCategoryIndex(cat);
        string_split_iterator new_part = part;
        ++new_part;

        if (part->front() == '_') {
            int num_values = cat->getValues().size();
            for (int i = 1; i < num_values; i++) {
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

    void parseMorfeuszTag(const string& mtag, const wstring& base,
            Lexeme& lex) {
        string_split_iterator cat_it =
                boost::make_split_iterator(mtag, boost::token_finder(
                            boost::is_any_of(":")));
        const PartOfSpeech* pos = morf_tagset->getPartOfSpeech(
                boost::copy_range<string>(*cat_it));
        int pos_idx = morf_tagset->getPartOfSpeechIndex(pos);
        tag_type tag;
        tag.clear();
        tag.setPos(pos_idx);
        parseMorfeuszTagSuffix(tag, pos, 0, ++cat_it, base, lex);
    }

    void adjustMorfeuszTag(const wstring& orth, wstring& base, string& tag) {
#ifndef MORFEUSZ_IPI
        static const char* regex_expression = 
            // 1. Klasy gramatyczne
            //
            // a) num:
            //
            // - jeżeli rodzaj to "n1.p1.p2" (lub podzbiór tych rodzajów), to
            //   zamieniamy klasę na numcol (reszta tagu pozostaje taka sama, z
            //   dokładnością do zmiany rodzaju; zob. poniżej)
            //
            // - jeżeli rodzaj inny (w tym zawierający powyższe), to pozostaje
            //   num
            "(^num(:.*:(?:n1\\.p1\\.p2|n1\\.p1|n1\\.p2|p1\\.p2|n1|p1|p2):.*)$)|"
 
            // b) intrj --> interj
            "(^intrj$)|"

            // 2. b) Skróty:
            //
            // brev:npun --> brev:pun.npun
            "(^brev:npun$)"
            ;
        static boost::regex regex(regex_expression);
        static const char* replacement_format =
            "(?1numcol$2)"
            "(?3interj)"
            "(?4brev\\:pun.npun)"
            ;

        //std::cerr << "adjustMorfeuszTag: " << tag;

        tag = boost::regex_replace(tag, regex, replacement_format,
                boost::match_default | boost::format_all);

        // 3. Lematyzacja:
        //
        // Jeżeli klasą gramatyczną jest "siebie", to lematem też musi być
        // "siebie" (a nie "się", jak w Morfeuszu).
        //
        // Jest tylko jedno "się", o interpretacji "qub".
        if (tag.substr(0, 6) == "siebie") {
            base = L"się";
        }

        //std::cerr << " -> " << tag << std::endl;
#endif
    }

    void parseMorfeuszTags(string mtags, wstring base,
            Lexeme& lex) {
        // Jeżeli formą ortograficzną jest "to" (z dowolną kasztowością), to
        // dodajemy interpretację "pred" z formą hasłową "to" (o ile takiej
        // interpretacji nie ma).
        wstring orth = boost::to_lower_copy(lex.getOrth(), get_locale("pl_PL"));
        if (orth == L"to") {
            parseMorfeuszTag("pred", orth, lex);
        }

        boost::iterator_range<std::string::const_iterator> range(mtags.begin(),
                mtags.end());
        for (string_split_iterator tag_it =
                boost::make_split_iterator(range,
                    boost::token_finder(boost::is_any_of("|")));
                tag_it != string_split_iterator(); ++tag_it) {
            string tag(boost::copy_range<string>(*tag_it));
            adjustMorfeuszTag(orth, base, tag);
            if (!tag.empty())
                parseMorfeuszTag(tag, base, lex);
        }
    }

    void parseOdgadywaczResponse(const string& forms, Lexeme& lex) {
        bool got_form = false;
        for (string_split_iterator segment_it =
                boost::make_split_iterator(forms, boost::token_finder(
                        boost::is_any_of("\n")));
                segment_it != string_split_iterator(); ++segment_it) {
            if (segment_it->begin() == segment_it->end())
                break;

            string_split_iterator interp_it = boost::make_split_iterator(
                    *segment_it, boost::token_finder(boost::is_any_of("\t")));
            wstring odg_orth = utf8_to_wstring(
                    boost::copy_range<string>(*interp_it));
            if (got_form) {
                if (!quiet) {
                    std::cerr << "Odgadywacz generated unexpected segment '" <<
                        wstring_to_utf8(odg_orth) << "', ignoring." << std::endl;
                }
                continue;
            } else {
                got_form = true;
            }

            if (odg_orth != lex.getOrth() && !quiet) {
                std::cerr << "Odgadywacz generated form '" <<
                    wstring_to_utf8(odg_orth) << "', but expected '" <<
                    lex.getUtf8Orth() << "'" << std::endl;
            }
            
            for (++interp_it; interp_it != string_split_iterator(); ++interp_it)
            {
                vector<boost::iterator_range<string::const_iterator> > parts;
                boost::split(parts, *interp_it, boost::is_any_of(" "));
                tag_type tag = tag_type::parseString(odg_tagset,
                        boost::copy_range<string>(parts[1]));
                tag_type out_tag = odg_converter->convert(lex, tag);
                lex.addAllowedTag(out_tag);
                lex.addTagBase(out_tag,
                        utf8_to_wstring(boost::copy_range<string>(parts[0])));
            }
        }
    }

    void parseDictEntry(const std::vector<std::pair<wstring, string> >& entry,
            Lexeme& lex) {
        typedef std::pair<wstring, string> ss_type;
        BOOST_FOREACH(const ss_type& ss, entry) {
            tag_type tag = tag_type::parseString(out_tagset, ss.second);
            lex.addAllowedTag(tag);
            lex.addTagBase(tag, ss.first);
        }
    }

public:
    MorfeuszAnalyzer(const Tagset* out_tagset)
        : out_tagset(out_tagset),
          morf_tagset(load_tagset(MORFEUSZ_TAGSET)),
          morf_converter(PolishTagsetConverter<tag_type>::getSharedInstance(
                      morf_tagset, out_tagset)),
          odg_tagset(load_tagset(ODGADYWACZ_TAGSET)),
          odg_converter(PolishTagsetConverter<tag_type>::getSharedInstance(
                      odg_tagset, out_tagset)),
          quiet(false)
    {
    }

    void setQuiet(bool value = true) {
        quiet = value;
    }

    // Format of morphological dictionary file:
    // 
    // |word
    // | [base_form]
    // | tag
    // | tag
    // | [base_form]
    // | tag
    // 
    // Full-line comments starting with # are allowed.
    //
    void loadMorphDict(std::istream& stream) {
        wstring key;
        wstring base;
        std::vector<std::pair<wstring, string> > interps;

        char line_buffer[1024];
//        ifstream stream(filename.c_str());
//        stream.exceptions(ifstream::badbit);
        while (!stream.eof()) {
            stream.getline(line_buffer, sizeof(line_buffer));
            string line(line_buffer);
            bool cont = !line.empty() && boost::is_space()(line[0]);
            boost::trim(line);

            if (line.empty() || line[0] == '#')
                continue;

            if (cont) {
                // Continuing existing entry.
                if (key.empty()) {
                    throw Exception(boost::str(boost::format(
                                    "loadMorphDict() expected definition "
                                    "at the beginning of the file. "
                                    "Definition starts with non-whitespace, "
                                    "but a non-comment line starting with "
                                    "whitespace found. (Line: '%1%')")
                                % line));
                }

                if (line[0] == '[') {
                    // Definition of base form.
                    if (line[line.length() - 1] != ']') {
                        throw Exception(boost::str(boost::format(
                                        "loadMorphDict() expected ']' at "
                                        "end of line starting with '['. "
                                        "(Line: '%1%')")
                                    % line));
                    }
                    base = utf8_to_wstring(line.substr(1, line.length() - 2));
                } else {
                    interps.push_back(std::make_pair(base, line));
                }
            } else {
                // New entry.
                if (!key.empty()) {
                    morph_dict[key] = interps;
                }

                base.clear();
                interps.clear();
                key = utf8_to_wstring(line);
                boost::to_lower(key, get_locale("pl_PL"));
            }
        }

        if (!key.empty()) {
            morph_dict[key] = interps;
        }
    }

    void clearMorphDict() {
        morph_dict.clear();
    }

    vector<Lexeme> analyzeText(const vector<Lexeme>& text, const bool use_odgadywacz) {
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
            //std::cerr << "To morf: " << lex.getUtf8Orth().c_str() << std::endl;

            int start_i = 0;
            while (start_i != -1) {
                int i;
                
                bool has_ambiguous = false;
                int start_of_amb = 100000;
                int end_of_amb = -1;

                int segm = -1;
                int segmk = -1;
                for (i = start_i; ; i++) {
                    InterpMorf& interp = interps[i];
                    //std::cerr << '[' << interp.p << ',' << interp.k << ']';
                    if (interp.p == -1)
                        break;
                    if (i > 0 && interp.p == interps[i-1].p
                            && interp.k == interps[i-1].k)
                        continue;
                    if (interp.p < segm) {
                        has_ambiguous = true;
                        end_of_amb = segmk;
                        start_of_amb = interp.p;
                        break;
                    }
                    segm = interp.p;
                    segmk = interp.k;
                }

                //std::cerr << "\n\nAMBI " << start_of_amb << ' ' << end_of_amb << '\n';

                Lexeme current_lex;
                bool in_ambiguous = false;
                segm = -1;
                i = start_i;
                start_i = -1;
                for (; ; i++) {
                    InterpMorf& interp = interps[i];

                    if (interp.p == -1)
                        break;

                    if (!in_ambiguous && interp.p == start_of_amb) {
                        if (segm != -1)
                            ret.push_back(current_lex);
                        ret.push_back(Lexeme(Lexeme::START_OF_AMBIGUITY));
                        ret.push_back(Lexeme(Lexeme::UNRESOLVED_FRAGMENT));
                        segm = -1;
                        in_ambiguous = true;
                    }

                    /*if (in_ambiguous) {
                        std::cerr << interp.p << ' ' << interp.k << ' ' <<
                            interp.forma << ' ' << interp.haslo << std::endl;
                    }*/

                    if (in_ambiguous && interp.p >= end_of_amb) {
                        ret.push_back(current_lex);
                        ret.push_back(Lexeme(Lexeme::END_OF_AMBIGUITY));
                        ret.push_back(Lexeme(Lexeme::NO_SPACE));
                        start_i = i;
                        in_ambiguous = false;
                        segm = -1;
                        break;
                    }

                    if (interp.p < segm) {
                        if (!(in_ambiguous && interp.p == start_of_amb))
                            throw Exception(boost::str(boost::format(
                                            "Too complex segmentation "
                                            "ambiguity returned by Morfeusz "
                                            "for '%1%'.") % lex.getUtf8Orth()));
                        ret.push_back(current_lex);
                        ret.push_back(Lexeme(Lexeme::UNRESOLVED_FRAGMENT));
                        segm = -1;
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

                    wstring forma = utf8_to_wstring(interp.forma);
                    if (current_lex.getOrth().length() < forma.length())
                        current_lex.setOrth(forma);

                    if (!morph_dict.empty()) {
                        wstring morph_key(utf8_to_wstring(interp.forma));
                        boost::to_lower(morph_key, get_locale("pl_PL"));

                        std::map<wstring, std::vector<std::pair<wstring, string> > >
                           ::const_iterator i = morph_dict.find(morph_key);
                        if (i != morph_dict.end()) {
                            parseDictEntry(i->second, current_lex);
                            continue;
                        }
                    }

                    if (interp.interp == NULL) {
                        string forma_copy(interp.forma);

                        SetCorpusEncoding(GUESSER_UTF8);

                        if (use_odgadywacz) {
                            try {
                                //std::cerr << "To odg: " << interp.forma << ' ' << lex.getUtf8Orth() << std::endl;
                                string forms = GuessForm(forma_copy.c_str());
                                //std::cerr << "Odg:" << forms << " LEX: " << lex.getUtf8Orth() << std::endl;
                                parseOdgadywaczResponse(forms, current_lex);
                            } catch (std::exception const& e) {
                                if (!quiet) {
                                    std::cerr << 
                                            boost::format("Odgadywacz failed for "
                                                    "word '%1%'. Error was: %2%.")
                                            % interp.forma % e.what() << std::endl;
                                }
                            }
                        }

                        // Add "ign".
                        tag_type tag = tag_type::parseString(out_tagset, string("ign"));
                        current_lex.addAllowedTag(tag);
                        current_lex.addTagBase(tag, forma);

                        morfeusz_set_option(MORFOPT_ENCODING, MORFEUSZ_UTF_8);
                        interps = morfeusz_analyse(
                                const_cast<char*>(lex.getUtf8Orth().c_str()));
                    } else {
                        //std::cerr << "Morf:" << interp.forma << ' ' << interp.haslo << ' ' << interp.interp << std::endl;
                        parseMorfeuszTags(interp.interp,
                                utf8_to_wstring(interp.haslo == NULL ?
                                    interp.forma : interp.haslo),
                                current_lex);
                    }
                }
                if (segm >= 0) {
                    ret.push_back(current_lex);
                    //std::cerr << "Morf: " << current_lex.getUtf8Orth() << std::endl;
                }
                if (in_ambiguous) {
                    ret.push_back(Lexeme(Lexeme::END_OF_AMBIGUITY));
                }
            }
        }

        return ret;
    }
};


} // namespace NLPCommon

#endif /* MORFEUSZANALYZER_H_ */
