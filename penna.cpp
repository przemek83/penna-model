#include <vector>
#include <iostream>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sfmt.h>
#include <limits.h>

#define CALE_WYJSCIE             //zapisz dane o kazdej z symulacji
#define UZYJ_MPI                 //uzyj MPI
#define UZYJ_ATI_STREAM          //uzyj ATI STREAM
#define SYMULACJA_DORSZY         //wlacz tryb symulacji odlowow

#ifdef  SYMULACJA_DORSZY
#undef  UZYJ_MPI
#endif
#define WIELKOSC 2               //*32 bity
#define INT_W 32                 //wielkosc integera
#define MAX_POP 1500000          //maksymalna wielkosc populacji
#define MAX_POP_LAT 150000       //maksymalna dlugosc symulacji
#define ZYC_START 300000         //zyc na starcie
#define ILOSC_MUTACJI 1          //ilosc mutacji przy rozmnazaniu
#define MAX_JEDYNEK 4            //ilosc 1 po ktorych osobnik umiera
#define JEDYNEK_START 4          //losc 1 w genomie osobnika na starcie symulacji 
#define ROZMNAZANIE_OD_ROKU 10   //rozmnazanie od roku
#define SZANSA_NA_POTOMSTWO 100  //z jakim prawd osobnik wyda potomstwo (w %)
#define ILOSC_POTOMSTWA 3        //ilosc potmostwa
#define SYMULACJI_NA_PROCES 25   //ilosc symulacji, ktore ma zrobic jeden proces
#ifdef  SYMULACJA_DORSZY
#ifndef CALE_WYJSCIE
#define CALE_WYJSCIE             //wlacz wyjscie gdy symulowane sa odlowy
#endif
#define ODLOWY_OD 50000          //rok, od ktorego rozpoczynaja sie odlowy
#define MINIMALNY_WIEK 4         //minimalny wiek odlawianych ryb
#define START_ODLOWOW 50         //startowa wielkosc odlowow (w %)
#define KONIEC_ODLOWOW 54        //koncowa wielkosc odlowow (w %)
float krok_symulacji =           //wielkosc kroku symulacji 
abs(START_ODLOWOW - KONIEC_ODLOWOW)/(float)SYMULACJI_NA_PROCES;
#endif

int numer_procesu = 0;              //numer procesu gdy zdefiniowane jest UZYJ_MPI (domyslnie 0)
int wielkosc_klastra = 1;           //wielkosc klastra, na ktorym symulacja jest liczona (domyslnie 1)

#ifdef UZYJ_ATI_STREAM
#include "brookgenfiles/podziel.h"
#endif

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

//nazwy plikow wyjscia
std::string nazwy_plikow[6] = { "statystyki.txt",
	"osobniki.txt",
	"rozklad_wieku.txt",
	"rozklad_bitow.txt",
	"gompertz.txt",
	"rodziny.txt" };

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
void przelicz_srednie_konwencjonalnie(float dzielnik)
{
	for(int v = 0; v < WIELKOSC*INT_W; v++)
	{
		sr_gompertz_final[v] = sr_gompertz_final[v]/dzielnik;
		sr_bity_final[v] = sr_bity_final[v]/dzielnik;
		sr_wiek_final[v] = sr_wiek_final[v]/dzielnik;
	}

	for(int v = 0; v < MAX_POP_LAT; v++)
	sr_rodziny_final[v] = sr_rodziny_final[v]/dzielnik;

	for(int v = 0; v < MAX_POP_LAT; v++)
	for(int w = 0; w < 4; w++)
	{
		sr_stat_final[v][w] = sr_stat_final[v][w]/dzielnik;
	}
}
//---------------------------------------------------------------//
#ifdef UZYJ_ATI_STREAM
int przelicz(float tab[], int rozmiar, float dzielnik)
{
	unsigned int RozmiarStrumienia[] = {rozmiar, 1};
	unsigned int stopien = 1;
	brook::Stream<float> StrumienWejsciowy(stopien, RozmiarStrumienia);
	brook::Stream<float> StrumienWyjsciowy(stopien, RozmiarStrumienia);

	StrumienWejsciowy.read(tab);

	podziel(StrumienWejsciowy, dzielnik , StrumienWyjsciowy);

	StrumienWyjsciowy.write(tab);

	return 0;
}

int przelicz_srednie_uzywajac_ATI_STREAM(float dzielnik)
{
	przelicz(sr_gompertz_final,sizeof(sr_gompertz_final)/sizeof(sr_gompertz_final[0]), dzielnik);
	przelicz(sr_bity_final,sizeof(sr_bity_final)/sizeof(sr_bity_final[0]), dzielnik);
	przelicz(sr_wiek_final,sizeof(sr_wiek_final)/sizeof(sr_wiek_final[0]), dzielnik);
	przelicz(sr_rodziny_final,sizeof(sr_rodziny_final)/sizeof(sr_rodziny_final[0]), dzielnik);
	przelicz(sr_stat_final[0],sizeof(sr_stat_final)/sizeof(sr_stat_final[0])*4, dzielnik);

	return 0;
}
#endif
//---------------------------------------------------------------//
class dane {
public:     
	int wiek;
	int ilosc_1;
	int przodek;
	unsigned int ciag[WIELKOSC];

	void czy1(unsigned int miejsce)
	{
		unsigned long int pomoc = ciag[miejsce/INT_W];
		pomoc = pomoc >> (INT_W-(miejsce%INT_W));
		if( pomoc&1 ) 
			this->ilosc_1++;
	}

	void itob(FILE * plik)
	{
		char ciag_binarny[INT_W+1];
		for(int x = 0; x<WIELKOSC; x++)
		{
			_itoa(ciag[x], ciag_binarny, 2);
			for(int i = INT_W-strlen(ciag_binarny); i>0; i--)
			fprintf(plik,"0");
			fprintf(plik,"%s", ciag_binarny);
		}
		fprintf(plik,"\n");
	}

	void inicjuj(unsigned int a, unsigned int bity[WIELKOSC])
	{
		this->wiek = 0;
		this->ilosc_1 = 0;
		this->przodek = a;
		for(int i = 0; i<WIELKOSC; i++)
			this->ciag[i] = bity[i];     
	}
private:
};
//---------------------------------------------------------------//
static dane populacja[MAX_POP];
//---------------------------------------------------------------//
class pliki {
private:
	FILE * plik_statystyki;
	FILE * plik_osobniki;
	FILE * plik_rozklad_wieku;
	FILE * plik_rozklad_bitow;
	FILE * plik_gompertz;
	FILE * plik_rodziny;

	enum { STATYSTYKI = 0, POPULACJE, ROZKLAD_WIEKU, ROZKLAD_JEDYNEK, GOMPERTZ, RODZINY };

	std::string nazwa(int przedrostek, int numer)
	{
		std::string plik_nazwa;
		char bufor[10],bufor2[10];
		
		#ifdef SYMULACJA_DORSZY
		sprintf(bufor,"%3.2f",START_ODLOWOW + przedrostek * krok_symulacji);
		#else	
		_itoa(przedrostek,bufor,10);
		#endif
		_itoa(numer_procesu,bufor2,10);

		plik_nazwa.erase();
		#ifndef SYMULACJA_DORSZY
		plik_nazwa.append("proces");
		plik_nazwa.append(bufor2);
		plik_nazwa.append("_symulacja");
		#endif
		plik_nazwa.append(bufor);
		plik_nazwa.append("_");
		plik_nazwa.append(nazwy_plikow[numer]);

		return plik_nazwa;
	}
public:     
	void otworz_pliki(int przedrostek)
	{
		fopen_s (&plik_statystyki,nazwa(przedrostek,STATYSTYKI).data(),"w");
		if(przedrostek != 0)
		fopen_s(&plik_osobniki, nazwa(przedrostek,POPULACJE).data(),"w");
		fopen_s (&plik_rozklad_wieku, nazwa(przedrostek,ROZKLAD_WIEKU).data(),"w");
		fopen_s (&plik_rozklad_bitow, nazwa(przedrostek,ROZKLAD_JEDYNEK).data(),"w");
		fopen_s (&plik_gompertz, nazwa(przedrostek,GOMPERTZ).data(),"w");
		fopen_s (&plik_rodziny, nazwa(przedrostek,RODZINY).data(),"w");
	}

	void otworz_pliki2(int przedrostek)
	{
		#ifdef CALE_WYJSCIE
		wyjscie.otworz_pliki(przedrostek);
		#endif
	}

	void zamknij_pliki(int przedrostek)
	{
		if(przedrostek != 0)
		fclose(plik_osobniki);
		fclose(plik_statystyki);
		fclose(plik_rozklad_wieku);
		fclose(plik_rozklad_bitow);
		fclose(plik_gompertz);  
	}

	void zapisz_srednie(int symulacji)
	{
		#ifdef UZYJ_ATI_STREAM
		przelicz_srednie_uzywajac_ATI_STREAM((float)symulacji);
		#else
		przelicz_srednie_konwencjonalnie((float)symulacji);
		#endif

		for(int v = 0; v<MAX_POP_LAT; v++)
		{
			if(sr_rodziny_final[v]>1)
			fprintf(plik_rodziny,"%d\t%f\n",v,sr_rodziny_final[v]);
			fprintf(plik_statystyki, "%d\t%f\t%f\t%f\t%f\n",v,sr_stat_final[v][0],sr_stat_final[v][1],sr_stat_final[v][2],sr_stat_final[v][3]);
		}
		
		for(int v = 0; v<WIELKOSC*INT_W; v++)
		{
			fprintf(plik_rozklad_wieku,"%d\t%f\n",v,sr_wiek_final[v]);
			fprintf(plik_rozklad_bitow,"%d\t%.2f\n",v,sr_bity_final[v]);
			if(sr_gompertz_final[v]>0)
			fprintf(plik_gompertz,"%d\t%.3f\n",v,sr_gompertz_final[v]);
			else 
			fprintf(plik_gompertz,"%d\t1\n",v);
		}
	}

	void zapisz_kolejne(bool rodzina1,int rok)
	{
		if(!rodzina1)
		{
			#ifdef CALE_WYJSCIE
			fprintf(plik_rodziny,"%d\t%d\n",rok,ilosc_rodzin);
			#endif
			sr_rodziny[rok] += ilosc_rodzin;
		}
		#ifdef CALE_WYJSCIE
		fprintf(plik_statystyki,"%d\t%d\t%d\t%d\t%d\n",rok,ilosc_osobnikow,ilosc_narodzin,ilosc_osobnikow - zgon,zgon);
		#endif
		sr_stat[rok][0] += ilosc_osobnikow;
		sr_stat[rok][1] += ilosc_narodzin;
		sr_stat[rok][2] += ilosc_osobnikow - zgon;
		sr_stat[rok][3] += zgon;
		
		if(rok+1 == MAX_POP_LAT)
		{
			for(int v=0; v<WIELKOSC*INT_W; v++)
			{
				#ifdef CALE_WYJSCIE
				fprintf(plik_rozklad_wieku,"%d\t%d\n",v,rozklad_wieku[v]);
				#endif
				sr_wiek[v] += rozklad_wieku[v];
			}

			for(int v=0; v<WIELKOSC*INT_W; v++)
			{
				#ifdef CALE_WYJSCIE
				fprintf(plik_rozklad_bitow,"%d\t%.2f\n",v,rozklad_bitow[v]*1.0/ilosc_osobnikow);
				#endif
				sr_bity[v] += (float)rozklad_bitow[v]/(float)ilosc_osobnikow;
			}

			for(int v = 0; v<WIELKOSC*INT_W; v++)
			if(rozklad_wieku[v]>0)
			{
				#ifdef CALE_WYJSCIE
				fprintf(plik_gompertz,"%d\t%.3f\n",v,gompertz_zgony[v]*1.0/rozklad_wieku[v]);
				#endif
				sr_gompertz[v] += (float)gompertz_zgony[v]/(float)rozklad_wieku[v];
			}
			else 
			{
				#ifdef CALE_WYJSCIE
				fprintf(plik_gompertz,"%d\t1\n",v);
				#endif
				sr_gompertz[v]+=1;
			}
		}
	}

	void zapisz_losowana_populacje(int numer)
	{
		#ifdef CALE_WYJSCIE
		fprintf(plik_osobniki,"%u ",numer-1);
		populacja[numer-1].itob(plik_osobniki);
		#endif
	}

	void zapisz_koncowa_populacje(int x, unsigned int ostatni_el)
	{
		#ifdef CALE_WYJSCIE
		for(unsigned d = 0; d<ostatni_el; d++)
		if(populacja[d].przodek!=-1)
		{
			fprintf(plik_osobniki,"%u %d %d %d %u ",d,populacja[d].przodek,populacja[d].wiek,populacja[d].ilosc_1, populacja[d].ciag[0]);
			populacja[d].itob(plik_osobniki);
		}
		wyjscie.zamknij_pliki(x);
		#endif
	}
private:
} wyjscie;
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
int losuj_populacje(CRandomSFMT1* generator)
{
	unsigned int nowy[WIELKOSC];
	unsigned int liczba_losowa = 0;
	unsigned int ktore = 0;
	unsigned int temp = 0;
	unsigned int numer = 0;
	
	for(int i = 0; i<ZYC_START; i++)
	{
		for(int x = 0; x<WIELKOSC; x++)
			nowy[x] = 0;
		for(int j = 0; j<JEDYNEK_START; j++)
		{
			do
			{
				liczba_losowa = generator->IRandomX(0,INT_W-1);
				ktore = generator->IRandomX(0,WIELKOSC-1);
				temp = 1;
				temp <<= liczba_losowa;
			}while(nowy[ktore] == (nowy[ktore] | temp));

			nowy[ktore] = (nowy[ktore] | temp);
		}
		numer++;
		populacja[numer-1].inicjuj(i,nowy);
		wyjscie.zapisz_losowana_populacje(numer);
	} 
	return numer;
}
//---------------------------------------------------------------//
class rownolegle{
public:
	void start(int argc,char* argv[])
	{
		#ifdef UZYJ_MPI
		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &numer_procesu);
		MPI_Comm_size(MPI_COMM_WORLD, &wielkosc_klastra);
		#endif
	}
	void rozeslij_ziarno(int * glowne_ziarno )
	{
		#ifdef UZYJ_MPI
		MPI_Bcast( glowne_ziarno, 1, MPI_INT, 0, MPI_COMM_WORLD); 
		#endif
	}
	void zbierz_dane_z_procesow()
	{
		#ifdef UZYJ_MPI
		MPI_Reduce(sr_gompertz,sr_gompertz_final,WIELKOSC*INT_W,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
		MPI_Reduce(sr_bity,sr_bity_final,WIELKOSC*INT_W,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
		MPI_Reduce(sr_wiek,sr_wiek_final,WIELKOSC*INT_W,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
		MPI_Reduce(sr_rodziny,sr_rodziny_final,MAX_POP_LAT,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
		MPI_Reduce(sr_stat,sr_stat_final,MAX_POP_LAT*4,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
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
	mpi.start(argc,argv);

	int glowne_ziarno = 0;

	if (numer_procesu == 0) // glowny
	{
		Randomize();
		glowne_ziarno = abs(RandomInteger(0,INT_MAX));
	}

	mpi.rozeslij_ziarno(& glowne_ziarno);

	CRandomSFMT1 * generator = new CRandomSFMT1(glowne_ziarno);
	int seeds[2] = {glowne_ziarno,numer_procesu};
	generator->RandomInitByArray(seeds,2);

	std::vector < unsigned int > puste; //kontener z indeksami pustych miejsc w tablicy populacja[]

	for(int o = 1; o<=SYMULACJI_NA_PROCES+1; o++) //start kolejne symulacje
	{
		clock_t start, koniec;
		unsigned int rok = 0; //aktualny rok w symulacji
		bool rodzina1 = false; //flaga pokazujaca istnienie tylko jednej rodziny
		puste.clear();

		if(o == SYMULACJI_NA_PROCES+1 && numer_procesu == 0 )
		wyjscie.otworz_pliki(0);
		else
		{
			if(o != SYMULACJI_NA_PROCES+1)
			wyjscie.otworz_pliki2(o);
		}

		if(o != SYMULACJI_NA_PROCES+1)
		{         
			unsigned int ostatni_el = losuj_populacje(generator);
			if (numer_procesu == 0)
			{
				printf("%d/%d Postep:       [                                                  ]",o,SYMULACJI_NA_PROCES);
				for(int k = 0; k<=50; k++)
				printf("\b");
				start = clock();
			}
			while(rok < MAX_POP_LAT) //kolejne lata
			{
				zerowanie();

				#ifdef SYMULACJA_DORSZY
				if(rok+1 == MAX_POP_LAT)
				{
					if(ostatni_el-puste.size() == 0)
					return 0;
				}
				#endif

				for(unsigned int i = 0; i<ostatni_el; i++)
				{
					if(populacja[i].przodek == -1)
					continue;
					else 
					ilosc_osobnikow++;
					
					if(!rodzina1)//gromadz dane o rodzinach
					{
						rodziny[populacja[i].przodek]++;
						if(rodziny[populacja[i].przodek] == 1)
						ilosc_rodzin++;
					}
					
					if(rok+1 == MAX_POP_LAT) //zgromadz dane o bitach i wieku
					{
						rozklad_wieku[populacja[i].wiek]++;
						for(int v = 0;v<WIELKOSC*INT_W; v++)
							if(populacja[i].ciag[v/INT_W] & (1 << (INT_W-(v+1)%INT_W)))
						rozklad_bitow[v]++;
					}

					//decyzja o zyciu badz smierci osobnika
					if ((populacja[i].ilosc_1 >= MAX_JEDYNEK) || //jedynki
						(populacja[i].wiek >= WIELKOSC*INT_W) || //starosc
						((float)generator->IRandomX(0,100) <= (float)(ostatni_el-puste.size())/MAX_POP*100.0) //verhulst
						#ifdef SYMULACJA_DORSZY
						|| ( ( rok > ODLOWY_OD ) && ( populacja[i].wiek >= MINIMALNY_WIEK ) && 
							( (float)generator->IRandomX(0,10000)/100 <= START_ODLOWOW + o * krok_symulacji ) )
						#endif
						) //smierc
					{
						zgon++;  
						gompertz_zgony[populacja[i].wiek]++;
						puste.push_back(i);
						populacja[i].przodek = -1;
						continue;
					}
					else //zycie
					{
						if((populacja[i].wiek>ROZMNAZANIE_OD_ROKU) && generator->IRandomX(1,100) <= SZANSA_NA_POTOMSTWO) //potomstwo
						{
							dane osobnik;
							for(unsigned int l = 0; l<ILOSC_POTOMSTWA; l++) //ile potomstwa
							{ 
								ilosc_narodzin++;              
								for(int x = 0; x<WIELKOSC; x++) //przepisz genom rodzica
									osobnik.ciag[x] = populacja[i].ciag[x];
								for(unsigned m = 0; m<ILOSC_MUTACJI; m++) //mutacje
								{
									unsigned int liczba_losowa = generator->IRandomX(0,INT_W-1);
									unsigned int ktore = generator->IRandomX(0,WIELKOSC-1);
									unsigned int temp = 1;
									temp <<= liczba_losowa;
									osobnik.ciag[ktore] = (osobnik.ciag[ktore] | temp);
								}                              

								if(puste.size() == 0 && ostatni_el<MAX_POP) //dodaj osobnika na nowym miejscu
								{
									populacja[ostatni_el].inicjuj(populacja[i].przodek,osobnik.ciag);                 
									ostatni_el++;
								}
								else //dodaj osobnika w miejsce puste
								{  
									if(puste.size()>0)
									{
										populacja[puste.back()].inicjuj(populacja[i].przodek,osobnik.ciag);                
										puste.pop_back();  
									}
								}
							}
						}
					}
					populacja[i].wiek++;//dodaj rok do wieku
					populacja[i].czy1(populacja[i].wiek);//sprawdzanie chorob
				}
				if(ilosc_rodzin == 1)
					rodzina1 = true;
				wyjscie.zapisz_kolejne(rodzina1,rok);
				rok++;
				if (numer_procesu == 0 && (rok%(MAX_POP_LAT/50)) == 0)
					printf("*");//progress bar          
			} //kolejne lata
			wyjscie.zapisz_koncowa_populacje(SYMULACJI_NA_PROCES+1-o,ostatni_el);
		}
		else
		{
			mpi.zbierz_dane_z_procesow();
			if (numer_procesu == 0)
			{
				wyjscie.zapisz_srednie(SYMULACJI_NA_PROCES*wielkosc_klastra);
				wyjscie.zamknij_pliki(SYMULACJI_NA_PROCES+1-o);
			}
		}
		if (numer_procesu == 0)
		{
			koniec = clock();
			if(o != SYMULACJI_NA_PROCES+1)
				printf("\nCzas wykonania: %d godzin, %d minut %d sekund\n", (koniec - start) / (1000*60*60), ((koniec - start) % (1000*60*60)) / (1000*60), (((koniec - start) % (1000*60*60)) % (1000*60)) / 1000);
		}
	} // kolejne symulacje
	delete(generator);
	mpi.koniec();
	return 0;
}
//============================================================//