#include "../../../corpuslib_headers.h"
#include "PreSplitLayer.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			boost::shared_ptr<SPretoken> CPreSplitLayer::GetNext()
			{
				if(queue.IsEmpty())
				{
					// fetch next
					boost::shared_ptr<SPretoken> current(lower->GetNext());

					if(!current)
					{ // NULL (no more tokens): pass it to higher layers and exit
						return current;
					}

					// probably there was a space
					queue.NewWhole(current->wasNS);
					// split the rest assuming no space
					CCore *core = GetCore();
					const char *cp_orth = current->cp_orth.c_str();
					int pos = 0, lastcharpos = 0, len = (int)current->cp_orth.size();
					for(; pos < len; pos++)
					{
						if(core->IsAlwaysSep(cp_orth[pos]))
						{
							if(lastcharpos < pos)
							{
								// flush the previous fragment as ign
								std::string cp_preSepOrth(cp_orth, lastcharpos, pos - lastcharpos);
								queue.PushPart(cp_preSepOrth, cf_ign, gc_none);
							}
							// flush the current sep as an interp
							std::string cp_sepOrth(cp_orth, pos, 1);
							queue.PushPart(cp_sepOrth, cf_interp, gc_none);
							// remember the next position
							lastcharpos = pos + 1;
						}
					}
					// flush the remainder as ign
					if(lastcharpos < pos)
					{
						std::string cp_postSepOrth(cp_orth, lastcharpos, pos - lastcharpos);
						queue.PushPart(cp_postSepOrth, cf_ign, gc_none);
					}
				}

				return queue.Pop();
			} // GetNext

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus
