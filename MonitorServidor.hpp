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

        multiset<Tupla> almacen;

	public:
		//------------------------- constructor
		MonitorServidor(multiset<Tupla> *almacen);
		//------------------------- destructor
		~MonitorServidor();
		//Los "r" serán los recursos que se quieren reservar o liberar.

		void RdN(Tupla tupla);
        void RN (Tupla tupla);
        void RdN_2(Tupla t1, Tupla t2);
        void RN_2(Tupla t1, Tupla t2);
		void PN (Tupla tupla);
};
#endif
