#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <ctime>
#include <cstdlib>


using namespace std;

class ruch;
class ruchw;

class ruchw{
public:
	int wspolczynnik;
	ruch *niewiadoma;
	//int numer;

	ruchw() {

	}

	ruchw(int a, ruch *b) {
		wspolczynnik = a;
		niewiadoma = b;
		//numer = niewiadoma->numer;
	}
};

class ruch
{	
//(pole gdzie gracz1, pole gdzie gracz2, który gracz ma ruch) 
public: 
	int gracz1Pozycja, gracz2Pozycja, gracz;
	vector<ruchw> tab;
	int pewne = 0;
	int numer = 0;
	double wynik = 1.0;
	double wynik2 = 0.0;

public:
	ruch() {

	}

	ruch(int a, int b, int c) {
		gracz1Pozycja = a;
		gracz2Pozycja = b;
		gracz = c;
		
	}

	bool wstaw(ruch *nowyruch) {
		ruch *findy = new ruch();
		//if (find(wektor.begin(), wektor.end(), *nowyruch) != wektor.end()) {
		for (int is = 0; is < tab.size(); is++) {
			findy = tab[is].niewiadoma;
			if (findy->gracz != nowyruch->gracz || findy->gracz1Pozycja != nowyruch->gracz1Pozycja || findy->gracz2Pozycja != nowyruch->gracz2Pozycja)
				continue;
			else {
				tab[is].wspolczynnik++;
				return true;
				break;
			}
		}
		ruchw *r = new ruchw(1, nowyruch);
		tab.push_back(*r);
		return false;
	}

};

class Macierz {
public:
	double **macierz;
	int rozmiar_macierzy;

	Macierz(int rozmiar){
		rozmiar_macierzy = rozmiar;
		macierz = new double*[rozmiar];
		for (int i = 0; i < rozmiar; i++)
			macierz[i] = new double[rozmiar]; //+1 na macierz wyrazów wolnych

		for (int j = 0; j < rozmiar; j++) {
			for (int k = 0; k < rozmiar; k++){
				macierz[j][k] = 0;
			}
		}
	}

	int znajdz_numer_kolumny(vector<ruch> wektor, ruch *szukany) {
		ruch *findy = new ruch();
		for (int is = 0; is < wektor.size(); is++) {
			*findy = wektor[is];
			if (findy->gracz != szukany->gracz || findy->gracz1Pozycja != szukany->gracz1Pozycja || findy->gracz2Pozycja != szukany->gracz2Pozycja)
				continue;
			else {
				return is;
				break;
			}
		}
		return false;
	}

	void wypelnienie(vector<ruch> wektor) {
		double a, b;
		int c, d;
		for (int i = 0; i < wektor.size(); i++) {
			for (int j = 0; j < wektor[i].tab.size(); j++) {
				//c = wektor[i].tab[j].niewiadoma->numer;
				c = znajdz_numer_kolumny(wektor, wektor[i].tab[j].niewiadoma);
				d = wektor[i].tab[j].wspolczynnik;
				this->macierz[i][c] =  (double)d / 6.0;

			}
			
			this->macierz[i][i] = 1;
			this->macierz[i][wektor.size()] = (double)wektor[i].pewne / 6.0;
		}
		
	}

	void wypelnienieGauss(vector<ruch> wektor) {
		double a, b;
		int c, d;
		for (int i = 0; i < wektor.size(); i++) {
			for (int j = 0; j < wektor[i].tab.size(); j++) {
				//c = wektor[i].tab[j].niewiadoma->numer;
				c = znajdz_numer_kolumny(wektor, wektor[i].tab[j].niewiadoma);
				d = -wektor[i].tab[j].wspolczynnik;
				this->macierz[i][c] = (double)d / 6.0;

			}

			this->macierz[i][i] = 1;
			this->macierz[i][wektor.size()] = (double)wektor[i].pewne / 6.0;
		}

	}

	void wypisz_macierz() {
		for (int j = 0; j < rozmiar_macierzy; j++) {
			cout << j << ": ";
			for (int k = 0; k <= rozmiar_macierzy; k++){
				cout << macierz[j][k] << " ";
			}
			cout << endl;
		}
	}

	void gauss(vector<ruch> wektor) {

		//eliminacja
		double m;
		for (int i = 0; i < this->rozmiar_macierzy; i++) { // ka¿da kolumna pokolei

			for (int j = i+1; j < this->rozmiar_macierzy; j++) {

				m = macierz[j][i] / macierz[i][i]; //wspo³czynnik mno¿enia pierwszego weirsza, by odj¹æ od kolejnych kolumn, by wyzerowaæ dolny trójk¹t
				for (int k = i; k < this->rozmiar_macierzy + 1; k++) { // zerowanie kolumny
					macierz[j][k] = macierz[j][k] - m * macierz[i][k];
				}				
			}
		}

		//wypisz_macierz();

		double s = 0;
		//obliczanie
		int n = wektor.size() - 1;
		wektor[n].wynik = macierz[n][n + 1] / macierz[n][n];

		for (int i = n - 1; i >= 0; i--){
			s = 0;
			for (int k = i + 1; k <= n; k++) {
				s = s + macierz[i][k] * wektor[k].wynik;
				wektor[i].wynik = (macierz[i][n + 1] - s) / macierz[i][i];
			}
		}

		cout << endl << wektor[0].wynik << endl;

	}

	void iteracyjne(vector<ruch> wektor) {

		double c;
		for (int obieg = 0; obieg < 5; obieg++) {

			for (int i = 0; i < rozmiar_macierzy; i++) {//dla ka¿dego wiersza
				for (int j = 0; j < rozmiar_macierzy; j++) {
					if (i != j) {
						c = this->macierz[i][j] * wektor[j].wynik2;
						wektor[i].wynik2 -= c;
					}
				}
				wektor[i].wynik2 += this->macierz[i][rozmiar_macierzy]; // wyrazy wolne
			}

			for (int i = 0; i < wektor.size(); i ++)
				cout << i << ": " << wektor[i].wynik2 << endl;
		}

		cout  << wektor[0].wynik2 << endl;
	}

	void iteracyjne2(vector<ruch> wektor) {

		int stop = 15;
		bool koniec = true;
		int n = wektor.size();
		double eps = 0.000001;
		vector<ruch> wektor2;
		float *B2 = new float[n];

		do
		{
			// przepisanie x - aktualnych wynikow do x2 - wynikow z poprzedniej iteracji
		//	for (int i = 0; i < n; i++)
				//x2[i] = x[i];
				wektor2 = wektor;

			// wykonanie kolejnej iteracji
			for (int i = 0; i < n; i++)
			{
				B2[i] = macierz[i][rozmiar_macierzy];//wyraz wolny

				for (int j = 0; j < i; j++)
					B2[i] -= (macierz[i][j] * wektor[j].wynik2);

				for (int j = i + 1; j < n; j++)
					B2[i] -= macierz[i][j] * wektor2[j].wynik2;

				wektor[i].wynik2 = B2[i] / macierz[i][i];
			}

			// sprawdzenie warunku zakonczenia: ||x(k)-x(k-1)|| <= epsilon
			for (int i = 0; i < n; i++)
			{
				if (fabs(wektor[i].wynik2 -wektor2[i].wynik2) > eps) { koniec = true; break; }
				else koniec = false;
			}
			stop--;
		} while (koniec && stop > 0);

		cout << wektor[0].wynik2 << endl;
	}
};

class Plansza{
public:
	int ilosc_pol;
	map<int, int> polapki;

	Plansza(int ilosc_pol,	map<int, int> polapki) {
		this->ilosc_pol = ilosc_pol;
		this->polapki = polapki;
	}
};

class MonteCarlo {
public:
	Plansza *plansza;
	int ilosc_gier = 1000000;

	MonteCarlo(Plansza *plansza) {
		this->plansza = plansza;
		srand(time(NULL));
	}

	int rzutKoscia() {
		return ( (rand() % 6) + 1 );
	}

	double wynik() {
		
		int tablica_wygranych[2] = { 0, 0 };

		for (int przebieg = 0; przebieg < this->ilosc_gier; przebieg++) {
			tablica_wygranych[gra()]++;
		}

		return (double)tablica_wygranych[0] / (double)this->ilosc_gier;

	}

	int gra() {
		//pozycja na planszy graczy
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

int ktorynumer = 0;

vector<ruch> wektor;
vector<ruch> wektor2;
//int pola = 28;
//map<int, int> polapki = { { 1, -1 }, { 4, -1 }, { 6, -3 }, { 7, -2 }, { 13, -4 }, { 14, -5 }, { 17, -1 }, { 20, -2 }, { 21, -2 }, { 22, -10 }, { 24, -14 }, { 25, -2 }, { 27, -8 } };

int pola = 6;
map<int, int> polapki = { { 4, -2 }, { 5, -2 } };

//int pola = 27;
//map<int, int> polapki = { { 4, -2 }, { 5, -2 }, { 7, -5 }, { 9, -3 }, { 14, -12 }, { 15, -2 }, { 17, -7 }, { 19, -8 }, { 21, -3 }, { 22, -16 }, { 25, -9 } };

queue<ruch> kolejka;


class Zagadka {

	bool znajdz(ruch *nowyruch) {
		ruch *findy = new ruch();
		//if (find(wektor.begin(), wektor.end(), *nowyruch) != wektor.end()) {
		for (int is = 0; is < wektor.size(); is++) {
			*findy = wektor[is];
			if (findy->gracz != nowyruch->gracz || findy->gracz1Pozycja != nowyruch->gracz1Pozycja || findy->gracz2Pozycja != nowyruch->gracz2Pozycja)
				continue;
			else {
				return true;
				break;
			}
		}

		return false;
	}

	void analiza() {
		int ktorygracz, mozliwapulapka;

		ruch *pierwszy = new ruch(0, 0, 1);
		kolejka.push(*pierwszy);
		wektor.push_back(*pierwszy);

		ruch analizowany;
		ruch *nowyruch;

		while (!kolejka.empty())
		{
			analizowany = kolejka.front();
			kolejka.pop();

			analizowany.numer = ktorynumer; // numeruje zmienne
			ktorynumer++;

			ktorygracz = analizowany.gracz % 2 + 1; //modulo 2 + 1 daje przeciwnego gracza
			mozliwapulapka = (ktorygracz == 2) ? analizowany.gracz1Pozycja : analizowany.gracz2Pozycja;

			for (int i = 1; i <= 6; i++) {
				if (analizowany.gracz1Pozycja + i *  ((int)ktorygracz / 2) < pola && analizowany.gracz2Pozycja + i *abs(ktorygracz - 2) < pola) {

					if (polapki.find(mozliwapulapka + i) != polapki.end()) { //nie znlezioon 
						nowyruch = new ruch(analizowany.gracz1Pozycja + (i + polapki[mozliwapulapka + i]) *((int)ktorygracz / 2), analizowany.gracz2Pozycja + (i + polapki[mozliwapulapka + i]) * abs(ktorygracz - 2), ktorygracz); //koment
					}
					else { // znaleziono 
						nowyruch = new ruch(analizowany.gracz1Pozycja + i *  ((int)ktorygracz / 2), analizowany.gracz2Pozycja + i *abs(ktorygracz - 2), ktorygracz);
					}

					analizowany.wstaw(nowyruch);

					if (znajdz(nowyruch) == false) {
						wektor.push_back(*nowyruch);
						kolejka.push(*nowyruch);
					}

				}
				else { // dodaje pewne zwyciêstwo
					if (analizowany.gracz == 1)
						analizowany.pewne += 1;
				}

			}

			wektor2.push_back(analizowany);


		}
	}


	void wypisz_rownania() {
		cout << "Iloœæ równañ: " << wektor2.size() << endl;
		for (int iter = 0; iter < wektor2.size(); iter++) {
			cout << iter << ":" << wektor2[iter].gracz1Pozycja << wektor2[iter].gracz2Pozycja << wektor2[iter].gracz << " : ";
			for (int iter2 = 0; iter2 < wektor2[iter].tab.size(); iter2++) {
				cout << wektor2[iter].tab[iter2].wspolczynnik << " " << wektor2[iter].tab[iter2].niewiadoma->gracz1Pozycja << wektor2[iter].tab[iter2].niewiadoma->gracz2Pozycja << wektor2[iter].tab[iter2].niewiadoma->gracz << " , ";
			}
			cout << " " << wektor2[iter].pewne << endl;
		}
	}
};


bool znajdz(ruch *nowyruch) {
	ruch *findy = new ruch();	
	//if (find(wektor.begin(), wektor.end(), *nowyruch) != wektor.end()) {
	for (int is = 0; is < wektor.size(); is++) {
		*findy = wektor[is];
		if (findy->gracz != nowyruch->gracz || findy->gracz1Pozycja != nowyruch->gracz1Pozycja || findy->gracz2Pozycja != nowyruch->gracz2Pozycja)
			continue;
		else {
			return true;
			break;
		}
	}
	
	return false;
}


int main()
{

	int ktorygracz, mozliwapulapka;

	ruch *pierwszy = new ruch(0, 0, 1);
	kolejka.push(*pierwszy);
	wektor.push_back(*pierwszy);

	ruch analizowany;
	ruch *nowyruch;

	while (!kolejka.empty())
	{
		analizowany = kolejka.front();
		kolejka.pop();

		analizowany.numer = ktorynumer; // numeruje zmienne
		ktorynumer++;

		ktorygracz = analizowany.gracz % 2 + 1; //modulo 2 + 1 daje przeciwnego gracza
		mozliwapulapka = (ktorygracz == 2) ? analizowany.gracz1Pozycja : analizowany.gracz2Pozycja;

		for (int i = 1; i <= 6; i++) {
			if (analizowany.gracz1Pozycja + i *  ((int)ktorygracz / 2) < pola && analizowany.gracz2Pozycja + i *abs(ktorygracz - 2) < pola) {
				
				if (polapki.find(mozliwapulapka + i) != polapki.end()) { //nie znlezioon 
					nowyruch = new ruch(analizowany.gracz1Pozycja + (i + polapki[mozliwapulapka + i]) *((int)ktorygracz / 2), analizowany.gracz2Pozycja + (i + polapki[mozliwapulapka + i]) * abs(ktorygracz - 2), ktorygracz); //koment
				} 
				else { // znaleziono 
					nowyruch = new ruch(analizowany.gracz1Pozycja + i *  ((int)ktorygracz / 2), analizowany.gracz2Pozycja + i *abs(ktorygracz - 2), ktorygracz);
				}
				
				analizowany.wstaw(nowyruch);				

				if (znajdz(nowyruch) == false) {
					wektor.push_back(*nowyruch);
					kolejka.push(*nowyruch);
				}

			} else { // dodaje pewne zwyciêstwo
				if (analizowany.gracz == 1)
				analizowany.pewne += 1;
			}

		}
		
		wektor2.push_back(analizowany);
		

	}

	cout << wektor2.size()<< endl;
	for (int iter = 0; iter < wektor2.size(); iter++) {
		cout << iter << ":" << wektor2[iter].gracz1Pozycja << wektor2[iter].gracz2Pozycja << wektor2[iter].gracz << " : ";
		for (int iter2 = 0; iter2 < wektor2[iter].tab.size(); iter2++) {
			cout << wektor2[iter].tab[iter2].wspolczynnik << " " << wektor2[iter].tab[iter2].niewiadoma->gracz1Pozycja << wektor2[iter].tab[iter2].niewiadoma->gracz2Pozycja << wektor2[iter].tab[iter2].niewiadoma->gracz <<  " , ";
		}
		
		cout << " " << wektor2[iter].pewne << endl;
	}





	Macierz *m = new Macierz(wektor2.size());
//	m->wypelnienieGauss(wektor2);
//	m->wypisz_macierz();
//	m->gauss(wektor2);
//	m->wypisz_macierz();

	m->wypelnienie(wektor2);
	//m->wypisz_macierz();
	m->iteracyjne2(wektor2);
	//m->wypisz_macierz();


	//Plansza *plansza = new Plansza(pola, polapki);
	//MonteCarlo *gra = new MonteCarlo(plansza);
	//cout << gra->wynik() << endl;;



	return 0;
}
