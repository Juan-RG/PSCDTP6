//*****************************************************************
// File:   mainLinda.hpp
// Author: PSCD-Unizar
// Date:   noviembre 2020
// Coms:   
//*****************************************************************

#ifndef LINDA_DRIVER
#define LINDA_DRIVER
#include <iostream>
#include <string>
#include "Tupla.hpp"

#include "Socket/Socket.hpp" // añadido, para tener sockets en lindaDriver

using namespace std;

static const int MESSAGE_SIZE = 5000; // tamaño máximo de los mensajes 
static const int NUM_MAX_INTENTOS = 50;
//static const string SOLICITUD_IP_SERVER_1 = "IP_1";
//static const string SOLICITUD_PUERTO_SERVER_1 = "P_1";
//static const string SOLICITUD_IP_SERVER_2 = "IP_2";
//static const string SOLICITUD_PUERTO_SERVER_2 = "P_2";
//static const string SOLICITUD_IP_SERVER_3 = "IP_3";
//static const string SOLICITUD_PUERTO_SERVER_3 = "P_3";
static const string RESPUESTA_CONFIRMACION = "OK";
static const string MENSAJE_PN = "PN,";
static const string MENSAJE_RN = "RN,";
//static const string MENSAJE_RN_2 = "RN_2,";
static const string MENSAJE_RDN = "RDN,";
//static const string MENSAJE_RDN_2 = "RDN_2,";
static const string MENSAJE_DESCONEXION = "DESCONEXION";

class LindaDriver {
public:
	//constructores -----------------------------
/*<<<<<<< HEAD
	LindaDriver(string ipServerRegistro, int puertoServerRegistro,string tipo);
=======

>>>>>>> 63b10faad2fba1977004c0adf34ceed29a0fbc27
 */
    LindaDriver(string ipServerRegistro, int puertoServerRegistro, string tipoCliente);
	//destructor -----------------------------
	~LindaDriver();
	//operadores -----------------------------

	//PostNote
	void PN(const Tupla t);

	//Pre:  "p" y "t" tienen el mismo tamaño
	//Post: Remove Note, siendo "p" el patrón y "t" la tupla
	void RN(const Tupla p, Tupla& t);

	//Pre:  "p1" y "t1" tienen el mismo tamaño
	//      "p2" y "t2" tienen el mismo tamaño
	//Post: Remove Notes, siendo "p1" y "p2" los patrones y "t1" y "t2" las tuplas
	void RN_2(const Tupla p1, const Tupla p2, Tupla& t1, Tupla& t2);

	//Pre:  "p" y "t" tienen el mismo tamaño
	//Post: ReaD Note, siendo "p" el patrón y "t" la tupla
	void RDN(const Tupla p, Tupla& t);

	//Pre:  "p1" y "t1" tienen el mismo tamaño
	//      "p2" y "t2" tienen el mismo tamaño
	//Post: ReaD Notes, siendo "p1" y "p2" los patrones y "t1" y "t2" las tuplas
	void RDN_2(const Tupla p1, const Tupla p2, Tupla& t1, Tupla& t2);

private:
	// se podrá? o habrá que crear uno temporalmente en cada operación? <-- ver si se puede, de momento se crean en cada operación
	// Socket chanServer1; // Servidor de tuplas de tamaño 1 a 3
	// Socket chanServer2; // Servidor de tuplas de tamaño 4 a 5
	// Socket chanServer3; // Servidor de tuplas de tamaño 6
	
	string ip_server_1;
	//string puerto_server_1;
	string ip_server_2;
	//string puerto_server_2;
	string ip_server_3;
	//string puerto_server_3;
	string puerto;
	//string tipo_cliente; // PUBLICADOR | BUSCADOR, de momento no se lo guarda
};
#endif
