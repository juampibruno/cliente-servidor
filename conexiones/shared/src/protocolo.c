#include "../include/protocolo.h"

//Esta funcion me sirve para enviar un mensaje directamente, no es necesario serealizarla
//En realidad hace las 2 cosas juntas pq es una serializacion muuuy chiquita
void enviar_mensaje(char* mensaje, int socket_cliente) {
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

// Esta funcion me va a servir para serealizar el paquete
void* serializar_paquete(t_paquete* paquete, int bytes) {
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}


void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(t_log* logger,int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}


void eliminar_paquete(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}


// APROBAR_OPERATIVOS
static void* serializar_aprobar_operativos(uint8_t nota1, uint8_t nota2) {
    void* stream = malloc(sizeof(op_code) + sizeof(uint8_t) * 2);

    op_code cop = APROBAR_OPERATIVOS;
    memcpy(stream, &cop, sizeof(op_code));
    memcpy(stream+sizeof(op_code), &nota1, sizeof(uint8_t));
    memcpy(stream+sizeof(op_code)+sizeof(uint8_t), &nota2, sizeof(uint8_t));
    return stream;
}

static void deserializar_aprobar_operativos(void* stream, uint8_t* nota1, uint8_t* nota2) {
    memcpy(nota1, stream, sizeof(uint8_t));
    memcpy(nota2, stream+sizeof(uint8_t), sizeof(uint8_t));
}

bool send_aprobar_operativos(int fd, uint8_t nota1, uint8_t nota2) {
    size_t size = sizeof(op_code) + sizeof(uint8_t) * 2;
    void* stream = serializar_aprobar_operativos(nota1, nota2);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

bool recv_aprobar_operativos(int fd, uint8_t* nota1, uint8_t* nota2) {
    size_t size = sizeof(uint8_t) * 2;
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_aprobar_operativos(stream, nota1, nota2);

    free(stream);
    return true;
}

// MIRAR_NETFLIX
static void* serializar_mirar_netflix(size_t* size, char* peli, uint8_t cant_pochoclos) {
    size_t size_peli = strlen(peli) + 1;
    *size =
          sizeof(op_code)   // cop
        + sizeof(size_t)    // total
        + sizeof(size_t)    // size de char* peli
        + size_peli         // char* peli
        + sizeof(uint8_t);  // cant_pochoclos
    size_t size_payload = *size - sizeof(op_code) - sizeof(size_t);

    void* stream = malloc(*size);

    op_code cop = MIRAR_NETFLIX;
    memcpy(stream, &cop, sizeof(op_code));
    memcpy(stream+sizeof(op_code), &size_payload, sizeof(size_t));
    memcpy(stream+sizeof(op_code)+sizeof(size_t), &size_peli, sizeof(size_t));
    memcpy(stream+sizeof(op_code)+sizeof(size_t)*2, peli, size_peli);
    memcpy(stream+sizeof(op_code)+sizeof(size_t)*2+size_peli, &cant_pochoclos, sizeof(uint8_t));

    return stream;
}

static void deserializar_mirar_netflix(void* stream, char** peli, uint8_t* cant_pochoclos) {
    // Peli
    size_t size_peli;
    memcpy(&size_peli, stream, sizeof(size_t));

    char* r_peli = malloc(size_peli);
    memcpy(r_peli, stream+sizeof(size_t), size_peli);
    *peli = r_peli;

    // Pochoclos
    memcpy(cant_pochoclos, stream+sizeof(size_t)+size_peli, sizeof(uint8_t));
}

bool send_mirar_netflix(int fd, char* peli, uint8_t cant_pochoclos) {
    size_t size;
    void* stream = serializar_mirar_netflix(&size, peli, cant_pochoclos);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

bool recv_mirar_netflix(int fd, char** peli, uint8_t* cant_pochoclos) {
    size_t size_payload;
    if (recv(fd, &size_payload, sizeof(size_t), 0) != sizeof(size_t))
        return false;

    void* stream = malloc(size_payload);
    if (recv(fd, stream, size_payload, 0) != size_payload) {
        free(stream);
        return false;
    }

    deserializar_mirar_netflix(stream, peli, cant_pochoclos);

    free(stream);
    return true;
}

// DEBUG
bool send_debug(int fd) {
    op_code cop = DEBUG;
    if (send(fd, &cop, sizeof(op_code), 0) != sizeof(op_code))
        return false;
    return true;
}
