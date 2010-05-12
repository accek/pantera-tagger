#include "../../../corpuslib_headers.h"
#include "../../Documents/TaggedDocument.h"
#include "Corpus.h"

namespace Corpus
{
	namespace TextUtils
	{
		namespace CorpusUtil
		{

			void CreateCorpusFromTaggedFiles(Corpus::CCorpus &corpus, std::string filename, std::string prefix) throw (std::string)
			{
				std::ifstream file(filename.c_str());
				std::string line;
				if(!file)
					throw (std::string("File not found: ")+filename);

				while(file)
				{
					file >> line;
					boost::algorithm::trim(line);
					if( !line.empty() )
					{
						corpus.AddDocument( new Corpus::Documents::TaggedDocument(prefix+line) );
					}

				};
			} // CreateCorpusFromTaggedFiles

		} // namespace CorpusUtil
	} // namespace TextUtils
} // namespace Corpus
