//--------------------------------------------------------------------------------------------------
// File:    MonitorServidor.cpp
// Date:    Enero 2021
// Coms:	Especificación de las funciones del MonitorServidor.
//--------------------------------------------------------------------------------------------------

#include "MonitorServidor.hpp"
#include <iostream>
#include "../Tupla/Tupla.hpp"
#include <regex>

//------------------------- constructor
// Pre:  --
//
// Post: Crea un MonitorServidor con el almacen pasado por el argumento.
MonitorServidor::MonitorServidor(unordered_multiset<Tupla, TuplaHash> *almacen) {
    this->almacen = *almacen;
}

//------------------------- destructor
// Pre:  --
//
// Post: Libera la memoria dinámica usada por el MonitorServidor.
MonitorServidor::~MonitorServidor() {}

// Pre:  --
//
//
// Post: Inserta la tupla pasada como argumento en el almacen.
//       Una vez insertada, desbloquea a cualquier otro proceso bloqueado en espera de una nueva tupla.
void MonitorServidor::PN(Tupla &tupla) {
    unique_lock<mutex> lck(mtx);
    almacen.insert(tupla);          // Guardamos la tupla que pasamos a la operacion del monitor
    unordered_multiset<Tupla, TuplaHash>::iterator itr;

    // Avisamos a todos que están en espera de que se ha añadido una nueva tupla, según el tamaño de la misma
    if (tupla.size() == 1) {
        enEsperaTam1.notify_all();
    } else if (tupla.size() == 2) {
        enEsperaTam2.notify_all();
    } else if (tupla.size() == 3) {
        enEsperaTam3.notify_all();
    } else if (tupla.size() == 4) {
        enEsperaTam4.notify_all();
    } else if (tupla.size() == 5) {
        enEsperaTam5.notify_all();
    } else {
        enEsperaTam6.notify_all();
    }
}

// Pre:  --
//
//
// Post: Busca la tupla pasada como argumento en el almacén y la devuelve como resultado.
//       En caso de no encontrarla en el almacén, el proceso se quedará bloqueado hasta que se realice un nuevo
//       PostNote, y procederá a buscar de nuevo la tupla en el almacén.
//       Este ciclo se repite hasta encontrar una coincidencia.
void MonitorServidor::RdN(Tupla &tupla) {
    unique_lock<mutex> lck(mtx);
    Tupla resultado("");
    unordered_multiset<Tupla, TuplaHash>::iterator itr;
    bool bandera = false;

    while (!bandera) {
        itr = almacen.begin();
        while ((itr != almacen.end())) {
            Tupla tmp(*itr);
            if ((tmp.size() == tupla.size()) && (tupla.match(tmp))) {
                resultado.from_string(tmp.to_string());
                bandera = true;
            }
            itr++;

            if (bandera) {
                itr = almacen.end();
            }

        }
        if (!bandera) {
            // Esperamos, según el tamaño de la tupla, a que se introduzca una con el mismo tamaño
            if (tupla.size() == 1) {
                enEsperaTam1.wait(lck);
            } else if (tupla.size() == 2) {
                enEsperaTam2.wait(lck);
            } else if (tupla.size() == 3) {
                enEsperaTam3.wait(lck);
            } else if (tupla.size() == 4) {
                enEsperaTam4.wait(lck);
            } else if (tupla.size() == 5) {
                enEsperaTam5.wait(lck);
            } else {
                enEsperaTam6.wait(lck);
            }
        }
    }
    tupla.from_string(resultado.to_string());
}

// Pre:  --
//
//
// Post: Busca la tupla pasada como argumento en el almacén y la elimina.
//       En caso de no encontrarla en el almacén, el proceso se quedará bloqueado hasta que se realice un nuevo
//       PostNote, y procederá a buscar de nuevo la tupla en el almacén.
//       Este ciclo se repite hasta encontrar una coincidencia.
void MonitorServidor::RN(Tupla &tupla) {
    unique_lock<mutex> lck(mtx);
    Tupla resultado(tupla);

    unordered_multiset<Tupla, TuplaHash>::iterator itr;
    bool bandera = false;

    while (!bandera) {
        itr = almacen.begin();
        while ((itr != almacen.end())) {
            Tupla tmp(*itr);

            if ((tmp.size() == tupla.size()) && (tupla.match(tmp))) {
                resultado.from_string(tmp.to_string());
                bandera = true;
            }
            itr++;
            if (bandera) {
                itr = almacen.end();
            }
        }
        if (!bandera) {
            // Esperamos, según el tamaño de la tupla, a que se introduzca una con el mismo tamaño
            if (tupla.size() == 1) {
                enEsperaTam1.wait(lck);
            } else if (tupla.size() == 2) {
                enEsperaTam2.wait(lck);
            } else if (tupla.size() == 3) {
                enEsperaTam3.wait(lck);
            } else if (tupla.size() == 4) {
                enEsperaTam4.wait(lck);
            } else if (tupla.size() == 5) {
                enEsperaTam5.wait(lck);
            } else {
                enEsperaTam6.wait(lck);
            }
        }
    }

    almacen.erase(almacen.equal_range(resultado).first);
    tupla.from_string(resultado.to_string());
}

// Pre:  --
//
// Post: Crea una estructura comodines con 3 variables.
struct MonitorServidor::comodines {
    string valor;
    int numIndices;
    int indices[6]; // tamaño máximo de una tupla
};

// Pre:  --
//
// Post: Crea una estructura comodinesComunes con 5 variables.
struct MonitorServidor::comodinesComunes {
    string valor;
    int numIndicesp1, numIndicesp2;
    // acotados a los tamaños máximos de una tupla
    int indicesp1[6];
    int indicesp2[6];
};

// Pre:  --
//
// Post: Busca los comodines de la tupla p, y los devuelve por referencia mediante un array de comodines junto al número de
//       comodines en numComodinesp.
void MonitorServidor::obtenerComodines(Tupla p, comodines arrayComodinesp[], int &numComodinesp) {
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
            } else { // ya estaba
                // se resetea para la siguiente iteración
                estaba = false;
            }
        }
    }
}

// Pre:  --
//
// Post: Compara los comodines de los dos arrays introducidos y devuelve los que coinciden junto a las posiciones mediante
//       un array de comodinesComunes, junto al numero de comodines comunes en numComodinesComunes.
void MonitorServidor::obtenerComodinesComunes(comodines arrayComodinesp1[], comodines arrayComodinesp2[],
                                              comodinesComunes arrayComodinesComunes[], int &numComodinesComunes,
                                              int numComodinesp1, int numComodinesp2) {
    numComodinesComunes = 0;

    // Se juntan los comodines de ambas en el vector de estructuras de comodines comunes
    for (int i = 0; i < numComodinesp1; i++) {
        for (int j = 0; j < numComodinesp2; j++) {
            // Si dos coinciden, se añade un nuevo comodín común
            // Se sabe que en ninguno de los dos hay comodines repetidos
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

// Pre:  --
//
// Post: Obtiene pares de tuplas diferentes del almacen, y si hacen match con p1 y p2, teniendo en cuenta sus comodines comunes
//       indicados en comodinesComunes junto a su número en numComodinesComunes, las devuelve como resultado en p1 y p2.
//       En el caso de no encontrarlas devuelve las tuplas originales p1 y p2.
void MonitorServidor::busquedaDoble(Tupla &p1, Tupla &p2, bool &encontrado, int numComodinesComunes,
                               comodinesComunes arrayComodinesComunes[]) {
    // Se declaran los iteradores externo(itr) e interno(itr2)
    // tuplaTemp1 contendrá las tuplas obtenidas en el iterador externo
    // tuplaTemp2 contendrá las tuplas obtenidas en el iterador interno
    unordered_multiset<Tupla, TuplaHash>::iterator itr;
    unordered_multiset<Tupla, TuplaHash>::iterator itr2;
    bool sigueLocal = true;

    for (itr = almacen.begin(); itr != almacen.end();) {
        Tupla tuplaTemp1(*itr);
        if (p1.size() == tuplaTemp1.size()) { // Si la tupla obtenida es de tamaño distinto a p1, se salta
            for (itr2 = almacen.begin(); itr2 != almacen.end();) {
                if (itr != itr2) { // Si el objeto al que apuntan ambos iteradores es el mismo, se descarta
                    Tupla tuplaTemp2(*itr2);
                    if (p2.size() == tuplaTemp2.size()) { // Si la tupla obtenida es de tamaño distinto a p2, se salta
                        if (numComodinesComunes == 0) { // si no hay índices comunes, solo necesitan hacer match
                            if (p1.match(tuplaTemp1) && p2.match(tuplaTemp2)) {
                                p1.from_string(tuplaTemp1.to_string());
                                p2.from_string(tuplaTemp2.to_string());
                                // forzamos la salida de los bucles, ya se han encontrado
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
                            if (sigueLocal) { // si todos los pares de posiciones respecto a los comodines comunes son iguales,
                                              // necesitamos comprobar si hacen match con p1 y p2 respectivamente
                                if (p1.match(tuplaTemp1) && p2.match(tuplaTemp2)) {
                                    p1.from_string(tuplaTemp1.to_string());
                                    p2.from_string(tuplaTemp2.to_string());
                                    // forzamos la salida de los bucles, ya se han encontrado
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

// Pre:  p1.size() == p2.size()
//
// Post: Busca un par de tuplas en el almacén que hagan match con p1 y p2 respectivamente, teniendo en cuenta que el
//       contenido de las posiciones de ambas tuplas que coincidan con posiciones en p1 y p2 que tengan comodines comunes
//       a ambas deben ser iguales en las dos. En caso de que se encuentren, se devuelven en p1 y p2.
//       En caso de no encontrar ningún par de tuplas, se esperará a la condición enEspera a un nuevo PN.
void MonitorServidor::RdN_2(Tupla &p1, Tupla &p2) {
    unique_lock<mutex> lck(mtx);
    bool encontrado = false;
    if (p1.size() == p2.size()) {
        // Tuplas a sobreescribir con el resultado, de momento son copias de p1 y p2
        Tupla tuplaTemp1(p1);
        Tupla tuplaTemp2(p2);
        int numComodinesComunes = 0;

        // acotados a los tamaños de cada una de las tuplas
        comodines arrayComodinesp1[p1.size()];
        for (int i = 0; i < p1.size(); ++i) {
            arrayComodinesp1[i].numIndices = 0;
        }
        comodines arrayComodinesp2[p2.size()];
        for (int i = 0; i < p2.size(); ++i) {
            arrayComodinesp2[i].numIndices = 0;
        }
        comodinesComunes arrayComodinesComunes[p1.size()];  // max: el tamaño de la menor o de cualquiera de las dos
        // Se guardan los comodines de la tupla p1 junto a los índices donde aparecen
        int numComodinesp1 = 0;
        obtenerComodines(p1, arrayComodinesp1, numComodinesp1);
        int numComodinesp2 = 0;
        obtenerComodines(p2, arrayComodinesp2, numComodinesp2);
        obtenerComodinesComunes(arrayComodinesp1, arrayComodinesp2, arrayComodinesComunes, numComodinesComunes,
                          numComodinesp1, numComodinesp2);
        while (!encontrado) {

            busquedaDoble(p1, p2, encontrado, numComodinesComunes, arrayComodinesComunes);
            if (!encontrado) {
                if (p1.size() == 1) {
                    enEsperaTam1.wait(lck);
                } else if (p1.size() == 2) {
                    enEsperaTam2.wait(lck);
                } else if (p1.size() == 3) {
                    enEsperaTam3.wait(lck);
                } else if (p1.size() == 4) {
                    enEsperaTam4.wait(lck);
                } else if (p1.size() == 5) {
                    enEsperaTam5.wait(lck);
                } else {
                    enEsperaTam6.wait(lck);
                }
            } // else: se han sobreescrito p1 y p2 con las tuplas resultado
        }
    }
}

// Pre:  p1.size() == p2.size()
//
// Post: Busca un par de tuplas en el almacén que hagan match con p1 y p2 respectivamente, teniendo en cuenta que el
//       contenido de las posiciones de ambas tuplas que coincidan con posiciones en p1 y p2 que tengan comodines comunes
//       a ambas deben ser iguales en las dos. En caso de que se encuentren, se devuelven en p1 y p2 y se eliminan del almacen.
//       En caso de no encontrar ningún par de tuplas, se esperará a la condición enEspera a un nuevo PN.
void MonitorServidor::RN_2(Tupla &p1, Tupla &p2) {
    unique_lock<mutex> lck(mtx);
    bool encontrado = false;
    if (p1.size() == p2.size()) {
        // Tuplas a sobreescribir con el resultado, de momento son copias de p1 y p2
        Tupla tuplaTemp1(p1);
        Tupla tuplaTemp2(p2);
        int numComodinesComunes = 0;

        // acotados a los tamaños de cada una de las tuplas
        comodines arrayComodinesp1[p1.size()];
        for (int i = 0; i < p1.size(); ++i) {
            arrayComodinesp1[i].numIndices = 0;
        }

        comodines arrayComodinesp2[p2.size()];
        for (int i = 0; i < p2.size(); ++i) {
            arrayComodinesp2[i].numIndices = 0;
        }
        comodinesComunes arrayComodinesComunes[p1.size()]; // max: el tamaño de la menor o de cualquiera de las dos

        // Se guardan los comodines de la tupla p1 junto a los índices donde aparecen

        int numComodinesp1 = 0;
        obtenerComodines(p1, arrayComodinesp1, numComodinesp1);
        int numComodinesp2 = 0;
        obtenerComodines(p2, arrayComodinesp2, numComodinesp2);
        obtenerComodinesComunes(arrayComodinesp1, arrayComodinesp2, arrayComodinesComunes, numComodinesComunes,
                          numComodinesp1, numComodinesp2);
        while (!encontrado) {
            busquedaDoble(p1, p2, encontrado, numComodinesComunes, arrayComodinesComunes);
            if (!encontrado) {
                if (p1.size() == 1) {
                    enEsperaTam1.wait(lck);
                } else if (p1.size() == 2) {
                    enEsperaTam2.wait(lck);
                } else if (p1.size() == 3) {
                    enEsperaTam3.wait(lck);
                } else if (p1.size() == 4) {
                    enEsperaTam4.wait(lck);
                } else if (p1.size() == 5) {
                    enEsperaTam5.wait(lck);
                } else {
                    enEsperaTam6.wait(lck);
                }
            }
        }
        if (encontrado) { // Las tuplas p1 y p2 se han sobreescrito con el resultado, ahora se borran del almacen
            almacen.erase(almacen.equal_range(p1).first);
            almacen.erase(almacen.equal_range(p2).first);
        }

    }
}

