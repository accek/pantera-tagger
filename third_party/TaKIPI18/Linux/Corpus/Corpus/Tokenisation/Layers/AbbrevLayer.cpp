#include "../../../corpuslib_headers.h"
#include "AbbrevLayer.h"
#include "../../ResourceManager.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			boost::shared_ptr<SPretoken> CAbbrevLayer::GetNext()
			{
				if(queue.empty())
				{
					// fetch next
					boost::shared_ptr<SPretoken> current(lower->GetNext());
					if(!current || (current->flexClass != cf_tsym
									   && current->flexClass != cf_turi))
					{ // NULL (no more tokens): pass it to higher layers and exit
						return current;
					}

					// if it is an URI but not on the SymAbbrev list we leave it unchanged
					if(current->flexClass == cf_turi)
					{
						std::string withDot = current->cp_orth + ".";
						if(!CResourceManager::GetInstance()->GetSentencerCore()->
											  IsSymAbbrev(withDot))
							return current;
					}

					// we have a symbol or URI, examine next tokens
					boost::shared_ptr<SPretoken> second(lower->GetNext());
					if(second && second->wasNS
									  && (second->cp_orth.compare(".") == 0))
					{
						// the symbol is followed by an NS-ed dot
						// check the third one, if it is not an NS-ed dot, we
						// combine the first with the second
						boost::shared_ptr<SPretoken> third(lower->GetNext());
						if(!third || !third->wasNS
											  || (third->cp_orth.compare(".") != 0))
						{
							// combine
							boost::shared_ptr<SPretoken> combined(
									new SPretoken(current->cp_orth + second->cp_orth, current->wasNS));
							combined->flexClass = cf_tsym; // symbol (abbrev actually)
							queue.push(combined);
							queue.push(third); // NULL should be OK as well, it will be passed up
						}
						else
						{
							// just pass them all as they are
							queue.push(current);
							queue.push(second);
							queue.push(third);
						}
					}
					else
					{
						// pass them both as they are (there is no third one)
						queue.push(current);
						queue.push(second);
					}
				}
				boost::shared_ptr<SPretoken> returning = queue.front();
				queue.pop();
				return returning;
			} // GetNext
		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus
