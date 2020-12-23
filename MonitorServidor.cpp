//--------------------------------------------------------------------------------------------------
// File:    MonitorServidor.cpp
// Author:
// Date:    diciembre 2020
// Coms:	Este fichero contiene la especificación de las funciones del MonitorServidor.
//--------------------------------------------------------------------------------------------------

#include "MonitorServidor.hpp"
#include <iostream>
#include <unistd.h>                                         //////// <------------------------------------------------- QUITAME PLs
#include "Tupla.hpp"

//------------------------- constructor
MonitorServidor::MonitorServidor(multiset<Tupla> *almacen){
	this->almacen = *almacen;
}

//------------------------- destructor
MonitorServidor::~MonitorServidor(){}
void MonitorServidor::borrar(Tupla &tupla){
    multiset <Tupla> :: iterator itr;
    Tupla tuplaTemp1("");
    almacen.erase(tupla);
    for (itr = almacen.begin(); itr != almacen.end(); ++itr) {
        Tupla tmp(*itr);
        tuplaTemp1.from_string(tmp.to_string());
        cout << tuplaTemp1.to_string() <<"paso\n";
    }
}
void MonitorServidor::PN(Tupla tupla) {
    unique_lock<mutex> lck(mtx);
    almacen.insert(tupla);          // Guardamos la tupla que pasamos a la operacion del monitor
    enEspera.notify_all();          //Avisamos a todos que estan en espera de que se ha anyadido una nueva tupla
    multiset <Tupla> :: iterator itr;
    Tupla tuplaTemp1("");
    for (itr = almacen.begin(); itr != almacen.end(); ++itr) {
        Tupla tmp(*itr);
        tuplaTemp1.from_string(tmp.to_string());
        cout << tuplaTemp1.to_string() <<"paso\n";
    }
}

void MonitorServidor::RdN(Tupla &tupla) {    //TODO: Tenemos que controlar el caso de que llegue un comodin ?A-Z
	unique_lock<mutex> lck(mtx);
    /**/const bool is_in = almacen.find(tupla) != almacen.end();            // Todo: if con find para detectar si la tupla esta si no comprobacion recorriendo el multiset con match
    Tupla temporal("");
    multiset <Tupla> :: iterator itr;
    bool bandera = false;
    while (!bandera){
        for (itr = almacen.begin(); itr != almacen.end(); ++itr) {
            Tupla tmp(*itr);
            if(tmp.size() == tupla.size()){
                temporal.from_string(tmp.to_string());
                if (tupla.match(temporal)) {
                    bandera = true;
                }
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
}

void MonitorServidor::RN(Tupla &tupla) {                         //TODO: Tenemos que controlar el caso de que llegue un comodin ?A-Z
    unique_lock<mutex> lck(mtx);
    /**/const bool is_in = almacen.find(tupla) != almacen.end();            // Todo: if con find para detectar si la tupla esta si no comprobacion recorriendo el multiset con match
    Tupla temporal("");
    multiset <Tupla> :: iterator itr;
    bool bandera = false;
    while (!bandera){
        for (itr = almacen.begin(); itr != almacen.end(); ++itr) {
            Tupla tmp(*itr);
            if(tmp.size() == tupla.size()){
                temporal.from_string(tmp.to_string());
                if (tupla.match(temporal)) {
                   bandera = true;
                }
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

void MonitorServidor::RdN_2(Tupla &p1, Tupla &p2) {  //TODO: Desarrollar
    multiset <Tupla>::iterator itr;
    multiset <Tupla>::iterator itr2;
    bool encontrado = false;

    if (p1.size() == p2.size()) {
        // Tuplas a sobreescribir con el resultado, de momento son copias de p1 y p2
        Tupla tuplaTemp1(p1);
        Tupla tuplaTemp2(p2);
        bool parar = false;
        int lengthArrayComodines;

        struct comodines {
            string valor;
            int numIndices;
            int indices[6]; // tamaño máximo de una tupla
        };

        struct comodinesComunes {
            string valor;
            int numIndicesp1, numIndicesp2;
            // acotados a los tamaños máximos de una tupla
            int indicesp1[6];
            int indicesp2[6];
        };

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
        bool estaba = false;
        for(int i = 0; i < p1.size(); i++) {
            if( (p1.get(i))[0] == '?') { // TODO: Sustituir por una regex
                // comprueba si ya estaba el comodín en la lista
                for (int j = 0; j < p1.size(); j++) {
                    // si está, se añade otro índice
                    if (arrayComodinesp1[j].valor == p1.get(i)) {
                        arrayComodinesp1[j].indices[arrayComodinesp1[j].numIndices] = i;
                        arrayComodinesp1[j].numIndices++;
                        estaba = true;
                    }
                }
                // si no estaba, se añade otra entrada en la lista
                if (!estaba) {
                    //cout << "paso 1\n";
                    arrayComodinesp1[numComodinesp1].valor = p1.get(i);

                    //cout << "paso 2\n";
                    //cout << "numero "<< arrayComodinesp1[numComodinesp1].numIndices<< "\n";
                    arrayComodinesp1[numComodinesp1].indices[arrayComodinesp1[numComodinesp1].numIndices] = i;
                    arrayComodinesp1[numComodinesp1].numIndices++;
                    numComodinesp1++;
                } else {
                    // se resetea estaba para la siguiente iteración
                    estaba = false;
                }
            }
        }


        //cout << "asdaasd\n";
        for (int i = 0; i < p1.size(); ++i) {
            //cout <<" aa " <<arrayComodinesp1[i].valor << "\n";
            //cout <<"sa "<< arrayComodinesp1[i].numIndices<< "\n";
            for (int j = 0; j < 6; ++j) {
                //cout <<"valores  " <<arrayComodinesp1[i].indices[j] << "\n";
            }

        }

        ////cout << "asdaaa 11\n";
        //for (int i = 0; i < arrayComodinesp1->numIndices; ++i) {

        //cout << "valor en i "<< arrayComodinesp1->indices[i]<< " sss  "<< arrayComodinesp1->valor << "\n";
        // }

        // Se guardan los comodines de la tupla p2 junto a los índices donde aparecen
        int numComodinesp2 = 0;
        estaba = false;
        for(int i = 0; i < p2.size(); i++) {
            if( (p2.get(i))[0] == '?') { // TODO: Sustituir por una regex
                // comprueba si ya estaba el comodín en la lista
                for (int j = 0; j < p2.size(); j++) {
                    // si está, se añade otro índice
                    if (arrayComodinesp2[j].valor == p2.get(i)) {
                        arrayComodinesp2[j].indices[arrayComodinesp2[j].numIndices] = i;
                        arrayComodinesp2[j].numIndices++;
                        estaba = true;
                    }
                }
                // si no estaba, se añade otra entrada en la lista
                if (!estaba) {
                    arrayComodinesp2[numComodinesp2].valor = p2.get(i);
                    arrayComodinesp2[numComodinesp2].indices[arrayComodinesp2[numComodinesp2].numIndices] = i;
                    arrayComodinesp2[numComodinesp2].numIndices++;
                    numComodinesp2++;
                } else {
                    // se resetea estaba para la siguiente iteración
                    estaba = false;
                }
            }
        }


        //cout << "segundoaaa  11\n";
        // Se juntan los comodines de ambas en el vector de estructuras de
        // comodines comunes
        int numComodinesComunes = 0;
        for (int i = 0; i < numComodinesp1; i++) {
            for (int j = 0; j < numComodinesp2; j++) {
                // si dos coinciden , se añade un nuevo comodín común
                // se sabe que en ninguno de los dos hay comodines repetidos
                //cout << "paso 1\n";
                if (arrayComodinesp1[i].valor == arrayComodinesp2[j].valor) {
                    cout << "Comodin Rep" << arrayComodinesp1[i].valor;
                    cout << "paso 2\n";
                    arrayComodinesComunes[numComodinesComunes].valor = arrayComodinesp1[i].valor;
                    cout << "paso 3\n";
                    arrayComodinesComunes[numComodinesComunes].numIndicesp1 = arrayComodinesp1[i].numIndices;
                    cout << "paso 4\n";
                    // copia el vector de indices de comodinesp1 en el de comunes para p1
                    std::copy(arrayComodinesp1[i].indices, arrayComodinesp1[i].indices + 6, arrayComodinesComunes[numComodinesComunes].indicesp1);
                    cout << "paso 5\n";
                    // hace lo mismo para p2
                    arrayComodinesComunes[numComodinesComunes].numIndicesp2 = arrayComodinesp2[j].numIndices;
                    cout << "paso 6\n";
                    // copia el vector de indices de comodinesp2 en el de comunes para p2
                    std::copy(arrayComodinesp2[j].indices, arrayComodinesp2[j].indices + 6, arrayComodinesComunes[numComodinesComunes].indicesp2);

                    numComodinesComunes++;
                }
            }
        }

        //multiset<Tupla> almacen2 = almacen
        bool sigueLocal = true;
        while (!parar) {
            for (itr = almacen.begin(); itr != almacen.end(); ++itr) {
                Tupla tmp(*itr);
                tuplaTemp1.from_string(tmp.to_string());                                        // FIXME: Tiene que haber una forma mejor de hacerlo

                if (p1.size() == tuplaTemp1.size()) { // Si la tupla obtenida es de tamaño distinto a p1, se salta
                    for (itr2 = almacen.begin(); itr2 != almacen.end(); ++itr2) {
                        if (itr != itr2) { // si el objeto al que apuntan ambos iteradores es el mismo, se descarta
                            Tupla tmp(*itr2);
                            tuplaTemp2.from_string(tmp.to_string());                                    // FIXME: Tiene que haber una forma mejor de hacerlo
                            if (p2.size() == tuplaTemp2.size()) { // Si la tupla obtenida es de tamaño distinto a p2, se salta
                                if (numComodinesComunes == 0) { // si no hay índices comunes
                                    // ya se puede matchear sin más
                                    cout << "NUMCOMODINESCOMUNES == 0" << endl;
                                    cout << p1.match(tuplaTemp1) << endl;
                                    cout << p2.match(tuplaTemp2) << endl;
                                    if (p1.match(tuplaTemp1) && p2.match(tuplaTemp2)) {
                                        cout << "Matcheo!!!! parar = true" << endl;
                                        //cout << "itr era..." << &itr << endl;
                                        //cout << "itr2 era..." << &itr2 << endl;
                                        // while ()
                                        cout << "match Si";
                                        itr2 = almacen.end();
                                        itr = almacen.end();
                                        itr--;itr2--;
                                        //cout << "ahora es..." << &itr << endl;
                                        //cout << "itr2 era..." << &itr2 << endl;
                                        //usleep(1000000);
                                        //std::chrono::milliseconds timespan(111605); // or whatever
                                        //std::this_thread::sleep_for(timespan);

                                        parar = true;
                                        encontrado = true;
                                    }
                                } else {

                                    //p1: [?y, cosa, ?y]
                                    //p2: [cosa, ?y, ?z]

                                    //tuplaTemp1: [cosita, cosa, cosin]
                                    //tuplaTemp2: [cosaza, cosita, cosin]

                                    //arrayCOmodinesCOmunes[0] = ?y
                                    cout << numComodinesComunes;
                                    cout << arrayComodinesComunes[0].numIndicesp1;
                                    cout << arrayComodinesComunes[0].numIndicesp2 << endl;

                                    sigueLocal = true;
                                    for (int i = 0;(i < numComodinesComunes) && sigueLocal; i++) {
                                        for (int j = 0;(j <arrayComodinesComunes[i].numIndicesp1)&& sigueLocal; j++) {
                                            for (int k = 0; (k < arrayComodinesComunes[i].numIndicesp2)&&sigueLocal; k++) {
                                                // sigueLocal será false si se encuentra algún par de posiciones con comodines comunes entre
                                                // las dos tuplas que sean diferentes en contenido

                                                cout << tuplaTemp2.get(arrayComodinesComunes[i].indicesp2[k]) << endl;
                                                cout << tuplaTemp1.get(arrayComodinesComunes[i].indicesp1[j]) << endl;
                                                cout << arrayComodinesComunes[i].valor << endl;

                                                sigueLocal = (tuplaTemp1.get(arrayComodinesComunes[i].indicesp1[j]) == tuplaTemp2.get(arrayComodinesComunes[i].indicesp2[k]));
                                                cout << sigueLocal << endl;
                                            }
                                        }
                                    }
                                    if (sigueLocal) { // si todos los pares de posiciones son iguales
                                        // las hemos encontrado
                                        if (p1.match(tuplaTemp1) && p2.match(tuplaTemp2)) {
                                            cout << "compruebo si matchean...:" << tuplaTemp1.to_string() << " y " << tuplaTemp2.to_string() << endl;

                                            cout << "match Si" << endl;
                                            itr2 = almacen.end();
                                            itr = almacen.end();
                                            itr--;itr2--;
                                            parar = true;
                                            encontrado = true;
                                        }
                                    }
                                }
                            } else {                                                                                        // TODO: quitar esto, es solo para comprobar
                                cout << "p2.size() != tuplaTemp2.size()!!!" << endl;
                            }
                        }

                    }
                } else {                                                                                                    // TODO: quitar esto, es solo para comprobar
                    cout << "p1.size() != tuplaTemp1.size()!!!" << endl;
                }
            }

            // No se ha encontrado
            parar = true;
        }


        if (encontrado) {
            cout << "ENCONTRADO!" << endl;
            p1.from_string(tuplaTemp1.to_string());                                                                         // FIXME: Esto es horrendo
            p2.from_string(tuplaTemp2.to_string());
        } else {
            cout << "NO ENCONTRADO!" << endl;
        }
        // devolvemos las tuplas
    } else {
        cout << "Tuplas de tamaños diferentes, mal!" << endl;                                                               // TODO: quitar esto, es solo para comprobar
    }

}


void MonitorServidor::RN_2(Tupla &t1, Tupla &t2) {  //TODO: Desarrollar
    /*
        Tupla temporal("");
    Tupla temporal2("");
    multiset <Tupla> :: iterator itr;
     */

    multiset <Tupla>::iterator itr;
    multiset <Tupla>::iterator itr2;
    int i = 0;
    cout << almacen.size() <<"\n";
    for (itr = almacen.begin(); itr != almacen.end(); ++itr) {
        for (itr2 = almacen.begin(); itr2 != almacen.end(); ++itr2) {
            cout<< "paso "<< i <<"\n";
        }
    }

}

