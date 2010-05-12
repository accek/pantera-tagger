#include "takipi_headers.h"
//#include <vld.h>

#ifdef TEACHER_EXE
#include "legacy.h"
#endif 

#ifdef WIN32

#ifndef WITH_MAIN

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#endif //!TEST_WITH_MAIN


#endif // WIN32

#include "TaKIPI/takipi_all.h"


TaKIPI::CRTTTagger* taggerDll = 0;
TaKIPI::CLayers* taggerLayers = 0;
// Corpus::Parser::Encoding user_encoding = Corpus::Parser::utf8;

TAKIPI_API void TaggerStart()
{
	//Corpus::Morfeusz::SetEncoding(1250);
	// Corpus::CConfig::inputEncoding = Corpus::Parser::cp1250;

	taggerLayers = new TaKIPI::CLayers();
	taggerLayers->SetUpDefaultLayers();

	taggerDll = new TaKIPI::CRTTTagger(*taggerLayers,1,0.01);
	taggerDll->Load();
}

TAKIPI_API void TaggerEnd()
{
	if(taggerDll)
		delete taggerDll;
	if(taggerLayers)
		delete taggerLayers;
}

/** Glowna funkcja z API tagera -- taguje tekst zakladajac wejsciowe kodowanie
  * w Corpus::CConfig::inputEncoding. Zwraca wynik jako XML, rowniez w kodowaniu
  * Corpus::CConfig::inputEncoding */
TAKIPI_API char* Tag(const char *text)
{
	using namespace std;
	using namespace TaKIPI;
	using namespace Corpus;
	using namespace JOSKIPI::Operators;

	stringstream out;
	stringstream in;
	in << text;

	COperator::collectNames = false;

	string tags;

	Corpus::Tokenisation::CTokeniser tokr(in, true, false);
	//CSentencer sentencer(&tokr);
	Corpus::ExtSentencer::CSentencer sentencer(tokr);
	CSentence* s = sentencer.GetNext();
	CSentence* org = NULL;

	if(Corpus::CConfig::inputType == Corpus::CConfig::text)
		tags = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
		"<!DOCTYPE cesAna SYSTEM \"xcesAnaIPI.dtd\">\n" \
		"<cesAna version=\"1.0\" type=\"lex disamb\">\n" \
		"<chunkList>\n";
	
	CXmlWriter writer(out);

	do
	{
		// cp1250 -> utf8 (wymog regul tagerowych)
		s->ConvertEncoding(Corpus::Parser::cp1250, Corpus::Parser::utf8);
		//tags = sentencer.befTextTags; TODO: na pewno to niepotrzebne??
		if(s)
		{
			org = s->CreateCopy();
			taggerDll->Disambiguate(*s,true,false,true);
			taggerDll->TagSentence(org,s);
			org->ConvertEncoding(Corpus::Parser::utf8, Corpus::CConfig::inputEncoding);
			writer.WriteSentence(org, tags, false);
		}
		else
		{
			s->ConvertEncoding(Corpus::Parser::utf8, Corpus::CConfig::inputEncoding);
			writer.WriteSentence(s, tags, false);
		}
		if(s)
		{
			Corpus::ExtSentencer::CSentenceReader::Free(org);
			Corpus::ExtSentencer::CSentenceReader::Free(s);
		}
		tags = "";
	}
	while( (s = sentencer.GetNext()) != 0);

	if(Corpus::CConfig::inputType == Corpus::CConfig::text)
		tags = "\n</chunkList>\n</cesAna>\n";
	/* else
		tags = sentencer.befTextTags; */
	// s->ConvertEncoding(Corpus::Parser::utf8, Corpus::CConfig::inputEncoding);
	// teraz s == NULL
	writer.WriteSentence(s,tags,true);

	std::string outstring = out.str();
	int outlen = (int)outstring.length();
	char *buff = new char[outlen + 1];
	strcpy(buff, outstring.c_str());
	buff[outlen] = 0;
	
	return buff;
}

TAKIPI_API void SetInputEncoding(int encoding)
{
	//Corpus::Morfeusz::SetEncoding(1250);
	////CConfig::morfeuszEncoding = encoding;
	//Corpus::CConfig::inputEncoding = Corpus::Parser::cp1250;
	Corpus::CConfig::inputEncoding = (Corpus::Parser::Encoding) encoding;
	//user_encoding = (Corpus::Parser::Encoding) encoding;

}

TAKIPI_API void FreeTaggerMemory(char *memory)
{
	delete memory;
}

TAKIPI_API void SetDataDir(const char *dataPath)
{
	TaKIPI::CConfig::commonPath = dataPath;
}

#ifdef WITH_MAIN


#ifdef TAGGER_EXE

#include "TaKIPI/takipi_all.h"

void LoadDoc(char *filename, std::string &out)
{
	static const size_t SIZE = 100000;
	std::ifstream file(filename);
	char bufer[SIZE];

	while(file)
	{
		file.getline(bufer, SIZE-1);
		out += bufer;
	}
}


int main(int ac, char** av)
{	
	try
	{
		std::cout << "Testuje!\n";
		std::string text;
		LoadDoc("test.txt", text);
		TaggerStart();
	
		std::ofstream out("out_test.txt");
	
		Corpus::CConfig::inputType = Corpus::CConfig::preMorph;
	
		SetInputEncoding(1250);
	
		std::cout << "Tagging..." << std::endl;
		char *x = Tag(text.c_str());
		out << x;
		out.close();
		
		FreeTaggerMemory(x);
	
		std::cout << "Done... Cleaning up...\n";
		TaggerEnd();
	}
	catch(std::string &s)
	{
		std::cerr << "Error: " << s << std::endl;
	}
	catch(std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}
#endif //#ifdef TAGGER_EXE 

#ifdef DICTIONARY_EXE

int main()
{
	std::cout << "Tutaj powinien byc uruchomiony rzeczy zwiazane z dictionary exe..." << std::endl;
	return 0;
}

#endif 

#endif //WITH_MAIN
