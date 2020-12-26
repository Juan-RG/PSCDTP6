//--------------------------------------------------------------------------------------------------
// File:    ServidorLinda.cpp
// Author:
// Date:    diciembre 2020
// Comms:   Servidor que provee recursos a múltiples clientes.
//--------------------------------------------------------------------------------------------------


#include "Socket/Socket.hpp"
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

static const string MENSAJE_PN = "PN";
static const string MENSAJE_RN = "RN";
static const string MENSAJE_RDN = "RDN";
static const string MENSAJE_RN_2 = "RN_2";
static const string MENSAJE_RDN_2 = "RDN_2";

static const string MENSAJE_DESCONEXION = "DESCONEXION";
static const string RECIBIDO = "OK";

//-------------------------------------------------------------
void trocea(string s, string &t1, string &t2) {
	const char delim1[] = ","; //los separadores aquí son ","
	char* token;
	char* copia = strdup(s.c_str()); //trabajaremos sobre una copia

	//token = strtok(copia, delim1); //hasta el primer '[' <---- NOK
    token = strtok(copia, delim1);
    t1 = token;

	token = strtok(nullptr, "\n");
    t2 = token;
}

void prueba(MonitorServidor& mS){
    Tupla tprueba("prueba1","prueba2");
    Tupla tprueba2("prueba1","prueba1");
    mS.PN(tprueba);
    mS.PN(tprueba2);
}
void prueba1(MonitorServidor& mS){
    Tupla tprueba("?X","prueba2");
    //sleep(2);
    mS.RN(tprueba);
    cout<<"salgo dormido\n";
}

// [prueba1, prueba2] [prueba1, prueba2]
// [prueba1, prueba1] [prueba1, prueba1]

// ----->OK
// Tupla tprueba1("?X","?Y");
// Tupla tprueba2("?Y","?X");
//
// ----->OK
// Tupla tprueba1("?X","?Y");
// Tupla tprueba2("prueba1","?X");
//
// ----->OK
// Tupla tprueba1("prueba1","?Y");
// Tupla tprueba2("prueba1","?X");
//
// ----->OK (no debería salir encontrado)
// Tupla tprueba1("prueba2","?Y");
// Tupla tprueba2("prueba1","prueba2");
//
// ----->OK (no debería salir encontrado)
// Tupla tprueba1("prueba2","?Y");
// Tupla tprueba2("prueba1","prueba2");

// [aqui, prueba2] [prueba1, aqui]          ---> [prueba1, prueba2] [prueba1, prueba1]


void prueba2(MonitorServidor& mS) {
    cout << "entro RDN2" << endl;
    Tupla tprueba1("?X","?Y");
    Tupla tprueba2("prueba1","?X");
    //sleep(2);
    mS.RdN_2(tprueba1, tprueba2);
    cout<<"RDN2!\n";
}

//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd, MonitorServidor& mS) {
	// Buffer para recibir el mensaje
    int length = 100;
    string buffer;

	int rcv_bytes;
	int send_bytes;

	string operacion;
	string tupla;

	Tupla tuplaTemp("");                    //Para buscar la tupla en la memoria
    Tupla tuplaTemp2("");                   //Para buscar la tupla en la memoria para operaciones "2".
    bool out = false;                       // Inicialmente no salir del bucle

	//while(!out) {
		// Recibimos el mensaje del cliente
		rcv_bytes = soc.Recv(client_fd,buffer,length);
        cout << "paso \n";
		if (rcv_bytes == -1) {
			cerr << "Error al recibir datos: " + string(strerror(errno)) + "\n";
			soc.Close(client_fd); // Cerramos los sockets.
			exit(1);
		}

        trocea(buffer, operacion, tupla);			//Separamos la orden de la tupla (son strings)
        string tuplaString;
		if(operacion == MENSAJE_PN) {//postnote, mete algo en memoria
            tuplaTemp.from_string(tupla);	//pasamos la tupla tipo string a tipo "tupla"

            mS.PN(tuplaTemp);    //Guardamos en la coleccion la tupla que nos han pasado(llamamos al monitor)

            send_bytes = soc.Send(client_fd, RECIBIDO);
            if(send_bytes == -1) {
                cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                soc.Close(client_fd); // Cerramos los sockets.
                exit(1);
            }
		} else if(operacion == MENSAJE_RDN) {//lee tupla y la copia
            tuplaTemp.from_string(tupla);	//pasamos la tupla tipo string a tipo "tupla"

            mS.RdN(tuplaTemp);

		    send_bytes = soc.Send(client_fd, tuplaTemp.to_string());    //Enviamos la tupla
		    if(send_bytes == -1) {
		        cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
		        soc.Close(client_fd); // Cerramos los sockets.
		        exit(1);
		    }
		} else if(operacion == MENSAJE_RN){ //Busca tupla y la borra
            tuplaTemp.from_string(tupla);	//pasamos la tupla tipo string a tipo "tupla"

            mS.RdN(tuplaTemp);

            send_bytes = soc.Send(client_fd, tuplaTemp.to_string());    //Enviamos la tupla encontrada
            if(send_bytes == -1) {
                cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                soc.Close(client_fd); // Cerramos los sockets.
                exit(1);
            }
		} else if(operacion == MENSAJE_RDN_2){
            string tuplaString1 = "";
            string tuplaString2 = "";

            trocea(tupla, tuplaString1, tuplaString2);      //Separamos la tupla doble para guardarla en dos

            cout << "LA TUPLA 1: " << tuplaString1 << endl;
            cout << "LA TUPLA 2: " << tuplaString2 << endl;

            tuplaTemp.from_string(tuplaString1);		    //pasamos la tupla 1 tipo string a tipo "tupla"
            tuplaTemp2.from_string(tuplaString2);			//pasamos la tupla 1 tipo string a tipo "tupla"

            mS.RdN_2(tuplaTemp, tuplaTemp2);         //Llamamos a la operacion de servidor

            tuplaString = tuplaTemp.to_string() + tuplaTemp2.to_string(); //Juntamos las dos tuplas para enviarlas

            send_bytes = soc.Send(client_fd, tuplaString);   //Enviamos la Tupla.
            if(send_bytes == -1) {
                cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                soc.Close(client_fd); // Cerramos los sockets.
                exit(1);
            }
        } else if(operacion == MENSAJE_RN_2){
            string tuplaString1 = "";
            string tuplaString2 = "";

            trocea(tupla, tuplaString1, tuplaString2);      //Separamos la tupla doble para guardarla en dos

            cout << "LA TUPLA 1: " << tuplaString1 << endl;
            cout << "LA TUPLA 2: " << tuplaString2 << endl;

            tuplaTemp.from_string(tuplaString1);		    //pasamos la tupla 1 tipo string a tipo "tupla"
            tuplaTemp2.from_string(tuplaString2);			//pasamos la tupla 1 tipo string a tipo "tupla"

            mS.RN_2(tuplaTemp, tuplaTemp2);         //Llamamos a la operacion de servidor

            tuplaString = tuplaTemp.to_string() + tuplaTemp2.to_string(); //Juntamos las dos tuplas para enviarlas

            send_bytes = soc.Send(client_fd, tuplaString);   //Enviamos la Tupla.
            if(send_bytes == -1) {
                cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                soc.Close(client_fd); // Cerramos los sockets.
                exit(1);
            }
        } else if(operacion != MENSAJE_DESCONEXION){
		    out = true;
		}
	//}
}

int main(int argc, char *argv[]) {
    /*
    //Tupla match("prueba1","prueba");
    Tupla matchG("?Z","prueba");
    //cout << matchG.match(match)<< "\n";
    string operacion,tupla;
    string buffer = "PN,[2,hola,adios,oaooaoa,asdasda,asdasda]";
    trocea_3(buffer, operacion, tupla);
    //cout << "buffer: '"<<  buffer << "' operacion '" << operacion << "' tupla '" << tupla << "'" << endl;
    Tupla t1(4); // TODO: Ver si se puede meter el constructor en from_string, tal que no haya que decir el tamaño de la tupla antes de meterle el string
    t1.from_string(tupla);
    //cout << "n tupla " << t1.to_string()<<"\n";
*/
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
    cout<<"asdasdada\n";
    mS1.PN(t2);
    cout<<"asdasdasdadasdadwwwwwwwwwwwwww\n";
    mS1.mostrar();
    mS1.borrar(t1);
    mS1.mostrar();
    cout<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11\n";
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
    cout<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!12\n";
    Tupla t5(3);//0-2
    cout<< "??????????????????????????\n";

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
    cout<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n";
// ejemplos de RemoveNote
    mS1.RN(p1); // res1 tomar´a ´el valor que ten´ıa t2
    mS1.RN(p2); // res2 tomar´a el valor que ten´ıa t3
    cout << p1.to_string() << endl; //mostrar´a [1000]
    cout << p2.to_string() << endl; //mostrar´a [a,b,c,45,34,pan]

    cout<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!2\n";
// ¿Si necesitamos un array de tuplas?
// Tupla v[2]; // NO permitido: no hay constructor por defecto
    Tupla* v[2];
    v[0] = new Tupla("Juan", "1000");
    v[1] = new Tupla("Luisa", "1000", "enero");
    cout<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!3\n";
    delete v[0];
    delete v[1];

    Tupla t6("EINA","AOC","DIIS");
    Tupla t7("EINA","PSCD","DIIS");
    mS1.PN(t6);
    mS1.PN(t7);
    Tupla p3("EINA","?Y","?X");
    Tupla p4("EINA","?Z","?X");
    cout<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!4\n";
    Tupla t8(3), t9(3);
    mS1.RdN_2(p3,p4);
// Podr´a haber cargado t8 con ["EINA","AOC","DIIS"] y
// t9 con ["EINA","PSCD","DIIS"], o viceversa
    cout << p3.to_string() << endl;
    cout << p4.to_string() << endl;

    Tupla p5("EINA","?Y","?X");
    Tupla p6("EINA","?Z","?X");
    mS1.RN_2(p5,p6);
// Podr´a haber cargado t8 con ["EINA","AOC","DIIS"] y
// t9 con ["EINA","PSCD","DIIS"], o viceversa
    cout << p5.to_string() << endl;
    cout << p6.to_string() << endl;

    //thread p(&prueba1, ref(mS1));
    //thread p1(&prueba1, ref(mS1));
    //thread p2(&prueba1, ref(mS1));


    //thread p3(&prueba, ref(mS1));

    //thread p4(&prueba, ref(mS1));

    //thread p5(&prueba, ref(mS1));

    //p.join();
    //p1.join();
    //p2.join();


    //p3.join();

    //p4.join();

    //p5.join();




    //thread p6(&prueba2, ref(mS1));

    //p6.join();


   /*
    Tupla prueba("prueba1","prueba");
    mS1.PN(match);
    mS1.PN(matchG);
    mS1.RdN(prueba);
*/
  /*
    cout << "primera prueba "<< tprueba.match(match)<<"\n";
    cout << "2 prueba "<< match.match(tprueba)<<"\n";
    cout << "3 prueba "<< tprueba.match(matchG)<<"\n";
    cout << "4 prueba "<< matchG.match(tprueba)<<"\n";
    // PRUEBAS de añadir tuplas al set con el monitor

    string buffer = "PN,[1,zaragoza,valencia,5]";
    string operacion,tupla;
    trocea_3(buffer, operacion, tupla);
    cout << "buffer: '"<<  buffer << "' operacion '" << operacion << "' tupla '" << tupla << "'" << endl;
    multiset<Tupla> almacenPrueba;
    MonitorServidor mS1(&almacenPrueba);
    Tupla t(4); // TODO: Ver si se puede meter el constructor en from_string, tal que no haya que decir el tamaño de la tupla antes de meterle el string
    t.from_string(tupla);
    thread p(&prueba1, ref(mS1));
    thread p1(&prueba1, ref(mS1));
    thread p2(&prueba1, ref(mS1));
    mS1.PN(t);
    mS1.PN(tprueba);
    mS1.RN(tprueba);

    p1.join();
    p2.join();
    p.join();
    buffer = "PN,[2,teruel,34]";
    trocea_3(buffer, operacion, tupla);
    cout << "buffer: '"<<  buffer << "' operacion '" << operacion << "' tupla '" << tupla << "'" << endl;
    Tupla t1(4); // TODO: Ver si se puede meter el constructor en from_string, tal que no haya que decir el tamaño de la tupla antes de meterle el string
    t1.from_string(tupla);
    mS1.PN(t1);

    buffer = "PN,[3,Madrid,La nada,5]";
    trocea_3(buffer, operacion, tupla);
    cout << "buffer: '"<<  buffer << "' operacion '" << operacion << "' tupla '" << tupla << "'" << endl;
    Tupla t2(4); // TODO: Ver si se puede meter el constructor en from_string, tal que no haya que decir el tamaño de la tupla antes de meterle el string
    t2.from_string(tupla);
    mS1.PN(t2);
    mS1.PN(t2);
    mS1.PN(t2);
    mS1.PN(t2);
    mS1.PN(t2);
    cout<< "Guardo otra vez la tupla\n";
    buffer = "PN,[3,Madrid,La nada,5]";
    trocea_3(buffer, operacion, tupla);
    Tupla t3(4);
    t3.from_string(tupla);
    mS1.PN(t);
    */
    if (argc != 2) {
        cerr << "Número de parámetros incorrecto \n";
        cerr << "Introduce ./ServidorMulticliente, puerto del servidor para hacer bind\n";
        exit(1); // finaliza el programa indicando salida incorrecta (1)
    }
    const int N = 5;

	//Creamos el tipo de set que vamos a usar (donde guardamos las tuplas)
    unordered_multiset<Tupla,TuplaHash> almacen;
    MonitorServidor mS(&almacen);
    /*
	MonitorServidor mS(&almacen);
    Tupla tprueba("prueba1","prueba");
    Tupla tprueba3("prueba2","prueba");
    cout << "creada tupla prueba 1" << endl;
    mS.PN(tprueba2);
    mS.PN(tprueba);
    cout << "posteada tupla prueba 1 2 veces" << endl;
    mS.RdN(tprueba);
    cout << "leída tupla prueba 1" << endl;
    //mS1.RdN(tprueba2);
    cout << "leída tupla inexistente" << endl;
    // Puerto donde escucha el proceso servidor
     */
    //int SERVER_PORT = atoi(argv[1]);
    int SERVER_PORT = 22;
    thread cliente[N];
    int client_fd[N];

    // Creación del socket con el que se llevará a cabo la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // Bind
    int socket_fd = chan.Bind();
    if (socket_fd == -1) {
        cerr << "Error en el bind: " + string(strerror(errno)) + "\n";
        exit(1);
    }

    // Listen
    int max_connections = N;
    int error_code = chan.Listen(max_connections);
    if (error_code == -1) {
        cerr << "Error en el listen: " + string(strerror(errno)) + "\n";
        chan.Close(socket_fd); // Cerramos el socket.
        exit(1);
    }

    for (int i = 0; i < max_connections; i++) {
        // Accept
        client_fd[i] = chan.Accept();

        if(client_fd[i] == -1) {
            cerr << "Error en el accept: " + string(strerror(errno)) + "\n";
            chan.Close(socket_fd); // Cerramos el socket.
            exit(1);
        }

        cliente[i] = thread(&servCliente, ref(chan), client_fd[i], ref(mS));
    }

    //¿Qué pasa si algún thread acaba inesperadamente?
    for (int i = 0; i < max_connections; i++) {
        cliente[i].join();
    }

    // Cerramos el socket del servidor
    error_code = chan.Close(socket_fd);
    if (error_code == -1) {
        cerr << "Error cerrando el socket del servidor: " + string(strerror(errno)) + " aceptado" + "\n";
    }

    // Despedida
    cout << "Bye bye" << endl;

    return error_code;
}
