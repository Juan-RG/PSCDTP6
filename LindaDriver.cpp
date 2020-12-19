//*****************************************************************
// File:   LindaDriver.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2020
// Coms:   Especificación de la clase
//*****************************************************************

#include <cassert>
#include <string>
#include "LindaDriver.hpp"

#include "Socket/Socket.hpp"
#include <iostream> // mensajes de error
#include <sstream>  // stringstream para reemplazar sscanf
#include <regex>    // regex para reconocer mensajes del servidor de registro

void conectar(Socket& chan, int& socket_fd);

//constructores -----------------------------
LindaDriver::LindaDriver(string ipServerRegistro, int puertoServerRegistro, string tipoCliente) {
    Socket chan(ipServerRegistro, puertoServerRegistro);
    printf("paso\n");
    int socket_fd;
    conectar(chan, socket_fd);
    // Conectamos con el servidor de registro. Probamos varias conexiones.      // <------ Pasado a función conectar()
    // const int MAX_ATTEMPS = 10;
    // int count = 0;
    // int socket_fd;
    // do {
        // Conexión con el servidor
    //     socket_fd = chan.Connect();
    //     count++;

        // Si error --> esperamos 1 segundo para reconectar
    //     if(socket_fd == -1) {
    //         sleep(1);
    //     }
    // } while(socket_fd == -1 && count < MAX_ATTEMPS);
    // Nos aseguramos de que se ha realizado la conexión
    // if(socket_fd == -1) {
    //     std::cerr << "No se ha podido conectar con el servidor en " 
    //               << ipServerRegistro << ":" << puertoServerRegistro 
    //               << std::endl;    
    // }

    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer; 

    // manda qué tipo de cliente es
    mensaje = tipoCliente; // BUSCADOR | PUBLICADOR
    send_bytes = chan.Send(socket_fd, mensaje);
    if(send_bytes == -1) {
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }

    // Recibe los datos de los servidores (ip1,ip2,ip3,puerto)
    read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
    cout << buffer + "\n";
    cout<< "hola \n";
    // Se desconecta del servidor
    mensaje = MENSAJE_DESCONEXION;
    send_bytes = chan.Send(socket_fd, mensaje);
    if(send_bytes == -1) {
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }
    chan.Close(socket_fd); // y cierra el Socket

    // seguramente explote:
    //sscanf(buffer.c_str(), "%s,%s,%s,%s", &this->ip_server_1.c_str(), &this->ip_server_2.c_str(), &this->ip_server_3.c_str(), &this->puerto.c_str());

    // TODO: probar si acaso funciona...
    std::istringstream stream_buffer(buffer);
    //stream_buffer >> this->ip_server_1 >> "," >> this->ip_server_2 >> "," >> this->ip_server_3 >> "," >> this->puerto;
    stream_buffer >> this->ip_server_1 >> this->ip_server_2 >> this->ip_server_3 >> this->puerto;
    //some_stream >> num1 >> num2 >> type3;
    // 3 direcciones IP (1,2,3) + 3 puertos (1,2,3)
    regex er("([0-9]+.[0-9]+.[0-9]+.[0-9]+),([0-9]+.[0-9]+.[0-9]+.[0-9]+),([0-9]+.[0-9]+.[0-9]+.[0-9]+),([0-9]+.[0-9]+.[0-9]+.[0-9]+),[0-9]+,[0-9]+,[0-9]+");
    smatch sm; //almacenará los resultado del "matching"

    //busca la "er" en "s", almacenando los resultados en "sm"
    //sm[0] es todo el contenido en que se ha hecho el "matching"
    //sm[1] la parte correspondiente a la primera expresión regular entre paréntesis. En este caso [0-9]+
    //sm[2] la parte correspondiente a la segunda expresión regular entre paréntesis.
    //      También [0-9]+ en este caso
    //...
    regex_search(buffer, sm, er);
    //escribirlar como strings
    // for (int i=0; i<sm.size(); i++) {
    // 	cout << "sm[" << i << "]: " << sm[i].str() << endl;
    // }
    //también se pueden convertir en números con "stoi", claro

    this->ip_server_1 = sm[0];
    this->ip_server_2 = sm[1];
    this->ip_server_3 = sm[2];
    this->puerto_server_1 = sm[3];
    this->puerto_server_2 = sm[4];
    this->puerto_server_3 = sm[5];
    //n1 = stoi(sm[1].str());
    //n2 = stoi(sm[2].str());
    //n3 = stoi(sm[3].str());
    //n4 = stoi(sm[4].str());

    // solicitamos los datos del servidor 1
//    mensaje = SOLICITUD_IP_SERVER_1;
//    send_bytes = chan.Send(socket_fd, mensaje);
//    if(send_bytes == -1) {
//        cerr << "Error al enviar datos: " << strerror(errno) << endl;
//        // Cerramos el socket
//        chan.Close(socket_fd);
//        exit(1);
//    }
//    read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
//    this->ip_server_1 = buffer;
//    mensaje = SOLICITUD_puerto;
//    send_bytes = chan.Send(socket_fd, mensaje);
//    if(send_bytes == -1) {
//        cerr << "Error al enviar datos: " << strerror(errno) << endl;
//        // Cerramos el socket
//        chan.Close(socket_fd);
//        exit(1);
//    }
//    read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
//    this->puerto = buffer;
//
//    // solicitamos los datos del servidor 2
//    mensaje = SOLICITUD_IP_SERVER_2;
//    send_bytes = chan.Send(socket_fd, mensaje);
//    if(send_bytes == -1) {
//        cerr << "Error al enviar datos: " << strerror(errno) << endl;
//        // Cerramos el socket
//        chan.Close(socket_fd);
//        exit(1);
//    }
//    read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
//    this->ip_server_2 = buffer;
//    mensaje = SOLICITUD_puerto;
//    send_bytes = chan.Send(socket_fd, mensaje);
//    if(send_bytes == -1) {
//        cerr << "Error al enviar datos: " << strerror(errno) << endl;
//        // Cerramos el socket
//        chan.Close(socket_fd);
//        exit(1);
//    }
//    read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
//    this->puerto = buffer;
//
//    // solicitamos los datos del servidor 3
//    mensaje = SOLICITUD_IP_SERVER_3;
//    send_bytes = chan.Send(socket_fd, mensaje);
//    if(send_bytes == -1) {
//        cerr << "Error al enviar datos: " << strerror(errno) << endl;
//        // Cerramos el socket
//        chan.Close(socket_fd);
//        exit(1);
//    }
//    read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
//    this->ip_server_3 = buffer;
//    mensaje = SOLICITUD_puerto;
//    send_bytes = chan.Send(socket_fd, mensaje);
//    if(send_bytes == -1) {
//        cerr << "Error al enviar datos: " << strerror(errno) << endl;
//        // Cerramos el socket
//        chan.Close(socket_fd);
//        exit(1);
//    }
//    read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
//    this->puerto = buffer;
    
    // creo que será así? Resulta que no...
    //this->chanServer1 = new Socket(ip_server_1, stoi(puerto));
    //this->chanServer2 = new Socket(ip_server_2, stoi(puerto));
    //this->chanServer3 = new Socket(ip_server_3, stoi(puerto));
    
    std::cout   << "Datos de los servidores:\n" 
                << "Server 1(tam. 1 a 3): '"    << this->ip_server_1 << "':'"  << this->puerto_server_1 << "'\n"
                << "Server 2(tam. 4 a 5): '"    << this->ip_server_2 << "':'"  << this->puerto_server_2 << "'\n"
                << "Server 3(tam. 6): '"        << this->ip_server_3 << "':'"  << this->puerto_server_3 << std::endl;
}

//destructor -----------------------------
LindaDriver::~LindaDriver(){

}

//operadores ----------------------------
//PostNote
void LindaDriver::PN(const Tupla t) {
//    string mensaje;
//    int read_bytes;   // num de bytes recibidos en un mensaje
//    int send_bytes;   // num de bytes enviados en un mensaje
//    int socket_fd;
//    string buffer;
//    Socket chanServer1(this->ip_server_1, stoi(this->puerto));
//    Socket chanServer2(this->ip_server_2, stoi(this->puerto));
//    Socket chanServer3(this->ip_server_3, stoi(this->puerto));
//    mensaje = MENSAJE_PN + t.to_string();
//
//    if (t.size() < 4) { // tam. 1 a 3, va al servidor 1
//        conectar(chanServer1, socket_fd);
//
//        // manda el código de operación junto a la tupla convertida a string
//        send_bytes = chanServer1.Send(socket_fd, mensaje);
//        if(send_bytes == -1) {
//            cerr << "Error al enviar datos: " << strerror(errno) << endl;
//            // Cerramos el socket
//            chanServer1.Close(socket_fd);
//            exit(1);
//        }
//
//        // Espera a recibir una confirmación
//        read_bytes = chanServer1.Recv(socket_fd, buffer, MESSAGE_SIZE);
//        chanServer1.Close(socket_fd); // y cierra el socket
//    } else if (t.size() < 6) { // tam. 4 a 5, va al servidor 2
//        conectar(chanServer2, socket_fd);
//
//        // manda el código de operación junto a la tupla convertida a string
//        send_bytes = chanServer2.Send(socket_fd, mensaje);
//        if(send_bytes == -1) {
//            cerr << "Error al enviar datos: " << strerror(errno) << endl;
//            // Cerramos el socket
//            chanServer2.Close(socket_fd);
//            exit(1);
//        }
//
//        // Espera a recibir una confirmación
//        read_bytes = chanServer2.Recv(socket_fd, buffer, MESSAGE_SIZE);
//        chanServer2.Close(socket_fd); // y cierra el socket
//    } else { // tam. 6, va al servidor 3
//        conectar(chanServer3, socket_fd);
//
//        // manda el código de operación junto a la tupla convertida a string
//        send_bytes = chanServer3.Send(socket_fd, mensaje);
//        if(send_bytes == -1) {
//            cerr << "Error al enviar datos: " << strerror(errno) << endl;
//            // Cerramos el socket
//            chanServer3.Close(socket_fd);
//            exit(1);
//        }
//
//        // Espera a recibir una confirmación
//        read_bytes = chanServer3.Recv(socket_fd, buffer, MESSAGE_SIZE);
//        chanServer3.Close(socket_fd); // y cierra el socket
//    }
//
//    if (buffer != RESPUESTA_CONFIRMACION) {
//        std::cerr << "error al enviar la tupla " << t.to_string()
//                  << " en la operación PN" << std::endl;
//    }
}
    

//Pre:  "p" y "t" tienen el mismo tamaño
//Post: Remove Note, siendo "p" el patrón y "t" la tupla
void LindaDriver::RN(const Tupla p, Tupla& t) {
//    string mensaje;
//    int read_bytes;   // num de bytes recibidos en un mensaje
//    int send_bytes;   // num de bytes enviados en un mensaje
//    int socket_fd;
//    string buffer;
//    Socket chanServer1(this->ip_server_1, stoi(this->puerto));
//    Socket chanServer2(this->ip_server_2, stoi(this->puerto));
//    Socket chanServer3(this->ip_server_3, stoi(this->puerto));
//    mensaje = MENSAJE_RN + p.to_string();
//
//    if (p.size() < 4) { // tam. 1 a 3, va al servidor 1
//        conectar(chanServer1, socket_fd);
//
//        // manda el código de operación junto a la tupla convertida a string
//        send_bytes = chanServer1.Send(socket_fd, mensaje);
//        if(send_bytes == -1) {
//            cerr << "Error al enviar datos: " << strerror(errno) << endl;
//            // Cerramos el socket
//            chanServer1.Close(socket_fd);
//            exit(1);
//        }
//
//        // Espera a obtener la tupla resultado
//        read_bytes = chanServer1.Recv(socket_fd, buffer, MESSAGE_SIZE);
//        chanServer1.Close(socket_fd); // y cierra el socket
//    } else if (p.size() < 6) { // tam. 4 a 5, va al servidor 2
//        conectar(chanServer2, socket_fd);
//
//        // manda el código de operación junto a la tupla convertida a string
//        send_bytes = chanServer2.Send(socket_fd, mensaje);
//        if(send_bytes == -1) {
//            cerr << "Error al enviar datos: " << strerror(errno) << endl;
//            // Cerramos el socket
//            chanServer2.Close(socket_fd);
//            exit(1);
//        }
//
//        // Espera a obtener la tupla resultado
//        read_bytes = chanServer2.Recv(socket_fd, buffer, MESSAGE_SIZE);
//        chanServer2.Close(socket_fd); // y cierra el socket
//    } else { // tam. 6, va al servidor 3
//        conectar(chanServer3, socket_fd);
//
//        // manda el código de operación junto a la tupla convertida a string
//        send_bytes = chanServer3.Send(socket_fd, mensaje);
//        if(send_bytes == -1) {
//            cerr << "Error al enviar datos: " << strerror(errno) << endl;
//            // Cerramos el socket
//            chanServer3.Close(socket_fd);
//            exit(1);
//        }
//
//        // Espera a obtener la tupla resultado
//        read_bytes = chanServer3.Recv(socket_fd, buffer, MESSAGE_SIZE);
//        chanServer3.Close(socket_fd); // y cierra el socket
//    }
//
//    // escribe los datos de la tupla obtenida
//    t.from_string(buffer);
}

//Pre:  "p1" y "t1" tienen el mismo tamaño
//      "p2" y "t2" tienen el mismo tamaño
//Post: Remove Notes, siendo "p1" y "p2" los patrones y "t1" y "t2" las tuplas
void LindaDriver::RN_2(const Tupla p1, const Tupla p2, Tupla& t1, Tupla& t2) {
//    // Tuplas a sobreescribir con el resultado, de momento son copias de p1 y p2
//    Tupla tuplaTemp1(p1);
//    Tupla tuplaTemp2(p2);
//    bool parar = false;
//    int lengthArrayComodines;
//
//    struct comodines {
//        string valor;
//        int numIndices;
//        int indices[6]; // tamaño máximo de una tupla
//    };
//
//    struct comodinesComunes {
//        string valor;
//        int numIndicesp1, numIndicesp2;
//        // acotados a los tamaños máximos de una tupla
//        int indicesp1[6];
//        int indicesp2[6];
//    };
//
//    // acotados a los tamaños de cada una de las tuplas
//    comodines arrayComodinesp1[p1.size()];
//    comodines arrayComodinesp2[p2.size()];
//    comodinesComunes arrayComodinesComunes[p1.size()]; // max: el tamaño
//                                                              // de la menor
//                                                              // o de cualquiera
//                                                              // de las dos
//
//
//    // Se guardan los comodines de la tupla p1 junto a los índices donde aparecen
//    int numComodinesp1 = 0;
//    bool estaba = false;
//    for(int i = 0; i < p1.size(); i++) {
//        if( (p1.get(i))[0] == '?') { // TODO: Sustituir por una regex
//	        // comprueba si ya estaba el comodín en la lista
//	        for (int j = 0; j < p1.size(); j++) {
//	            // si está, se añade otro índice
//	            if (arrayComodinesp1[j].valor == p1.get(i)) {
//	                arrayComodinesp1[j].indices[arrayComodinesp1[j].numIndices] = i;
//                    arrayComodinesp1[j].numIndices++;
//                    estaba = true;
//	            }
//	        }
//	        // si no estaba, se añade otra entrada en la lista
//	        if (!estaba) {
//	            arrayComodinesp1[numComodinesp1].valor == p1.get(i);
//	            arrayComodinesp1[numComodinesp1].indices[arrayComodinesp1[numComodinesp1].numIndices] == i;
//	            arrayComodinesp1[numComodinesp1].numIndices++;
//	            numComodinesp1++;
//	        } else {
//	            // se resetea estaba para la siguiente iteración
//	            estaba = false;
//	        }
//        }
//    }
//
//
//    // Se guardan los comodines de la tupla p2 junto a los índices donde aparecen
//    int numComodinesp2 = 0;
//    estaba = false;
//    for(int i = 0; i < p2.size(); i++) {
//        if( (p2.get(i))[0] == '?') { // TODO: Sustituir por una regex
//	        // comprueba si ya estaba el comodín en la lista
//	        for (int j = 0; j < p2.size(); j++) {
//	            // si está, se añade otro índice
//	            if (arrayComodinesp2[j].valor == p2.get(i)) {
//	                arrayComodinesp2[j].indices[arrayComodinesp2[j].numIndices] = i;
//                    arrayComodinesp2[j].numIndices++;
//                    estaba = true;
//	            }
//	        }
//	        // si no estaba, se añade otra entrada en la lista
//	        if (!estaba) {
//	            arrayComodinesp2[numComodinesp2].valor == p2.get(i);
//	            arrayComodinesp2[numComodinesp2].indices[arrayComodinesp2[numComodinesp2].numIndices] == i;
//	            arrayComodinesp2[numComodinesp2].numIndices++;
//	            numComodinesp2++;
//	        } else {
//	            // se resetea estaba para la siguiente iteración
//	            estaba = false;
//	        }
//        }
//    }
//
//    // Se juntan los comodines de ambas en el vector de estructuras de
//    // comodines comunes
//    int numComodinesComunes;
//    for (int i = 0; i < p1.size(); i++) {
//        for (int j = 0; j < p2.size(); j++) {
//            // si dos coinciden , se añade un nuevo comodín común
//            // se sabe que en ninguno de los dos hay comodines repetidos
//            if (arrayComodinesp1[i].valor == arrayComodinesp2[j].valor) {
//                arrayComodinesComunes[numComodinesComunes].valor = arrayComodinesp1[i].valor;
//                arrayComodinesComunes[numComodinesComunes].numIndicesp1 = arrayComodinesp1[i].numIndices;
//                // copia el vector de indices de comodinesp1 en el de comunes para p1
//                std::copy(arrayComodinesp1[i].indices, arrayComodinesp1[i].indices + 6, arrayComodinesComunes[numComodinesComunes].indicesp1);
//
//                // hace lo mismo para p2
//                arrayComodinesComunes[numComodinesComunes].numIndicesp2 = arrayComodinesp2[i].numIndices;
//                // copia el vector de indices de comodinesp2 en el de comunes para p2
//                std::copy(arrayComodinesp2[i].indices, arrayComodinesp2[i].indices + 6, arrayComodinesComunes[numComodinesComunes].indicesp2);
//
//                numComodinesComunes++;
//            }
//        }
//    }
//
//    bool sigueLocal = false;
//    while (!parar) {
//        RDN(p1, tuplaTemp1);
//
//        for (int i = 0; i < NUM_MAX_INTENTOS; i++) {
//            RDN(p2, tuplaTemp2);
//
//            // se comprueba si en los índices con los mismos comodines (si hay),
//            // las dos tuplas obtenidas tienen los mismos valores
//
//            if (numComodinesComunes == 0) { // si no hay índices comunes
//                                            // ya se puede parar
//                parar = true;
//            } else {
//                for (int i = 0; (i < numComodinesComunes) && (sigueLocal); i++) {
//                    for (int j = 0; (j < arrayComodinesComunes[i].numIndicesp1) && (sigueLocal); j++) {
//                        for (int k = 0; (k < arrayComodinesComunes[i].numIndicesp2) && (sigueLocal); k++) {
//                            // sigueLocal será false si se encuentra algún par de posiciones con comodines comunes entre
//                            // las dos tuplas que sean diferentes en contenido
//                            sigueLocal = (tuplaTemp1.get(arrayComodinesComunes[i].indicesp1[j]) != tuplaTemp2.get(arrayComodinesComunes[i].indicesp2[k]));
//                        }
//                    }
//                }
//
//                if (sigueLocal) { // si todos los pares de posiciones son iguales
//                                  // las hemos encontrado
//                    parar = true;
//                }
//            }
//        }
//    }
//
//    // devolvemos las tuplas, mandando borrarlas antes
//
//    // posible bloqueo si alguien se colara entre medio y las borrara?
//    // ...podría pasar igualmente si dos hicieran un RN sobre la misma, se
//    // bloquearía uno de ellos también
//    RN(tuplaTemp1);
//    RN(tuplaTemp2);
//
//    t1 = tuplaTemp1;
//    t2 = tuplaTemp2;
}

//Pre:  "p" y "t" tienen el mismo tamaño
//Post: ReaD Note, siendo "p" el patrón y "t" la tupla
void LindaDriver::RDN(const Tupla p, Tupla& t) {
//    string mensaje;
//    int read_bytes;   // num de bytes recibidos en un mensaje
//    int send_bytes;   // num de bytes enviados en un mensaje
//    int socket_fd;
//    string buffer;
//    Socket chanServer1(this->ip_server_1, stoi(this->puerto));
//    Socket chanServer2(this->ip_server_2, stoi(this->puerto));
//    Socket chanServer3(this->ip_server_3, stoi(this->puerto));
//    mensaje = MENSAJE_RDN + p.to_string();
//
//    if (p.size() < 4) { // tam. 1 a 3, va al servidor 1
//        conectar(chanServer1, socket_fd);
//
//        // manda el código de operación junto a la tupla convertida a string
//        send_bytes = chanServer1.Send(socket_fd, mensaje);
//        if(send_bytes == -1) {
//            cerr << "Error al enviar datos: " << strerror(errno) << endl;
//            // Cerramos el socket
//            chanServer1.Close(socket_fd);
//            exit(1);
//        }
//
//        // Espera a obtener la tupla resultado
//        read_bytes = chanServer1.Recv(socket_fd, buffer, MESSAGE_SIZE);
//        chanServer1.Close(socket_fd); // y cierra el socket
//    } else if (p.size() < 6) { // tam. 4 a 5, va al servidor 2
//        conectar(chanServer2, socket_fd);
//
//        // manda el código de operación junto a la tupla convertida a string
//        send_bytes = chanServer2.Send(socket_fd, mensaje);
//        if(send_bytes == -1) {
//            cerr << "Error al enviar datos: " << strerror(errno) << endl;
//            // Cerramos el socket
//            chanServer2.Close(socket_fd);
//            exit(1);
//        }
//
//        // Espera a obtener la tupla resultado
//        read_bytes = chanServer2.Recv(socket_fd, buffer, MESSAGE_SIZE);
//        chanServer2.Close(socket_fd); // y cierra el socket
//    } else { // tam. 6, va al servidor 3
//        conectar(chanServer3, socket_fd);
//
//        // manda el código de operación junto a la tupla convertida a string
//        send_bytes = chanServer3.Send(socket_fd, mensaje);
//        if(send_bytes == -1) {
//            cerr << "Error al enviar datos: " << strerror(errno) << endl;
//            // Cerramos el socket
//            chanServer3.Close(socket_fd);
//            exit(1);
//        }
//
//        // Espera a obtener la tupla resultado
//        read_bytes = chanServer3.Recv(socket_fd, buffer, MESSAGE_SIZE);
//        chanServer3.Close(socket_fd); // y cierra el socket
//    }
//
//    // escribe los datos de la tupla obtenida
//    t.from_string(buffer);
}

//Pre:  "p1" y "t1" tienen el mismo tamaño
//      "p2" y "t2" tienen el mismo tamaño
//Post: ReaD Notes, siendo "p1" y "p2" los patrones y "t1" y "t2" las tuplas
void LindaDriver::RDN_2(const Tupla p1, const Tupla p2, Tupla& t1, Tupla& t2) { // Exactamente igual que RN_2 pero sin borrarlas al final
//    // Tuplas a sobreescribir con el resultado, de momento son copias de p1 y p2
//    Tupla tuplaTemp1(p1);
//    Tupla tuplaTemp2(p2);
//    bool parar = false;
//    int lengthArrayComodines;
//
//    struct comodines {
//        string valor;
//        int numIndices;
//        int indices[6]; // tamaño máximo de una tupla
//    };
//
//    struct comodinesComunes {
//        string valor;
//        int numIndicesp1, numIndicesp2;
//        // acotados a los tamaños máximos de una tupla
//        int indicesp1[6];
//        int indicesp2[6];
//    };
//
//    // acotados a los tamaños de cada una de las tuplas
//    comodines arrayComodinesp1[p1.size()];
//    comodines arrayComodinesp2[p2.size()];
//    comodinesComunes arrayComodinesComunes[p1.size()]; // max: el tamaño
//                                                              // de la menor
//                                                              // o de cualquiera
//                                                              // de las dos
//
//
//    // Se guardan los comodines de la tupla p1 junto a los índices donde aparecen
//    int numComodinesp1 = 0;
//    bool estaba = false;
//    for(int i = 0; i < p1.size(); i++) {
//        if( (p1.get(i))[0] == '?') { // TODO: Sustituir por una regex
//	        // comprueba si ya estaba el comodín en la lista
//	        for (int j = 0; j < p1.size(); j++) {
//	            // si está, se añade otro índice
//	            if (arrayComodinesp1[j].valor == p1.get(i)) {
//	                arrayComodinesp1[j].indices[arrayComodinesp1[j].numIndices] = i;
//                    arrayComodinesp1[j].numIndices++;
//                    estaba = true;
//	            }
//	        }
//	        // si no estaba, se añade otra entrada en la lista
//	        if (!estaba) {
//	            arrayComodinesp1[numComodinesp1].valor == p1.get(i);
//	            arrayComodinesp1[numComodinesp1].indices[arrayComodinesp1[numComodinesp1].numIndices] == i;
//	            arrayComodinesp1[numComodinesp1].numIndices++;
//	            numComodinesp1++;
//	        } else {
//	            // se resetea estaba para la siguiente iteración
//	            estaba = false;
//	        }
//        }
//    }
//
//
//    // Se guardan los comodines de la tupla p2 junto a los índices donde aparecen
//    int numComodinesp2 = 0;
//    estaba = false;
//    for(int i = 0; i < p2.size(); i++) {
//        if( (p2.get(i))[0] == '?') { // TODO: Sustituir por una regex
//	        // comprueba si ya estaba el comodín en la lista
//	        for (int j = 0; j < p2.size(); j++) {
//	            // si está, se añade otro índice
//	            if (arrayComodinesp2[j].valor == p2.get(i)) {
//	                arrayComodinesp2[j].indices[arrayComodinesp2[j].numIndices] = i;
//                    arrayComodinesp2[j].numIndices++;
//                    estaba = true;
//	            }
//	        }
//	        // si no estaba, se añade otra entrada en la lista
//	        if (!estaba) {
//	            arrayComodinesp2[numComodinesp2].valor == p2.get(i);
//	            arrayComodinesp2[numComodinesp2].indices[arrayComodinesp2[numComodinesp2].numIndices] == i;
//	            arrayComodinesp2[numComodinesp2].numIndices++;
//	            numComodinesp2++;
//	        } else {
//	            // se resetea estaba para la siguiente iteración
//	            estaba = false;
//	        }
//        }
//    }
//
//    // Se juntan los comodines de ambas en el vector de estructuras de
//    // comodines comunes
//    int numComodinesComunes;
//    for (int i = 0; i < p1.size(); i++) {
//        for (int j = 0; j < p2.size(); j++) {
//            // si dos coinciden , se añade un nuevo comodín común
//            // se sabe que en ninguno de los dos hay comodines repetidos
//            if (arrayComodinesp1[i].valor == arrayComodinesp2[j].valor) {
//                arrayComodinesComunes[numComodinesComunes].valor = arrayComodinesp1[i].valor;
//                arrayComodinesComunes[numComodinesComunes].numIndicesp1 = arrayComodinesp1[i].numIndices;
//                // copia el vector de indices de comodinesp1 en el de comunes para p1
//                std::copy(arrayComodinesp1[i].indices, arrayComodinesp1[i].indices + 6, arrayComodinesComunes[numComodinesComunes].indicesp1);
//
//                // hace lo mismo para p2
//                arrayComodinesComunes[numComodinesComunes].numIndicesp2 = arrayComodinesp2[i].numIndices;
//                // copia el vector de indices de comodinesp2 en el de comunes para p2
//                std::copy(arrayComodinesp2[i].indices, arrayComodinesp2[i].indices + 6, arrayComodinesComunes[numComodinesComunes].indicesp2);
//
//                numComodinesComunes++;
//            }
//        }
//    }
//
//    bool sigueLocal = false;
//    while (!parar) {
//        RDN(p1, tuplaTemp1);
//
//        for (int i = 0; i < NUM_MAX_INTENTOS; i++) {
//            RDN(p2, tuplaTemp2);
//
//            // se comprueba si en los índices con los mismos comodines (si hay),
//            // las dos tuplas obtenidas tienen los mismos valores
//
//            if (numComodinesComunes == 0) { // si no hay índices comunes
//                                            // ya se puede parar
//                parar = true;
//            } else {
//                for (int i = 0; (i < numComodinesComunes) && (sigueLocal); i++) {
//                    for (int j = 0; (j < arrayComodinesComunes[i].numIndicesp1) && (sigueLocal); j++) {
//                        for (int k = 0; (k < arrayComodinesComunes[i].numIndicesp2) && (sigueLocal); k++) {
//                            // sigueLocal será false si se encuentra algún par de posiciones con comodines comunes entre
//                            // las dos tuplas que sean diferentes en contenido
//                            sigueLocal = (tuplaTemp1.get(arrayComodinesComunes[i].indicesp1[j]) != tuplaTemp2.get(arrayComodinesComunes[i].indicesp2[k]));
//                        }
//                    }
//                }
//
//                if (sigueLocal) { // si todos los pares de posiciones son iguales
//                                  // las hemos encontrado
//                    parar = true;
//                }
//            }
//        }
//    }
//
//    // devolvemos las tuplas
//    t1 = tuplaTemp1;
//    t2 = tuplaTemp2;
}


void conectar(Socket& chan, int& socket_fd) {
    const int MAX_ATTEMPS = 10;
    int count = 0;
    do {
        // Conexión con el servidor
        socket_fd = chan.Connect();
        count++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd == -1) {
            sleep(1);
        }
    } while(socket_fd == -1 && count < MAX_ATTEMPS);
    
    // Nos aseguramos de que se ha realizado la conexión
    if(socket_fd == -1) {
        std::cerr << "No se ha podido conectar con el servidor" << std::endl;    
    }
}
