#include <iostream>
#include <set>




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
    for (int i = 0; i <8 ; ++i) {
       cout<<"paso\n";
    }

}


