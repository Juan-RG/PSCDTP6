
#include <iostream>
#include "LindaDriver.hpp"
#include "Tupla.hpp"

using namespace std;



int main(int argc, char* argv[]) {
    srand(time(NULL)); //reseteamos la semilla
    const string PUBLICADOR = "PUBLICADOR";
    const string BUSCADOR = "BUSCADOR";
    //LindaDriver pizarra(argv[1],argv[2]);
    LindaDriver pizarra("localhost",2021,BUSCADOR);
    //si controlamos con tuplas
    Tupla tuplaControl("BUSCADOR");
    pizarra.RN(tuplaControl,tuplaControl);
    for (int i = 0;i < 10; i++){
        int precioMin = 1, precioMax = 3;
        //Calculamos un valor aleatorio entre el min y el max
        int numeroAleatorio = rand () % (precioMax - precioMin + 1) + precioMin;
        int idTupla;
        Tupla oferta("?X","?Y","?Z","?R");
        for(int j = 0; j < numeroAleatorio; j++){
            if(j == 0){

               // string oferta = pirzarra.RdN([?p,?CiudadOrigen,?CiudadDestino,?Precio]); //poner mas datos en rd de tupla --- cambiar string por tupla
               pizarra.RDN(oferta,oferta);
               idTupla = stoi(oferta.get(0));
            }else{
                idTupla=idTupla+1;
                oferta.set(0,to_string(idTupla));
                pizarra.RDN(oferta,oferta);
            }
          cout<< "Oferta leida: "+ oferta.to_string()<< "\n";
        }
    }
    pizarra.PN(tuplaControl);


    return 0;
}
