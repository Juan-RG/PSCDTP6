
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

    //aumento numero de Buscadores
    Tupla buscadores("Buscadores", "?X");
    //cojo la tupla de control de total buscadores
    pizarra.RN(buscadores, buscadores);
    //extraigo el numero de buscadores actuales
    int numeroTuplas = stoi(buscadores.get(1));
    //aumento en 1 los buscadores
    numeroTuplas = numeroTuplas + 1;
    buscadores.set(1, to_string(numeroTuplas));
    pizarra.PN(buscadores);

    //aumento numero de Buscadores
    Tupla publicadores("Publicadores", "?X");
    //cojo la tupla de control de total buscadores
    pizarra.RDN(publicadores, publicadores);
    //aumento numero de Buscadores
    Tupla totalTuplas("TotalTuplas", "?X");
    //cojo la tupla de control de total buscadores
    pizarra.RDN(totalTuplas, totalTuplas);

    if (stoi(totalTuplas.get(1)) != 0 || stoi(totalTuplas.get(1)) != 0) {
        //Genero la tupla de testigo
        Tupla tuplaControl("BUSCADOR");
        //cojo el testigo de la pizarra
        pizarra.RN(tuplaControl, tuplaControl);
        srand(time(NULL)); //reseteamos la semilla

        for (int i = 0; i < 10; i++) {
            int min = 1, max = 3;
            //Calculamos un valor aleatorio entre el min y el max
            int numeroAleatorio = rand() % (max - min + 1) + min;
            int idTupla;
            //Genero la tupla a buscar
            Tupla oferta("?X", "?Y", "?Z", "?R");
            for (int j = 0; j < numeroAleatorio; j++) {
                //Si es la primera busqueda obtengo la tupla si no cojo el siguiente ID
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
                if (j == 0) {
                    //cojo la tupla y extraigo el ID
                    pizarra.RDN(oferta, oferta);
                    idTupla = stoi(oferta.get(0));
                } else {
                    //incremento el id
                    idTupla = idTupla + 1;
                    //reseteo parametros de la tupla
                    oferta.set(0, to_string(idTupla));
                    oferta.set(1, "?X");
                    oferta.set(2, "?Y");
                    oferta.set(3, "?Z");
                    pizarra.RDN(oferta, oferta);
                }
                cout << "Oferta leida: " + oferta.to_string() << "\n";
                sleep(1);
            }
        }
        //escribo el testigo para el siguiente buscador
        pizarra.PN(tuplaControl);

        //diminuyo el numero de buscadores combinados
        buscadores.set(1, "?X");
        //cojo la tupla de control de buscadores combinados
        pizarra.RN(buscadores, buscadores);
        //extraigo el numero el numero actual
        numeroTuplas = stoi(buscadores.get(1));
        //disminuyo en 1 con la publicada
        numeroTuplas = numeroTuplas - 1;
        buscadores.set(1, to_string(numeroTuplas));
        pizarra.PN(buscadores);
    }else{
        //diminuyo el numero de buscadores combinados
        buscadores.set(1, "?X");
        //cojo la tupla de control de buscadores combinados
        pizarra.RN(buscadores, buscadores);
        //extraigo el numero el numero actual
        numeroTuplas = stoi(buscadores.get(1));
        //disminuyo en 1 con la publicada
        numeroTuplas = numeroTuplas - 1;
        buscadores.set(1, to_string(numeroTuplas));
        pizarra.PN(buscadores);
        cout<<"No hay tuplas ni publicadores acabo\n";
    }

    return 0;
}
