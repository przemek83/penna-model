#include <vector>
#include <iostream>
#include <math.h>
#include <string.h>
#include <time.h>
// #include <sfmt.h>
#include <limits.h>
#include <random>

#include "Common.h"
#include "Individual.h"
#include "Output.h"

#ifdef SYMULACJA_DORSZY
float krok_symulacji =  // wielkosc kroku symulacji
    abs(START_ODLOWOW - KONIEC_ODLOWOW) / (float)SYMULACJI_NA_PROCES;
#endif

int numer_procesu = 0;              // numer procesu gdy zdefiniowane jest UZYJ_MPI (domyslnie 0)
int wielkosc_klastra = 1;           //wielkosc klastra, na ktorym symulacja jest liczona (domyslnie 1)

//tablice uzywane do zbierania danych z symulacji
static int rodziny[ZYC_START];
static int rozklad_wieku[WIELKOSC*INT_W];
static int rozklad_bitow[WIELKOSC*INT_W];
static int gompertz_zgony[WIELKOSC*INT_W]; 
static unsigned int osobniki[WIELKOSC*INT_W];

//tablice zbierajace dane z symulacji o srednich 
static float sr_gompertz[WIELKOSC*INT_W];
static float sr_rodziny[MAX_POP_LAT];
static float sr_bity[WIELKOSC*INT_W];
static float sr_wiek[WIELKOSC*INT_W];
static float sr_stat[MAX_POP_LAT][4];

//tablice uzywane przez MPI do zebrania srednich z wszystkich procesow
#ifdef UZYJ_MPI
#include "mpi.h"
static float sr_gompertz_final[WIELKOSC*INT_W];
static float sr_rodziny_final[MAX_POP_LAT];
static float sr_bity_final[WIELKOSC*INT_W];
static float sr_wiek_final[WIELKOSC*INT_W];
static float sr_stat_final[MAX_POP_LAT][4];
#else
//referencje do tablic przetrzymujacych srednie
float (& sr_gompertz_final)[WIELKOSC*INT_W] = sr_gompertz;
float (& sr_rodziny_final)[MAX_POP_LAT] = sr_rodziny;
float (& sr_bity_final)[WIELKOSC*INT_W] = sr_bity;
float (& sr_wiek_final)[WIELKOSC*INT_W] = sr_wiek;
float (& sr_stat_final)[MAX_POP_LAT][4] = sr_stat;
#endif

//zmienne przetrzymujace informacje potrzebne do statystyk
int ilosc_osobnikow;
int ilosc_narodzin;
int ilosc_rodzin;
int zgon;
//===============================================================//
void Randomize(void)
{
	srand((int) time(NULL));
}
//---------------------------------------------------------------//
int RandomInteger(int low, int high) 
{
	double d = (double) rand() / ((double) RAND_MAX + 1);
	int k = (int) (d * (high - low + 1));
	return (low + k);
}
//---------------------------------------------------------------//
namespace
{
Individual populacja[MAX_POP];
}  // namespace
//---------------------------------------------------------------//
void zerowanie()
{
	ilosc_osobnikow = 0;
	ilosc_narodzin = 0;
	ilosc_rodzin = 0;
	zgon = 0;

	for(int v = 0; v<ZYC_START; v++)
	rodziny[v]=0;
	for(int v = 0; v<WIELKOSC*INT_W; v++)
	{
		osobniki[v] = 0;
		rozklad_wieku[v] = 0;
		rozklad_bitow[v] = 0;
		gompertz_zgony[v] = 0; 
	}
}
//---------------------------------------------------------------//
int losuj_populacje(std::mt19937& rng, Output& wyjscie)
{
    unsigned int nowy[WIELKOSC];
    unsigned int liczba_losowa = 0;
    unsigned int ktore = 0;
    unsigned int temp = 0;
    unsigned int numer = 0;

    std::uniform_int_distribution<std::mt19937::result_type> randomNumber(0, INT_W - 1);    // distribution in range [1, 6]
    std::uniform_int_distribution<std::mt19937::result_type> randomWhich(0, WIELKOSC - 1);  // distribution in range [1, 6]

    for (int i = 0; i < ZYC_START; i++)
    {
        for (int x = 0; x < WIELKOSC; x++)
            nowy[x] = 0;
        for (int j = 0; j < JEDYNEK_START; j++)
        {
            do
            {
            liczba_losowa = randomNumber(rng);
            ktore = randomWhich(rng);
            temp = 1;
            temp <<= liczba_losowa;
            } while (nowy[ktore] == (nowy[ktore] | temp));

            nowy[ktore] = (nowy[ktore] | temp);
        }
        numer++;
        populacja[numer - 1].inicjuj(i, nowy);
        wyjscie.zapisz_losowana_populacje(populacja, numer);
    }
    return numer;
}
//---------------------------------------------------------------//
class rownolegle{
public:
    void start(int argc, char* argv[])
    {
#ifdef UZYJ_MPI
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &numer_procesu);
        MPI_Comm_size(MPI_COMM_WORLD, &wielkosc_klastra);
#endif
    }
    void rozeslij_ziarno(int* glowne_ziarno)
    {
#ifdef UZYJ_MPI
        MPI_Bcast( glowne_ziarno, 1, MPI_INT, 0, MPI_COMM_WORLD);
#endif
    }
    void zbierz_dane_z_procesow()
    {
#ifdef UZYJ_MPI
        MPI_Reduce(sr_gompertz, sr_gompertz_final, WIELKOSC * INT_W, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(sr_bity, sr_bity_final, WIELKOSC * INT_W, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(sr_wiek, sr_wiek_final, WIELKOSC * INT_W, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(sr_rodziny, sr_rodziny_final, MAX_POP_LAT, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(sr_stat, sr_stat_final, MAX_POP_LAT * 4, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
#endif
    }
    void koniec()
    {
#ifdef UZYJ_MPI
        MPI_Finalize();
#endif
    }
} mpi;
//===============================================================//
int main(int argc,char* argv[])
{
    mpi.start(argc, argv);

    int glowne_ziarno = 0;
    Output wyjscie(krok_symulacji, numer_procesu, MAX_POP_LAT);

    if (numer_procesu == 0)  // glowny
    {
        Randomize();
        glowne_ziarno = abs(RandomInteger(0, INT_MAX));
    }

    mpi.rozeslij_ziarno(&glowne_ziarno);

    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<std::mt19937::result_type> genOld(0, 100);
    std::uniform_int_distribution<std::mt19937::result_type> randomNumber(0, INT_W - 1);
    std::uniform_int_distribution<std::mt19937::result_type> randomWhich(0, WIELKOSC - 1);
    std::uniform_int_distribution<std::mt19937::result_type> tenK(0, 10000);
    std::uniform_int_distribution<std::mt19937::result_type> offsrpingGenerator(1, 100);

    std::vector<unsigned int> puste;                    // kontener z indeksami pustych miejsc w tablicy populacja[]

    for (int o = 1; o <= SYMULACJI_NA_PROCES + 1; o++)  // start kolejne symulacje
    {
        clock_t start, koniec;
        unsigned int rok = 0;   // aktualny rok w symulacji
        bool rodzina1 = false;  // flaga pokazujaca istnienie tylko jednej rodziny
        puste.clear();

        if (o == SYMULACJI_NA_PROCES + 1 && numer_procesu == 0)
            wyjscie.otworz_pliki(0);
        else
        {
            if (o != SYMULACJI_NA_PROCES + 1)
            wyjscie.otworz_pliki2(o);
        }

        if (o != SYMULACJI_NA_PROCES + 1)
        {
            unsigned int ostatni_el = losuj_populacje(rng, wyjscie);
            if (numer_procesu == 0)
            {
            printf("%d/%d Postep:       [                                                  ]", o, SYMULACJI_NA_PROCES);
            for (int k = 0; k <= 50; k++)
                printf("\b");
            start = clock();
            }
            while (rok < MAX_POP_LAT)  // kolejne lata
            {
            zerowanie();

#ifdef SYMULACJA_DORSZY
            if (rok + 1 == MAX_POP_LAT)
            {
                if (ostatni_el - puste.size() == 0)
                    return 0;
            }
#endif

            for (unsigned int i = 0; i < ostatni_el; i++)
            {
                if (populacja[i].przodek == -1)
                    continue;
                else
                    ilosc_osobnikow++;

                if (!rodzina1)  // gromadz dane o rodzinach
                {
                    rodziny[populacja[i].przodek]++;
                    if (rodziny[populacja[i].przodek] == 1)
                        ilosc_rodzin++;
                }

                if (rok + 1 == MAX_POP_LAT)  // zgromadz dane o
                                             // bitach i wieku
                {
                    rozklad_wieku[populacja[i].wiek]++;
                    for (int v = 0; v < WIELKOSC * INT_W; v++)
                        if (populacja[i].ciag[v / INT_W] & (1 << (INT_W - (v + 1) % INT_W)))
                            rozklad_bitow[v]++;
                }

                // decyzja o zyciu badz smierci osobnika
                if ((populacja[i].ilosc_1 >= MAX_JEDYNEK) ||                                      // jedynki
                    (populacja[i].wiek >= WIELKOSC * INT_W) ||                                    // starosc
                    ((float)genOld(rng) <= (float)(ostatni_el - puste.size()) / MAX_POP * 100.0)  // verhulst
#ifdef SYMULACJA_DORSZY
                    || ((rok > ODLOWY_OD) && (populacja[i].wiek >= MINIMALNY_WIEK) && ((float)tenK(rng) / 100 <= START_ODLOWOW + o * krok_symulacji))
#endif
                        )  // smierc
                {
                    zgon++;
                    gompertz_zgony[populacja[i].wiek]++;
                    puste.push_back(i);
                    populacja[i].przodek = -1;
                    continue;
                }
                else                                                                                                  // zycie
                {
                    if ((populacja[i].wiek > ROZMNAZANIE_OD_ROKU) && offsrpingGenerator(rng) <= SZANSA_NA_POTOMSTWO)  // potomstwo
                    {
                        Individual osobnik;
                        for (unsigned int l = 0; l < ILOSC_POTOMSTWA; l++)  // ile potomstwa
                        {
                            ilosc_narodzin++;
                            for (int x = 0; x < WIELKOSC; x++)            // przepisz genom
                                                                          // rodzica
                                osobnik.ciag[x] = populacja[i].ciag[x];
                            for (unsigned m = 0; m < ILOSC_MUTACJI; m++)  // mutacje
                            {
                                unsigned int liczba_losowa = randomNumber(rng);
                                unsigned int ktore = randomWhich(rng);
                                unsigned int temp = 1;
                                temp <<= liczba_losowa;
                                osobnik.ciag[ktore] = (osobnik.ciag[ktore] | temp);
                            }

                            if (puste.size() == 0 && ostatni_el < MAX_POP)  // dodaj
                                                                            // osobnika
                                                                            // na
                                                                            // nowym
                                                                            // miejscu
                            {
                                populacja[ostatni_el].inicjuj(populacja[i].przodek, osobnik.ciag);
                                ostatni_el++;
                            }
                            else  // dodaj osobnika w
                                  // miejsce puste
                            {
                                if (puste.size() > 0)
                                {
                                    populacja[puste.back()].inicjuj(populacja[i].przodek, osobnik.ciag);
                                    puste.pop_back();
                                }
                            }
                        }
                    }
                }
                populacja[i].wiek++;                   // dodaj rok do wieku
                populacja[i].czy1(populacja[i].wiek);  // sprawdzanie chorob
            }
            if (ilosc_rodzin == 1)
                rodzina1 = true;
            wyjscie.zapisz_kolejne(rodzina1, rok, sr_gompertz, sr_rodziny, sr_bity, sr_wiek, sr_stat, ilosc_osobnikow, ilosc_narodzin, ilosc_rodzin, zgon,
                                   rozklad_wieku, rozklad_bitow, gompertz_zgony);
            rok++;
            if (numer_procesu == 0 && (rok % (MAX_POP_LAT / 50)) == 0)
                printf("*");  // progress bar
            }                 // kolejne lata
            wyjscie.zapisz_koncowa_populacje(populacja, SYMULACJI_NA_PROCES + 1 - o, ostatni_el);
        }
        else
        {
            mpi.zbierz_dane_z_procesow();
            if (numer_procesu == 0)
            {
            wyjscie.zapisz_srednie(SYMULACJI_NA_PROCES * wielkosc_klastra, sr_gompertz_final, sr_rodziny_final, sr_bity_final, sr_wiek_final, sr_stat_final);
            wyjscie.zamknij_pliki(SYMULACJI_NA_PROCES + 1 - o);
            }
        }
        if (numer_procesu == 0)
        {
            koniec = clock();
            if (o != SYMULACJI_NA_PROCES + 1)
            printf(
                "\nCzas wykonania: %d godzin, %d minut %d "
                "sekund\n",
                (koniec - start) / (1000 * 60 * 60), ((koniec - start) % (1000 * 60 * 60)) / (1000 * 60),
                (((koniec - start) % (1000 * 60 * 60)) % (1000 * 60)) / 1000);
        }
    }  // kolejne symulacje
       //        delete generator;
    mpi.koniec();
    return 0;
}
//============================================================//
