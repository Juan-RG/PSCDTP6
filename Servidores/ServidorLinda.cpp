//--------------------------------------------------------------------------------------------------
// File:    ServidorLinda.cpp
// Author:
// Date:    diciembre 2020
// Comms:   Servidor que provee recursos a múltiples clientes.
//--------------------------------------------------------------------------------------------------

#include "../Socket/Socket.hpp"
#include "MonitorServidor.hpp"
#include "../Tupla/Tupla.hpp"

#include <iostream>
#include <thread>
#include <cstdlib>
#include <cstring>
#include <string>

using namespace std;

static const string MENSAJE_PN = "PN";
static const string MENSAJE_RN = "RN";
static const string MENSAJE_RDN = "RDN";
static const string MENSAJE_RN_2 = "RN_2";
static const string MENSAJE_RDN_2 = "RDN_2";
static const string MENSAJE_DESCONEXION = "DESCONEXION";
static const string MENSAJE_CERRAR = "CERRAR";
static const string RECIBIDO = "OK";

//-------------------------------------------------------------


// Pre:  Recibimos tres strings, la cadena a trocear (s), y dos variables (t1 y t2) por referencia.
// Post: Trocea la cadena s y guarda en t1 los caracteres antes de llegar a una "," y el resto lo guarda en t2
void trocea(string s, string &t1, string &t2) {
    const char delim[] = ","; //los separadores aquí son ","
    char *token;
    char *copia = strdup(s.c_str()); //trabajaremos sobre una copia

    token = strtok(copia, delim);    //hasta el primer ','
    t1 = token;

    token = strtok(nullptr, "\n");
    t2 = token;
}

// Pre:  Recibimos tres strings, la cadena a trocear (s), y dos variables (t1 y t2) por referencia.
// Post: Trocea la cadena s y guarda en t1 los caracteres antes de llegar a un "]" y el resto lo guarda en t2
void troceaTuplaDoble(string s, string &t1, string &t2) {
    const char delim[] = "]"; //los separadores aquí son "]"
    char *token;
    char *copia = strdup(s.c_str()); //trabajaremos sobre una copia

    token = strtok(copia, delim);    //hasta el primer ']'
    t1 = token;
    t1 = t1 + "]";                   //Anyadimos ] al final de la primera tupla troceada

    token = strtok(nullptr, "\n");

    token = &token[1];               //apuntamos a la sig posicion para quitarnos la ","

    t2 = token;
}


//-------------------------------------------------------------

// Pre:  Funcion usada en cada hilo para cada cliente que recibe el socket, el numero de cliente y el monitor por referencia
//       inicializado en el main.
// Post: Se encarga de recibir la tupla, trocearla y dependiendo de que operacion a realizar recibe, llamara a una funcion
//       del monitor u otra (PN, RN, RdN, RN_2, RdN_2). Si es PN, envia mensaje de OK y si es RN o RdN (de una o dos tuplas),
//       envia la tupla encontrada. Finalizara cuando reciba mensaje de desconexion ("DESCONEXION").
void servCliente(Socket& soc, int client_fd, MonitorServidor& mS) {

	// Buffer para recibir el mensaje
    int length = 100;
    string buffer;

	int rcv_bytes;
	int send_bytes;

	string operacion;                       //Operacion a realizar en el servidor (RN, PN...)
	string tupla;                           //Guardamos la tupla recibida

	Tupla tuplaTemp1("");                   //Para buscar la tupla en la memoria
    Tupla tuplaTemp2("");                   //Para buscar la tupla en la memoria para operaciones "2".

    string tuplaString1 = "";               //Variable que almacena la primera tupla si pide operaciones dobles
    string tuplaString2 = "";               //Variable que almacena la segunda tupla si pide operaciones dobles

    bool out = false;                       // Inicialmente no salir del bucle

	while(!out) {
		// Recibimos el mensaje del cliente
		rcv_bytes = soc.Recv(client_fd,buffer,length);

		if (rcv_bytes == -1) {
			cerr << "Error al recibir datos: " + string(strerror(errno)) + "\n";
			soc.Close(client_fd); // Cerramos los sockets.
            std::terminate();
		}

        if (buffer == MENSAJE_DESCONEXION){  //Salimos del bucle
            out = true;
        } else if(buffer == MENSAJE_CERRAR) {
            cout << "Recibido mensaje de cierre del servidor, terminando la ejecución..." << endl;
            exit(0);
        } else {
            trocea(buffer, operacion, tupla);			//Separamos la orden de la tupla (son strings)

            string tuplaDobleString;
            if(operacion == MENSAJE_PN) {  //postnote, mete algo en memoria
                tuplaTemp1.from_string(tupla);	        //pasamos la tupla tipo string a tipo "tupla"

                mS.PN(tuplaTemp1);    //Guardamos en la coleccion la tupla que nos han pasado (llamamos al monitor)

                send_bytes = soc.Send(client_fd, RECIBIDO);
                if(send_bytes == -1) {
                    cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                    soc.Close(client_fd); // Cerramos los sockets.
                    std::terminate();
                }
            } else if(operacion == MENSAJE_RDN) {//lee tupla y la copia
                tuplaTemp1.from_string(tupla);	//pasamos la tupla tipo string a tipo "tupla"

                mS.RdN(tuplaTemp1);    //Buscamos en la coleccion la tupla que nos han pasado (llamamos al monitor)

                send_bytes = soc.Send(client_fd, tuplaTemp1.to_string());    //Enviamos la tupla encontrada
                if(send_bytes == -1) {
                    cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                    soc.Close(client_fd); // Cerramos los sockets.
                    std::terminate();
                }
            } else if(operacion == MENSAJE_RN){  //Busca tupla y la borra
                tuplaTemp1.from_string(tupla);	//pasamos la tupla tipo string a tipo "tupla"

                mS.RN(tuplaTemp1);   //Buscamos en la coleccion la tupla que nos han pasado y la eliminamos (monitor)

                send_bytes = soc.Send(client_fd, tuplaTemp1.to_string());    //Enviamos la tupla encontrada
                if(send_bytes == -1) {
                    cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                    soc.Close(client_fd); // Cerramos los sockets.
                    std::terminate();
                }
            } else if(operacion == MENSAJE_RDN_2){  //lee tupla doble y la copia
                troceaTuplaDoble(tupla, tuplaString1, tuplaString2);   //Separamos la tupla doble para guardarla en dos

                tuplaTemp1.from_string(tuplaString1);		    //pasamos la tupla 1 tipo string a tipo "tupla"
                tuplaTemp2.from_string(tuplaString2);			//pasamos la tupla 1 tipo string a tipo "tupla"

                mS.RdN_2(tuplaTemp1, tuplaTemp2);         //Llamamos a la operacion de servidor

                tuplaDobleString = tuplaTemp1.to_string() + ";" + tuplaTemp2.to_string(); //Juntamos las dos tuplas para enviarlas

                send_bytes = soc.Send(client_fd, tuplaDobleString);   //Enviamos la Tupla.
                if(send_bytes == -1) {
                    cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                    soc.Close(client_fd); // Cerramos los sockets.
                    std::terminate();
                }
            } else if(operacion == MENSAJE_RN_2){  //lee tupla doble y se la "lleva" (la borra de la coleccion)
                troceaTuplaDoble(tupla, tuplaString1, tuplaString2);   //Separamos la tupla doble para guardarla en dos

                tuplaTemp1.from_string(tuplaString1);		    //pasamos la tupla 1 tipo string a tipo "tupla"
                tuplaTemp2.from_string(tuplaString2);			//pasamos la tupla 1 tipo string a tipo "tupla"

                mS.RN_2(tuplaTemp1, tuplaTemp2);         //Llamamos a la operacion de servidor

                tuplaDobleString = tuplaTemp1.to_string() + ";" + tuplaTemp2.to_string(); //Juntamos las dos tuplas para enviarlas

                send_bytes = soc.Send(client_fd, tuplaDobleString);   //Enviamos la Tupla.
                if(send_bytes == -1) {
                    cerr << "Error al enviar confirmacion: " + string(strerror(errno)) + "\n";
                    soc.Close(client_fd); // Cerramos los sockets.
                    std::terminate();
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



    const int N = 5000;                                                                                                    // FIXME: AUMENTAR O HACER INFINITO
	//Creamos el tipo de set que vamos a usar (donde guardamos las tuplas)
    unordered_multiset<Tupla, TuplaHash> almacen;
    MonitorServidor mS(&almacen);

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
