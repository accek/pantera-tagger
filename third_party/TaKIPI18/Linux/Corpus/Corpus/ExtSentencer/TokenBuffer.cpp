#include "../../corpuslib_headers.h"
#include "TokenBuffer.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		bool CTokenBuffer::Write(const std::string *cp_orth, const bool wasNS, const bool isSym)
		{
			if(items == bufflen)
				return false;

			buff[writepos].cp_orth = *cp_orth;
			buff[writepos].wasNS = wasNS;
			buff[writepos].isSym = isSym;
			writepos = (writepos + 1) % bufflen;
			items++;
			return true;
		}

		bool CTokenBuffer::SkipOne()
		{
			if(items == 0)
				return false;

			readpos = (readpos + 1) % bufflen;
			items--;
			return true;
		}

		const std::string *CTokenBuffer::GetToken(INTEGER index, bool &wasNS, bool &isSym)
		{
			wasNS = false; // zakladamy, ze po koncu tekstu jest odstep
			if(index < items)
			{
				INTEGER abs_index = (readpos + index) % bufflen;
				wasNS = buff[abs_index].wasNS;
				isSym = buff[abs_index].isSym;
				return &(buff[abs_index].cp_orth);
			}
			else
			{
				return NULL;
			}
		}

		void CTokenBuffer::Dump()
		{
			std::cout << "BUFF of " << bufflen << ": ";
			for(INTEGER indx = 0; indx < bufflen; indx++)
			{
				if(indx < items)
				{
					INTEGER pos = (indx + readpos) % bufflen;
					std::cout << "[";
					if(!buff[pos].wasNS) std::cout << " ";
					std::cout << buff[pos].cp_orth;
					std::cout << "]";
				}
				else
				{
					std::cout << "(*)";
				}
			}
			std::cout << std::endl;
		}

	} // ExtSentencer
} // Corpus
