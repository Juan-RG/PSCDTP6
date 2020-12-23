//--------------------------------------------------------------------------------------------------
// File:    MonitorServidor.cpp
// Author:
// Date:    diciembre 2020
// Coms:	Este fichero contiene la especificación de las funciones del MonitorServidor.
//--------------------------------------------------------------------------------------------------

#include "MonitorServidor.hpp"
#include <iostream>

#include "Tupla.hpp"

//------------------------- constructor
MonitorServidor::MonitorServidor(multiset<Tupla> *almacen){
	this->almacen = *almacen;
}

//------------------------- destructor
MonitorServidor::~MonitorServidor(){}

void MonitorServidor::PN(Tupla tupla) {
    unique_lock<mutex> lck(mtx);
    almacen.insert(tupla);          // Guardamos la tupla que pasamos a la operacion del monitor
    enEspera.notify_all();          //Avisamos a todos que estan en espera de que se ha anyadido una nueva tupla

    multiset <Tupla> :: iterator itr;
    for (itr = almacen.begin(); itr != almacen.end(); ++itr) {
        Tupla tmp(*itr);
        cout << tmp.to_string() <<"\n";

    }
    cout<<"asda\n";
}

void MonitorServidor::RdN(Tupla &tupla) {    //TODO: Tenemos que controlar el caso de que llegue un comodin ?A-Z
	unique_lock<mutex> lck(mtx);
    /*while (almacen.find(tupla) == almacen.end()){
        enEspera.wait(lck);
    }*/
    cout << "hola" << endl;
    multiset <Tupla> :: iterator itr;
    //Tupla(tuplaDelIterador);
    Tupla tuplaDelIterador("");
    cout << "hola 1" << endl;
    //itr = almacen.begin();  //Inicializamos el iterador a la primera posicion del almacen (multiset)
    /*if (tupla.match(*itr)) {
        cout << "tupla encontrada";
    }*/
    /*while(!tupla.match(*itr)){
        cout << "comienza el while" << endl;
        for (itr = almacen.begin(); itr != almacen.end(); ++itr){
            //tuplaDelIterador = *itr;
            if(tupla.match(*itr)){
                cout << "tupla encontrada";
            }*/
    //tuplaDelIterador = *itr;
    cout << "funciona tuplaDelIterador = *itr " << endl;
    while(!tupla.match(tuplaDelIterador)){
        //Tupla temp(*itr);
        //tuplaDelIterador.from_string(temp.to_string());
        //tuplaDelIterador = *itr;
        cout<< "esto que da como resultado " <<tuplaDelIterador.to_string()<<"\n";
        cout << "funciona tuplaDelIterador = *itr  2 " << endl;
        //Buscamos en el almacen la tupla que queremos, hasta encontrarla o terminar la iteracion
        for (itr = almacen.begin(); itr!= almacen.end() || tupla.match(tuplaDelIterador); ++itr) {
            Tupla temp(*itr);
            tuplaDelIterador.from_string(temp.to_string());                                                               //FAIL: *ITR HAY QUE INICIALIZARLO EN TUPLA(T)
            cout <<"paso iteracion \n";
        }
        //Si no la encuentra se pondra en espera hasta nuevo aviso
        if(!tupla.match(tuplaDelIterador)){
            cout<<"me bloqueo\n";
            enEspera.wait(lck);
        }
    }

    //Si llega hasta aqui es que la ha encontrado
    //tupla = tuplaDelIterador;       //Pasamos por referencia la tupla para reenviarsela al lindaDriver
    tupla.from_string(tuplaDelIterador.to_string());
}

void MonitorServidor::RN(Tupla &tupla) {                         //TODO: Tenemos que controlar el caso de que llegue un comodin ?A-Z
    unique_lock<mutex> lck(mtx);
    /*const bool is_in = almacen.find(tupla) != almacen.end();            // Todo: if con find para detectar si la tupla esta si no comprobacion recorriendo el multiset con match
    cout << "que me dices "<< is_in<<"\n";
    while (almacen.find(tupla) == almacen.end()){
        cout<< "esperando\n";
        enEspera.wait(lck);
    }
    cout<< "paso eliminando\n";
    almacen.erase(tupla);*/
    Tupla temporal("");
    multiset <Tupla> :: iterator itr;
    bool bandera = false;
    while (!bandera){
        for (itr = almacen.begin(); itr != almacen.end(); ++itr) {
            Tupla tmp(*itr);
            temporal.from_string(tmp.to_string());
            if (tupla.match(temporal)) {
               bandera = true;
            }
        }
        if (bandera == false){
            cout << "bloqueado\n";
            enEspera.wait(lck);
        }
    }
    cout << "entrada "<<tupla.to_string()<<"\n";
    tupla.from_string(temporal.to_string());
    cout << "salida "<<tupla.to_string()<<"\n";
    almacen.erase(temporal);
    cout<<"prueba\n";
/*
    multiset <Tupla> :: iterator itr;
    Tupla tuplaDelIterador(*itr);

    while(!tupla.match(tuplaDelIterador)){
        //tuplaDelIterador = *itr;
        //Buscamos en el almacen la tupla que queremos, hasta encontrarla o terminar la iteracion
        for (itr = almacen.begin(); itr!= almacen.end() || tupla.match(tuplaDelIterador); ++itr) {
            tuplaDelIterador = *itr;
        }
        //Si no la encuentra se pondra en espera hasta nuevo aviso
        if(!tupla.match(tuplaDelIterador)){
            enEspera.wait(lck);
        }
    }

    //Si llega hasta aqui es que la ha encontrado
    tupla = tuplaDelIterador;       //Pasamos por referencia la tupla para reenviarsela al lindaDriver

    almacen.erase(tuplaDelIterador);    //Una vez encontrada la borramos
**/
}

void MonitorServidor::RdN_2(Tupla &t1, Tupla &t2) {  //TODO: Desarrollar
    Tupla temporal("");
    Tupla temporal2("");
    multiset <Tupla> :: iterator itr;

}
void MonitorServidor::RN_2(Tupla &p1, Tupla &p2) {  //TODO: Desarrollar
    //Tupla temporal("");
    //Tupla temporal2("");
    multiset <Tupla> :: iterator itr;
    //-----------------------------------------------------------------------------------------------------
    // Tuplas a sobreescribir con el resultado, de momento son copias de p1 y p2
    Tupla tuplaTemp1(p1);
    Tupla tuplaTemp2(p2);
    bool parar = false;
    int lengthArrayComodines;

    struct comodines {
        string valor;
        int numIndices;
        int indices[6]; // tamaño máximo de una tupla
    };

    struct comodinesComunes {
        string valor;
        int numIndicesp1, numIndicesp2;
        // acotados a los tamaños máximos de una tupla
        int indicesp1[6];
        int indicesp2[6];
    };

    // acotados a los tamaños de cada una de las tuplas
    cout<< "aa "<< p1.size()<<" asd "<< p2.size()<< "\n";
    comodines arrayComodinesp1[p1.size()];
    comodines arrayComodinesp2[p2.size()];
    comodinesComunes arrayComodinesComunes[p1.size()]; // max: el tamaño
    // de la menor
    // o de cualquiera
    // de las dos

    cout<< "ppppp" <<"\n";
    // Se guardan los comodines de la tupla p1 junto a los índices donde aparecen
    int numComodinesp1 = 0;
    bool estaba = false;
    for(int i = 0; i < p1.size(); i++) {
        if( (p1.get(i))[0] == '?') { // TODO: Sustituir por una regex
            cout<< "ppppp" <<"\n";
            // comprueba si ya estaba el comodín en la lista
            for (int j = 0; j < p1.size(); j++) {
                // si está, se añade otro índice
                if (arrayComodinesp1[j].valor == p1.get(i)) {
                    arrayComodinesp1[j].indices[arrayComodinesp1[j].numIndices] = i;
                    arrayComodinesp1[j].numIndices++;
                    estaba = true;
                }
            }
            // si no estaba, se añade otra entrada en la lista
            if (!estaba) {
                arrayComodinesp1[numComodinesp1].valor == p1.get(i);
                arrayComodinesp1[numComodinesp1].indices[arrayComodinesp1[numComodinesp1].numIndices] == i;
                arrayComodinesp1[numComodinesp1].numIndices++;
                numComodinesp1++;
            } else {
                // se resetea estaba para la siguiente iteración
                estaba = false;
            }
        }
    }


    // Se guardan los comodines de la tupla p2 junto a los índices donde aparecen
    int numComodinesp2 = 0;
    estaba = false;
    for(int i = 0; i < p2.size(); i++) {
        if( (p2.get(i))[0] == '?') { // TODO: Sustituir por una regex
            // comprueba si ya estaba el comodín en la lista
            for (int j = 0; j < p2.size(); j++) {
                // si está, se añade otro índice
                if (arrayComodinesp2[j].valor == p2.get(i)) {
                    arrayComodinesp2[j].indices[arrayComodinesp2[j].numIndices] = i;
                    arrayComodinesp2[j].numIndices++;
                    estaba = true;
                }
            }
            // si no estaba, se añade otra entrada en la lista
            if (!estaba) {
                arrayComodinesp2[numComodinesp2].valor == p2.get(i);
                arrayComodinesp2[numComodinesp2].indices[arrayComodinesp2[numComodinesp2].numIndices] == i;
                arrayComodinesp2[numComodinesp2].numIndices++;
                numComodinesp2++;
            } else {
                // se resetea estaba para la siguiente iteración
                estaba = false;
            }
        }
    }

    // Se juntan los comodines de ambas en el vector de estructuras de
    // comodines comunes
    int numComodinesComunes;
    for (int i = 0; i < p1.size(); i++) {
        for (int j = 0; j < p2.size(); j++) {
            // si dos coinciden , se añade un nuevo comodín común
            // se sabe que en ninguno de los dos hay comodines repetidos
            if (arrayComodinesp1[i].valor == arrayComodinesp2[j].valor) {
                arrayComodinesComunes[numComodinesComunes].valor = arrayComodinesp1[i].valor;
                arrayComodinesComunes[numComodinesComunes].numIndicesp1 = arrayComodinesp1[i].numIndices;
                // copia el vector de indices de comodinesp1 en el de comunes para p1
                std::copy(arrayComodinesp1[i].indices, arrayComodinesp1[i].indices + 6, arrayComodinesComunes[numComodinesComunes].indicesp1);

                // hace lo mismo para p2
                arrayComodinesComunes[numComodinesComunes].numIndicesp2 = arrayComodinesp2[i].numIndices;
                // copia el vector de indices de comodinesp2 en el de comunes para p2
                std::copy(arrayComodinesp2[i].indices, arrayComodinesp2[i].indices + 6, arrayComodinesComunes[numComodinesComunes].indicesp2);

                numComodinesComunes++;
            }
        }
    }


}



