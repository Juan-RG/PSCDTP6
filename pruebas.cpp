//
// Created by sam on 27/12/20.
//
#include "MonitorServidor.hpp"
#include "Tupla.hpp"

#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <cstring>
#include <string>

//Librerias para guardar datos dinamicamente
#include <set>
#include <iterator>

using namespace std;

int main() {
    unordered_multiset<Tupla,TuplaHash> almacenPrueba;
    MonitorServidor mS1(&almacenPrueba);

    Tupla t1("prueba1","prueba2","prueba 3","prueba4"); // 3 elementos
    Tupla t2("1000");
    Tupla t3("aprieta","el","pan","45","34","88");
    Tupla t4("aprieta","fuerte","pan","tt","34","pan");

    mS1.PN(t1);
    mS1.PN(t1);
    mS1.PN(t1);
    mS1.PN(t1);
    mS1.PN(t1);
    mS1.PN(t1);
    mS1.PN(t2);
    mS1.PN(t3);
    mS1.PN(t3);
    mS1.PN(t3);
    mS1.PN(t3);
    mS1.RN(t1);
    mS1.RN(t1);
    mS1.RN(t1);
    mS1.RN(t1);
    mS1.RN(t1);
    mS1.RN(t1);
    mS1.RN(t1);
    mS1.RN(t1);
    mS1.PN(t2);

}