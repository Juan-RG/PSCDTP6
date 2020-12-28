
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
    LindaDriver pizarra("localhost",2022);
    //Genero la tupla de testigo
    Tupla tuplaControl("BUSCADOR");
    //cojo el testigo de la pizarra
    pizarra.RN(tuplaControl,tuplaControl);
    srand(time(NULL)); //reseteamos la semilla

    for (int i = 0; i < 10; i++){
        int min = 1, max = 3;
        //Calculamos un valor aleatorio entre el min y el max
        int numeroAleatorio = rand () % (max - min + 1) + min;
        int idTupla;
        //Genero la tupla a buscar
        Tupla oferta("?X","?Y","?Z","?R");
        for(int j = 0; j < numeroAleatorio; j++){
            //Si es la primera busqueda obtengo la tupla si no cojo el siguiente ID
            if(j == 0){
                //cojo la tupla y extraigo el ID
               pizarra.RDN(oferta,oferta);
               idTupla = stoi(oferta.get(0));
            }else{
                //incremento el id
                idTupla = idTupla + 1;
                //reseteo parametros de la tupla
                oferta.set(0,to_string(idTupla));
                oferta.set(1,"?X");
                oferta.set(2,"?Y");
                oferta.set(3,"?Z");
                pizarra.RDN(oferta,oferta);
            }
          cout<< "Oferta leida: "+ oferta.to_string()<< "\n";
        }
    }
    //escribo el testigo para el siguiente buscador
    pizarra.PN(tuplaControl);


    return 0;
}
