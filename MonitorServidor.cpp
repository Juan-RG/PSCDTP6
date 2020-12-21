//--------------------------------------------------------------------------------------------------
// File:    MonitorServidor.cpp
// Author:  Daniel Ariza Antón y Miguel Beltrán Pardos.
// Date:    diciembre 2020
// Coms:	Este fichero contiene la especificación de las funciones del MonitorServidor.
//--------------------------------------------------------------------------------------------------

#include "MonitorServidor.hpp"


//------------------------- constructor
MonitorServidor::MonitorServidor(multiset<Tupla> *almacen){
	this->almacen = *almacen;
}

//------------------------- destructor
MonitorServidor::~MonitorServidor(){}

void MonitorServidor::RdN(Tupla tupla) {
	unique_lock<mutex> lck(mtx);
    while (almacen.find(tupla) == almacen.end()){
        enEspera.wait(lck);
    }

}
void MonitorServidor::RN(Tupla tupla) {
    unique_lock<mutex> lck(mtx);
    const bool is_in = almacen.find(tupla) != almacen.end();
    cout << "que me dices "<< is_in<<"\n";
    while (almacen.find(tupla) == almacen.end()){
        cout<< "esperando\n";
        enEspera.wait(lck);
    }
    cout<< "paso eliminando\n";
    almacen.erase(tupla);
}
void MonitorServidor::PN(Tupla tupla) {
    unique_lock<mutex> lck(mtx);
    almacen.insert(tupla);
    enEspera.notify_all();
    for (set<Tupla>::iterator i = almacen.begin(); i != almacen.end(); i++) {
        Tupla t(*i);
        cout << t.to_string()<<"\n";

    }
    /*
    const bool is_in = almacen.find(tupla) != almacen.end();
    set<Tupla>::iterator y = almacen.find(tupla);
    if(is_in){   //control si ya existe el que vamos a guardar
        cout << "entro repe!!!!!!!!!!!!!!!!!!!!!!!\n";
        Tupla t(*y);
        cout<< " t "<< t.to_string() + "\n";
        string value = t.get(0);
        cout<< "asdasdasda\n";
        set<Tupla>::iterator final = almacen.end();
        cout<< "asdasdasda\n";
        final--;
        Tupla tfinal(*final);
        cout << "paso\n";
        cout << tupla.to_string()<<" t final\n";
        cout << "paso\n";
        cout << value << "  aa" << tfinal.get(0)<< "\n";

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
    */
}

