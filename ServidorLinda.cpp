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
//#include <iterator>

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
    const char delim[] = ","; //los separadores aquí son ","
    char* token;
    char* copia = strdup(s.c_str()); //trabajaremos sobre una copia

    token = strtok(copia, delim);    //hasta el primer ','
    t1 = token;

    token = strtok(nullptr, "\n");
    t2 = token;
}

void troceaTuplaDoble(string s, string &t1, string &t2) {
    const char delim[] = "]"; //los separadores aquí son "]"
    char* token;
    char* token2;
    char* copia = strdup(s.c_str()); //trabajaremos sobre una copia

    token = strtok(copia, delim);    //hasta el primer ']'
    t1 = token;
    t1 = t1 + "]";

    token = strtok(nullptr, "\n");


    token2 = &token[1];             //apuntamos a la sig posicion para quitarnos la ","

    t2 = token2;
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
	cout << "Hilo ServCliente creado" << endl;
    // Buffer para recibir el mensaje
    int length = 100;
    string buffer;

	int rcv_bytes;
	int send_bytes;

	string operacion;
	string tupla;

	Tupla tuplaTemp1("");                    //Para buscar la tupla en la memoria
    Tupla tuplaTemp2("");                   //Para buscar la tupla en la memoria para operaciones "2".
    bool out = false;                       // Inicialmente no salir del bucle

	while(!out) {
		// Recibimos el mensaje del cliente
		rcv_bytes = soc.Recv(client_fd,buffer,length);
        cout << "nueva iter \n";
		if (rcv_bytes == -1) {
			cerr << "Error al recibir datos: " + string(strerror(errno)) + "\n";
			soc.Close(client_fd); // Cerramos los sockets.
			exit(1);
		}
        cout << "buffer: " << buffer << endl;
        if(buffer == MENSAJE_DESCONEXION){  //Salimos del bucle
            out = true;
            cout << "recibo mensaje de desconexion, me muero" << endl;
            cout << "ahora la cond de salida es: " << out << endl;
        } else {
            cout << "no es mensaje de desconexion, miro a ver qué es" << endl;
            trocea(buffer, operacion, tupla);			//Separamos la orden de la tupla (son strings)
            cout<< buffer<<"\n";
            string tuplaDobleString;
            if(operacion == MENSAJE_PN) {//postnote, mete algo en memoria
                tuplaTemp1.from_string(tupla);	//pasamos la tupla tipo string a tipo "tupla"

                mS.PN(tuplaTemp1);    //Guardamos en la coleccion la tupla que nos han pasado(llamamos al monitor)

                send_bytes = soc.Send(client_fd, RECIBIDO);
                if(send_bytes == -1) {
                    cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                    soc.Close(client_fd); // Cerramos los sockets.
                    exit(1);
                }
            } else if(operacion == MENSAJE_RDN) {//lee tupla y la copia
                tuplaTemp1.from_string(tupla);	//pasamos la tupla tipo string a tipo "tupla"

                mS.RdN(tuplaTemp1);

                send_bytes = soc.Send(client_fd, tuplaTemp1.to_string());    //Enviamos la tupla
                if(send_bytes == -1) {
                    cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                    soc.Close(client_fd); // Cerramos los sockets.
                    exit(1);
                }
            } else if(operacion == MENSAJE_RN){ //Busca tupla y la borra
                tuplaTemp1.from_string(tupla);	//pasamos la tupla tipo string a tipo "tupla"

                mS.RN(tuplaTemp1);

                send_bytes = soc.Send(client_fd, tuplaTemp1.to_string());    //Enviamos la tupla encontrada
                if(send_bytes == -1) {
                    cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                    soc.Close(client_fd); // Cerramos los sockets.
                    exit(1);
                }
            } else if(operacion == MENSAJE_RDN_2){
                string tuplaString1 = "";
                string tuplaString2 = "";

                troceaTuplaDoble(tupla, tuplaString1, tuplaString2);      //Separamos la tupla doble para guardarla en dos

                cout << "LA TUPLA 1: " << tuplaString1 << endl;
                cout << "LA TUPLA 2: " << tuplaString2 << endl;

                tuplaTemp1.from_string(tuplaString1);		    //pasamos la tupla 1 tipo string a tipo "tupla"
                tuplaTemp2.from_string(tuplaString2);			//pasamos la tupla 1 tipo string a tipo "tupla"

                mS.RdN_2(tuplaTemp1, tuplaTemp2);         //Llamamos a la operacion de servidor

                tuplaDobleString = tuplaTemp1.to_string() +","+ tuplaTemp2.to_string(); //Juntamos las dos tuplas para enviarlas
                cout<< tuplaDobleString<<endl;
                send_bytes = soc.Send(client_fd, tuplaDobleString);   //Enviamos la Tupla.
                if(send_bytes == -1) {
                    cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                    soc.Close(client_fd); // Cerramos los sockets.
                    exit(1);
                }
            } else if(operacion == MENSAJE_RN_2){
                string tuplaString1 = "";
                string tuplaString2 = "";

                troceaTuplaDoble(tupla, tuplaString1, tuplaString2);      //Separamos la tupla doble para guardarla en dos

                cout << "LA TUPLA 1: " << tuplaString1 << endl;
                cout << "LA TUPLA 2: " << tuplaString2 << endl;

                tuplaTemp1.from_string(tuplaString1);		    //pasamos la tupla 1 tipo string a tipo "tupla"
                tuplaTemp2.from_string(tuplaString2);			//pasamos la tupla 1 tipo string a tipo "tupla"

                mS.RN_2(tuplaTemp1, tuplaTemp2);         //Llamamos a la operacion de servidor

                tuplaDobleString = tuplaTemp1.to_string() +","+ tuplaTemp2.to_string(); //Juntamos las dos tuplas para enviarlas

                send_bytes = soc.Send(client_fd, tuplaDobleString);   //Enviamos la Tupla.
                if(send_bytes == -1) {
                    cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                    soc.Close(client_fd); // Cerramos los sockets.
                    exit(1);
                }
            }
        }
	}
}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        cerr << "Número de parámetros incorrecto \n";
        cerr << "Introduce ./ServidorLinda, puerto del servidor para hacer bind\n";
        exit(1); // finaliza el programa indicando salida incorrecta (1)
    }
    const int N = 5;                                                                                                    // FIXME: AUMENTAR O HACER INFINITO

	//Creamos el tipo de set que vamos a usar (donde guardamos las tuplas)
    unordered_multiset<Tupla, TuplaHash> almacen;
    MonitorServidor mS(&almacen);

    int SERVER_PORT = atoi(argv[1]);
    //int SERVER_PORT = 22;
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
