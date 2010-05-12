#include "../takipi_headers.h"
#include "Config.h"
#include "IO.h"

#ifndef TAKIPI_DATA_PATH
#define TAKIPI_DATA_PATH ""
#endif

namespace TaKIPI
{
	//default values:
	CConfig::ClassifierType CConfig::classifierType = CConfig::c45;

	std::string CConfig::commonPath = TAKIPI_DATA_PATH;
	std::string CConfig::treeSubDir = "Trees";
	std::string CConfig::negTreeSubDir = "NegTrees";
	std::string CConfig::ruleSubDir = "Rules";

	std::string CConfig::generalRuleFileName = "general.rul";
	std::string CConfig::wordDictFileName = "WordDict.bin";
	std::string CConfig::uDictFileName = "UDict.bin";

	bool CConfig::useNegRules = false;
	std::string CConfig::path2UnigramDictionary = "Dictionary.txt";
	int CConfig::rulesUseNum = 1;

	int CConfig::iterations = 1;
	double CConfig::cutLevel = 0.01;

	CConfig::Task CConfig::task = CConfig::tagging;

	CConfig::TestMode CConfig::testMode = CConfig::NormalTest;
	int CConfig::layers = 2;
	std::string CConfig::testFile = TAKIPI_DATA_PATH;

	std::string CConfig::teacherSourceFile = "";

	std::string CConfig::dictionaryBaseFile = "";
	
	CConfig::HowMany CConfig::writeMultiple = CConfig::few;

	bool CConfig::useCollocations = false;

	bool CConfig::useOldReaders = false;

	///////////// LEGACY CODE -- old takipi
	/////////////////////////////////////////

	std::string CConfig::inputFile = "";
	std::string CConfig::inputScriptFile = "";
	std::string CConfig::outPutFile = "";
	std::string CConfig::testOutFile = "";
	std::string CConfig::formsListInput = "";
	std::string CConfig::formsListOutput = "";

	void CConfig::ErrorMessage(std::string message)
	{
		std::cerr << message << std::endl << std::endl;
		std::cerr << "Poprawne wywolanie powinno wygladac nastepujaco: " << std::endl;
		std::cerr << "------------------------------------------------------------" << std::endl;
		std::cerr << "takipi [-i plikWej] [-is plikSkrypt] [-o plikWyj] [kodowanie] [-c klasyfikator]" << std::endl;
		std::cerr << "------------------------------------------------------------" << std::endl;
		std::cerr << "Gdzie: " << std::endl;
		std::cerr << "\t-i        -nazwa pliku wejściowego" << std::endl;
		std::cerr << "\t-is       -nazwa pliku zawierającego listę plików wejściowych" << std::endl;
		std::cerr << "\t-o        -nazwa pliku wyjściowego" << std::endl;
		std::cerr << "\t           (niedozwolone użycie z parametrem -is)" << std::endl;
		std::cerr << "\t           (brak powyższego paramteru, spowoduje dla opcji -i" << std::endl;
		std::cerr << "\t           utworzenie pliku temp.xml w bieżącym katalogu)" << std::endl;
		std::cerr << "\tkodowanie -UTF    kodowanie wejścia UTF8 (domyślne)" << std::endl;
		std::cerr << "\t          -ISO    kodowanie wejścia ISO8859_2" << std::endl;
		std::cerr << "\t          -CP1250 kodowanie wejścia CP1250" << std::endl;
		std::cerr << "\t-it       -rodzaj pliku wejściowego" << std::endl;
		std::cerr << "\t          \tXML    - plik xml, np. pre_morph (domyślnie)," << std::endl;
		std::cerr << "\t          \tTXT    - tekstowy." << std::endl;
		std::cerr << "\t          \tCORPUS - plik korpusu poddany analizie morfologicznej." << std::endl << std::endl;
		std::cerr << "\t-noguess -wylacza odgadywacz morfosyntaktyczny" << std::endl;
		std::cerr << "\t          \t(wiecej tagow nierozpoznanych, lecz bardziej przewidywalne wyjscie)" << std::endl << std::endl;
		std::cerr << "\t-old      stary tokenizator, bez nowych klas fleksyjnych" << std::endl;
		std::cerr << "\t-force    -kontrola liczby tagów na wyjściu" << std::endl;
		std::cerr << "\t          \tfew - rozsądne minimum (domyślnie)" << std::endl;
		std::cerr << "\t          \tone - zawsze dokładnie jeden tag" << std::endl;
		std::cerr << "\t          \tmany - wszystkie pozostawione na ost. warstwie (do testów)" << std::endl;
		std::cerr << std::endl << "Zaawansowane (dotyczy głównie testowania i uczenia):" << std::endl;
		std::cerr << "\t-ti      -nazwa pliku dla trybu testowania" << std::endl;
		std::cerr << "\t-to      -nazwa pliku dla wyników testu" << std::endl;
		std::cerr << "\t-rules %liczba       -ile razy użyć reguł " << std::endl;
		std::cerr << "\t-l%liczba " << std::endl;
		std::cerr << "\t-L%liczba-określa warstwę działania " << std::endl;
		std::cerr << "\t-c       -klasyfikator do użycia" << std::endl;
		std::cerr << "\t          \tc - c4.5 (domyślnie)," << std::endl;
		std::cerr << "\t          \tl - LMT" << std::endl;
		std::cerr << "\t          \tr - Ripper" << std::endl;
		std::cerr << "\t          \tp - PART" << std::endl;
		std::cerr << "\t          \tb - Bag (wszystkie powyżej na raz)" << std::endl;
		std::cerr << "\t          \tw - głosowany WTA (najlepsza propozycja jednego z clrp)" << std::endl;
		std::cerr << "\t-koder    kodowanie drzew -- konwersja z txt na bin" << std::endl;
		std::cerr << "\t-gendict  -nazwa pliku uczącego, na postawie którego mają zostać wygenerowane słowniki" << std::endl;
		std::cerr << "\t-convdata -konwertuje słowniki z .txt na .bin (-convdata bin) lub na odwrót (-convdata txt)" << std::endl;
		std::cerr << "\t-datainfo -wypisuje uzywane sciezki danych (do celow diagnostycznych)" << std::endl << std::endl;
		std::cerr << "------------------------------------------------------------" << std::endl;
		exit(1);
	}

	bool CConfig::ParseArgs(int argc, char* argv[])
	{
		// standardowo odgadywacz jest uzywany -- o ile uzytkownik nie uruchomi z -noguess
		Corpus::CConfig::guesserUsage = Corpus::GuessingSystem::FormsIdentifier::guessAll;
		
		for(int i = 1; i < argc; i++)
		{
			if(!strcmp(argv[i],"-d"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -d brak nazwy pliku do tworzenia słownika unigramowego.");
				ParseDictionaryCreation(std::string(argv[i]));
			}
			else if(!strcmp(argv[i],"-list"))
			{
				i += 2;
				if(i == argc)
					ErrorMessage("Po argumencie -list powinna być następująca składnia: <plikListaDokumentow> <plikWyjściowy>.");
				ParseFormsList(std::string(argv[i-1]), std::string(argv[i]));
				return true;
			}
			else if(!strcmp(argv[i],"-noguess"))
			{
				Corpus::CConfig::guesserUsage = Corpus::GuessingSystem::FormsIdentifier::guessNone;
			}
			else if(!strcmp(argv[i],"-t"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -i brak nazwy pliku wejściowego.");
				ParseTeacherSourceFile(std::string(argv[i]));
			}
			else if(!strcmp(argv[i],"-i"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -i brak nazwy pliku wejściowego.");
				ParseInputFile(std::string(argv[i]));
			}
			else if(!strcmp(argv[i],"-o"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -o brak nazwy pliku wyjściowego.");
				ParseOutputFile(std::string(argv[i]));
			}
			else if(!strcmp(argv[i],"-it"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -it brak nazwy pliku wyjściowego.");
				if(!ParseInputFileType(std::string(argv[i])))
					ErrorMessage("Zły typ wejścia.");
			}
			else if(!strcmp(argv[i],"-force"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -force brak okreslenia licznosci.");
				if(!ParseMulti(std::string(argv[i])))
					ErrorMessage("Zly parametr dla -force.");
			}
			else if(!strcmp(argv[i],"-gendict"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -gendict brak nazwy pliku uczacego.");
				if(!ParseDictInput(std::string(argv[i])))
					ErrorMessage("Zły typ wejścia.");
			}
			else if(!strcmp(argv[i],"-iterations"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -iterations brak parametru liczbowego.");
				if(!ParseIterations(std::string(argv[i])))
					ErrorMessage("Zla wartosc liczby iteracji.");
			}
			else if(!strcmp(argv[i],"-rules"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -rules brak parametru liczbowego.");
				if(!ParseRulesUseNumber(std::string(argv[i])))
					ErrorMessage("Zla wartosc liczby iteracji.");
			}
			else if(!strcmp(argv[i],"-cutlevel"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -cutlevel brak parametru liczbowego.");
				if(!ParseCutLevel(std::string(argv[i])))
					ErrorMessage("Zla wartosc poziomu odciecia.");
			}
			else if(!strcmp(argv[i],"-UTF8")
				|| !strcmp(argv[i],"-ISO")
				|| !strcmp(argv[i],"-CP1250"))
				ParseEncoding(std::string(argv[i]));
			else if(!strcmp(argv[i],"-ti"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -ti brak nazwy pliku wejsciowego dla testu.");
				if(!ParseTestFileIn(std::string(argv[i])))
					ErrorMessage("Z�y plik testowy.");
			}
			else if(!strcmp(argv[i],"-neg"))
			{
				useNegRules = true;
			}
			else if(!strcmp(argv[i],"-to"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -to brak nazwy pliku wyjściowego dla testu.");
				if(!ParseTestFileOut(std::string(argv[i])))
					ErrorMessage("Z�y plik wyjsciowy dla testu.");
			}
			else if(!strcmp(argv[i],"-L0")
				|| !strcmp(argv[i],"-L1")
				|| !strcmp(argv[i],"-L2")
				|| !strcmp(argv[i],"-l0")
				|| !strcmp(argv[i],"-l1")
				|| !strcmp(argv[i],"-l2"))
			{
				ParseLayers(argv[i]);
			}		
			else if(!strcmp(argv[i],"-is"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -is brak nazwy pliku skryptu.");
				ParseInputScriptFile(argv[i]);
			}
			else if(!strcmp(argv[i],"-convdata"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -convdata brak okreslenia formatu (txt -- z bin do txt, bin -- odwrotnie).");
				ParseConvData(argv[i]);
			}
			else if(!strcmp(argv[i],"-c"))
			{
				i++;
				if(i == argc)
					ErrorMessage("Po argumencie -c brak kodu klasyfikatora do u�ycia.");
				if(!ParseClassifierToUse(argv[i]))
					ErrorMessage("Po argumencie -c wystepuje nierozpoznany kod klasyfikatora.");
			}
			else if(!strcmp(argv[i],"-koder"))
			{
				task = koding;
			}
			else if(!strcmp(argv[i],"-old"))
			{
				useOldReaders = true;
			}
			else if(!strcmp(argv[i],"-datainfo"))
			{
				DumpDiagInfo();
			}
		}

		if(task != tagging && task != training) // nie potrzebujemy zadnych dodatkowych parametrow
			return true;

		/* slownik i tak jest deserializowany pozniej
		std::cout << "Deserializacja slownika slow.";
		std::ifstream dictFile(IO::GetFullPath(commonPath, wordDictFileName).c_str(), std::ios::binary);
		Corpus::CWordDictionary::Deserialize(dictFile);
		dictFile.close();
		*/
		//CReader::formsIdentifier = new FormsIdentifier(CConfig::inputEncoding,Parser::utf8, CWordDictionary::dictionary, guesserUsage);
		
		if(teacherSourceFile.empty()
			&& inputFile.empty()
			&& inputScriptFile.empty())
			ErrorMessage("Nie podano argumentow.");

		if(dictionaryBaseFile.length() != 0)
			return true;
		if(teacherSourceFile.length() != 0 && layers != -1)
			return true;

		if(iterations < 1 || iterations > 10)
			ErrorMessage("Liczba iteracji musi si� zawierac w zbiorze [1..10].");
		if(cutLevel > 1 || cutLevel < 0)
			ErrorMessage("Poziom odciecia musi sie zaiwerac w zbiorze (0..1)");
		if(layers == -1)
			ErrorMessage("Nie ustalono warstwy.");
		if(!testOutFile.empty() && testFile.empty())
			ErrorMessage("Brak podanego wejsciowego pliku testujacego.");
		if(inputScriptFile.empty() && inputFile.empty() && testFile.empty())
			ErrorMessage("Brak podanego wej�cia do taggera.");
		if(!inputScriptFile.empty() && !outPutFile.empty())
			ErrorMessage("Nie mo�na stosowa� razem parametr�w -o oraz -is.");

		return true;
	}

	bool CConfig::ParseDictionaryCreation(std::string arg)
	{
		CConfig::dictionaryBaseFile = arg;

		return true;
	}

	bool CConfig::ParseTeacherSourceFile(std::string arg)
	{
		CConfig::teacherSourceFile = arg;
		task = training;

		return true;
	}

	bool CConfig::ParseFormsList(std::string inArg, std::string outArg)
	{
		CConfig::formsListInput = inArg;
		CConfig::formsListOutput = outArg;

		return true;
	}

	bool CConfig::ParseRulesUseNumber(std::string arg)
	{
		try
		{
			CConfig::rulesUseNum = atoi(arg.c_str());
		}
		catch(Corpus::tagexception e)
		{
			return false;
		}

		return true;
	}

	bool CConfig::ParseLayers(std::string arg)
	{
		if(!arg.compare("-L0"))
			CConfig::layers = 0;	
		else if(!arg.compare("-L1"))
			CConfig::layers = 1;	
		else if(!arg.compare("-L2"))
			CConfig::layers = 2;	

		return true;
	}

	bool CConfig::ParseTestFileIn(std::string arg)
	{
		testFile = arg;
		task = testing;

		return true;
	}

	bool CConfig::ParseIterations(std::string arg)
	{
		try
		{
			CConfig::iterations = atoi(arg.c_str());
		}
		catch(Corpus::tagexception e)
		{
			return false;
		}

		return true;
	}

	bool CConfig::ParseCutLevel(std::string arg)
	{
		try
		{
			CConfig::cutLevel = atof(arg.c_str());
		}
		catch(Corpus::tagexception e)
		{
			return false;
		}

		return true;
	}

	bool CConfig::ParseTestFileOut(std::string arg)
	{
		CConfig::testOutFile = arg;

		return true;
	}

	bool CConfig::ParseEncoding(std::string arg)
	{
		if(!arg.compare("-ISO"))
		{
			Corpus::CConfig::inputEncoding = Corpus::Parser::iso88592;
		}
		if(!arg.compare("-UTF"))
		{
			Corpus::CConfig::inputEncoding = Corpus::Parser::utf8;
		}
		else if(!arg.compare("-CP1250"))
		{
			Corpus::CConfig::inputEncoding = Corpus::Parser::cp1250;
		}

		return true;
	}
	
	bool CConfig::ParseMulti(std::string arg)
	{
		if(!arg.compare("many"))
		{
			CConfig::writeMultiple = CConfig::many;
			return true;
		}
		if(!arg.compare("one"))
		{
			CConfig::writeMultiple = CConfig::one;
			return true;
		}
		if(!arg.compare("few"))
		{
			CConfig::writeMultiple = CConfig::few;
			return true;
		}
		
		
		return false;
	}

	bool CConfig::ParseInputFile(std::string arg)
	{
		CConfig::inputFile = arg;

		return true;
	}

	bool CConfig::ParseInputScriptFile(std::string arg)
	{
		CConfig::inputScriptFile = arg;

		return true;
	}
	
	bool CConfig::ParseDictInput(std::string arg)
	{
		CConfig::teacherSourceFile = arg;
		task = generatingDicts;
		return true;
	}

	bool CConfig::ParseOutputFile(std::string arg)
	{
		CConfig::outPutFile = arg;

		return true;
	}

	bool CConfig::ParseInputFileType(std::string arg)
	{
		if(!arg.compare("xml"))
		{
			Corpus::CConfig::inputType = Corpus::CConfig::preMorph;
			return true;
		}
		if(!arg.compare("XML"))
		{
			Corpus::CConfig::inputType = Corpus::CConfig::preMorph;
			return true;
		}
		if(!arg.compare("txt"))
		{
			Corpus::CConfig::inputType = Corpus::CConfig::text;
			return true;
		}
		if(!arg.compare("TXT"))
		{
			Corpus::CConfig::inputType = Corpus::CConfig::text;
			return true;
		}
		if(!arg.compare("corpora"))
		{
			Corpus::CConfig::inputType = Corpus::CConfig::corpora;
			return true;
		}
		if(!arg.compare("CORPORA"))
		{
			Corpus::CConfig::inputType = Corpus::CConfig::corpora;
			return true;
		}
		if(!arg.compare("corpus"))
		{
			Corpus::CConfig::inputType = Corpus::CConfig::corpora;
			return true;
		}
		if(!arg.compare("CORPUS"))
		{
			Corpus::CConfig::inputType = Corpus::CConfig::corpora;
			return true;
		}
		
		return false;
	}

	bool CConfig::ParseClassifierToUse(std::string arg)
	{
		if(!arg.compare("r") || !arg.compare("R"))
		{
			classifierType = CConfig::Ripper;
			return true;
		}
		if(!arg.compare("l") || !arg.compare("L"))
		{
			classifierType = CConfig::LMT;
			return true;
		}
		if(!arg.compare("c") || !arg.compare("C"))
		{
			classifierType = CConfig::c45;
			return true;
		}
		if(!arg.compare("p") || !arg.compare("P"))
		{
			classifierType = CConfig::PART;
			return true;
		}
		if(!arg.compare("b") || !arg.compare("B"))
		{
			classifierType = CConfig::Bag;
			return true;
		}
		if(!arg.compare("w") || !arg.compare("W"))
		{
			classifierType = CConfig::VotingWTA;
			return true;
		}
		if(!arg.compare("a") || !arg.compare("A"))
		{
			classifierType = CConfig::VotingAverage;
			return true;
		}
		return false;
	}

	bool CConfig::ParseConvData(std::string arg)
	{
		if(!arg.compare("txt") || !arg.compare("TXT"))
		{
			task = convertingToTxt;
			return true;
		}
		else if(!arg.compare("bin") || !arg.compare("BIN"))
		{
			task = convertingToBin;
			return true;
		}
		return false;
	}
	
	void CConfig::DumpDiagInfo()
	{
		std::string empty;
		std::cout << "Informacje diagnostyczne" << std::endl
				<< "Dane tagera (TaKIPI::CConfig::commonPath): "
				<< commonPath << std::endl
				<< "Dane proj. Corpus (Corpus::CConfig::directory): "
				<< Corpus::CConfig::GetFullPath(empty) << std::endl
				<< "Typ pliku (input type): ";
		switch(Corpus::CConfig::inputType)
		{
			case Corpus::CConfig::text:
				std::cout << "text";
				break;
			case Corpus::CConfig::corpora:
				std::cout << "corpora";
				break;
			case Corpus::CConfig::preMorph:
				std::cout << "preMorph";
				break;
			default:
				std::cout << "NIEZNANY!";
		}
		std::cout << std::endl << "Wejsciowe kodowanie: "
				<< Corpus::Parser::GetEncodingName(Corpus::CConfig::inputEncoding, false)
				<< std::endl << std::endl;
	}

}//namespace TaKIPI
