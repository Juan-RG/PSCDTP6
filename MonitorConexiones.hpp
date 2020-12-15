
 #ifndef MONITOR_CONEXIONES_HPP
 #define MONITOR_CONEXIONES_HPP

 #include <mutex>
 #include <condition_variable>
 using namespace std; //mutex, condition_variable, etc.

 class MonitorConexiones {
     private:
     int numeroConexionesMaximas; //primer recurso
     int numeroConexiones;
     mutex mtx; //para la ejecución de procs en exclusión mutua
     condition_variable recursosDisponibles; //condicion para dormir los clientes que no tengan los recursos disponibles

 public:
     //------------------------- constructores
     MonitorConexiones(const int NR1);
     //------------------------- destructor
     ~MonitorConexiones();
     //------------------------- usuario

     //metodo para liberar recursos en caso que se pueda
     void liberarConexion();
     //metodo para conceder recursos en caso que se pueda
     void puedeConectar();

     };
 #endif
