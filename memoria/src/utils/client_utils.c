#include "../../includes/client_utils.h"

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family,server_info->ai_socktype,server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}

void crear_header(void *a_enviar, t_buffer *buffer, uint32_t lineas)
{
	//Reservo el stream para el header del paquete
	int offset = 0;
	uint32_t i = 1;

	//Añado los datos del header al stream
	memcpy(a_enviar + offset, &(i), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(lineas), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(buffer->size), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, buffer->stream, buffer->size);
}

void serializar_tabla_segmentos(int socket_cliente, t_list *lista_tablas)
{
	t_buffer *buffer = malloc(sizeof(t_buffer));

	buffer->size = calcular_tam_total(lista_tablas);

	void *stream = malloc(buffer->size);

	copiar_tabla_segmentos(stream, lista_tablas);

	buffer->stream = stream;

	void *a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar, buffer, list_size(lista_tablas));

	send(socket_cliente, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	free(buffer->stream);
	free(buffer);
	free(a_enviar);
}
void serializar_respuesta_memoria_kernel(int socket_cliente, t_resp_mem respuesta)
{
	t_buffer *buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(t_resp_mem);

	void *stream = malloc(buffer->size);

	memcpy(stream, &respuesta, buffer->size);

	buffer->stream = stream;

	void *a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar, buffer, 0);

	send(socket_cliente, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	// Liberar memoria utilizada
	free(buffer->stream);
	free(buffer);
	free(a_enviar);

}

void serializar_instruccion_memoria(int socket, t_instruc_mem *instruccion)
{
	//Creo el buffer a utilizar para las instrucciones
	t_buffer *buffer = malloc(sizeof(t_buffer));

	buffer->size = calcular_tam_instruc_mem(instruccion);

	//Asigno memoria para el stream del tamaño de mi lista
	void *stream = malloc(buffer->size);

	//Leo toda la lista para copiar los valores en memoria
	copiar_instruccion_memoria(stream, instruccion);

	//Añado el stream a mi buffers
	buffer->stream = stream;

	void *a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar, buffer, 0);

	//Envio todo el stream al servidor
	send(socket, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	//Libero memoria que ya no voy a utilizar
	free(buffer->stream);
	free(buffer);
	free(a_enviar);
}

void serializar_memoria(int socket_cliente, void* memoria, int tam_memoria)
{
	t_buffer *buffer = malloc(sizeof(t_buffer));

	buffer->size = tam_memoria;

	void *stream = malloc(buffer->size);

	memcpy(stream,memoria,tam_memoria);

	buffer->stream = stream;

	void *a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar, buffer, 0);

	send(socket_cliente, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	free(buffer->stream);
	free(buffer);
	free(a_enviar);
}

void serializar_instruccion_mov(int socket, t_instruc_mov *instruccion)
{
	//Creo el buffer a utilizar para las instrucciones
	t_buffer *buffer = malloc(sizeof(t_buffer));

	buffer->size = calcular_tam_instruc_mov(instruccion);

	//Asigno memoria para el stream del tamaño de mi lista
	void *stream = malloc(buffer->size);

	//Leo toda la lista para copiar los valores en memoria
	copiar_instruccion_mov(stream, instruccion);

	//Añado el stream a mi buffers
	buffer->stream = stream;

	void *a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar, buffer, 0);

	//Envio todo el stream al servidor
	send(socket, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	//Libero memoria que ya no voy a utilizar
	free(buffer->stream);
	free(buffer);
	free(a_enviar);
}
