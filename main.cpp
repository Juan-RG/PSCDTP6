#include <iostream>
#include <unordered_set>
#include "Tupla.hpp"
#include "TuplaHash.cpp"




using namespace std;
void patata(int i, string basicString);

void asignarCiudad(string *ciudad) {
    int valorMin = 1, valorMax = 10;
    //Calculamos un valor aleatorio entre el min y el max
    int nAleatorio = rand () % (valorMax - valorMin +1) + valorMin;
    switch (nAleatorio) {
        case 1:
            *ciudad = "Barcelona";
            break;
        case 2:
            *ciudad = "Madrid";
            break;
        case 3:
            *ciudad = "Valencia";
            break;
        case 4:
            *ciudad = "Sevilla";
            break;
        case 5:
            *ciudad = "La Coruña";
            break;
        case 6:
            *ciudad = "Tenerife";
            break;
        case 7:
            *ciudad = "Bilbao";
            break;
        case 8:
            *ciudad = "Badajoz";
            break;
        case 9:
            *ciudad = "Zaragoza";
            break;
        case 10:
            *ciudad = "Teruel";
            break;
    }
}
int main() {
    unordered_multiset<int>::iterator itr2;
    unordered_multiset<int> almacen;

    almacen.insert(1);
    almacen.insert(2);
    almacen.insert(3);
    almacen.insert(4);
    almacen.insert(4);
    almacen.insert(1);
    for (itr2 = almacen.begin(); itr2 != almacen.end(); ++itr2) {
        cout<<"paso\n";
    }
    cout<<"-----------\n";
    itr2 = almacen.begin();
    while (itr2 != almacen.end()){
        cout<<"paso\n";
        itr2++;
    }
}



