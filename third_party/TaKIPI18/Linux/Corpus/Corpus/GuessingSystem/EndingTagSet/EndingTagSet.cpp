#include "../../../corpuslib_headers.h"
#include "EndingTagSet.h"
#include "../../Parser/Parser.h"
#include "../../Morfeusz/morfeusz.h"
#include "../../Tag.h" // negacja
#include "../../Config.h" // maska klas fleksyjnych
#include "../BaseEnding/Prefixes.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace EndingTagSet
		{
			// Konstruktor deserializuj�cy
			TagCount::TagCount(std::ifstream &file)
			{
				file.read((char *)&this->flexClass, sizeof(DLONG));
				file.read((char *)&this->gramCat, sizeof(DLONG));
				file.read((char *)&this->count, sizeof(UINTEGER));
				file.read((char *)&this->cutOffset, sizeof(UBYTE));
				file.read((char *)&this->baseEndingId, sizeof(UINTEGER));
			}

			void TagCount::Serialize(std::ofstream &file)
			{
				file.write((char *)&this->flexClass, sizeof(DLONG));
				file.write((char *)&this->gramCat, sizeof(DLONG));
				file.write((char *)&this->count, sizeof(UINTEGER));
				file.write((char *)&this->cutOffset, sizeof(UBYTE));
				file.write((char *)&this->baseEndingId, sizeof(UINTEGER));
			}

			// Konstruktor deserializuj�cy
			EndingTagSet::EndingTagSet(std::ifstream &file)
			{
				file.read((char *)&offset, sizeof(UINTEGER));
				file.read((char *)&count_, sizeof(UINTEGER));
				// TagCounts
				tagsCounts_ = new TagCount*[count_];
				for(UINTEGER i = 0; i < count_; i++)
				{
					tagsCounts_[i] = new TagCount(file);
				}
			}

			// Destruktor -- zwalnia tablic� tagsCounts (obiekty TagCount).
			EndingTagSet::~EndingTagSet()
			{
				if(this->tagsCounts_ != NULL)
				{
					for(UINTEGER i = 0; i < this->count_; i++)
					{
						delete this->tagsCounts_[i];
					}
					delete [] this->tagsCounts_;
				}
			}

			/** Dodaje tagset z podanej formy.
			Poprawka: dodane formCount (liczno�� formy w danych ucz�cych).
			Poprawka: odgadywanie form bazowych.
			*/
			void EndingTagSet::AddTags(const std::string &form, int formCount)
			{
				std::vector<Corpus::CToken *> morphed;
				Morfeusz::Analyse(morphed, form);
				CToken *tok = morphed[0];

				for(int i = 0; i < (int)tok->GetLexCount(); i++)
				{
					CLexem *lex = tok->GetLexem(i);
					DLONG flexClass = (DLONG)lex->GetTag().FlexClass;
					DLONG gramCat   = (DLONG)lex->GetTag().Categories;

					// bierzemy pod uwag� tylko leksemy o wybranych warto�ciach POS (flexClass)
					if(flexClass & CConfig::flexClassesToGuess)
					{
						UBYTE cutOffset;
						UINTEGER baseEndingId;

						int prefixToSkip = BaseEnding::PreProcess(flexClass, gramCat, lex->GetBase().c_str(), tok->GetOrth().c_str());
						BaseEnding::FindBaseEnding(lex->GetBase().c_str(),
							tok->GetOrth().c_str(), cutOffset, baseEndingId, prefixToSkip);

						int position = this->FindTag(flexClass,gramCat);
						if(position == -1)
						{
							this->AddNewTagCount(new TagCount(flexClass, gramCat, (UINTEGER)formCount, cutOffset, baseEndingId));
						}
						else
						{
							this->tagsCounts_[position]->count += formCount;
						}
					}
				}
				
				// free memory
				for(int tok_num = 0; tok_num < (int)morphed.size(); ++tok_num)
				{
					delete morphed[tok_num];
				}
			}

			/** Dodaje nowy tag do struktury. Poprawka: dodane formCount (liczno�� formy w danych ucz�cych).
			Poprawka: zgadywanie form bazowych.
			*/
			void EndingTagSet::AddNewTagCount(TagCount *tagCount)
			{
				if(this->tagsCounts_ == NULL) // tablica nie zosta�a jeszcze utworzona
				{
					this->tagsCounts_ = new TagCount *[1];
					this->tagsCounts_[0] = tagCount;
					this->count_ = 1;
				}
				else
				{
					TagCount** newTagsCounts = new TagCount*[this->count_+1];
					
					newTagsCounts[this->count_] = tagCount;

					memcpy(newTagsCounts, this->tagsCounts_, this->count_ * sizeof(UINTEGER));

					delete[]this->tagsCounts_;
					this->tagsCounts_ = newTagsCounts;
					this->count_++;
				}
			}

			// Dodawnaie tagu (mo�e ju� istnie� w wektorze, liczno�ci s� dodawane).
			void EndingTagSet::AddTag(TagCount *tagCount)
			{
				int tagNo = FindTag(tagCount->flexClass, tagCount->gramCat);

				if(tagNo != -1)
				{
					this->tagsCounts_[tagNo]->count += tagCount->count;
				}
				else
				{
					this->AddNewTagCount(tagCount);
					//this->AddNewTag(tagCount->flexClass, tagCount->gramCat, tagCount->count);
					this->tagsCounts_[this->count_-1]->count += tagCount->count - 1;
				}
			}

			// Wyszukuje podany tag, zwracaj�c jego numer lub -1 je�li takowego nie znajdzie.
			int EndingTagSet::FindTag(DLONG flexClass, DLONG gramCat)
			{
				for(UINTEGER i = 0; i < this->count_; i++)
				{
					if(this->tagsCounts_[i]->flexClass == flexClass
						&& this->tagsCounts_[i]->gramCat == gramCat)
						return i;
				}

				return -1;
			}

			void EndingTagSet::Serialize(std::ofstream &file)
			{
				file.write((char *)&offset, sizeof(UINTEGER));
				file.write((char *)&count_, sizeof(UINTEGER));
				// TagCounts
				for(UINTEGER i = 0; i < count_; i++)
				{
					tagsCounts_[i]->Serialize(file);
				}
			}

			void Serialize(std::ofstream& file, std::vector<EndingTagSet *> &endingVector)
			{
				INTEGER size = (INTEGER)endingVector.size();
				file.write((char*)&size, sizeof(INTEGER));
				for(INTEGER i = 0; i < size; i++)
					endingVector[i]->Serialize(file);
			}

			void Deserialize(std::ifstream &file, std::vector<EndingTagSet *> &endingVector)
			{
				INTEGER size;
				file.read((char*)&size, sizeof(INTEGER));
				endingVector.reserve(size);
				for(INTEGER i = 0; i < size; i++)
					endingVector.push_back(new EndingTagSet(file));
			}

			void CreateEndingTagSetsList(EndingTree::EndingTreeNode* tree, STRING2INT_MAP &formsList,
										std::vector<EndingTagSet*>& outList, int consoleInformFrequency)
			{
				std::cerr << "Rozpoczecie zapisywania tagsetow koncowek." << std::endl;

				// Wyczysc licznosci w drzewie
				tree->ClearCount();

				// Dla ka�dej formy...
				STRING2INT_MAP::iterator formIter;
				int formsProcessed = 0;
				for(formIter = formsList.begin(); formIter != formsList.end(); formIter++)
				{
					// ... odwr�� wynikow� form� (heurystyka dla prefiks�w wy��czona)
					
					// std::string postForm = BaseEnding::Prefixes::GetInstance()->ProcessPrefixes(formIter->first);
					std::string postForm = formIter->first;
					reverse(postForm.begin(), postForm.end());

					// ... u�yj odwr�conej formy do znalezienia w�z�a drzewa
					EndingTree::EndingTreeNode *node = tree->FindLongestEndingRef(postForm.c_str());
					EndingTagSet *tagSet = NULL;

					// POPRAWKA: je�li wyl�dowali�my w korzeniu, nie dodajemy tag�w
					if(node != tree)
					{
						// Je�li w�ze� istnieje to jest pobierany, je�li nie to jest tworzony.
						if(node->count_ == 0)
						{
							tagSet = new EndingTagSet();
							tagSet->count_ = 0;
							tagSet->offset = (int)outList.size() + 1;
							node->count_ = (int)outList.size() + 1;
							outList.push_back(tagSet);
						}
						else
						{
							tagSet = outList[node->count_-1];
						}

						tagSet->AddTags(formIter->first, formIter->second);
					}

					formsProcessed++;

					if(consoleInformFrequency != 0
						&& formsProcessed % consoleInformFrequency == 0)
						std::cerr << "\n- dodano " << formsProcessed << " form";
				}

				std::cerr << std::endl << "\nZakonczenie zapisywania tagsetow koncowek." << std::endl;
			}

			// Zgaduje analiz� morfologiczn� formy. Je�li nie znajdzie nic sensownego, zwraca igna.
			CToken* GuessForm(const char* form, EndingTree::EndingTreeNode* tree, std::vector<EndingTagSet*>& outList, bool takeNodeBefIgn)
			{
				bool ignNow =  true;
				std::string orth(form);
				CToken* token = CToken::CreateBlank(orth);
				token->ClearLexems();

				std::string lower(form);
				Parser::ToLowerCP1250(lower);
				//std::string postForm = BaseEnding::Prefixes::GetInstance()->ProcessPrefixes(lower); // heurystyka wy��czona
				std::string postForm(lower);
				
				reverse(postForm.begin(), postForm.end());
				EndingTree::EndingTreeNode* node = tree->FindLongestEndingRef(postForm.c_str(),takeNodeBefIgn);

				if(node != NULL && node->count_ != 0) // uda�o si� odnale�� w�ze� w drzewie
				{
					EndingTagSet* endingTagSet = outList[node->count_ - 1];
					
					if(endingTagSet->count_ != 0) // w�ze� jest niepusty => to nie ign
					{
						ignNow = false;
						double sum = 0;
						for(UINTEGER i = 0; i < endingTagSet->count_; i++)
						{
							sum += endingTagSet->tagsCounts_[i]->count;
						}
	
						for(UINTEGER i = 0; i < endingTagSet->count_; i++)
						{
							CLexem *lexem = new CLexem();
							TagCount *tagCount = endingTagSet->tagsCounts_[i];
							
							TFlexClass postFlex = tagCount->flexClass;
							TCategories postCat = tagCount->gramCat;
							
							int prefixToSkip = BaseEnding::PostProcess(postFlex, postCat, orth.c_str());
							lexem->SeBase(BaseEnding::GenerateBase(lower.c_str(), tagCount->cutOffset, tagCount->baseEndingId, prefixToSkip));
							
							// ustawiamy warto�ci atrybut�w leksemu wraz z prawdopodobie�stwem
							CTag tag(postFlex);
							tag.Categories = postCat;
							lexem->SetTag(tag);
							lexem->probability = (double)tagCount->count/sum;
							token->AddLexem(lexem);
						}
					}
				}
				
				if(ignNow) // nie uda�o si� odgadn�� wyrazu, zwracamy ign
				{
					CLexem* lexem = new CLexem();
					lexem->SeBase(orth); // jak w KIPI, zostawiamy oryginalne litery (wielkie/ma�e)
					CTag tag(cf_ign);
					lexem->SetTag(tag);
					/* Leksemy odgadni�te przez guesser maj� ustalone prawdopodobie�stwo,
					 * te z Morfeusza maj� zawsze 0; przyjmujemy, �e nieodgadani�te maj� te� 0
					 * (poniewa� nie s� one pr�b� zgadni�cia, lecz po prostu ignem, zgodnie z Morf.)
					 */
					lexem->probability = 0.0;
					token->AddLexem(lexem);
				}

				return token;
			}

			// Metoda dla w�z�a nie posiadaj�cego ko�c�wki przypisuje ko�c�wk� kt�rego� z jego dzieci b�d�cego li�ciem.
			/* Wracaj�c z rekurencji szukamy w�z��w bez przypisanych tag�w;
			   dla takiego w�z�a pr�bujemy bezpo�rednio znale�� dziecko bez potomstwa,
			   je�li mamy, to wpisujemy w nasz w�ze� TagSet owego dziecka,
			   a dziecko usuwamy.
			   Mo�emy w ten spos�b skr�ci� �cie�ki, kt�re nie rozga��ziaj� si�.
			   Niestety nie pomo�e to na w�z�y bez tag�w kt�re nie maj� bezdzietnego potomstwa.
			   */
			void AttachLeafToIgn(EndingTree::EndingTreeNode* tree)
			{
				// Zako�czenie rekurencji.
				if(tree->childsCount_ == 0)
					return;

				// Najperw nastepuje rekurencja w li�ciach id�c w stron� korzenia.
				for(int child = 0; child < tree->childsCount_; child++)
				{
					AttachLeafToIgn(tree->childsNodes_[child]);
				}

				// tree->count to adres w tablicy outList
				// je�li to pu�cimy po pierwszym konstruktorze Guessera, to mamy adresy
				if(tree->count_ == 0)
				{
					for(int child = 0; child < tree->childsCount_; child++)
					{
						if(tree->childsNodes_[child]->childsCount_ == 0)
						{
							tree->count_ = tree->childsNodes_[child]->count_;
							tree->CutSubTree(child);
							break;
						}
					}
				}
			}

			// Przycinanie drzewa
			// - weightedSpecificityRatio - okre�la jak specyficzne musz� by� tagi by ich nie przycina�.
			void Pruning(EndingTree::EndingTreeNode* tree, double weightedSpecificityRatio, std::vector<EndingTagSet*>& tagSetsList)
			{
				// Zako�czenie rekurencji.
				if(tree->childsCount_ == 0)
					return;

				// Najperw nastepuje rekurencja w li�ciach id�c w stron� korzenia.
				for(int child = 0; child < tree->childsCount_; child++)
				{
					Pruning(tree->childsNodes_[child],weightedSpecificityRatio, tagSetsList);
				}

				// Je�li dla w�z�a jest przypisana ko�c�wka.
				if(tree->count_ != 0)
				{
					int parentWeightedCount = 0;
					// Tablica m�wi�ca, czy mo�emy przycina� ga��zie.
					bool* canBranchBeMerged = new bool[tree->childsCount_];
					// Wyci�gamy tagSet ko�c�wki drzewa.
					EndingTagSet* treeTagSet = tagSetsList[tree->count_-1];
					// Zliczamy wa�on� sum� tag�w ojca.
					for(UINTEGER i = 0; i < treeTagSet->count_; i++)
					{
						parentWeightedCount += treeTagSet->tagsCounts_[i]->count;
					}

					// Dla ka�dego dziecka.
					for(UINTEGER j = 0; j < tree->childsCount_; j++)
					{
						// Je�li dziecko nie posiada przypisanej ko�c�wki to porzucamy ide� wyci�cia go.
						if(tree->childsNodes_[j]->count_ == 0)
						{
							canBranchBeMerged[j] = false;
							continue;
						}

						// Wagi dziecka.
						int childWeightedCount = 0;
						// Wagi wsp�lne.
						int bothWeightedCount = 0;
						// TagSet ko�c�wki dziecka.
						EndingTagSet* childTagSet = tagSetsList[tree->childsNodes_[j]->count_-1];
						// Dla ka�dego tagu ko�c�wki dziecka.
						// liczymy ile jest wyst�pie� wsp�lnych tag�wu rodzica i bie��cego dziecka
						for(UINTEGER k = 0; k < childTagSet->count_; k++)
						{
							// Dodajemy do sumy liczno�ci tag�w dziecka.
							childWeightedCount += childTagSet->tagsCounts_[k]->count;
							// Dla ka�dego tagu ko�c�wki rodzica.
							for(UINTEGER l = 0; l < treeTagSet->count_; l++)
							{
								// Je�li tagi s� takie same to zwi�kszamy sum� wsp�lnej cz�ci.
								if(childTagSet->tagsCounts_[k]->flexClass == treeTagSet->tagsCounts_[l]->flexClass
									&& childTagSet->tagsCounts_[k]->gramCat == treeTagSet->tagsCounts_[l]->gramCat)
								{
									bothWeightedCount += childTagSet->tagsCounts_[k]->count;
									bothWeightedCount += treeTagSet->tagsCounts_[l]->count;
								}
							}
						}

						// Obliczamy specyficzno�� danego dziecka.
						double probability = 1.0 - ((double)bothWeightedCount/((double)parentWeightedCount + (double)childWeightedCount));
						if(probability < weightedSpecificityRatio)
							canBranchBeMerged[j] = true;
						else
							canBranchBeMerged[j] = false;
					} // to by�o dla k. dziecka

					std::vector<char> subTrees;
					for(int child = 0; child < tree->childsCount_; child++)
					{
						if(canBranchBeMerged[child])
						{
							subTrees.push_back(tree->childMapChars_[child]);
							EndingTagSet* childTagSet = tagSetsList[tree->childsNodes_[child]->count_-1];
							for(UINTEGER childTagNum = 0; childTagNum < childTagSet->count_; childTagNum++)
							{
								treeTagSet->AddTag(childTagSet->tagsCounts_[childTagNum]);
							}
						}
					}
					for(int i = 0; i < (int)subTrees.size(); i++)
					{
						tree->CutSubTree(subTrees[i]);
					}
				}
			}

			// Przycinanie drzewa (metoda druga)
			// - weightedSpecificityRatio - okre�la jak specyficzne musz� by� tagi by ich nie przycina�.
			void Pruning2(EndingTree::EndingTreeNode* tree, double weightedSpecificityRatio, std::vector<EndingTagSet*>& tagSetsList)
			{
				// Zako�czenie rekurencji.
				if(tree->childsCount_ == 0)
					return;

				// Najperw nastepuje rekurencja w li�ciach id�c w stron� korzenia.
				for(int child = 0; child < tree->childsCount_; child++)
				{
					Pruning2(tree->childsNodes_[child],weightedSpecificityRatio, tagSetsList);
				}

				// Je�li dla w�z�a jest przypisana ko�c�wka.
				if(tree->count_ != 0)
				{
					// Tablica m�wi�ca, czy mo�emy przycina� ga��zie.
					bool *canBranchBeMerged = new bool[tree->childsCount_];
					// Wyci�gamy tagSet ko�c�wki drzewa.
					EndingTagSet* treeTagSet = tagSetsList[tree->count_-1];

					// Dla ka�dego dziecka.
					for(int j = 0; j < tree->childsCount_; j++)
					{
						// Je�li dziecko nie posiada przypisanej ko�c�wki, to porzucamy ide� wyci�cia go.
						if(tree->childsNodes_[j]->count_ == 0)
						{
							canBranchBeMerged[j] = false;
							continue;
						}

						// Wagi dziecka.
						int childWeightedCount = 0;
						// Wagi wsp�lne.
						int bothWeightedCount = 0;
						// TagSet ko�c�wki dziecka.
						EndingTagSet* childTagSet = tagSetsList[tree->childsNodes_[j]->count_-1];
						// Dla ka�dego tagu ko�c�wki dziecka.
						for(UINTEGER k = 0; k < childTagSet->count_; k++)
						{
							// Dodajemy do sumy liczno�ci tag�w dziecka.
							childWeightedCount += childTagSet->tagsCounts_[k]->count;
							// Dla ka�dego tagu ko�c�wki rodzica.
							for(UINTEGER l = 0; l < treeTagSet->count_; l++)
							{
								// Je�li tagi s� takie same to zwi�kszamy sum� wsp�lnej cz�ci.
								if(childTagSet->tagsCounts_[k]->flexClass == treeTagSet->tagsCounts_[l]->flexClass
									&& childTagSet->tagsCounts_[k]->gramCat == treeTagSet->tagsCounts_[l]->gramCat)
								{
									bothWeightedCount += childTagSet->tagsCounts_[k]->count;
									//bothWeightedCount += treeTagSet->tagsCounts_[l]->count;
								}
							}
						}

						// Obliczamy specyficzno�� danego dziecka.
						double probability = 1.0 - ((double)bothWeightedCount/(double)childWeightedCount);
						if(probability < weightedSpecificityRatio)
							canBranchBeMerged[j] = true;
						else
							canBranchBeMerged[j] = false;
					}

					std::vector<char> subTrees;
					for(int child = 0; child < tree->childsCount_; child++)
					{
						if(canBranchBeMerged[child])
						{
							subTrees.push_back(tree->childMapChars_[child]);
							EndingTagSet* childTagSet = tagSetsList[tree->childsNodes_[child]->count_-1];
							for(UINTEGER childTagNum = 0; childTagNum < childTagSet->count_; childTagNum++)
							{
								treeTagSet->AddTag(childTagSet->tagsCounts_[childTagNum]);
							}
						}
					}
					for(int i = 0; i < (int)subTrees.size(); i++)
					{
						tree->CutSubTree(subTrees[i]);
					}

					delete [] canBranchBeMerged;
				}
			} // void Pruning2(EndingTree::EndingTreeNode* tree...
			
			
		} // namespace EndingTagSet
	} // namespace GuessingSystem
} // namespace Corpus
