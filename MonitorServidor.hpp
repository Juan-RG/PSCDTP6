//--------------------------------------------------------------------------------------------------
// File:    MonitorServidor.hpp
// Author:
// Date:    diciembre 2020
// Coms:	Este fichero contiene la declaración de las funciones y variables del MonitorServidor.
//--------------------------------------------------------------------------------------------------

//Fichero: MonitorServidor.hpp
#ifndef MONITOR_SERVIDOR_HPP
#define MONITOR_SERVIDOR_HPP

#include <mutex>
#include <condition_variable>
#include <unordered_set>
#include "Tupla.hpp"
#include "TuplaHash.cpp"

//-----------------------------------------------------
using namespace std; //mutex, condition_variable, etc.

class MonitorServidor {
	private:
		mutex mtx; //para la ejecución de procs en exclusión mutua

		condition_variable enEspera;

        unordered_multiset<Tupla,TuplaHash> almacen;

	public:
		//------------------------- constructor
		MonitorServidor(unordered_multiset<Tupla,TuplaHash> *almacen);
		//------------------------- destructor
		~MonitorServidor();
		//Los "r" serán los recursos que se quieren reservar o liberar.
        //Pre:
        void PN (Tupla tupla);
        void RdN(Tupla &tupla);
        void borrar(Tupla &tupla);
        void mostrar();
        void RN (Tupla &tupla);
        void RdN_2(Tupla &t1, Tupla &t2);
        void RN_2(Tupla &t1, Tupla &t2);

};
#endif
