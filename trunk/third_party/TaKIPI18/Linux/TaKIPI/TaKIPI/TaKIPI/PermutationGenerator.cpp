#include "../takipi_headers.h"
#include "PermutationGenerator.h"

namespace TaKIPI
{

	CPermutationGenerator::	CPermutationGenerator(TaKIPI::CLayers& layers)
		:layers(layers)
	{
		las = new CLexemAscendingSort(layers);
	}

	CPermutationGenerator::~CPermutationGenerator(void)
	{
		if (las)
			delete las;
	}

	// Ustawia zdanie do permutowania.
	void CPermutationGenerator::SetSentence(Corpus::CSentence* sourceSentence, Corpus::CSentence* workingSentence)
	{
		srcSentence = sourceSentence;
		wrkSentence = workingSentence;

		//for(int i = 0; i < srcSentence->Size(); i++)
		//{
		//	if ( srcSentence->operator [](i)->GetLexCount() == (size_t)1)
		//	{
		//		(*wrkSentence).Replace((*srcSentence)[i],i);
		//		(*(*srcSentence)[i]).RefCount++;
		//	}
		//	else
		//	{
		//		(*wrkSentence)[i]->ClearLexems();
		//		for(int j = 0; j < (int)(*srcSentence)[i]->GetLexCount(); j++)
		//		{
		//			(*wrkSentence)[i]->AddLexem((*srcSentence)[i]->GetLexem(j));
		//			(*srcSentence)[i]->GetLexem(j)->RefCount++;
		//		}
		//	}
		//}
	}

	bool CPermutationGenerator::NewLayer(bool withPermutations)
	{
		// Jeœli nie nastêpnej warstwy to wychodzimy z wynikiem false.
		if(!layers.GotoNextLayer())
		{
			return false;
		}
		else
		{
			// We must update the states' vector.
			// Offset is set to the end of the lexems group.
			if(withPermutations)
			{
				// Jeœli zdanie nie ma wyrazów to wychodzimy uznawszy, ¿e zdanie jest posortowane
				if(!srcSentence->FirstPosition())
					return true;

				states.clear();

				// Dla ka¿dego Tokena w zdaniu przesortowywujemy.
				do
				{
					Sort(*srcSentence->GetCurrentToken());
					states.push_back(0);
				}
				while(srcSentence->NextPosition());
				permPos = srcSentence->Size()-1;

			}
			//wrkSentence->Goto(CPosition::nowhere);

			return true;
		}
	}

	bool CPermutationGenerator::FirstLayer(bool withPermutations)
	{
		// Jeœli nie nastêpnej warstwy to wychodzimy z wynikiem false.
		if(!layers.GotoFirstLayer())
		{
			return false;
		}
		else
		{
			// We must update the states' vector.
			// Offset is set to the end of the lexems group.
			if(withPermutations)
			{
				// Jeœli zdanie nie ma wyrazów to wychodzimy uznawszy, ¿e zdanie jest posortowane
				if(!srcSentence->FirstPosition())
					return true;

				states.clear();

				// Dla ka¿dego Tokena w zdaniu przesortowywujemy.
				do
				{
					Sort(*srcSentence->GetCurrentToken());
					states.push_back(0);
				}
				while(srcSentence->NextPosition());
				permPos = srcSentence->Size()-1;
			}

			return true;
		}
	}

	// Metoda pobiera numer tokenu i zwracaj¹ca informacjê, czy wskazany token jest
	// niejednoznaczny w odniesieniu do bie¿¹cej warstwy.
	bool CPermutationGenerator::IsAmb(int tokenNum)
	{
		Corpus::CValue mask;	
		layers.GetLayerMask(mask);

		Corpus::CValue tokenValue = (*(*srcSentence)[tokenNum])[mask];
		Corpus::CValue lexemValue;

		(*srcSentence)[tokenNum]->GetLexem(0)->GetMaskedValue(mask,lexemValue);
		if( lexemValue.first == tokenValue.first 
			&& lexemValue.second == tokenValue.second)
			return false;

		return true;
	}

	void CPermutationGenerator::Sort(Corpus::CToken& token)
	{
		CLexemAscendingSort las(layers);

		sort(token.GetLexems().begin(),token.GetLexems().end(),las);
	}

	// Moves the sentence's pointer to the next ambiguate token.
	bool CPermutationGenerator::MoveToNextAmbToken()
	{
		// We must loot through the sentence's tokens, until we'll find ambiguate one.
		do
		{
			//wrkSentence->NextPosition();
			if (!srcSentence->NextPosition())
				return false;
		}
		while(!IsAmb(srcSentence->GetPosition()));

		return true;
	}

	// Przechodzi do nastêpnej grupy lexemów bêd¹cych nastêpn¹ niejednoznacznoœci¹.
	bool CPermutationGenerator::NextLexemGroup()
	{
		Corpus::CToken* t = (*srcSentence)[permPos];
		int groupPosition = states[permPos];
		int newPosition = groupPosition;

		if (groupPosition < (int)t->GetLexCount())
		{
			for(std::vector<Corpus::CLexem*>::iterator i = t->GetLexems().begin()+groupPosition+1;
				i != t->GetLexems().end(); i++)
			{
				newPosition++;
				if ( (*las)(t->GetLexems()[groupPosition],*i))
				{
					states[permPos] = newPosition;
					return true;
				}
			}
		}

		if (permPos == 0)
			return false;

		// TODO: Metoda powinna siê przenosiæ tylko o niejednoznacznych, a nie tylko wstecz o jeden.
		permPos--;

		if (!NextLexemGroup())
			return false;

		permPos++;
		states[permPos] = 0;
		return true;
	}

	// Przechodzi do pierwszej permutacji.
	bool CPermutationGenerator::FirstPermutation()
	{
		bool temp = false;

		for(int i = srcSentence->Size()-1; i >= 0; i--)
		{
			if ( IsAmb(i) )
			{
				permPos = i;
				temp = true;
			}
		}

		for(int i = 0; i < srcSentence->Size(); i++)
		{
			(*wrkSentence)[i]->GetLexems().clear();

			for(std::vector<Corpus::CLexem*>::iterator j = (*srcSentence)[i]->GetLexems().begin()+states[i];
				j != (*srcSentence)[i]->GetLexems().end(); j++)
			{
				if ( (*las)((*srcSentence)[i]->GetLexems()[states[i]],*j) )
					continue;
				else
					(*wrkSentence)[i]->GetLexems().push_back((*srcSentence)[i]->GetLexems()[states[i]]);
			}		
		}

		return temp;
	}

	bool CPermutationGenerator::NextPermutation()
	{
		if (!NextLexemGroup())
			return false;

		for(int i = 0; i < srcSentence->Size(); i++)
		{
			(*wrkSentence)[i]->GetLexems().clear();

			for(std::vector<Corpus::CLexem*>::iterator j = (*srcSentence)[i]->GetLexems().begin()+states[i];
				j != (*srcSentence)[i]->GetLexems().end(); j++)
			{
				if ( (*las)((*srcSentence)[i]->GetLexems()[states[i]],*j) )
					continue;
				else
					(*wrkSentence)[i]->GetLexems().push_back((*srcSentence)[i]->GetLexems()[states[i]]);
			}		
		}

		return true;
	}

	void CPermutationGenerator::GetAmbClass(int tokenNum, Corpus::CValue& out)
	{
		Corpus::CValue mask;
		layers.GetLayerMask(mask);
		out = (*(*srcSentence)[tokenNum])[mask];
	}

}//namespace TaKIPI
