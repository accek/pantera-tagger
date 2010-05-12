#include "../../corpuslib_headers.h"
#include "Reader.h"
#include "../Config.h"

namespace Corpus
{
	namespace Readers
	{
		CReader::CReader(std::istream& stream)
			:stream(stream)
		{
			this->newTag = false;
			this->forceUtfEncoding = false;
		}

		CReader::~CReader(void)
		{
		}

		void CReader::CleanUp()
		{
			// kept for backward compatibility
		}

		std::string CReader::GetNextTokenText(std::istream& inStream)
		{
			std::string line;
			std::string empty;
			std::stringstream tokenStream;
			//char *buff = new char[512];

			do // skip all lines without tok or ns
			{ // what about beef?
				std::getline(inStream, line);
				//inStream.getline(buff,512);
				//line = buff;
				if(!inStream)
				{
					//delete [] buff;
					return empty;
				}
			}
			while(line.find("<tok>") == line.npos
				&& line.find("<ns/>") == line.npos);

			do // gather all lines until </tok>
			{
				tokenStream << line << std::endl;
				std::getline(inStream, line);
				//inStream.getline(buff,512);
				//line = buff;
				if(!inStream)
				{
					//delete [] buff;
					return empty;
				}
			}
			while(line.find("</tok>") == line.npos);

			tokenStream << line; // include </tok>

			//delete [] buff;
			return tokenStream.str();
		}
	}
}
