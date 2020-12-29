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

    Tupla t1("1","mi␣casa","´arbol"); // 3 elementos
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
    mS1.mostrar();
    mS1.borrar(t1);
    mS1.mostrar();
    //muestra "mi casa" por stdout
    cout << t1.get(2) << endl;
    // t3.to_string() devuelve el string "[aprieta,el,pan,45,34,88]"
    string serial = t3.to_string();
    cout << serial << endl;
    // las componentes de t3 tomar´an, respectivamente,
    // los valores "a","b","c","45","34","pan"
    t3.from_string("[a,b,c,45,34,pan]");
    // mostrar´a [a,b,c,45,34,pan] por stdout
    cout << t3.to_string() << endl;
    mS1.PN(t3);
    Tupla t5(3);//0-2
    t5.set(1, "hola");
    cout<<"******************************\n";
    t5.set(2, "Mundo");  //1 2 3
    cout<< "-------------------------------\n";
    mS1.PN(t5);
    // mostrar´a [,hola,Mundo] por stdout
    cout << t5.to_string() << endl;
    // informar´a de que tiene 3 elementos
    cout << "t5␣tiene␣" << t5.size() << "␣elementos" << endl;
    // Un patr´on no es otra cosa que una tupla con
    // la posibilidad de contener el comod´ın "?" en una o m´as posiciones
    // "?" no puede ser un valor de componente de tupla. Para ello se
    // tendr´ıa que representar mediante "??". Creamos dos patrones
    Tupla p1("?X");
    Tupla p2("a","?X","c","?Y","34","?Z");
    // Dos nuevas tuplas, de tama˜no 1 y 6, respectivamente
    Tupla res1(1),res2(p2.size());
    // ejemplos de RemoveNote
    mS1.RN(p1); // res1 tomar´a ´el valor que ten´ıa t2
    mS1.RN(p2); // res2 tomar´a el valor que ten´ıa t3
    cout << p1.to_string() << endl; //mostrar´a [1000]
    cout << p2.to_string() << endl; //mostrar´a [a,b,c,45,34,pan]

    // ¿Si necesitamos un array de tuplas?
    // Tupla v[2]; // NO permitido: no hay constructor por defecto
    Tupla* v[2];
    v[0] = new Tupla("Juan", "1000");
    v[1] = new Tupla("Luisa", "1000", "enero");
    delete v[0];
    delete v[1];

    Tupla t6("EINA","AOC","DIIS");
    Tupla t7("EINA","PSCD","DIIS");
    mS1.PN(t6);
    mS1.PN(t7);
    Tupla p3("EINA","?Y","?X");
    Tupla p4("EINA","?Z","?X");
    Tupla t8(3), t9(3);
    cout << "mando RDN_2" << endl;
    mS1.RdN_2(p3,p4);
    // Podr´a haber cargado t8 con ["EINA","AOC","DIIS"] y
    // t9 con ["EINA","PSCD","DIIS"], o viceversa
    cout << "salgo de RDN_2" << endl;
    cout << p3.to_string() << endl;
    cout << p4.to_string() << endl;

    Tupla p5("EINA","?Y","?X");
    Tupla p6("EINA","?Z","?X");
    //mS1.RN_2(p5,p6);
    // Podr´a haber cargado t8 con ["EINA","AOC","DIIS"] y
    // t9 con ["EINA","PSCD","DIIS"], o viceversa
    //cout << p5.to_string() << endl;
    //cout << p6.to_string() << endl;
}