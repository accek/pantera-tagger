#include "../../corpuslib_headers.h"
#include "CorpusSentenceReader.h"
#include "../Config.h"
#include "../Tokenisation/CorpusTokenReader.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		CCorpusSentenceReader::CCorpusSentenceReader(std::istream &co_inStream,
				bool disambOnly, bool convertToCP, bool onlySentChunks,
				std::ostream *chunkOutput)
			:co_inputStream(co_inStream), filterDisamb(disambOnly),
				convertOutput(convertToCP), chunkOut(chunkOutput)
		{
			chunkBeg = onlySentChunks ? "<chunk type=\"s\"" : "<chunk ";
		}
		
		CCorpusSentenceReader::~CCorpusSentenceReader()
		{
		}
		
		CSentence *CCorpusSentenceReader::GetNext(bool eliminateSubstGerIgn)
		{
			std::stringstream co_sentBuff;
			std::string line;
			
			if(chunkOut && !unflushedLine.empty())
			{
				*chunkOut << unflushedLine << std::endl;
				unflushedLine = "";
			}
			
			while(true)
			{
				do // skip all lines before chunk type="s"
				{
					std::getline(co_inputStream, line);
					if(!co_inputStream)
					{
						return NULL;
					}
					if(chunkOut) // flush to chunkOut
					{
						*chunkOut << line << std::endl;
					}
				}
				while(line.find(chunkBeg) == line.npos);
				
				do // gather all lines before </chunk>
				{
					std::getline(co_inputStream, line);
					if(!co_inputStream)
					{
						return NULL;
					}
					if(!line.empty())
						co_sentBuff << line << std::endl;
				}
				while(line.find("</chunk") == line.npos);
				if(chunkOut)
				{
					unflushedLine = line;
				}
				
				// now parse the sentence
				Tokenisation::CCorpusTokenReader rdr(co_sentBuff, filterDisamb, convertOutput);
				CSentence *sent = new Corpus::CSentence();
				CToken *token;
				while( (token = rdr.GetNext()) != NULL)
				{
					sent->AddToken(token);
				}
				
				if(sent->Size() == 0)
				{
					// we skip the sentence and proceed with next
					delete sent;
				}
				else
				{
					if(eliminateSubstGerIgn)
					{
						sent->EliminateSubstGerIgn();
					}
					return sent;
				}
			}
		}
		
	} // namespace ExtSentencer
} // namespace Corpus

