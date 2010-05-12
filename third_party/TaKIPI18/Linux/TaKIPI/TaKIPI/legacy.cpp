//// Some stuff from old TaKIPI version -- a quick fix.
//// Used only for Teacher configuration.

#include "takipi_headers.h"

#ifdef TEACHER_EXE

#include "legacy.h"

int main(int argc, char* argv[])
{
	try
	{
		return legacymain(argc, argv);
	}
	catch(Corpus::tagexception &e)
	{
		std::cout << "tagexception:\n"
				<< e.what() << "\n";
	}
	catch(std::exception &e)
	{
		std::cout << std::endl
			<< typeid(e).name() << std::endl
			<< e.what();
	}
	catch(antlr::ANTLRException &e)
	{
		std::cout << "Error while parsing rules or patterns. ANTLR message:\n"
				<< e.toString()
				<< std::endl;
	}
	catch(std::string &e)
	{
		std::cout << e;
	}
	catch(...)
	{
		std::cout << "Unknown exception" << std::endl;
	}
	return -1;

}
int legacymain(int argc, char* argv[])
{
	using namespace std;
	using namespace TaKIPI;

	CConfig::ParseArgs(argc,argv);
	if(CConfig::task == CConfig::koding) // 0. KODER lub KONWERSJA
	{
		TaKIPI::CRTTTagger::RunKoder();
	}
	else if(CConfig::task == CConfig::convertingToBin)
	{
		TaKIPI::CRTTTagger::ConvertDicts(false);
	}
	else if(CConfig::task == CConfig::convertingToTxt)
	{
		TaKIPI::CRTTTagger::ConvertDicts(true);
	}
	else if(CConfig::task == CConfig::generatingDicts)
	{
		TaKIPI::CRTTTagger::GenerateDicts(CConfig::teacherSourceFile);
	}
	else if(CConfig::task == CConfig::training) // 1. UCZENIE DRZEW
	{
		ifstream file,file2;
		cout << "TRYB: Uczenie drzew.\n";
		if(CConfig::teacherSourceFile.empty())
		{
			std::cerr << "Blad: nie podano pliku uczacego\n";
			return 1;
		}
		file.open(TaKIPI::IO::GetFullPath(TaKIPI::CConfig::commonPath, TaKIPI::CConfig::wordDictFileName).c_str(),
			std::ios::binary);
		Corpus::CWordDictionary::Deserialize(file); // TODO: s�ownik nie jest te� �adowany przy tworzeniu teachera?
		file.close();

		// sciezka podana przez uzytkownika, nie robimy wiec GetFullPath
		file.open(CConfig::teacherSourceFile.c_str());

		Corpus::Tokenisation::CCorpusTokenReader reader(file);

		TaKIPI::CLayers l;
		l.SetUpDefaultSingleLayer(CConfig::layers);

		TaKIPI::Learning::CTeacher t(l,reader,CConfig::layers);
		//std::cout << "Teacher object created.\n";
		t.CreateFiles();
		std::cout << "Training started...\n";
		t.Learn(true,false,true);
		std::cout << "Training done!\n";

		ofstream fileO;
		fileO.open(TaKIPI::IO::GetFullPath(TaKIPI::CConfig::commonPath, TaKIPI::CConfig::wordDictFileName).c_str(), std::ios::binary);
		Corpus::CWordDictionary::GetInstance()->Serialize(fileO);
		fileO.close();
	}
	else if(CConfig::task == CConfig::testing) // 2. TESTOWANIE
	{
		int iterations = CConfig::iterations;
		double cutLevel = CConfig::cutLevel;
		JOSKIPI::Operators::COperator::collectNames = false;
		cout << "TRYB: Testowanie.\n";

		TaKIPI::CLayers l;
		l.SetUpDefaultLayers(CConfig::layers);

		CRTTTagger tagger(l,iterations,cutLevel,true);
		tagger.Load(); // laduje tez slowniki

		CTest *testClass = new CTest(l);
		ifstream file;
		// sciezka podana przez uzytkownika, nie robimy wiec GetFullPath
		file.open(CConfig::testFile.c_str());

		Corpus::Tokenisation::CCorpusTokenReader reader(file);
		Corpus::ExtSentencer::CSentencer sentr(reader);

		bool useRules = (CConfig::rulesUseNum > 0);

		Corpus::CSentence *readSent = NULL;
		Corpus::CSentence *disdSent = NULL, *tagdSent = NULL;

		Corpus::DLONG toks_total = 0;
		Corpus::DLONG toks_missed = 0;
		int counter = 0;
		int befLex = 0;
		int aftLex = 0;

		int amb = 0;
		int rejd = 0;

		while(true)
		{
			readSent = sentr.GetNext(false); // póki co nie mieszamy przy subst/ger/ign, wywołamy to potem (Eliminate)
			if(readSent == NULL)
			{ // koniec korpusu
				break;
			}
			// Corpus::Tokenisation wypluwa w cp1250
			// tager oczekuje kodowania utf8
			readSent->ConvertEncoding(Corpus::Parser::cp1250, Corpus::Parser::utf8);
			disdSent = readSent->CreateHardCopy(); // zdanie ujednoznacznione, zawiera tylko pozostawione leksemy (disamb nie ma znaczenia)
			disdSent->EliminateSubstGerIgn(); // nie psujemy readSent
			disdSent->ClearDisambsAndProbs(); // na wszelki wypadek -- żeby tager nie widział disambów w korpusie wzorcowym
			tagdSent = disdSent->CreateHardCopy(); // tu będzie zdanie oznakowane, zostawione wszystkie leksemy, liczy się disamb
			counter++;
			toks_total += readSent->Size();
			tagger.Disambiguate(*disdSent,true,false,useRules);
			tagger.TagSentence(tagdSent,disdSent);
			
			toks_missed += testClass->TestInterTagged(*tagdSent,*readSent);
			for(int i = 0; i < tagdSent->Size(); i++)
			{
				if(testClass->IsAmb(*tagdSent,i))
					amb++;

				befLex += (int)tagdSent->operator [](i)->GetLexCount();
				if((int)tagdSent->operator [](i)->GetLexCount() == 1)
				{
					aftLex++;
				}
				else
				{
					for(int j = 0; j < (int)tagdSent->operator [](i)->GetLexCount(); j++)
					{
						if( (int)tagdSent->operator [](i)->GetLexem(j)->GetDisamb())
						{
							aftLex++;
						}
					}
				}
			}

			Corpus::ExtSentencer::CSentenceReader::Free(disdSent);
			Corpus::ExtSentencer::CSentenceReader::Free(tagdSent);
			Corpus::ExtSentencer::CSentenceReader::Free(readSent);

			if (counter % 1000 == 0)
			{
				cout << "Przerobiono " << counter << " zdan (" 
					<< toks_missed << " nietrafionych / " << toks_total << " ["
					<< (double)toks_missed/toks_total*100 << "%])." << endl;
				cout << "Stosunek leksemow (przed/po): "
					<< (double)aftLex/befLex*100 << "%" << endl;

				cout << "Drzewa sie zgubily: " << TaKIPI::Classifiers::CDecisionTreeNode::statystyka << " razy." 
					<< endl;
				if(CConfig::useNegRules)
					cout << "Drzewa negatywne sie zgubily: " << TaKIPI::Classifiers::CNegRuleDecisionTreeNode::statystyka << " razy." 
						<< endl;

				cout << "Wycieto " << CCutOff::counter << " lexemow." << endl;
			}
		}

		file.close();

		char filename[255];
		if(CConfig::testOutFile.empty())
		{
			time_t rawtime;
			tm * ptm;
			time ( &rawtime );
			ptm = gmtime ( &rawtime );
	
			sprintf(filename,"Test_%d_%.2d.txt",ptm->tm_hour+1,ptm->tm_min);
		}
		else
			sprintf(filename,"%s",CConfig::testOutFile.c_str());

		ofstream fileO;
		fileO.open(filename,ios_base::out);
		fileO << "Ilosc iteracji:  " << iterations << endl;
		fileO << "Poziom odciecia: " << cutLevel << endl;
		fileO << "Przerobiono " << toks_total << " tokenow" << endl;
		fileO << "Nie trafiono " << toks_missed << " tokenow" << endl ;
		fileO << "Stosunek trafionych do calosci: " << (double)(toks_total-toks_missed)/toks_total*100 << "%" << endl;
		fileO << "Stosunek trafionych do niejednoznacznych: " << (double)(amb-toks_missed)/amb*100 << "%" << endl;
		fileO << "Ilosc tokenow niejednoznacznych:  " << amb << endl;
		fileO << "Ilosc leksemow przed:  " << befLex << endl;
		fileO << "Ilosc leksemow po:  " << aftLex << endl;
		fileO << "Stosunek lexemow (przed/po): " << (double)aftLex/befLex*100 << "%" << endl;
		fileO << "Stosunek disambow na token: " << (double)aftLex/toks_total << endl;
		fileO << "Drzewa sie zgubily: " << TaKIPI::Classifiers::CDecisionTreeNode::statystyka << " razy." << endl << endl;
		testClass->PrintStatistics(fileO);
		fileO.close();

		tagger.GetRules()->GetRulesInfo("Rules/RulesInfo.txt");

		delete testClass;
		return 0;
	}
	else // 3. TAGOWANIE.
	{
		std::cout << "TRYB: Tagowanie.\n";
		//ofstream errorLog;
		//errorLog.open("errorsLog.txt");

		JOSKIPI::Operators::COperator::collectNames = false;

		TaKIPI::CLayers l;
		l.SetUpDefaultLayers(CConfig::layers);
		
		int iterations = CConfig::iterations;
		double cutLevel = CConfig::cutLevel;

		CRTTTagger tagger(l,iterations,cutLevel);
		tagger.Load(); // laduje tez slowniki

		vector<string> inFiles;
		vector<string> outFiles;
		if(!CConfig::inputFile.empty())
		{
			inFiles.push_back(CConfig::inputFile);
			if(CConfig::outPutFile.empty())
				outFiles.push_back("temp.xml");
			else
				outFiles.push_back(CConfig::outPutFile);
		}
		else
		{
			ifstream scriptFile;
			scriptFile.open(CConfig::inputScriptFile.c_str());
			string line;

			do
			{
				std::getline(scriptFile, line);
				line = Corpus::Parser::Trim(line);
				if(!line.empty())
				{
					inFiles.push_back(line);
					line.append(".tag");
					outFiles.push_back(line);
				}
			}
			while(!scriptFile.eof() && !Corpus::Parser::Trim(line).empty());

			scriptFile.close();
		}
		
		for(int i = 0; i < (int)inFiles.size(); i++)
		{
			ifstream inFile;
			inFile.open(inFiles.at(i).c_str());
			if(!inFile)
			{
				std::cerr << "BLAD: Nie mozna otworzyc pliku "
						<< inFiles.at(i) << std::endl;
				continue;
			}
			ofstream outFile;
			outFile.open(outFiles.at(i).c_str());
			if(!outFile)
			{
				std::cerr << "BLAD: Nie mozna zapisywac do pliku "
						<< outFiles.at(i) << std::endl;
				continue;
			}
			Corpus::CXmlWriter writer(outFile);
			cout << endl << "Plik " << (i + 1) << "/" << ((int)inFiles.size())
				<< ": " << inFiles.at(i) << "\n";
			
			// Przypadek pierwszy -- bez fragmentow (tekst lub korpus)
			if(Corpus::CConfig::inputType != Corpus::CConfig::preMorph)
			{ // otwarcie XML -- nie skopiuje sie to z pliku wejsciowego
				outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
						"<!DOCTYPE cesAna SYSTEM \"xcesAnaIPI.dtd\">\n"
						"<cesAna version=\"1.0\" type=\"lex disamb\">\n"
						"<chunkList>\n";
				
				Corpus::Tokenisation::CTokenReader *reader = NULL;
				
				if(Corpus::CConfig::inputType == Corpus::CConfig::text)
				{
					reader = new Corpus::Tokenisation::CTokeniser(
							inFile, true, false, TaKIPI::CConfig::useOldReaders);
				}
				else if(Corpus::CConfig::inputType == Corpus::CConfig::corpora)
				{
					// useOldReaders nie ma znaczenia
					// -- i tak odczytujemy klasy z pliku
					reader = new Corpus::Tokenisation::CCorpusTokenReader(inFile);
				}
				else
				{
					// na wszelki wypadek, to raczej niemozliwe
					std::cerr << "BLAD: nieobslugiwany typ pliku wejsciowego\n";
					return 1;
				}
				// tworzymy sentencer na naszym readerze
				Corpus::ExtSentencer::CSentencer sentr(*reader);
				// pierwsze zdanie i petla
				Corpus::CSentence *s = sentr.GetNext();
				int counter = 0;
				do
				{
					tag_sentence(s, tagger, writer, l, counter);
					if(counter % 100 == 0)
						std::cout << "Przerobiono " << counter << " zdan.\r";
				}
				while( (s = sentr.GetNext()) != NULL );
				
				// zamkniecie XML -- nie skopiuje sie to z pliku wejsciowego
				outFile << "\n</chunkList>\n</cesAna>\n";
				delete reader;
			}
			else // Przypadek drugi: fragmenty (pre_morph)
			{ // tworzymy PRE_MORPH reader podajac wyjscie na tagi XML-owe
				Corpus::Tokenisation::CPremorphReader pm_reader(
						inFile, &outFile, true, false, TaKIPI::CConfig::useOldReaders);
				
				int counter = 0;
				while(pm_reader.NextFragment())
				{ // tworzymy sentencer w obrebie biezacego fragmentu
					Corpus::ExtSentencer::CSentencer sentr(pm_reader);
					// pierwsze zdanie i petla
					Corpus::CSentence *s = sentr.GetNext();
					do
					{
						// zdanie moze byc puste, ale obsluguje to tag_sentence
						tag_sentence(s, tagger, writer, l, counter);
						if(counter % 100 == 0)
							std::cout << "Przerobiono " << counter << " zdan.\r";
					}
					while( (s = sentr.GetNext()) != NULL );
				}
			}

			cout << endl;
			
			// writer.WriteSentence(s,empty,true);

			inFile.close();
			outFile.close();
		}

		//errorLog.close();
		return 0;
	}
	return 0;
}

void tag_sentence(Corpus::CSentence *s, TaKIPI::CRTTTagger &tagger, Corpus::CXmlWriter &writer, TaKIPI::CLayers &l, int &counter)
{
	static std::string empty; // dla XmlWritera -- pole befTextTags
	if(s && s->Size() != 0)
	{
		// w zdaniu mamy kodowanie CP1250 (z TokenReader i ExtSentencer),
		// tager oczekuje UTF8
		s->ConvertEncoding(Corpus::Parser::cp1250, Corpus::Parser::utf8);

		TaKIPI::CLexemAscendingSort las(l);
		for(int i = 0; i < s->Size(); i++)
		{
			sort((*s)[i]->GetLexems().begin(),(*s)[i]->GetLexems().end(),las);
		}
		Corpus::CSentence *final = s->CreateCopy();

		counter++;
		tagger.Disambiguate(*s,true,false,true);
		// wypisujemy wyjscie w UTF8 -- chyba to najlepsze rozwiazanie
		// (mozna ewentualnie konwertowac z powrotem na CConfig::inputEncoding,
		// ale wtedy wypadaloby wstawic inna wartosc w naglowek XML)
		switch(TaKIPI::CConfig::writeMultiple)
		{
		case TaKIPI::CConfig::one:
			tagger.TagSentence(final,s, true);
			writer.WriteSentence(final, empty, false);
			break;
		case TaKIPI::CConfig::many:
			// wyrzucamy zdanie bezposrednio po dezambiguacji
			writer.WriteSentence(s, empty, false);
			break;
		default:
			// wybieramy najlepszy tag
			tagger.TagSentence(final,s);
			writer.WriteSentence(final, empty, false);
		}
	
		Corpus::ExtSentencer::CSentenceReader::Free(final);
		Corpus::ExtSentencer::CSentenceReader::Free(s);
	}
}

#endif
