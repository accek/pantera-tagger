#include "../../../corpuslib_headers.h"
#include "EndingTreeNode.h"
namespace Corpus
{
	namespace GuessingSystem
	{
		namespace EndingTree
		{
			// Dodaje formę do drzewa.
			void EndingTreeNode::AddForm(const char* form)
			{
				this->count_++;

				if(form == 0)
					return;

				if(strlen(form) == 0)
				{
					this->flag_ = true;
					return;
				}

				EndingTreeNode *childNode = this->GetSubTree(form[0]);

				if(childNode == NULL)
				{
					this->AddSubTree(form[0]);
					childNode = this->childsNodes_[this->childsCount_-1];
				}

				childNode->AddForm(form+1);
			}

			// Dodaje poddrzewo (puste) odpowiadające podanej literze.
			void EndingTreeNode::AddSubTree(char character)
			{
				EndingTreeNode** newChildNodes = new EndingTreeNode*[this->childsCount_+1];
				newChildNodes[this->childsCount_] = new EndingTreeNode();
				newChildNodes[this->childsCount_]->childsCount_ = 0;
				newChildNodes[this->childsCount_]->count_ = 0;

				char* newMapChars = new char[this->childsCount_+1];
				newMapChars[this->childsCount_] = character;

				if(this->childsCount_ != 0)
				{
					memcpy(newChildNodes, this->childsNodes_, this->childsCount_ * sizeof(EndingTreeNode*));
					memcpy(newMapChars, this->childMapChars_, this->childsCount_ * sizeof(char));
					delete[]this->childMapChars_;
					delete[]this->childsNodes_;
				}

				this->childMapChars_ = newMapChars;
				this->childsNodes_ = newChildNodes;
				this->childsCount_++;
			}

			// Usuwa poddrzewo.
			void EndingTreeNode::CutSubTree(char character)
			{
				INTEGER childNumber = -1;

				for(UBYTE i = 0; i < this->childsCount_; i++)
				{
					if(this->childMapChars_[i] == character)
					{
						childNumber = i;
						break;
					}
				}

				if(childNumber != -1)
				{
					CutSubTree(childNumber);
				}
			}

			// Usuwa poddrzewo.
			void EndingTreeNode::CutSubTree(INTEGER childNumber)
			{
				EndingTreeNode** newChildNodes = new EndingTreeNode*[this->childsCount_-1];
				char *newMapChars = new char[this->childsCount_-1];

				memcpy(newChildNodes, this->childsNodes_, childNumber * sizeof(EndingTreeNode*));
				memcpy(newMapChars, this->childMapChars_, childNumber * sizeof(char));
				memcpy(newChildNodes + childNumber, this->childsNodes_ + childNumber + 1, (this->childsCount_ - childNumber - 1) * sizeof(EndingTreeNode*));
				memcpy(newMapChars + childNumber, this->childMapChars_ + childNumber + 1, (this->childsCount_ - childNumber - 1) * sizeof(char));

				this->childsNodes_[childNumber]->Free();
				delete this->childsNodes_[childNumber];
				delete[]this->childMapChars_;
				delete[]this->childsNodes_;
				this->childMapChars_ = newMapChars;
				this->childsNodes_ = newChildNodes;
				this->childsCount_--;
			}

			// Zwraca poddrzewo odpowiadające podanej literze, lub NULL gdy nie istnieje takowe.
			EndingTreeNode* EndingTreeNode::GetSubTree(char character)
			{
				for(UBYTE i = 0; i < this->childsCount_; i++)
				{
					if(this->childMapChars_[i] == character)
						return this->childsNodes_[i];
				}

				return NULL;
			}

			// Zwalnia pamięć (czyści drzewo).
			void EndingTreeNode::Free()
			{
				for(UBYTE i = 0; i < this->childsCount_; i++)
				{
					this->childsNodes_[i]->Free();
					delete this->childsNodes_[i];
				}

				if(this->childsNodes_ != NULL)
				{
					delete[]this->childsNodes_;
					delete[]this->childMapChars_;
				}
			}

			// Sprawdza czy dana podgałąź może być wycięta (jest "sierotą").
			bool EndingTreeNode::CanBeCut()
			{
				if(this->childsCount_ == 0)
					return true;

				return (this->childsCount_ == 1) && this->childsNodes_[0]->CanBeCut();
			}

			// Wycina "sieroty", czyli gałęzie bez rozgałęzień.
			void EndingTreeNode::CutOrphans()
			{
				for(UBYTE i = 0; i < this->childsCount_; i++)
				{
					if(this->childsNodes_[i]->count_ == 1)
					{
						this->CutSubTree((INTEGER)i);
					}
					else
					{
						this->childsNodes_[i]->CutOrphans();
					}
				}
			}

			// Czyści liczności (będą wykorzystane jako identyfikator)
			void EndingTreeNode::ClearCount()
			{
				this->count_ = 0;

				for(UBYTE i = 0; i < this->childsCount_; i++)
				{
					this->childsNodes_[i]->ClearCount();
				}
			}

			// Znajduje najdłuższą końcówkę dla danej formy.
			std::string EndingTreeNode::FindLongestEnding(const char *form)
			{
				if(form == 0)
					return "";

				if(strlen(form) == 0)
				{
					return "";
				}

				EndingTreeNode* subTree = this->GetSubTree(form[0]);

				if(subTree == NULL)
					return "";
				else
					return subTree->FindLongestEnding(form+1) + form[0];
			}

			// Zwraca referencję do węzła o najdłuższej pasującej końcówce.
			EndingTreeNode* EndingTreeNode::FindLongestEndingRef(const char* form, bool takeNodeBefIgn)
			{
				if(form == 0)
					return this;

				if(strlen(form) == 0)
				{
					return this;
				}

				EndingTreeNode* subTree = this->GetSubTree(form[0]);

				if(subTree == NULL)
				{
					if(takeNodeBefIgn && this->count_ == 0)
						return 0;
					return this;
				}
				else
				{
					EndingTreeNode* subTreeTemp = subTree->FindLongestEndingRef(form+1, takeNodeBefIgn);
					if(subTreeTemp != 0)
					{
						return subTreeTemp;
					}
					else if(takeNodeBefIgn && this->count_ == 0)
					{
						return 0;
					}
					else
						return this;
				}
			}

			// Serializacja
			void EndingTreeNode::Serialize(std::ofstream &file)
			{
				file.write((char *)&this->flag_, sizeof(USHORT));
				file.write((char *)&this->count_, sizeof(UINTEGER));
				file.write((char *)&this->childsCount_, sizeof(UBYTE));

				if(this->childsCount_ != 0)
					file.write((char*)this->childMapChars_,this->childsCount_ * sizeof(char));
				for(UBYTE i = 0; i < this->childsCount_; i++)
				{
					this->childsNodes_[i]->Serialize(file);
				}
			}

			// Deserializacja
			EndingTreeNode* Deserialize(std::ifstream &file)
			{
				EndingTreeNode* node = new EndingTreeNode();
				
				file.read((char *)&node->flag_, sizeof(USHORT));
				file.read((char *)&node->count_, sizeof(UINTEGER));
				file.read((char *)&node->childsCount_, sizeof(UBYTE));

				node->childMapChars_ = new char[node->childsCount_];
				if(node->childsCount_ != 0)
					file.read((char*)node->childMapChars_,node->childsCount_ * sizeof(char));
				node->childsNodes_ = new EndingTreeNode*[node->childsCount_];
				for(int i = 0; i < node->childsCount_; i++)
				{
					node->childsNodes_[i] = Deserialize(file);
				}

				return node;
			}
		}
	}
}
