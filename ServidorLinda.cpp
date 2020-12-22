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
void trocea_3(string s, string &operacion, string &tupla) {
	const char delim1[] = ","; //los separadores aquí son ","
	char* token;
	char* copia = strdup(s.c_str()); //trabajaremos sobre una copia

	//token = strtok(copia, delim1); //hasta el primer '[' <---- NOK
    token = strtok(copia, delim1);
	operacion = token;

	token = strtok(nullptr, "\n");
	tupla = token;
}
/*void prueba(MonitorServidor& mS){
    Tupla tprueba("prueba1","prueba");

    sleep(3);
    mS.PN(tprueba);
}
void prueba1(MonitorServidor& mS){
    Tupla tprueba("prueba1","prueba");
    //sleep(2);
    mS.RN(tprueba);
    cout<<"salgo dormido\n";
}*/
//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd, MonitorServidor& mS) {
	// Buffer para recibir el mensaje
    int length = 100;
    string buffer;

	int rcv_bytes;
	int send_bytes;

	string operacion;
	string tupla;

	//set <Tupla> :: iterator iter;			//para saber donde buscar en la lista
	//set <Tupla> :: iterator iter_fin;		//para conparar si estamos en la posicion final
	Tupla tuplaTemp("");                    //Para buscar la tupla en la memoria

    bool out = false; // Inicialmente no salir del bucle

	while(!out) {
		// Recibimos el mensaje del cliente
		rcv_bytes = soc.Recv(client_fd,buffer,length);
        cout << "paso \n";
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
            mS.PN(tuplaTemp);    //Guardamos en la coleccion la tupla que nos han pasado(llamamos al monitor)
		} else if(operacion == MENSAJE_RDN) {//lee tupla y la copia
		    mS.RdN(tuplaTemp);
		    tuplaTemp = tuplaTemp.to_string();		//Pasamos la tupla encontrada a string para enviarla

		    send_bytes = soc.Send(client_fd, tuplaTemp.to_string());    //Enviamos la tupla
		    if(send_bytes == -1) {
		        cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
		        soc.Close(client_fd); // Cerramos los sockets.
		        exit(1);
		    }
		} else if(operacion == MENSAJE_RN){ //Busca tupla y la borra
            mS.RdN(tuplaTemp);
            tuplaTemp = tuplaTemp.to_string();		//Pasamos la tupla encontrada a string para enviarla

            send_bytes = soc.Send(client_fd, tuplaTemp.to_string());    //Enviamos la tupla encontrada
            if(send_bytes == -1) {
                cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                soc.Close(client_fd); // Cerramos los sockets.
                exit(1);
            }
		} else if(operacion == MENSAJE_RDN_2){


            send_bytes = soc.Send(client_fd, tuplaTemp.to_string());    //Enviamos la tupla
            if(send_bytes == -1) {
                cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                soc.Close(client_fd); // Cerramos los sockets.
                exit(1);
            }
        } else if(operacion == MENSAJE_RN_2){


            send_bytes = soc.Send(client_fd, tuplaTemp.to_string());    //Enviamos la tupla
            if(send_bytes == -1) {
                cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                soc.Close(client_fd); // Cerramos los sockets.
                exit(1);
            }
        }
	}
}

int main(int argc, char *argv[]) {
    Tupla tprueba("prueba1","prueba");
    Tupla match("prueba1","prueba");
    Tupla matchG("?Z","prueba");
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
    thread p(&prueba, ref(mS1));
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

    if (argc != 2) {
        cerr << "Número de parámetros incorrecto \n";
        cerr << "Introduce ./ServidorMulticliente, puerto del servidor para hacer bind\n";
        exit(1); // finaliza el programa indicando salida incorrecta (1)
    }
    const int N = 5;

	//Creamos el tipo de set que vamos a usar (donde guardamos las tuplas)
    multiset<Tupla> almacen;

	MonitorServidor mS(&almacen);

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
