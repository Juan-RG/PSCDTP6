//--------------------------------------------------------------------------------------------------
// File:    MonitorServidor.cpp
// Author:  Daniel Ariza Antón y Miguel Beltrán Pardos.
// Date:    diciembre 2020
// Coms:	Este fichero contiene la especificación de las funciones del MonitorServidor.
//--------------------------------------------------------------------------------------------------

#include "MonitorServidor.hpp"


//------------------------- constructor
MonitorServidor::MonitorServidor(set<Tupla> *almacen){
	this->almacen = *almacen;
}

//------------------------- destructor
MonitorServidor::~MonitorServidor(){}

void MonitorServidor::disponible(Tupla tupla) {
	unique_lock<mutex> lck(mtx);
    almacen.find(tupla);
    enEspera.notify_one();
}
void MonitorServidor::borrar(Tupla tupla) {
    unique_lock<mutex> lck(mtx);
    //almacen.erase(almacen.find(tuplaTemp));
    enEspera.notify_one();
}
void MonitorServidor::guardar(Tupla tupla) {
    unique_lock<mutex> lck(mtx);
    const bool is_in = almacen.find(tupla) != almacen.end();
    cout << is_in <<" que me dices \n";
    set<Tupla>::iterator y = almacen.find(tupla);
    if(y == almacen.end()){
        cout<<"nO ENTIENDO NADA \n";
    }
    if(y != almacen.end()){   //control si ya existe el que vamos a guardar
        cout << "entro repe!!!!!!!!!!!!!!!!!!!!!!!\n";
        Tupla t(*y);
        string value = t.get(0);
        cout << value << "  aa" << tupla.get(0)<< "\n";

    }else{
        almacen.insert(tupla);
        enEspera.notify_all();
        cout<<"entro\n";
        cout<< almacen.size()<<"\n";
        for (set<Tupla>::iterator i = almacen.begin(); i != almacen.end(); i++) {
            Tupla t(*i);
            cout << t.to_string()<<"\n";

        }
        cout<<"fin entro\n";
    }

}

