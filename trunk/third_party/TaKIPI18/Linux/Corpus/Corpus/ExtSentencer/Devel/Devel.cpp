#include "../../../corpuslib_headers.h"
#include "Devel.h"
#include "../../Corpus.h"
#include "../../TextUtils/Misc/Timer.h"
#include "../../Documents/Document.h"
#include "../../typedefs.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		namespace Devel
		{

			/// Przerobione z WordListExtractor Bartka Brody.
			void AbbrevExtractor::Extract()
			{
				// wczytaj korpus
				Corpus::CCorpus corpus;
				std::cout << "Reading the list of documents...";
				corpus.LoadTaggedDocuments(morphFN, false, prefixFN);
				std::cout << "The corpus contains " << corpus.Size() << " documents" << std::endl;
				
				Documents::Document *doc = NULL;

				Corpus::TextUtils::Misc::CTimer timer, last;
				double sumTime = 0;
				timer.Start();
				last.Start();

				CToken *token = NULL;
				size_t docNo = 0;

				if(corpus.GotoFirst())
				{
					do
					{
						doc = corpus.GetDocument();
						doc->Open();

						while((token = doc->NextToken()) != NULL)
						{
							// nowa grupa (==ciag tokenow bez spacji)
							if(!token->NS)
							{
								HandleGroup();
							}

							lastGroup.push_back(token);
						} // foreach token in doc
						HandleGroup();

						sumTime += timer.Stop();
						docNo++;
						if(last.Stop() > 10)
						{
							double avg = sumTime / docNo;
							double eta =  avg*(corpus.Size() - docNo);

							std::string jednostka = " s ";
							if( eta > 3*3600)
							{
								eta /= 3600;
								jednostka = " h ";
							}
							else
							{
								if( eta > 3* 60)
								{
									eta /= 60;
									jednostka = " min ";
								}
							}
							std::cout << "Done doc " << docNo << " out of " << corpus.Size() 
								<< " Time left: " << eta << jednostka << "Time avg: " << avg  <<"           \r";
							std::cout.flush();
							last.Start();
						}
						timer.Start();
						doc->Close();
					}
					while(corpus.Next());
				}
			}

			void AbbrevExtractor::HandleGroup()
			{
				int beg, end, glen = (int)lastGroup.size();
				if(glen == 0)
					return;

				for(beg = 0; beg < glen && IsInterp(lastGroup[beg]); beg++)
					; // pomijamy poczatkowe interp
				for(end = glen - 1; end >= 0 && IsNDInterp(lastGroup[end]); end--)
					; // pomijamy koncowe (interp \ {'.'})

				if(beg <= end)
				{
					if(IsDot(lastGroup[end]))
					{
						if(beg < end
							&& !IsInterp(lastGroup[end - 1]))
						{ // uznajemy jako skrot, dodajemy bez ostatniej kropki
							Insert(beg, end - 1, lst_abbr);
						}
					}
					else
					{
						Insert(beg, end, lst_notabbr);
					}
				}

				// zwalniamy pamiec
				for(int pos = 0; pos < glen; pos++)
				{
					delete lastGroup[pos];
				}
				lastGroup.clear();
			}

			bool AbbrevExtractor::IsDot(CToken *tok)
			{
				return
					(tok->GetOrth().compare(".") == 0);
			}

			bool AbbrevExtractor::IsInterp(CToken *tok)
			{
				return
					(tok->GetLexCount() > 0)
					&& tok->GetLexem(0)->GetTag().FlexClass == cf_interp;
			}

			bool AbbrevExtractor::IsNDInterp(CToken *tok)
			{
				return
					(tok->GetLexCount() > 0)
					&& tok->GetLexem(0)->GetTag().FlexClass == cf_interp
					&& (tok->GetOrth().compare(".") != 0);
			}

			void AbbrevExtractor::WriteDiffList(const std::string &filename)
			{
				const size_t mincount = 5;

				std::ofstream out(filename.c_str());
				if(!out)
				{
					std::cout << "Error opening output file: " << filename
						<< std::endl;
				}
				else
				{
					std::map<std::string, size_t>::iterator iter =
						lst_abbr.begin();

					while(iter != lst_abbr.end())
					{
						// co najmniej mincount wystapien i nie ma w lst_notabbr
						if(iter->second >= mincount
							&& lst_notabbr.find(iter->first) == lst_notabbr.end())
						{
							out << iter->first << "\t" << iter->second
								<< std::endl;
						}
						iter++;
					}
				}
			}

			void AbbrevExtractor::Write(const std::string &outFileName,
					std::map<std::string, size_t> &collection)
			{
				std::ofstream out(outFileName.c_str());
				if(!out)
				{
					std::cout << "Error opening output file: " << outFileName
						<< std::endl;
				}
				else
				{
					std::map<std::string, size_t>::iterator iter =
						collection.begin();

					while(iter != collection.end())
					{
						out << iter->first << "\t" << iter->second
							<< std::endl;
						iter++;
					}
				}
			}

			void AbbrevExtractor::Insert(int beg, int end,
					std::map<std::string, size_t> &collection)
			{
				std::stringstream form_buff;

				for(int pos = beg; pos <= end; pos++)
				{
					form_buff << lastGroup[pos]->GetOrth();
				}

				std::string form = form_buff.str();

				std::map<std::string, size_t>::iterator found =
					collection.find(form);
				if(found == collection.end())
				{
					// pierwsze wystapienie
					collection.insert(std::pair<std::string, size_t>(form, 1));
				}
				else
				{
					found->second++;
				}
			}

			void AbbrevExtractor::RunAll(const std::string &pseudococonfigPath)
			{
				try
				{
					//std::string morph, prefix, outA, outNA, outDiff;
					const int lines_total = 5;
					std::string lines[lines_total];
					std::ifstream conf(pseudococonfigPath.c_str());
					if(!conf) return;

					for(int line_num = 0; line_num < lines_total; line_num++)
					{
						do
						{
							std::getline(conf, lines[line_num]);
							if(!conf) throw std::string("Unexpected end of conf.txt");
						}
						while(lines[line_num].size() == 0 || lines[line_num][0] == '#');
						std::cout << lines[line_num] << std::endl;
					}

					AbbrevExtractor abb(lines[0], lines[1]);
					abb.Extract();
					abb.WriteAbbrList(lines[2]);
					abb.WriteNotAbbrList(lines[3]);
					abb.WriteDiffList(lines[4]);
				}
				catch(std::string &e)
				{
					std::cout << "ERROR: " << e << std::endl;
				}
				catch(std::exception &e)
				{
					std::cout << "ERROR: " << e.what() << std::endl;
				}
			}
			/////
			void AbbrevExtractor::HandleGroupRetok()
			{
				std::stringstream form_buff;
				int glen = (int)lastGroup.size();

				for(int pos = 0; pos < glen; pos++)
				{
					form_buff << lastGroup[pos]->GetOrth();
				}

				Corpus::Tokenisation::CTokeniser tokr(form_buff, false, true);
				// TODO: porownac z wejsciowymi i w NIEKTORYCH sytuacjach wyrzucic nowe,
				// w niektorych -- stare

				// zwalniamy pamiec
				for(int pos = 0; pos < glen; pos++)
				{
					delete lastGroup[pos];
				}
				lastGroup.clear();
			}

		}// namespace Devel
	} // namespace ExtSentencer
} // namespace Corpus
