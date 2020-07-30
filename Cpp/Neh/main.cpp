// main.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>
#include <functional> 

std::ofstream output;

clock_t StartCmax;
clock_t StopCmax;

double CzasCmax = 0;

int temp[501][21]; // tablica pomocnicza do funkcji calculateC_MAX



class Praca {
public:
	int zadania[20]; // 20 - max maszyn
	int dlugosc = 0;
	int indeks = 0;
	Praca() {}


	void caladlugosc(int size) {
		this->dlugosc = 0;

		for (int i = 0; i < size; i++) {
			this->dlugosc += zadania[i];
		}
	}

	void reset() {
		dlugosc = 0;
	}

};


void wypisz1(Praca jobs[], int sizeP, int sizeM) {
	for (int i = 0; i < sizeP; i++) {
		std::cout << "\nInit index: " << jobs[i].indeks + 1 << " entireLength: " << jobs[i].dlugosc << "     [";
		for (int j = 0; j < sizeM; j++) {
			std::cout << jobs[i].zadania[j] << ", ";
		}
		std::cout << "]\n";
	}
}

void wypisz2(Praca jobs[], int size) {
	for (int i = 0; i < size; i++) {
		output << jobs[i].indeks + 1 << " ";
	}
}


int liczCmax(Praca jobs[], const int iloscP, const int iloscM) {
	StartCmax = clock();
	temp[1][1] = jobs[1].zadania[1];

	int valueCarryForNextRow = 0;

	for (int i = 0; i < iloscP; i++) {
		temp[i][0] = 0;
	}

	for (int i = 0; i < iloscM; i++) {
		temp[0][i] = 0;
	}


	for (int row = 1; row <= iloscP; row++) {
		for (int column = 1; column <= iloscM; column++) {
			valueCarryForNextRow = jobs[row - 1].zadania[column - 1];
			temp[row][column] = std::max(temp[row - 1][column], temp[row][column - 1]) + valueCarryForNextRow;
		}
	}
	StopCmax = clock();
	CzasCmax += double(StopCmax - StartCmax) / (CLOCKS_PER_SEC);
	return temp[iloscP][iloscM];
}




int main() {
	std::cout << "Zapisuje dane do pliku: neh.txt";

	clock_t Start, Koniec, StartSort, KoniecSort;

	double czasSort = 0;

	clock_t StartNeh, KoniecNeh;

	double czasNeh = 0;

	std::ifstream myfile("neh.txt");
	output.open("Wyniki.txt", std::ofstream::out | std::ofstream::trunc);

	int iloscZadan = 0;
	int iloscMaszyn = 0;
	int pom;//wartosc tymczasowa


	Praca jobs[500];

	std::string line;
	std::string Stringpom = "";

	Praca wykonywaneZadanie;

	if (!myfile.is_open()) throw std::runtime_error("Nie udalo sie otworzyc pliku");
	Start = clock();
	for (int i = 0; i <= 120; i++) {

		if (i < 10) {
			Stringpom = "data.00" + std::to_string(i) + ":\n";
		}
		else if (i >= 10 && i < 100) {
			Stringpom = "data.0" + std::to_string(i) + ":\n";
		}
		else {
			Stringpom = "data." + std::to_string(i) + ":\n";
		}
		while (getline(myfile, line) && (line[0] != 'd'));

		myfile >> iloscZadan;
		myfile >> iloscMaszyn;
		try
		{
			if (iloscZadan == 0) throw std::runtime_error(std::string("Blad odczytu z pliku"));
		}
		catch (const std::exception & e)
		{
			std::cerr << e.what() << "\n";
			exit(-1);
		}


		for (int i = 0; i < iloscZadan; i++) {
			for (int col = 0; col < iloscMaszyn; col++) {
				myfile >> pom;
				wykonywaneZadanie.zadania[col] = pom;
			}
			wykonywaneZadanie.indeks = i;
			wykonywaneZadanie.caladlugosc(iloscMaszyn);

			jobs[i] = wykonywaneZadanie;
		}



		/*
		Trzeba posortować tak żeby w przypadku zadań o tej samej dlugosci
		pierwsze było to które ma mniejszą wartość indeks.
		*/


		StartSort = clock();
		// Najpierw sortuje tablice na podstawie dlugosci
		std::sort(jobs, jobs + iloscZadan, [](const Praca& first, const Praca& second) { return first.dlugosc > second.dlugosc; });
		int indeks1 = 0; //pierwszy indeks z ta sama dlugoscia
		int indeks2 = 0; //ostatni indeks z ta sama dlugoscia

		// teraz szukam takich fragmentów w tablicy gdzie obok siebie są Job których dlugosc jest taka sama
		// następnie sortuje ten fragment tablicy na podstawie indeksu
		for (int i = 0; i < iloscZadan - 1; i++) {
			if (jobs[i].dlugosc == jobs[i + 1].dlugosc) {
				indeks1 = i;
				while (jobs[i].dlugosc == jobs[i + 1].dlugosc) {
					i++;
				}
				indeks2 = i;
			}
			std::sort(jobs + indeks1, jobs + indeks2 + 1, [](const Praca& first, const Praca& second) { return first.indeks < second.indeks; });
		}

		KoniecSort = clock();

		czasSort += double(KoniecSort - StartSort) / (CLOCKS_PER_SEC / 1000);

		// neh

		int obecnyCmax = 0;
		int minCmax = 0;

		int liczbaZmian = 0;
		int liczbaZmianNajlepszymRezultatem = 0;

		StartNeh = clock();

		for (int i = 0, j = 2; i < iloscZadan - 1; i++, j++) {
			obecnyCmax = liczCmax(jobs, j, iloscMaszyn);
			minCmax = obecnyCmax;


			liczbaZmian = 0;
			liczbaZmianNajlepszymRezultatem = 0;
			for (int i = j - 1; i >= 1; i--) {
				liczbaZmian++;
				std::swap(jobs[i], jobs[i - 1]);
				obecnyCmax = liczCmax(jobs, j, iloscMaszyn);

				if (obecnyCmax <= minCmax) {
					minCmax = obecnyCmax;
					liczbaZmianNajlepszymRezultatem = liczbaZmian;
				}
			}

			for (int x = liczbaZmian, k = 0; x > liczbaZmianNajlepszymRezultatem; x--) {
				std::swap(jobs[k], jobs[k + 1]);
				k++;
			}
		}
		KoniecNeh = clock();

		czasNeh += double(KoniecNeh - StartNeh) / (CLOCKS_PER_SEC);


		output << Stringpom <<"  ";
		wypisz2(jobs, iloscZadan);
		output << "\nWynik = " << liczCmax(jobs, iloscZadan, iloscMaszyn); output << "\n";

	}




	Koniec = clock();
	double czas = double(Koniec - Start) / (CLOCKS_PER_SEC);

	std::cout << "\nCałkowity czas: " << czas << " seconds\n";
	std::cout << "Czas sortowania: " << czasSort << "\n";
	std::cout << "Czas Neh: " << czasNeh << "\n";

	std::cout << "Czas liczenia C_MAX = " << CzasCmax << "\n";

	myfile.close();
	return 0;
}


// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln

