//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: PSCD-Unizar
// Date:   Noviembre 2015
// Coms:   Ejemplo de servidor multicliente con comunicación síncrona mediante sockets
//*****************************************************************

#include "Socket/Socket.hpp" //añadir
#include "MonitorConexiones.hpp"
#include <iostream>
#include <thread>
#include <cstdlib>
#include <regex>

using namespace std;

using namespace std;

const string PUBLICADOR = "PUBLICADOR";
const string MENS_SOLIC = "CONECTAR";
const string BUSCADOR = "BUSCADOR";
const string CONEXION_ESTABLECIDA = "CONEXION ESTABLECIDA";
const string DESCONEXION = "DESCONEXION";

const int TOTAL_CONEXIONES = 3;

const int tamanioBufferMensaje = 100;

void comprobarErrorRecepcion(Socket &soc, int client_fd, int rcv_bytes);

void comprobarErrorEnvio(Socket &soc, int client_fd, int send_bytes);


//-------------------------------------------------------------
void servCliente(Socket &soc, int client_fd, MonitorConexiones& monitorCone, const int i, const string ip1, const string ip2,
                 const string ip3, const int puerto) {
    // Buffer para recibir el mensaje
    int length = tamanioBufferMensaje;
    string buffer = "";
    bool out = false; // Inicialmente no salir del bucle
    /*******************************************************************           ENVIO CONECTAR */
    int rcv_bytes = soc.Recv(client_fd,buffer,length);
    comprobarErrorRecepcion(soc, client_fd, rcv_bytes);
    cout << "Mensaje del cliente " + to_string(i)  + " recibido : " + buffer + "\n";
    if(buffer != MENS_SOLIC){
        cout << "Mensaje incorrecto del cliente " + to_string(i)  + " recibido : " + buffer + "\n";
        //cerrar
        soc.Close(client_fd);
        pthread_exit(NULL);
    }
    // Enviamos la respuesta
    /*******************************************************************           ENVIO CONEXION ESTABLECIDA */
    int send_bytes = soc.Send(client_fd, CONEXION_ESTABLECIDA);
    comprobarErrorEnvio(soc, client_fd, send_bytes);
    //--Serivicio-----
    /*******************************************************************            ESPERANDO TIPO DE CLIENTE PUBLICADOR BUSCADOR */
    rcv_bytes = soc.Recv(client_fd,buffer,length);
    comprobarErrorRecepcion(soc, client_fd, rcv_bytes);
    cout << "Mensaje del cliente " + to_string(i)  + " recibido : " + buffer + "\n";
    if(buffer == BUSCADOR || buffer == PUBLICADOR){
        string datosConexion;
        if(buffer == BUSCADOR ){
            monitorCone.puedeConectar();
            cout << "paso\n";
            datosConexion = ip1+","+ip2+","+ip3+","+to_string(puerto);                          //llevar a metodo
        }else{
            datosConexion = ip1+","+ip2+","+ip3+","+to_string(puerto);
        }
        cout << datosConexion;
        /*******************************************************************            ENVIO DATOS */
        int send_bytes = soc.Send(client_fd, datosConexion);
        comprobarErrorEnvio(soc, client_fd, send_bytes);
    }else{  //si mensaje erroneo cerramos conexion
        cerr << "Mensaje incorrecto del cliente " + to_string(i)  + " recibido : " + buffer + "\n";
        //cerrar
        // soc.Close(client_fd);
        pthread_exit(NULL);
    }
    /*******************************************************************            Esperando mensaje DESCONEXION */
    //espero final de conexion
    rcv_bytes = soc.Recv(client_fd,buffer,length);
    monitorCone.liberarConexion();                                              //ELIMINAR EL METODO Y LIBERAR BIEN :>
    comprobarErrorRecepcion(soc, client_fd, rcv_bytes);
    //if(buffer == DESCONEXION)                                                 //mirar la elegancia de esto
}

void comprobarErrorEnvio(Socket &soc, int client_fd, int send_bytes) {
    if(send_bytes == -1) {
        cerr << "Error al enviar datos: " + string(strerror(errno)) + "\n";
        // Cerramos los sockets
        soc.Close(client_fd);
        pthread_exit(NULL);
    }
}

void comprobarErrorRecepcion(Socket &soc, int client_fd, int rcv_bytes) {
    if (rcv_bytes == -1) {
        cerr << "Error al recibir datos: " + string(strerror(errno)) + "\n";
        // Cerramos los sockets
        soc.Close(client_fd);
        pthread_exit(NULL);
    }
}


//-------------------------------------------------------------
int main(int numArg, char *args[]) {
    //args[1] == ip server 1
    //args[2] == ip server 2
    //args[3] == ip server 3
    //args[4] == ip server 4
    //args[5] == Puerto
    //if(numArg != 3){
    /*                                                                                      Realizar un mejor control de er
    if(numArg != 5){
        cerr << "Numero de parametros introducios incorrecto:" + string(strerror(errno)) + "\n";
        exit(1);
    }
     */
    string ip1;
    string ip2;
    string ip3;
    //total de conexiones;
    //const int N = atoi(args[2]);
    const int N = 100;                      //INTENTAR MIRAR PARA NO LIMITAR
    /*
    if(N == 0){
        cerr << "Error asignacion de nº puerto: " + string(strerror(errno)) + "\n";
        exit(1);
    }
    */
    MonitorConexiones monitorC(TOTAL_CONEXIONES);


    thread cliente[N];
    int client_fd[N];

    // Puerto donde escucha el proceso servidor
    //int SERVER_PORT = atoi(args[1]);
    int SERVER_PORT = 2021;
    /*
    if(N==0){
        cerr << "error asignacion de puerto: " + string(strerror(errno)) + "\n";
        exit(1);
    }
     */
    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);


    // Bind
    int socket_fd = chan.Bind(); //reserva del puerto en el SO
    if (socket_fd == -1) {
        cerr << "Error en el bind: " + string(strerror(errno)) + "\n";
        exit(1);
    }
    // Listen
    int max_connections = N;
    int error_code = chan.Listen(max_connections);  //escucho conexiones
    if (error_code == -1) {
        cerr << "Error en el listen: " + string(strerror(errno)) + "\n";
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }

    printf("Servidor arrancado esperando clientes\n");
    for (int i = 0; i < N; i++) {
        // Accept
        client_fd[i] = chan.Accept(); //bloquea hasta que se acepta conexion
        if (client_fd[i] == -1) {
            cerr << "Error en el accept: " + string(strerror(errno)) + "\n";
            // Cerramos el socket
            chan.Close(socket_fd);
            exit(1);
        }

        cliente[i] = thread(&servCliente, ref(chan), client_fd[i],ref(monitorC), i, ip1, ip2, ip3, SERVER_PORT); //lanzar servicio para cliente modelo RPC
        cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
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
//-------------------------------------------------------------
