//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: PSCD-Unizar
// Date:   Noviembre 2015
// Coms:   Ejemplo de servidor multicliente con comunicación síncrona mediante sockets
//*****************************************************************

#include "../Socket/Socket.hpp" //añadir
#include <iostream>
#include <thread>
#include <cstdlib>

using namespace std;

static const string MENSAJE = "PETICION_DATOS";
static const string MENSAJE_CERRAR = "CERRAR";
const int tamanioBufferMensaje = 100;

void comprobarErrorRecepcion(Socket &soc, int client_fd, int rcv_bytes);

void comprobarErrorEnvio(Socket &soc, int client_fd, int send_bytes);


//-------------------------------------------------------------
/**
 * metodo para atender las peticiones de los clientes con RCP
 * @param soc
 * @param client_fd
 * @param i
 * @param ip1
 * @param ip2
 * @param ip3
 * @param puerto1
 * @param puerto2
 * @param puerto3
 */
void servCliente(Socket &soc, int client_fd, const string ip1, const string ip2,
                 const string ip3, const int puerto1, const int puerto2, const int puerto3) {
    // Buffer para recibir el mensaje
    int length = tamanioBufferMensaje;
    string buffer = "";
    int rcv_bytes;
    /*******************************************************************           Compruebo que la conexion es correcta */
    rcv_bytes = soc.Recv(client_fd, buffer, length);
    comprobarErrorRecepcion(soc, client_fd, rcv_bytes);
    cout << "Mensaje del cliente recibido : " + buffer + "\n";

    if (buffer == MENSAJE) {
        string datosConexion = ip1 + "," + ip2 + "," + ip3 + "," + to_string(puerto1) + "," + to_string(puerto2) + "," +
                               to_string(puerto3);
        /*******************************************************************            ENVIO DATOS */
        int send_bytes = soc.Send(client_fd, datosConexion);
        comprobarErrorEnvio(soc, client_fd, send_bytes);
    } else if (buffer == MENSAJE_CERRAR) {
        cout << "Recibido mensaje de cierre del servidor, terminando la ejecución..." << endl;
        exit(0);
    } else {
        //si mensaje erroneo cerramos conexion
        cerr << "Mensaje incorrecto del cliente recibido : " + buffer + "\n";
        //cerrar
        soc.Close(client_fd);
        pthread_exit(NULL);
    }
}

/**
 * metodo para detectar el error en el envio de mensajes
 * @param soc
 * @param client_fd
 * @param send_bytes
 */
void comprobarErrorEnvio(Socket &soc, int client_fd, int send_bytes) {
    if (send_bytes == -1) {
        cerr << "Error al enviar datos: " + string(strerror(errno)) + "\n";
        // Cerramos los sockets
        soc.Close(client_fd);
        pthread_exit(NULL);
    }
}

/**
 * Metodo para comprobar el error en la recepcion de los mensajes
 * @param soc
 * @param client_fd
 * @param rcv_bytes
 */
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
    //Ejecutar con ./MainServidor IP1 IP2 IP3 PUERTO1 PUERTO2 PUERTO3 puertoServerDespliegue


    //Realizar un mejor control de er
    if (numArg != 8) {
        cerr << "Numero de parametros introducios incorrecto:" + string(strerror(errno)) + "\n";
        exit(1);
    }


    string ip1 = args[1];
    string ip2 = args[2];
    string ip3 = args[3];
    int puerto1 = stoi(args[4]);
    int puerto2 = stoi(args[5]);
    int puerto3 = stoi(args[6]);

    //string ip1="localhost";
    //string ip2="localhost";
    //string ip3="localhost";
    //int puerto1 = 2023;
    //int puerto2 = 2024;
    //int puerto3 = 2025;

    //total de conexiones;
    const int N = 5000;                      //INTENTAR MIRAR PARA NO LIMITAR

    thread cliente[N];
    int client_fd[N];

    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = atoi(args[7]);
    //int SERVER_PORT = 2022;

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

        cliente[i] = thread(&servCliente, ref(chan), client_fd[i], ip1, ip2, ip3, puerto1, puerto2,
                            puerto3); //lanzar servicio para cliente modelo RPC
        //cout << "Nuevo cliente aceptado\n";
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
