//--------------------------------------------------------------------------------------------------
// File:    MonitorServidor.cpp
// Author:  Daniel Ariza Antón y Miguel Beltrán Pardos.
// Date:    diciembre 2020
// Coms:	Este fichero contiene la especificación de las funciones del MonitorServidor.
//--------------------------------------------------------------------------------------------------

#include "Downloads/MonitorServidor.hpp"

//------------------------- constructor
MonitorServidor::MonitorServidor(const multiset<Tupla> almacen){
	this->almacen = almacen;
}

//------------------------- destructor
MonitorServidor::~MonitorServidor(){}

void MonitorServidor::disponible(multiset<Tupla> almacen){
	unique_lock<mutex> lck(mtx);

}
