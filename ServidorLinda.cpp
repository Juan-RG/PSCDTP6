//--------------------------------------------------------------------------------------------------
// File:    ServidorMulticliente.cpp
// Author:  Daniel Ariza Antón y Miguel Beltrán Pardos.
// Date:    diciembre 2020
// Comms:   Servidor que provee recursos a múltiples clientes.
//--------------------------------------------------------------------------------------------------

#include "Socket.hpp"
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

static const string MENSAJE_PN = "PN,";
static const string MENSAJE_RN = "RN,";
static const string MENSAJE_RDN = "RDN,";
static const string MENSAJE_DESCONEXION = "DESCONEXION";

static const string RECIBIDO = "OK";

//-------------------------------------------------------------
void trocea_3(string s, string &operacion, string &tupla) {
	const char delim1[] = "["; //los separadores aquí son ","
	char* token;
	char* copia = strdup(s.c_str()); //trabajaremos sobre una copia

	token = strtok(copia, delim1); //hasta el primer '['
	operacion = token;

	token = strtok(nullptr, "");
	tupla = token;

	tupla = "["+tupla;
}

//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd, MonitorServidor& mS, multiset<Tupla> &almacen) {
	// Buffer para recibir el mensaje
    int length = 100;
    string buffer;

	int rcv_bytes;
	int send_bytes;

	string operacion;
	string tupla;

	//multiset <Tupla> :: iterator iter;			//para saber donde buscar en la lista
	//multiset <Tupla> :: iterator iter_fin;		//para conparar si estamos en la posicion final
	Tupla tuplaTemp;                            //Para buscar la tupla en la memoria

    bool out = false; // Inicialmente no salir del bucle

	while(!out) {
		// Recibimos el mensaje del cliente
		rcv_bytes = soc.Recv(client_fd,buffer,length);

		if (rcv_bytes == -1) {
			cerr << "Error al recibir datos: " + string(strerror(errno)) + "\n";
			soc.Close(client_fd); // Cerramos los sockets.
			exit(1);
		}

		trocea_3(buffer, operacion, tupla);			//Separamos la orden de la tupla (son strings)
		tuplaTemp.from_string(tupla);				//pasamos la tupla tipo string a tipo "tupla"

		if(operacion == MENSAJE_PN) {//postnote, mete algo en memoria
            send_bytes = soc.Send(client_fd, RECIBIDO);
            if(send_bytes == -1) {
                cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                soc.Close(client_fd); // Cerramos los sockets.
                exit(1);
            }
            mS.guardar(tuplaTemp);
			almacen.insert(tuplaTemp); //Guardamos en la coleccion la tupla que nos han pasado
		} else if(operacion == MENSAJE_RN) {//Lee tupla y la borra de memoria
            iter_fin = almacen.end();           //Buscamos la posicion final
			iter = almacen.find(tuplaTemp);     //Guardamos donde a encontrado la tupla a sacar
			if(iter != iter_fin) {
				tuplaTemp = tuplaTemp.to_string();		//Pasamos la tupla encontrada a string para enviarla
                mS.borrar(tuplaTemp);     //Borra la tupla una sola vez, si la ha encontrado
				send_bytes = soc.Send(client_fd, tuplaTemp);    //Enviamos la tupla
				if(send_bytes == -1) {
					cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
					soc.Close(client_fd); // Cerramos los sockets.
					exit(1);
				}
			}
		} else if(operacion == MENSAJE_RDN) {//lee tupla y la copia
			//Algo similar a lo anterior pero que si lo encuentra (iter != iterFin), solo lo "copia" y lo envia
			iter_fin = almacen.end();           //Buscamos la posicion final
			iter = almacen.find(tuplaTemp);     //Guardamos donde a encontrado la tupla a sacar
			if(iter != iter_fin) {
				tuplaTemp = tuplaTemp.to_string();		//Pasamos la tupla encontrada a string para enviarla
                mS.disponible(tuplaTemp);
				send_bytes = soc.Send(client_fd, tuplaTemp);    //Enviamos la tupla
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
        cerr << "Introduce ./ServidorMulticliente, puerto del servidor para hacer bind\n";
        exit(1); // finaliza el programa indicando salida incorrecta (1)
    }
    const int N = 5;

	//Creamos el tipo de set que vamos a usar (donde guardamos las tuplas)
	multiset<Tupla> almacen;

	MonitorServidor mS(almacen);

    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = atoi(argv[1]);
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

        cliente[i] = thread(&servCliente, ref(chan), client_fd[i], ref(mS), ref(almacen));
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
