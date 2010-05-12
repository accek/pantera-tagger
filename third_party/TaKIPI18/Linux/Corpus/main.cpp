#include "corpuslib_headers.h"

//#include <vld.h>

#ifdef WIN32

#if  defined(TEST_WITH_MAIN) && !defined(CORPUS_STATIC)

BOOL APIENTRY DllMain( HANDLE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif //test with main

#endif // win32

#ifdef TEST_WITH_MAIN

#include "Corpus/corpus_all.h"

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		bool old = (argv[1][0] == 'o');
		// Corpus::CConfig::guesserUsage = Corpus::GuessingSystem::FormsIdentifier::guessNone;
		//Corpus::CConfig::inputEncoding = Corpus::Parser::cp852;
		//Corpus::CConfig::SetPathsToDirectory("Data");
		while(true)
		{
			std::cout << "> ";
			std::string line;
			std::getline(std::cin, line);
			if(line.empty())
				break;
			
			std::stringstream stream(line);
			Corpus::Tokenisation::CTokeniser tokr(stream, true, false, old);
			Corpus::ExtSentencer::CSentencer sentr(tokr);
			Corpus::CSentence *sent;
			while((sent = sentr.GetNext()) != NULL)
			{
				sent->ConvertEncoding(Corpus::Parser::cp1250, Corpus::CConfig::inputEncoding);
				std::cout << "NEW SENTENCE\n" << sent->ToXmlString(true);
				
				Corpus::ExtSentencer::CSentencer::Free(sent);
			}
		}
	}
	else
	{
		std::cerr << morfeusz_about();
		Corpus::GuessingSystem::GuesserTest::TestingConsoleUI tester;
		tester.AskAndExecute();
	
		Corpus::Readers::CReader::CleanUp();
	}

	return 0;
}

#endif //test debug
