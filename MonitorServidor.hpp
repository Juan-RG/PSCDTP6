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
#include "Tupla/Tupla.hpp"
#include "TuplaHash.cpp"

//-----------------------------------------------------
using namespace std; //mutex, condition_variable, etc.

class MonitorServidor {
private:
    mutex mtx; //para la ejecución de procs en exclusión mutua

    condition_variable enEspera;

    unordered_multiset <Tupla, TuplaHash> almacen;

    // Estructuras y funciones auxiliares a RDN_2 y RN_2
    struct comodines;
    struct comodinesComunes;

    void comodines_comunes(comodines arrayComodinesp1[], comodines arrayComodinesp2[],
                           comodinesComunes arrayComodinesComunes[], int &numComodinesComunes, int numComodinesp1,
                           int numComodinesp2);

    void buscando(Tupla &p1, Tupla &p2, bool &encontrado, int numComodinesComunes, comodinesComunes arrayComodinesComunes[]);

    void proceso_comodines(Tupla p, comodines arrayComodinesp[], int &numComodinesp);

public:
    //------------------------- constructor
    MonitorServidor(unordered_multiset <Tupla, TuplaHash> *almacen);

    //------------------------- destructor
    ~MonitorServidor();

    void PN(Tupla &tupla);

    void RdN(Tupla &tupla);

    void RN(Tupla &tupla);

    void RdN_2(Tupla &t1, Tupla &t2);

    void RN_2(Tupla &t1, Tupla &t2);
};
#endif
