
#include <iostream>
#include "LindaDriver.hpp"
#include "Tupla/Tupla.hpp"

using namespace std;


int main(int argc, char* argv[]) {

    //comprobacion de los datos

    if(argc != 3){
        cerr << "Numero de parametros introducios incorrecto:" + string(strerror(errno)) + "\n";
        exit(1);
    }
    string ipServidorDespliegue = argv[1];
    int puerto = stoi(argv[2]);

    //pasar los datos por el constructor
    LindaDriver pizarra(ipServidorDespliegue, puerto);
    //genero la tupla de testigo
    Tupla tuplaControl("BUSCADOR");
    //cojo la tupla de testigo
    pizarra.RN(tuplaControl,tuplaControl);
    //creo las 2 tuplas para la busqueda doble
    Tupla primeraOferta("?A","?B","?C","?D");
   Tupla segundaOferta("?E","?C","?F","?G");

    for (int i = 0;i < 5; i++){
        Tupla primeraOfertaLocal(4);
        Tupla segundaOfertaLocal(4);
        //---------------------------------------------------------
        //aumento numero de lecturas                                ---------------------------
        //declaro la tupla de control lecturas
        Tupla peticionesLectura("PeticionesLectura","?X");
        //cojo la tupla de control del total de tuplas
        pizarra.RN(peticionesLectura,peticionesLectura);
        //extraigo el numero de tuplas actuales
        int numeroTuplas = stoi(peticionesLectura.get(1));
        //aumento en 1 con la publicada
        numeroTuplas = numeroTuplas + 1;
        peticionesLectura.set(1,to_string(numeroTuplas));
        pizarra.PN(peticionesLectura);

        //_-------------------------------------------------------
        //realizamos la busqueda doble
        pizarra.RDN_2(primeraOferta,segundaOferta,primeraOfertaLocal,segundaOfertaLocal);
        cout<<"Oferta combinada seleccioanda: "<< primeraOfertaLocal.to_string() <<" ---- "<<segundaOfertaLocal.to_string()<<"\n";
    }
    //escribimos en la pizarra la tupla de control
    pizarra.PN(tuplaControl);
    return 0;
}
