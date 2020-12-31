//--------------------------------------------------------------------------------------------------
// File:    LindaDriver.cpp
// Author:
// Date:    diciembre 2020
// Comms:   LindaDriver que permite a los clientes comunicarse con servidores Linda
//--------------------------------------------------------------------------------------------------

#include <cassert>
#include <string>
#include "LindaDriver.hpp"

#include "Socket/Socket.hpp"
#include <iostream> // mensajes de error
#include <sstream>  // stringstream para reemplazar sscanf
#include <regex>    // regex para reconocer mensajes del servidor de registro

//constructor -----------------------------
// Pre: ipServerRegistro y puertoServerRegistro representan direcciones IP y puertos válidos, respectivamente.
// Post: Pide los datos de los servidores Linda al servidor de registro con la dirección y puerto especificados, y establece
//       una conexión con ellos.
LindaDriver::LindaDriver(string ipServerRegistro, int puertoServerRegistro) {
    Socket chan(ipServerRegistro, puertoServerRegistro);
    int socket_fd;

    // Conecta con el servidor de registro
    conectar(chan, socket_fd);

    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer; 

    // manda la petición de obtener datos de los servidores
    mensaje = PETICION_DATOS;
    mandarMensaje(chan, socket_fd, mensaje);

    // Recibe los datos de los servidores
    recibirMensaje(chan, socket_fd, buffer);

    // y cierra el socket
    chan.Close(socket_fd);
    std::cout << "socket con server de registro cerrado" << std::endl;

    // Parsea los datos recibidos del servidor de registro: ip1","ip2","ip3","puerto1","puerto2","puerto3
    stringstream s_stream(buffer); // Crea un stringstream a partir del buffer
    string substr;
    getline(s_stream, substr, ','); // Obtiene un string delimitado por una coma
    this->ip_server_1 = substr;
    getline(s_stream, substr, ',');
    this->ip_server_2 = substr;
    getline(s_stream, substr, ',');
    this->ip_server_3 = substr;
    getline(s_stream, substr, ',');
    this->puerto_server_1 = substr;
    getline(s_stream, substr, ',');
    this->puerto_server_2 = substr;
    getline(s_stream, substr, ',');
    this->puerto_server_3 = substr;
    
    // Cambia los detalles de los sockets de cada servidor con los datos recibidos
    chanServer1.CambiaDetallesServidor(ip_server_1, stoi(puerto_server_1));
    chanServer2.CambiaDetallesServidor(ip_server_2, stoi(puerto_server_2));
    chanServer3.CambiaDetallesServidor(ip_server_3, stoi(puerto_server_3));
    std::cout   << "Datos de los servidores:\n"
                << "Server 1(tam. 1 a 3): '"    << this->ip_server_1 << "':'"  << this->puerto_server_1 << "'\n"
                << "Server 2(tam. 4 a 5): '"    << this->ip_server_2 << "':'"  << this->puerto_server_2 << "'\n"
                << "Server 3(tam. 6): '"        << this->ip_server_3 << "':'"  << this->puerto_server_3 << "'" << std::endl;

    // Conexiones con los servidores Linda
    conectar(chanServer1, fdChanServer1);
    conectar(chanServer2, fdChanServer2);
    conectar(chanServer3, fdChanServer3);

    std::cout << "Conexión realizada con todos los servidores" << std::endl;
}

//destructor -----------------------------
// Pre:  --
// Post: Manda un mensaje de desconexión a cada uno de los servidores Linda, y cierra los sockets.
LindaDriver::~LindaDriver() {
    string mensaje = MENSAJE_DESCONEXION;
    // Manda un mensaje de desconexión a cada servidor
    mandarMensaje(chanServer1, fdChanServer1, mensaje);
    mandarMensaje(chanServer2, fdChanServer2, mensaje);
    mandarMensaje(chanServer3, fdChanServer3, mensaje);

    // Cierra los sockets
    chanServer1.Close(fdChanServer1);
    chanServer2.Close(fdChanServer2);
    chanServer3.Close(fdChanServer3);

    std::cout << "Conexión terminada con todos los servidores" << std::endl;
}

//operadores ----------------------------
//PostNote
// Pre: --
// Post: Manda un mensaje con la orden y la tupla a uno de los servidores Linda, según el tamaño de la tupla,
//       y espera a recibir una confirmación.
void LindaDriver::PN(const Tupla t) {
    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer;

    // mandará la orden PN concatenada a la tupla convertida a string
    mensaje = MENSAJE_PN + t.to_string();
    std::cout << "LindaDriver(PN): Mando..." << mensaje << ", tamaño de la tupla: " << t.size();

    if (t.size() < 4) { // tam. 1 a 3, va al servidor 1
        std::cout << " , se manda al servidor 1" << std::endl;

        mandarMensaje(chanServer1, fdChanServer1, mensaje);

        // Espera a recibir una confirmación
        std::cout << "LindaDriver(PN): espero a la confirmación del servidor 1" << std::endl;
        recibirMensaje(chanServer1, fdChanServer1, buffer);
        std::cout << "LindaDriver(PN): recibo " << buffer << ", acabo." << std::endl;
    }
    else if (t.size() < 6) { // tam. 4 a 5, va al servidor 2
        std::cout << " , se manda al servidor 2" << std::endl;

        mandarMensaje(chanServer2, fdChanServer2, mensaje);
        // Espera a recibir una confirmación
        std::cout << "LindaDriver(PN): espero a la confirmación del servidor 2" << std::endl;
        recibirMensaje(chanServer2, fdChanServer2, buffer);
        std::cout << "LindaDriver(PN): recibo " << buffer << ", acabo." << std::endl;
    }
    else { // tam. 6, va al servidor 3
        std::cout << "LindaDriver: mando al server 3" << std::endl;

        mandarMensaje(chanServer3, fdChanServer3, mensaje);
        // Espera a recibir una confirmación
        std::cout << "LindaDriver(PN): espero a la confirmación del servidor 3" << std::endl;
        recibirMensaje(chanServer3, fdChanServer3, buffer);
        std::cout << "LindaDriver(PN): recibo " << buffer << ", acabo." << std::endl;
    }

    if (buffer != RESPUESTA_CONFIRMACION) {
        std::cerr << "error al enviar la tupla " << t.to_string() << " en la operación PN" << std::endl;
    }
}
    

// Pre:  "p" y "t" tienen el mismo tamaño
// Post: Remove Note, siendo "p" el patrón y "t" la tupla
void LindaDriver::RN(const Tupla p, Tupla& t) {
    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer;

    // mandará la orden RN concatenada a la tupla convertida a string
    mensaje = MENSAJE_RN + p.to_string();
    std::cout << "LindaDriver(RN): Mando..." << mensaje << ", tamaño de la tupla: " << p.size();

    if (p.size() < 4) { // tam. 1 a 3, va al servidor 1
        std::cout << " , se manda al servidor 1" << std::endl;

        mandarMensaje(chanServer1, fdChanServer1, mensaje);
        // Espera a obtener la tupla resultado
        recibirMensaje(chanServer1, fdChanServer1, buffer);
    }
    else if (p.size() < 6) { // tam. 4 a 5, va al servidor 2
        std::cout << " , se manda al servidor 2" << std::endl;

        mandarMensaje(chanServer2, fdChanServer2, mensaje);
        // Espera a obtener la tupla resultado
        recibirMensaje(chanServer2, fdChanServer2, buffer);
    }
    else { // tam. 6, va al servidor 3
        std::cout << " , se manda al servidor 3" << std::endl;

        mandarMensaje(chanServer3, fdChanServer3, mensaje);
        // Espera a obtener la tupla resultado
        recibirMensaje(chanServer3, fdChanServer3, buffer);
    }

    std::cout << "LindaDriver(RN): recibo " << buffer << ", acabo." << std::endl;
    // escribe los datos de la tupla obtenida
    t.from_string(buffer);
}

//Pre:  "p1", "t1", "p2" y "t2" tienen el mismo tamaño
//Post: Remove Notes, siendo "p1" y "p2" los patrones y "t1" y "t2" las tuplas
void LindaDriver::RN_2(const Tupla p1, const Tupla p2, Tupla& t1, Tupla& t2) {
    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer;

    // mandará la orden RN_2 concatenada a las dos tuplas convertidas a string
    mensaje = MENSAJE_RN_2 + p1.to_string() + "," + p2.to_string();
    std::cout << "LindaDriver(RN_2): Mando..." << mensaje << ", tamaño de las tuplas: " << p1.size();
    if ((p1.size() == p2.size()) && (t1.size() == t2.size()) && (p1.size() == t2.size())) { // Si las tuplas son de diferente tamaño,
                                                                                            // no las vamos a enviar (requisito)
        if (p1.size() < 4) { // tam. 1 a 3, va al servidor 1
            std::cout << " , se manda al servidor 1" << std::endl;

            mandarMensaje(chanServer1, fdChanServer1, mensaje);
            // Espera a obtener la tupla resultado
            recibirMensaje(chanServer1, fdChanServer1, buffer);
        }
        else if (p1.size() < 6) { // tam. 4 a 5, va al servidor 2
            std::cout << " , se manda al servidor 2" << std::endl;

            mandarMensaje(chanServer2, fdChanServer2, mensaje);
            // Espera a obtener la tupla resultado
            recibirMensaje(chanServer2, fdChanServer2, buffer);
        }
        else { // tam. 6, va al servidor 3
            std::cout << " , se manda al servidor 3" << std::endl;

            mandarMensaje(chanServer3, fdChanServer3, mensaje);
            // Espera a obtener la tupla resultado
            recibirMensaje(chanServer3, fdChanServer3, buffer);
        }

        std::cout << "LindaDriver(RN_2): recibo " << buffer << ", acabo." << std::endl;
        // escribe los datos de las tuplas obtenidas
        stringstream s_stream(buffer);
        string substr;
        getline(s_stream, substr, ';');
        t1.from_string(substr);
        getline(s_stream, substr, ';');
        t2.from_string(substr);
    } else {
        std::cout << std::endl;
        std::cout << "LindaDriver(RN_2): Los tamaños de las tuplas no coinciden, acabo." << std::endl;
    }
}

//Pre:  "p" y "t" tienen el mismo tamaño
//Post: ReaD Note, siendo "p" el patrón y "t" la tupla
void LindaDriver::RDN(const Tupla p, Tupla& t) {
    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer;

    // mandará la orden RDN concatenada a la tupla convertida a string
    mensaje = MENSAJE_RDN + p.to_string();
    std::cout << "LindaDriver(RDN): Mando..." << mensaje << ", tamaño de la tupla: " << p.size();

    if (p.size() < 4) { // tam. 1 a 3, va al servidor 1
        std::cout << " , se manda al servidor 1" << std::endl;

        mandarMensaje(chanServer1, fdChanServer1, mensaje);
        // Espera a obtener la tupla resultado
        recibirMensaje(chanServer1, fdChanServer1, buffer);
    }
    else if (p.size() < 6) { // tam. 4 a 5, va al servidor 2
        std::cout << " , se manda al servidor 2" << std::endl;

        mandarMensaje(chanServer2, fdChanServer2, mensaje);
        // Espera a obtener la tupla resultado
        recibirMensaje(chanServer2, fdChanServer2, buffer);
    }
    else { // tam. 6, va al servidor 3
        std::cout << " , se manda al servidor 3" << std::endl;

        mandarMensaje(chanServer3, fdChanServer3, mensaje);
        // Espera a obtener la tupla resultado
        recibirMensaje(chanServer3, fdChanServer3, buffer);
    }

    std::cout << "LindaDriver(RDN): recibo " << buffer << ", acabo." << std::endl;
    // escribe los datos de la tupla obtenida
    t.from_string(buffer);
}

// Pre:  "p1", "t1", "p2" y "t2" tienen el mismo tamaño
// Post: ReaD Notes, siendo "p1" y "p2" los patrones y "t1" y "t2" las tuplas
void LindaDriver::RDN_2(const Tupla p1, const Tupla p2, Tupla& t1, Tupla& t2) { // Exactamente igual que RN_2 pero sin borrarlas al final
    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer;

    // mandará la orden RDN_2 concatenada a las dos tuplas convertidas a string
    mensaje = MENSAJE_RDN_2 + p1.to_string() + "," + p2.to_string();
    std::cout << "LindaDriver(RDN_2): Mando..." << mensaje << ", tamaño de las tuplas: " << p1.size();

    if ((p1.size() == p2.size()) && (t1.size() == t2.size()) && (p1.size() == t2.size())) { // Si las tuplas son de diferente tamaño,
                                                                                            // no las vamos a enviar (requisito)
        if (p1.size() < 4) { // tam. 1 a 3, va al servidor 1
            std::cout << " , se manda al servidor 1" << std::endl;

            mandarMensaje(chanServer1, fdChanServer1, mensaje);
            // Espera a obtener la tupla resultado
            recibirMensaje(chanServer1, fdChanServer1, buffer);
        }
        else if (p1.size() < 6) { // tam. 4 a 5, va al servidor 2
            std::cout << " , se manda al servidor 2" << std::endl;

            mandarMensaje(chanServer2, fdChanServer2, mensaje);
            // Espera a obtener la tupla resultado
            recibirMensaje(chanServer2, fdChanServer2, buffer);
        }
        else { // tam. 6, va al servidor 3
            std::cout << " , se manda al servidor 3" << std::endl;

            mandarMensaje(chanServer3, fdChanServer3, mensaje);
            // Espera a obtener la tupla resultado
            recibirMensaje(chanServer3, fdChanServer3, buffer);
        }

        std::cout << "LindaDriver(RDN_2): recibo " << buffer << ", acabo." << std::endl;
        // escribe los datos de las tuplas obtenidas
        stringstream s_stream(buffer);
        string substr;
        getline(s_stream, substr, ';');
        t1.from_string(substr);
        getline(s_stream, substr, ';');
        t2.from_string(substr);
    } else {
        std::cout << std::endl;
        std::cout << "LindaDriver(RDN_2): Los tamaños de las tuplas no coinciden, acabo." << std::endl;
    }
}

// Pre: --
// Post: Hace chan.Connect() sobre socket_fd, tratando los errores
void LindaDriver::conectar(Socket& chan, int& socket_fd) {
    int count = 0;
    do {
        // Conexión con el servidor
        socket_fd = chan.Connect();
        count++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd == -1) {
            sleep(1);
        }
    } while(socket_fd == -1 && count < NUM_MAX_INTENTOS);

    // Nos aseguramos de que se ha realizado la conexión
    if(socket_fd == -1) {
        std::cerr << "No se ha podido contactar con el servidor" << std::endl;
        std::terminate();
    }
}

// Pre: --
// Post: Manda "mensaje" por el socket con fd indicados, tratando los errores
void LindaDriver::mandarMensaje(Socket& chan, const int& socket_fd, const string& mensaje) {
    int send_bytes = chan.Send(socket_fd, mensaje);

    if(send_bytes == -1) {
        std::cerr << "Error al enviar datos: " << strerror(errno) << std::endl;
        string mensaje = MENSAJE_DESCONEXION;
        // Cierra los sockets
        chanServer1.Close(fdChanServer1);
        chanServer2.Close(fdChanServer2);
        chanServer3.Close(fdChanServer3);

        std::cout << "Conexión terminada con todos los servidores" << std::endl;
        std::terminate();
    }
}

// Pre: --
// Post: Recibe "buffer" por el socket con fd indicados, tratando los errores
void LindaDriver::recibirMensaje(Socket& chan, const int& socket_fd, string& buffer) {
    int read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);

    if(read_bytes == -1) {
        std::cerr << "Error al recibir datos " << strerror(errno) << std::endl;
        // Cierra los sockets
        chanServer1.Close(fdChanServer1);
        chanServer2.Close(fdChanServer2);
        chanServer3.Close(fdChanServer3);

        std::cout << "Conexión terminada con todos los servidores" << std::endl;
        std::terminate();
    }

}