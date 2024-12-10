
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>

using namespace std;

// *** jatekos oszt�ly ***
// Priv�t adattagok: nev (string), pontszam (int), talalat (int),
// jatekos_szam (statikus int), attack (int), defense (int)
// Getter �s setter f�ggv�nyek az adattagokhoz
// Konstruktor: param�tere a n�v, pontszam=0, talalat=0
// Az attack �s defense �rt�ket k�s�bb a General f�ggv�ny �ll�tja be

class jatekos {
private:
    string nev;
    int pontszam;
    int talalat;
    int attack;
    int defense;
    static int jatekos_szam; // statikus adattag a j�t�kosok sz�m�nak nyilv�ntart�s�ra

public:
    jatekos(const string& n) : nev(n), pontszam(0), talalat(0), attack(0), defense(0) {
        jatekos_szam++;
    }

    // Getterek
    string getNev() const { return nev; }
    int getPontszam() const { return pontszam; }
    int getTalalat() const { return talalat; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }

    // Setterek
    void setNev(const string& n) { nev = n; }
    void setPontszam(int p) { pontszam = p; }
    void setTalalat(int t) { talalat = t; }
    void setAttack(int a) { attack = a; }
    void setDefense(int d) { defense = d; }

    static int getJatekosSzam() { return jatekos_szam; }

    void novTalalatPont() {
        talalat++;
        pontszam++;
    }
};

int jatekos::jatekos_szam = 0; // Statikus adattag defin�ci�ja


// *** General f�ggv�ny ***
// V�letlenszer�en oszt el 10 egys�get attack �s defense k�z�tt
void General(jatekos& j) {
    int sum = 10;
    int att = rand() % (sum + 1); // att 0-t�l 10-ig
    int def = sum - att;
    j.setAttack(att);
    j.setDefense(def);
}


// *** csata oszt�ly ***
// Tartalmaz egy max 10 elem� jatekos t�mb�t.
// Konstruktorban inicializ�ljuk, destruktorban felszabad�tjuk a dinamikusan l�trehozott j�t�kosokat.
// Hozzaad f�ggv�ny: �j j�t�kos hozz�ad�sa, ha van hely.
// Csatra_fel: minden j�t�kos 5-sz�r dob h�goly�t, v�letlen c�lponttal �s val�sz�n�s�gi tal�latvizsg�lattal.
// Rendezes: bubor�krendez�s pontszam, majd talalat alapj�n cs�kken� sorrendbe.
// Kiir, KiirFajlba: Eredm�nyek ki�r�sa.

class csata {
private:
    jatekos* jatekosok[10];
    int aktualis_letszam;

public:
    csata() : aktualis_letszam(0) {
        for (int i = 0; i < 10; i++) {
            jatekosok[i] = nullptr;
        }
    }

    ~csata() {
        for (int i = 0; i < aktualis_letszam; i++) {
            delete jatekosok[i];
            jatekosok[i] = nullptr;
        }
    }

    void Hozzaad(const string& nev) {
        if (aktualis_letszam < 10) {
            jatekos* uj = new jatekos(nev);
            // Attack/defense be�ll�t�sa a General f�ggv�nnyel
            General(*uj);
            jatekosok[aktualis_letszam] = uj;
            aktualis_letszam++;
        }
        else {
            cout << nev << " mar nem tud csatlakozni ebben a korben" << endl;
        }
    }

    void Csatra_fel() {
        for (int i = 0; i < aktualis_letszam; i++) {
            for (int dob = 0; dob < 5; dob++) {
                int cel;
                do {
                    cel = rand() % aktualis_letszam;
                } while (cel == i); // A c�lpont ne legyen ugyanaz a j�t�kos

                int att = jatekosok[i]->getAttack();
                int def = jatekosok[cel]->getDefense();

                // P�lda val�sz�n�s�gi formula:
                double p = (att - def + 10) * 0.05;
                if (p < 0) p = 0;
                if (p > 1) p = 1;

                double r = (double)rand() / RAND_MAX;
                if (r < p) {
                    // Tal�lat
                    jatekosok[i]->novTalalatPont();
                }
            }
        }
    }

    void Rendezes() {
        bool csere = true;
        while (csere) {
            csere = false;
            for (int i = 0; i < aktualis_letszam - 1; i++) {
                int p1 = jatekosok[i]->getPontszam();
                int p2 = jatekosok[i + 1]->getPontszam();
                int t1 = jatekosok[i]->getTalalat();
                int t2 = jatekosok[i + 1]->getTalalat();

                // Cs�kken� sorrend
                if ((p1 < p2) || ((p1 == p2) && (t1 < t2))) {
                    jatekos* tmp = jatekosok[i];
                    jatekosok[i] = jatekosok[i + 1];
                    jatekosok[i + 1] = tmp;
                    csere = true;
                }
            }
        }
    }

    void Kiir() {
        cout << "Nev;Pontszam;Talalat" << endl;
        for (int i = 0; i < aktualis_letszam; i++) {
            cout << jatekosok[i]->getNev() << ";"
                << jatekosok[i]->getPontszam() << ";"
                << jatekosok[i]->getTalalat() << endl;
        }
    }

    void KiirFajlba(const string& fajlnev) {
        ofstream out(fajlnev.c_str());
        if (!out) {
            cerr << "Hiba a fajl megnyitasakor: " << fajlnev << endl;
            return;
        }
        out << "Nev;Pontszam;Talalat" << endl;
        for (int i = 0; i < aktualis_letszam; i++) {
            out << jatekosok[i]->getNev() << ";"
                << jatekosok[i]->getPontszam() << ";"
                << jatekosok[i]->getTalalat() << endl;
        }
        out.close();
    }
};

int main() {
    srand((unsigned int)time(NULL));

    csata c;

    // A felhaszn�l�t�l bek�rj�k, h�ny j�t�kost szeretne hozz�adni
    int db;
    cout << "Hany jatekost szeretne hozzaadni? (max 10): ";
    cin >> db;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // newline eldob�sa

    for (int i = 0; i < db; i++) {
        cout << "Adja meg a(z) " << i + 1 << ". jatekos nevet: ";
        string nev;
        std::getline(cin, nev);
        c.Hozzaad(nev);
    }

    // Lefuttatjuk a szimul�ci�t
    c.Csatra_fel();

    // Rendez�s pontsz�m �s tal�lat alapj�n cs�kken� sorrendbe
    c.Rendezes();

    // Eredm�nyek ki�r�sa konzolra
    c.Kiir();

    // Eredm�nyek ki�r�sa f�jlba
    c.KiirFajlba("eredmeny.csv");

    return 0;
}
