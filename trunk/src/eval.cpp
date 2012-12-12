/*
 ============================================================================
 Name        : btagger.cpp
 Author      : Szymon Acedanski
 Version     :
 Copyright   : Copyright (C) Institute of CS, Polish Academy of Sciences
 Description : Hello World in C++,
 ============================================================================
 */

#include <boost/foreach.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <clocale>

#include <nlpcommon/lexeme.h>
#include <nlpcommon/tag.h>
#include <nlpcommon/tagset.h>
#include <nlpcommon/spejdtagsetloader.h>
#include <nlpcommon/ipipanlexer.h>
#include <nlpcommon/scorer.h>
#include <nlpcommon/poliqarp-weights.h>
#include <nlpcommon/stats.h>
#include <nlpcommon/finderrors.h>

using namespace std;
using namespace NLPCommon;

// XXX bo jest jako extern w nlpcommon/util.h
boost::program_options::variables_map options;

// XXX bo nie chcemy, by sie wywalało przez MPI
boost::mpi::environment env;
boost::mpi::communicator world;

typedef Lexeme<Tag> MyLexeme;

TaggingErrorsCollector<MyLexeme>* errors_collector;

void lexing_progress(int token) {
    wcerr << "\rLexing...  " << token;
}

bool are_same_tags(vector<Tag> t1, vector<Tag> t2) {
	std::sort(t1.begin(), t1.end());
	std::sort(t2.begin(), t2.end());
	return t1 == t2;
}

template <class Statistician>
void calculateStats(const char* label, const Tagset* tagset, const
		vector<MyLexeme>& text, bool final) {
	static Statistician s(tagset);
	static int run = 0;

	run++;
	s.addStatistics(text);

	if (final) {
		wcout << boost::wformat(
				L"\\newcommand{\\defstat%s}{%%\n"
				L"\\newcommand{\\statruns}{%d}\n"
			    L"\\newcommand{\\stattype}{%s}\n"
			    L"\\newcommand{\\statP}{%.2lf%%}\n"
			    L"\\newcommand{\\statR}{%.2lf%%}\n"
			    L"\\newcommand{\\statF}{%.2lf%%}\n"
			    L"\\newcommand{\\statWC}{%.2lf%%}\n"
			    L"\\newcommand{\\statSC}{%.2lf%%}}\n")
				% label
				% run % label
				% (s.getPrecision() * 100.0)
				% (s.getRecall() * 100.0)
				% (s.getFMeasure() * 100.0)
			   	% (s.getWeakCorrectness() * 100.0)
				% (s.getStrongCorrectness() * 100.0);
	} 
	wcout << boost::wformat(L"%% RUN %d  STATS %s  P %lf  R %lf  F %lf  WC %lf  SC %lf\n")
			% run % label % s.getPrecision() % s.getRecall()
			% s.getFMeasure() % s.getWeakCorrectness()
			% s.getStrongCorrectness();
}

int process_part(const Tagset* tagset,
		const string& golden_filename,
		const string& tested_filename,
		bool final) {
    vector<MyLexeme> golden, tested;

	wcerr << endl << "Lexing golden ..." << endl;
	ifstream golden_stream(golden_filename.c_str(), ios::in | ios::binary);
	IpiPanLexer<MyLexeme> golden_lexer(golden_stream);
	golden_lexer.setProgressHandler(lexing_progress, 1000);
	golden_lexer.parseStreamToVector(golden, &tagset);
	wcerr << "\rLexing tested ..." << endl;
	ifstream tested_stream(tested_filename.c_str(), ios::in | ios::binary);
	IpiPanLexer<MyLexeme> tested_lexer(tested_stream, true);
	tested_lexer.setProgressHandler(lexing_progress, 1000);
	tested_lexer.parseStreamToVector(tested, &tagset);
	wcerr << "\rLexing ...  done.       " << endl;

	int n = std::min<int>(golden.size(), tested.size());
	for (int i = 0; i < n; i++) {
		MyLexeme& golden_lex = golden[i];
		MyLexeme& tested_lex = tested[i];

		if (golden_lex.getOrth() != tested_lex.getOrth()) {
			wcerr << "*** Incorrect orth at position " << i << " --- "
				"in golden is '" << golden_lex.getOrth() << "', in tested is '"
				<< tested_lex.getOrth() << "'" << endl;
			return 1;
		}

		if (golden_lex.getGoldenTags().empty()) {
			wcerr << "*** No golden tags in golden "
				"at position " << i << "(word is '" <<
				golden_lex.getOrth() << "')" << endl;
			return 1;
		}
	}

	bool has_any_autoselected_tags = false;
	BOOST_FOREACH(const MyLexeme& lex, tested) {
		if (!lex.getAutoselectedTags().empty() &&
				lex.getAutoselectedTags() != lex.getAllowedTags()) {
			has_any_autoselected_tags = true;
			break;
		}
	}

	for (int i = 0; i < n; i++) {
		MyLexeme& golden_lex = golden[i];
		MyLexeme& tested_lex = tested[i];

		if (!has_any_autoselected_tags) {
			tested_lex.setAutoselectedTags(
					tested_lex.getGoldenTags());
			tested_lex.setGoldenTags(vector<Tag>());
		}

		if (tested_lex.getGoldenTags().empty()) {
			tested_lex.setGoldenTags(golden_lex.getGoldenTags());
		}

		if (!are_same_tags(golden_lex.getGoldenTags(),
					tested_lex.getGoldenTags())) {
			wcerr << "*** Golden tags incorrectly rewritten "
				"at position " << i << "(word is '" <<
				golden_lex.getOrth() << "')" << endl;
			return 1;
		}
	}

	/*
	for (int i = 0; i < 20; i++) {
		const MyLexeme& lex = tested[i];
		wcerr << lex.getOrth() << ' ' << lex.getAutoselectedTags()[0].asWString(tagset) << ' ' << lex.getGoldenTags()[0].asWString(tagset) << endl;
	}
	*/


	typedef BestScoreMultiGoldenScorer<
		BinaryScorer<MyLexeme::tag_type> > MyBinaryScorer;
	calculateStats<TaggingStatistician<MyLexeme, MyBinaryScorer > >
		("BINARY", tagset, tested, final);

	typedef BestScoreMultiGoldenScorer<
		BinaryPoSScorer<MyLexeme::tag_type> > MyPosScorer;
	calculateStats<TaggingStatistician<MyLexeme, MyPosScorer > >
		("POSBINARY", tagset, tested, final);

	typedef BestScoreMultiGoldenScorer<
		APScorer<PoliqarpWeights, true, MyLexeme::tag_type> > MyPoliqarpScorer;
	calculateStats<TaggingStatistician<MyLexeme, MyPoliqarpScorer > >
		("POLIQARP", tagset, tested, final);

	typedef BestScoreMultiGoldenScorer<
		PAScorer<MyLexeme::tag_type> > MyPAScorer;
	calculateStats<TaggingStatistician<MyLexeme, MyPAScorer > >
		("PA", tagset, tested, final);
	
	typedef BestScoreMultiGoldenScorer<
		WPAScorer<PoliqarpWeights, MyLexeme::tag_type> > MyPoliqarpWPAScorer;
	calculateStats<TaggingStatistician<MyLexeme, MyPoliqarpWPAScorer > >
		("POLIQARPWPA", tagset, tested, final);

	vector<MyLexeme>* copy = new vector<MyLexeme>(tested);
	errors_collector->addTaggingErrors(*copy);

	return 0;
}

int real_main(int argc, char** argv) {
    const Tagset* tagset;

    SpejdTagsetLoader tagset_loader;
    ifstream config_stream("tagset.cfg");
    tagset = tagset_loader.loadTagset(config_stream);

	errors_collector = new TaggingErrorsCollector<MyLexeme>(tagset);

	string golden_arg(argv[1]), tested_arg(argv[2]);
	if (golden_arg[golden_arg.length()-1] == '/' &&
	    tested_arg[tested_arg.length()-1] == '/') {
		// If directories are specified, we do 10-fold cross-validation
		// on files named partN.xml and partN.xml.disamb.
		
		for (int i = 0; i < 10; i++) {
			wcerr << "PROCESSING FOLD " << i << endl;

			int retcode = process_part(tagset,
					boost::str(
						boost::format("%spart%d.xml") % golden_arg % i),
					boost::str(
						boost::format("%spart%d.xml.disamb") % tested_arg % i),
					i == 9);

			if (retcode)
				return retcode;
		}
	} else {
		int retcode = process_part(tagset, golden_arg, tested_arg, true);
		if (retcode)
			return retcode;
	}

	if (argv[3] != NULL) {
		wcerr << "GENERATING ERRORS REPORT..." << endl;
        wofstream errors_report(argv[3]);
        std::locale errors_locale;
        try {
            errors_locale = std::locale("");
            if (errors_locale.name() == "*")
                errors_locale = std::locale("pl_PL.UTF-8");
        } catch (...) { }
        errors_report.imbue(errors_locale);
		errors_collector->printErrors(errors_report);
        errors_report.close();
	}

	return 0;
}

int main(int argc, char** argv) {
	// This is needed for correct wcin/wcout behaviour.
	setlocale(LC_CTYPE, "");

	try {
		return real_main(argc, argv);
	} catch (std::exception const& e) {
        wcerr << "***FATAL***: Caught " << typeid(e).name() << " with message:" << endl;
        wcerr << e.what() << endl;
		throw;
	} catch (...) {
        wcerr << "***FATAL***: Caught unknown exception." << endl;
		throw;
	}
}
