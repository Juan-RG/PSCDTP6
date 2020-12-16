
#include <iostream>
#include <chrono>
#include <thread>
#include "Socket/Socket.hpp"

using namespace std;

const int MESSAGE_SIZE = 100; //mensajes de no más 4000 caracteres

void asignarCiudad(string *ciudad) {
    int valorMin = 1, valorMax = 10;
    //Calculamos un valor aleatorio entre el min y el max
    int nAleatorio = rand () % (valorMax - valorMin +1) + valorMin;
    switch (nAleatorio) {
        case 1:
            *ciudad = "Barcelona";
            break;
        case 2:
            *ciudad = "Madrid";
            break;
        case 3:
            *ciudad = "Valencia";
            break;
        case 4:
            *ciudad = "Sevilla";
            break;
        case 5:
            *ciudad = "La Coruña";
            break;
        case 6:
            *ciudad = "Tenerife";
            break;
        case 7:
            *ciudad = "Bilbao";
            break;
        case 8:
            *ciudad = "Badajoz";
            break;
        case 9:
            *ciudad = "Zaragoza";
            break;
        case 10:
            *ciudad = "Teruel";
            break;
    }
}


int main(int argc, char* argv[]) {
    // servidor
    const string CONEXION_ESTABLECIDA = "CONEXION ESTABLECIDA";
    // cliente
    string CONECTAR = "CONECTAR";
    const string PUBLICADOR = "PUBLICADOR";
    const string BUSCADOR = "BUSCADOR";
    const string DESCONEXION = "DESCONEXION";

    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS = "localhost";                                            //configruar para coger de arv
    int SERVER_PORT = 2021;                                             //configruar para coger de arv

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_ADDRESS, SERVER_PORT);

    // Conectamos con el servidor. Probamos varias conexiones
    const int MAX_ATTEMPS = 10;
    int count = 0;
    int socket_fd;
    do {
        // Conexión con el servidor
        socket_fd = chan.Connect();
        count++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_fd == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_fd == -1) {
        return socket_fd;
    }

    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer; 
    
    // solicitamos registro
    mensaje = CONECTAR;
    send_bytes = chan.Send(socket_fd, mensaje);
    if(send_bytes == -1) {
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }
    
    read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
    
    if (buffer.compare(CONEXION_ESTABLECIDA) != 0) {
        cout << "error: no se ha respondido bien al pedir alta registro" << endl;
        chan.Close(socket_fd);
        exit(1);
    }
    
    mensaje = PUBLICADOR;                                                     //CONFIGURAR PARA CADA CASO
    send_bytes = chan.Send(socket_fd, mensaje);
    if(send_bytes == -1) {
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }
    //datos de conexion
    read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
    cout << mensaje + "\n";

    //cortamos el string recibido con los datos ip1,ip2,ip3,puerto
    //hilo para utilizar el linda driver con la conexion o bucle
    //join del hilo para cuando finalice
    int totalTuplas;
    //extraeremos la tupla de RN['totalTuplas',?totalTuplas]
    string origen;
    asignarCiudad(&origen);

    string destino;
    asignarCiudad(&destino);

    while(origen == destino){
        asignarCiudad(&destino);
    }

    int precioMin = 1, precioMax = 10;
    //Calculamos un valor aleatorio entre el min y el max
    int precio = rand () % (precioMax - precioMin + 1) + precioMin;

    //Linda.pn[totalTuplas+1,origen,destino,precio] //habra que mirar el parametro id si controlarlo aqui o con la estructura de datos
    //si no eliminamos las ofertas no sera muy complicado comerse el coco y se elimiman ojo cuidado
    //linda.Pn['totalTuplas',totalTuplas+1]

    sleep(10);
    mensaje = DESCONEXION;
    send_bytes = chan.Send(socket_fd, mensaje);
    if(send_bytes == -1) {
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }
    // Cerramos el socket
    int error_code = chan.Close(socket_fd);
    if(error_code == -1) {
        cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }

    return error_code;
}
