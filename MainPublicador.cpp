
#include <iostream>
#include <chrono>
#include "LindaDriver.hpp"

using namespace std;


/**
 * Metodo que asigna una ciudad aleatoria de las 10 colocadas en el switch
 * @param ciudad
 */
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

int main(int argc, char* argv[]) {
    //comprobacion de los datos
    unsigned t0, t1;
    //pasar los datos por el constructor
    LindaDriver pizarra("localhost", 2020);
    int numeroTuplas;
    //extraeremos la tupla de RN['totalTuplas',?totalTuplas]
    srand(time(NULL)); //reseteamos la semilla
    int n = 0;
    while (n < 1000){ //eliminar
        //precio minimo y maximo de las ofertas
        int precioMin = 1, precioMax = 2;

        if(n < 5){
            Tupla nuevaTupla("0","teruel","zaragoza","50");
            pizarra.PN(nuevaTupla);
        }else{
            Tupla nuevaTupla("0","zaragoza","ok","50");
            pizarra.PN(nuevaTupla);
        }
        n++;
    }
    n = 0;
    Tupla primeraOferta("?A","?B","?C","?D");
    Tupla segundaOferta("?E","?C","?F","?G");

    t0=clock();
    while (n < 1000){ //eliminar
        Tupla primeraOfertaLocal(4);
        Tupla segundaOfertaLocal(4);
        pizarra.RDN_2(primeraOferta,segundaOferta,primeraOfertaLocal,segundaOfertaLocal);

        n++;
    }
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
    /*
    if(argc != 3){
        cerr << "Numero de parametros introducios incorrecto:" + string(strerror(errno)) + "\n";
        exit(1);
    }
    string ipServidorDespliegue = argv[1];
    int puerto = stoi(argv[2]);
    //pasar los datos por el constructor
    LindaDriver pizarra(ipServidorDespliegue, puerto);
    int numeroTuplas;
    //extraeremos la tupla de RN['totalTuplas',?totalTuplas]
    srand(time(NULL)); //reseteamos la semilla
    int n = 0;

    while (n < 10){ //eliminar
        n++;//eliminar
    //Asigno la ciudad de origen
    string origen;
    asignarCiudad(&origen);

    //Asigno la ciudad de destino
    string destino;
    asignarCiudad(&destino);

    while(origen == destino){
        asignarCiudad(&destino);
    }
    //precio minimo y maximo de las ofertas
    int precioMin = 10, precioMax = 100;
    //Calculamos un valor aleatorio entre el min y el max
    int precio = rand () % (precioMax - precioMin + 1) + precioMin;
    //aumento numero de lecturas                                ---------------------------
        //declaro la tupla de control lecturas
        Tupla peticionesEscritura("PeticionesEscritura","?X");
        //cojo la tupla de control del total de tuplas
        pizarra.RN(peticionesEscritura,peticionesEscritura);
        //extraigo el numero de tuplas actuales
        numeroTuplas = stoi(peticionesEscritura.get(1));
        //aumento en 1 con la publicada
        numeroTuplas = numeroTuplas + 1;
        peticionesEscritura.set(1,to_string(numeroTuplas));
        pizarra.PN(peticionesEscritura);
    //                                                          ---------------------------
    //declaro la tupla de control
    Tupla totalTuplas("TotalTuplas","?X");
    //cojo la tupla de control del total de tuplas
    pizarra.RN(totalTuplas,totalTuplas);
    //extraigo el numero de tuplas actuales
    numeroTuplas = stoi(totalTuplas.get(1));
    cout<<numeroTuplas<<"\n";
    //aumento en 1 con la publicada
    numeroTuplas = numeroTuplas + 1;
    //Genero la nueva tupla con los datos
    Tupla nuevaTupla(to_string(numeroTuplas),origen,destino,to_string(precio));
    //escribo la Tupla
    pizarra.PN(nuevaTupla);
    //actualizo el valor y lo escribo en la pizarra
    totalTuplas.set(1,to_string(numeroTuplas));
    pizarra.PN(totalTuplas);
    }
*/
    return 0;

}
