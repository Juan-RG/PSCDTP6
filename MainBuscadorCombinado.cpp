
#include <iostream>

#include "LindaDriver.hpp"
#include "Tupla.hpp"

using namespace std;


int main(int argc, char* argv[]) {
    const string PUBLICADOR = "PUBLICADOR";
    const string BUSCADOR = "BUSCADOR";

    //cortamos el string recibido con los datos ip1,ip2,ip3,puerto
    //hilo para utilizar el linda driver con la conexion o bucle
    //join del hilo para cuando finalice
    //LindaDriver pizarra(argv[1],argv[2]);
    LindaDriver pizarra("localhost",2021,BUSCADOR);
    Tupla tuplaControl("BUSCADOR");
    pizarra.RN(tuplaControl,tuplaControl);
    Tupla primeraOferta("?A","?B","?C","?D");
    Tupla segundaOferta("?E","?C","?F","?G");
    for (int i = 0;i < 5; i++){
        Tupla primeraOfertaLocal(4);
        Tupla segundaOfertaLocal(4);
        pizarra.RDN_2(primeraOferta,segundaOferta,primeraOfertaLocal,segundaOfertaLocal);
        cout<<"Oferta combinada seleccioanda: "<< primeraOfertaLocal.to_string() <<" ---- "<<segundaOfertaLocal.to_string()<<"\n";
    }
    pizarra.PN(tuplaControl);
    return 0;
}
