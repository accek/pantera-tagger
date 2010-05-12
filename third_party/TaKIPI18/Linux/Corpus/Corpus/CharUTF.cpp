#include "../corpuslib_headers.h"
#include "CharUTF.h"

namespace Corpus
{
	/*
	Tworzy znak utf z podanych w wektorze znaków sk³adowych.
	*/
	CCharUTF::CCharUTF(std::vector<unsigned char> utfChars)
	{
		for(int i = 0; i < (int)utfChars.size(); i++)
		{
			utfChars_.push_back(utfChars[i]);
		}
	}

	CCharUTF::~CCharUTF(void)
	{
	}

	/*
	Sprawdza czy dany znak UTF jest spacj¹.
	*/
	bool CCharUTF::IsSpace()
	{
		if(utfChars_[0] == (unsigned char)'Â' && utfChars_[1] == (unsigned char)-96)
			return true;
		else
			return false;
	}

	// Nie u¿ywane.
	bool CCharUTF::IsPunct()
	{
		return false;
	}

	/*
	Do³¹cza niniejszy znak do stringa s.
	*/
	void CCharUTF::AppendToString(std::string& s)
	{
		for(size_t i = 0; i < utfChars_.size(); i++)
		{
			s += utfChars_[i];
		}
	}

	/*
	Czy znak jest znakiem EOF.
	*/
	bool CCharUTF::IsEOF()
	{
		return utfChars_[0] == 255;
	}

	/*
	Operator do iterowania po znakach sk³adowych.
	*/
	unsigned char CCharUTF::operator [](int index)
	{
		if(index < 0 || index >= (int)utfChars_.size())
			throw tagexception("Blad indeksowania znaku.");
		else
			return utfChars_[index];
	}
}

