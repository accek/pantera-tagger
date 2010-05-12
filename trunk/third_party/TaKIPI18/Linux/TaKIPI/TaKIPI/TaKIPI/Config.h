/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_CONFIG_H
#define TAKIPI_CONFIG_H

#include "../takipi_headers.h"

namespace TaKIPI
{
	class TAKIPI_DEV_API CConfig
	{
	public:

		enum ClassifierType
		{
			c45,
			LMT,
			Ripper,
			PART,
			Bag,
			VotingWTA,
			VotingAverage
		};

		// the main path for the tagger data (may be an empty string)
		static std::string commonPath;
		// the directory for trees
		static std::string treeSubDir;
		// the directory for neg trees
		static std::string negTreeSubDir;
		// the directory for SOME (which?) rules (generally rules are stored in commonPath)
		static std::string ruleSubDir;

		static std::string generalRuleFileName;
		static std::string wordDictFileName;
		static std::string uDictFileName;

		static std::string path2UnigramDictionary;

		static ClassifierType classifierType;

		static bool useNegRules;

		static int rulesUseNum;
		
		enum HowMany
		{
			few, // default
			one, // force one
			many // for diagnosis
		};
		static HowMany writeMultiple;

		//teacher:
		static int iterations;
		static double cutLevel;

		enum Task
		{
			tagging,
			training,
			testing,
			koding, // for KODER mode
			convertingToBin, convertingToTxt,
			generatingDicts
		};
		static Task task;


		enum TestMode
		{
			NormalTest,
			OnlyCollocationTest
		};
		static TestMode testMode;

		//added for Dictionary
		static std::string dictionaryBaseFile;

		//added for Teacher
		static std::string teacherSourceFile;

		//added for tests
		static int layers;
		static std::string testFile;
		static bool useCollocations;

		// forces the use of old reader framework (default: false)
		static bool useOldReaders;

		
		////////////////// LEGACY CODE: For parsing console args. Consider moving it elsewhere.
		static bool ParseArgs(int argc, char* argv[]);

		
		static std::string inputFile;
		static std::string inputScriptFile;
		static std::string outPutFile;
		//static Corpus::FileType inputType;
		static std::string testOutFile;
		static std::string formsListInput;
		static std::string formsListOutput;


	private:
		static bool ParseRulesUseNumber(std::string arg);
		static bool ParseEncoding(std::string arg);
		static bool ParseTestFileIn(std::string arg);
		static bool ParseTestFileOut(std::string arg);
		static bool ParseInputFile(std::string arg);
		static bool ParseInputScriptFile(std::string arg);
		static bool ParseOutputFile(std::string arg);
		static bool ParseInputFileType(std::string arg);
		static bool ParseLayers(std::string arg);
		static bool ParseIterations(std::string arg);
		static bool ParseCutLevel(std::string arg);
		static bool ParseClassifierToUse(std::string arg);
		static bool ParseDictionaryCreation(std::string arg);
		static bool ParseTeacherSourceFile(std::string arg);
		static bool ParseFormsList(std::string inArg, std::string outArg);
		static bool ParseConvData(std::string arg);
		static bool ParseDictInput(std::string arg);
		static bool ParseMulti(std::string arg);

		static void ErrorMessage(std::string message);
		static void DumpDiagInfo();
	};
}//namespace TaKIPI
#endif//TAKIPI_CONFIG_H

