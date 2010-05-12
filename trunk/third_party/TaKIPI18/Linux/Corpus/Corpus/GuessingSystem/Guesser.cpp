#include "../../corpuslib_headers.h"
#include "Guesser.h"
#include "FrequencyList/FrequencyList.h"
#include "GuesserTest/TreeHistogram.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		// konstruktor ucz¹cy guesser
		Guesser::Guesser(const char* formsListFile)
		{
			std::map<std::string,int> list;
			GuessingSystem::FrequencyList::LoadList(list,formsListFile,10000);
			tree_ = GuessingSystem::EndingTree::CreateEndingTree(list,10000);
			std::cerr << "Przycinanie drzewa." << std::endl;
			tree_->CutOrphans();
			std::cerr << "Ukonczono przycinanie drzewa." << std::endl;
			GuessingSystem::EndingTagSet::CreateEndingTagSetsList(tree_,list,outList_,10000);
		}

		// konstruktor deserializuj¹cy przygotowane dane
		Guesser::Guesser(const char* serializedEndingTreeFile, const char* serializedEndingTagSetVectorFile)
		{
			std::ifstream endingTreeFile(serializedEndingTreeFile, std::ios::binary);
			std::ifstream endingTagSetVectorFile(serializedEndingTagSetVectorFile, std::ios::binary);

			if(!endingTreeFile)
				throw tagexception("Guesser: cannot open ETree");
			if(!endingTagSetVectorFile)
				throw tagexception("Guesser: cannot open ETS");

			std::cerr << "Rozpoczecie deserializacji drzewa identyfikacji koncowek." << std::endl;
			this->tree_ = GuessingSystem::EndingTree::Deserialize(endingTreeFile);
			if(this->tree_ == NULL) std::cerr << "Blad odczytu drzewa..." << std::endl;
			else std::cerr << "Zakonczenie deserializacji drzewa identyfikacji koncowek." << std::endl;
			std::cerr << "Rozpoczecie deserializacji wektora tagsetow koncowek." << std::endl;
			GuessingSystem::EndingTagSet::Deserialize(endingTagSetVectorFile,this->outList_);
			std::cerr << "Zakonczenie deserializacji wektora tagsetow koncowek." << std::endl;

			endingTreeFile.close();
			endingTagSetVectorFile.close();
		}

		Guesser::~Guesser()
		{
			tree_->Free();
			delete tree_;
			for(int i = 0; i < (int)outList_.size(); i++)
			{
				GuessingSystem::EndingTagSet::EndingTagSet *ets = outList_[i];
				delete ets;
			}
		}

		void Guesser::Serialize(const char* serializedEndingTreeFile, const char* serializedEndingTagSetVectorFile)
		{
			std::ofstream endingTreeFile(serializedEndingTreeFile, std::ios::binary);
			std::ofstream endingTagSetVectorFile(serializedEndingTagSetVectorFile, std::ios::binary);

			std::cerr << "Rozpoczecie serializacji drzewa identyfikacji koncowek." << std::endl;
			this->tree_->Serialize(endingTreeFile);
			std::cerr << "Zakonczenie serializacji drzewa identyfikacji koncowek." << std::endl;
			std::cerr << "Rozpoczecie serializacji wektora tagsetow koncowek." << std::endl;
			GuessingSystem::EndingTagSet::Serialize(endingTagSetVectorFile,this->outList_);
			std::cerr << "Zakonczenie serializacji wektora tagsetow koncowek." << std::endl;

			endingTreeFile.close();
			endingTagSetVectorFile.close();
		}

		CToken* Guesser::Guess(const char* form, bool takeNodeBeforeIgn)
		{
			return GuessingSystem::EndingTagSet::GuessForm(form,tree_,outList_,takeNodeBeforeIgn);
		}

		void Guesser::AttachLeafToIgn()
		{
			GuessingSystem::EndingTagSet::AttachLeafToIgn(this->tree_);
		}

		// Przycinanie drzewa
		void Guesser::Pruning(double weightedSpecificityRatio)
		{
			GuessingSystem::EndingTagSet::Pruning(this->tree_,weightedSpecificityRatio,this->outList_);
		}

		// Przycinanie drzewa
		void Guesser::Pruning2(double weightedSpecificityRatio)
		{
			GuessingSystem::EndingTagSet::Pruning2(this->tree_,weightedSpecificityRatio,this->outList_);
		}

		void Guesser::DumpTreeStats(std::ostream &out)
		{
			out << "Statystyki dlugosci sciezek\n\n";
			GuesserTest::TreeHistogram hist(tree_, &outList_);
			hist.Update();
			hist.Dump(out);
		}

		void Guesser::DumpTreePaths(std::ostream &out)
		{
			GuesserTest::TreeHistogram hist(tree_, &outList_);
			//hist.Update();
			hist.GetAll(out);
		}
	}
}
