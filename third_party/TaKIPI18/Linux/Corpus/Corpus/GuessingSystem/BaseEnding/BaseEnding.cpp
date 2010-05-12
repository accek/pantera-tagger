#include "../../../corpuslib_headers.h"
#include "BaseEnding.h"
#include "BaseEndingDict.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace BaseEnding
		{
			const int histosize = 8;
			int cutOffsetSum = 0;
			int endingsAnalysed = 0;
			int histogram[histosize + 1];

			void ClearStats()
			{
				cutOffsetSum = 0;
				endingsAnalysed = 0;
				for(int i = 0; i < histosize + 1; i++)
					histogram[i] = 0;
			}

			void DumpStats()
			{
				std::cout << "\nStatystyki koncowek form bazowych\n"
					<< "\nPrzeanalizowanych koncowek: " << endingsAnalysed
					<< "\nSrednia dlugosc napotkanej koncowki: "
					<< ((double)cutOffsetSum / (double)endingsAnalysed)
					<< "\n\n";
				for(int i = 0; i < histosize; i++)
				{
					std::cout << "Koncowek o dlugosci " << i << ": " << histogram[i]
					<< ", " << ((double)histogram[i] / (double) endingsAnalysed) << "\n";
				}
				std::cout << "Dluzszych koncowek: " << histogram[histosize]
					<< ", " << ((double)histogram[histosize] / (double) endingsAnalysed) << "\n";
			}

			void UpdateHistogram(int cutOffset)
			{
				endingsAnalysed++;
				if(cutOffset < histosize)
				{
					if(cutOffset >= 0)
						histogram[cutOffset]++;
				}
				else
				{
					histogram[histosize]++;
				}
			}

			void FindBaseEnding(const char *base, const char *orth,
					unsigned char &cutOffset, unsigned int &baseEndingId, int prefixToSkip)
			{
				// pomijamy prefiks w orth (znaleziony przez PreProcess), np. "nie" w imies�owach
				orth += prefixToSkip;

				// pomijamy wsp�lny pocz�tek base i orth
				while(*base == *orth && *base != 0)
				{
					base++;
					orth++;
				}

				// w tym miejscu zaczyna si� ko�cowka base
				baseEndingId = BaseEndingDict::GetInstance()->Add(base);

				// liczymy dlugosc koncowki orth
				cutOffset = (unsigned char)strlen(orth);
				UpdateHistogram(cutOffset);
			}

			const std::string &GenerateBase(const char *orth, unsigned char cutOffset, unsigned int baseEndingId, int prefixToSkip)
			{
				// pomijamy prefix w orth (znaleziony przez Postprocess)
				orth += prefixToSkip;
				
				int baselen = ((int)strlen(orth)) - ((int)cutOffset);
				
				static std::string restored;
				
				if(baselen > 0)
				{
					/* Do debugowania... 
					std::cout << "### orth(" << orth << "), cutOffset(" << (int)cutOffset <<"), baseEndingId("
						<< baseEndingId << "), baseEnding(" << BaseEndingDict::GetInstance()->GetAt(baseEndingId)
						<< "), needsNeg(" << needsNeg << ")\n"; */
					
					// wstawiamy baselen pierwszych znak�w z orth + ko�c�wk� ze s�ownika
					restored = std::string(orth, baselen) + BaseEndingDict::GetInstance()->GetAt(baseEndingId);
				}
				else
				{
					// obcieliby�my wszystkie litery z ortha, wi�c nie robimy tego, a w zamian zwracamy base==orth`
					restored = orth;
					// TODO: prawdopodobnie nale�a�o by w og�le nie dodawa� tego leksemu
					// wtedy trzeba by pomin�� ten leksem, i je�li sko�czy si� na tym,
					// �e token nie b�dzie mia� �adnego leksemu, to doda� IGN.
					// Wymaga�oby to r�wnie� ustalania prawdopodobie�stw na ko�cu -- �eby dope�nia�y si� do 1.
				}

				return restored;
			}
			
			int PreProcess(TFlexClass &flexClass, TCategories &gramCat,
						   const char *base, const char *orth)
			{
				int prefixToSkip = 0;
				prefixToSkip = PreFixMarkedness(gramCat, gc_neg, gc_aff, "nie", 3, base, orth);
				if(prefixToSkip == 0)
					prefixToSkip = PreFixMarkedness(gramCat, gc_sup, gc_comp, "naj", 3, base, orth);
				return prefixToSkip;
			}
			
			int PostProcess(TFlexClass &flexClass, TCategories &gramCat,
							const char *orth)
			{
				int prefixToSkip;
				prefixToSkip = PostFixMarkedness(gramCat, gc_neg, gc_aff, "nie", 3, orth);
				if(prefixToSkip == 0)
					prefixToSkip = PostFixMarkedness(gramCat, gc_sup, gc_comp, "naj", 3, orth);
				return prefixToSkip;
			}
			
			int PreFixMarkedness(TCategories &gramCat, TCategories gc_marked, TCategories gc_unmarked,
								  const char *prefix, int prefixlen, const char *base, const char *orth)
			{
				// je�li atrybuty nie zawieraj� gc_unmarked (mog�cego wymaga� zmiany), wychodzimy
				if((gramCat & gc_marked) == 0)
					return 0;
				// je�li orth z morfeusza nie zaczyna si� prefiksem, wychodzimy
				if(strncmp(orth, prefix, prefixlen) != 0)
					return 0;
				// je�li i orth, i base zaczynaj si� prefiksem, wychodzimy
				if(strncmp(base, prefix, prefixlen) == 0)
					return 0;
				// warunki spe�nione, zmieniamy gc_marked na gc_unmarked
				gramCat = (gramCat | gc_unmarked) & ~gc_marked;
				// zwracamy liczb� znak�w do pomini�cia
				return prefixlen;
			}
			
			int PostFixMarkedness(TCategories &gramCat, TCategories gc_marked, TCategories gc_unmarked,
							const char *prefix, int prefixlen, const char *orth)
			{
				// je�li atrybuty nie zawieraj� gc_unmarked (mog�cego wymaga� zmiany), wychodzimy
				if((gramCat & gc_unmarked) == 0)
					return 0;
				// je�li zgadywany orth nie zaczyna si� prefiksem, wychodzimy
				if(strncmp(orth, prefix, prefixlen) != 0)
					return 0;
				// warunki spe�nione, zmieniamy gc_unmarked na gc_marked
				gramCat = (gramCat | gc_marked) & ~gc_unmarked;
				// zwracamy liczb� znak�w do pomini�cia
				return prefixlen;
			}
			
		}
	}
}
