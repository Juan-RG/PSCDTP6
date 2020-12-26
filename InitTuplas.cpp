
#include <iostream>
#include "LindaDriver.hpp"
#include "Tupla.hpp"

using namespace std;



int main(int argc, char* argv[]) {
    srand(time(NULL)); //reseteamos la semilla
    const string PUBLICADOR = "PUBLICADOR";
    const string BUSCADOR = "BUSCADOR";
    //LindaDriver pizarra(argv[1],argv[2]);
    LindaDriver pizarra("localhost",2021,BUSCADOR);
    //si controlamos con tuplas
    Tupla numeroTuplas("TotalTuplas","0");
    pizarra.PN(numeroTuplas);
    Tupla tuplaControl("BUSCADOR");
    for (int i = 0; i <8 ; ++i) {
        pizarra.PN(tuplaControl);
    }
    return 0;
}
