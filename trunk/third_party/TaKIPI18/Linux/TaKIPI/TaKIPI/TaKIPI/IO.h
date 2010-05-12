/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_IO_H
#define TAKIPI_IO_H

#include "../takipi_headers.h"

namespace TaKIPI
{
	class TAKIPI_DEV_API IO
	{
	public:
		IO(void);
		~IO(void);

		static const char *GetSeparator();

		static std::string GetFullPath(const std::string &dir, const std::string &file);
		static std::string GetFullPath(const std::string &dir, const std::string &subdir,
									   const std::string &file);

		// Zwraca wartoœæ true, jak uda³o siê zmieniæ katalog.
		static bool ChangeDir(const char* dir);

		static bool NextTreeFileP(std::string * file, Corpus::CValue* p, const char* filePattern, std::string &subdir);
		static bool NextTreeFile(std::string * file, Corpus::CValue* p, bool txt, std::string &subdir);
		static bool NextLMTreeFile(std::string *file, Corpus::CValue* p, std::string &subdir);
		static bool NextRipperFile(std::string *file, Corpus::CValue * p, std::string &subdir);
		static bool NextPartFile(std::string *file, Corpus::CValue * p, std::string &subdir);

		static bool NextDataTreeFileP(Corpus::CValue* p, std::string &subdir);


#ifdef WIN32

		static bool NextFile(Corpus::CValue* p, const char* filePattern, __finddata64_t* findData);

		// makes subdir in the CConfig::commonPath directory
		static void MakeSubdir(std::string subdir);
		static void MakeDir(const char *dir);

#else

		static std::vector<std::string> files;
		static int iterator;

		static bool MakeSubdir(std::string subdir);
		static bool MakeDir(const char *dirName);

		static std::vector<std::string> &getFiles();

		static void readFilesFromDirectory(const char *ext, const std::string& subdir);

#endif

		static void SearchAgain();

		static std::string ReadLine(std::ifstream& inFile);


		static void WriteToFile(const char* fileName, std::stringstream& out);

	private:
		static intptr_t fileFound;

#ifndef WIN32
		static bool hasExtension(dirent *file, const char *ext);
#endif
	};

}//namespace TaKIPI
#endif // TAKIPI_IO_H
