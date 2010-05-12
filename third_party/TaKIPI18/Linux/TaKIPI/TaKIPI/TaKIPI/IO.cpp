#include "../takipi_headers.h"
#include "IO.h"
#include "Config.h"

namespace TaKIPI
{

#ifndef WIN32
	int IO::iterator = -1;
	std::vector<std::string> IO::files = std::vector<std::string>();
#endif

	// Zwraca wartoœæ true, jak uda³o siê zmieniæ katalog.
	bool IO::ChangeDir(const char* dir)
	{
		return chdir(dir) == 0;
	}

	intptr_t IO::fileFound = -1;
	
	const char *IO::GetSeparator()
	{
#ifdef WIN32
		return "\\";
#else
		return "/";
#endif
	}

	bool IO::NextTreeFileP(std::string * file, Corpus::CValue* p, const char* filePattern, std::string &subdir)
	{
		std::stringstream path;
		if(!subdir.empty())
		{
			path << subdir << IO::GetSeparator();
		}
		path << "*" << filePattern;
		
#ifdef WIN32
		__finddata64_t findData;
		if(NextFile(p,path.str().c_str(),&findData))
		{
			(*file) = findData.name;
			return true;
		}
		return false;
#else
		if(fileFound == -1)
		{
			readFilesFromDirectory(filePattern, subdir);
			if((int)files.size() == 0)
				return false;
			fileFound = (intptr_t)0;
			iterator = 0;
		}
		else
		{
			iterator++;
			if(iterator >= (int)files.size())
				return false;
		}

		(*file) = files[iterator];
		std::vector<std::string> tokens;
		std::string temp = Corpus::Parser::Trim(*file);
		Corpus::Parser::Tokenize(temp,tokens,".");
		temp = tokens[0];
		Corpus::Parser::Tokenize(temp,tokens,"_");

		p->first  = Corpus::Parser::ParseDLONG(tokens[0]);
		p->second = Corpus::Parser::ParseDLONG(tokens[1]); 

		return true;
#endif
	}

	bool IO::NextTreeFile(std::string * file, Corpus::CValue* p, bool txt, std::string &subdir)
	{
		return NextTreeFileP(file,p,txt?".txt":".bin",subdir);
	}

	bool IO::NextLMTreeFile(std::string *file, Corpus::CValue* p, std::string &subdir)
	{
		return NextTreeFileP(file,p,".lmt",subdir);
	}

	bool IO::NextRipperFile(std::string *file, Corpus::CValue * p, std::string &subdir)
	{
		return IO::NextTreeFileP(file,p,".rip",subdir);
	}

	bool IO::NextPartFile(std::string *file, Corpus::CValue * p, std::string &subdir)
	{
		return NextTreeFileP(file,p,".PART",subdir);
	}


	bool IO::NextDataTreeFileP(Corpus::CValue* p, std::string &subdir)
	{
		std::string file;
		return NextTreeFile(&file,p,".data", subdir);
	}

	std::string IO::GetFullPath(const std::string &dir, const std::string &file)
	{
		if(dir.length() == 0)
		{
			std::string result(file);
			return result;
		}

		std::string result(dir);
		result.append(GetSeparator());
		result.append(file);
		return result;
	}

	std::string IO::GetFullPath(const std::string &dir, const std::string &subdir,
								const std::string &file)
	{
		std::string subdirPath = GetFullPath(dir, subdir);
		return GetFullPath(subdirPath, file);
	}

#ifdef WIN32

	bool IO::NextFile(Corpus::CValue* p, const char* filePattern, __finddata64_t* findData)
	{
		if (fileFound == -1)
		{
			if ((fileFound = _findfirst64(filePattern,findData)) == -1)
			{
				return false;
			}
		}
		else
		{
			if (_findnext64(fileFound,findData) == -1)
			{
				return false;
			}
		}

		std::vector<std::string> tokens;
		std::string temp = Corpus::Parser::Trim(std::string(findData->name));
		Corpus::Parser::Tokenize(temp,tokens,".");
		temp = tokens[0];
		Corpus::Parser::Tokenize(temp,tokens,"_");

		p->first  = Corpus::Parser::ParseDLONG(tokens[0]);
		p->second = Corpus::Parser::ParseDLONG(tokens[1]);

		return true;
	}

	void IO::MakeSubdir(std::string subdir)
	{
		std::string path = TaKIPI::IO::GetFullPath(TaKIPI::CConfig::commonPath, subdir);
		MakeDir(path.c_str());
	}

	void IO::MakeDir(const char *dir)
	{
		mkdir(dir);
	}

#else


	bool IO::hasExtension(dirent *file, const char *ext)
	{
		size_t len = strlen(ext);
		if(len == 0)
			return false;
		
		if(file != NULL)
		{
			std::string fname(file->d_name);
			if(fname.rfind(ext) == (fname.size() - len))
				return true;
		}
		return false;
	}

	bool IO::MakeSubdir(std::string subdir)
	{
		std::string path = TaKIPI::IO::GetFullPath(TaKIPI::CConfig::commonPath, subdir);
		return MakeDir(path.c_str());
	}

	bool IO::MakeDir(const char *dirName)
	{
		bool result = false;
		mkdir(dirName, 0777);
		return result;
	}

	std::vector<std::string> &IO::getFiles()
	{
		return files;
	}

	void IO::readFilesFromDirectory(const char *ext, const std::string& subdir)
	{
		files.clear();

		DIR *direntry;
		struct dirent *fileentry;

		if(subdir.empty())
			direntry = opendir("./");
		else
			direntry = opendir(subdir.c_str());
		fileentry = readdir(direntry);

		while (fileentry != NULL)
		{
			if (hasExtension(fileentry, ext))
			{
				files.push_back(fileentry->d_name);
			}
			fileentry = readdir(direntry);
		}
		closedir(direntry);
	}

#endif

	void IO::SearchAgain()
	{
		fileFound = -1;
	}

	std::string IO::ReadLine(std::ifstream& inFile)
	{
		static char line[255];
		inFile.getline(line,255);
		return std::string(line);
	}


	void IO::WriteToFile(const char* fileName, std::stringstream& out)
	{
		std::ofstream myFile;
		myFile.open(fileName);
		myFile << out.rdbuf();
		myFile.flush();
		myFile.close();
	}

}//namespace TaKIPI
