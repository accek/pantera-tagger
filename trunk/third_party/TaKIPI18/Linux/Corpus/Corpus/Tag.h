/*  Biblioteka Corpus
 *  Zestaw narzÄ™dzi do przetwarzania tekstÃ³w w jÄ™zyku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef CORPUSLIB_TAG_H
#define CORPUSLIB_TAG_H

#include "../corpuslib_headers.h"
#include "typedefs.h"
#include "Value.h"


#define toStrBufSize 100

namespace Corpus
{

	class CORPUS_API CTag
	{
	public:

	/***********************************************
		KONSTRUKCJA / DESTRUKCJA OBIEKTU
	************************************************/
		/// Konstruktor pobierajacy poczatkowe wartosci pol.
		CTag(TFlexClass fcl = cf_none);

		CTag(const std::string &tag);
		
		/** Konstruktor parsujacy ze strumienia wejsciowego.
		  * Dane w strumieniu wejsciowym musza byc zawarte
		  * w  XML-owym tagu "tag". */
		CTag(std::istream&);
		/// Konstruktor kopiuj¹cy.
		CTag(const CTag& t);
		/// Destruktor.
		~CTag(void);

		/// Operator przypisania.
		const CTag& operator=(const CTag& t);

	/***********************************************
		POLA OBIEKTU
	************************************************/
		/// Klasa fleksyjna danego tagu.
		TFlexClass FlexClass;
		/// Zbior w postaci flag kategorii gramatycznych danego tagu.
		TCategories Categories;


	/***********************************************
		METODY OBIEKTU instancyjne
	************************************************/
		//pobiera napis odpowiadaj¹cy klasie fleksyjnej obiektu
		const char* GetFlexClassString() const;
		//ustawia pierwsz¹ kategoriê zgodnie z podanym stringiem
		void Set1stCat(const std::string &category);
		//ustawia drug¹ kategoriê zgodnie z podanym stringiem
		void Set2ndCat(const std::string &category);
		//ustawia trzeci¹ kategoriê zgodnie ze stringiem
		void Set3rdCat(const std::string &category);
		//ustawia czwart¹ kategoriê zgodnie ze stringiem
		void Set4thCat(const std::string &category);
		//ustawia pi¹t¹ kategoriê zgodnie ze stringiem
		void Set5thCat(const std::string &category);
		//ustawia pi¹t¹ kategoriê zgodnie ze stringiem
		void Set6thCat(const std::string &category);
		//pobiera pierwsz¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
		const char * Get1stCatS() const;
		//pobiera drug¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
		const char * Get2ndCatS() const;
		//pobiera trzeci¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
		const char * Get3rdCatS() const;
		//pobiera czwart¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
		const char * Get4thCatS() const;
		//pobiera pi¹t¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
		const char * Get5thCatS() const;
		//pobiera szóst¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
		const char * Get6thCatS() const;

		/// Ustawia dane na podstawie podanego stringa.
		void ParseContent(const std::string &content);
		/// Pobiera ca³oœciowo tag jako string.
		const char * ToString() const;

		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetLiczbaS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetPrzypadekS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetRodzajS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetOsobaS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetStopienS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetAspektS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetNegacjaS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetAkcentS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetPoprzyimS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetAglutaS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetWokalS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetAkomodS() const;
		//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
		const char* GetInterpS() const;

		// nowe: pobiera kategorie numtype (klasa tnum)
		const char* GetNumtypeS() const;

		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetNmb() const;
		static const DLONG GetNmb(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetCas() const;
		static const DLONG GetCas(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetGnd() const;
		static const DLONG GetGnd(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetPer() const;
		static const DLONG GetPer(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetDeg() const;
		static const DLONG GetDeg(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetAsp() const;
		static const DLONG GetAsp(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetNeg() const;
		static const DLONG GetNeg(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetAcn() const;
		static const DLONG GetAcn(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetPpr() const;
		static const DLONG GetPpr(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetAgg() const;
		static const DLONG GetAgg(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetVcl() const;
		static const DLONG GetVcl(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetAcm() const;
		static const DLONG GetAcm(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetInp() const;
		static const DLONG GetInp(const std::string& stringCat);
		// nowe: pobiera kategorie numtype (klasa tnum)
		const DLONG GetTnt() const;
		static const DLONG GetTnt(const std::string& stringCat);
		//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
		const DLONG GetFlex() const;

		// pobiera wartoœæ danego atrybutu dla danego taga
		const DLONG GetAttribValue(int attIndex) const;

		static const DLONG GetGrammCatValue(const std::string& stringCat);

	/***********************************************
		METODY STATYCZNE
	************************************************/
		//pobiera napis odpowiadaj¹cy podanej klasie fleksyjnej
		static const char* GetFlexClassString(const TFlexClass);
		//pobiera klase fleksyjn¹ odpowiadaj¹cy jej string'owi
		static const TFlexClass GetFlexClass(const std::string& stringClass);
		// pobiera index attrybutu i zwraca ³añcuch jego identyfikuj¹cy
		static const std::string GetAttribString(int index);
		// pobiera ³añcuch attrybutu i zwraca index jego identyfikuj¹cy
		static const int GetAttribIndex(std::string attrib);
		// pobiera maskê atrybutu i wrzuca j¹ do out
		static const void GetAttribMask(int index, CValue& out);

		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetNmbNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetNmbNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetCasNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetCasNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetGndNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetGndNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetPerNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetPerNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetDegNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetDegNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetAspNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetAspNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetNegNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetNegNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetAcnNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetAcnNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetPprNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetPprNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetAggNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetAggNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetVclNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetVclNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetAcmNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetAcmNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetInpNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetInpNames(std::stringstream& out, const CValue& mask);
		//nowe: numtype (klasa tnum)
		static void GetTntNames(std::stringstream& out);
		static void GetTntNames(std::stringstream& out, const CValue& mask);
		//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
		static void GetFlexNames(std::stringstream& out);
		//wype³nia wektor mo¿liwymi wartosciami dla danego attrybutu
		static const void GetAttribNames(int attribute, std::stringstream& out);
		static const void GetAttribNames(int attribute, std::stringstream& out,const CValue& mask);
	/***********************************************
		PRIVATE! KEEP OUT;)
	************************************************/

	private:
	/***********************************************
		ustawianie wartosci na podstawie parsowania
	************************************************/
		//laduje strukture danymi przeparsowanymi z podanego stringa
		//void ParseContent(std::string &content); -- przeniesone do public
		//ustawia klase flexyjna zgodna z podana nazwa
		void SetFlexClass(const std::string & flexClassName);
		//na podstawie przydzielonej klasy fleksyjnej przypisuje info, ze ominieto specyfikacje kategorii
		void SetCategoryOmitted();
		//ustawia kategorie gramatyczne na podstawie przekazanego stringa
		void SetCategories(std::string & catContent);
		//ustawia pierwsza kategorie gramatyczna
		void SetFirstCategory(const std::string & category);
		//bufor do zwracania wartoœci ToString()
		static char toStrBuf[toStrBufSize];
	};
}
#endif

