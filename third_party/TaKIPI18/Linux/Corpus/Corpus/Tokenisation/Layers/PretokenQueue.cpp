#include "../../../corpuslib_headers.h"
#include "PretokenQueue.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{

			void CPretokenQueue::PushPart(const std::string cp_orth, TFlexClass flex, TCategories attrs)
			{
				boost::shared_ptr<SPretoken> pretoken(new SPretoken(cp_orth, nsNow));
				pretoken->flexClass = flex;
				pretoken->attrValues = attrs;
				pretokens.push(pretoken);
				nsNow = true;
			}
			
			boost::shared_ptr<SPretoken> CPretokenQueue::Pop()
			{
				if(pretokens.empty())
				{
					boost::shared_ptr<SPretoken> returningNull;
					return returningNull;
				}
				boost::shared_ptr<SPretoken> returning(pretokens.front());
				pretokens.pop();
				return returning;
			}

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus
