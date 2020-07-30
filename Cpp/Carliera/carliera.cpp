
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>

class Job
{
public:
	int R;
	int P;
	int Q;

	int PpomShrage;

	int Index;

	bool LewaStrona = false;

	int CzasRozpoczecia = 0;

	Job job() {};

	void pokazPrace() {
		std::cout << "R: " << R << " P: " << P << " Q: " << Q << " init index: " << Index << "\n";
	}

	void pokazIndex()
	{
		std::cout << Index << " ";
	}

	void pokazStrone()
	{
		std::cout << LewaStrona << " ";
	}

	void pokazPoczatek()
	{
		std::cout << CzasRozpoczecia << " ";
	}
};

int calculateC_MAX(const std::vector<Job>& praca) {
	int m = 0, c = 0;
	for (int i = 0; i < praca.size(); i++) {
		m = std::max(praca[i].R, m) + praca[i].P;
		c = std::max(c, m + praca[i].Q);
	}
	return c;
}

// Zadanie o indexie index jest uwzględniane w liczonym CMAX
int calculateC_MAX(const std::vector<Job>& jobs, const int index) {
	int m = 0, c = 0;
	for (int i = 0; i <= index; i++) {
		m = std::max(jobs[i].R, m) + jobs[i].P;
		c = std::max(c, m + jobs[i].Q);
	}
	return c;
}

struct DTO // DataTransferObject
{
	std::vector<Job> order;
	int C_MAX;
};

 //Funkcja znajduje indexy dostępnych zadań w przedziale czasu od zera do argumentu elapsedTime włącznie

std::vector<int> getIndiciesAvailableJobs(const std::vector<Job>& praca, const int czas) {
	std::vector<int> wynik;
	for (int i = 0; i < praca.size(); i++) {
		if (praca[i].R <= czas) {
			wynik.push_back(i);
		}
	}

	return wynik;
}

// arguemtn jobs musi byc posorotwany rosnąco po R
DTO shrageNormal(std::vector<Job> praca) {
	std::sort(praca.begin(), praca.end(), [](const Job& first, const Job& second) { return first.R < second.R; });
	std::vector<Job> wynik;

	int czas = praca[0].R;
	int nowyCzas = 0;
	int nastepnyCzas;
	int Cmax = 0;
	int Maxqindex;
	int MaxQ;

	int pom = 0;

	std::vector<int> dostepnePrace;


	while (praca.size() != 0) {
		dostepnePrace = getIndiciesAvailableJobs(praca, czas);

		if (dostepnePrace.size() != 0)
		{
			Maxqindex = dostepnePrace[0];
			MaxQ = praca[Maxqindex].Q;

			for (int index : dostepnePrace) {
				if (MaxQ < praca[index].Q) {
					MaxQ = praca[index].Q;
					Maxqindex = index;

				}
				else if (MaxQ == praca[index].Q) { // jeżeli takie same Q to bierzemy o mniejszym indexie początkowym

					if (praca[Maxqindex].Index > praca[index].Index) {
						MaxQ = praca[index].Q;
						Maxqindex = index;
					}
				}
			}

			// trzeba znaleźć index pierwszego zadania którego czas R jest wiekszy od elapsedTime, to będzie tNext
			nastepnyCzas = std::numeric_limits<int>::max();
			for (int i = 0; i < praca.size(); i++) {
				if (praca[i].R > czas) {
					nastepnyCzas = praca[i].R;
					break;
				}
			}

			/*
				Ten fragment jest potrzebny do poszukiwania zadania A w algorytmie Cariera
				mianowicie jeśli conajmniej dwa razy z rzędu wszedłem do tego if'a to znaczy że pomiędzy tymi zadaniami nie ma dziury.
			*/

			pom++;
			if (pom > 1)
			{
				praca[Maxqindex].LewaStrona = true;
			}

			// potrzebne w algorytmie Cariera podczas szukania zadania A
			praca[Maxqindex].CzasRozpoczecia = czas;

			czas += praca[Maxqindex].P;

			Cmax = std::max(czas + praca[Maxqindex].Q, Cmax);

			wynik.push_back(praca[Maxqindex]);

			// usunać z kolekcji jobs obiekt pod indexem biggestQIndex
			praca.erase(praca.begin() + Maxqindex);

		}
		else {
			czas = nastepnyCzas;
			pom = 0;
		}
	}

	DTO dto;
	dto.C_MAX = Cmax;
	dto.order = wynik;

	//std::cout << "Cmax: " << Cmax << "\n";

	return dto;
}


// arguement jobs musi byc posorotwany rosnąco po R
int shrageWithInterrupt(std::vector<Job> praca) {
	std::sort(praca.begin(), praca.end(), [](const Job& first, const Job& second) { return first.R < second.R; });
	int czas = praca[0].R;
	int nowyCzas = 0;
	int nastepnyCzas;
	int Cmax = 0;
	int MaxQIndeks;
	int MaxQ;

	std::vector<int> dostepnePrace;


	while (praca.size() != 0) {
		dostepnePrace = getIndiciesAvailableJobs(praca, czas);

		if (dostepnePrace.size() != 0) {
			MaxQIndeks = dostepnePrace[0];
			MaxQ = praca[MaxQIndeks].Q;

			for (int index : dostepnePrace) {
				if (MaxQ < praca[index].Q) {
					MaxQ = praca[index].Q;
					MaxQIndeks = index;

				}
				else if (MaxQ == praca[index].Q) { // jeżeli takie same Q to bierzemy o mniejszym indexie początkowym

					if (praca[MaxQIndeks].Index > praca[index].Index) {
						MaxQ = praca[index].Q;
						MaxQIndeks = index;
					}
				}
			}
			// trzeba znaleźć index pierwszego zadania którego czas R jest wiekszy od czasu, to będzie nastepnyczas

			nastepnyCzas = std::numeric_limits<int>::max();
			for (int i = 0; i < praca.size(); i++) {
				if (praca[i].R > czas) {
					nastepnyCzas = praca[i].R;
					break;
				}
			}


			nowyCzas = std::min(czas + praca[MaxQIndeks].PpomShrage, nastepnyCzas);
			praca[MaxQIndeks].PpomShrage = praca[MaxQIndeks].PpomShrage - (nowyCzas - czas);
			czas = nowyCzas;




			// jeśli P zmniejszyło się do zera to obliczay Cmax i usuwamy tą prace z kolekcji
			if (praca[MaxQIndeks].PpomShrage == 0) {
				Cmax = std::max(czas + praca[MaxQIndeks].Q, Cmax);

				//result.push_back(jobs[biggestQIndex]);

				// usunać z kolekcji jobs obiekt pod indexem MAxQI
				praca.erase(praca.begin() + MaxQIndeks);
			}


		}
		else {
			czas = nastepnyCzas;
		}
	}


	return Cmax;
}


// Funkcja zwraca index zadania B, zadanie B zawsze będzie znalezione
// Do funkcji musimy przekazać kolekcje posortowaną w shrageNormal
int findBjob(const std::vector<Job>& praca)
{
	/*
	Poszukiwanie zadania B:
	liczym Cmax całej kolekcji jobs
	*/

	int polCmax = calculateC_MAX(praca);
	int dlugoscPracy = praca.size();
	int IndexPracyB = 0;

	for (int pracaIndex = dlugoscPracy - 1; pracaIndex >= 0; pracaIndex--)
	{
		if (calculateC_MAX(praca, pracaIndex) != polCmax)
		{
			IndexPracyB = pracaIndex + 1;
			break;
		}
	}
	return IndexPracyB;
}

// Zadanie A nie zawsze musi się znajdować w kolekcji
int findAjob(const std::vector<Job>& praca, const int IndexB)
{
	//Zadanie A musi się znajdować w jednym bloku razem z zadaniem B. te zadania muszą być przyklejone do siebie
	
	// zaczynamy od zadania o indexie indexOfBJob idziemy w lewo i szukamy pierwszego zadania którego pole stickWithLeftSideJob wynosi false
	int pomIndex = -1;
	for (int index = IndexB; index >= 0; index--)
	{
		if (praca[index].LewaStrona == false)
		{
			pomIndex = index;
			break;
		}
	}
	// po tej pętli znaleźliśmy index lewej granicy bloku zadań. (zadanie o indexie indexOfLeftBound jest ostatnim przyklejonym zadaniem w bloku razem z zadaniem B)
	// teraz w tym bloku szukamy od lewej pierwszego zadania którego fatkyczny czas rozpoczęcia minus jego R wynosi zero.
	int IndexPracyA = -1;
	for (int index = pomIndex; index <= IndexB; index++)
	{
		if ((praca[index].CzasRozpoczecia - praca[index].R) == 0)
		{
			IndexPracyA = index;
			break;
		}
	}


	return IndexPracyA; // może zwrócić -1
}

// Zadanie C nie zawsze musi się znajdować w kolekcji
// Do funkcji musimy przekazać kolekcje posortowaną w shrageNormal

int findCJob(const std::vector<Job>& jobs, const int indexOfAJob, const int indexOfBJob)
{
	/*
	   Teraz w kolekcji jobs rozpatrujemy blok zadań zaczynający się w zadaniu o indexie indexOfAJob do zadania o indexie indexOfBJob włącznie z tymi zadaniami.
	   szukamy w tym bloku takiego zadania którego Q jest mniejsze od Q zadania B
	   jeśli Q są takie same to bierzemy to najbardziej na prawo (czyli zaczniemy przeszukiwanie od prawej do lewej i zwracamy pierwsze znalezione)
	*/

	int IndexPracyC = -1;
	for (int index = (indexOfBJob - 1); index >= indexOfAJob; index--)
	{
		if (jobs[index].Q < jobs[indexOfBJob].Q)
		{
			IndexPracyC = index;
			break;
		}
	}
	return IndexPracyC; // może zwrócić -1
}


DTO carier(DTO iDto)
{

	DTO dto = shrageNormal(iDto.order);
	int IndexB = findBjob(dto.order);
	int IndexA = findAjob(dto.order, IndexB);
	int IndexC = findCJob(dto.order, IndexA, IndexB);

	if (IndexC == -1)
	{
		return dto;
	}

	std::vector<Job> K;
	for (int index = (IndexC + 1); index <= IndexB; index++)
	{
		K.push_back(dto.order[index]);
	}


	int MinRwK = (*std::min_element(K.begin(), K.end(),
		[](const Job& lhs, const Job& rhs)
		{
			return lhs.R < rhs.R;
		})).R;

	int MinQwK = (*std::min_element(K.begin(), K.end(),
		[](const Job& lhs, const Job& rhs)
		{
			return lhs.Q < rhs.Q;
		})).Q;

	int sumWK = std::accumulate(K.begin(), K.end(), 0,
		[](int i, const Job& praca)
		{
			return praca.P + i;
		});


	int pom1 = dto.order[IndexC].R;

	dto.order[IndexC].R = std::max(pom1, (MinRwK + sumWK));

	int CmaxShragerI = shrageWithInterrupt(dto.order);
//dolaczam zadanie k na poczatek

	std::vector<Job> Pomk = K;

	Pomk.insert(Pomk.begin(), dto.order[IndexC]);

	int hk = MinRwK + sumWK + MinQwK;


	CmaxShragerI = std::max(hk, CmaxShragerI);


	if (CmaxShragerI < dto.C_MAX)
	{
		dto = carier(dto);
	}

	dto.order[IndexC].R = pom1;

	int Pom2 = dto.order[IndexC].Q;

	dto.order[IndexC].Q = std::max(Pom2, (MinQwK + sumWK));

	CmaxShragerI = shrageWithInterrupt(dto.order);


	hk = MinRwK + sumWK + MinQwK;

	CmaxShragerI = std::max(hk, CmaxShragerI);

	if (CmaxShragerI < dto.C_MAX)
	{
		dto = carier(dto);
	}
	dto.order[IndexC].Q = Pom2;

	return dto;
}

int main(int argc, char** argv) {

	std::vector<Job> praca;

	Job Pracaaktualna;
	DTO dto;
	int NajlepszeCmax = std::numeric_limits<int>::max();
	std::vector<Job> NajlepszePoszeregowanie;

	std::ifstream myfile("carl.txt");
	int Iloscdanych = 0;

	std::string linia;

	if (!myfile.is_open()) throw std::runtime_error("Nie udalo sie otworzyc pliku");

	std::string Stringpom;

	for (int i = 0; i <= 8; i++) {

		Stringpom = "data.00" + std::to_string(i) + ":\n";

		while (getline(myfile, linia) && (linia[0] != 'd'));
		myfile >> Iloscdanych;
		try
		{
			if (Iloscdanych == 0) throw std::runtime_error(std::string("Blad odczytu z pliku"));
		}
		catch (const std::exception & e)
		{
			std::cerr << e.what() << "\n";
			exit(-1);
		}


		for (int i = 0; i < Iloscdanych; i++) {
			myfile >> Pracaaktualna.R >> Pracaaktualna.P >> Pracaaktualna.Q;
			Pracaaktualna.Index = i;
			Pracaaktualna.PpomShrage = Pracaaktualna.P;

			praca.push_back(Pracaaktualna);
		}

		// Posortujmye vector prac po R rosnąco

		std::sort(praca.begin(), praca.end(), [](const Job& first, const Job& second) { return first.R < second.R; });




		// Carier
		dto.order = praca;
		dto = carier(dto);
		std::cout << "Zestaw danych: " << i << "\n";
		std::cout << "dto.cmax : " << dto.C_MAX << "\n\n";
		std::cout << "Kolejnosc: [";
		for (auto& obj : dto.order) {
			obj.pokazIndex();
		}
		std::cout << "]\n\n";


		praca.clear();
	}


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
