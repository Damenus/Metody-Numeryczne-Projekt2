#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <ctime>
#include <cstdlib>

using namespace std;

class Niewiadoma
{
public:
	//(pole gdzie gracz1, pole gdzie gracz2, który gracz ma Niewiadoma) 
	int gracz1Pozycja, gracz2Pozycja, gracz;
	vector<Niewiadoma> tab;
	int pewne = 0;						//
	double wynik_gauss = 1.0;			//zmienna wynik dla algorytmy gaussa
	double wynik_iteracyjne = 0.0;		//zmienna wynik dla algorytmy iteracyjnego
	double wspolczynnik = 1.0;			//wspó³czynik stoj¹cy przy niewiadomej

	Niewiadoma() {}

	Niewiadoma(int a, int b, int c) {
		gracz1Pozycja = a;
		gracz2Pozycja = b;
		gracz = c;
	}

	bool wstaw(Niewiadoma *ruch) {
		Niewiadoma *findy = new Niewiadoma();
		//if (find(wektor.begin(), wektor.end(), *ruch) != wektor.end()) 
		for (int is = 0; is < tab.size(); is++) {
			findy = &tab[is];
			if (findy->gracz != ruch->gracz || findy->gracz1Pozycja != ruch->gracz1Pozycja || findy->gracz2Pozycja != ruch->gracz2Pozycja)
				continue;
			else {
				tab[is].wspolczynnik++;
				return true;
				break;
			}
		}
		
		tab.push_back(*ruch);
		return false;
	}

};

class Macierz {
public:
	double **macierz_wspolczynnikow;
	vector<Niewiadoma> wektor_niewiadomych;
	int rozmiar_macierzy;

	Macierz(vector<Niewiadoma> wektor){
		this->wektor_niewiadomych = wektor;
		this->rozmiar_macierzy = wektor.size();
		//tworzenie tablicy dwuwymiarowej
		macierz_wspolczynnikow = new double*[rozmiar_macierzy];
		for (int i = 0; i < rozmiar_macierzy; i++)
			macierz_wspolczynnikow[i] = new double[rozmiar_macierzy]; //+1 na macierz wyrazów wolnych

		for (int j = 0; j < rozmiar_macierzy; j++) {
			for (int k = 0; k < rozmiar_macierzy; k++){
				macierz_wspolczynnikow[j][k] = 0;
			}
		}

		//wype³nianie macierzy wspó³czynnikami		
		for (int i = 0; i < wektor.size(); i++) {
			for (int j = 0; j < wektor[i].tab.size(); j++) {	
				//dla ka¿dej niewiadomej, która jest w równaniu do wyliczenia danej niewiadmoej, dzielimy wspó³czynnik przez -6 (bo zmieniamy stronê) i wpisujemy wpó³czynnik na odpowiedni¹ pozycjê
				this->macierz_wspolczynnikow[i][znajdz_numer_kolumny(&this->wektor_niewiadomych[i].tab[j])] = -wektor[i].tab[j].wspolczynnik / 6.0; // w iteracyjnym jest bez minusa, ale dzielenie przez wpó³czynik zmienia na dobry wynik
			}
			this->macierz_wspolczynnikow[i][i] = 1;
			this->macierz_wspolczynnikow[i][wektor.size()] = (double)wektor[i].pewne / 6.0;
		}

	}

	int znajdz_numer_kolumny(Niewiadoma *szukany) {
		Niewiadoma *findy = new Niewiadoma();
		for (int is = 0; is < wektor_niewiadomych.size(); is++) {
			*findy = wektor_niewiadomych[is];
			if (findy->gracz != szukany->gracz || findy->gracz1Pozycja != szukany->gracz1Pozycja || findy->gracz2Pozycja != szukany->gracz2Pozycja)
				continue;
			else {
				return is;
				break;
			}
		}
		return false;
	}

	void wypisz_macierz() {
		for (int j = 0; j < rozmiar_macierzy; j++) {
			cout << j << ": ";
			for (int k = 0; k <= rozmiar_macierzy; k++){
				cout << macierz_wspolczynnikow[j][k] << " ";
			}
			cout << endl;
		}
	}

	double gauss() {

		//eliminacja
		double m;
		for (int i = 0; i < this->rozmiar_macierzy; i++) { // ka¿da kolumna pokolei
			for (int j = i + 1; j < this->rozmiar_macierzy; j++) { //tylko dolny trójk¹t
				m = macierz_wspolczynnikow[j][i] / macierz_wspolczynnikow[i][i]; //wspo³czynnik mno¿enia pierwszego wiersza, by odj¹æ od kolejnych kolumn, by wyzerowaæ dolny trójk¹t
				for (int k = i; k < this->rozmiar_macierzy + 1; k++) { // zerowanie kolumny
					macierz_wspolczynnikow[j][k] = macierz_wspolczynnikow[j][k] - m * macierz_wspolczynnikow[i][k];
				}
			}
		}
		
		//obliczanie
		double s;
		wektor_niewiadomych[rozmiar_macierzy - 1].wynik_gauss = macierz_wspolczynnikow[rozmiar_macierzy - 1][rozmiar_macierzy] / macierz_wspolczynnikow[rozmiar_macierzy - 1][rozmiar_macierzy - 1]; //obliczanie zaczynamy od ostatniej niewiadomej, poniewa¿ wyzerowaliœmy dlon¹ trójk¹t macierzy i zosta³ jednie wyraz wolny, wiêc by wyliczyæ wartoœæ niewiadmowej, wystarczy podzieliæ ten wyraz przez wppó³czynnik przy niewiadomej

		for (int i = rozmiar_macierzy - 2; i >= 0; i--){ // wyznaczamy niewiadome od koñca, -1 bo indeksujemy macierz od 0, -1 bo ostatni¹ niewiadm¹ ju¿ wyliczyliœmy
			s = 0;
			for (int k = i + 1; k < rozmiar_macierzy; k++) {
				s = s + macierz_wspolczynnikow[i][k] * wektor_niewiadomych[k].wynik_gauss; //sumujemy iloczyny wczeœniej wyliczonych niewiadomych z wspó³czynnikami				
			}
			wektor_niewiadomych[i].wynik_gauss = (macierz_wspolczynnikow[i][rozmiar_macierzy] - s) / macierz_wspolczynnikow[i][i]; //od wyrazu wolnego odejmujemy policzon¹ sumê i dzielimy przez wpó³czynnik przy niewiadomej
		}

		return wektor_niewiadomych[0].wynik_gauss;
	}

	void iteracyjne(int stop = 1000, double eps = 0.000001) {

		bool koniec = true;		
		vector<Niewiadoma> wektor2;
		double a;
		do
		{
			// przepisanie x - aktualnych wynikow do x2 - wynikow z poprzedniej iteracji
			wektor2 = wektor_niewiadomych;

			// wykonanie kolejnej iteracji
			for (int i = 0; i < rozmiar_macierzy; i++)
			{
				a = macierz_wspolczynnikow[i][rozmiar_macierzy];//wyraz wolny

				for (int j = 0; j < i; j++)
					a -= macierz_wspolczynnikow[i][j] * wektor_niewiadomych[j].wynik_iteracyjne;

				for (int j = i + 1; j < rozmiar_macierzy; j++)
					a -= macierz_wspolczynnikow[i][j] * wektor2[j].wynik_iteracyjne;


				wektor_niewiadomych[i].wynik_iteracyjne = a / macierz_wspolczynnikow[i][i];
			}

			// sprawdzenie warunku zakonczenia: ||x(k)-x(k-1)|| <= epsilon
			for (int i = 0; i < rozmiar_macierzy; i++)
			{
				if (fabs(wektor_niewiadomych[i].wynik_iteracyjne - wektor2[i].wynik_iteracyjne) > eps) { koniec = true; break; }
				else koniec = false;
			}
			stop--;
		} while (koniec && stop > 0);


		cout << wektor_niewiadomych[0].wynik_iteracyjne << " dla " << 1000 - stop << " ilosci obiegow i epsilona: " << eps << endl;
	}
};

class Plansza{
public:
	int ilosc_pol;
	map<int, int> polapki;

	Plansza() {}

	Plansza(int ilosc_pol, map<int, int> polapki) {
		this->ilosc_pol = ilosc_pol;
		this->polapki = polapki;
	}
};

class MonteCarlo {

	Plansza *plansza;
	int ilosc_gier;

public:
	MonteCarlo(Plansza *plansza, int ilosc = 1000000) {
		this->plansza = plansza;
		this->ilosc_gier = ilosc;
		srand(time(NULL));
	}

	double wynik() {

		double tablica_wygranych[2] = { 0, 0 };

		for (int przebieg = 0; przebieg < this->ilosc_gier; przebieg++) { //gramy w grê, i zzliczamy ile razy kto wygra³
			tablica_wygranych[gra()]++;
		}

		return tablica_wygranych[0] / (double)this->ilosc_gier; //zwracamy procent wygranych gier przez pierwszego gracza

	}
private:

	int rzutKoscia() {
		return ((rand() % 6) + 1);
	}

	int gra() { //gra zwaraca 0 lub 1 w zale¿noœci który gracz wygra³
		
		int gracz1 = 0;
		int gracz2 = 0;
		int czyja_tura = 1;
		int rzut_koscia;

		while (true) {

			rzut_koscia = rzutKoscia();

			if (czyja_tura == 1) {
				gracz1 += rzut_koscia;

				if (plansza->polapki.find(gracz1) != plansza->polapki.end()) { // trafi³ na pu³pakê
					gracz1 = gracz1 + plansza->polapki[gracz1];
				}

				if (gracz1 >= plansza->ilosc_pol) { //sprawdzenie czy wygra³
					return 0;
					break;
				}

			}
			else {
				gracz2 += rzut_koscia;

				if (plansza->polapki.find(gracz2) != plansza->polapki.end()) {
					gracz2 = gracz2 + plansza->polapki[gracz2];
				}

				if (gracz2 >= plansza->ilosc_pol) {
					return 1;
					break;
				}

			}

			czyja_tura = czyja_tura % 2 + 1; //modulo 2 + 1 daje przeciwnego gracza

		}

	}


};

class Zagadka {
public:
	Plansza plansza;
	int pola;
	map<int, int> polapki;
	vector<Niewiadoma> wektor;

	Zagadka() {
	}

	Zagadka(Plansza plansza) {
		this->plansza = plansza;
		this->polapki = plansza.polapki;
		this->pola = plansza.ilosc_pol;
	}

	bool znajdz(Niewiadoma *ruch) {
		Niewiadoma *findy = new Niewiadoma();
		//if (find(wektor.begin(), wektor.end(), *ruch) != wektor.end()) {
		for (int is = 0; is < wektor.size(); is++) {
			*findy = wektor[is];
			if (findy->gracz != ruch->gracz || findy->gracz1Pozycja != ruch->gracz1Pozycja || findy->gracz2Pozycja != ruch->gracz2Pozycja)
				continue;
			else {
				return true;
				break;
			}
		}

		return false;
	}

	vector<Niewiadoma> analiza() {
		int ktorygracz, mozliwapulapka;
		Niewiadoma analizowany, *ruch;

		vector<Niewiadoma> wektor2;
		queue<Niewiadoma> kolejka;

		ruch = new Niewiadoma(0, 0, 1); //pierwszy ruch w grze, ruch gracza 1

		kolejka.push(*ruch);
		wektor.push_back(*ruch);

		while (!kolejka.empty())
		{
			analizowany = kolejka.front();
			kolejka.pop();

			ktorygracz = analizowany.gracz % 2 + 1; //modulo 2 + 1 daje przeciwnego gracza
			mozliwapulapka = (ktorygracz == 2) ? analizowany.gracz1Pozycja : analizowany.gracz2Pozycja;

			for (int i = 1; i <= 6; i++) {
				if (analizowany.gracz1Pozycja + i *  ((int)ktorygracz / 2) < pola && analizowany.gracz2Pozycja + i *abs(ktorygracz - 2) < pola) {

					if (polapki.find(mozliwapulapka + i) != polapki.end()) {
						ruch = new Niewiadoma(analizowany.gracz1Pozycja + (i + polapki[mozliwapulapka + i]) *((int)ktorygracz / 2), analizowany.gracz2Pozycja + (i + polapki[mozliwapulapka + i]) * abs(ktorygracz - 2), ktorygracz);
					}
					else {
						ruch = new Niewiadoma(analizowany.gracz1Pozycja + i *  ((int)ktorygracz / 2), analizowany.gracz2Pozycja + i *abs(ktorygracz - 2), ktorygracz);
					}

					analizowany.wstaw(ruch);

					if (znajdz(ruch) == false) {
						wektor.push_back(*ruch);
						kolejka.push(*ruch);
					}

				}
				else { // dodaje pewne zwyciêstwo
					if (analizowany.gracz == 1)
						analizowany.pewne += 1;
				}

			}

			wektor2.push_back(analizowany);


		}

		return wektor2;
	}


	void wypisz_rownania(vector<Niewiadoma> wektor) {
		cout << "Iloœæ równañ: " << wektor.size() << endl;
		for (int iter = 0; iter < wektor.size(); iter++) {
			cout << iter << ":" << wektor[iter].gracz1Pozycja << wektor[iter].gracz2Pozycja << wektor[iter].gracz << " : ";
			for (int iter2 = 0; iter2 < wektor[iter].tab.size(); iter2++) {
				cout << wektor[iter].tab[iter2].wspolczynnik << " " << wektor[iter].tab[iter2].gracz1Pozycja << wektor[iter].tab[iter2].gracz2Pozycja << wektor[iter].tab[iter2].gracz << " , ";
			}
			cout << " " << wektor[iter].pewne << endl;
		}
	}
};

int main()
{
	cout.precision(16);

	int pola = 28;
	map<int, int> polapki = { { 1, -1 }, { 4, -1 }, { 6, -3 }, { 7, -2 }, { 13, -4 }, { 14, -5 }, { 17, -1 }, { 20, -2 }, { 21, -2 }, { 22, -10 }, { 24, -14 }, { 25, -2 }, { 27, -8 } };
	//int pola = 27;
	//map<int, int> polapki = { { 4, -2 }, { 5, -2 }, { 7, -5 }, { 9, -3 }, { 14, -12 }, { 15, -2 }, { 17, -7 }, { 19, -8 }, { 21, -3 }, { 22, -16 }, { 25, -9 } };
	//int pola = 6;
	//map<int, int> polapki = { { 4, -2 }, { 5, -2 } };

	Plansza *plansza = new Plansza(pola, polapki);

	Zagadka *rozwiazana_zagadka = new Zagadka(*plansza);
	vector<Niewiadoma> wektor_rownan = rozwiazana_zagadka->analiza();

	Macierz *m = new Macierz(wektor_rownan);
	cout << "Wynik algorytmem Gaussa: ";
	cout << m->gauss() << endl;

	Macierz *ma = new Macierz(wektor_rownan);
	cout << "Wynik algorytmem iteracyjnym Gaussa-Seidela: ";
	ma->iteracyjne();

	MonteCarlo *gra = new MonteCarlo(plansza, 100);
	cout << "Wynik Monte Carlo: " << gra->wynik() << endl;

	return 0;
}
