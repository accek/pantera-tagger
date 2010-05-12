#include "../../../corpuslib_headers.h"
#include "Misc.h"

namespace Corpus
{
	namespace TextUtils
	{
		namespace Misc
		{
			void ReadWords(std::set<std::string> & words, std::string filename) throw(std::string)
			{
				std::ifstream file(filename.c_str());
				const size_t BUF_SIZE = 1024;
				char buf[BUF_SIZE];
				std::string line;
				if(!file)
					throw (std::string("File not found: ")+filename);
				size_t line_no = 0;
				while(file)
				{
					file.getline(buf,BUF_SIZE);
					line = buf;
					boost::algorithm::trim(line);
					if( !line.empty() )
					{
						bool unique = words.insert( line ).second;

						if(!unique)
							std::cerr << "Found not unique word:" << line << std::endl;
					}
					line_no++;

				};
			}

			void ReadWords(std::set<std::string> & words, std::string filename, std::string stoplist_filename) throw(std::string)
			{
				std::set<std::string> stoplist;
				if(! stoplist_filename.empty() )
					ReadWords(stoplist, stoplist_filename);
				ReadWords(words, filename);
				if(! stoplist_filename.empty() )
					FilterWords(words, stoplist);
			}


			void ReadWords(std::vector<std::string>&words, std::string filename, std::string stoplist_filename)
			{
				std::set<std::string> uniqueWords;
				ReadWords(uniqueWords, filename, stoplist_filename);

				for(std::set<std::string>::iterator word = uniqueWords.begin(); word != uniqueWords.end(); word++)
					words.push_back(*word);

			}

			void ReadWords(std::vector<std::string>&words, std::string filename)
			{
				std::set<std::string> uniqueWords;
				ReadWords(uniqueWords, filename);

				for(std::set<std::string>::iterator word = uniqueWords.begin(); word != uniqueWords.end(); word++)
					words.push_back(*word);
			}


			void ReadIntList(std::vector<int> & values, std::string filename) throw(std::string)
			{
				using boost::lexical_cast;
				using boost::bad_lexical_cast;

				const size_t BUF_SIZE = 1024;
				char buf[BUF_SIZE];

				std::ifstream in(filename.c_str());
				if(!in)
					throw std::string( std::string("Couldn't open file for reading:" ) + filename);
				std::string line;
				int line_no = 0;

				std::cout << "Reading rows descriptions from file "<< filename <<"..." << std::endl;
				while(in)
				{
					in.getline(buf,BUF_SIZE);
					line = buf;
					boost::algorithm::trim(line);
					if( !line.empty() )
					{
						try
						{
							values.push_back(lexical_cast<int>(line));
						}
						catch(bad_lexical_cast)
						{
							std::cerr << "Not a number in line " << line_no << ". Line contains: " << line << std::endl;
						}
					}

					line_no++;

				};
			}

			void ReadIntList(std::vector<size_t> & values, std::string filename) throw(std::string)
			{
				using boost::lexical_cast;
				using boost::bad_lexical_cast;
				const size_t BUF_SIZE = 1024;
				char buf[BUF_SIZE];

				std::ifstream in(filename.c_str());
				if(!in)
					throw std::string( std::string("Couldn't open file for reading:" ) + filename);
				std::string line;
				int line_no = 0;

				std::cout << "Reading rows descriptions from file "<< filename <<"..." << std::endl;
				while(in)
				{
					in.getline(buf,BUF_SIZE);
					line = buf;
					boost::algorithm::trim(line);
					if( !line.empty() )
					{
						try
						{
							values.push_back(lexical_cast<size_t>(line));
						}
						catch(bad_lexical_cast)
						{
							std::cerr << "Not a number in line " << line_no << ". Line contains: " << line << std::endl;
						}
					}

					line_no++;

				};
			}

			void ReadWords(std::set<int> & words, std::string filename, Corpus::CWordDictionary * dict) throw(std::string)
			{

				std::ifstream file(filename.c_str());
				const size_t BUF_SIZE = 1024;
				char buf[BUF_SIZE];
				std::string line;
				if(!file)
					throw (std::string("File not found: ")+filename);

				while(file)
				{
					file.getline(buf,BUF_SIZE);
					line = buf;
					boost::algorithm::trim(line);
					if( !line.empty() )
					{
						words.insert( dict->Add(line));
					}

				};
			}



			void SaveWords(std::set<int> & words, std::string filename, Corpus::CWordDictionary * dict) throw(std::string, Corpus::tagexception)
			{
				std::ofstream file(filename.c_str());

				std::string line;
				if(!file)
					throw (std::string("File not found: ")+filename);

				for(std::set<int>::iterator i = words.begin(); i != words.end(); i++)
				{
					file << dict->GetAt(*i) << std::endl;
				}
				file.close();

			}

			void SaveWords(std::set<std::string> & words, std::string filename) throw(std::string)
			{
				std::ofstream file(filename.c_str());

				std::string line;
				if(!file)
					throw (std::string("File not found: ")+filename);

				for(std::set<std::string>::iterator i = words.begin(); i != words.end(); i++)
				{
					file << *i << std::endl;
				}
				file.close();

			}

			void SaveWords(std::vector<std::string> & words, std::string filename)
			{
				std::ofstream file(filename.c_str());

				std::string line;
				if(!file)
					throw (std::string("File not found: ")+filename);

				for(std::vector<std::string>::iterator i = words.begin(); i != words.end(); i++)
				{
					file << *i << std::endl;
				}
				file.close();
			}

			void FilterWords(std::set<std::string> & words, std::set<std::string> & stoplist)
			{
				for(std::set<std::string>::iterator i = stoplist.begin(); i != stoplist.end(); i++)
					words.erase(*i);

			}

		} // namespace Misc
	} // namespace TextUtils
} // namespace Corpus
