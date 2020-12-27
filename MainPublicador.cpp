
#include <iostream>
#include <chrono>
#include "LindaDriver.hpp"
#include <unordered_set>

using namespace std;


//const int MESSAGE_SIZE = 100; //mensajes de no más 4000 caracteres


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
    const string PUBLICADOR = "PUBLICADOR";
    const string BUSCADOR = "BUSCADOR";

    //Tupla t1("cosa1");
    //Tupla t2("cosa1", "cosa2","cosa3");
    //Tupla t3("cosa1", "cosa2","cosa3");
    Tupla t4("cosa1", "cosa2","cosa3");
    Tupla t5("cosa1", "cosa2","cosa3","cosa4");
    Tupla t6("cosa1", "cosa2","cosa3","cosa4","cosa1");
    //Tupla t3("?X");
/*
    std::cout << t1.to_string() << std::endl;
    std::cout << t2.to_string() << std::endl;
    std::cout << t3.to_string() << std::endl;
*/
   // bool a = t2.match(t3);
    //printf("%d",a);
    Tupla t1("1","mi casa","árbol");  // 3 elementos
    Tupla t2("?X","mi casa","árbol");
    Tupla t3("aprieta","el","pan","45","34","88");
    Tupla t7("el","pan","45","34","88");
   // Tupla t(t3);
    //class Hash = std::hash<Key>,
    //class Pred = std::equal_to<Key>>

    struct TuplaHash {
        const std::hash<std::string> m_stringHash {};

        size_t operator()(const Tupla& value) const {
            return m_stringHash(value.get(0));
        };
    };

    unordered_multiset<Tupla, TuplaHash> s1;

    s1.insert(t1);
    s1.insert(t2);
    s1.insert(t3);
    s1.insert(t7);

    cout << "insertados..." << endl;


    unordered_multiset<Tupla, TuplaHash>::iterator itr;
    int y = 0;
    for (unordered_multiset<Tupla, TuplaHash>::iterator i = s1.begin(); i != s1.end(); i++) {
        Tupla t(*i);
        cout << t.to_string();
        y++;
    }
    cout << "numTuplas= " << y << endl;


    Tupla t22("?X","árbol");
    //cout <<"repe\n";
    unordered_multiset<Tupla, TuplaHash>::iterator i = s1.find(t2);
    if(i != s1.end()){
        cout << "\nresultado del find:\n";
        Tupla p(*i);
        cout << p.to_string() <<"   " << t2.to_string()<< "\n";
    }

    cout <<"\nAhora borro... " << t3.to_string() << endl;

    int k = 0;
    s1.erase(t3);
    for (unordered_multiset<Tupla, TuplaHash>::iterator i = s1.begin(); i != s1.end(); i++) {
        Tupla t(*i);
        cout << t.to_string() << "\n";
        k++;
    }
    cout << "numTuplas= " << k << endl;

    // fuerza bruta
    int numTuplas = 5;
    for (int i = 0; i < numTuplas; i++) {
        s1.insert(t1);
    }
    int j = 0;
    cout <<"\nAñadidas " << numTuplas << " tuplas a fuerza bruta: " << t1.to_string() << endl;
    for (unordered_multiset<Tupla, TuplaHash>::iterator i = s1.begin(); i != s1.end(); i++) {
        Tupla t(*i);
        cout << t.to_string() << "\n";
        j++;
    }

    cout << "numTuplas= " << j << endl;

    cout <<"\nAhora borro... " << t1.to_string() << endl;

    int l = 0;
    s1.erase(s1.equal_range(t1).first); // = que en multiset normal, indicar que se borra 1
    for (unordered_multiset<Tupla, TuplaHash>::iterator i = s1.begin(); i != s1.end(); i++) {
        Tupla t(*i);
        cout << t.to_string() << "\n";
        l++;
    }
    cout << "numTuplas= " << l << endl;


    //cout << "\nThe set s1 is : \n";
    // printing set s1
    //set<Tupla,std::less<Tupla>>::iterator itr;
    //cout << "\nThe set s1 is : \n";



    /*
    for (auto& currentElement : s1){
        currentElement
    }*/
/*
    if(t2.match(t1)){
        cout << "Match";
    }else{
        cout << "Dont match";
    }

    t3.from_string("[a,b,c,45,34,pan]");
    cout << t3.to_string() << endl;
*/

   // std::cout << temp.to_string() << std::endl;

    /*
    // servidor
    const string CONEXION_ESTABLECIDA = "CONEXION ESTABLECIDA";
    // cliente
    string CONECTAR = "CONECTAR";

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
    */

    //cortamos el string recibido con los datos ip1,ip2,ip3,puerto
    //hilo para utilizar el linda driver con la conexion o bucle
    //join del hilo para cuando finalice

    //LindaDriver pizarra(argv[1],argv[2]);
    LindaDriver pizarra("localhost",2022,PUBLICADOR);
    int numeroTuplas;
    //extraeremos la tupla de RN['totalTuplas',?totalTuplas]
    srand(time(NULL)); //reseteamos la semilla
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
    Tupla totalTuplas("TotalTuplas","?X");
    pizarra.RN(totalTuplas,totalTuplas);
    numeroTuplas = stoi(totalTuplas.get(1));
    //Linda.pn[totalTuplas+1,origen,destino,precio]
    numeroTuplas=numeroTuplas+1;
    cout<< numeroTuplas<<"\n";
    Tupla nuevaTupla(to_string(numeroTuplas),origen,destino,to_string(precio));
    //Tupla nuevaTupla(to_string(1),origen,destino,to_string(precio));
    pizarra.PN(nuevaTupla);
    totalTuplas.set(1,to_string(numeroTuplas));
    pizarra.PN(totalTuplas);

    //Tupla nuevaTupla2(to_string(1),origen,to_string(precio)); // 3
    //pizarra.PN(nuevaTupla2);
    ///asdas

    //Tupla nuevaTupla3(to_string(1),to_string(precio)); // 2
    //pizarra.PN(nuevaTupla3);

    //Tupla nuevaTupla4(to_string(1),origen,destino,destino,destino,to_string(precio)); // 6
    //pizarra.PN(nuevaTupla4);
    return 0;
}
