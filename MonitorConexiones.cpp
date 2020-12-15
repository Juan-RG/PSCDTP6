
#include "MonitorConexiones.hpp"



MonitorConexiones::MonitorConexiones(const int NR1){
    this->numeroConexionesMaximas = NR1;
    this->numeroConexiones = 0;

}
/**
 * destructor de la clase
 */
MonitorConexiones::~MonitorConexiones() {
}

void MonitorConexiones::puedeConectar(){
    unique_lock<mutex> lck (mtx);
    //se bloqueara ambos procesos uno esperando la respuesta y el del servidor esperando recursos
    while(numeroConexiones >= numeroConexionesMaximas){
        printf("me bloqueo\n");
        recursosDisponibles.wait(lck);
    }
    numeroConexiones++;
}


void MonitorConexiones::liberarConexion(){
    unique_lock<mutex> lck (mtx);
    numeroConexiones--;
    //notificamos a los procesos dormidos
    recursosDisponibles.notify_one();
}
