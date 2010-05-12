#include "../../../corpuslib_headers.h"
#include "CombineLayer.h"
#include "../../ResourceManager.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			boost::shared_ptr<SPretoken> CCombineLayer::Fetch()
			{
				if(queue.empty())
				{
					return lower->GetNext();
				}
				else
				{
					boost::shared_ptr<SPretoken> returning = queue.front();
					queue.pop();
					return returning;
				}
			}
			
			boost::shared_ptr<SPretoken> CCombineLayer::GetNext()
			{
				// fetch next
				boost::shared_ptr<SPretoken> basetok(Fetch());
				
				if(basetok && basetok->flexClass == cf_interp && basetok->cp_orth.compare("-") == 0)
				{
					while(true)
					{
						boost::shared_ptr<SPretoken> second(Fetch());
						if(second && second->flexClass == cf_interp && second->wasNS && second->cp_orth.compare("-") == 0)
						{
							// join second with basetok into basetok
							basetok->cp_orth.append(second->cp_orth);
						}
						else
						{
							// leave second for another run, return basetok
							queue.push(second);
							return basetok;
						}
					}
				}
				else // nothing interesting, flush as it is
				{
					return basetok;
				}
			} // GetNext
		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus
