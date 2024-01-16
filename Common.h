#pragma once

#include <cstdlib>

#define WIELKOSC 2               //*32 bity
#define INT_W 32                 // wielkosc integera

#define CALE_WYJSCIE             // zapisz dane o kazdej z symulacji
// #define SYMULACJA_DORSZY         // wlacz tryb symulacji odlowow

#ifdef SYMULACJA_DORSZY
#ifndef CALE_WYJSCIE
#define CALE_WYJSCIE       // wlacz wyjscie gdy symulowane sa odlowy
#endif
#define ODLOWY_OD 50000    // rok, od ktorego rozpoczynaja sie odlowy
#define MINIMALNY_WIEK 4   // minimalny wiek odlawianych ryb
#define START_ODLOWOW 50   // startowa wielkosc odlowow (w %)
#define KONIEC_ODLOWOW 54  // koncowa wielkosc odlowow (w %)
#endif
