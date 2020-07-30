
#pragma once

#define ILOSC_TABLIC 100
#define ROZMIAR_1 10000
#define ROZMIAR_2 50000
#define ROZMIAR_3 100000
#define ROZMIAR_4 500000
#define ROZMIAR_5 1000000

// funkcje inicjujace tablice danymi
void Inic_wszystkie_tablice_losowo();
void Inic_wszystkie_tablice_posortowane_czesciowo(float procent_posortowania);
void Inic_wszystkie_tablice_posortowane_malejaco();

//funkcje pomiarowe
void Wykonaj_pomiary_sortowanie_scalanie1();
void Wykonaj_pomiary_sortowanie_scalanie2();
void Wykonaj_pomiary_sortowanie_scalanie3(float procent_posortowania);

void Wykonaj_pomiary_sortowanie_quicksort1();
void Wykonaj_pomiary_sortowanie_quicksort2();
void Wykonaj_pomiary_sortowanie_quicksort3(float procent_posortowania);

void Wykonaj_pomiary_sortowanie_introsort1();
void Wykonaj_pomiary_sortowanie_introsort2();
void Wykonaj_pomiary_sortowanie_introsort3(float procent_posortowania);


// algorytmy sortujace
//void Scalanie_Sortowanie(int poczatek, int koniec, int *tablica, int ile);
void sortowanie_scalanie(int* tab, int lewy, int prawy);
void scalanie(int* tab, int lewy, int srodek, int prawy);
void QuickSort(int lewy, int prawy, int* tab);

// intro sort
int partycjonowanie(int* tab, int lewy, int prawy);
void wstawianie(int* tab, int n);
void tworzenie_kopca(int* tab, int index, int rozmiar);
void kopcowanie(int* tab, int rozmiar);
void Introspektywne(int* tab, int rozmiar, int M);
void hybrydowe(int* tab, int rozmiar);


// zapisywanie do pliku
void Zapisz_tablice_do_pliku(double tablica[], int ilosc_elementow, const char* napis);
void Zapisz_wiadomosc_do_pliku(const char* napis, float procent_posortowania = 0);