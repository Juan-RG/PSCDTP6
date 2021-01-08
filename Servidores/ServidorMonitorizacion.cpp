
#include <iostream>
#include "../LindaDriver/LindaDriver.hpp"
#include "../Tupla/Tupla.hpp"
#include <sstream>  // stringstream

using namespace std;

// Funciones auxiliares para mandar el mensaje de cierre a los servidores
void conectarConServidores(string ipServerRegistro, int puertoServerRegistro, Socket &chanServer1,
                           Socket &chanServer2, Socket &chanServer3, Socket &chanRegistro, int &fdChanServer1,
                           int &fdChanServer2, int &fdChanServer3, int &fdRegistro);

void conectar(Socket &chan, int &socket_fd);

void mandarMensaje(Socket &chan, const int &socket_fd, const string &mensaje);

void recibirMensaje(Socket &chan, const int &socket_fd, string &buffer);

static const string MENSAJE_CERRAR = "CERRAR";

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Numero de parametros introducios incorrecto:" + string(strerror(errno)) + "\n";
        exit(1);
    }
    string ipServidorDespliegue = argv[1];
    int puerto = stoi(argv[2]);

    // Sockets y sus fd's con los tres servidores Linda y el de registro, independientes de LindaDriver, para enviar el mensaje de cierre
    Socket chanServer1;
    Socket chanServer2;
    Socket chanServer3;
    Socket chanRegistro(ipServidorDespliegue, puerto);
    int fdChanServer1;
    int fdChanServer2;
    int fdChanServer3;
    int fdRegistro;

    // Se realiza la conexión por separado con los tres servidores Linda y el de registro
    conectarConServidores(ipServidorDespliegue, puerto, chanServer1, chanServer2, chanServer3,
                          chanRegistro, fdChanServer1, fdChanServer2, fdChanServer3, fdRegistro);

    /*
    // EJEMPLO PARA MATAR A LOS 4 SERVIDORES:
    cout << "morid!" << endl;
    mandarMensaje(chanServer1, fdChanServer1, MENSAJE_CERRAR);
    mandarMensaje(chanServer2, fdChanServer2, MENSAJE_CERRAR);
    mandarMensaje(chanServer3, fdChanServer3, MENSAJE_CERRAR);
    mandarMensaje(chanRegistro, fdRegistro, MENSAJE_CERRAR);
*/
    // Se instancia el LindaDriver
    LindaDriver pizarra(ipServidorDespliegue, puerto);

    bool continuar = true;
    int contador = 0;
    int numeroTuplasPasado = -1;
    bool nuevasTuplas = true;
    //tuplas de valores
    Tupla peticionesLectura("PeticionesLectura", "?X");
    Tupla peticionesEscritura("PeticionesEscritura", "?X");
    Tupla totalTuplas("TotalTuplas", "?X");
    Tupla publicadores("Publicadores", "?X");
    Tupla buscadores("Buscadores", "?X");
    Tupla buscadoresCombinados("BuscadoresCombinados", "?X");

    while (continuar) {
        Tupla peticionesLecturaTmp("", "");
        Tupla peticionesEscrituraTmp("", "");
        Tupla totalTuplasTmp("", "");
        Tupla publicadoresTmp("", "");
        Tupla buscadoresTmp("", "");
        Tupla buscadoresCombinadosTmp("", "");

        //leer tuplas de estado
        pizarra.RDN(peticionesLectura, peticionesLecturaTmp);
        pizarra.RDN(peticionesEscritura, peticionesEscrituraTmp);
        pizarra.RDN(totalTuplas, totalTuplasTmp);

        //leer numero clientes
        pizarra.RDN(publicadores, publicadoresTmp);
        pizarra.RDN(buscadores, buscadoresTmp);
        pizarra.RDN(buscadoresCombinados, buscadoresCombinadosTmp);


        cout << "\rPeticiones de lectura: " + peticionesLecturaTmp.get(1) + "\r\n";
        cout << "\rPeticiones de escritura: " + peticionesEscrituraTmp.get(1) + "\r\n";
        cout << "\rTotal de tuplas: " + totalTuplasTmp.get(1) + "\r\n";
        cout << "\rPublicadores en el sistema: " + publicadoresTmp.get(1) + "\r\n";
        cout << "\rBuscadores en el sistema: " + buscadoresTmp.get(1) + "\r\n";
        cout << "\rBuscadores Combinados en el sistema: " + buscadoresCombinadosTmp.get(1) + "\r\n";

        if (numeroTuplasPasado != stoi(totalTuplasTmp.get(1))) {
            numeroTuplasPasado = stoi(totalTuplasTmp.get(1));
            nuevasTuplas = true;
        } else {
            nuevasTuplas = false;
        }

        if (stoi(publicadoresTmp.get(1)) == 0 && stoi(buscadoresTmp.get(1)) == 0 &&
            stoi(buscadoresCombinadosTmp.get(1)) == 0
            && !nuevasTuplas) {
            cout << "if1 " + to_string(contador) + "\n";
            contador++;
            if (contador == 3) {

                mandarMensaje(chanServer1, fdChanServer1, MENSAJE_CERRAR);
                mandarMensaje(chanServer2, fdChanServer2, MENSAJE_CERRAR);
                mandarMensaje(chanServer3, fdChanServer3, MENSAJE_CERRAR);
                mandarMensaje(chanRegistro, fdRegistro, MENSAJE_CERRAR);
                continuar = false;
            }

        } else {
            cout << "paso else\n";
            contador = 0;
        }
        sleep(2);
    }
    cout << "acabo\n";
    sleep(10);


    //return 0;
}


void conectarConServidores(string ipServerRegistro, int puertoServerRegistro, Socket &chanServer1,
                           Socket &chanServer2, Socket &chanServer3, Socket &chanRegistro, int &fdChanServer1,
                           int &fdChanServer2, int &fdChanServer3, int &fdRegistro) {
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
    //std::cout << "socket con server de registro cerrado" << std::endl;

    // Parsea los datos recibidos del servidor de registro: ip1","ip2","ip3","puerto1","puerto2","puerto3
    stringstream s_stream(buffer); // Crea un stringstream a partir del buffer
    string substr;
    getline(s_stream, substr, ','); // Obtiene un string delimitado por una coma
    string ip_server_1 = substr;
    getline(s_stream, substr, ',');
    string ip_server_2 = substr;
    getline(s_stream, substr, ',');
    string ip_server_3 = substr;
    getline(s_stream, substr, ',');
    string puerto_server_1 = substr;
    getline(s_stream, substr, ',');
    string puerto_server_2 = substr;
    getline(s_stream, substr, ',');
    string puerto_server_3 = substr;

    // Cambia los detalles de los sockets de cada servidor con los datos recibidos
    chanServer1.CambiaDetallesServidor(ip_server_1, stoi(puerto_server_1));
    chanServer2.CambiaDetallesServidor(ip_server_2, stoi(puerto_server_2));
    chanServer3.CambiaDetallesServidor(ip_server_3, stoi(puerto_server_3));

    // Conexiones con los servidores Linda
    conectar(chanServer1, fdChanServer1);
    conectar(chanServer2, fdChanServer2);
    conectar(chanServer3, fdChanServer3);
    conectar(chanRegistro, fdRegistro);
}


void conectar(Socket &chan, int &socket_fd) {
    const int NUM_MAX_INTENTOS = 10;
    int count = 0;
    do {
        // Conexión con el servidor
        socket_fd = chan.Connect();
        count++;

        // Si error --> esperamos 1 segundo para reconectar
        if (socket_fd == -1) {
            sleep(1);
        }
    } while (socket_fd == -1 && count < NUM_MAX_INTENTOS);

    // Nos aseguramos de que se ha realizado la conexión
    if (socket_fd == -1) {
        std::cerr << "No se ha podido contactar con el servidor" << std::endl;
        std::terminate();
    }
}


// Pre: --
// Post: Manda "mensaje" por el socket con fd indicados, tratando los errores
void mandarMensaje(Socket &chan, const int &socket_fd, const string &mensaje) {
    int send_bytes = chan.Send(socket_fd, mensaje);
    if (send_bytes == -1) {
        std::cerr << "Error al enviar el mensaje de cierre a los servidores: " << strerror(errno) << std::endl;
        std::terminate();
    }
}


void recibirMensaje(Socket &chan, const int &socket_fd, string &buffer) {
    int read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);

    if (read_bytes == -1) {
        std::cerr << "Error al recibir datos de los servidores Linda: " << strerror(errno) << std::endl;
        std::terminate();
    }

}