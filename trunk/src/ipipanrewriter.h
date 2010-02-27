/*
 * ipipanrewriter.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef IPIPANREWRITER_H_
#define IPIPANREWRITER_H_

#include <boost/format.hpp>
#include <iostream>
#include <nlpcommon/exception.h>
#include "rewriter.h"

namespace BTagger {

using namespace NLPCommon;

using std::cerr;
using std::endl;

template<class Lexeme>
class IpiPanRewriter : Rewriter<Lexeme>
{
private:
    int token;
    void (*progress_handler)(int);
    int progress_every;

public:
    void setProgressHandler(void (*progress)(int token), int every) {
        progress_handler = progress;
        progress_every = every;
    }

    void rewriteStream(int phase, const Tagset* tagset, const vector<Lexeme>& lexemes,
            std::istream& in, std::ostream& out) {
        int i = 0;
        int lineno = 0;
        string ctag_to_set;
        while (!in.eof()) {
            char cline[1024];
            in.getline(cline, sizeof(cline));
            string line = cline;
            lineno++;

            if (line.substr(0, 6) == "<orth>") {
                string orth = line.substr(6, line.length() - 13);
                while (i < lexemes.size() && lexemes[i].getUtf8Orth() != orth)
                    i++;
                if (i >= lexemes.size())
                    throw Exception(boost::str(
                                boost::format("Rewriter could not find matching"
                                    " lexeme for token '%1%' in input line %2%")
                                % orth % lineno));
                //cerr << i << ' ' << lexemes[i].getOrth() << endl;
                ctag_to_set = lexemes[i].chosen_tag[phase].asString(tagset);

                // Add a comment with specific tag found in case none of the
                // allowed ones was selected (which should never happen in
                // fact).
                line += string("\n<!-- btagger: ") + ctag_to_set + " -->";

                if (progress_handler && progress_every &&
                        (i % progress_every) == 0)
                    progress_handler(i);

            } else if (line.substr(0, 4) == "<lex") {
                int ctag_start = line.find("<ctag>");
                if (ctag_start != string::npos) {
                    ctag_start += 6;
                    string ctag = line.substr(ctag_start, line.length() -
                            ctag_start - 13);
                    if (ctag != ctag_to_set) {
                        int dinks = line.find(">");
                        assert(dinks != string::npos);
                        line = line.substr(0, dinks) + " disamb_sh=\"0\"" +
                            line.substr(dinks);
                    }
                }
            }
            out << line << std::endl;
        }
    }
};

} // namespace BTagger

#endif

