#pragma once

#include <cstdlib>

#define WIELKOSC 2               //*32 bity
#define INT_W 32                 // wielkosc integera

#define CALE_WYJSCIE             // zapisz dane o kazdej z symulacji
#define SYMULACJA_DORSZY         // wlacz tryb symulacji odlowow

#define MAX_POP 15000            // maksymalna wielkosc populacji
#define MAX_POP_LAT 1500         // maksymalna dlugosc symulacji
#define ZYC_START 3000           // zyc na starcie
#define ILOSC_MUTACJI 1          // ilosc mutacji przy rozmnazaniu
#define MAX_JEDYNEK 4            // ilosc 1 po ktorych osobnik umiera
#define JEDYNEK_START 4          // losc 1 w genomie osobnika na starcie symulacji
#define ROZMNAZANIE_OD_ROKU 10   // rozmnazanie od roku
#define SZANSA_NA_POTOMSTWO 100  // z jakim prawd osobnik wyda potomstwo (w %)
#define ILOSC_POTOMSTWA 3        // ilosc potmostwa
#define SYMULACJI_NA_PROCES 25   // ilosc symulacji, ktore ma zrobic jeden proces
#ifdef SYMULACJA_DORSZY
#ifndef CALE_WYJSCIE
#define CALE_WYJSCIE       // wlacz wyjscie gdy symulowane sa odlowy
#endif
#define ODLOWY_OD 50000    // rok, od ktorego rozpoczynaja sie odlowy
#define MINIMALNY_WIEK 4   // minimalny wiek odlawianych ryb
#define START_ODLOWOW 50   // startowa wielkosc odlowow (w %)
#define KONIEC_ODLOWOW 54  // koncowa wielkosc odlowow (w %)
#endif
