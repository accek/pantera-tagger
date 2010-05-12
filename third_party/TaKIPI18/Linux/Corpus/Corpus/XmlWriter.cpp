#include "../corpuslib_headers.h"
#include "XmlWriter.h"

namespace Corpus
{

	CXmlWriter::CXmlWriter(std::ostream& xmlFile)
		:xmlFile_(xmlFile), randstate(7)
	{
	}

	CXmlWriter::~CXmlWriter(void)
	{
	}

	void CXmlWriter::WriteSentence(CSentence *sentence, std::string& befTextTags, bool eof)
	{
		held.clear(); // just in case
		
		std::stringstream ss;
		UBYTE lastvarnum = 0;

		if(!befTextTags.empty())
		{
			xmlFile_ << befTextTags;
		}

		if(sentence == 0)
		{
			if(!eof)
				xmlFile_ << "</chunk>" << std::endl;
			return;
		}

		xmlFile_ << "<chunk type=\"s\">" << std::endl;

		for(int i = 0; i < sentence->Size(); i++)
		{
			Corpus::CToken *currTok = sentence->operator [](i);
			if(currTok->variantNum == 0)
			{
				Resolve(); // if held non-empty, resolves and clears it
				xmlFile_ << currTok->ToXmlString(); // current token is unambiguous
			}
			else if(currTok->variantNum < lastvarnum) // actually variantNum == 1
			{
				// first variant of another ambiguity => flush that one
				Resolve();
				// ...and hold next
				held.push_back(currTok);
			}
			else
			{
				// some ambiguity => just hold it
				held.push_back(currTok);
			}
			
			lastvarnum = currTok->variantNum;
		} // foreach token in sentence
		// resolve remaining tokens (if any)
		Resolve();

		xmlFile_ << "</chunk>" << std::endl;
	} // CXmlWriter::WriteSentence
	
	void CXmlWriter::Resolve()
	{
		// create once, then reuse it (a vector of variants, each being a deque of tokens)
		static std::vector< tokque_ptr > variants;
		variants.clear(); // just in case
		
		if(!held.empty())
		{
			Corpus::CToken *lastTok = held.front();
			held.pop_front();
			
			tokque_ptr currQue(new TOK_QUE());
			currQue->push_back(lastTok);
			
			while(!held.empty())
			{
				Corpus::CToken *currTok = held.front();
				held.pop_front();
				
				if(currTok->variantNum == lastTok->variantNum)
				{
					currQue->push_back(currTok);
				}
				else
				{
					variants.push_back(currQue);
					currQue = tokque_ptr(new TOK_QUE());
					currQue->push_back(currTok);
				}
				lastTok = currTok;
			}
			variants.push_back(currQue);
			
			// TODO heuristics desired, now just a pseudo-random (predictable) choice among variants
			int best_num = RandomR() % variants.size();
			
			/* find variant maximising heuristic rating
			double best_rate = RateVariant(variants[best_num]);
			for(int vnum = 1; vnum < (int)variants.size(); ++vnum)
			{
				double new_rate = RateVariant(variants[vnum]);
				if(new_rate > best_rate)
				{
					best_rate = new_rate;
					best_num = vnum;
				}
			}
			*/
			
			// feed the variant to out
			tokque_ptr best_variant(variants[best_num]);
			for(TOK_QUE::iterator where = best_variant->begin(); where != best_variant->end(); ++where)
			{
				xmlFile_ << (*where)->ToXmlString();
			}
		}
		
	} // CXmlWriter::Resolve()
	
	unsigned int CXmlWriter::RandomR()
	{
		// RAND_MAX assumed to be 32767
		randstate = randstate * 1103515245 + 12345;
		return((unsigned int)(randstate/65536) % 32768);
	}
	
	double CXmlWriter::RateVariant(tokque_ptr variant)
	{
		// TODO variant rating
		/*double rate = (variant->front())->GetBestDisambProbability();
		for(TOK_QUE::iterator where = variant->begin(); where != variant->end(); ++where)
		{
			double currentProb = (*where)->GetBestDisambProbability();
			if(currentProb > rate)
				rate = currentProb;
		}
		return rate;
		*/
		return (double)-variant->size();
	}

}
