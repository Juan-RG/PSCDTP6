CC=g++

SOCKET_DIR=Socket
SOCKET=${SOCKET_DIR}/Socket

TUPLA=Tupla

LINDA_DRIVER=LindaDriver

CLIENTE_PUBLICADOR=MainPublicador
CLIENTE_BUSCADOR=MainBuscador

SERVER_REGISTRO=MainServerDespliegue
MONITOR_CONEXIONES=MonitorConexiones

SERVER_LINDA=ServidorLinda
MONITOR_LINDA=MonitorServidor

CPPFLAGS=-I. -I${SOCKET_DIR} -O2 -std=c++11 -lsockets # Flags compilacion
LDFLAGS=-pthread # Flags linkado threads

all: ${SERVER_REGISTRO} ${SERVER_LINDA} ${CLIENTE_PUBLICADOR} ${CLIENTE_BUSCADOR}

server_registro: ${SERVER_REGISTRO}

server_linda: ${SERVER_LINDA}

cliente_publicador: ${CLIENTE_PUBLICADOR}

cliente_buscador: ${CLIENTE_BUSCADOR}

clientes: ${CLIENTE_PUBLICADOR} ${CLIENTE_BUSCADOR}

#----------------------------------------------------------------------------
#Descomentar la siguiente línea para compilar en hendrix
#SOCKETSFLAGS=-lsocket -lnsl
#----------------------------------------------------------------------------

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
# CLIENTES
# Compilacion
${CLIENTE_PUBLICADOR}.o: ${CLIENTE_PUBLICADOR}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENTE_PUBLICADOR}.cpp
# Linkado
${CLIENTE_PUBLICADOR}: ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_PUBLICADOR}.o
	${CC} ${LDFLAGS} ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_PUBLICADOR}.o -o ${CLIENTE_PUBLICADOR} ${SOCKETSFLAGS}

${CLIENTE_BUSCADOR}.o: ${CLIENTE_BUSCADOR}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENTE_BUSCADOR}.cpp
# Linkado
${CLIENTE_BUSCADOR}: ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_BUSCADOR}.o
	${CC} ${LDFLAGS} ${LINDA_DRIVER}.o ${TUPLA}.o ${SOCKET}.o ${CLIENTE_BUSCADOR}.o -o ${CLIENTE_BUSCADOR} ${SOCKETSFLAGS}

#-----------------------------------------------------------
# MONITOR_CONEXIONES
# Compilacion
${MONITOR_CONEXIONES}.o: ${MONITOR_CONEXIONES}.hpp ${MONITOR_CONEXIONES}.cpp
	${CC} -c ${CPPFLAGS} ${MONITOR_CONEXIONES}.cpp -o ${MONITOR_CONEXIONES}.o

#-----------------------------------------------------------
# SERVIDOR DE REGISTRO
# Compilacion
${SERVER_REGISTRO}.o: ${SERVER_REGISTRO}.cpp
	${CC} -c ${SERVER_REGISTRO}.cpp

# Linkado
${SERVER_REGISTRO}: ${MONITOR_CONEXIONES}.o ${SOCKET}.o ${SERVER_REGISTRO}.o
	${CC} ${LDFLAGS} ${MONITOR_CONEXIONES}.o ${SOCKET}.o ${SERVER_REGISTRO}.o -o ${SERVER_REGISTRO} ${SOCKETSFLAGS}

#-----------------------------------------------------------
# MONITOR_LINDA
# Compilacion
${MONITOR_LINDA}.o: ${MONITOR_LINDA}.cpp
	${CC} -c ${MONITOR_LINDA}.cpp

#-----------------------------------------------------------
# SERVER_LINDA
# Compilacion
${SERVER_LINDA}.o: ${SERVER_LINDA}.cpp
	${CC} -c ${SERVER_LINDA}.cpp
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
	$(RM) ${MONITOR_CONEXIONES}.o
	$(RM) ${SERVER_REGISTRO} ${SERVER_REGISTRO}.o
	$(RM) ${MONITOR_LINDA}.o
	$(RM) ${SERVER_LINDA} ${SERVER_LINDA}.o