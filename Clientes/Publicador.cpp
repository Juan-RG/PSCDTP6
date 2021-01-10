
#include <iostream>
#include <chrono>
#include "../LindaDriver/LindaDriver.hpp"

using namespace std;

static const int NUMERO_TOTAL_TUPLAS = 1000;
/**
 * Metodo que asigna una ciudad aleatoria de las 10 colocadas en el switch
 * @param ciudad
 */
void asignarCiudad(string *ciudad) {
    int valorMin = 1, valorMax = 10;
    //Calculamos un valor aleatorio entre el min y el max
    int nAleatorio = rand() % (valorMax - valorMin + 1) + valorMin;
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

int main(int argc, char *argv[]) {

    if (argc != 3) {
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

    //aumento numero de publicadores
    Tupla publicadores("Publicadores", "?X");
    //cojo la tupla de control del total de tuplas
    pizarra.RN(publicadores, publicadores);
    //extraigo el numero de tuplas actuales
    numeroTuplas = stoi(publicadores.get(1));
    //aumento en 1 con la publicada
    numeroTuplas = numeroTuplas + 1;
    publicadores.set(1, to_string(numeroTuplas));
    pizarra.PN(publicadores);


    while (numeroTuplas < NUMERO_TOTAL_TUPLAS) { //eliminar
        //Asigno la ciudad de origen
        string origen;
        asignarCiudad(&origen);

        //Asigno la ciudad de destino
        string destino;
        asignarCiudad(&destino);

        while (origen == destino) {
            asignarCiudad(&destino);
        }
        //precio minimo y maximo de las ofertas
        int precioMin = 10, precioMax = 100;
        //Calculamos un valor aleatorio entre el min y el max
        int precio = rand() % (precioMax - precioMin + 1) + precioMin;
        //aumento numero de lecturas                                ---------------------------
        //declaro la tupla de control lecturas
        Tupla peticionesEscritura("PeticionesEscritura", "?X");
        //cojo la tupla de control del total de tuplas
        pizarra.RN(peticionesEscritura, peticionesEscritura);
        //extraigo el numero de tuplas actuales
        numeroTuplas = stoi(peticionesEscritura.get(1));
        //aumento en 1 con la publicada
        numeroTuplas = numeroTuplas + 1;
        peticionesEscritura.set(1, to_string(numeroTuplas));
        pizarra.PN(peticionesEscritura);
        //                                                          ---------------------------
        //declaro la tupla de control
        Tupla totalTuplas("TotalTuplas", "?X");
        //cojo la tupla de control del total de tuplas
        Tupla totalTuplasB(2);
        pizarra.RN(totalTuplas, totalTuplasB);
        //extraigo el numero de tuplas actuales
        numeroTuplas = stoi(totalTuplasB.get(1));
        if (numeroTuplas != NUMERO_TOTAL_TUPLAS){
            //aumento en 1 con la publicada
            numeroTuplas = numeroTuplas + 1;
            //Genero la nueva tupla con los datos
            Tupla nuevaTupla(to_string(numeroTuplas), origen, destino, to_string(precio));
            //escribo la Tupla
            cout << "Oferta escrita: " << nuevaTupla.to_string() << "\n";
            pizarra.PN(nuevaTupla);
            //actualizo el valor y lo escribo en la pizarra
            totalTuplasB.set(1, to_string(numeroTuplas));

            pizarra.PN(totalTuplasB);
        }else{
            pizarra.PN(totalTuplasB);
        }

        sleep(1);
    }
    //diminuyo nº de publicadores
    //cojo la tupla de control del total de publicadores
    publicadores.set(1, "?X");
    pizarra.RN(publicadores, publicadores);
    //extraigo el numero de publicadores
    numeroTuplas = stoi(publicadores.get(1));
    //disminuyo en 1 con la publicada
    numeroTuplas = numeroTuplas - 1;
    publicadores.set(1, to_string(numeroTuplas));
    pizarra.PN(publicadores);

    return 0;

}
