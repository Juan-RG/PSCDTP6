//--------------------------------------------------------------------------------------------------
// File:    MonitorServidor.hpp
// Author:  Daniel Ariza Antón y Miguel Beltrán Pardos.
// Date:    diciembre 2020
// Coms:	Este fichero contiene la declaración de las funciones y variables del MonitorServidor.
//--------------------------------------------------------------------------------------------------

//Fichero: MonitorServidor.hpp
#ifndef MONITOR_SERVIDOR_HPP
#define MONITOR_SERVIDOR_HPP

#include <mutex>
#include <condition_variable>
#include <set>
#include "Tupla.hpp"

//-----------------------------------------------------
using namespace std; //mutex, condition_variable, etc.

class MonitorServidor {
	private:
		mutex mtx; //para la ejecución de procs en exclusión mutua

		condition_variable enEspera;

        set<Tupla> almacen;

	public:
		//------------------------- constructor
		MonitorServidor(set<Tupla> *almacen);
		//------------------------- destructor
		~MonitorServidor();
		//Los "r" serán los recursos que se quieren reservar o liberar.

		void disponible(Tupla tupla);
        void borrar (Tupla tupla);
		void guardar (Tupla tupla);
};
#endif