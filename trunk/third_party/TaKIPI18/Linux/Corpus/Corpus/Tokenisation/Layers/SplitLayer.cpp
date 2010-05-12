#include "../../../corpuslib_headers.h"
#include "SplitLayer.h"

//#define TOKR_DEBUG

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			boost::shared_ptr<SPretoken> CSplitLayer::GetNext()
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
					const char *cp_orth = current->cp_orth.c_str();
					int bodyBeg, bodyEnd, pos;
					FindBodyBounds(cp_orth, bodyBeg, bodyEnd);
#ifdef TOKR_DEBUG
					std::cerr << "whole pretok = [" << cp_orth << "] of " << current->cp_orth.size() << "\n";
					std::cerr << "bodyBeg = " << bodyBeg << ", bodyEnd = " << bodyEnd << "\n";
#endif
					for(pos = 0; pos < bodyBeg; pos++)
					{
						// flush every char before body as an interp
						std::string cp_preBodyOrth(cp_orth, pos, 1);
						queue.PushPart(cp_preBodyOrth, cf_interp, gc_none);
#ifdef TOKR_DEBUG
						std::cerr << "pre: [" << cp_preBodyOrth << "]\n";
#endif
					}
					if(bodyBeg <= bodyEnd)
					{
						// push the body as ign (to be recognised)
						std::string cp_bodyOrth(cp_orth, bodyBeg, bodyEnd - bodyBeg + 1);
						queue.PushPart(cp_bodyOrth, cf_ign, gc_none);
#ifdef TOKR_DEBUG
						std::cerr << "body: [" << cp_bodyOrth << "] of " << (bodyEnd - bodyBeg + 1) << "\n";
#endif
					}
					if(pos < bodyEnd + 1) pos = bodyEnd + 1;
					for(; cp_orth[pos] != 0; pos++)
					{
						// flush every char after body as an interp
						std::string cp_postBodyOrth(cp_orth, pos, 1);
						queue.PushPart(cp_postBodyOrth, cf_interp, gc_none);
#ifdef TOKR_DEBUG
						std::cerr << "post: [" << cp_postBodyOrth << "]\n";
#endif
					}
				}

				return queue.Pop();
			} // GetNext
			
			void CSplitLayer::FindBodyBounds(const char *cp_orth, int &bodyBeg, int &bodyEnd)
			{
				bodyBeg = 0;
				// IsBeforeBody iff char in BegSep and char != 0
				// IsAfterBody iff char in EndSep or char == 0
				while(IsBeforeBody(cp_orth[bodyBeg]))
					bodyBeg++;
				bodyEnd = (int)strlen(cp_orth) - 1;
				while(bodyEnd >= 0 && IsAfterBody(cp_orth[bodyEnd]))
					bodyEnd--;
				/* Now bodyBeg is at the beginning of body,
				 * while bodyEnd is at the last char of body */
			}

			bool CSplitLayer::IsBeforeBody(const char ch)
			{
				return (ch != 0)
					&& GetCore()->IsBegSep(ch);
			}
			bool CSplitLayer::IsAfterBody(const char ch)
			{
				return (ch == 0)
					|| GetCore()->IsEndSep(ch);
			}

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus
