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


}



