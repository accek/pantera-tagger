
#include "../../takipi_headers.h"
#include "Filter.h"

namespace TaKIPI
{
	namespace Learning
	{

		CFilter::CFilter(void)
		{
		}

		CFilter::~CFilter(void)
		{
		}

		STATIC void CFilter::Filter(Corpus::CSentence* in, Corpus::CSentence* out, TaKIPI::CLayers& layers, bool previousLayer)
		{
			using namespace Corpus;
			if( !in->FirstPosition() )
				return;
			out->FirstPosition();

			do
			{
				CToken* tokIn = in->GetCurrentToken();
				CToken* tokOut = out->GetCurrentToken();
				tokOut->ClearLexems();

				if ((int)tokIn->GetLexCount() == 1)
				{
					tokOut->AddLexem(tokIn->GetLexem(0));
					out->NextPosition();
					continue;
				}

				CValue mask;
				CValue maskedToken;
				CValue maskedValue;

				if(previousLayer)
					layers.GetLayerMask(mask,true);
				else
					layers.GetLayerMask(mask);

				maskedToken = (*tokIn)[mask];
				tokIn->DisambLexem().GetMaskedValue(mask,maskedValue);

				tokOut->ClearLexems();
				if( maskedToken.first == maskedValue.first
					&& maskedToken.second == maskedValue.second)
				{
					for(int i = 0; i < (int)tokIn->GetLexCount(); i++)
					{
						tokOut->AddLexem(tokIn->GetLexem(i));
					}
				}
				else
				{
					CValue currentMaskedValue;
					for(int i = 0; i < (int)tokIn->GetLexCount(); i++)
					{
						(*tokIn->GetLexem(i)).GetMaskedValue(mask,currentMaskedValue);
						if(currentMaskedValue.first == maskedValue.first
							&& currentMaskedValue.second == maskedValue.second)
							tokOut->AddLexem(tokIn->GetLexem(i));
					}
				}
				out->NextPosition();
			}
			while(in->NextPosition());
		}

		STATIC bool CFilter::IsAmb(Corpus::CSentence& sentence, TaKIPI::CLayers& layers)
		{
			using namespace Corpus;
			int current = layers.GetCurrentAttribute();
			CValue mask;
			CValue tokenValue;

			if(!layers.GotoFirst())
				throw tagexception("There is no attributes in layer.");

			do
			{
				CTag::GetAttribMask(layers.GetCurrentAttribute(),mask);
				tokenValue = (*sentence.GetCurrentToken())[mask];
				if(layers.GetCurrentAttribute() == CToken::flex)
				{
					if(tokenValue.first == (cf_subst|cf_ger))
						return false;
					else if(Parser::BitCount(tokenValue.first) > 1)
						return true;
				}
				else
				{
					if(Parser::BitCount(tokenValue.second) > 1)
						return true;
				}
			}
			while(layers.GotoNext());

			layers.GotoFirst();

			while(layers.GetCurrentAttribute() != current)
				layers.GotoNext();

			return false;
		}

	}//namespace Learning 
}//	namespace TaKIPI

