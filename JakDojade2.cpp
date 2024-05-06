#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdio>
#include <climits>
#pragma warning(disable : 4996)
using namespace std;

#define lineLen 100

template<typename T>
class Wektor;

template<typename T>
class Kolejka;

class MinimalHeap;

class punkt;

class miasto;

class Somsiad;

class prioQ;

class retardedString;


void dijkstra(punkt start, punkt stop, int typ, miasto* tab, int rozmiarTab, int rows, int cols);

void JakDojade();

int main() {
    JakDojade();
    return 0;
}

template<typename T>
class Kolejka {
public:
    Wektor<T> pojemnik;
    int size = 0;

    void enqueue(T& value) {
        pojemnik.dodaj(value);
        size++;
    }
    void dequeue() {
        pojemnik.erase(0);
        size--;
    }
    T& operator[](int i) {
        return pojemnik[i];
    }
};

class retardedString {
public:
    char* text;
    int len;
    retardedString(char* input, int dlugosc) {
        text = input;
        len = dlugosc;
    }

    retardedString() {
        len = 0;
    }

    void wypisz() {
        putchar(32);
        for (int i = 0; i < len; i++)
            putchar(text[i]);
    }

};

class punkt {
public:
    int x;
    int y;
    bool operator==(punkt pkt) const {
        if (this->x == pkt.x && this->y == pkt.y)
            return true;
        else
            return false;
    }
};






class Somsiad {
public:
    int indexRodzica;
    punkt start;
    punkt polozenie;
    int nameLen;
    char* nazwa;
    int droga;
    Somsiad() {
        droga = 0;
    }
    Somsiad(punkt pkt) {
        droga = 0;
        polozenie = pkt;
    }
    void stworzSasiada(punkt pkt, int len, char** mapa, int maxRow, int maxCol, punkt startt, int rodzic) {
        this->indexRodzica = rodzic;
        this->start = startt;
        polozenie = pkt;
        droga = len + 1;
        bool found = false;
        int szukanyX;
        int szukanyY;
        for (int i = pkt.x - 1; i <= pkt.x + 1; i++) {
            for (int j = pkt.y - 1; j <= pkt.y + 1; j++) {
                if (i < 0 || j < 0 || i >= maxCol || j >= maxRow)
                    continue;
                else {
                    if (mapa[j][i] > '.') {
                        found = true;
                        szukanyY = j;
                        szukanyX = i;
                        break;
                    }
                }
            }
            if (found)
                break;
        }

        int szukanyXcpy = szukanyX;
        while (szukanyX > 0 && mapa[szukanyY][szukanyX - 1] > '.')
            szukanyX--;
        while (szukanyXcpy < maxCol && mapa[szukanyY][szukanyXcpy + 1] > '.')
            szukanyXcpy++;
        int leng = szukanyXcpy - szukanyX + 1;
        nazwa = new char[leng];
        nameLen = leng;
        strncpy(nazwa, &mapa[szukanyY][szukanyX], leng);
    }
};






template <typename D>
class Wektor {
public:
    D* zawartosc;
    int cap;
    int size;

    Wektor() {
        cap = 100;                      //z parametrem nie dzialalo
        size = 0;
        zawartosc = new D[cap];
    }
    void dodaj(const D &wartosc) {
        if (size == cap)
            rozszerz(size * 2);
        zawartosc[size] = wartosc;
        size++;
    }
    void rozszerz(int newCap) {
        D* nowyWektor = new D[newCap];
        for (int i = 0; i < cap; i++)
            nowyWektor[i] = zawartosc[i];
        delete[] zawartosc;
        zawartosc = nowyWektor;
        cap = newCap;
    }
    void erase(int index) {
        for (int i = index; i < size - 1; i++)
            zawartosc[i] = zawartosc[i + 1];
        size--;
    }
    D& operator[](int i) {
        return zawartosc[i];
    }
};

class MinimalHeap {
public:
    Wektor<Somsiad> heap;
    void heapify_dodaj(int index) {
        while (index != 0) {
            int parent = (index - 1) / 2;
            if (heap[parent].droga > heap[index].droga) {
                Somsiad temp = heap[parent];
                heap[parent] = heap[index];
                heap[index] = temp;
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void heapify_usun(int index) {
        while (index < heap.size) {
            int childLeft = 2 * index + 1;
            int childRight = 2 * index + 2;
            int smallestNode = index;
            if (childLeft < heap.size && heap[childLeft].droga < heap[smallestNode].droga)
                smallestNode = childLeft;
            if (childRight < heap.size && heap[childRight].droga < heap[smallestNode].droga)
                smallestNode = childRight;
            if (smallestNode != index) {
                Somsiad temp = heap[index];
                heap[index] = heap[smallestNode];
                heap[smallestNode] = temp;
                index = smallestNode;
            }
            else
                break;
        }

    }

    void insert(Somsiad sasiad) {
        heap.dodaj(sasiad);
        heapify_dodaj(heap.size - 1);
    }

    Somsiad extract() {
        Somsiad retValue = heap[0];
        heap.erase(0);
        heapify_usun(0);
        return retValue;
    }
};



class miasto {
public:
    punkt polozenie;
    int nameLen;
    int indeks;
    char* nazwa;
    Wektor<Somsiad*> sasiedzi;
    miasto() {
        sasiedzi.cap = 2;
    }
    char* wypisz() {
        return nazwa;
    }
    void TworzMiasto(punkt pkt, char** mapa, int maxRow, int maxCol, int index) {
        indeks = index;
        polozenie = pkt;
        bool found = false;
        int szukanyX;
        int szukanyY;
        for (int i = pkt.x - 1; i <= pkt.x + 1; i++) {
            for (int j = pkt.y - 1; j <= pkt.y + 1; j++) {
                if (i < 0 || j < 0 || i >= maxCol || j >= maxRow)
                    continue;
                else {
                    if (mapa[j][i] > '.') {
                        found = true;
                        szukanyY = j;
                        szukanyX = i;
                        break;
                    }
                }
            }
            if (found)
                break;
        }

        int szukanyXcpy = szukanyX;
        while (szukanyX > 0 && mapa[szukanyY][szukanyX - 1] > '.')
            szukanyX--;
        while (szukanyXcpy < maxCol && mapa[szukanyY][szukanyXcpy + 1] > '.')
            szukanyXcpy++;
        int len = szukanyXcpy - szukanyX + 1;
        nazwa = new char[len];
        nameLen = len;
        strncpy(nazwa, &mapa[szukanyY][szukanyX], len);
    }
    void dodajLot(char* kierunek, int odleglosc, punkt pkt, int index) {
        Somsiad* s = new Somsiad;
        s->indexRodzica = index;
        s->start = this->polozenie;
        s->polozenie = pkt;
        s->nazwa = kierunek;
        s->droga = odleglosc;
        sasiedzi.dodaj(s);
    }
    void bfs(miasto mst, char** mapa, int maxRow, int maxCol, miasto* miasta) {
        int** mapInt = new int* [maxRow];
        for (int i = 0; i < maxRow; i++)
            mapInt[i] = new int[maxCol];
        for (int i = 0; i < maxRow; i++) {
            for (int j = 0; j < maxCol; j++)
                mapInt[i][j] = -1;
        }
        Kolejka<punkt> q;
        mapInt[mst.polozenie.y][mst.polozenie.x] = 0;                           //punkt startowy
        int kierX[4] = { -1,1,0,0 };
        int kierY[4] = { 0,0,1,-1 };
        q.enqueue(mst.polozenie);
        while (q.size != 0)
        {
            for (int i = 0; i < 4; i++) {
                if (!(q[0].x + kierX[i] < 0 || q[0].y + kierY[i] < 0 || q[0].x + kierX[i] >= maxCol || q[0].y + kierY[i] >= maxRow)) {
                    if (mapInt[q[0].y + kierY[i]][q[0].x + kierX[i]] == -1) {
                        if (mapa[q[0].y + kierY[i]][q[0].x + kierX[i]] == '#') {
                            mapInt[q[0].y + kierY[i]][q[0].x + kierX[i]] = mapInt[q[0].y][q[0].x] + 1;
                            punkt pkt = { q[0].x + kierX[i] ,q[0].y + kierY[i] };
                            q.enqueue(pkt);
                        }
                        else if (mapa[q[0].y + kierY[i]][q[0].x + kierX[i]] == '*') {
                            punkt pkt1 = { q[0].x + kierX[i] ,q[0].y + kierY[i] };
                            mapInt[q[0].y + kierY[i]][q[0].x + kierX[i]] = -2;
                            Somsiad* s = new Somsiad(pkt1);
                            int j = 0;
                            while (!(miasta[j].polozenie == pkt1)) {
                                j++;
                            }
                            s->stworzSasiada(pkt1, mapInt[q[0].y][q[0].x], mapa, maxRow, maxCol, this->polozenie, this->indeks);
                            sasiedzi.dodaj(s);
                        }
                    }
                }
            }
            q.dequeue();
        }
        for (int i = 0; i < maxRow; i++)
            delete[] mapInt[i];
        delete[] mapInt;

    }
};

void dijkstra(punkt start, punkt stop, int typ, miasto* tab, int rozmiarTab, int rows, int cols) {
    if (start == stop) {
        cout << '0' << '\n';
        return;
    }
    int* lastIndex = new int[rozmiarTab];
    for (int i = 0; i < rozmiarTab; i++)
        lastIndex[i] = -1;
    bool** visitedMap = new bool* [rows];
    int** distanceMap = new int* [rows];
    bool foundPath = false;
    for (int i = 0; i < rows; i++) {
        visitedMap[i] = new bool[cols];
        distanceMap[i] = new int[cols];
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            visitedMap[i][j] = false;
            distanceMap[i][j] = INT_MAX;
        }
    }
    distanceMap[start.y][start.x] = 0;
    visitedMap[start.y][start.x] = true;
    MinimalHeap heap;
    punkt current = start;
    int index;
    while (!foundPath) {
        index = 0;
        while (index < rozmiarTab) {
            if (current == tab[index].polozenie)
                break;
            index++;
        }
        for (int i = 0; i < tab[index].sasiedzi.size; i++) {
            if (!visitedMap[tab[index].sasiedzi[i]->polozenie.y][tab[index].sasiedzi[i]->polozenie.x]) {
                heap.insert(*tab[index].sasiedzi[i]);
            }

            if (distanceMap[tab[index].sasiedzi[i]->polozenie.y][tab[index].sasiedzi[i]->polozenie.x] > tab[index].sasiedzi[i]->droga + distanceMap[current.y][current.x])
                distanceMap[tab[index].sasiedzi[i]->polozenie.y][tab[index].sasiedzi[i]->polozenie.x] = tab[index].sasiedzi[i]->droga + distanceMap[current.y][current.x];
        }


        while (heap.heap.size != 0) {
            if (!visitedMap[heap.heap[0].polozenie.y][heap.heap[0].polozenie.x] && distanceMap[heap.heap[0].start.y][heap.heap[0].start.x] + heap.heap[0].droga == distanceMap[heap.heap[0].polozenie.y][heap.heap[0].polozenie.x]) {
                int indexHelp = heap.heap[0].indexRodzica;
                current = heap.heap[0].polozenie;
                index = 0;
                while (index < rozmiarTab) {
                    if (current == tab[index].polozenie)
                        break;
                    index++;
                }
                lastIndex[index] = indexHelp;
                visitedMap[current.y][current.x] = true;
                if (current == stop) {
                    foundPath = true;
                }
                heap.extract();
                break;
            }
            heap.extract();
        }
    }

    Wektor<retardedString> stringi;
    while (lastIndex[index] != -1) {
        char* nazwa = tab[index].nazwa;
        int nameLen = tab[index].nameLen;
        retardedString* rs = new retardedString(nazwa, nameLen);
        stringi.dodaj(*rs);
        index = lastIndex[index];
    }


    cout << distanceMap[stop.y][stop.x];
    if (typ == 1) {
        for (int i = stringi.size - 1; i > 0; i--) {
            stringi[i].wypisz();
        }
    }
    cout << '\n';
    for (int i = 0; i < rows; i++) {
        delete[] distanceMap[i];
        delete[] visitedMap[i];
    }
    delete[] distanceMap;
    delete[] visitedMap;
    delete[] lastIndex;

}


void JakDojade() {
    Wektor<punkt> punkty;
    int rows, cols, loty;
    scanf("%d", &cols);
    scanf("%d", &rows);
    char** mapa;
    mapa = new char* [rows];
    for (int i = 0; i < rows; i++) {
        mapa[i] = new char[cols];
    }
    char c;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            c = getchar();
            if (c == '*')
                punkty.dodaj({ j,i });
            if (c > ' ') {
                mapa[i][j] = c;
            }
            else
                j--;
        }
    }
    miasto* miasta = new miasto[punkty.size];
    for (int i = 0; i < punkty.size; i++)
        miasta[i].TworzMiasto(punkty[i], mapa, rows, cols, i);
        for (int i = 0; i < punkty.size; i++)
            miasta[i].bfs(miasta[i], mapa, rows, cols, miasta);
    for (int i = 0; i < rows; i++)
        delete[] mapa[i];
    delete[] mapa;
    scanf("%d", &loty);
    cin.ignore();
    for (int i = 0; i < loty; i++) {
        char* line = new char[lineLen];
        fgets(line, lineLen, stdin);

        int len1 = 0;
        int len2 = 0;
        bool space = false;
        for (int j = 0; j < lineLen; j++) {
            if (space && line[j] == ' ')
                break;
            else if (line[j] == ' ')
                space = true;
            else if (!space)
                len1++;
            else if (space)
                len2++;
        }
        char* start = new char[len1];
        char* ladowanie = new char[len2];
        int odleglosc;

        sscanf(line, "%s %s %d", start, ladowanie, &odleglosc);
        delete[] line;
        punkt pkt;
        for (int j = 0; j < punkty.size; j++) {
            bool znaleziony = true;
            for (int k = 0; k < len1; k++) {
                if (start[k] != miasta[j].nazwa[k] || len1 != miasta[j].nameLen) {
                    znaleziony = false;
                    break;
                }
            }
            if (znaleziony) {
                for (int m = 0; m < punkty.size; m++) {
                    bool destynacja = true;
                    for (int k = 0; k < len2; k++) {
                        if (ladowanie[k] != miasta[m].nazwa[k] || len2 != miasta[m].nameLen) {
                            destynacja = false;
                            break;
                        }
                    }
                    if (destynacja) {
                        pkt = miasta[m].polozenie;
                        break;
                    }
                }
                miasta[j].dodajLot(ladowanie, odleglosc, pkt, miasta[j].indeks);
                break;
            }
        }
        delete[] start;
        delete[] ladowanie;
    }
    int trasy;
    scanf("%d", &trasy);
    cin.ignore();
    for (int i = 0; i < trasy; i++) {
        char* line = new char[100];
        fgets(line, 100, stdin);
        int len1 = 0;
        int len2 = 0;
        bool space = false;
        for (int j = 0; j < 50; j++) {
            if (space && line[j] == ' ')
                break;
            else if (line[j] == ' ')
                space = true;
            else if (!space)
                len1++;
            else if (space)
                len2++;
        }
        char* start = new char[len1];
        char* ladowanie = new char[len2];
        int typ;
        bool first_found = false;
        bool second_found = false;
        punkt pkt1;
        punkt pkt2;
        sscanf(line, "%s %s %d", start, ladowanie, &typ);
        for (int j = 0; j < punkty.size; j++) {
            bool found1 = true;
            bool found2 = true;
            if (!first_found) {
                for (int m = 0; m < len1; m++) {
                    if (start[m] != miasta[j].nazwa[m] || len1 != miasta[j].nameLen) {
                        found1 = false;
                        break;
                    }
                }
            }
            if (found1 && !first_found) {
                pkt1 = miasta[j].polozenie;
                first_found = true;
            }
            if (!second_found) {
                for (int m = 0; m < len2; m++) {
                    if (ladowanie[m] != miasta[j].nazwa[m] || len2 != miasta[j].nameLen) {
                        found2 = false;
                        break;
                    }
                }
            }
            if (found2 && !second_found) {
                pkt2 = miasta[j].polozenie;
                second_found = true;
            }
            if (found1 && found2)
                break;
        }
        dijkstra(pkt1, pkt2, typ, miasta, punkty.size, rows, cols);
    }
    delete[] miasta;
}