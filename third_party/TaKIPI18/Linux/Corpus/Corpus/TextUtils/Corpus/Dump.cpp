#include "../../../corpuslib_headers.h"
#include "Dump.h"

namespace Corpus
{
	namespace TextUtils
	{
		namespace CorpusUtil
		{
			CDump::CDump(const char *fileName)
			{
				outStream.open(fileName);
			}

			CDump::~CDump(void)
			{
				outStream.close();
			}

			void CDump::Dump(Corpus::CSentence& sentence)
			{
				int position = sentence.GetPosition();

				for(int i = 0; i < sentence.Size(); i++)
				{
					sentence.Goto(i);

					outStream << i << ": \"" << sentence.GetCurrentToken()->GetOrth() << "\"\n";
					for(int j = 0; j < (int)sentence.GetCurrentToken()->GetLexCount(); j++)
					{
						if ( sentence.GetCurrentToken()->GetLexem(j)->GetDisamb() )
							outStream << "--";
						else 
							outStream << "  ";
						outStream << "- " << sentence.GetCurrentToken()->GetLexem(j)->GetBase() 
							<< " \t[" << sentence.GetCurrentToken()->GetLexem(j)->GetTag().ToString()
							<< "]: " << sentence.GetCurrentToken()->GetLexem(j)->probability << std::endl;
					}
					outStream << "--------------------------------------------" << std::endl;
					outStream.flush();
				}

				sentence.Goto(position);
			}

			void CDump::Dump(const char *str)
			{
				outStream << str << std::endl;
				outStream << "--------------------------------------------" << std::endl;
			}

		} // namespace CorpusUtil
	} // namespace TextUtils
} // namespace Corpus
