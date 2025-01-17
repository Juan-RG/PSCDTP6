//--------------------------------------------------------------------------------------------------
// File:    BuscadorCombinado.cpp
// Date:    Enero 2021
// Comms:   BuscadorCombinado busca 2 ofertas en las cuales se pasa por una ciudad intermedia
//--------------------------------------------------------------------------------------------------
#include <iostream>
#include "../LindaDriver/LindaDriver.hpp"
#include "../Tupla/Tupla.hpp"

using namespace std;


int main(int argc, char *argv[]) {

    //comprobacion de los datos

    if (argc != 3) {
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
    pizarra.RN(tuplaControl, tuplaControl);
    //aumento numero de Buscadores combinados
    Tupla buscadoresCombinados("BuscadoresCombinados", "?X");
    //cojo la tupla de control de total buscadores combinados
    pizarra.RN(buscadoresCombinados, buscadoresCombinados);
    //extraigo el numero de buscadores combinados actuales
    int numeroTuplas = stoi(buscadoresCombinados.get(1));
    //aumento en 1 los buscadores
    numeroTuplas = numeroTuplas + 1;
    buscadoresCombinados.set(1, to_string(numeroTuplas));
    pizarra.PN(buscadoresCombinados);

    //aumento numero de Buscadores
    Tupla publicadores("Publicadores", "?X");
    //cojo la tupla de control de total buscadores
    pizarra.RDN(publicadores, publicadores);
    //aumento numero de Buscadores
    Tupla totalTuplas("TotalTuplas", "?X");
    //cojo la tupla de control de total buscadores
    pizarra.RDN(totalTuplas, totalTuplas);

    if (stoi(totalTuplas.get(1)) != 0 || stoi(totalTuplas.get(1)) != 0) {

        //creo las 2 tuplas para la busqueda doble
        Tupla primeraOferta("?A", "?B", "?C", "?D");
        Tupla segundaOferta("?E", "?C", "?F", "?G");

        for (int i = 0; i < 5; i++) {
            Tupla primeraOfertaLocal(4);
            Tupla segundaOfertaLocal(4);
            //---------------------------------------------------------
            //aumento numero de lecturas                                ---------------------------
            //declaro la tupla de control lecturas
            Tupla peticionesLectura("PeticionesLectura", "?X");
            //cojo la tupla de control del total de tuplas
            pizarra.RN(peticionesLectura, peticionesLectura);
            //extraigo el numero de tuplas actuales
            int numeroTuplas = stoi(peticionesLectura.get(1));
            //aumento en 1 con la publicada
            numeroTuplas = numeroTuplas + 1;
            peticionesLectura.set(1, to_string(numeroTuplas));
            pizarra.PN(peticionesLectura);

            //_-------------------------------------------------------
            //realizamos la busqueda doble
            pizarra.RDN_2(primeraOferta, segundaOferta, primeraOfertaLocal, segundaOfertaLocal);
            //cout << "Oferta combinada seleccionada: " + primeraOfertaLocal.to_string() + " ---- "
            //     + segundaOfertaLocal.to_string() + "\n";
            sleep(1);
        }
        //escribimos en la pizarra la tupla de control
        pizarra.PN(tuplaControl);

        //diminuyo el numero de buscadores combinados
        buscadoresCombinados.set(1, "?X");
        //cojo la tupla de control de buscadores combinados
        pizarra.RN(buscadoresCombinados, buscadoresCombinados);
        //extraigo el numero el numero actual
        numeroTuplas = stoi(buscadoresCombinados.get(1));
        //disminuyo en 1 con la publicada
        numeroTuplas = numeroTuplas - 1;
        buscadoresCombinados.set(1, to_string(numeroTuplas));
        pizarra.PN(buscadoresCombinados);
    }else{
        buscadoresCombinados.set(1, "?X");
        //cojo la tupla de control de buscadores combinados
        pizarra.RN(buscadoresCombinados, buscadoresCombinados);
        //extraigo el numero el numero actual
        numeroTuplas = stoi(buscadoresCombinados.get(1));
        //disminuyo en 1 con la publicada
        numeroTuplas = numeroTuplas - 1;
        buscadoresCombinados.set(1, to_string(numeroTuplas));
        pizarra.PN(buscadoresCombinados);
        //cout<<"No hay tuplas ni publicadores acabo\n";
        pizarra.PN(tuplaControl);
    }

    return 0;
}
