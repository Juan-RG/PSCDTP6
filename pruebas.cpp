//
// Pruebas de LindaDriver, ejecutar sin ningún otro cliente y servidores sin tuplas almacenadas.
//
#include <iostream>
#include "LindaDriver.hpp"
#include "Tupla.hpp"

using namespace std;

int main() {
    LindaDriver pizarra("localhost",2022);
    Tupla tuplaTam1("TuplaDeTamaño1");
    Tupla tuplaTam2("TuplaDeTamaño", "2");
    Tupla tuplaTam3("Tupla", "DeTamaño", "3");
    Tupla tuplaTam4("Tupla", "De", "Tamaño", "4");
    Tupla tuplaTam5("Tupla", "De", "Tamaño", "..", "5");
    Tupla tuplaTam6("Tupla", "De", "Tamaño", "..", "..", "6");

    Tupla tuplaTemp1(tuplaTam1);
    Tupla tuplaTemp2(tuplaTam2);
    Tupla tuplaTemp3(tuplaTam3);
    Tupla tuplaTemp4(tuplaTam4);
    Tupla tuplaTemp5(tuplaTam5);
    Tupla tuplaTemp6(tuplaTam6);

    pizarra.PN(tuplaTam1); // a server 1
    pizarra.PN(tuplaTam2); // a server 1
    pizarra.PN(tuplaTam3); // a server 1
    pizarra.PN(tuplaTam4); // a server 2
    pizarra.PN(tuplaTam5); // a server 2
    pizarra.PN(tuplaTam6); // a server 3

    pizarra.RDN(tuplaTam1,tuplaTemp1); // a server 1
    pizarra.RDN(tuplaTam2,tuplaTemp2); // a server 1
    pizarra.RDN(tuplaTam3,tuplaTemp3); // a server 1
    pizarra.RDN(tuplaTam4,tuplaTemp4); // a server 2
    pizarra.RDN(tuplaTam5,tuplaTemp5); // a server 2
    pizarra.RDN(tuplaTam6,tuplaTemp6); // a server 3

    pizarra.PN(tuplaTam1); // a server 1
    pizarra.PN(tuplaTam2); // a server 1
    pizarra.PN(tuplaTam3); // a server 1
    pizarra.PN(tuplaTam4); // a server 2
    pizarra.PN(tuplaTam5); // a server 2
    pizarra.PN(tuplaTam6); // a server 3

    pizarra.RDN_2(tuplaTam1, tuplaTam1, tuplaTemp1, tuplaTemp1); // a server 1
    pizarra.RDN_2(tuplaTam2, tuplaTam2, tuplaTemp2, tuplaTemp2); // a server 1
    pizarra.RDN_2(tuplaTam3, tuplaTam3, tuplaTemp3, tuplaTemp3); // a server 1
    pizarra.RDN_2(tuplaTam4, tuplaTam4, tuplaTemp4, tuplaTemp4); // a server 2
    pizarra.RDN_2(tuplaTam5, tuplaTam5, tuplaTemp5, tuplaTemp5); // a server 2
    pizarra.RDN_2(tuplaTam6, tuplaTam6, tuplaTemp6, tuplaTemp6); // a server 3

    pizarra.RDN_2(tuplaTam5, tuplaTam6, tuplaTemp2, tuplaTemp6); // tupla con tamaños diferentes, que se rechaza
    pizarra.RDN_2(tuplaTam6, tuplaTam6, tuplaTemp5, tuplaTemp6); // tupla con tamaños diferentes, que se rechaza

    pizarra.PN(tuplaTam1); // a server 1
    pizarra.PN(tuplaTam2); // a server 1
    pizarra.PN(tuplaTam3); // a server 1
    pizarra.PN(tuplaTam4); // a server 2
    pizarra.PN(tuplaTam5); // a server 2
    pizarra.PN(tuplaTam6); // a server 3

    pizarra.RN(tuplaTam1,tuplaTemp1); // a server 1
    pizarra.RN(tuplaTam2,tuplaTemp2); // a server 1
    pizarra.RN(tuplaTam3,tuplaTemp3); // a server 1
    pizarra.RN(tuplaTam4,tuplaTemp4); // a server 2
    pizarra.RN(tuplaTam5,tuplaTemp5); // a server 2
    pizarra.RN(tuplaTam6,tuplaTemp6); // a server 3

    pizarra.PN(tuplaTam1); // a server 1
    pizarra.PN(tuplaTam2); // a server 1
    pizarra.PN(tuplaTam3); // a server 1
    pizarra.PN(tuplaTam4); // a server 2
    pizarra.PN(tuplaTam5); // a server 2
    pizarra.PN(tuplaTam6); // a server 3

    pizarra.RN_2(tuplaTam1, tuplaTam1, tuplaTemp1, tuplaTemp1); // a server 1
    pizarra.RN_2(tuplaTam2, tuplaTam2, tuplaTemp2, tuplaTemp2); // a server 1
    pizarra.RN_2(tuplaTam3, tuplaTam3, tuplaTemp3, tuplaTemp3); // a server 1
    pizarra.RN_2(tuplaTam4, tuplaTam4, tuplaTemp4, tuplaTemp4); // a server 2
    pizarra.RN_2(tuplaTam5, tuplaTam5, tuplaTemp5, tuplaTemp5); // a server 2
    pizarra.RN_2(tuplaTam6, tuplaTam6, tuplaTemp6, tuplaTemp6); // a server 3

    cout << "------------------------------------------------------------------------" << endl;
    cout << "A partir de aquí me bloqueo, no hay más tuplas..." << endl;
    cout << "------------------------------------------------------------------------" << endl;

    pizarra.RN_2(tuplaTam1, tuplaTam1, tuplaTemp1, tuplaTemp1); // a server 1
    pizarra.RN_2(tuplaTam2, tuplaTam2, tuplaTemp2, tuplaTemp2); // a server 1
    pizarra.RN_2(tuplaTam3, tuplaTam3, tuplaTemp3, tuplaTemp3); // a server 1
    pizarra.RN_2(tuplaTam4, tuplaTam4, tuplaTemp4, tuplaTemp4); // a server 2
    pizarra.RN_2(tuplaTam5, tuplaTam5, tuplaTemp5, tuplaTemp5); // a server 2
    pizarra.RN_2(tuplaTam6, tuplaTam6, tuplaTemp6, tuplaTemp6); // a server 3
}