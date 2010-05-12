#include "../../corpuslib_headers.h"
#include "Tokeniser.h"
#include "Layers/InputLayer.h"
#include "Layers/ClassifyLayer.h"
#include "Layers/PreSplitLayer.h"
#include "Layers/SplitLayer.h"
#include "Layers/AbbrevLayer.h"
#include "Layers/CombineLayer.h"
#include "../ResourceManager.h"

namespace Corpus
{
	namespace Tokenisation
	{
		CTokeniser::CTokeniser(std::istream &co_inStream, bool useMorpher, bool reencode, bool noNewTags)
			:CTokenReader(co_inStream), useMorfeusz(useMorpher), useReencoding(reencode), outputEncoding(CConfig::inputEncoding)
		{
			// setup layers
			if(noNewTags)
			{ // CLASSIFY layer introduces new tags, so we don't use it
				boost::shared_ptr<CTokLayer> layer1(new Layers::CInputLayer(co_input));
				boost::shared_ptr<CTokLayer> layer2(new Layers::CPreSplitLayer(layer1));
				boost::shared_ptr<CTokLayer> layer3(new Layers::CSplitLayer(layer2));
				boost::shared_ptr<CTokLayer> layer4(new Layers::CAbbrevLayer(layer3));
				boost::shared_ptr<CTokLayer> layer5(new Layers::CCombineLayer(layer4));

				upperLayer = layer5;
			}
			else
			{
				boost::shared_ptr<CTokLayer> layer1(new Layers::CInputLayer(co_input));
				boost::shared_ptr<CTokLayer> layer2(new Layers::CPreSplitLayer(layer1));
				boost::shared_ptr<CTokLayer> layer3(new Layers::CSplitLayer(layer2));
				boost::shared_ptr<CTokLayer> layer4(new Layers::CClassifyLayer(layer3));
				boost::shared_ptr<CTokLayer> layer5(new Layers::CAbbrevLayer(layer4));
				boost::shared_ptr<CTokLayer> layer6(new Layers::CCombineLayer(layer5));

				upperLayer = layer6;
			}
		}

		CTokeniser::~CTokeniser()
		{
		}

		CToken *CTokeniser::GetNext()
		{
			/* It may happen that the pretoken being fetched consist of unicode
			characters that all evaluate to whitespaces; Morfeusz would leave
			none of them in the outputTokens queue. Therefore we need to fetch
			pretokens UNTIL we get a non-empty sequence or the end of input.
			
			An alternative behaviour could assume leaving those `weird blanks'
			as tsym tokens containing unicode characters, the rationale being
			that most probably they are included in the source text as
			characters being explicitly referred to (for instance in a unicode
			reference paper or a discussion on character encoding problems).
			Yet it seems incoherent and unsafe for further levels of processing
			to have separate kinds of blanks treated differently.
			The implemented tokenisation policy assumes ANY WHITESPACE to be
			a certain token delimiter. */
			while(outputTokens.empty())
			{
				boost::shared_ptr<SPretoken> pretoken = upperLayer->GetNext();
				// no more tokens
				if(!pretoken)
				{
					return NULL;
				}
				FeedToQueue(*pretoken);
			}
			
			CToken *outputToken = outputTokens.front();
			outputTokens.pop();
			if(useReencoding) outputToken->ConvertEncoding(Parser::cp1250, outputEncoding);
			return outputToken;
		}
		
		void CTokeniser::FeedToQueue(SPretoken &pretoken)
		{
			if(useMorfeusz && (pretoken.flexClass & mask_FlexNeedsMorphAnalysis))
			{ // have it analysed (with morfeusz/guesser)
				std::vector<CToken *> morphedTokens;
				Corpus::CResourceManager::GetInstance()->GetFormsIdentifier()->
						IdentifyNoConv(pretoken.cp_orth, morphedTokens);
				
				// if there is segmentation ambiguity, need to remember NS at first variant, the rest will be restored to this value
				bool noSpaceAtFirstVariant = false;
				UBYTE lastvarnum = 0;
				
				for(size_t indx = 0; indx < morphedTokens.size(); indx++)
				{
					CToken *tok = morphedTokens[indx];
					if(indx == 0) tok->NS = pretoken.wasNS;
					else tok->NS = true;
					// maybe store or restore tok->NS (consider only variant-initial tokens)
					if(tok->variantNum != lastvarnum)
					{
						if(tok->variantNum == 1) noSpaceAtFirstVariant = tok->NS;
						else if(tok->variantNum > 1) tok->NS = noSpaceAtFirstVariant;
					}
					lastvarnum = tok->variantNum;
					// feed it anyway
					outputTokens.push(tok);
				}
			}
			else
			{ // just put pretoken's class and attr values
				CToken *tok = CToken::CreateBlank(pretoken.cp_orth);
				tok->NS = pretoken.wasNS;
				tok->ClearLexems();
				CLexem *lex = new CLexem();
				lex->SeBase(pretoken.cp_orth);
				CTag tag;
				tag.FlexClass = pretoken.flexClass;
				tag.Categories = pretoken.attrValues;
				lex->SetTag(tag);
				tok->AddLexem(lex);
				outputTokens.push(tok);
			}
		}

		void CTokeniser::ReadAndDump()
		{
			CToken *tok;
			while((tok = GetNext()) != NULL)
			{
				std::cout << tok->ToXmlString();
				delete tok;
			}
		}
		
		void CTokeniser::SetOutputEncoding(Parser::Encoding outputEnc)
		{
			useReencoding = true;
			outputEncoding = outputEnc;
		}
		
		void CTokeniser::DisableReencoding()
		{
			useReencoding = false;
		}
	} // namespace Tokenisation
} // namespace Corpus
