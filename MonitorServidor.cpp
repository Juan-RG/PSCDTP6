//--------------------------------------------------------------------------------------------------
// File:    MonitorServidor.cpp
// Author:
// Date:    diciembre 2020
// Coms:	Este fichero contiene la especificación de las funciones del MonitorServidor.
//--------------------------------------------------------------------------------------------------

#include "MonitorServidor.hpp"
#include <iostream>
#include "Tupla.hpp"
#include <regex>

//------------------------- constructor
// Pre:  --
//
// Post: Crea un MonitorServidor con el almacen pasado por el argumento.
MonitorServidor::MonitorServidor(unordered_multiset<Tupla, TuplaHash> *almacen) {
	this->almacen = *almacen;
}

//------------------------- destructor
// Pre:  Existe un MonitorServidor.
//
// Post: Libera la memoria dinámica usada por el MonitorServidor.
MonitorServidor::~MonitorServidor() {}

// Pre:  Existe un MonitorServidor y una tupla pasada como argumento.
//
//
// Post: Inserta la tupla pasada como argumento en el almacen.
//       Una vez insertada, desbloquea a cualquier otro proceso bloqueado en espera de una nueva tupla.
void MonitorServidor::PN(Tupla tupla) {
    unique_lock<mutex> lck(mtx);
    cout << "Operacion PN llamada" << endl;
    almacen.insert(tupla);          // Guardamos la tupla que pasamos a la operacion del monitor

    unordered_multiset<Tupla, TuplaHash> :: iterator itr;
    Tupla tuplaTemp1("");
    for (itr = almacen.begin(); itr != almacen.end(); ++itr) {     //FIXME ESTE FOR PARA PRUEBAS, MÁTAME POR FAVOR
        Tupla tmp(*itr);
        tuplaTemp1.from_string(tmp.to_string());
        cout << tuplaTemp1.to_string() <<"paso\n";
    }

    enEspera.notify_all();          //Avisamos a todos que estan en espera de que se ha anyadido una nueva tupla
}

// Pre:  Existe un MonitorServidor y una tupla pasada como argumento.
//
//
// Post: Busca la tupla pasada como argumento en el almacén y la devuelve como resultado.
//       En caso de no encontrarla en el almacén, el proceso se quedará bloqueado hasta que se realice un nuevo
//       PostNote, y procederá a buscar de nuevo la tupla en el almacén.
//       Este ciclo se repite hasta encontrar una coincidencia.
//

void MonitorServidor::RdN(Tupla &tupla) {
    unique_lock<mutex> lck(mtx);
    cout << "Operacion RdN llamada" << endl; //FIXME ¿QUITAR EN LA VERSIÓN FINAL?
    Tupla resultado("");
    unordered_multiset<Tupla, TuplaHash> :: iterator itr;
    bool bandera = false;

    while (!bandera) {
        itr = almacen.begin();
        while((itr != almacen.end()) || (!bandera)){
            Tupla tmp(*itr);
            if( ( tmp.size() == tupla.size() ) && ( tupla.match(tmp) ) ) {
                resultado.from_string(tmp.to_string());
                bandera = true;
            }
            ++itr;
        }
        if (!bandera) {
            cout << "Operacion RdN bloqueada\n";
            enEspera.wait(lck);
        }
    }

    tupla.from_string(resultado.to_string());
}

// Pre:  Existe un MonitorServidor y una tupla pasada como argumento.
//
//
// Post: Busca la tupla pasada como argumento en el almacén y la elimina.
//       En caso de no encontrarla en el almacén, el proceso se quedará bloqueado hasta que se realice un nuevo
//       PostNote, y procederá a buscar de nuevo la tupla en el almacén.
//       Este ciclo se repite hasta encontrar una coincidencia.
//
void MonitorServidor::RN(Tupla &tupla) {
    unique_lock<mutex> lck(mtx);
    cout << "Operacion RN llamada" << endl;                     //FIXME ¿QUITAR EN LA VERSIÓN FINAL?
    Tupla resultado("");
    unordered_multiset<Tupla, TuplaHash> :: iterator itr;
    bool bandera = false;

    while (!bandera) {
        itr = almacen.begin();
        while((itr != almacen.end()) || (!bandera)){
            Tupla tmp(*itr);
            if( ( tmp.size() == tupla.size() ) && ( tupla.match(tmp) ) ) {
                resultado.from_string(tmp.to_string());
                bandera = true;
            }
            ++itr;
        }
        if (!bandera) {
            cout << "Operacion RdN bloqueada\n";
            enEspera.wait(lck);
        }
    }

    tupla.from_string(resultado.to_string());
    almacen.erase(almacen.equal_range(resultado).first);
}

// Pre:  Existe un MonitorServidor.
//
// Post: Crea una estructura comodines con 3 variables.
struct MonitorServidor::comodines {
    string valor;
    int numIndices;
    int indices[6]; // tamaño máximo de una tupla
};

// Pre:  Existe un MonitorServidor.
//
// Post: Crea una estructura comodinesComunes con 5 variables.
struct MonitorServidor::comodinesComunes {
    string valor;
    int numIndicesp1, numIndicesp2;
    // acotados a los tamaños máximos de una tupla
    int indicesp1[6];
    int indicesp2[6];
};

// Pre:  Existe un MonitorServidor y una Tupla p.
//
// Post: Busca los comodines de la tupla p, los devuelve por referencia mediante un array de comodines y el numero de
//       comodines en numComodinesp.

void MonitorServidor::proceso_comodines(Tupla p, comodines arrayComodinesp[], int &numComodinesp) {
    regex e("\\?[A-Z]");
    bool estaba = false;
    for (int i = 0; i < p.size(); i++) {
        if (regex_match((p.get(i)), e)) {
            // comprueba si ya estaba el comodín en la lista
            for (int j = 0; j < p.size(); j++) {
                // si está, se añade otro índice
                if (arrayComodinesp[j].valor == p.get(i)) {
                    arrayComodinesp[j].indices[arrayComodinesp[j].numIndices] = i;
                    arrayComodinesp[j].numIndices++;
                    estaba = true;
                }
            }
            // si no estaba, se añade otra entrada en la lista
            if (!estaba) {
                arrayComodinesp[numComodinesp].valor = p.get(i);
                arrayComodinesp[numComodinesp].indices[arrayComodinesp[numComodinesp].numIndices] = i;
                arrayComodinesp[numComodinesp].numIndices++;
                numComodinesp++;
            } else {
                // se resetea estaba para la siguiente iteración
                estaba = false;
            }
        }
    }
}

// Pre:  Existe un MonitorServidor y dos arrays de comodines.
//
// Post: Compara los comodines de los arrays y los devuelve  los que coinciden mediante un array de comodinesComunes
//       y el numero de comodines comunes en numComodinesComunes
//
void MonitorServidor::comodines_comunes(comodines arrayComodinesp1[], comodines arrayComodinesp2[],
                                        comodinesComunes arrayComodinesComunes[], int &numComodinesComunes,
                                        int numComodinesp1, int numComodinesp2) {
    // Se juntan los comodines de ambas en el vector de estructuras de
    // comodines comunes
    numComodinesComunes = 0;
    for (int i = 0; i < numComodinesp1; i++) {
        for (int j = 0; j < numComodinesp2; j++) {
            // si dos coinciden , se añade un nuevo comodín común
            // se sabe que en ninguno de los dos hay comodines repetidos
            if (arrayComodinesp1[i].valor == arrayComodinesp2[j].valor) {
                arrayComodinesComunes[numComodinesComunes].valor = arrayComodinesp1[i].valor;
                arrayComodinesComunes[numComodinesComunes].numIndicesp1 = arrayComodinesp1[i].numIndices;
                // copia el vector de indices de comodinesp1 en el de comunes para p1
                std::copy(arrayComodinesp1[i].indices, arrayComodinesp1[i].indices + 6,
                          arrayComodinesComunes[numComodinesComunes].indicesp1);
                // hace lo mismo para p2
                arrayComodinesComunes[numComodinesComunes].numIndicesp2 = arrayComodinesp2[j].numIndices;
                // copia el vector de indices de comodinesp2 en el de comunes para p2
                std::copy(arrayComodinesp2[j].indices, arrayComodinesp2[j].indices + 6,
                          arrayComodinesComunes[numComodinesComunes].indicesp2);

                numComodinesComunes++;
            }
        }
    }
}

// Pre:  Existe un MonitorServidor y dos tuplas p1 y p2.
//
// Post: Busca las tuplas pasadas como argumento en el almacen y las devuelve el resultado.
//       En el caso de no encontrarlas devolveria las tuplas originales p1 y p2.
void MonitorServidor::buscando(Tupla &p1, Tupla &p2, bool &encontrado, int numComodinesComunes,
                               comodinesComunes arrayComodinesComunes[]) {
    unordered_multiset<Tupla, TuplaHash>::iterator itr;
    unordered_multiset<Tupla, TuplaHash>::iterator itr2;
    bool sigueLocal = true;
    for (itr = almacen.begin(); itr != almacen.end();) {
        Tupla tuplaTemp1(*itr);         // FIXME: Tiene que haber una forma mejor de hacerlo
        cout << "iter1" << endl;
        if (p1.size() == tuplaTemp1.size()) { // Si la tupla obtenida es de tamaño distinto a p1, se salta
            for (itr2 = almacen.begin(); itr2 != almacen.end();) {
                if (itr != itr2) { // si el objeto al que apuntan ambos iteradores es el mismo, se descarta
                    Tupla tuplaTemp2(*itr2);
                    // FIXME: Tiene que haber una forma mejor de hacerlo
                    if (p2.size() == tuplaTemp2.size()) { // Si la tupla obtenida es de tamaño distinto a p2, se salta
                        if (numComodinesComunes == 0) { // si no hay índices comunes
                            if (p1.match(tuplaTemp1) && p2.match(tuplaTemp2)) {
                                //p1.igual(tuplaTemp1);
                                //p2.igual(tuplaTemp2);
                                p1.from_string(tuplaTemp1.to_string());
                                p2.from_string(tuplaTemp2.to_string());
                                encontrado = true;
                                itr2 = almacen.end();
                            } else {
                                itr2++;
                            }
                        } else {
                            sigueLocal = true;
                            for (int i = 0; (i < numComodinesComunes) && sigueLocal; i++) {
                                for (int j = 0; (j < arrayComodinesComunes[i].numIndicesp1) && sigueLocal; j++) {
                                    for (int k = 0; (k < arrayComodinesComunes[i].numIndicesp2) && sigueLocal; k++) {
                                        // sigueLocal será false si se encuentra algún par de posiciones con comodines comunes entre
                                        // las dos tuplas que sean diferentes en contenido

                                        sigueLocal = (tuplaTemp1.get(arrayComodinesComunes[i].indicesp1[j]) ==
                                                      tuplaTemp2.get(arrayComodinesComunes[i].indicesp2[k]));
                                    }
                                }
                            }
                            if (sigueLocal) { // si todos los pares de posiciones son iguales
                                // las hemos encontrado
                                if (p1.match(tuplaTemp1) && p2.match(tuplaTemp2)) {
                                    //p1.igual(tuplaTemp1);                                                                         // FIXME: Esto es horrendo
                                    //p2.igual(tuplaTemp2);

                                    p1.from_string(tuplaTemp1.to_string());
                                    p2.from_string(tuplaTemp2.to_string());
                                    encontrado = true;
                                    itr2 = almacen.end();
                                }
                            } else {
                                itr2++;
                            }
                        }
                    } else {
                        itr2++;
                    }
                } else {
                    itr2++;
                }
            }
        }
        if (encontrado) {
            itr = almacen.end();
        } else {
            itr++;
        }
    }
}

// Pre:  Existe un MonitorServidor y dos tuplas pasadas como argumentos.
//
//
// Post: Busca las dos tuplas pasadas como argumentos en el almacén y las devuelve como resultado.
//       En caso de no encontrarla en el almacén, el proceso se quedará bloqueado hasta que se realice un nuevo
//       PostNote, y procederá a buscarlas de nuevo las tuplas en el almacén.
//       Este ciclo se repite hasta encontrar una coincidencia.
//
void MonitorServidor::RdN_2(Tupla &p1,
                            Tupla &p2) {                                                                     // TODO: Desarrollar
    unique_lock <mutex> lck(mtx);
    bool encontrado = false;
    if (p1.size() == p2.size()) {
        // Tuplas a sobreescribir con el resultado, de momento son copias de p1 y p2
        Tupla tuplaTemp1(p1);
        Tupla tuplaTemp2(p2);
        bool parar = false;
        int lengthArrayComodines;
        int numComodinesComunes = 0;

        // acotados a los tamaños de cada una de las tuplas
        //?x -> arrayComodines[0] y los valores
        comodines arrayComodinesp1[p1.size()];
        for (int i = 0; i < p1.size(); ++i) {
            arrayComodinesp1[i].numIndices = 0;
        }
        comodines arrayComodinesp2[p2.size()];
        for (int i = 0; i < p2.size(); ++i) {
            arrayComodinesp2[i].numIndices = 0;
        }
        comodinesComunes arrayComodinesComunes[p1.size()]; // max: el tamaño
        // de la menor
        // o de cualquiera
        // de las dos


        // Se guardan los comodines de la tupla p1 junto a los índices donde aparecen
        int numComodinesp1 = 0;
        proceso_comodines(p1, arrayComodinesp1, numComodinesp1);
        int numComodinesp2 = 0;
        proceso_comodines(p2, arrayComodinesp2, numComodinesp2);
        comodines_comunes(arrayComodinesp1, arrayComodinesp2, arrayComodinesComunes, numComodinesComunes,
                          numComodinesp1, numComodinesp2);
        while (!encontrado) {

            buscando(p1, p2, encontrado, numComodinesComunes, arrayComodinesComunes);
            if (!encontrado) {
                cout << "bloqueo\n";
                enEspera.wait(lck);
            }
        }
        if (encontrado) {
            cout << "ENCONTRADO!" << endl;
        } else {
            cout << "NO ENCONTRADO!" << endl;
        }
        // devolvemos las tuplas
    } else {
        cout << "Tuplas de tamaños diferentes, mal!"
             << endl;                                                               // TODO: quitar esto, es solo para comprobar
    }

}

// Pre:  Existe un MonitorServidor y dos tuplas pasadas como argumentos.
//
//
// Post: Busca las dos tuplas pasadas como argumentos en el almacén y borra 2 que coincidan.
//       En caso de no encontrarlas en el almacén, el proceso se quedará bloqueado hasta que se realice un nuevo
//       PostNote, y procederá a buscar de nuevo las tuplas en el almacén.
//       Este ciclo se repite hasta encontrar una coincidencia.
//
void MonitorServidor::RN_2(Tupla &p1,
                           Tupla &p2) {                                                                     // TODO: Desarrollar
    unique_lock <mutex> lck(mtx);
    bool encontrado = false;
    if (p1.size() == p2.size()) {
        // Tuplas a sobreescribir con el resultado, de momento son copias de p1 y p2
        Tupla tuplaTemp1(p1);
        Tupla tuplaTemp2(p2);
        int numComodinesComunes = 0;

        // acotados a los tamaños de cada una de las tuplas
        //?x -> arrayComodines[0] y los valores
        comodines arrayComodinesp1[p1.size()];
        for (int i = 0; i < p1.size(); ++i) {
            arrayComodinesp1[i].numIndices = 0;
        }
        //cout<< "inicializo "<< arrayComodinesp1->numIndices << " a s "<< arrayComodinesp1->valor << " asd "<< arrayComodinesp1->indices[1]<<"\n";
        comodines arrayComodinesp2[p2.size()];
        for (int i = 0; i < p2.size(); ++i) {
            arrayComodinesp2[i].numIndices = 0;
        }
        comodinesComunes arrayComodinesComunes[p1.size()]; // max: el tamaño
        // de la menor
        // o de cualquiera
        // de las dos


        // Se guardan los comodines de la tupla p1 junto a los índices donde aparecen


        int numComodinesp1 = 0;
        proceso_comodines(p1, arrayComodinesp1, numComodinesp1);
        int numComodinesp2 = 0;
        proceso_comodines(p2, arrayComodinesp2, numComodinesp2);
        comodines_comunes(arrayComodinesp1, arrayComodinesp2, arrayComodinesComunes, numComodinesComunes,
                          numComodinesp1, numComodinesp2);
        while (!encontrado) {
            buscando(p1, p2, encontrado, numComodinesComunes, arrayComodinesComunes);
            if (!encontrado) {
                cout << "bloqueo\n";
                enEspera.wait(lck);
            }
        }
        if (encontrado) {
            cout << "ENCONTRADO!" << endl;
            almacen.erase(almacen.equal_range(p1).first);
            almacen.erase(almacen.equal_range(p2).first);
        } else {
            cout << "NO ENCONTRADO!" << endl;
        }
        // devolvemos las tuplas
    } else {
        cout << "Tuplas de tamaños diferentes, mal!" << endl;                                                               // TODO: quitar esto, es solo para comprobar
    }

}

