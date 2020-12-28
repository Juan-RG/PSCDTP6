
#include <iostream>
#include "LindaDriver.hpp"
#include "Tupla.hpp"

using namespace std;



int main(int argc, char* argv[]) {
    srand(time(NULL)); //reseteamos la semilla
    const string PUBLICADOR = "PUBLICADOR";
    const string BUSCADOR = "BUSCADOR";
    //LindaDriver pizarra(argv[1],argv[2]);
    LindaDriver pizarra("localhost",2022);
    //si controlamos con tuplas
    Tupla tuplaControl("BUSCADOR");
    pizarra.RN(tuplaControl,tuplaControl);
    for (int i = 0;i < 10; i++){
        cout<<"for 1 \n";
        int precioMin = 1, precioMax = 3;
        //Calculamos un valor aleatorio entre el min y el max
        int numeroAleatorio = rand () % (precioMax - precioMin + 1) + precioMin;
        int idTupla;
        Tupla oferta("?X","?Y","?Z","?R");
        for(int j = 0; j < numeroAleatorio; j++){
            cout<<"for 2 \n";
            if(j == 0){
                cout<<"if 1 \n";
               // string oferta = pirzarra.RdN([?p,?CiudadOrigen,?CiudadDestino,?Precio]); //poner mas datos en rd de tupla --- cambiar string por tupla
               pizarra.RDN(oferta,oferta);
               idTupla = stoi(oferta.get(0));
            }else{
                idTupla=idTupla+1;
                cout<<"idTupla "<<idTupla<<"\n";
                oferta.set(0,to_string(idTupla));
                oferta.set(1,"?X");
                oferta.set(2,"?Y");
                oferta.set(3,"?Z");
                cout<<"paso el set\n";
                cout<<oferta.to_string()<<"\n";
                pizarra.RDN(oferta,oferta);
                cout<<"oferta devuelta del server\n";
                cout<<oferta.to_string()<<"\n";
            }
          cout<< "Oferta leida: "+ oferta.to_string()<< "\n";
        }
    }
    pizarra.PN(tuplaControl);


    return 0;
}
