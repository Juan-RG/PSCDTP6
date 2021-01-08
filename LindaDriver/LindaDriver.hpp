//--------------------------------------------------------------------------------------------------
// File:    LindaDriver.hpp
// Author:
// Date:    diciembre 2020
// Comms:   LindaDriver que permite a los clientes comunicarse con servidores Linda
//--------------------------------------------------------------------------------------------------

#ifndef LINDA_DRIVER
#define LINDA_DRIVER

#include <iostream>
#include <string>
#include "../Tupla/Tupla.hpp"

#include "../Socket/Socket.hpp" // añadido, para tener sockets en lindaDriver

using namespace std;

static const int MESSAGE_SIZE = 5000;               // Tamaño máximo de los mensajes
static const int NUM_MAX_INTENTOS = 10;             // Intentos de conexión

// Mensajes para el servidor de registro
static const string PETICION_DATOS = "PETICION_DATOS";

// Mensajes para servidores Linda
static const string MENSAJE_PN = "PN,";
static const string MENSAJE_RN = "RN,";
static const string MENSAJE_RDN = "RDN,";
static const string MENSAJE_RDN_2 = "RDN_2,";
static const string MENSAJE_RN_2 = "RN_2,";
static const string MENSAJE_DESCONEXION = "DESCONEXION";
static const string RESPUESTA_CONFIRMACION = "OK";

class LindaDriver {
public:
    // Constructor ----------------------------
    LindaDriver(string ipServerRegistro, int puertoServerRegistro);

    // Destructor -----------------------------
    ~LindaDriver();
    // Operadores -----------------------------

    // PostNote
    void PN(const Tupla t);

    //Pre:  "p" y "t" tienen el mismo tamaño
    //Post: Remove Note, siendo "p" el patrón y "t" la tupla
    void RN(const Tupla p, Tupla &t);

    //Pre:  "p1" y "t1" tienen el mismo tamaño
    //      "p2" y "t2" tienen el mismo tamaño
    //Post: Remove Notes, siendo "p1" y "p2" los patrones y "t1" y "t2" las tuplas
    void RN_2(const Tupla p1, const Tupla p2, Tupla &t1, Tupla &t2);

    //Pre:  "p" y "t" tienen el mismo tamaño
    //Post: ReaD Note, siendo "p" el patrón y "t" la tupla
    void RDN(const Tupla p, Tupla &t);

    //Pre:  "p1", "t1", "p2" y "t2" tienen el mismo tamaño
    //Post: ReaD Notes, siendo "p1" y "p2" los patrones y "t1" y "t2" las tuplas
    void RDN_2(const Tupla p1, const Tupla p2, Tupla &t1, Tupla &t2);

private:
    Socket chanServer1; // Socket para el servidor Linda de tuplas de tamaño 1 a 3
    Socket chanServer2; // Socket para el servidor Linda de tuplas de tamaño 4 a 5
    Socket chanServer3; // Socket para el servidor Linda de tuplas de tamaño 6
    int fdChanServer1;  // fd del Socket chanServer1
    int fdChanServer2;  // fd del Socket chanServer2
    int fdChanServer3;  // fd del Socket chanServer3

    // Direcciones y puertos de los tres servidores
    string ip_server_1;
    string puerto_server_1;
    string ip_server_2;
    string puerto_server_2;
    string ip_server_3;
    string puerto_server_3;

    // Funciones auxiliares
    void conectar(Socket &chan, int &socket_fd);

    void mandarMensaje(Socket &chan, const int &socket_fd, const string &mensaje);

    void recibirMensaje(Socket &chan, const int &socket_fd, string &buffer);
};

#endif
