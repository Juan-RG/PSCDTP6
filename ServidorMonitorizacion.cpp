
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

    //pasarlo los datos por el constructor
    LindaDriver pizarra(ipServidorDespliegue, puerto);
    //tuplas de valores
    Tupla peticionesLectura("PeticionesLectura","?X");
    Tupla peticionesEscritura("PeticionesEscritura","?X");
    Tupla totalTuplas("TotalTuplas","?X");
    while (true){
        Tupla peticionesLecturaTmp("","");
        Tupla peticionesEscrituraTmp("","");
        Tupla totalTuplasTmp("","");

        pizarra.RDN(peticionesLectura,peticionesLecturaTmp);
        pizarra.RDN(peticionesEscritura,peticionesEscrituraTmp);
        pizarra.RDN(totalTuplas,totalTuplasTmp);

        cout<< "Peticiones de lectura: " + peticionesLecturaTmp.get(1)<<"\n";
        cout<< "Peticiones de escritura: " + peticionesEscrituraTmp.get(1)<<"\n";
        cout<< "Total de tuplas: " + totalTuplasTmp.get(1)<<"\n";

        sleep(60);
    }
    return 0;
}
