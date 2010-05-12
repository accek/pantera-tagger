#include "../../corpuslib_headers.h"
#include "Document.h"

namespace Corpus
{
	namespace Documents
	{
		Document::Document(std::string fn)
		{
			this->filename = fn;
			this->reader = 0;
			this->sentencer_ = 0;
			this->file = NULL;
		}

		Document::~Document(void)
		{
			if(sentencer_)
				delete sentencer_;
			Close();
		}

		void Document::Close()
		{
			if(reader)
			{
				delete reader;
				reader = 0;
			}
			if(file != NULL)
			{
				if(file->is_open())
					file->close();
				delete file;
				file= NULL;
			}
		}
	}
}
