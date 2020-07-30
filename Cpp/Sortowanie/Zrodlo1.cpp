#include "Zrodlo1.h"
#include <iostream>
#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
using std::cout; using std::endl;


// tablice z danymi do sortowania
int tablice_10k[ILOSC_TABLIC][ROZMIAR_1];
int tablice_50k[ILOSC_TABLIC][ROZMIAR_2];
int tablice_100k[ILOSC_TABLIC][ROZMIAR_3];
int tablice_500k[ILOSC_TABLIC][ROZMIAR_4];
int tablice_1mln[ILOSC_TABLIC][ROZMIAR_5];


// tablice z pomiarami czasow algorytmow sortujacych 
double wyniki_10k[ILOSC_TABLIC] = { 0 };
double wyniki_50k[ILOSC_TABLIC] = { 0 };
double wyniki_100k[ILOSC_TABLIC] = { 0 };
double wyniki_500k[ILOSC_TABLIC] = { 0 };
double wyniki_1mln[ILOSC_TABLIC] = { 0 };

int pomocnicza[1000000]; // wykorzystanie tablicy statycznej zdefiniowej poza funkcja skraca czas sortowania scalanie dla 100k elementow z 10 sek do 10 ms.

// algorytmy wykorzystywane przez sortowanie hybrydowe
int partycjonowanie(int* tab, int lewy, int prawy)
{
	int i = (lewy + prawy) / 2;
	int piwot = tab[i];
	int temp = 0;
	tab[i] = tab[prawy];
	int j = lewy;

	for (i = lewy; i < prawy; i++)
	{
		if (tab[i] < piwot)
		{
			temp = tab[i];
			tab[i] = tab[j];
			tab[j] = temp;
			j++;
		}
	}
	tab[prawy] = tab[j];
	tab[j] = piwot;

	return j;

}

void wstawianie(int* tab, int n)
{
	for (int i = 1; i < n; i++)
	{
		int j = i;

		while (j > 0)
		{
			if (tab[j - 1] > tab[j])
			{
				tab[j - 1] ^= tab[j];
				tab[j] ^= tab[j - 1];
				tab[j - 1] ^= tab[j];
				--j;
			}
			else
			{
				break;
			}
		}
	}
}

void tworzenie_kopca(int* tab, int index, int rozmiar)
{
	int j;
	while (index <= rozmiar / 2)
	{
		j = 2 * index;
		if (j + 1 <= rozmiar && tab[j + 1] > tab[j])
			j = j + 1;
		if (tab[index] < tab[j])
		{
			int temp = tab[index];
			tab[index] = tab[j];
			tab[j] = temp;
		}
		else break;
		index = j;
	}
}

void kopcowanie(int* tab, int rozmiar)
{
	int i;

	for (i = rozmiar / 2; i > 0; --i)
		tworzenie_kopca(tab - 1, i, rozmiar);
	for (i = rozmiar - 1; i > 0; --i)
	{
		int temp = tab[0];
		tab[0] = tab[i];
		tab[i] = temp;
		tworzenie_kopca(tab - 1, 1, i);
	}

}

void Introspektywne(int* tab, int rozmiar, int M) // M - z³o¿onoœæ obliczeniowa
{
	int i;
	if (M == 0)
	{
		//cout<<"kopcowanie"<<endl;
		kopcowanie(tab, rozmiar);
		return;
	}

	i = partycjonowanie(tab, 0, rozmiar - 1);
	if (i > 9)
	{
		//cout<<"intro"<<endl;
		Introspektywne(tab, i, M - 1);
	}

	if (rozmiar - 1 - i > 9)
	{
		// cout<<"intro2"<<endl;
		Introspektywne(tab + i + 1, rozmiar - 1 - i, M - 1);
	}



}

void hybrydowe(int* tab, int rozmiar)
{
	Introspektywne(tab, rozmiar, (int)floor(2 * log2(rozmiar)));
	wstawianie(tab, rozmiar);
}



//scalenie posortowanych podtablic
void scalanie(int* tab, int lewy, int srodek, int prawy)
{
	int i, j;

	//zapisujemy lew¹ czêsæ podtablicy w tablicy pomocniczej
	for (i = srodek + 1; i > lewy; i--)
		pomocnicza[i - 1] = tab[i - 1];

	//zapisujemy praw¹ czêsæ podtablicy w tablicy pomocniczej
	for (j = srodek; j < prawy; j++)
		pomocnicza[prawy + srodek - j] = tab[j + 1];

	//scalenie dwóch podtablic pomocniczych i zapisanie ich 
	//we w³asciwej tablicy
	for (int k = lewy; k <= prawy; k++)
		if (pomocnicza[j] < pomocnicza[i])
			tab[k] = pomocnicza[j--];
		else
			tab[k] = pomocnicza[i++];
}

void sortowanie_scalanie(int* tab, int lewy, int prawy)
{
	//gdy mamy jeden element, to jest on juz posortowany
	if (prawy <= lewy) return;

	//znajdujemy srodek podtablicy
	int srodek = (prawy + lewy) / 2;

	//dzielimy tablice na czesc lewa i prawa
	sortowanie_scalanie(tab, lewy, srodek);
	sortowanie_scalanie(tab, srodek + 1, prawy);

	//scalamy dwie posortowane tablice
	scalanie(tab, lewy, srodek, prawy);
}



void QuickSort(int lewy, int rozmiar, int* tab)
{
	int prawy = rozmiar - 1;
	int k = 0;
	if (lewy < prawy)
	{
		k = partycjonowanie(tab, lewy, prawy);
		QuickSort(lewy, k - 1, tab);
		QuickSort(k + 1, prawy, tab);
	}

}

// funkcje inicjalizuj¹ce tablice z danymi losowymi wartoœciami
void Inic_wszystkie_tablice_losowo()
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		for (int a = 0; a < ROZMIAR_1; a++)
		{
			tablice_10k[i][a] = rand() % ROZMIAR_1;
		}
		for (int a = 0; a < ROZMIAR_2; a++)
		{
			tablice_50k[i][a] = rand() % ROZMIAR_2;
		}

		for (int a = 0; a < ROZMIAR_3; a++)
		{
			tablice_100k[i][a] = rand() % ROZMIAR_3;
		}

		for (int a = 0; a < ROZMIAR_4; a++)
		{
			tablice_500k[i][a] = rand() % ROZMIAR_4;
		}

		for (int a = 0; a < ROZMIAR_5; a++)
		{
			tablice_1mln[i][a] = rand() % ROZMIAR_5;
		}

	}
}

void Inic_wszystkie_tablice_posortowane_malejaco()
{
	Inic_wszystkie_tablice_losowo();

	// posortowanie tablic malejaco

	int* wsk;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_10k[i][0];
		std::sort(wsk, wsk + ROZMIAR_1, std::greater<int>());
	}

	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_50k[i][0];
		std::sort(wsk, wsk + ROZMIAR_2, std::greater<int>());
	}

	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_100k[i][0];
		std::sort(wsk, wsk + ROZMIAR_3, std::greater<int>());
	}

	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_500k[i][0];
		std::sort(wsk, wsk + ROZMIAR_4, std::greater<int>());
	}

	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_1mln[i][0];
		std::sort(wsk, wsk + ROZMIAR_5, std::greater<int>());
	}
}

void Inic_wszystkie_tablice_posortowane_czesciowo(float procent_posortowania)
{
	Inic_wszystkie_tablice_losowo();

	int* wsk; // wskaŸnik pomocniczny wskazuj¹cy na tablice


			  // czesciowe posortowanie tablic reszta losowa 
	int dokad_sortowac = (int)(procent_posortowania / 100) * ROZMIAR_1;
	for (unsigned int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_10k[i][0];
		std::sort(wsk, wsk + dokad_sortowac);
	}

	dokad_sortowac = (int)(procent_posortowania / 100) * ROZMIAR_2;
	for (unsigned int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_50k[i][0];
		std::sort(wsk, wsk + dokad_sortowac);
	}

	dokad_sortowac = (int)(procent_posortowania / 100) * ROZMIAR_3;
	for (unsigned int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_100k[i][0];
		std::sort(wsk, wsk + dokad_sortowac);
	}

	dokad_sortowac = (int)(procent_posortowania / 100) * ROZMIAR_4;
	for (unsigned int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_500k[i][0];
		std::sort(wsk, wsk + dokad_sortowac);
	}

	dokad_sortowac = (int)(procent_posortowania / 100) * ROZMIAR_5;
	for (unsigned int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_1mln[i][0];
		std::sort(wsk, wsk + dokad_sortowac);
	}
}




// funkcje wykonuj¹ce pomiary badanych algorytmów



void Wykonaj_pomiary_sortowanie_scalanie1()
{
	double czas = 0, suma_czasow = 0;
	clock_t start, stop;
	int* wsk; // wskaŸnik pomocniczny wskazuj¹cy na tablice
	cout << "Wariant 1.(LOSOWY)\t-Inicjalizuje wszystkie tablice losowymi wartosciami..." << endl;
	Inic_wszystkie_tablice_losowo();

	cout << "Wariant 1.(LOSOWY)\t-Rozpoczynam pomiar czasu wykonania algorytmu sortowania rosnaco, przez scalanie..." << endl << endl;

	// dla tablic10k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		czas = 0;
		wsk = &tablice_10k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_1 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_10k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	wyniki_10k;
	cout.precision(10);
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, przez scalanie dla tablicy " << ROZMIAR_1 << " liczb, wynosi: " << 1000.0 * suma_czasow / 10.0 << " mili sec." << endl;



	// dla tablic50k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_50k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_2 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_50k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, przez scalanie dla tablicy " << ROZMIAR_2 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic100k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_100k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_3 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_100k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, przez scalanie dla tablicy " << ROZMIAR_3 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic500k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_500k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_4 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_500k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, przez scalanie dla tablicy " << ROZMIAR_4 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	//dal tablic1mln
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_1mln[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_5 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_1mln[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, przez scalanie dla tablicy " << ROZMIAR_5 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	cout << "Wariant 1.(LOSOWY)\t-Zapisuje pomiary do pliku..." << endl;
	Zapisz_tablice_do_pliku(wyniki_10k, ROZMIAR_1, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_50k, ROZMIAR_2, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_100k, ROZMIAR_3, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_500k, ROZMIAR_4, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_1mln, ROZMIAR_5, "Sortowanie Scalanie");

	Zapisz_wiadomosc_do_pliku("Zakonczono Wariant 1.(LOSOWY) - Sortowanie rosnaco, przez scalanie");

	cout << "Zakonczono Scalanie Wariant 1.(LOSOWY) - Sortowanie rosnaco, przez scalanie" << endl << endl << endl;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wykonaj_pomiary_sortowanie_scalanie2()
{
	double czas = 0, suma_czasow = 0;
	clock_t start, stop;
	int* wsk; // wskaŸnik pomocniczny wskazuj¹cy na tablice

	cout << "Wariant 2.(NIEKORZYSTNY)\t-Inicjalizuje wszystkie tablice liczbami posortowanymi malejaco..." << endl;
	Inic_wszystkie_tablice_posortowane_malejaco();

	cout << "Wariant 2.(NIEKORZYSTNY)\t-Rozpoczynam pomiar czasu wykonania algorytmu Sortowania rosnaco, przez scalanie..." << endl << endl;

	// dla tablic10k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		czas = 0;
		wsk = &tablice_10k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_1 - 1);
		stop = clock();

		czas = (float)(stop - start) / CLOCKS_PER_SEC;
		wyniki_10k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, " << ROZMIAR_1 << " liczb posortowanych malejaco, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;



	// dla tablic50k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_50k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_2 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_50k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, " << ROZMIAR_2 << " liczb posortowanych malejaco, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic100k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_100k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_3 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_100k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, " << ROZMIAR_3 << " liczb posortowanych malejaco, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic500k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_500k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_4 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_500k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, " << ROZMIAR_4 << " liczb posortowanych malejaco, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic1mln
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_1mln[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_5 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_1mln[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, " << ROZMIAR_5 << " liczb posortowanych malejaco, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;
	
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Zapisuje pomiary do pliku..." << endl;
	Zapisz_tablice_do_pliku(wyniki_10k, ROZMIAR_1, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_50k, ROZMIAR_2, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_100k, ROZMIAR_3, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_500k, ROZMIAR_4, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_1mln, ROZMIAR_5, "Sortowanie Scalanie");

	Zapisz_wiadomosc_do_pliku("Zakonczono Wariant 2.(NIEKORZYSTNY) - Sortowanie rosnaco, przez scalanie");

	cout << "Zakonczono Scalanie Wariant 2.(NIEKORZYSTNY) - Sortowanie rosnaco, przez scalanie" << endl << endl << endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wykonaj_pomiary_sortowanie_scalanie3(float procent_posortowania)
{
	double czas = 0, suma_czasow = 0;
	clock_t start, stop;
	int* wsk; // wskaŸnik pomocniczny wskazuj¹cy na tablice

	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Inicjalizuje wszystkie tablice. " << procent_posortowania << "% posortowane rosnaco, reszta tablicy losowa." << endl;
	Inic_wszystkie_tablice_posortowane_czesciowo(procent_posortowania);

	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Rozpoczynam pomiar czasu wykonania algorytmu Sortowania przez scalanie..." << endl << endl;

	// dla tablic10k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		czas = 0;
		wsk = &tablice_10k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_1 - 1);
		stop = clock();

		czas = (float)(stop - start) / CLOCKS_PER_SEC;
		wyniki_10k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, " << ROZMIAR_1 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;



	// dla tablic50k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_50k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_2 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_50k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, " << ROZMIAR_2 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic100k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_100k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_3 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_100k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, " << ROZMIAR_3 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic500k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_500k[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_4 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_500k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, " << ROZMIAR_4 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic1mln
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_1mln[i][0];

		start = clock();
		sortowanie_scalanie(wsk, 0, ROZMIAR_5 - 1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_1mln[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, " << ROZMIAR_5 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;
	
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Zapisuje pomiary do pliku..." << endl;
	Zapisz_tablice_do_pliku(wyniki_10k, ROZMIAR_1, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_50k, ROZMIAR_2, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_100k, ROZMIAR_3, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_500k, ROZMIAR_4, "Sortowanie Scalanie");
	Zapisz_tablice_do_pliku(wyniki_1mln, ROZMIAR_5, "Sortowanie Scalanie");

	Zapisz_wiadomosc_do_pliku("Zakonczono Wariant 3 - Sortowanie rosnaco, przez scalanie", procent_posortowania);

	cout << "Zakonczono Scalanie Wariant 3.(" << procent_posortowania << "% KORZYSTNY) - Sortowanie rosnaco, przez scalanie" << endl << endl << endl;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wykonaj_pomiary_sortowanie_quicksort1()
{
	double czas = 0, suma_czasow = 0;
	clock_t start, stop;
	int* wsk; // wskaŸnik pomocniczny wskazuj¹cy na tablice

	cout << "Wariant 1.(LOSOWY)\t-Inicjalizuje wszystkie tablice losowymi wartosciami..." << endl;
	Inic_wszystkie_tablice_losowo();

	cout << "Wariant 1.(LOSOWY)\t-Rozpoczynam pomiar czasu wykonania algorytmu sortowania rosnaco, QuickSort..." << endl << endl;
	// dla tablic10k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		czas = 0;
		wsk = &tablice_10k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_1, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_10k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_1 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;



	// dla tablic50k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_50k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_2, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_50k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_2 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic100k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_100k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_3, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_100k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_3 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic500k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_500k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_4, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_500k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_4 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;\

	// dla tablic1mln
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_1mln[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_5, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_1mln[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_5 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;


	cout << "Wariant 1.(LOSOWY)\t-Zapisuje pomiary do pliku..." << endl;
	Zapisz_tablice_do_pliku(wyniki_10k, ROZMIAR_1, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_50k, ROZMIAR_2, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_100k, ROZMIAR_3, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_500k, ROZMIAR_4, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_1mln, ROZMIAR_5, "QuickSort");

	Zapisz_wiadomosc_do_pliku("Zakonczono Wariant 1.(LOSOWY) - QuickSort");

	cout << "Zakonczono QS Wariant 1.(LOSOWY) - QuickSort" << endl << endl << endl;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wykonaj_pomiary_sortowanie_quicksort2()
{
	double czas = 0, suma_czasow = 0;
	clock_t start, stop;
	int* wsk; // wskaŸnik pomocniczny wskazuj¹cy na tablice


	cout << "Wariant 2.(NIEKORZYSTNY)\t-Inicjalizuje wszystkie tablice liczbami posortowanymi malejaco..." << endl;
	Inic_wszystkie_tablice_posortowane_malejaco();

	cout << "Wariant 2.(NIEKORZYSTNY)\t-Rozpoczynam pomiar czasu wykonania algorytmu sortowania rosnaco, QuickSort..." << endl << endl;

	// dla tablic10k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_10k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_1, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_10k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_1 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;



	// dla tablic50k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_50k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_2, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_50k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_2 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic100k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_100k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_3, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_100k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_3 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic500k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_500k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_4, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_500k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_4 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic1mln
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_1mln[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_5, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_1mln[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_5 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;


	cout << "Wariant 2.(NIEKORZYSTNY)\t-Zapisuje pomiary do pliku..." << endl;
	Zapisz_tablice_do_pliku(wyniki_10k, ROZMIAR_1, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_50k, ROZMIAR_2, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_100k, ROZMIAR_3, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_500k, ROZMIAR_4, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_1mln, ROZMIAR_5, "QuickSort");

	Zapisz_wiadomosc_do_pliku("Zakonczono Wariant 2.(NIEKORZYSTNY) - QuickSort");

	cout << "Zakonczono QS Wariant 2.(NIEKORZYSTNY) - QuickSort" << endl << endl << endl;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wykonaj_pomiary_sortowanie_quicksort3(float procent_posortowania)
{
	double czas = 0, suma_czasow = 0;
	clock_t start, stop;
	int* wsk; // wskaŸnik pomocniczny wskazuj¹cy na tablice

	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Inicjalizuje wszystkie tablice. " << procent_posortowania << "% posortowane rosnaco, reszta tablicy losowa." << endl;
	Inic_wszystkie_tablice_posortowane_czesciowo(procent_posortowania);

	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Rozpoczynam pomiar czasu wykonania algorytmu sortowania rosnaco, QuickSort..." << endl << endl;


	// dla tablic10k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_10k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_1, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_10k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_1 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;



	// dla tablic50k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_50k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_2, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_50k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_2 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic100k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_100k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_3, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_100k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_3 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic500k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_500k[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_4, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_500k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}

	// dla tablic 1MLN
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_1mln[i][0];

		start = clock();
		QuickSort(0, 0 + ROZMIAR_5, wsk);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_1mln[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, QS dla tablicy " << ROZMIAR_5 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;


	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Zapisuje pomiary do pliku..." << endl;
	Zapisz_tablice_do_pliku(wyniki_10k, ROZMIAR_1, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_50k, ROZMIAR_2, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_100k, ROZMIAR_3, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_500k, ROZMIAR_4, "QuickSort");
	Zapisz_tablice_do_pliku(wyniki_1mln, ROZMIAR_5, "QuickSort");

	Zapisz_wiadomosc_do_pliku("Zakonczono Wariant 3 - QuickSort", procent_posortowania);

	cout << "Zakonczono QS Wariant 3.(" << procent_posortowania << "% KORZYSTNY) - QuickSort" << endl << endl << endl;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wykonaj_pomiary_sortowanie_introsort1()
{
	double czas = 0, suma_czasow = 0;
	clock_t start, stop;
	int* wsk; // wskaŸnik pomocniczny wskazuj¹cy na tablice

	cout << "Wariant 1.(LOSOWY)\t-Inicjalizuje wszystkie tablice losowymi wartosciami..." << endl;
	Inic_wszystkie_tablice_losowo();

	cout << "Wariant 1.(LOSOWY)\t-Rozpoczynam pomiar czasu wykonania algorytmu sortowania rosnaco, IntroSort..." << endl << endl;

	// dla tablic10k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_10k[i][0];

		start = clock();
		hybrydowe(wsk, ROZMIAR_1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_10k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_1 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;



	// dla tablic50k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_50k[i][0];

		start = clock();
		hybrydowe(wsk, ROZMIAR_2);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_50k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_2 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic100k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_100k[i][0];

		start = clock();
		hybrydowe(wsk, ROZMIAR_3);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_100k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_3 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic500k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_500k[i][0];

		start = clock();
		hybrydowe(wsk, ROZMIAR_4);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_500k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_4 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic1mln
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_1mln[i][0];

		start = clock();
		hybrydowe(wsk, ROZMIAR_5);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_1mln[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 1.(LOSOWY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_5 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	cout << "Wariant 1.(LOSOWY)\t-Zapisuje pomiary do pliku..." << endl;
	Zapisz_tablice_do_pliku(wyniki_10k, ROZMIAR_1, "Sortowanie IntroSort");
	Zapisz_tablice_do_pliku(wyniki_50k, ROZMIAR_2, "Sortowanie IntroSort");
	Zapisz_tablice_do_pliku(wyniki_100k, ROZMIAR_3, "Sortowanie IntroSort");
	Zapisz_tablice_do_pliku(wyniki_500k, ROZMIAR_4, "Sortowanie IntroSort");
	Zapisz_tablice_do_pliku(wyniki_1mln, ROZMIAR_5, "Sortowanie IntroSort");

	Zapisz_wiadomosc_do_pliku("Zakonczono Wariant 1.(LOSOWY) - Sortowanie IntroSort");

	cout << "Zakonczono IS Wariant 1.(LOSOWY) - Sortowanie IntroSort" << endl << endl << endl;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wykonaj_pomiary_sortowanie_introsort2()
{
	{
		double czas = 0, suma_czasow = 0;
		clock_t start, stop;
		int* wsk; // wskaŸnik pomocniczny wskazuj¹cy na tablice

		cout << "Wariant 2.(NIEKORZYSTNY)\t-Inicjalizuje wszystkie tablice liczbami posortowanymi malejaco..." << endl;
		Inic_wszystkie_tablice_posortowane_malejaco();

		cout << "Wariant 2.(NIEKORZYSTNY)\t-Rozpoczynam pomiar czasu wykonania algorytmu sortowania rosnaco, IntroSort..." << endl;

		// dla tablic10k
		suma_czasow = 0;
		for (int i = 0; i < ILOSC_TABLIC; i++)
		{
			wsk = &tablice_10k[i][0];

			start = clock();
			hybrydowe(wsk, ROZMIAR_1);
			stop = clock();

			czas = (double)(stop - start) / CLOCKS_PER_SEC;
			wyniki_10k[i] = czas;
			suma_czasow = suma_czasow + czas;
		}
		cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_1 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;



		// dla tablic50k
		suma_czasow = 0;
		for (int i = 0; i < ILOSC_TABLIC; i++)
		{
			wsk = &tablice_50k[i][0];

			start = clock();
			hybrydowe(wsk, ROZMIAR_2);
			stop = clock();

			czas = (double)(stop - start) / CLOCKS_PER_SEC;
			wyniki_50k[i] = czas;
			suma_czasow = suma_czasow + czas;
		}
		cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_2 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

		// dla tablic100k
		suma_czasow = 0;
		for (int i = 0; i < ILOSC_TABLIC; i++)
		{
			wsk = &tablice_100k[i][0];

			start = clock();
			hybrydowe(wsk, ROZMIAR_3);
			stop = clock();

			czas = (double)(stop - start) / CLOCKS_PER_SEC;
			wyniki_100k[i] = czas;
			suma_czasow = suma_czasow + czas;
		}
		cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_3 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

		// dla tablic500k
		suma_czasow = 0;
		for (int i = 0; i < ILOSC_TABLIC; i++)
		{
			wsk = &tablice_500k[i][0];

			start = clock();
			hybrydowe(wsk, ROZMIAR_4);
			stop = clock();

			czas = (double)(stop - start) / CLOCKS_PER_SEC;
			wyniki_500k[i] = czas;
			suma_czasow = suma_czasow + czas;
		}
		cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_4 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

		// dla tablic1mln
		suma_czasow = 0;
		for (int i = 0; i < ILOSC_TABLIC; i++)
		{
			wsk = &tablice_1mln[i][0];

			start = clock();
			hybrydowe(wsk, ROZMIAR_5);
			stop = clock();

			czas = (double)(stop - start) / CLOCKS_PER_SEC;
			wyniki_1mln[i] = czas;
			suma_czasow = suma_czasow + czas;
		}
		cout << "Wariant 2.(NIEKORZYSTNY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_5 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;


		cout << "Wariant 2.(NIEKORZYSTNY)\t-Zapisuje pomiary do pliku..." << endl;
		Zapisz_tablice_do_pliku(wyniki_10k, ROZMIAR_1, "Sortowanie IntroSort");
		Zapisz_tablice_do_pliku(wyniki_50k, ROZMIAR_2, "Sortowanie IntroSort");
		Zapisz_tablice_do_pliku(wyniki_100k, ROZMIAR_3, "Sortowanie IntroSort");
		Zapisz_tablice_do_pliku(wyniki_500k, ROZMIAR_4, "Sortowanie IntroSort");
		Zapisz_tablice_do_pliku(wyniki_1mln, ROZMIAR_5, "Sortowanie IntroSort");

		Zapisz_wiadomosc_do_pliku("Zakonczono Wariant 2.(NIEKORZYSTNY) - Sortowanie IntroSort");

		cout << "Zakonczono IS Wariant 2.(NIEKORZYSTNY) - Sortowanie IntroSort" << endl << endl << endl;

	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wykonaj_pomiary_sortowanie_introsort3(float procent_posortowania)
{


	double czas = 0, suma_czasow = 0;
	clock_t start, stop;
	int* wsk; // wskaŸnik pomocniczny wskazuj¹cy na tablice

	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Inicjalizuje wszystkie tablice. " << procent_posortowania << "% posortowane rosnaco, reszta tablicy losowa." << endl;
	Inic_wszystkie_tablice_posortowane_czesciowo(procent_posortowania);

	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Rozpoczynam pomiar czasu wykonania algorytmu sortowania rosnaco, IntroSort..." << endl << endl;

	// dla tablic10k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_10k[i][0];

		start = clock();
		hybrydowe(wsk, ROZMIAR_1);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_10k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_1 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;



	// dla tablic50k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_50k[i][0];

		start = clock();
		hybrydowe(wsk, ROZMIAR_2);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_50k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_2 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic100k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_100k[i][0];

		start = clock();
		hybrydowe(wsk, ROZMIAR_3);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_100k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_3 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;

	// dla tablic500k
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_500k[i][0];

		start = clock();
		hybrydowe(wsk, ROZMIAR_4);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_500k[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_4 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;
	
	// dla tablic1mln
	suma_czasow = 0;
	for (int i = 0; i < ILOSC_TABLIC; i++)
	{
		wsk = &tablice_1mln[i][0];

		start = clock();
		hybrydowe(wsk, ROZMIAR_5);
		stop = clock();

		czas = (double)(stop - start) / CLOCKS_PER_SEC;
		wyniki_1mln[i] = czas;
		suma_czasow = suma_czasow + czas;
	}
	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Sredni czas sortowania rosnaco, IS dla tablicy " << ROZMIAR_5 << " liczb, wynosi: " << suma_czasow / ILOSC_TABLIC << " sec." << endl;


	cout << "Wariant 3.(" << procent_posortowania << "% KORZYSTNY)\t-Zapisuje pomiary do pliku..." << endl;
	Zapisz_tablice_do_pliku(wyniki_10k, ROZMIAR_1, "Sortowanie IntroSort");
	Zapisz_tablice_do_pliku(wyniki_50k, ROZMIAR_2, "Sortowanie IntroSort");
	Zapisz_tablice_do_pliku(wyniki_100k, ROZMIAR_3, "Sortowanie IntroSort");
	Zapisz_tablice_do_pliku(wyniki_500k, ROZMIAR_4, "Sortowanie IntroSort");
	Zapisz_tablice_do_pliku(wyniki_1mln, ROZMIAR_5, "Sortowanie IntroSort");

	Zapisz_wiadomosc_do_pliku("Zakonczono Wariant 3. - Sortowanie IntroSort", procent_posortowania);

	cout << "Zakonczono IS Wariant 3.(" << procent_posortowania << "% KORZYSTNY) - IntroSort" << endl << endl << endl;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// funkcje zapisu danych do pliku
void Zapisz_tablice_do_pliku(double tablica[], int ilosc_elementow, const char* napis)
{
	double srednia=0;
	std::fstream plik;
	plik.precision(5); plik.width(10);


	plik.open("pomiary.txt", std::ios::out | std::ios::app);
	if (plik.good() == true)
	{
		plik << "Czasy sortowania: " << napis << " " << ILOSC_TABLIC << " tablic o wielkosci: " << ilosc_elementow << " wynosza: " << endl;
		for (int i = 0; i < ILOSC_TABLIC; i++)
		{
			srednia = (srednia + tablica[i]) / ILOSC_TABLIC;
		}

		plik << srednia;
		plik << endl;

		plik.close();
	}
	else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;

}

void Zapisz_wiadomosc_do_pliku(const char* napis, float procent_posortowania) // procent_posortowania domyslnie rowna sie zero
{
	std::fstream plik;

	plik.open("pomiary.txt", std::ios::out | std::ios::app);
	if (plik.good() == true)
	{
		if (procent_posortowania != 0) plik << "Sortowania wykonano dla " << procent_posortowania << "% tablicy posortowanej korzystnie (ROSNACO) ";
		plik << napis;
		plik << endl << endl << endl << endl;

		plik.close();
	}
	else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;

}