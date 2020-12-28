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
LindaDriver::LindaDriver(string ipServerRegistro, int puertoServerRegistro, string tipoCliente) {                       // FIXME: tipoCliente debería desaparecer de aquí
    Socket chan(ipServerRegistro, puertoServerRegistro);
    int socket_fd;

    // Conecta con el servidor de registro
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

    // manda qué tipo de cliente es                                                                                     // FIXME: Debería desaparecer de aquí
    mensaje = tipoCliente; // BUSCADOR | PUBLICADOR
    send_bytes = chan.Send(socket_fd, mensaje);
    if(send_bytes == -1) {
        cerr << "Error al enviar datos al servidor de registro: " << strerror(errno) << endl;
        // Cerramos el socket
        chan.Close(socket_fd);
        std::terminate();
    }

    // Recibe los datos de los servidores (ip1,ip2,ip3,puerto)
    read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
    cout << buffer + "\n";
    //cout<< "hola \n";
    // Se desconecta del servidor
    //mensaje = MENSAJE_DESCONEXION;
    //send_bytes = chan.Send(socket_fd, mensaje);
    //if(send_bytes == -1) {
    //    cerr << "Error al enviar datos al servidor de registro: " << strerror(errno) << endl;
    //    // Cerramos el socket
    //    chan.Close(socket_fd);
    //    std::terminate();                                                                                                        // FIXME: Terminar el hilo, no el proceso
    //}
    chan.Close(socket_fd); // y cierra el Socket
    cout<< "socket con server de registro cerrado\n";
    // seguramente explote:
    //sscanf(buffer.c_str(), "%s,%s,%s,%s", &this->ip_server_1.c_str(), &this->ip_server_2.c_str(), &this->ip_server_3.c_str(), &this->puerto.c_str());

    // TODO: probar si acaso funciona...
    // regex er("([0-9]+\\\\.[0-9]+\\\\.[0-9]+\\\\.[0-9]+),([0-9]+\\\\.[0-9]+\\\\.[0-9]+\\\\.[0-9]+),([0-9]+\\\\.[0-9]+\\\\.[0-9]+\\\\.[0-9]+),([0-9]+\\\\.[0-9]+\\\\.[0-9]+\\\\.[0-9]+),[0-9]+,[0-9]+,[0-9]+");
    //regex er(".,.,.,.,.,.");
    //smatch sm; //almacenará los resultado del "matching"

    //busca la "er" en "s", almacenando los resultados en "sm"
    //sm[0] es todo el contenido en que se ha hecho el "matching"
    //sm[1] la parte correspondiente a la primera expresión regular entre paréntesis. En este caso [0-9]+
    //sm[2] la parte correspondiente a la segunda expresión regular entre paréntesis.
    //      También [0-9]+ en este caso
    //...
    //regex_search(buffer, sm, er);

    stringstream s_stream(buffer); //create string stream from the string
    string substr;
    getline(s_stream, substr, ','); //get first string delimited by comma
    this->ip_server_1 = substr;
    getline(s_stream, substr, ','); //get first string delimited by comma
    this->ip_server_2 = substr;
    getline(s_stream, substr, ','); //get first string delimited by comma
    this->ip_server_3 = substr;
    getline(s_stream, substr, ','); //get first string delimited by comma
    this->puerto_server_1 = substr;
    getline(s_stream, substr, ','); //get first string delimited by comma
    this->puerto_server_2 = substr;
    getline(s_stream, substr, ','); //get first string delimited by comma
    this->puerto_server_3 = substr;
    
    // Cambia los detalles de los sockets de cada servidor con los datos recibidos
    chanServer1.CambiaDetallesServidor(ip_server_1, stoi(puerto_server_1));
    chanServer2.CambiaDetallesServidor(ip_server_2, stoi(puerto_server_2));
    chanServer3.CambiaDetallesServidor(ip_server_3, stoi(puerto_server_3));
    std::cout   << "Datos de los servidores:\n"
                << "Server 1(tam. 1 a 3): '"    << this->ip_server_1 << "':'"  << this->puerto_server_1 << "'\n"
                << "Server 2(tam. 4 a 5): '"    << this->ip_server_2 << "':'"  << this->puerto_server_2 << "'\n"
                << "Server 3(tam. 6): '"        << this->ip_server_3 << "':'"  << this->puerto_server_3 << "'" <<std::endl;

    // Conexiones con los servidores Linda
    conectar(chanServer1, fdChanServer1);
    conectar(chanServer2, fdChanServer2);
    conectar(chanServer3, fdChanServer3);

    std::cout << "Conexión realizada con todos los servidores" << std::endl;
}

//destructor -----------------------------
LindaDriver::~LindaDriver() {
    string mensaje = MENSAJE_DESCONEXION;
    int send_bytes = chanServer1.Send(fdChanServer1, mensaje);
    if(send_bytes == -1) {
        cerr << "Error al enviar despedida al servidor 1: " << strerror(errno) << endl;
    }

    send_bytes = chanServer2.Send(fdChanServer2, mensaje);
    if(send_bytes == -1) {
        cerr << "Error al enviar despedida al servidor 2: " << strerror(errno) << endl;
    }

    send_bytes = chanServer3.Send(fdChanServer3, mensaje);
    if(send_bytes == -1) {
        cerr << "Error al enviar despedida al servidor 3: " << strerror(errno) << endl;
    }

    // Cierra los sockets
    chanServer1.Close(fdChanServer1);
    chanServer2.Close(fdChanServer2);
    chanServer3.Close(fdChanServer3);

    std::cout << "Conexión terminada con todos los servidores" << std::endl;
}

//operadores ----------------------------
//PostNote
void LindaDriver::PN(const Tupla t) {
    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer;
    cout << "LindaDriver: PN" << endl;
    //int socket_fd;
    //Socket chanServer1(this->ip_server_1, stoi(this->puerto));
    //Socket chanServer2(this->ip_server_2, stoi(this->puerto));
    //Socket chanServer3(this->ip_server_3, stoi(this->puerto));
    mensaje = MENSAJE_PN + t.to_string();
    cout << "LindaDriver: Mando..." << mensaje << "y t.size() es" << t.size()<< endl;

    if (t.size() < 4) { // tam. 1 a 3, va al servidor 1
        //conectar(chanServer1, fdChanServer1);
        cout << "LindaDriver: mando al server 1" << endl;
        // manda el código de operación junto a la tupla convertida a string
        send_bytes = chanServer1.Send(fdChanServer1, mensaje);
        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chanServer1.Close(fdChanServer1);
            std::terminate();
        }

        // Espera a recibir una confirmación
        cout << "LindaDriver: espero confirmacion del server 1" << endl;
        read_bytes = chanServer1.Recv(fdChanServer1, buffer, MESSAGE_SIZE);
        cout << "LindaDriver: recibo..." << buffer << endl;
        //chanServer1.Close(fdChanServer1); // y cierra el socket
    } else if (t.size() < 6) { // tam. 4 a 5, va al servidor 2
        //conectar(chanServer2, fdChanServer2);
        cout << "LindaDriver: mando al server 2" << endl;
        // manda el código de operación junto a la tupla convertida a string
        send_bytes = chanServer2.Send(fdChanServer2, mensaje);
        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chanServer2.Close(fdChanServer2);
            std::terminate();
        }

        // Espera a recibir una confirmación
        cout << "LindaDriver: espero confirmacion del server 2" << endl;
        read_bytes = chanServer2.Recv(fdChanServer2, buffer, MESSAGE_SIZE);
        cout << "LindaDriver: recibo..." << buffer << endl;
        //chanServer2.Close(fdChanServer2); // y cierra el socket
    } else { // tam. 6, va al servidor 3
        //conectar(chanServer3, fdChanServer3);
        cout << "LindaDriver: mando al server 3" << endl;
        // manda el código de operación junto a la tupla convertida a string
        send_bytes = chanServer3.Send(fdChanServer3, mensaje);
        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chanServer3.Close(fdChanServer3);
            std::terminate();
        }

        // Espera a recibir una confirmación
        cout << "LindaDriver: espero confirmacion del server 3" << endl;
        read_bytes = chanServer3.Recv(fdChanServer3, buffer, MESSAGE_SIZE);
        cout << "LindaDriver: recibo..." << buffer << endl;
        //chanServer3.Close(fdChanServer3); // y cierra el socket
    }

    if (buffer != RESPUESTA_CONFIRMACION) {
        std::cerr << "error al enviar la tupla " << t.to_string()
                  << " en la operación PN" << std::endl;
    }
}
    

//Pre:  "p" y "t" tienen el mismo tamaño
//Post: Remove Note, siendo "p" el patrón y "t" la tupla
void LindaDriver::RN(const Tupla p, Tupla& t) {
    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer;
    //int socket_fd;

    //Socket chanServer1(this->ip_server_1, stoi(this->puerto));
    //Socket chanServer2(this->ip_server_2, stoi(this->puerto));
    //Socket chanServer3(this->ip_server_3, stoi(this->puerto));
    mensaje = MENSAJE_RN + p.to_string();

    if (p.size() < 4) { // tam. 1 a 3, va al servidor 1
        //conectar(chanServer1, fdChanServer1);

        // manda el código de operación junto a la tupla convertida a string
        send_bytes = chanServer1.Send(fdChanServer1, mensaje);
        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chanServer1.Close(fdChanServer1);
            std::terminate();
        }

        // Espera a obtener la tupla resultado
        read_bytes = chanServer1.Recv(fdChanServer1, buffer, MESSAGE_SIZE);
        //chanServer1.Close(socket_fd); // y cierra el socket
    } else if (p.size() < 6) { // tam. 4 a 5, va al servidor 2
        //conectar(chanServer2, fdChanServer2);

        // manda el código de operación junto a la tupla convertida a string
        send_bytes = chanServer2.Send(fdChanServer2, mensaje);
        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chanServer2.Close(fdChanServer2);
            std::terminate();
        }

        // Espera a obtener la tupla resultado
        read_bytes = chanServer2.Recv(fdChanServer2, buffer, MESSAGE_SIZE);
        //chanServer2.Close(socket_fd); // y cierra el socket
    } else { // tam. 6, va al servidor 3
        //conectar(chanServer3, fdChanServer3);

        // manda el código de operación junto a la tupla convertida a string
        send_bytes = chanServer3.Send(fdChanServer3, mensaje);
        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chanServer3.Close(fdChanServer3);
            std::terminate();
        }

        // Espera a obtener la tupla resultado
        read_bytes = chanServer3.Recv(fdChanServer3, buffer, MESSAGE_SIZE);
        //chanServer3.Close(socket_fd); // y cierra el socket
    }

    // escribe los datos de la tupla obtenida
    t.from_string(buffer);
}

//Pre:  "p1" y "t1" tienen el mismo tamaño                                                                              // TODO: cambiar pres y post a que las dos tengan = tamaño tb.
//      "p2" y "t2" tienen el mismo tamaño
//Post: Remove Notes, siendo "p1" y "p2" los patrones y "t1" y "t2" las tuplas
void LindaDriver::RN_2(const Tupla p1, const Tupla p2, Tupla& t1, Tupla& t2) {
    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer;
    //int socket_fd;

    //Socket chanServer1(this->ip_server_1, stoi(this->puerto));
    //Socket chanServer2(this->ip_server_2, stoi(this->puerto));
    //Socket chanServer3(this->ip_server_3, stoi(this->puerto));
    mensaje = MENSAJE_RN_2 + p1.to_string() + "," + p2.to_string();

    if (p1.size() == p2.size()) { // Si las tuplas son de diferente tamaño, no las vamos a enviar (requisito)
        if (p1.size() < 4) { // tam. 1 a 3, va al servidor 1
            //conectar(chanServer1, fdChanServer1);

            // manda el código de operación junto a la tupla convertida a string
            send_bytes = chanServer1.Send(fdChanServer1, mensaje);
            if(send_bytes == -1) {
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                // Cerramos el socket
                chanServer1.Close(fdChanServer1);
                std::terminate();
            }

            // Espera a obtener la tupla resultado
            read_bytes = chanServer1.Recv(fdChanServer1, buffer, MESSAGE_SIZE);
            //chanServer1.Close(socket_fd); // y cierra el socket
        } else if (p1.size() < 6) { // tam. 4 a 5, va al servidor 2
            //conectar(chanServer2, fdChanServer2);

            // manda el código de operación junto a la tupla convertida a string
            send_bytes = chanServer2.Send(fdChanServer2, mensaje);
            if(send_bytes == -1) {
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                // Cerramos el socket
                chanServer2.Close(fdChanServer2);
                std::terminate();
            }

            // Espera a obtener la tupla resultado
            read_bytes = chanServer2.Recv(fdChanServer2, buffer, MESSAGE_SIZE);
            //chanServer2.Close(socket_fd); // y cierra el socket
        } else { // tam. 6, va al servidor 3
            //conectar(chanServer3, fdChanServer3);

            // manda el código de operación junto a la tupla convertida a string
            send_bytes = chanServer3.Send(fdChanServer3, mensaje);
            if(send_bytes == -1) {
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                // Cerramos el socket
                chanServer3.Close(fdChanServer3);
                std::terminate();
            }

            // Espera a obtener la tupla resultado
            read_bytes = chanServer3.Recv(fdChanServer3, buffer, MESSAGE_SIZE);
            //chanServer3.Close(socket_fd); // y cierra el socket
        }
        // escribe los datos de la tupla obtenida

        stringstream s_stream(buffer); //create string stream from the string
        string substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        t1.from_string(substr);
        getline(s_stream, substr, ','); //get first string delimited by comma
        t2.from_string(substr);
    }
}

//Pre:  "p" y "t" tienen el mismo tamaño
//Post: ReaD Note, siendo "p" el patrón y "t" la tupla
void LindaDriver::RDN(const Tupla p, Tupla& t) {
    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer;
    //int socket_fd;

    //Socket chanServer1(this->ip_server_1, stoi(this->puerto));
    //Socket chanServer2(this->ip_server_2, stoi(this->puerto));
    //Socket chanServer3(this->ip_server_3, stoi(this->puerto));
    mensaje = MENSAJE_RDN + p.to_string();

    if (p.size() < 4) { // tam. 1 a 3, va al servidor 1
        //conectar(chanServer1, fdChanServer1);

        // manda el código de operación junto a la tupla convertida a string
        send_bytes = chanServer1.Send(fdChanServer1, mensaje);
        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chanServer1.Close(fdChanServer1);
            std::terminate();
        }

        // Espera a obtener la tupla resultado
        read_bytes = chanServer1.Recv(fdChanServer1, buffer, MESSAGE_SIZE);
        //chanServer1.Close(socket_fd); // y cierra el socket
    } else if (p.size() < 6) { // tam. 4 a 5, va al servidor 2
        //conectar(chanServer2, fdChanServer2);

        // manda el código de operación junto a la tupla convertida a string
        send_bytes = chanServer2.Send(fdChanServer2, mensaje);
        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chanServer2.Close(fdChanServer2);
            std::terminate();
        }

        // Espera a obtener la tupla resultado
        read_bytes = chanServer2.Recv(fdChanServer2, buffer, MESSAGE_SIZE);
        //chanServer2.Close(socket_fd); // y cierra el socket
    } else { // tam. 6, va al servidor 3
        //conectar(chanServer3, fdChanServer3);

        // manda el código de operación junto a la tupla convertida a string
        send_bytes = chanServer3.Send(fdChanServer3, mensaje);
        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chanServer3.Close(fdChanServer3);
            std::terminate();
        }

        // Espera a obtener la tupla resultado
        read_bytes = chanServer3.Recv(fdChanServer3, buffer, MESSAGE_SIZE);
        //chanServer3.Close(socket_fd); // y cierra el socket
    }

    // escribe los datos de la tupla obtenida
    t.from_string(buffer);
}

// Pre:  "p1" y "t1" tienen el mismo tamaño
//       "p2" y "t2" tienen el mismo tamaño
// Post: ReaD Notes, siendo "p1" y "p2" los patrones y "t1" y "t2" las tuplas
void LindaDriver::RDN_2(const Tupla p1, const Tupla p2, Tupla& t1, Tupla& t2) { // Exactamente igual que RN_2 pero sin borrarlas al final
    string mensaje;
    int read_bytes;   // num de bytes recibidos en un mensaje
    int send_bytes;   // num de bytes enviados en un mensaje
    string buffer;
    //int socket_fd;
    //Socket chanServer1(this->ip_server_1, stoi(this->puerto));
    //Socket chanServer2(this->ip_server_2, stoi(this->puerto));
    //Socket chanServer3(this->ip_server_3, stoi(this->puerto));
    mensaje = MENSAJE_RDN_2 + p1.to_string() + "," + p2.to_string();

    if (p1.size() == p2.size()) { // Si las tuplas son de diferente tamaño, no las vamos a enviar (requisito)
        if (p1.size() < 4) { // tam. 1 a 3, va al servidor 1
            //conectar(chanServer1, fdChanServer1);

            // manda el código de operación junto a la tupla convertida a string
            send_bytes = chanServer1.Send(fdChanServer1, mensaje);
            if(send_bytes == -1) {
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                // Cerramos el socket
                chanServer1.Close(fdChanServer1);
                std::terminate();
            }

            // Espera a obtener la tupla resultado
            read_bytes = chanServer1.Recv(fdChanServer1, buffer, MESSAGE_SIZE);
            //chanServer1.Close(socket_fd); // y cierra el socket
        } else if (p1.size() < 6) { // tam. 4 a 5, va al servidor 2
            //conectar(chanServer2, fdChanServer2);

            // manda el código de operación junto a la tupla convertida a string
            send_bytes = chanServer2.Send(fdChanServer2, mensaje);
            if(send_bytes == -1) {
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                // Cerramos el socket
                chanServer2.Close(fdChanServer2);
                std::terminate();
            }

            // Espera a obtener la tupla resultado
            read_bytes = chanServer2.Recv(fdChanServer2, buffer, MESSAGE_SIZE);
            //chanServer2.Close(socket_fd); // y cierra el socket
        } else { // tam. 6, va al servidor 3
            //conectar(chanServer3, fdChanServer3);

            // manda el código de operación junto a la tupla convertida a string
            send_bytes = chanServer3.Send(fdChanServer3, mensaje);
            if(send_bytes == -1) {
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                // Cerramos el socket
                chanServer3.Close(fdChanServer3);
                std::terminate();
            }

            // Espera a obtener la tupla resultado
            read_bytes = chanServer3.Recv(fdChanServer3, buffer, MESSAGE_SIZE);
            //chanServer3.Close(socket_fd); // y cierra el socket
        }
        // escribe los datos de la tupla obtenida
        cout<<buffer<<endl;
        stringstream s_stream(buffer); //create string stream from the string
        string substr;
        cout<<"prueba 1\n";
        getline(s_stream, substr, ','); //get first string delimited by comma
        t1.from_string(substr);
        cout<<"prueba 2\n";
        getline(s_stream, substr, ','); //get first string delimited by comma
        t2.from_string(substr);
        cout<<"prueba 3\n";
    }
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
        std::cerr << "No se ha podido contactar con el servidor" << std::endl;
        std::terminate(); // Termina el hilo
    }
}
