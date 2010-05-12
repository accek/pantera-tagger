#include "../../takipi_headers.h"
#include "VotingWTAClassifier.h"


namespace TaKIPI
{
	namespace Classifiers
	{


		CVotingWTAClassifier::CVotingWTAClassifier(CClassifier * firstClassifier, JOSKIPI::Patterns::CPattern* firstPattern)
			: CBagClassifier(firstClassifier,firstPattern)
		{
		}

		CVotingWTAClassifier::~CVotingWTAClassifier(void)
		{
		}

		Corpus::CToken* CVotingWTAClassifier::Unify(JOSKIPI::Patterns::CPattern& pattern,Corpus::CSentence& srcSentence, TaKIPI::CLayers& layers)
		{ 
			std::vector<CClassifier *>::iterator it = baggedClassifiers->begin();
			Corpus::CSentence * winningClassifications = srcSentence.CreateHardCopy();
			Corpus::CToken * winningToken = winningClassifications->GetCurrentToken();
			for(int j = 0; j < (int)winningToken->GetLexCount(); j++)
			{
				winningToken->SetLexProbability(j,-1e200);
			}
			while(it != baggedClassifiers->end())
			{
				Corpus::CSentence * copiedSentence = srcSentence.CreateHardCopy();
				(*it)->Unify(pattern,*copiedSentence,layers);

				Corpus::CToken * proposedToken = copiedSentence->GetCurrentToken();

				for(int j = 0; j < (int)winningToken->GetLexCount(); j++)
				{
					Corpus::CLexem * proposalLexem = proposedToken->GetLexem(j);
					if(winningToken->GetLexem(j)->probability < proposalLexem->probability)
					{
						winningToken->GetLexem(j)->probability = proposalLexem->probability;
					}
				}

				delete copiedSentence;
				it++;
			}
			for(int j = 0; j < (int)winningToken->GetLexCount(); j++)
			{
				srcSentence.GetCurrentToken()->SetLexProbability(j,winningToken->GetLexem(j)->probability);
			}
			delete winningClassifications;

			return 0;
		}

	}// namespace Classifiers
}//namespace TaKIPI
