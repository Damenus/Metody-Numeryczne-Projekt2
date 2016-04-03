#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <set>


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
	int gracz1Pozycja, gracz2Pozycja, gracz, wynik;
	vector<ruchw> tab;
	int pewne = 0;
	int numer = 0;

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
			macierz[i] = new double[rozmiar + 1]; //+1 na macierz wyrazów wolnych

		for (int j = 0; j < rozmiar; j++) {
			for (int k = 0; k <= rozmiar; k++){
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
			this->macierz[i][wektor.size() + 1] = (double)wektor[i].pewne / 6.0;
		}
		
	}

	void wypisz_macierz() {
		for (int j = 0; j < rozmiar_macierzy; j++) {
			cout << j << ": ";
			for (int k = 0; k < rozmiar_macierzy+2; k++){
				cout << macierz[j][k] << " ";
			}
			cout << endl;
		}
	}

	void gauss() {

		//eliminacja
		double m;
		for (int i = 0; i < this->rozmiar_macierzy; i++) { // ka¿da kolumna pokolei

			for (int j = 1; j < this->rozmiar_macierzy; j++) {
				if (macierz[0][i] != 0) { // zero
					m = macierz[j][i] / macierz[0][i]; //wspo³czynnik mno¿enia pierwszego weirsza, by odj¹æ od kolejnych kolumn, by wyzerowaæ dolny trójk¹t
					for (int k = 0; k < this->rozmiar_macierzy + 1; k++) { // zerowanie kolumny
						macierz[j][k] = macierz[j][k] - m * macierz[0][k];
					}
				}
			}
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
	/*
	for (int i = 0; i < pola; i++) {
		for (int j = 0; j < pola; j++) {

			if (i != 4 && j != 4 && i != 5 && j!= 5) {
				if ( (j != 0 && i != 0) || (i == 0 && j == 0)) {
					ruch x = ruch(i, j, 1);
					lista.push_back(x);
				}
				if (i != 0) {
					ruch y = ruch(i, j, 2);
					lista.push_back(y);
				}
			}

		}
	}
	*/

	cout << "Start ";

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
	m->wypelnienie(wektor2);
	m->wypisz_macierz();
	m->gauss();
	m->wypisz_macierz();
	return 0;
}

/*


Platform.runLater(new Runnable() {
@Override public void run() {
mpb.getProgressBar().setProgress(mpb.getActualProgress()/mpb.getTotalProgress());
}
});
*/