#include "../corpuslib_headers.h"

#include "Tag.h"
#include "Parser/Parser.h"
#include "Token.h"
#include "functions.h"

namespace Corpus
{

	CTag::CTag(TFlexClass flexClass)
	{
		this->FlexClass = flexClass;
		this->Categories = 0;
	}

	CTag::CTag(const CTag& t)
	{
		this->Categories = t.Categories;
		this->FlexClass = t.FlexClass;
	}

	const CTag& CTag::operator=(const CTag& t)
	{
		this->Categories = t.Categories;
		this->FlexClass = t.FlexClass;

		return *this;
	}

	CTag::CTag(std::istream &input)
	{
		std::string readStr;
		if(!(Parser::EatXmlTag(input,"ctag",readStr)).empty())
			throw tagexception(std::string("Wykryto nieoczekiwane dane:\n").append(readStr).append("\n").c_str());
		std::string temp = Parser::EatXmlTag(input,"/ctag",readStr);
		CTag::ParseContent( temp );
	}
	CTag::CTag(const std::string &tag)
	{
		CTag::ParseContent(tag);
	};

	void CTag::ParseContent(const std::string &content)
	{
		size_t flxClassEndIdx = content.find_first_of(':');
		if(flxClassEndIdx != std::string::npos)
		{
			std::string copy(content);
			SetFlexClass(copy.substr(0,flxClassEndIdx));
			SetCategories(copy.erase(0,flxClassEndIdx+1));
		}
		else
		{
			SetFlexClass(content);
			SetCategoryOmitted();
		}
	}

	void CTag::SetCategories(std::string &catContent)
	{
		Categories = 0;

		if((DLONG)FlexClass	& mask_FlexNoCategoriable)
			throw tagexception(std::string("Nieoczekiwane info o kategoriach dla klasy fleksyjnej ").append(GetFlexClassString()).append(":\n").append(catContent).c_str());

		size_t catEnd = catContent.find_first_of(':');
		std::string category = catContent.substr(0,catEnd);
		Set1stCat(category);
		if(catEnd == std::string::npos)
			return;

		catContent.erase(0,catEnd+1);
		catEnd = catContent.find_first_of(':');
		category = catContent.substr(0,catEnd);
		Set2ndCat(category);
		if(catEnd == std::string::npos)
			catContent.erase();
		else
			catContent.erase(0,catEnd+1);

		catEnd = catContent.find_first_of(':');
		category = catContent.substr(0,catEnd);
		Set3rdCat(category);

		if(catEnd == std::string::npos)
			catContent.erase();
		else
			catContent.erase(0,catEnd+1);
		catEnd = catContent.find_first_of(':');
		category = catContent.substr(0,catEnd);
		Set4thCat(category);

		if(catEnd == std::string::npos)
			catContent.erase();
		else
			catContent.erase(0,catEnd+1);
		catEnd = catContent.find_first_of(':');
		category = catContent.substr(0,catEnd);
		Set5thCat(category);

		if(catEnd == std::string::npos)
			catContent.erase();
		else
			catContent.erase(0,catEnd+1);
		catEnd = catContent.find_first_of(':');
		category = catContent.substr(0,catEnd);
		Set6thCat(category);
		if(catEnd == std::string::npos)
			return;
		else
		{
			catContent.erase(0,catEnd);
			throw tagexception(std::string("Po przeanalizowaniu tagu napotkano nieoczekiwane dane: ").append(catContent).c_str());
		}

	}

	//pobiera ca³oœciowo tag jako string
	const char * CTag::ToString() const
	{
		std::stringstream strbuff;
		strbuff << GetFlexClassString()
				<< Get1stCatS() << Get2ndCatS() << Get3rdCatS()
				<< Get4thCatS() << Get5thCatS() << Get6thCatS();
		
		strncpy(toStrBuf,strbuff.str().c_str(), toStrBufSize);
		toStrBuf[toStrBufSize - 1] = 0;
		
		return toStrBuf;
	}

	//pobiera pierwsz¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
	const char * CTag::Get1stCatS() const
	{
		DLONG catval;
		if(FlexClass & mask_FlexWith1stLiczba)
		{
			catval = Categories & gcm_liczba;
			if(catval == gc_sg) { return ":sg";}
			if(catval == gc_pl) { return ":pl";}
		}
		if(FlexClass & mask_FlexWith1stPrzypadek)
		{
			catval = Categories & gcm_przypadek;
			if(catval == gc_nom) { return ":nom";}
			if(catval == gc_gen) { return ":gen";}
			if(catval == gc_dat) { return ":dat";}
			if(catval == gc_acc) { return ":acc";}
			if(catval == gc_inst) { return ":inst";}
			if(catval == gc_loc) { return ":loc";}
			if(catval == gc_voc) { return ":voc";}
		}
		if(FlexClass & mask_FlexWith1stAspekt)
		{
			catval = Categories & (DLONG)gcm_aspekt;
			if(catval == gc_imperf) { return ":imperf";}
			if(catval == gc_perf) { return ":perf";}
		}
		if(FlexClass & mask_FlexWith1stNumtype)
		{ // DONE: numtype (tnum)
			catval = Categories & (DLONG)gcm_numtype;
			if(catval == gc_integer) { return ":integer"; }
			if(catval == gc_frac) { return ":frac"; }
		}
		if(FlexClass & cf_interp)
		{
			catval = Categories & (DLONG)gcm_interp;
			if(catval == gc_nb) { return ":nb";}
			if(catval == gc_sp) { return ":sp";}
			if(catval == gc_na) { return ":na";}
		}
		if(FlexClass & cf_adv)
		{
			catval = Categories & (DLONG)gcm_stopien;
			if(catval == gc_pos) { return ":pos";}
			if(catval == gc_comp) { return ":comp";}
			if(catval == gc_sup) { return ":sup";}
		}
		if(FlexClass & cf_qub)
		{
			catval = Categories & (DLONG)gcm_wok;
			if(catval == gc_wok) { return ":wok";}
			if(catval == gc_nwok) { return ":nwok";}
		}
		return "";
	}
	//pobiera drug¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
	const char * CTag::Get2ndCatS() const
	{
		DLONG catval;
		if(FlexClass & mask_FlexWith2ndPrzypadek)
		{
			catval = Categories & (DLONG)gcm_przypadek;
			if(catval == gc_nom) { return ":nom";}
			if(catval == gc_gen) { return ":gen";}
			if(catval == gc_dat) { return ":dat";}
			if(catval == gc_acc) { return ":acc";}
			if(catval == gc_inst) { return ":inst";}
			if(catval == gc_loc) { return ":loc";}
			if(catval == gc_voc) { return ":voc";}
		}
		if(FlexClass & mask_FlexWith2ndRodzaj)
		{
			catval = Categories & (DLONG)gcm_rodzaj;
			if(catval == gc_m1) { return ":m1";}
			if(catval == gc_m2) { return ":m2";}
			if(catval == gc_m3) { return ":m3";}
			if(catval == gc_f) { return ":f";}
			if(catval == gc_n) { return ":n";}
		}
		if(FlexClass & mask_FlexWith2ndOsoba)
		{
			catval = Categories & (DLONG)gcm_osoba;
			if(catval == gc_pri) { return ":pri";}
			if(catval == gc_sec) { return ":sec";}
			if(catval == gc_ter) { return ":ter";}
		}
		if(FlexClass & cf_prep)
		{
			catval = Categories & (DLONG)gcm_wok;
			if(catval == gc_wok) { return ":wok";}
			if(catval == gc_nwok) { return ":nwok";}
		}
		return "";
	}

	//pobiera trzeci¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
	const char * CTag::Get3rdCatS() const
	{
		DLONG catval;
		if(FlexClass & mask_FlexWith3rdRodzaj)
		{
			catval = Categories & (DLONG)gcm_rodzaj;
			if(catval == gc_m1) { return ":m1";}
			if(catval == gc_m2) { return ":m2";}
			if(catval == gc_m3) { return ":m3";}
			if(catval == gc_f) { return ":f";}
			if(catval == gc_n) { return ":n";}
		}
		if(FlexClass & mask_FlexWith3rdAspekt)
		{
			catval = Categories & (DLONG)gcm_aspekt;
			if(catval == gc_imperf) { return ":imperf";}
			if(catval == gc_perf) { return ":perf";}
		}

		return "";
	}
	//pobiera czwart¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
	const char * CTag::Get4thCatS() const
	{
		DLONG catval;
		if(FlexClass & mask_FlexWith4thOsoba)
		{
			catval = Categories & (DLONG)gcm_osoba;
			if(catval == gc_pri) { return ":pri";}
			if(catval == gc_sec) { return ":sec";}
			if(catval == gc_ter) { return ":ter";}
		}
		if(FlexClass & cf_adj)
		{
			catval = Categories & (DLONG)gcm_stopien;
			if(catval == gc_pos) { return ":pos";}
			if(catval == gc_comp) { return ":comp";}
			if(catval == gc_sup) { return ":sup";}
		}
		if(FlexClass & mask_FlexWith4thAspekt)
		{
			catval = Categories & (DLONG)gcm_aspekt;
			if(catval == gc_imperf) { return ":imperf";}
			if(catval == gc_perf) { return ":perf";}
		}
		if(FlexClass & cf_aglt)
		{
			catval = Categories & (DLONG)gcm_wok;
			if(catval == gc_wok) { return ":wok";}
			if(catval == gc_nwok) { return ":nwok";}
		}
		if(FlexClass & cf_praet)
		{
			catval = Categories & (DLONG)gcm_agluta;
			if(catval == gc_nagl) { return ":nagl";}
			if(catval == gc_agl) { return ":agl";}
		}
		if(FlexClass & cf_num)
		{
			catval = Categories & (DLONG)gcm_akomod;
			if(catval == gc_congr) { return ":congr";}
			if(catval == gc_rec) { return ":rec";}
		}

		return "";
	}
	//pobiera pi¹t¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
	const char * CTag::Get5thCatS() const
	{
		DLONG catval;
		if(FlexClass & mask_FlexWith5thNegacja)
		{
			catval = Categories & (DLONG)gcm_negacja;
			if(catval == gc_aff) { return ":aff";}
			if(catval == gc_neg) { return ":neg";}
		}
		if(FlexClass & cf_ppron12)
		{
			catval = Categories & (DLONG)gcm_akcent;
			if(catval == gc_akc) { return ":akc";}
			if(catval == gc_nakc) { return ":nakc";}
		}
		if(FlexClass & cf_ppron3)
		{
			catval = Categories & (DLONG)gcm_akcent;
			if(catval == gc_akc) { return ":akc";}
			if(catval == gc_nakc) { return ":nakc";}
			catval = Categories & (DLONG)gcm_poprzyim;
			if(catval == gc_praep) { return ":praep";}
			if(catval == gc_npraep) { return ":npraep";}
		}
		return "";
	}
	//pobiera szóst¹ kategoriê jako string, z dwukropkiem przed jeœli niepusta
	const char * CTag::Get6thCatS() const
	{
		DLONG catval;
		if(FlexClass & cf_ppron3)
		{
			catval = Categories & (DLONG)gcm_akcent;
			if(catval != gc_akc && catval != gc_nakc)
			{
				// kategoria pi¹ta nie zawiera³a akcentowoœci,
				// wiêc poprzyimkowoœæ wypad³a jako pi¹ta kategoria;
				// zatem nie zwracamy jej ju¿ jako szósej
				return "";
			}
			catval = Categories & (DLONG)gcm_poprzyim;
			if(catval == gc_praep) { return ":praep";}
			if(catval == gc_npraep) { return ":npraep";}
		}
		return "";
	}

	//ustawia pierwsz¹ kategoriê zgodnie z podanym stringiem
	void CTag::Set1stCat(const std::string &category)
	{
		const char * categ = category.data();
		if(FlexClass & mask_FlexWith1stLiczba)
		{
			if(!stricmp(categ,"sg")) { Categories |= gc_sg; return; }
			if(!stricmp(categ,"pl")) { Categories |= gc_pl; return; }
		}
		if(FlexClass & mask_FlexWith1stPrzypadek)
		{
			if(!stricmp(categ,"nom")) { Categories |= gc_nom; return; }
			if(!stricmp(categ,"gen")) { Categories |= gc_gen; return; }
			if(!stricmp(categ,"dat")) { Categories |= gc_dat; return; }
			if(!stricmp(categ,"acc")) { Categories |= gc_acc; return; }
			if(!stricmp(categ,"inst")) { Categories |= gc_inst; return; }
			if(!stricmp(categ,"loc")) { Categories |= gc_loc; return; }
			if(!stricmp(categ,"voc")) { Categories |= gc_voc; return; }
		}
		if(FlexClass & mask_FlexWith1stAspekt)
		{
			if(!stricmp(categ,"imperf")) { Categories |= gc_imperf; return; }
			if(!stricmp(categ,"perf")) { Categories |= gc_perf; return; }
		}
		if(FlexClass & mask_FlexWith1stNumtype)
		{ // DONE: numtype (cf_tnum)
			if(!stricmp(categ,"integer")) { Categories |= gc_integer; return; }
			if(!stricmp(categ,"frac")) { Categories |= gc_frac; return; }
		}
		if(FlexClass & cf_interp)
		{
			if(category.empty()) { Categories |= gc_ointer; return; }
			if(!stricmp(categ,"nb")) { Categories |= gc_nb; return; }
			if(!stricmp(categ,"sp")) { Categories |= gc_sp; return; }
			if(!stricmp(categ,"na")) { Categories |= gc_na; return; }
		}
		if(FlexClass & cf_adv)
		{
			if(!stricmp(categ,"pos")) { Categories |= gc_pos; return; }
			if(!stricmp(categ,"comp")) { Categories |= gc_comp; return; }
			if(!stricmp(categ,"sup")) { Categories |= gc_sup; return; }
		}
		if(FlexClass & cf_qub)
		{
			if(category.empty()) { Categories |= gc_owok; return; }
			if(!stricmp(categ,"wok")) { Categories |= gc_wok; return; }
			if(!stricmp(categ,"nwok")) { Categories |= gc_nwok; return; }
		}
		throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
			.append(" odczytano niewlasciwy tag pierwszej kategorii: ")
			.append(categ).c_str());
	}



	//ustawia drug¹ kategoriê zgodnie z podanym stringiem
	void CTag::Set2ndCat(const std::string &category)
	{
		const char * categ = category.data();
		if(FlexClass & mask_FlexWithNo2nd)
		{
			if(category.empty())
				return;
			else
				throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
				.append(" oczekiwano braku drugiego tagu kategorii, a odczytano ")
				.append(categ).c_str());
		}
		if(FlexClass & mask_FlexWith2ndPrzypadek)
		{
			if(!stricmp(categ,"nom")) { Categories |= gc_nom; return; }
			if(!stricmp(categ,"gen")) { Categories |= gc_gen; return; }
			if(!stricmp(categ,"dat")) { Categories |= gc_dat; return; }
			if(!stricmp(categ,"acc")) { Categories |= gc_acc; return; }
			if(!stricmp(categ,"inst")) { Categories |= gc_inst; return; }
			if(!stricmp(categ,"loc")) { Categories |= gc_loc; return; }
			if(!stricmp(categ,"voc")) { Categories |= gc_voc; return; }
		}
		if(FlexClass & mask_FlexWith2ndRodzaj)
		{
			if(!stricmp(categ,"m1")) { Categories |= gc_m1; return; }
			if(!stricmp(categ,"m2")) { Categories |= gc_m2; return; }
			if(!stricmp(categ,"m3")) { Categories |= gc_m3; return; }
			if(!stricmp(categ,"f")) { Categories |= gc_f; return; }
			if(!stricmp(categ,"n")) { Categories |= gc_n; return; }
		}
		if(FlexClass & mask_FlexWith2ndOsoba)
		{
			if(!stricmp(categ,"pri")) { Categories |= gc_pri; return; }
			if(!stricmp(categ,"sec")) { Categories |= gc_sec; return; }
			if(!stricmp(categ,"ter")) { Categories |= gc_ter; return; }
		}
		if(FlexClass & cf_prep)
		{
			if(category.empty()) { Categories |= gc_owok; return; }
			if(!stricmp(categ,"wok")) { Categories |= gc_wok; return; }
			if(!stricmp(categ,"nwok")) { Categories |= gc_nwok; return; }
		}
		throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
			.append(" odczytano niewlasciwy tag drugiej kategorii: ")
			.append(categ).c_str());
	}
	//ustawia trzeci¹ kategoriê zgodnie z podanym stringiem
	void CTag::Set3rdCat(const std::string &category)
	{
		const char * categ = category.data();
		if(FlexClass & mask_FlexWithNo3rd)
		{
			if(category.empty())
				return;
			else
				throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
				.append(" oczekiwano braku trzeciego tagu kategorii, a odczytano ")
				.append(categ).c_str());
		}
		if(FlexClass & mask_FlexWith3rdRodzaj)
		{
			if(!stricmp(categ,"m1")) { Categories |= gc_m1; return; }
			if(!stricmp(categ,"m2")) { Categories |= gc_m2; return; }
			if(!stricmp(categ,"m3")) { Categories |= gc_m3; return; }
			if(!stricmp(categ,"f")) { Categories |= gc_f; return; }
			if(!stricmp(categ,"n")) { Categories |= gc_n; return; }
		}
		if(FlexClass & mask_FlexWith3rdAspekt)
		{
			if(!stricmp(categ,"imperf")) { Categories |= gc_imperf; return; }
			if(!stricmp(categ,"perf")) { Categories |= gc_perf; return; }
		}

		throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
			.append(" odczytano niewlasciwy tag trzeciej kategorii: ")
			.append(categ).c_str());

	}
	//ustawia czwart¹ kategoriê zgodnie ze stringiem
	void CTag::Set4thCat(const std::string &category)
	{
		const char * categ = category.data();
		if(FlexClass & mask_FlexWithNo4th)
		{
			if(category.empty())
				return;
			else
				throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
				.append(" oczekiwano braku czwartego tagu kategorii, a odczytano ")
				.append(categ).c_str());
		}
		if(FlexClass & mask_FlexWith4thOsoba)
		{
			if(!stricmp(categ,"pri")) { Categories |= gc_pri; return; }
			if(!stricmp(categ,"sec")) { Categories |= gc_sec; return; }
			if(!stricmp(categ,"ter")) { Categories |= gc_ter; return; }
		}
		if(FlexClass & cf_adj)
		{
			if(!stricmp(categ,"pos")) { Categories |= gc_pos; return; }
			if(!stricmp(categ,"comp")) { Categories |= gc_comp; return; }
			if(!stricmp(categ,"sup")) { Categories |= gc_sup; return; }
		}
		if(FlexClass & mask_FlexWith4thAspekt)
		{
			if(!stricmp(categ,"imperf")) { Categories |= gc_imperf; return; }
			if(!stricmp(categ,"perf")) { Categories |= gc_perf; return; }
		}
		if(FlexClass & cf_aglt)
		{
			if(!stricmp(categ,"wok")) { Categories |= gc_wok; return; }
			if(!stricmp(categ,"nwok")) { Categories |= gc_nwok; return; }
		}
		if(FlexClass & cf_praet)
		{
			if(category.empty()) { Categories |= gc_oagl; return; }
			if(!stricmp(categ,"nagl")) { Categories |= gc_nagl; return; }
			if(!stricmp(categ,"agl")) { Categories |= gc_agl; return; }
		}
		if(FlexClass & cf_num)
		{
			if(category.empty()) { Categories |= gc_oacmd; return; }
			if(!stricmp(categ,"congr")) { Categories |= gc_congr; return; }
			if(!stricmp(categ,"rec")) { Categories |= gc_rec; return; }
		}

		throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
			.append(" odczytano niewlasciwy tag czwartej kategorii: ")
			.append(categ).c_str());

	}
	//ustawia pi¹t¹ kategoriê zgodnie ze stringiem
	void CTag::Set5thCat(const std::string &category)
	{
		const char * categ = category.data();
		if(FlexClass & mask_FlexWithNo5th)
		{
			if(category.empty())
				return;
			else
				throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
				.append(" oczekiwano braku pi¹tego tagu kategorii, a odczytano ")
				.append(categ).c_str());
		}
		if(FlexClass & cf_ppron12)
		{
			if(category.empty()) { Categories |= gc_oakc; return; }
			if(!stricmp(categ,"akc")) { Categories |= gc_akc; return; }
			if(!stricmp(categ,"nakc")) { Categories |= gc_nakc; return; }
		}
		if(FlexClass & cf_ppron3)
		{
			if(category.empty())
			{
				Categories |= gc_oakc;
				Categories |= gc_opraep;
				return;
			}
			if(!stricmp(categ,"akc"))
			{
				Categories |= gc_akc;
				return;
			}
			if(!stricmp(categ,"nakc"))
			{
				Categories |= gc_nakc;
				return;
			}
			if(!stricmp(categ,"praep"))
			{
				Categories |= gc_praep;
				Categories |= gc_oakc;
				return;
			}
			if(!stricmp(categ,"npraep"))
			{
				Categories |= gc_npraep;
				Categories |= gc_oakc;
				return;
			}
		}
		if(FlexClass & mask_FlexWith5thNegacja)
		{
			if(!stricmp(categ,"aff")) { Categories |= gc_aff; return; }
			if(!stricmp(categ,"neg")) { Categories |= gc_neg; return; }
		}
		throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
			.append(" odczytano niewlasciwy tag pi¹tej kategorii: ")
			.append(categ).c_str());
	}
	//ustawia szóst¹ kategoriê zgodnie ze stringiem
	void CTag::Set6thCat(const std::string &category)
	{
		const char * categ = category.data();
		if(FlexClass & mask_FlexWithNo6th)
		{
			if(category.empty())
				return;
			else
				throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
				.append(" oczekiwano braku szóstego tagu kategorii, a odczytano ")
				.append(categ).c_str());
		}
		if(FlexClass & cf_ppron3)
		{
			if(category.empty()
				&& (Categories & gcm_poprzyim) == 0)
			{
				Categories |= gc_opraep;
				return;
			}
			else if(category.empty())
			{
				return;
			}

			if((Categories & gcm_akcent) == gc_oakc)
				throw tagexception(std::string("Dla klasy ppron3 próbowano ustawiæ tag szóstej kategorii: ")
				.append(categ).append(",\ntymaczasem nie ustawiono akcentowoœci jako pi¹tej kategorii!").c_str());

			if(!stricmp(categ,"praep"))
			{
				Categories |= gc_praep;
				return;
			}
			if(!stricmp(categ,"npraep"))
			{
				Categories |= gc_npraep;
				return;
			}
		}
		throw tagexception(std::string("Dla klasy ").append(GetFlexClassString())
			.append(" odczytano niewlasciwy tag szóstej kategorii: ")
			.append(categ).c_str());

	}

	void CTag::SetCategoryOmitted()
	{
		//switch(FlexClass)
		//{
		//	case cf_interp: Categories = gc_ointer; return;
		//	case cf_adja:
		//	case cf_adjp:
		//	case cf_pred:
		//	case cf_conj: + parê nowych CASE!
		//	case cf_xxx:
		//	case cf_ign: Categories = gc_none; return;
		//	case cf_qub: Categories = gc_owok; return;
		//}
		//a dla maniakow optymalizacji sadze ze dobre bedzie:

		if((DLONG)FlexClass & mask_FlexNoCategoriable)
			Categories = gc_none;
		else if(FlexClass & cf_interp)
			Categories = gc_ointer;
		else if(FlexClass & cf_qub)
			Categories = gc_owok;
		else throw tagexception(std::string("Dla klasy fleksyjnej ").append(GetFlexClassString())
			.append(" nie mozna ominac czesci z kategoriami!").c_str());
	}

	const char* CTag::GetFlexClassString() const
	{
		return GetFlexClassString(this->FlexClass);
	}



	STATIC const char* CTag::GetFlexClassString(const TFlexClass flexClass)
	{
		switch(flexClass)
		{
		case cf_interp: return "interp";
		case cf_subst: return "subst";
		case cf_depr: return "depr";
		case cf_adj: return "adj";
		case cf_adja: return "adja";
		case cf_adjp: return "adjp";
		case cf_adv: return "adv";
		case cf_num: return "num";
		case cf_ppron12: return "ppron12";
		case cf_ppron3: return "ppron3";
		case cf_siebie: return "siebie";
		case cf_fin: return "fin";
		case cf_bedzie: return "bedzie";
		case cf_aglt: return "aglt";
		case cf_praet: return "praet";
		case cf_impt: return "impt";
		case cf_imps: return "imps";
		case cf_inf: return "inf";
		case cf_pcon: return "pcon";
		case cf_pant: return "pant";
		case cf_ger: return "ger";
		case cf_pact: return "pact";
		case cf_ppas: return "ppas";
		case cf_winien: return "winien";
		case cf_pred: return "pred";
		case cf_prep: return "prep";
		case cf_conj: return "conj";
		case cf_qub: return "qub";
		case cf_xxs: return "xxs";
		case cf_xxx: return "xxx";
		case cf_ign: return "ign";
		//TODO: To trzeba jak najszybciej zmieniæ :)
		case (cf_subst|cf_ger):
			return "suger";
		case cf_numcol: return "numcol";
		case cf_tnum: return "tnum";
		case cf_tsym: return "tsym";
		case cf_turi: return "turi";
		case cf_tmail: return "tmail";
		case cf_tdate: return "tdate";
		case cf_ttime: return "ttime";
		case cf_twf: return "twf";
		}
		throw tagexception("CTag::GetFlexClassString - nie zaimplementowano obslugi klas niejednoznacznych!");
	}

	STATIC const TFlexClass CTag::GetFlexClass(const std::string& stringClass)
	{
		const char * name = stringClass.c_str();
		if(!stricmp(name, "interp")) { return cf_interp; }
		if(!stricmp(name, "subst")) { return cf_subst; }
		if(!stricmp(name, "depr")) { return cf_depr; }
		if(!stricmp(name, "adj")) { return cf_adj; }
		if(!stricmp(name, "adja")) { return cf_adja; }
		if(!stricmp(name, "adjp")) { return cf_adjp; }
		if(!stricmp(name, "adv")) { return cf_adv; }
		if(!stricmp(name, "num")) { return cf_num; }
		if(!stricmp(name, "ppron12")) { return cf_ppron12; }
		if(!stricmp(name, "ppron3")) { return cf_ppron3; }
		if(!stricmp(name, "siebie")) { return cf_siebie; }
		if(!stricmp(name, "fin")) { return cf_fin; }
		if(!stricmp(name, "bedzie")) { return cf_bedzie; }
		if(!stricmp(name, "aglt")) { return cf_aglt; }
		if(!stricmp(name, "praet")) { return cf_praet; }
		if(!stricmp(name, "impt")) { return cf_impt; }
		if(!stricmp(name, "imps")) { return cf_imps; }
		if(!stricmp(name, "inf")) { return cf_inf; }
		if(!stricmp(name, "pcon")) { return cf_pcon; }
		if(!stricmp(name, "pant")) { return cf_pant; }
		if(!stricmp(name, "ger")) { return cf_ger; }
		if(!stricmp(name, "pact")) { return cf_pact; }
		if(!stricmp(name, "ppas")) { return cf_ppas; }
		if(!stricmp(name, "winien")) { return cf_winien; }
		if(!stricmp(name, "pred")) { return cf_pred; }
		if(!stricmp(name, "prep")) { return cf_prep; }
		if(!stricmp(name, "conj")) { return cf_conj; }
		if(!stricmp(name, "qub")) { return cf_qub; }
		if(!stricmp(name, "xxs")) { return cf_xxs; }
		if(!stricmp(name, "xxx")) { return cf_xxx; }
		if(!stricmp(name, "ign")) { return cf_ign; }

		if(!stricmp(name, "numcol")) { return cf_numcol; }

		if(!stricmp(name, "tnum")) { return cf_tnum; }
		if(!stricmp(name, "tsym")) { return cf_tsym; }
		if(!stricmp(name, "turi")) { return cf_turi; }
		if(!stricmp(name, "tmail")) { return cf_tmail; }
		if(!stricmp(name, "tdate")) { return cf_tdate; }
		if(!stricmp(name, "ttime")) { return cf_ttime; }
		if(!stricmp(name, "twf")) { return cf_twf; }

		return cf_none;
	}

	// DONE: tnum + numtype (tnt)
	STATIC const void CTag::GetAttribMask(int index, CValue& out)
	{
		switch(index)
		{
			case CToken::flex:
				out.first = cf_interp|cf_subst|cf_depr|cf_adj|cf_adja|cf_adjp
							|cf_adv|cf_num|cf_ppron12|cf_ppron3|cf_siebie
							|cf_fin|cf_bedzie|cf_aglt|cf_praet|cf_impt|cf_imps
							|cf_inf|cf_pcon|cf_pant|cf_ger|cf_pact|cf_ppas
							|cf_winien|cf_pred|cf_prep|cf_conj|cf_qub|cf_xxs|cf_xxx|cf_ign
							|cf_numcol
							|cf_tnum|cf_tsym|cf_turi|cf_tmail|cf_tdate|cf_ttime|cf_twf;
				break;
			case CToken::acm:
				out.second = (DLONG)gcm_akomod;
				break;
			case CToken::acn:
				out.second = (DLONG)gcm_akcent;
				break;
			case CToken::agg:
				out.second = (DLONG)gcm_agluta;
				break;
			case CToken::asp:
				out.second = (DLONG)gcm_aspekt;
				break;
			case CToken::cas:
				out.second = (DLONG)gcm_przypadek;
				break;
			case CToken::deg:
				out.second = (DLONG)gcm_stopien;
				break;
			case CToken::gnd:
				out.second = (DLONG)gcm_rodzaj;
				break;
			case CToken::inp:
				out.second = (DLONG)gcm_interp;
				break;
			case CToken::neg:
				out.second = (DLONG)gcm_negacja;
				break;
			case CToken::nmb:
				out.second = (DLONG)gcm_liczba;
				break;
			case CToken::per:
				out.second = (DLONG)gcm_osoba;
				break;
			case CToken::ppr:
				out.second = (DLONG)gcm_poprzyim;
				break;
			case CToken::vcl:
				out.second = (DLONG)gcm_wok;
				break;
			case CToken::tnt:
				out.second = (DLONG)gcm_numtype;
				break;
			default:
				throw tagexception("Wrong argument.");
		}
	}

	STATIC const int CTag::GetAttribIndex(std::string attrib)
	{
		const char * name = attrib.c_str();

		if (!stricmp(name,"flex"))
			return CToken::flex;
		if (!stricmp(name,"orth"))
			return CToken::orth;
		if (!stricmp(name,"base"))
			return CToken::base;
		if (!stricmp(name,"nmb"))
			return CToken::nmb;
		if (!stricmp(name,"cas"))
			return CToken::cas;
		if (!stricmp(name,"gnd"))
			return CToken::gnd;
		if (!stricmp(name,"per"))
			return CToken::per;
		if (!stricmp(name,"deg"))
			return CToken::deg;
		if (!stricmp(name,"asp"))
			return CToken::asp;
		if (!stricmp(name,"neg"))
			return CToken::neg;
		if (!stricmp(name,"acm"))
			return CToken::acm;
		if (!stricmp(name,"acn"))
			return CToken::acn;
		if (!stricmp(name,"ppr"))
			return CToken::ppr;
		if (!stricmp(name,"agg"))
			return CToken::agg;
		if (!stricmp(name,"vcl"))
			return CToken::vcl;
		if (!stricmp(name,"inp"))
			return CToken::inp;
		if (!stricmp(name,"tnt"))
			return CToken::tnt;

		return -1;
	}

	const DLONG CTag::GetAttribValue(int attIndex) const
	{
		switch(attIndex)
		{
		case CToken::flex:
			// Part-of-speech
			return GetFlex();
		case CToken::nmb:
			// Number
			return GetNmb();
		case CToken::cas:
			// Case
			return GetCas();
		case CToken::gnd:
			// Gender
			return GetGnd();
		case CToken::per:
			// Person
			return GetPer();
		case CToken::deg:
			// Degree
			return GetDeg();
		case CToken::asp:
			// Aspect
			return GetAsp();
		case CToken::neg:
			// Negation
			return GetNeg();
		case CToken::acm:
			// Accommodability
			return GetAcm();
		case CToken::acn:
			// Accentability
			return GetAcn();
		case CToken::ppr:
			// Post - prepositionality
			return GetPpr();
		case CToken::agg:
			// Agglutination
			return GetAgg();
		case CToken::vcl:
			// Vocalicity
			return GetVcl();
		case CToken::inp:
			// Inp? interp?
			return GetInp();
		case CToken::tnt:
			// Number literal type
			return GetTnt();
		default:
			throw tagexception("Invalid attribute index.");
		}
	}

	STATIC const void CTag::GetAttribNames(int attribute, std::stringstream& out)
	{
		switch(attribute)
		{
		case CToken::flex:
			// Part of speech
			CTag::GetFlexNames(out);
			break;
		case CToken::nmb:
			// Number
			CTag::GetNmbNames(out);
			break;
		case CToken::cas:
			// Case
			CTag::GetCasNames(out);
			break;
		case CToken::gnd:
			// Gender
			CTag::GetGndNames(out);
			break;
		case CToken::per:
			// Person
			CTag::GetPerNames(out);
			break;
		case CToken::deg:
			// Degree
			CTag::GetDegNames(out);
			break;
		case CToken::asp:
			// Aspect
			CTag::GetAspNames(out);
			break;
		case CToken::neg:
			// Negation
			CTag::GetNegNames(out);
			break;
		case CToken::acm:
			// Accommodability
			CTag::GetAcmNames(out);
			break;
		case CToken::acn:
			// Accentability
			CTag::GetAcnNames(out);
			break;
		case CToken::ppr:
			// Post - prepositionality
			CTag::GetPprNames(out);
			break;
		case CToken::agg:
			// Agglutination
			CTag::GetAggNames(out);
			break;
		case CToken::vcl:
			// Vocalicity
			CTag::GetVclNames(out);
			break;
		case CToken::inp:
			// Inp: interp?
			CTag::GetInpNames(out);
			break;
		case CToken::tnt:
			// Number literal type
			CTag::GetTntNames(out);
		default:
			throw tagexception("Invalid attribute index.");
		}
	}

	STATIC const void CTag::GetAttribNames(int attribute, std::stringstream& out,
										const CValue& mask)
	{
		switch(attribute)
		{
		case CToken::flex:
			// Part of speech
			CTag::GetFlexNames(out);
			break;
		case CToken::nmb:
			// Number
			CTag::GetNmbNames(out,mask);
			break;
		case CToken::cas:
			// Case
			CTag::GetCasNames(out,mask);
			break;
		case CToken::gnd:
			// Gender
			CTag::GetGndNames(out,mask);
			break;
		case CToken::per:
			// Person
			CTag::GetPerNames(out,mask);
			break;
		case CToken::deg:
			// Degree
			CTag::GetDegNames(out,mask);
			break;
		case CToken::asp:
			// Aspect
			CTag::GetAspNames(out,mask);
			break;
		case CToken::neg:
			// Negation
			CTag::GetNegNames(out,mask);
			break;
		case CToken::acm:
			// Accommodability
			CTag::GetAcmNames(out,mask);
			break;
		case CToken::acn:
			// Accentability
			CTag::GetAcnNames(out,mask);
			break;
		case CToken::ppr:
			// Post - prepositionality
			CTag::GetPprNames(out,mask);
			break;
		case CToken::agg:
			// Agglutination
			CTag::GetAggNames(out,mask);
			break;
		case CToken::vcl:
			// Vocalicity
			CTag::GetVclNames(out,mask);
			break;
		case CToken::inp:
			// Inp:interp?
			CTag::GetInpNames(out,mask);
			break;
		case CToken::tnt:
			// Number literal type
			CTag::GetTntNames(out,mask);
			break;
		default:
			std::stringstream msg;
			msg << "CTag::GetAttribNames/3 Invalid attribute index: ";
			msg << attribute;
			throw tagexception(msg.str().c_str());
		}
	}

	STATIC const std::string CTag::GetAttribString(int index)
	{
		switch(index)
		{
		case CToken::flex:
			// Part of speech
			return std::string("flex");
		case CToken::orth:
			// Orth
			return std::string("orth");
		case CToken::base:
			// Base
			return std::string("base");
		case CToken::nmb:
			// Number
			return std::string("nmb");
		case CToken::cas:
			// Case
			return std::string("cas");
		case CToken::gnd:
			// Gender
			return std::string("gnd");
		case CToken::per:
			// Person
			return std::string("per");
		case CToken::deg:
			// Degree
			return std::string("deg");
		case CToken::asp:
			// Aspect
			return std::string("asp");
		case CToken::neg:
			// Negation
			return std::string("neg");
		case CToken::acm:
			// Accommodability
			return std::string("acm");
		case CToken::acn:
			// Accentability
			return std::string("acn");
		case CToken::ppr:
			// Post - prepositionality
			return std::string("ppr");
		case CToken::agg:
			// Agglutination
			return std::string("agg");
		case CToken::vcl:
			// Vocalicity
			return std::string("vcl");
		case CToken::inp:
			// Inp?
			return std::string("inp");
		case CToken::tnt:
			// Number literal type
			return std::string("tnt");
		default:
			throw tagexception("Invalid attribute index.");
		}
	}

	void CTag::SetFlexClass(const std::string & flexClassName)
	{
		const char * name = flexClassName.c_str();
		if(!stricmp(name, "interp")) { FlexClass = cf_interp; return; }
		if(!stricmp(name, "subst")) { FlexClass = cf_subst; return; }
		if(!stricmp(name, "depr")) { FlexClass = cf_depr; return; }
		if(!stricmp(name, "adj")) { FlexClass = cf_adj; return; }
		if(!stricmp(name, "adja")) { FlexClass = cf_adja; return; }
		if(!stricmp(name, "adjp")) { FlexClass = cf_adjp; return; }
		if(!stricmp(name, "adv")) { FlexClass = cf_adv; return; }
		if(!stricmp(name, "num")) { FlexClass = cf_num; return; }
		if(!stricmp(name, "ppron12")) { FlexClass = cf_ppron12; return; }
		if(!stricmp(name, "ppron3")) { FlexClass = cf_ppron3; return; }
		if(!stricmp(name, "siebie")) { FlexClass = cf_siebie; return; }
		if(!stricmp(name, "fin")) { FlexClass = cf_fin; return; }
		if(!stricmp(name, "bedzie")) { FlexClass = cf_bedzie; return; }
		if(!stricmp(name, "aglt")) { FlexClass = cf_aglt; return; }
		if(!stricmp(name, "praet")) { FlexClass = cf_praet; return; }
		if(!stricmp(name, "impt")) { FlexClass = cf_impt; return; }
		if(!stricmp(name, "imps")) { FlexClass = cf_imps; return; }
		if(!stricmp(name, "inf")) { FlexClass = cf_inf; return; }
		if(!stricmp(name, "pcon")) { FlexClass = cf_pcon; return; }
		if(!stricmp(name, "pant")) { FlexClass = cf_pant; return; }
		if(!stricmp(name, "ger")) { FlexClass = cf_ger; return; }
		if(!stricmp(name, "pact")) { FlexClass = cf_pact; return; }
		if(!stricmp(name, "ppas")) { FlexClass = cf_ppas; return; }
		if(!stricmp(name, "winien")) { FlexClass = cf_winien; return; }
		if(!stricmp(name, "pred")) { FlexClass = cf_pred; return; }
		if(!stricmp(name, "prep")) { FlexClass = cf_prep; return; }
		if(!stricmp(name, "conj")) { FlexClass = cf_conj; return; }
		if(!stricmp(name, "qub")) { FlexClass = cf_qub; return; }
		if(!stricmp(name, "xxs")) { FlexClass = cf_xxs; return; }
		if(!stricmp(name, "xxx")) { FlexClass = cf_xxx; return; }
		if(!stricmp(name, "ign")) { FlexClass = cf_ign; return; }
		
		if(!stricmp(name, "numcol")) { FlexClass = cf_numcol; return; }

		if(!stricmp(name, "tnum")) { FlexClass = cf_tnum; return; }
		if(!stricmp(name, "tsym")) { FlexClass = cf_tsym; return; }
		if(!stricmp(name, "turi")) { FlexClass = cf_turi; return; }
		if(!stricmp(name, "tmail")) { FlexClass = cf_tmail; return; }
		if(!stricmp(name, "tdate")) { FlexClass = cf_tdate; return; }
		if(!stricmp(name, "ttime")) { FlexClass = cf_ttime; return; }
		if(!stricmp(name, "twf")) { FlexClass = cf_twf; return; }

		throw tagexception(std::string("Nierozpoznana klasa fleksyjna: ").append(name).c_str());

	}

	STATIC char CTag::toStrBuf[toStrBufSize];

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetLiczbaS() const
	{
		if(FlexClass & mask_FlexHasLiczba)
			switch(Categories & gcm_liczba)
		{
			case gc_sg: return "sg";
			case gc_pl: return "pl";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetNmb() const
	{
		if(FlexClass & mask_FlexHasLiczba)
			switch(Categories & gcm_liczba)
		{
			case gc_sg: return (DLONG)gc_sg;
			case gc_pl: return (DLONG)gc_pl;
		}

		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetNmb(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "sg")) { return gc_sg;}
		if(!stricmp(name, "pl")) { return gc_pl;}

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetNmbNames(std::stringstream& out)
	{
		out << "0,"<<gc_sg<< "," <<gc_pl;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetNmbNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_sg)
			out << "\tsg";
		if(mask.second & gc_pl)
			out << "\tpl";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetPrzypadekS() const
	{
		if(FlexClass & mask_FlexHasPrzypadek)
			switch(Categories & gcm_przypadek)
		{
			case gc_nom: return "nom";
			case gc_gen: return "gen";
			case gc_dat: return "dat";
			case gc_acc: return "acc";
			case gc_inst: return "inst";
			case gc_loc: return "loc";
			case gc_voc: return "voc";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetCas() const
	{
		if(FlexClass & mask_FlexHasPrzypadek)
			switch(Categories & gcm_przypadek)
		{
			case gc_nom: return (DLONG)gc_nom;
			case gc_gen: return (DLONG)gc_gen;
			case gc_dat: return (DLONG)gc_dat;
			case gc_acc: return (DLONG)gc_acc;
			case gc_inst: return (DLONG)gc_inst;
			case gc_loc: return (DLONG)gc_loc;
			case gc_voc: return (DLONG)gc_voc;
		}

		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetCas(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "nom")) { return gc_nom;}
		if(!stricmp(name, "gen")) { return gc_gen;}
		if(!stricmp(name, "dat")) { return gc_dat;}
		if(!stricmp(name, "acc")) { return gc_acc;}
		if(!stricmp(name, "inst")) { return gc_inst; }
		if(!stricmp(name, "loc")) { return gc_loc;}
		if(!stricmp(name, "voc")) { return gc_voc;}

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetCasNames(std::stringstream& out)
	{
		out << "0,"<<gc_nom<< "," <<gc_gen<< ","
			<<gc_dat<< "," <<gc_acc<< "," <<gc_inst<< "," <<gc_loc<< "," <<gc_voc;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetCasNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_nom)
			out << "\tnom";
		if(mask.second & gc_gen)
			out << "\tgen";
		if(mask.second & gc_dat)
			out << "\tdat";
		if(mask.second & gc_acc)
			out << "\tacc";
		if(mask.second & gc_inst)
			out << "\tinst";
		if(mask.second & gc_loc)
			out << "\tloc";
		if(mask.second & gc_voc)
			out << "\tvoc";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetRodzajS() const
	{
		if(FlexClass & mask_FlexHasRodzaj)
			switch(Categories & gcm_rodzaj)
		{
			case gc_m1: return "m1";
			case gc_m2: return "m2";
			case gc_m3: return "m3";
			case gc_f: return "f";
			case gc_n: return "n";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetGnd() const
	{
		if(FlexClass & mask_FlexHasRodzaj)
			switch(Categories & gcm_rodzaj)
		{
			case gc_m1: return (DLONG)gc_m1;
			case gc_m2: return (DLONG)gc_m2;
			case gc_m3: return (DLONG)gc_m3;
			case gc_f: return  gc_f;
			case gc_n: return  gc_n;
		}

		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetGnd(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "m1")) { return gc_m1;}
		if(!stricmp(name, "m2")) { return gc_m2;}
		if(!stricmp(name, "m3")) { return gc_m3;}
		if(!stricmp(name, "f")) { return gc_f;}
		if(!stricmp(name, "n")) { return gc_n;}

		return gc_none;
	}


	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetGndNames(std::stringstream& out)
	{
		out << "0,"<<gc_m1<< "," <<gc_m2<< "," <<gc_m3<< "," <<gc_f<< "," <<gc_n;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetGndNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_m1)
			out << "\tm1";
		if(mask.second & gc_m2)
			out << "\tm2";
		if(mask.second & gc_m3)
			out << "\tm3";
		if(mask.second & gc_f)
			out << "\tf";
		if(mask.second & gc_n)
			out << "\tn";
	}


	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetOsobaS() const
	{
		if(FlexClass & mask_FlexHasOsoba)
			switch(Categories & gcm_osoba)
		{
			case gc_pri: return "pri";
			case gc_sec: return "sec";
			case gc_ter: return "ter";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetPer() const
	{
		if(FlexClass & mask_FlexHasOsoba)
			switch(Categories & gcm_osoba)
		{
			case gc_pri: return (DLONG)gc_pri;
			case gc_sec: return (DLONG)gc_sec;
			case gc_ter: return (DLONG)gc_ter;
		}

		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetPer(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "pri")) { return gc_pri;}
		if(!stricmp(name, "sec")) { return gc_sec;}
		if(!stricmp(name, "ter")) { return gc_ter;}

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetPerNames(std::stringstream& out)
	{
		out << "0,"<<gc_pri<< "," <<gc_sec<< "," <<gc_ter;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetPerNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_pri)
			out << "\tpri";
		if(mask.second & gc_sec)
			out << "\tsec";
		if(mask.second & gc_ter)
			out << "\tter";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetStopienS() const
	{
		if(FlexClass & mask_FlexHasStopien)
			switch(Categories & gcm_stopien)
		{
			case gc_pos: return "pos";
			case gc_comp: return "comp";
			case gc_sup: return "sup";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetDeg() const
	{
		if(FlexClass & mask_FlexHasStopien)
			switch(Categories & gcm_stopien)
		{
			case gc_pos: return (DLONG)gc_pos;
			case gc_comp: return (DLONG)gc_comp;
			case gc_sup: return (DLONG)gc_sup;
		}
		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetDeg(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "pos")) { return gc_pos;}
		if(!stricmp(name, "comp")) { return gc_comp;}
		if(!stricmp(name, "sup")) { return gc_sup;}

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetDegNames(std::stringstream& out)
	{
		out << "0,"<<gc_pos<< "," <<gc_comp<< "," <<gc_sup;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetDegNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_pos)
			out << "\tpos";
		if(mask.second & gc_comp)
			out << "\tcomp";
		if(mask.second & gc_sup)
			out << "\tsup";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetAspektS() const
	{
		if(FlexClass & mask_FlexHasAspekt)
			switch(Categories & gcm_aspekt)
		{
			case gc_imperf: return "imperf";
			case gc_perf: return "perf";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetAsp() const
	{
		if(FlexClass & mask_FlexHasAspekt)
			switch(Categories & gcm_aspekt)
		{
			case gc_imperf: return (DLONG)gc_imperf;
			case gc_perf: return (DLONG)gc_perf;
		}
		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetAsp(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "imperf")) { return gc_imperf; }
		if(!stricmp(name, "perf")) { return gc_perf; }

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetAspNames(std::stringstream& out)
	{
		out << "0,"<<gc_imperf<< "," <<gc_perf;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetAspNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_imperf)
			out << "\timperf";
		if(mask.second & gc_perf)
			out << "\tperf";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetNegacjaS() const
	{
		if(FlexClass & mask_FlexHasNegacja)
			switch(Categories & gcm_negacja)
		{
			case gc_aff: return "aff";
			case gc_neg: return "neg";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetNeg() const
	{
		if(FlexClass & mask_FlexHasNegacja)
			switch(Categories & gcm_negacja)
		{
			case gc_aff: return (DLONG)gc_aff;
			case gc_neg: return (DLONG)gc_neg;
		}

		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetNeg(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "aff")) { return gc_aff;}
		if(!stricmp(name, "neg")) { return gc_neg;}

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetNegNames(std::stringstream& out)
	{
		out << "0,"<<gc_aff<< "," <<gc_neg;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetNegNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_aff)
			out << "\taff";
		if(mask.second & gc_neg)
			out << "\tneg";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetAkcentS() const
	{
		if(FlexClass & mask_FlexHasAkcent)
			switch(Categories & gcm_akcent)
		{
			case gc_akc: return "akc";
			case gc_nakc: return "nakc";
			case gc_oakc: return "oakc";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetAcn() const
	{
		if(FlexClass & mask_FlexHasAkcent)
			switch(Categories & gcm_akcent)
		{
			case gc_akc: return (DLONG)gc_akc;
			case gc_nakc: return (DLONG)gc_nakc;
			case gc_oakc: return (DLONG)gc_oakc;
		}

		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetAcn(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "akc")) { return gc_akc; }
		if(!stricmp(name, "nakc")) { return gc_nakc; }
		if(!stricmp(name, "oakc")) { return gc_oakc; }

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetAcnNames(std::stringstream& out)
	{
		out << "0,"<<gc_akc<< "," <<gc_nakc<< "," <<gc_oakc;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetAcnNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_akc)
			out << "\takc";
		if(mask.second & gc_nakc)
			out << "\tnakc";
		if(mask.second & gc_oakc)
			out << "\toakc";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetPoprzyimS() const
	{
		if(FlexClass & mask_FlexHasPoprzyim)
			switch(Categories & gcm_poprzyim)
		{
			case gc_praep: return "praep";
			case gc_npraep: return "npraep";
			case gc_opraep: return "opraep";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetPpr() const
	{
		if(FlexClass & mask_FlexHasPoprzyim)
			switch(Categories & gcm_poprzyim)
		{
			case gc_praep: return (DLONG)gc_praep;
			case gc_npraep: return (DLONG)gc_npraep;
			case gc_opraep: return (DLONG)gc_opraep;
		}

		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetPpr(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "praep")) { return gc_praep;}
		if(!stricmp(name, "npraep")) { return gc_npraep;}
		if(!stricmp(name, "opraep")) { return gc_opraep;}

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetPprNames(std::stringstream& out)
	{
		out << "0,"<<gc_praep<< "," <<gc_npraep<< "," <<gc_opraep;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetPprNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_praep)
			out << "\tpraep";
		if(mask.second & gc_npraep)
			out << "\tnpraep";
		if(mask.second & gc_opraep)
			out << "\topraep";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetAglutaS() const
	{
		if(FlexClass & mask_FlexHasAgluta)
			switch(Categories & gcm_agluta)
		{
			case gc_nagl: return "nagl";
			case gc_agl: return "agl";
			case gc_oagl: return "oagl";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetAgg() const
	{
		if(FlexClass & mask_FlexHasAgluta)
			switch(Categories & gcm_agluta)
		{
			case gc_nagl: return (DLONG)gc_nagl;
			case gc_agl: return (DLONG)gc_agl;
			case gc_oagl: return (DLONG)gc_oagl;
		}
		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetAgg(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "nagl")) { return gc_nagl; }
		if(!stricmp(name, "agl")) { return gc_agl; }
		if(!stricmp(name, "oagl")) { return gc_oagl; }

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetAggNames(std::stringstream& out)
	{
		out << "0,"<<gc_nagl<< "," <<gc_agl<< "," <<gc_oagl;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetAggNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_nagl)
			out << "\tnagl";
		if(mask.second & gc_agl)
			out << "\tagl";
		if(mask.second & gc_oagl)
			out << "\toagl";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetWokalS() const
	{
		if(FlexClass & mask_FlexHasWokal)
			switch(Categories & gcm_wok)
		{
			case gc_wok: return "wok";
			case gc_nwok: return "nwok";
			case gc_owok: return "owok";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetVcl() const
	{
		if(FlexClass & mask_FlexHasWokal)
			switch(Categories & gcm_wok)
		{
			case gc_wok: return (DLONG)gc_wok;
			case gc_nwok: return (DLONG)gc_nwok;
			case gc_owok: return (DLONG)gc_owok;
		}

		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetVcl(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "wok")) { return gc_wok;}
		if(!stricmp(name, "nwok")) { return gc_nwok;}
		if(!stricmp(name, "owok")) { return gc_owok;}

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetVclNames(std::stringstream& out)
	{
		out << "0,"<<gc_wok<< "," <<gc_nwok<< "," <<gc_owok;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetVclNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_wok)
			out << "\tpraep";
		if(mask.second & gc_nwok)
			out << "\tnpraep";
		if(mask.second & gc_owok)
			out << "\topraep";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetAkomodS() const
	{
		if(FlexClass & mask_FlexHasAkomod)
			switch(Categories & gcm_akomod)
		{
			case gc_congr: return "congr";
			case gc_rec: return "rec";
			case gc_oacmd: return "oacmd";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetAcm() const
	{
		if(FlexClass & mask_FlexHasAkomod)
			switch(Categories & gcm_akomod)
		{
			case gc_congr: return (DLONG)gc_congr;
			case gc_rec: return (DLONG)gc_rec;
			case gc_oacmd: return (DLONG)gc_oacmd;
		}
		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetAcm(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "congr")) { return gc_congr; }
		if(!stricmp(name, "rec")) { return gc_rec; }
		if(!stricmp(name, "oacmd")) { return gc_oacmd; }

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetAcmNames(std::stringstream& out)
	{
		out  << "0,"<<gc_congr<< "," <<gc_rec<< "," <<gc_oacmd;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetAcmNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_congr)
			out << "\tcongr";
		if(mask.second & gc_rec)
			out << "\trec";
		if(mask.second & gc_oacmd)
			out << "\toacmd";
	}

	//pobiera kategorie zgodna z nazwa metody albo '-', jesli klasa nie posiada tej kategorii
	const char* CTag::GetInterpS() const
	{
		if(FlexClass & mask_FlexHasInterp)
			switch(Categories & gcm_interp)
		{
			case gc_nb: return "nb";
			case gc_sp: return "sp";
			case gc_na: return "na";
			case gc_ointer: return "ointer";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetInp() const
	{
		if(FlexClass && mask_FlexHasInterp)
			switch(Categories & gcm_interp)
		{
			case gc_nb: return (DLONG)gc_nb;
			case gc_sp: return (DLONG)gc_sp;
			case gc_na: return (DLONG)gc_na;
			case gc_ointer: return (DLONG)gc_ointer;
		}
		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetInp(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "nb")) { return gc_nb;}
		if(!stricmp(name, "sp")) { return gc_sp;}
		if(!stricmp(name, "na")) { return gc_na;}
		if(!stricmp(name, "ointer")) { return gc_ointer;}

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetInpNames(std::stringstream& out)
	{
		out << "0,"<<gc_nb<< "," <<gc_sp<< "," <<gc_na<< "," <<gc_ointer;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetInpNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_nb)
			out << "\tnb";
		if(mask.second & gc_sp)
			out << "\tsp";
		if(mask.second & gc_na)
			out << "\tna";
		if(mask.second & gc_ointer)
			out << "\tointer";
	}

	///////////////
	///////////////
	const char* CTag::GetNumtypeS() const
	{
		if(FlexClass & mask_FlexHasNumtype)
			switch(Categories & gcm_numtype)
		{
			case gc_integer: return "integer";
			case gc_frac: return "frac";
		}
		return "-";
	}

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const DLONG CTag::GetTnt() const
	{
		if(FlexClass & mask_FlexHasAspekt)
			switch(Categories & gcm_numtype)
		{
			case gc_integer: return (DLONG)gc_integer;
			case gc_frac: return (DLONG)gc_frac;
		}
		return 0;
	}

	//pobiera kategorie zgodna ze stringa, i zwraca jej wartoœæ binarn¹.
	const DLONG CTag::GetTnt(const std::string& stringCat)
	{
		const char * name = stringCat.c_str();
		if(!stricmp(name, "integer")) { return gc_integer; }
		if(!stricmp(name, "frac")) { return gc_frac; }

		return gc_none;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetTntNames(std::stringstream& out)
	{
		out << "0,"<<gc_integer<< "," <<gc_frac;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetTntNames(std::stringstream& out,const CValue& mask)
	{
		if(mask.second & gc_integer)
			out << "\tinteger";
		if(mask.second & gc_frac)
			out << "\tfrac";
	}
	///////////////

	//pobiera kategorie zgodna z nazwa metody albo 0, jesli klasa nie posiada tej kategorii
	const TFlexClass CTag::GetFlex() const
	{
		return FlexClass;
	}

	//wype³nia wektor mo¿liwymi wartoœciami danego attrybutu
	STATIC void CTag::GetFlexNames(std::stringstream& out)
	{
		out<< cf_interp<<",";
		out<< cf_subst<<",";
		out<< cf_depr<<",";
		out<< cf_adj<<",";
		out<< cf_adja<<",";
		out<< cf_adjp<<",";
		out<< cf_adv<<",";
		out<< cf_num<<",";
		out<< cf_ppron12<<",";
		out<< cf_ppron3<<",";
		out<< cf_siebie<<",";
		out<< cf_fin<<",";
		out<< cf_bedzie<<",";
		out<< cf_aglt<<",";
		out<< cf_praet<<",";
		out<< cf_impt<<",";
		out<< cf_imps<<",";
		out<< cf_inf<<",";
		out<< cf_pcon<<",";
		out<< cf_pant<<",";
		out<< cf_ger<<",";
		out<< cf_pact<<",";
		out<< cf_ppas<<",";
		out<< cf_winien<<",";
		out<< cf_pred<<",";
		out<< cf_prep<<",";
		out<< cf_conj<<",";
		out<< cf_qub<<",";
		out<< cf_xxs<<",";
		out<< cf_xxx<<",";
		out<< cf_ign<<",";
		// DONE: numcol+nowe
		out<< cf_numcol<<",";

		out<< cf_tnum<<",";
		out<< cf_tsym<<",";
		out<< cf_turi<<",";
		out<< cf_tmail<<",";
		out<< cf_tdate<<",";
		out<< cf_ttime<<",";
		out<< cf_twf;
	}

	CTag::~CTag(void)
	{
	}

	const DLONG CTag::GetGrammCatValue(const std::string& stringCat)
	{
		if(GetNmb(stringCat) != gc_none)
			return GetNmb(stringCat);
		if(GetCas(stringCat) != gc_none)
			return GetCas(stringCat);
		if(GetGnd(stringCat) != gc_none)
			return GetGnd(stringCat);
		if(GetPer(stringCat) != gc_none)
			return GetPer(stringCat);
		if(GetDeg(stringCat) != gc_none)
			return GetDeg(stringCat);
		if(GetAsp(stringCat) != gc_none)
			return GetAsp(stringCat);
		if(GetNeg(stringCat) != gc_none)
			return GetNeg(stringCat);
		if(GetAcn(stringCat) != gc_none)
			return GetAcn(stringCat);
		if(GetPpr(stringCat) != gc_none)
			return GetPpr(stringCat);
		if(GetAgg(stringCat) != gc_none)
			return GetAgg(stringCat);
		if(GetVcl(stringCat) != gc_none)
			return GetVcl(stringCat);
		if(GetAcm(stringCat) != gc_none)
			return GetAcm(stringCat);
		if(GetInp(stringCat) != gc_none)
			return GetInp(stringCat);
		if(GetTnt(stringCat) != gc_none)
			return GetTnt(stringCat);

		return gc_none;
	}
}
