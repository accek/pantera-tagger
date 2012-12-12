/*
 * libsegmentsentencer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LIBSEGMENTSENTENCER_H_
#define LIBSEGMENTSENTENCER_H_

#include <istream>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <nlpcommon/sentencer.h>
#include <nlpcommon/lexeme.h>
#include <nlpcommon/progress.h>
#include <nlpcommon/exception.h>

#include <nlpcommon/_pstream.h>
#include <nlpcommon/util.h>

#include <taggingoptions.h>

#ifndef SEGMENT_PATH
#error SEGMENT_PATH should be defined automatically by build system
#endif

#define DEFAULT_SEGMENT_RULES "segment.srx"
#define DELIMITER '\0'

namespace NLPCommon {

using namespace redi;

template<class Lexeme = DefaultLexeme>
class LibSegmentSentencer: public Sentencer<Lexeme> {
public:
    //~ typedef typename vector<Lexeme>::iterator LexemsIterator;
private:
    string segment_exec;
    bool ignoreSentsInsideSegment;
    pstream ps;

    std::string getCurrentChunk(
            const std::string& response,
            const int response_position,
            const std::string& utf8_orth) {
        string res = response.substr(
                                response_position,
                                std::min(response.length() - response_position, utf8_orth.length()));
        int currPos = response_position + res.length();
        while (chunkWithoutSentenceDelimiters(res).length() < utf8_orth.length()
                && currPos < response.length()) {
            res += response[currPos];
            currPos++;
        }
        return res;
    }

    std::string chunkWithoutSentenceDelimiters(const std::string& chunk) {
        std::string res = chunk;
        res.erase(std::remove(res.begin(), res.end(), DELIMITER), res.end());
        return res;
    }
    
    void splitChunk(vector<string>& res, const string& chunk) {
        std::stringstream currStream;
        bool addedCurrStream;
        BOOST_FOREACH( char c, chunk ) {
            addedCurrStream = false;
            if (c == DELIMITER) {
                res.push_back(currStream.str());
                currStream.str("");
                addedCurrStream = true;
            }
            else {
                currStream << c;
            }
        }
        if (!addedCurrStream) {
            res.push_back(currStream.str());
        }
    }

    std::vector<Lexeme> splitSegmentIntoSents(const Lexeme& lex, const std::string& currChunk) {
        std::vector<Lexeme> res;
        std::vector<std::string> delimitedChunks;
        splitChunk(delimitedChunks, currChunk);
        //~ std::cerr << "currChunk.length()=" << currChunk.length() << " delimitedChunks.size()=" << delimitedChunks.size() << std::endl;
        bool isFirst = true;
        BOOST_FOREACH( std::string chunk, delimitedChunks ) {
            if (!isFirst) {
                res.push_back(Lexeme(Lexeme::END_OF_SENTENCE));
                res.push_back(Lexeme(Lexeme::START_OF_SENTENCE));
                res.push_back(Lexeme(Lexeme::NO_SPACE));
            }
            Lexeme currLex(Lexeme::SEGMENT);
            currLex.setUtf8Orth(chunk);
            res.push_back(currLex);
            isFirst = false;
        }
        return res;
    }
    
    void runSegmentSubprocess(const string& rulesPath) {
        if (!ps.is_open()) {
            vector < string > args;
            //~ std::cerr << "SEGMENT EXEC=" << segment_exec << std::endl;
            args.push_back(segment_exec);
            args.push_back("--null-delimiter");
            args.push_back("-l");
            args.push_back("pl");
            args.push_back("-s");
            args.push_back(rulesPath);

            ps.open(segment_exec, args,
                    pstreams::pstdin | pstreams::pstdout | pstreams::pstderr);
            if (!ps.is_open()) {
                throw Exception("Cannot execute 'segment_batch' excutable with path '" SEGMENT_PATH "')");
            }
            ps.exceptions ( pstream::failbit | pstream::badbit );
        }
    }
    
    int rewriteNonParagraphLexems(const vector<Lexeme>& text, int textIdx, vector<Lexeme>& out) {
        Lexeme lex = text[textIdx];
        while (textIdx < text.size() && lex.getType() != Lexeme::START_OF_PARAGRAPH) {
            out.push_back(lex);
            this->advanceProgress();
            
            textIdx++;
            lex = text[textIdx];
        }
        return textIdx;
    }
    
    int getOneParagraph(const vector<Lexeme>& text, const int currTextIdx, vector<Lexeme>& paragraph) {
        int textIdx = currTextIdx;
        assert(textIdx < text.size());
        assert(text[textIdx].getType() == Lexeme::START_OF_PARAGRAPH);
        bool stop = false;
        while (textIdx < text.size() && !stop) {
            Lexeme lex = text[textIdx];
            paragraph.push_back(lex);
            if (lex.getType() == Lexeme::END_OF_PARAGRAPH) {
                stop = true;
            }
            textIdx++;
        }
        return textIdx;
    }
    
    void feedSentencerWithParagraph(const vector<Lexeme>& paragraph) {
        bool no_space = true;
        BOOST_FOREACH( Lexeme lex, paragraph ) {
            if (lex.getType() == Lexeme::NO_SPACE) {
                no_space = true;
            } 
            else if (lex.getType() == Lexeme::SEGMENT) {
                if (!no_space)
                    ps << ' ';
                ps << lex.getUtf8Orth();
                no_space = false;
            }
        }
        ps << '\n';
        ps.flush();
    }
    
    void readSentencerResponse(string& response) {
		getline(ps.out(), response);
		//~ std::cerr << "SEGMENT RESPONSE: " << response << std::endl;
        //~ if (!getline(ps.out(), response)) {
			//~ std::cerr << response << std::endl;
            //~ throw Exception("No response returned by 'segment_batch' executable");
		//~ }
    }
    
    int handleSpacesAndNewSentMarks(const bool no_space, const bool first_sentence, const string& response, int response_position, vector<Lexeme>& out) {
        
        bool was_space_before_newsent = false;
        
        // handle space before new sentence
        if (response[response_position] == ' '
                && response[response_position + 1] == DELIMITER) {
            was_space_before_newsent = true;
            response_position++;
        }
        
        // handle new sentence
        if (response[response_position] == DELIMITER) {
            if (!first_sentence) {
                out.push_back(Lexeme(Lexeme::END_OF_SENTENCE));
            }
            out.push_back(Lexeme(Lexeme::START_OF_SENTENCE));

            response_position++;
        }
        
        // handle regular space
        if (!no_space && !was_space_before_newsent) {
            if (response[response_position] != ' ') {
                throw Exception(
                    boost::str(boost::format(
                        "Expected space not found in "
                        "output of segment_batch. "
                        "(Response there: '%1%')")
                    % response.substr(response_position)));
            }
            response_position++;
        }
        
        return response_position;
    }
    
    int handleCurrentToken(const Lexeme& lex, const string& response, int response_position, vector<Lexeme>& out) {
        string utf8_orth = lex.getUtf8Orth();
        string currChunk = getCurrentChunk(response, response_position, utf8_orth);
        if (currChunk == utf8_orth) {
            response_position += utf8_orth.length();
            out.push_back(lex);
        }
        else if (chunkWithoutSentenceDelimiters(currChunk) == utf8_orth) {
            if (ignoreSentsInsideSegment) {
                std::cerr << "WARN: ignoring sentence delimiters in segment '" << currChunk << "'." << std::endl;
                out.push_back(lex);
                response_position += currChunk.length();
            }
            else {
                std::cerr << "WARN: adding sentence delimiters into segment '" << currChunk << "'." << std::endl;
                BOOST_FOREACH( Lexeme lex, splitSegmentIntoSents(lex, currChunk)) {
                    //~ if (lex.getType() == Lexeme::SEGMENT)
                        //~ std::cerr << "WARN: adding " << lex.getUtf8Orth() << std::endl;
                    out.push_back(lex);
                }
                response_position += currChunk.length();
            }
        }
        else {
            throw Exception(boost::str(boost::format(
                        "Expected word '%1%' not found in"
                        "output of segment_batch. "
                        "(Response there: '%2%')")
                        % utf8_orth
                        % response.substr(response_position)));
        }
        
        return response_position;
    }
    
    void correctMissingSentenceStartAtFront(vector<Lexeme>& par) {
		typename vector<Lexeme>::iterator it = par.begin();
		for(;it != par.end(); it++) {
			Lexeme lex = *it;
			if (lex.getType() == Lexeme::START_OF_SENTENCE)
				return;
			else if (lex.getType() == Lexeme::SEGMENT) {
				par.insert(it, Lexeme(Lexeme::START_OF_SENTENCE));
				return;
			}
		}
	}
	
	void correctMissingSentenceEndAtBack(vector<Lexeme>& par) {
		typename vector<Lexeme>::reverse_iterator it = par.rbegin();
		for(;it != par.rend(); it++) {
			Lexeme lex = *it;
			if (lex.getType() == Lexeme::END_OF_SENTENCE)
				return;
			else if (lex.getType() == Lexeme::SEGMENT) {
				par.insert(it.base(), Lexeme(Lexeme::END_OF_SENTENCE));
				return;
			}
		}
	}
    
    void correctMissingSentenceDelimiters(vector<Lexeme>& par) {
		correctMissingSentenceStartAtFront(par);
		correctMissingSentenceEndAtBack(par);
	}
    
    void parseSentencerResponse(const vector<Lexeme>& paragraph, const string& response, vector<Lexeme>& out) {
        bool no_space = true;
        bool first_sentence = true;
        int response_position = 0;
        BOOST_FOREACH( Lexeme lex, paragraph ) {
			if (lex.getType() == Lexeme::NO_SPACE) {
                no_space = true;
                out.push_back(lex);
            }
            else if (lex.getType() == Lexeme::SEGMENT) {
                //~ std::cerr << "segment " << lex.getUtf8Orth() << std::endl;
                response_position = handleSpacesAndNewSentMarks(no_space, first_sentence, response, response_position, out);
                //~ std::cerr << "handled spaces and new sent marks " << response_position << std::endl;
                response_position = handleCurrentToken(lex, response, response_position, out);
                //~ std::cerr << "handled token itself " << response_position << std::endl;
                no_space = false;
                first_sentence = false;
            }
            else {
                out.push_back(lex);
			}
        }
        correctMissingSentenceDelimiters(out);
    }
    
    int doOneParagraph(const vector<Lexeme>& text, int textIdx, vector<Lexeme>& out) {
        vector<Lexeme> paragraph;
        string response;
        textIdx = getOneParagraph(text, textIdx, paragraph);
        //~ std::cerr << "next paragraph" << std::endl;
        //~ debugText(paragraph);
        feedSentencerWithParagraph(paragraph);
        readSentencerResponse(response);
        //~ std::cerr << "response" << std::endl;
        //~ std::cerr << response << std::endl;
        parseSentencerResponse(paragraph, response, out);
        return textIdx;
    }
    
    void debugText(const vector<Lexeme>& text) {
		std::cerr << "DEBUG" << std::endl;
		BOOST_FOREACH( Lexeme lex, text ) {
			switch (lex.getType()) {
				case Lexeme::SEGMENT:
					std::cerr << "SEGMENT " << lex.getUtf8Orth();
					break;
				case Lexeme::NO_SPACE:
					std::cerr << "NS";
					break;
				case Lexeme::START_OF_PARAGRAPH:
					std::cerr << "START_OF_PARAGRAPH";
					break;
				case Lexeme::END_OF_PARAGRAPH:
					std::cerr << "END_OF_PARAGRAPH";
					break;
				case Lexeme::START_OF_SENTENCE:
					std::cerr << "START_OF_SENTENCE";
					break;
				case Lexeme::END_OF_SENTENCE:
					std::cerr << "END_OF_SENTENCE";
					break;
				default:
					std::cerr << lex.getType();
			}
			std::cerr << std::endl;
		}
		std::cerr << "END DEBUG" << std::endl;
	}

public:

    LibSegmentSentencer(const bool ignoreSentsInsideSegment = false) :
            Sentencer<Lexeme>(), 
            segment_exec(find_with_path(SEGMENT_PATH, "segment_batch").string()), 
            ignoreSentsInsideSegment(ignoreSentsInsideSegment) {}

    virtual ~LibSegmentSentencer() {
    }
    
    vector<Lexeme> addSentenceDelimiters(const vector<Lexeme>& text, const string& sentencerRules) {
        vector<Lexeme> res;
        
        string rulesPath = sentencerRules.empty() 
                ? find_with_path(SEGMENT_PATH, DEFAULT_SEGMENT_RULES).string()
                : sentencerRules;
        //~ std::cerr << "USING RULES AT PATH: " << rulesPath << std::endl;
        runSegmentSubprocess(rulesPath);
        
        int textIdx = 0;
        
        while (textIdx < text.size()) {
            textIdx = rewriteNonParagraphLexems(text, textIdx, res);
            if (textIdx < text.size())
                textIdx = doOneParagraph(text, textIdx, res);
        }
        
        //~ debugText(res);
        
        return res;
    }
};

} // namespace NLPCommon

#endif /* LIBSEGMENTSENTENCER_H_ */
