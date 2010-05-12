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

#ifndef CORPUSLI_TYPEDEFS_H
#define CORPUSLI_TYPEDEFS_H

#include "../corpuslib_headers.h"

namespace Corpus
{
	// WINDOWS
#ifndef WIN32
	typedef char _TCHAR; //vLINUX
#include <inttypes.h>
	typedef uint8_t UBYTE;
	typedef int32_t INTEGER;
	typedef uint32_t UINTEGER;
	typedef int64_t DLONG;
	typedef uint16_t USHORT;
#else
	typedef unsigned __int8 UBYTE;
	typedef __int32 INTEGER;
	typedef unsigned __int32 UINTEGER;
	typedef __int64 DLONG;
	typedef unsigned __int16 USHORT;
#endif

	typedef std::vector <std::string>	STRING_VECTOR;
	typedef std::map <std::string,INTEGER>	STRING2INT_MAP;
	typedef std::pair<std::string,INTEGER>	STRING_INT_PAIR;

	//to tak tylko dla przypomnienia w pliku cpp, ze cos jest statyczne
#define STATIC

	/// Do trzymania info o wartoœciach atrybutów klasy fleksyjnej.
	typedef DLONG TCategories;

	/// Reprezentuje maskê klas fleksyjnych (jedna lub wiele naraz).
	typedef DLONG TFlexClass;

	const DLONG negTree_False = 1ll;
	const DLONG negTree_True = 2ll;

	enum TestValues
	{
		tv_undefined	= 0x0,
		tv_true			= 0x1,
		tv_false		= 0x2
	};
	
	// Definicje masek bitowych klas fleksyjnych (POS itp.)

	const DLONG cf_none		= 0x00000000ll; // brak
	const DLONG cf_interp	= 0x00000001ll; // znak interpunkcyjny
	const DLONG cf_subst	= 0x00000002ll; // rzeczownik
	const DLONG cf_depr		= 0x00000004ll; // rzecz w formie deprecjatywnej
	const DLONG cf_adj		= 0x00000008ll; // przymiotnik
	const DLONG cf_adja		= 0x00000010ll; // przym przyprzym
	const DLONG cf_adjp		= 0x00000020ll; // przym poprzyim
	const DLONG cf_adv		= 0x00000040ll; // przys stopniowalny i/lub odprzym
	const DLONG cf_num		= 0x00000080ll; // liczebnik g³ówny
	const DLONG cf_ppron12	= 0x00000100ll; // zaimek os ON dla 1. i 2. os.
	const DLONG cf_ppron3	= 0x00000200ll; // zaimek os ON dla 3. os
	const DLONG cf_siebie	= 0x00000400ll; // zaimek siebie
	const DLONG cf_fin		= 0x00000800ll; // forma nieprzesza
	const DLONG cf_bedzie	= 0x00001000ll; // forma przysza BYï¿½
	const DLONG cf_aglt		= 0x00002000ll; // aglutynant BYï¿½
	const DLONG cf_praet	= 0x00004000ll; // pseudoimiesï¿½
	const DLONG cf_impt		= 0x00008000ll; // rozkanik
	const DLONG cf_imps		= 0x00010000ll; // bezosobnik
	const DLONG cf_inf		= 0x00020000ll; // bezokolicznik
	const DLONG cf_pcon		= 0x00040000ll; // imiesï¿½ przysï¿½kowy wspï¿½zesny
	const DLONG cf_pant		= 0x00080000ll; // imiesï¿½ przysï¿½kowy uprzedni
	const DLONG cf_ger		= 0x00100000ll; // odsownik
	const DLONG cf_pact		= 0x00200000ll; // imiesï¿½ przym. czynny
	const DLONG cf_ppas		= 0x00400000ll; // imiesï¿½ przym. bierny
	const DLONG cf_winien	= 0x00800000ll; // forma teraniejsza typu winien
	const DLONG cf_pred		= 0x01000000ll; // predykatyw
	const DLONG cf_prep		= 0x02000000ll; // przyimek
	const DLONG cf_conj		= 0x04000000ll; // spï¿½nik
	const DLONG cf_qub		= 0x08000000ll; // kublik
	const DLONG cf_xxs		= 0x10000000ll; // ciao obce rzecz.
	const DLONG cf_xxx		= 0x20000000ll; // ciao obce nierzecz.
	const DLONG cf_ign		= 0x40000000ll; // ignorowany
	const DLONG cf_numcol	= 0x80000000ll; // liczebnik zbiorowy
	// nowe klasy fleksyjne rozszerzaj¹ce standard KIPI
	// u¿ywane przez CTokeniser
	const DLONG cf_twf		= 0x0100000000ll; // forma wyr. do dalszej klasyf
	const DLONG cf_tnum		= 0x0200000000ll; // liczba explicite (np. 12,5)
	const DLONG cf_tsym		= 0x0400000000ll; // symbol (chyba np. a12)
	const DLONG cf_tdate	= 0x0800000000ll; // data (np. 12.09.2007)
	const DLONG cf_ttime	= 0x1000000000ll; // godzina (np. 23:47)
	const DLONG cf_turi		= 0x2000000000ll; // URI (np. http://korpus.pl)
	const DLONG cf_tmail	= 0x4000000000ll; // adres e-mail

	const DLONG last_cf		= cf_tmail; // tu wstawiaæ ostatni¹ klasê

#define	str_msk	0x8000000000000000ull // maska dla stringa

	// Definicje masek bitowych dla wartoœci kategorii gramatycznych
	// (== wartoœci atrybutów).
#define gc_none   0x00000000ull
	//przypadek - 3 bity pierwsze
	// & 0x00000007
#define gc_nom    0x00000001ull
#define gc_gen    0x00000002ull
#define gc_dat    0x00000004ull
#define gc_acc    0x00000008ull
#define gc_inst   0x00000010ull
#define gc_loc    0x00000020ull
#define gc_voc    0x00000040ull

#define gcm_przypadek	0x000000007Full

	//liczba   - 2 kolejne	//5 bitï¿½
	// & 0x00000018
#define gc_sg     0x00000080ull
#define gc_pl     0x00000100ull

#define gcm_liczba		0x00000000180ull

	//rodzaj   - 3 kolejne	//8 bitï¿½
	// & 0x000000E0
#define gc_m1     0x00000200ull
#define gc_m2     0x00000400ull
#define gc_m3     0x00000800ull
#define gc_f      0x00001000ull
#define gc_n      0x00002000ull

#define gcm_rodzaj		0x00000003E00ull

	//wokalicznoï¿½- nie koliduje z powyzszym te same 3
	// & 0x000000E0
#define gc_wok    0x00004000ull
#define gc_nwok   0x00008000ull
#define gc_owok   0x00010000ull

#define gcm_wok			0x0000001C000ull

	//osoba   - 2 kolejne	//10 bitï¿½
	// & 0x00000300
#define gc_pri    0x00020000ull
#define gc_sec    0x00040000ull
#define gc_ter    0x00080000ull

#define gcm_osoba		0x000000E0000ull

	//stopieï¿½- 2 te same
	// & 0x00000300
#define gc_pos    0x00100000ull
#define gc_comp   0x00200000ull
#define gc_sup    0x00400000ull

#define gcm_stopien		0x00000700000ull

	//negacja  - 2 te same
	// & 0x00000300
#define gc_aff    0x00800000ull
#define gc_neg    0x01000000ull

#define gcm_negacja		0x00001800000ull

	//typ interp - 2 te same + omit  0
	// & 0x00000300
#define gc_nb     0x02000000ull
#define gc_sp     0x04000000ull
#define gc_na     0x08000000ull
#define gc_ointer 0x10000000ull

#define gcm_interp		0x0001E000000ull

	//akomodacyjnoï¿½- 2 te same + 0
	// & 0x00000300
#define gc_congr  0x20000000ull
#define gc_rec    0x40000000ull
#define gc_oacmd  0x80000000ull
	//omieniete

#define gcm_akomod		0x000E0000000

	//aglutacyjnosc - 2 te same
#define gc_nagl   0x100000000ull
#define gc_agl    0x200000000ull
#define gc_oagl   0x400000000ull //omieniete

#define gcm_agluta	0x00700000000ull

	//aspekt - 2 kolejne	//12 bitï¿½
	// & 0x00000C00
#define gc_imperf 0x0800000000ull
#define gc_perf   0x1000000000ull

#define gcm_aspekt		0x01800000000ull

	//akcentowoï¿½- 2 te same
	// & 0x00000C00
#define gc_akc    0x2000000000ull
#define gc_nakc   0x4000000000ull
#define gc_oakc   0x8000000000ull //omieniete

#define gcm_akcent		0x0E000000000ull

	//poprzyimkowosï¿½- 2 kolejne//14 bitï¿½
	// & 0x00003000
#define gc_praep  0x10000000000ull
#define gc_npraep 0x20000000000ull
#define gc_opraep 0x40000000000ull //ominiete

#define gcm_poprzyim	0x70000000000ull

	// typ liczby: ca³k/u³amek dziesiêtny
#define gc_integer  0x100000000000ull
#define gc_frac     0x200000000000ull
	
#define gcm_numtype 0x300000000000ull
	
	
	// Maski bitowe dla grup klas fleksyjnych.
	const DLONG mask_FlexNeedsMorphAnalysis =
		{
			cf_ign | cf_twf
		};
	const DLONG mask_FlexNoCategoriable =
		(
		cf_none| cf_adja | cf_adjp | cf_pred
		| cf_conj | cf_xxx  | cf_ign
		| cf_twf | cf_tsym | cf_tdate | cf_ttime | cf_turi | cf_tmail
		);
	
	const DLONG mask_FlexWith1stNumtype = cf_tnum;
	const DLONG mask_FlexWith1stLiczba =
		(
		cf_subst| cf_depr  | cf_adj   | cf_num  | cf_ppron12| cf_ppron3
		| cf_fin  | cf_bedzie| cf_aglt  | cf_praet| cf_impt | cf_ger
		| cf_pact | cf_ppas  | cf_winien| cf_xxs | cf_numcol
		);
	const DLONG mask_FlexWith1stAspekt =
		(
		cf_imps |  cf_inf  | cf_pcon  | cf_pant
		);
	const DLONG mask_FlexWith1stPrzypadek =
		(
		cf_siebie | cf_prep
		);

	const DLONG mask_FlexWith2ndPrzypadek =
		(
		cf_subst | cf_depr | cf_adj | cf_num | cf_ppron12 | cf_ppron3
		| cf_ger  | cf_pact| cf_ppas| cf_xxs | cf_numcol
		);

	const DLONG mask_FlexWith2ndRodzaj =
		(
		cf_praet | cf_winien
		);
	const DLONG mask_FlexWith2ndOsoba =
		(
		cf_fin   | cf_bedzie | cf_aglt | cf_impt
		);

	const DLONG mask_FlexWith3rdRodzaj =
		(
		cf_subst | cf_depr | cf_adj | cf_num | cf_ppron12
		| cf_ppron3| cf_ger  | cf_pact| cf_ppas| cf_xxs | cf_numcol
		);

	const DLONG mask_FlexWith3rdAspekt =
		(
		cf_fin | cf_bedzie | cf_aglt | cf_praet | cf_impt | cf_winien
		);

	const DLONG mask_FlexWith4thOsoba =
		(
		cf_ppron12 | cf_ppron3
		);

	const DLONG mask_FlexWith4thAspekt =
		(
		cf_ger | cf_pact | cf_ppas
		);

	const DLONG mask_FlexWith5thNegacja =
		(
		cf_ger | cf_pact | cf_ppas
		);

	const DLONG mask_FlexWithNo2nd =
		(
		cf_interp | cf_adja | cf_adjp | cf_adv | cf_siebie
		| cf_imps	| cf_inf  | cf_pcon | cf_pant| cf_pred
		| cf_conj   | cf_qub  | cf_xxx  | cf_ign
		| cf_twf | cf_tsym | cf_tdate | cf_ttime | cf_turi | cf_tnum | cf_tmail
		);


	const DLONG mask_FlexWithNo3rd =
		(
		mask_FlexWithNo2nd | cf_prep
		);

	/* Klasy num i numcol nie maj¹ wartoœci akomodacyjnoœæ. Gdyby by³a, by³aby
	 * na czwartej pozycji. Jest nawet zdefiniowana wczeœniej jako gcm_akomod,
	 * ale nie jest nigdzie u¿ywane. Klasy te nie nale¿¹ te¿ do definicji
	 * mask_FlexWith4th ... Byæ mo¿e jest to celowe -- jeœli bêdzie podana
	 * wartoœæ akomodacyjnoœci, zostanie i nie bêdzie wyj¹tku (choæ nie
	 * zostanie uwzglêdniona). Zachowanie to nie jest sprawdzone */
	const DLONG mask_FlexWithNo4th =
		(
		mask_FlexWithNo3rd
		| cf_subst | cf_depr | cf_fin
		| cf_bedzie  | cf_impt | cf_winien | cf_xxs
		);

	const DLONG mask_FlexWithNo5th =
		(
		mask_FlexWithNo4th | cf_adj | cf_num | cf_aglt | cf_praet | cf_numcol
		);

	const DLONG mask_FlexWithNo6th =
		(
		mask_FlexWithNo5th | cf_ppron12 |cf_ger | cf_pact | cf_ppas
		);

	const DLONG mask_FlexHasNumtype = cf_tnum;
	const DLONG mask_FlexHasLiczba =
		(
		cf_subst | cf_depr | cf_adj    | cf_num  | cf_ppron12
		| cf_ppron3| cf_fin  | cf_bedzie | cf_aglt | cf_praet
		| cf_impt  | cf_ger  | cf_pact   | cf_ppas | cf_winien
		| cf_xxs | cf_numcol
		);

	const DLONG mask_FlexHasPrzypadek =
		(
		cf_subst  | cf_depr  | cf_adj   | cf_num
		| cf_ppron12| cf_ppron3| cf_siebie| cf_ger
		| cf_pact   | cf_ppas  | cf_prep  | cf_xxs | cf_numcol
		);

	const DLONG mask_FlexHasRodzaj =
		(
		cf_subst  | cf_depr  | cf_adj   | cf_num
		| cf_ppron12| cf_ppron3| cf_praet | cf_ger
		| cf_pact   | cf_ppas  | cf_winien| cf_xxs | cf_numcol
		);

	const DLONG mask_FlexHasOsoba =
		(
		cf_ppron12 | cf_ppron3 | cf_fin
		| cf_bedzie  | cf_aglt   | cf_impt

		);
	const DLONG mask_FlexHasStopien =
		(
		cf_adj | cf_adv
		);
	const DLONG mask_FlexHasAspekt =
		(
		cf_fin  | cf_bedzie| cf_aglt | cf_praet| cf_impt
		| cf_imps | cf_inf   | cf_pcon | cf_pant | cf_ger
		| cf_pact | cf_ppas  | cf_winien

		);
	const DLONG mask_FlexHasNegacja =
		(
		cf_ger | cf_pact | cf_ppas
		);
	const DLONG mask_FlexHasAkcent =
		(
		cf_ppron12 | cf_ppron3
		);
	const DLONG mask_FlexHasPoprzyim =
		(
		cf_ppron3
		);
	const DLONG mask_FlexHasAgluta =
		(
		cf_praet
		);
	const DLONG mask_FlexHasWokal =
		(
		cf_aglt | cf_prep | cf_qub
		);
	const DLONG mask_FlexHasAkomod =
		(
		cf_num
		);
	const DLONG mask_FlexHasInterp =
		(
		cf_interp
		);

	const DLONG mask_Verb = (cf_fin | cf_impt | cf_imps |cf_inf | cf_praet);

	const DLONG mask_All = 0xFFFFFFFFFFFFFFFFll;
} // namespace Corpus

#endif

