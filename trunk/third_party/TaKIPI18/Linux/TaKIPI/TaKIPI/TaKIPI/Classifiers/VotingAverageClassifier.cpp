#include "../../takipi_headers.h"
#include "VotingAverageClassifier.h"


namespace TaKIPI
{
	namespace Classifiers
	{

		CVotingAverageClassifier::CVotingAverageClassifier(CClassifier * firstClassifier, JOSKIPI::Patterns::CPattern* firstPattern)
			: CBagClassifier(firstClassifier, firstPattern)
		{
		}

		CVotingAverageClassifier::~CVotingAverageClassifier(void)
		{
		}

		Corpus::CToken* CVotingAverageClassifier::Unify(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& srcSentence, 
			TaKIPI::CLayers& layers)
		{ 
			std::vector<CClassifier *>::iterator it = baggedClassifiers->begin();
			Corpus::CToken * winningToken = srcSentence.GetCurrentToken()->CreateHardCopy();
			while(it != baggedClassifiers->end())
			{
				Corpus::CSentence * copiedSentence = srcSentence.CreateHardCopy();
				(*it)->Unify(pattern,*copiedSentence,layers);	

				Corpus::CToken * proposedToken = copiedSentence->GetCurrentToken();

				for(int j = 0; j < (int)winningToken->GetLexCount(); j++)
				{
					Corpus::CLexem * proposalLexem = proposedToken->GetLexem(j);
					winningToken->GetLexem(j)->probability += proposalLexem->probability
						- srcSentence.GetCurrentToken()->GetLexem(j)->probability;
				}

				delete copiedSentence;
				it++;
			}
			for(int j = 0; j < (int)winningToken->GetLexCount(); j++)
			{
				srcSentence.GetCurrentToken()->SetLexProbability(j,winningToken->GetLexem(j)->probability/baggedClassifiers->size());
			}

			return 0;
		}

	}// namespace Classifiers
}//namespace TaKIPI
