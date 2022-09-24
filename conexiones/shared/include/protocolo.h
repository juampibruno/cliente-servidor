#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <inttypes.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>

typedef enum {
    APROBAR_OPERATIVOS,
    MIRAR_NETFLIX,
    DEBUG = 69,
	MENSAJE,
	PAQUETE
} op_code;


typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

///

void enviar_mensaje(char* mensaje, int socket_cliente);
void* serializar_paquete(t_paquete* paquete, int bytes);
void* recibir_buffer(int* size, int socket_cliente);
void recibir_mensaje(t_log* logger,int socket_cliente);
t_list* recibir_paquete(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);


bool send_aprobar_operativos(int fd, uint8_t  nota1, uint8_t  nota2);
bool recv_aprobar_operativos(int fd, uint8_t* nota1, uint8_t* nota2);

bool send_mirar_netflix(int fd, char*  peli, uint8_t  cant_pochoclos);
bool recv_mirar_netflix(int fd, char** peli, uint8_t* cant_pochoclos);

bool send_debug(int fd);

#endif
