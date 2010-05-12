#include "../../../corpuslib_headers.h"
#include "FrequencyList.h"
#include "../../Parser/Parser.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace FrequencyList
		{
			// Wczytuje listê plików korpusu do wektora.
			void LoadCorporaFile(std::string listFileName, std::vector<std::string>& list)
			{
				std::ifstream script(listFileName.c_str());

				char* linia = new char[4096];
				std::string s;

				do
				{
					script.getline(linia,4096);
					s = linia;
					boost::algorithm::trim(s);
					if(!s.empty())
					{
						list.push_back(s);
					}
				}
				while(!script.eof() && !s.empty());

				delete[]linia;
				script.close();
			}

			// Znajduje nastêpne wyst¹pienie w pliku konstrukcji "<orth>forma</orth>",
			// zwraca true jak znajdzie forme, jeœli koniec pliku false.
			// parametf ignoreIng wymusza ignorowanie tokenów nieznanych (ign)
			bool FindNextOrth(std::ifstream& xmlFile, std::string& orth, bool ignoreIgn)
			{
				char* linia = new char[4096];
				std::string s = "";

				do
				{
					xmlFile.getline(linia,4096);
					s += linia;
					boost::algorithm::trim(s);

					std::string::size_type orthFound = s.find("<orth>");
					if(orthFound != s.npos)
					{
						std::string::size_type orthEndFound = s.find("</orth>");
						if(orthEndFound == s.npos)
							continue;
						else
						{
							orth = s.substr(orthFound + 6, orthEndFound - orthFound - 6);

							bool doBreak = false;
							do
							{
								xmlFile.getline(linia,4096);
								s = linia;
								boost::algorithm::trim(s);
								if(s.find("<ctag>ign</ctag>") != s.npos)
								{
									doBreak = true;
									break;
								}
								if(s.find("<tok>") != s.npos)
									break;
							}
							while(!xmlFile.eof() && !s.empty());

							if(!doBreak)
							{
								delete[]linia;
								return true;
							}
						}
					}
					s+= "";
				}
				while(!xmlFile.eof() && !s.empty());


				delete[]linia;
				return false;
			}

			// Dodaje dan¹ formê "orth" do listy "list", wraz ze zaktualizowaniem jej czêstoœci wyst¹pienia
			// (jeœli ju¿ istnia³a). Ostatni parametr sygnalizuje po ilu elementach dodanych do s³ownika
			// wyœwietliæ informacj na konsolê.
			void AddToList(std::string& orth, std::map<std::string,int>& list, int consoleInformFrequency)
			{
				if(list.find(orth) != list.end())
				{
					list[orth] = list[orth] + 1;
				}
				else
				{
					list.insert(std::pair<std::string,int>(orth,1));
					if(consoleInformFrequency != 0
						&& (int)list.size() % consoleInformFrequency == 0)
						std::cerr << "Dodano " << list.size() << " form do listy." << std::endl;
				}
			}

			// Zapisuje listê do pliku.
			void SaveList(std::map<std::string,int>& list, std::string fileName)
			{
				std::cerr << "Rozpoczecie zapisywania listy." << std::endl;

				std::ofstream fileU(fileName.c_str());
				if(!fileU)
				{
					std::string msg("FrequencyList::SaveList --- cannot open file ");
					msg.append(fileName);
					throw tagexception(msg.c_str());
				}

				std::map<std::string,int>::iterator iter = list.begin();
				for(;iter != list.end(); iter++)
				{
					fileU << iter->first << "\t" << iter->second << std::endl;
				}

				fileU.close();

				std::cerr << "Zakonczenie zapisywania listy." << std::endl;
			}

			// Wczytuje listê z pliku. Zak³adamy, ¿e kodowanie to CP1250!
			void LoadList(std::map<std::string,int>& list, std::string fileName, int consoleInformFrequency)
			{
				std::cerr << "Rozpoczecie wczytywania listy." << std::endl;

				std::ifstream fileU(fileName.c_str());
				if(!fileU)
				{
					std::string msg("FrequencyList::LoadList --- cannot open file ");
					msg.append(fileName);
					throw tagexception(msg.c_str());
				}

				char* buff = new char[4096];
				std::string s;

				do
				{
					fileU.getline(buff,4096);
					s = buff;
					boost::algorithm::trim(s);
					//transform(s.begin(),s.end(),s.begin(),(int(*)(int))tolower);

					if(!s.empty())
					{
						std::vector<std::string> tokens;
						Parser::Tokenize(s,tokens," \t");

						strcpy(buff, tokens.front().c_str());
						std::string form(Parser::ToLowerCP1250(buff));

						if(list.find(form) == list.end())
							list.insert(std::pair<std::string,int>(form,atoi(tokens.back().c_str())));
						else
							list.insert(std::pair<std::string,int>(form,list[tokens.front()] + atoi(tokens.back().c_str())));

						if(consoleInformFrequency != 0
							&& (list.size() % consoleInformFrequency) == 0)
							std::cerr << "\r- wczytano " << list.size() << " form";
					}
				}
				while(!fileU.eof() && !s.empty());

				delete[]buff;
				fileU.close();

				std::cerr << "\nZakonczenie wczytywania listy." << std::endl;
			}

			void DivideList(std::string fileName, std::string newFileName1, std::string newFileName2, double ratio)
			{
				std::cerr << "Rozpoczecie wczytywania listy." << std::endl;

				std::ifstream fileU(fileName.c_str());
				std::ofstream file1(newFileName1.c_str());
				std::ofstream file2(newFileName2.c_str());

				char* linia = new char[4096];
				std::string s;

				do
				{
					fileU.getline(linia,4096);
					s = linia;

					if( ((double)rand()/(double)RAND_MAX) < ratio )
						file1 << s << std::endl;
					else
						file2 << s << std::endl;
				}
				while(!fileU.eof() && !s.empty());

				delete[]linia;
				fileU.close();
				file1.close();
				file2.close();

				std::cerr << "\nZakonczenie wczytywania listy." << std::endl;
			}

			// G³ówna metoda tworz¹ca listê czêstoœci form wyrazów.
			void CreateFrequencyList(std::string corporaDocumentsListFile, std::string outputFile,  int consoleInformFrequency, bool ignoreIgn)
			{
				std::vector<std::string> corporaFileNamesList;
				LoadCorporaFile(corporaDocumentsListFile, corporaFileNamesList);

				std::map<std::string,int> formsList;
				for(int i = 0; i < (int)corporaFileNamesList.size(); i++)
				{
					std::ifstream file(corporaFileNamesList[i].c_str());

					std::string orth;
					while(FindNextOrth(file,orth, ignoreIgn))
					{
						AddToList(orth, formsList, 1000);
					}

					if(consoleInformFrequency != 0
						&& ((i+1) % consoleInformFrequency) == 0)
						std::cerr << "Przetworzono " << i+1 << " dokumentów." << std::endl;

					file.close();
				}

				SaveList(formsList, outputFile);
			}
		}
	}
}
