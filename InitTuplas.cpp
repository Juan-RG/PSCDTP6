
#include <iostream>
#include "LindaDriver.hpp"
#include "Tupla.hpp"

using namespace std;



int main(int argc, char* argv[]) {
    //comprobacion de los datos
    /*
    if(argc != 3){
        cerr << "Numero de parametros introducios incorrecto:" + string(strerror(errno)) + "\n";
        exit(1);
    }*/
    // string ipServidorDespliegue = argv[1];
    //int puerto = stoi(argv[2]);
    //pasarlo los datos por el constructor
    srand(time(NULL)); //reseteamos la semilla
    LindaDriver pizarra("localhost",2022);
    //colocamos la tupla para contear las tuplas
    Tupla numeroTuplas("TotalTuplas","0");
    pizarra.PN(numeroTuplas);

    //Escribo las tuplas de monitorizacion de lectura y escritura
    Tupla numeroEscrituras("PeticionesEscritura","0");
    pizarra.PN(numeroEscrituras);
    Tupla numeroLecturas("PeticionesLectura","0");
    pizarra.PN(numeroLecturas);

    //colocamos los 8 testigo de los buscadores
    Tupla tuplaControl("BUSCADOR");
    for (int i = 0; i < 8 ; ++i) {
        pizarra.PN(tuplaControl);
    }
    return 0;
}
