
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
    //pasarlo los datos por el constructor
    srand(time(NULL)); //reseteamos la semilla

    LindaDriver pizarra(ipServidorDespliegue, puerto);
    //colocamos la tupla para contear las tuplas
    Tupla numeroTuplas("TotalTuplas", "0");
    pizarra.PN(numeroTuplas);

    //colocamos tuplas de numero de publicadores para controlar los publicadores
    Tupla numeroPublicadores("Publicadores", "0");
    pizarra.PN(numeroPublicadores);

    //colocamos tuplas de numero de buscadores para controlar buscadores
    Tupla numeroBuscadores("Buscadores", "0");
    pizarra.PN(numeroBuscadores);

    //colocamos tuplas de numero de buscadoresCombinados para controlar buscadores combinados
    Tupla numeroBuscadoresCombinados("BuscadoresCombinados", "0");
    pizarra.PN(numeroBuscadoresCombinados);

    //Escribo las tuplas de monitorizacion de lectura y escritura
    Tupla numeroEscrituras("PeticionesEscritura", "0");
    pizarra.PN(numeroEscrituras);
    Tupla numeroLecturas("PeticionesLectura", "0");
    pizarra.PN(numeroLecturas);

    //colocamos los 8 testigo de los buscadores
    Tupla tuplaControl("BUSCADOR");
    for (int i = 0; i < 8; ++i) {
        pizarra.PN(tuplaControl);
    }
    return 0;
}
