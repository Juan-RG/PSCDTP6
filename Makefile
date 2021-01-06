#----------------------------------------------------------------------------
#Descomentar la siguiente línea para compilar en hendrix
#SOCKETSFLAGS=-lsocket -lnsl
#----------------------------------------------------------------------------

CC=g++

SOCKET_DIR=Socket
SOCKET=${SOCKET_DIR}/Socket

TUPLA_DIR=Tupla
TUPLA=${TUPLA_DIR}/Tupla

LINDA_DRIVER_DIR=LindaDriver
LINDA_DRIVER=${LINDA_DRIVER_DIR}/LindaDriver

CLIENTES_DIR=Clientes
CLIENTE_PUBLICADOR=${CLIENTES_DIR}/MainPublicador
CLIENTE_BUSCADOR=${CLIENTES_DIR}/MainBuscador
CLIENTE_BUSCADORCOMBINADO=${CLIENTES_DIR}/MainBuscadorCombinado
INIT_TUPLAS=${CLIENTES_DIR}/InitTuplas

SERVIDORES_DIR=Servidores
SERVIDOR_MONITORIZACION=${SERVIDORES_DIR}/ServidorMonitorizacion
SERVER_LINDA=${SERVIDORES_DIR}/ServidorLinda
MONITOR_LINDA=${SERVIDORES_DIR}/MonitorServidor
SERVER_REGISTRO=${SERVIDORES_DIR}/MainServerDespliegue

PRUEBAS_DIR=Pruebas
PRUEBAS=${PRUEBAS_DIR}/pruebas
CLIENTE_CARGA=${PRUEBAS_DIR}/clienteCarga

CPPFLAGS=-I. -I${SOCKET_DIR} -O2 -std=c++11 -lsockets # Flags compilacion
LDFLAGS=-pthread # Flags linkado threads


all: ${SERVER_REGISTRO} ${SERVER_LINDA} ${CLIENTE_PUBLICADOR} ${CLIENTE_BUSCADOR} ${INIT_TUPLAS} ${CLIENTE_BUSCADORCOMBINADO} ${SERVIDOR_MONITORIZACION}


server_registro: ${SERVER_REGISTRO}

server_linda: ${SERVER_LINDA}

servidor_monitorizacion: ${SERVIDOR_MONITORIZACION}

cliente_publicador: ${CLIENTE_PUBLICADOR}

cliente_buscador: ${CLIENTE_BUSCADOR}

cliente_buscadorCombinado: ${CLIENTE_BUSCADORCOMBINADO}

init_tuplas: ${INIT_TUPLAS}

clientes: ${CLIENTE_PUBLICADOR} ${CLIENTE_BUSCADOR} ${INIT_TUPLAS}

clientes_pruebas: ${CLIENTE_CARGA} ${PRUEBAS}


#-----------------------------------------------------------
# SOCKET
${SOCKET}.o: ${SOCKET}.hpp ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp -o ${SOCKET}.o

#----------------------------------------------------------------------------
# TUPLA
${TUPLA}.o: ${TUPLA}.hpp ${TUPLA}.cpp
	${CC} -c ${CPPFLAGS} ${TUPLA}.cpp -o ${TUPLA}.o

#-----------------------------------------------------------
# LINDA_DRIVER
${LINDA_DRIVER}.o: ${LINDA_DRIVER}.cpp ${LINDA_DRIVER}.hpp
	${CC} -c ${CPPFLAGS} ${LINDA_DRIVER}.cpp -o ${LINDA_DRIVER}.o

#-----------------------------------------------------------

#-----------------------------------------------------------
${SERVIDOR_MONITORIZACION}.o: ${SERVIDOR_MONITORIZACION}.cpp
	${CC} -c ${CPPFLAGS} ${SERVIDOR_MONITORIZACION}.cpp -o ${SERVIDOR_MONITORIZACION}.o
# Linkado
${SERVIDOR_MONITORIZACION}: ${MONITOR_LINDA}.o ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${SERVIDOR_MONITORIZACION}.o
	${CC} ${LDFLAGS} ${MONITOR_LINDA}.o ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${SERVIDOR_MONITORIZACION}.o -o ${SERVIDOR_MONITORIZACION} ${SOCKETSFLAGS}

#-----------------------------------------------------------
# CLIENTES
# Compilacion
${CLIENTE_PUBLICADOR}.o: ${CLIENTE_PUBLICADOR}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENTE_PUBLICADOR}.cpp -o ${CLIENTE_PUBLICADOR}.o
# Linkado
${CLIENTE_PUBLICADOR}: ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_PUBLICADOR}.o
	${CC} ${LDFLAGS} ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_PUBLICADOR}.o -o ${CLIENTE_PUBLICADOR} ${SOCKETSFLAGS}

${CLIENTE_BUSCADOR}.o: ${CLIENTE_BUSCADOR}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENTE_BUSCADOR}.cpp -o ${CLIENTE_BUSCADOR}.o
# Linkado
${CLIENTE_BUSCADOR}: ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_BUSCADOR}.o
	${CC} ${LDFLAGS} ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_BUSCADOR}.o -o ${CLIENTE_BUSCADOR} ${SOCKETSFLAGS}

${INIT_TUPLAS}.o: ${INIT_TUPLAS}.cpp
	${CC} -c ${CPPFLAGS} ${INIT_TUPLAS}.cpp -o ${INIT_TUPLAS}.o
# Linkado
${INIT_TUPLAS}: ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${INIT_TUPLAS}.o
	${CC} ${LDFLAGS} ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${INIT_TUPLAS}.o -o ${INIT_TUPLAS} ${SOCKETSFLAGS}

${CLIENTE_BUSCADORCOMBINADO}.o: ${CLIENTE_BUSCADORCOMBINADO}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENTE_BUSCADORCOMBINADO}.cpp -o ${CLIENTE_BUSCADORCOMBINADO}.o
# Linkado
${CLIENTE_BUSCADORCOMBINADO}: ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_BUSCADORCOMBINADO}.o
	${CC} ${LDFLAGS} ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_BUSCADORCOMBINADO}.o -o ${CLIENTE_BUSCADORCOMBINADO} ${SOCKETSFLAGS}


${PRUEBAS}.o: ${PRUEBAS}.cpp
	${CC} -c ${CPPFLAGS} ${PRUEBAS}.cpp -o ${PRUEBAS}.o
# Linkado
${PRUEBAS}: ${MONITOR_LINDA}.o ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${PRUEBAS}.o
	${CC} ${LDFLAGS} ${MONITOR_LINDA}.o ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${PRUEBAS}.o -o ${PRUEBAS} ${SOCKETSFLAGS}


${CLIENTE_CARGA}.o: ${CLIENTE_CARGA}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENTE_CARGA}.cpp -o ${CLIENTE_CARGA}.o
# Linkado
${CLIENTE_CARGA}: ${MONITOR_LINDA}.o ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_CARGA}.o
	${CC} ${LDFLAGS} ${MONITOR_LINDA}.o ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_CARGA}.o -o ${CLIENTE_CARGA} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# SERVIDOR DE REGISTRO
# Compilacion
${SERVER_REGISTRO}.o: ${SERVER_REGISTRO}.cpp
	${CC} -c ${CPPFLAGS} ${SERVER_REGISTRO}.cpp -o ${SERVER_REGISTRO}.o

# Linkado
${SERVER_REGISTRO}: ${SOCKET}.o ${SERVER_REGISTRO}.o
	${CC} ${LDFLAGS} ${SOCKET}.o ${SERVER_REGISTRO}.o -o ${SERVER_REGISTRO} ${SOCKETSFLAGS}

#-----------------------------------------------------------
# MONITOR_LINDA
# Compilacion
${MONITOR_LINDA}.o: ${TUPLA}.o ${MONITOR_LINDA}.cpp
	${CC} -c ${CPPFLAGS} ${MONITOR_LINDA}.cpp -o ${MONITOR_LINDA}.o

#-----------------------------------------------------------
# SERVER_LINDA
# Compilacion
${SERVER_LINDA}.o: ${SERVER_LINDA}.cpp
	${CC} -c ${CPPFLAGS} ${SERVER_LINDA}.cpp -o ${SERVER_LINDA}.o
# Linkado
${SERVER_LINDA}: ${SERVER_LINDA}.o ${TUPLA}.o ${SOCKET}.o ${MONITOR_LINDA}.o
	${CC} ${LDFLAGS} ${MONITOR_LINDA}.o ${TUPLA}.o ${SOCKET}.o ${SERVER_LINDA}.o -o ${SERVER_LINDA} ${SOCKETSFLAGS}

#-----------------------------------------------------------
# LIMPIEZA
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${TUPLA}.o
	$(RM) ${LINDA_DRIVER}.o
	$(RM) ${CLIENTE_PUBLICADOR} ${CLIENTE_PUBLICADOR}.o
	$(RM) ${CLIENTE_BUSCADOR} ${CLIENTE_BUSCADOR}.o
	$(RM) ${SERVER_REGISTRO} ${SERVER_REGISTRO}.o
	$(RM) ${MONITOR_LINDA}.o
	$(RM) ${SERVER_LINDA} ${SERVER_LINDA}.o
	$(RM) ${PRUEBAS} ${PRUEBAS}.o
	$(RM) ${INIT_TUPLAS} ${INIT_TUPLAS}.o
	$(RM) ${CLIENTE_BUSCADORCOMBINADO} ${CLIENTE_BUSCADORCOMBINADO}.o
	$(RM) ${PRUEBAS} ${PRUEBAS}.o
	$(RM) ${CLIENTE_CARGA} ${CLIENTE_CARGA}.o
