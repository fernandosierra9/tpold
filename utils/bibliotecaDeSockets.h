#ifndef BIBLIOTECADESOCKETS_H_
#define BIBLIOTECADESOCKETS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <fcntl.h>
#include <commons/log.h>

struct mProc {
	int PID;
	char* rutaRelativaDeArchivo;
	int estadoDelProceso;
	int PC;
} mProc;

struct peticionLecturaSwap{
	int idProceso;
	int numeroPagina;
};

struct mProcesoSwap{
	int id;
	int cantidadTotalPaginas;
	int framesAsignados[];//TODO sacar vector (con el frame inicial y la cantidad de paginas alcanza
};

struct peticionLineaTLB{
	int id_proceso;
	int dir_logica;
	int dir_fisica;
	int tiempo;
};

struct paqContenido{
	char* contenido;
};

enum resultado{
	CORRECTO,
	ERROR,
};

struct respuesta{
	enum resultado res;
};

//Estructura para manejar el protocolo
typedef enum{
	VALIDAR_ARCHIVO,//PARA EL MDJ
	CREAR_ARCHIVO,	//PARA EL MDJ
	OBTENER_DATOS,	//PARA EL MDJ
	GUARDAR_DATOS	//PARA EL MDJ
}t_protocolo;

typedef struct{
	char*path;
}peticion_validar;

typedef struct{
	char*path;
	int cant_bytes;
}peticion_crear;

typedef struct{
	char*path;
	int offset;
	int size;
}peticion_obtener;

typedef struct{
	char*path;
	int offset;
	int size;
	char*buffer;
}peticion_guardar;

enum paquete {
	PAQ_INT,
	PAQ_STRING,
	PAQ_PCB,
	PAQ_MPROCESO,
	PAQ_PETICION_PAGINA_SWAP,
	PAQ_LECTURA_CONTENIDO,
	PAQ_RESPUESTA_OPERACION,
};


typedef enum{
	FIFO,
	RR,
	VRR,
	PROPIO
}t_algoritmo;

typedef struct{

	int id;
	int pc;
	int f_inicializacion;
	char* escriptorio;
	int cant_archivos;
	char** archivos;

}__attribute__((packed)) t_DTB;

t_log* log_s;


int crearSocket(int *mySocket);

int conectar(int* mySocket, int puerto, char *ip);

int setearParaEscuchar(int *mySocket, int puerto);

int aceptarConexion(int);

int escuchar(int socket, fd_set *listaDeSockets,
	void *(*funcionParaSocketNuevo)(int, void*), void *parametrosParaSocketNuevo,
	void *(*funcionParaSocketYaConectado)(int, void*),
	void *parametrosParaSocketYaConectado);

void* recibirYDeserializar(int socket,int tipo);
char* recibirYDeserializarString(int socket);
int* recibirYDeserializarEntero(int socket);

void serializarYEnviar(int socket, int tipoDePaquete, void* package);
void serializarYEnviarString(int socket, char *string);
void serializarYEnviarEntero(int socket, int *numero);

int enviarTodo(int socketReceptor, void *buffer, int *cantidadTotal);

void serializarYEnviarDTB(int,void*,t_DTB);
t_DTB RecibirYDeserializarDTB(int);

#endif /* BIBLIOTECADESOCKETS_H_ */
