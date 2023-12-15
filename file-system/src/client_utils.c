#include "../includes/client_utils.h"

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

int handshake_cliente(int socket_cliente, uint8_t tipo_cliente, uint8_t tipo_servidor){ //char* handshake (int socket_cliente, uint8_t tipo_cliente)
	char* message = "";
	uint8_t handshake = tipo_cliente;
	uint8_t result;

	switch(tipo_servidor){
				case 1:
					message = "Cpu";
					break;
				case 2:
					message = "Kernel";
					break;
				case 3:
					message = "File system";
					break;
				case 4:
					message = "Memoria";
					break;
				default:
					break;
			}

	send(socket_cliente, &handshake, sizeof(uint8_t), NULL);
	log_info(logger, message);
	recv(socket_cliente, &result, sizeof(uint8_t), MSG_WAITALL); //consultar por un timeout

	if(result == 1){
		log_info(logger, "Se establecio correctamente la conexion");
	} else {
		log_info(logger, "Fallo al realizar el handshake, cerrando conexion");
		result = -1;
	}

	return result;
}

void crear_header(void* a_enviar, t_buffer* buffer, int lineas, uint32_t codigo){
	//Reservo el stream para el header del paquete
	int offset = 0;

	//Añado los datos del header al stream
	memcpy(a_enviar + offset, &(codigo), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(lineas), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(buffer->size), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, buffer->stream, buffer->size);
}

void serializar_respuesta_file_kernel(int socket_cliente, t_resp_file respuesta)
{
	t_buffer *buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(t_resp_file);

	void *stream = malloc(buffer->size);

	memcpy(stream, &respuesta, buffer->size);

	buffer->stream = stream;

	void *a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar, buffer, 0, 1);

	send(socket_cliente, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	// Liberar memoria utilizada
	free(buffer->stream);
	free(buffer);
	free(a_enviar);

}

uint32_t calcular_tam_instruc_mem(t_instruc_mem* instruccion){
	uint32_t size = 0;

	size = sizeof(contexto_estado_t) +
			sizeof(uint32_t) +
			sizeof(uint32_t) + instruccion->param1_length +
			sizeof(uint32_t) + instruccion->param2_length +
			sizeof(uint32_t) + instruccion->param3_length;

	return size;
}

void copiar_instruccion_memoria(void* stream, t_instruc_mem* instruccion){
	int offset = 0;

	memcpy(stream + offset, &instruccion->estado, sizeof(contexto_estado_t));
	offset += sizeof(contexto_estado_t);

	memcpy(stream + offset, &instruccion->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, &instruccion->param1_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, instruccion->param1, instruccion->param1_length);
	offset += instruccion->param1_length;

	memcpy(stream + offset, &instruccion->param2_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, instruccion->param2, instruccion->param2_length);
	offset += instruccion->param2_length;

	memcpy(stream + offset, &instruccion->param3_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, instruccion->param3, instruccion->param3_length);
}

void serializar_instruccion_memoria(int socket,t_instruc_mem* instruccion)
{
	//Creo el buffer a utilizar para las instrucciones
		t_buffer* buffer = malloc(sizeof(t_buffer));

		buffer->size = calcular_tam_instruc_mem(instruccion);

		//Asigno memoria para el stream del tamaño de mi lista
		void* stream = malloc(buffer->size);

		//Leo toda la lista para copiar los valores en memoria
		copiar_instruccion_memoria(stream,instruccion);

		//Añado el stream a mi buffers
		buffer->stream = stream;

		void* a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

		crear_header(a_enviar,buffer,0,1);

		//Envio todo el stream al servidor
		send(socket, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

		//Libero memoria que ya no voy a utilizar
		free(buffer->stream);
		free(buffer);
		free(a_enviar);
}

void serializar_instruccion_mov(int socket,t_instruc_mov* instruccion)
{
	//Creo el buffer a utilizar para las instrucciones
		t_buffer* buffer = malloc(sizeof(t_buffer));

		buffer->size = calcular_tam_instruc_mov(instruccion);

		//Asigno memoria para el stream del tamaño de mi lista
		void* stream = malloc(buffer->size);

		//Leo toda la lista para copiar los valores en memoria
		copiar_instruccion_memoria(stream,instruccion);

		//Añado el stream a mi buffers
		buffer->stream = stream;

		void* a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

		crear_header(a_enviar,buffer,0,1);

		//Envio todo el stream al servidor
		send(socket, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

		//Libero memoria que ya no voy a utilizar
		free(buffer->stream);
		free(buffer);
		free(a_enviar);
}

void generar_instruccion_mov(t_instruc_mov* instruccion_nueva,contexto_estado_t instruccion, uint32_t dir_fisica, uint32_t tamanio, char* valor){
	char* s_dir_fisica = string_itoa(dir_fisica);
	char* s_tamanio = string_itoa(tamanio);
	int size_dir = strlen(s_dir_fisica) + 1;
	int size_tamanio = strlen(s_tamanio) + 1;

	instruccion_nueva->param1_length = size_dir;
	instruccion_nueva->param1 = realloc(instruccion_nueva->param1,size_dir);
	memcpy(instruccion_nueva->param1,s_dir_fisica,size_dir);

	instruccion_nueva->param2_length = size_tamanio;
	instruccion_nueva->param2 = realloc(instruccion_nueva->param2,size_tamanio);
	memcpy(instruccion_nueva->param2,s_tamanio,size_tamanio);

	instruccion_nueva->param3_length = tamanio;
	instruccion_nueva->param3 = realloc(instruccion_nueva->param3, tamanio);
	memcpy(instruccion_nueva->param3,valor,tamanio);

	instruccion_nueva->estado = instruccion;
	free(s_dir_fisica);
	free(s_tamanio);
}

t_instruc_mov* inicializar_instruc_mov()
{
	t_instruc_mov* instruccion = malloc(sizeof(t_instruc_mov));
	instruccion->pid=0;
	instruccion->param1 = malloc(sizeof(char) * 2);
	memcpy(instruccion->param1, "0", (sizeof(char) * 2));
	instruccion->param1_length = sizeof(char) * 2;
	instruccion->param2 = malloc(sizeof(char) * 2);
	memcpy(instruccion->param2, "0", (sizeof(char) * 2));
	instruccion->param2_length = sizeof(char) * 2;
	instruccion->param3 = malloc(sizeof(char) * 2);
	memcpy(instruccion->param3, "0", (sizeof(char)));
	instruccion->param3_length = sizeof(char);
	instruccion->estado = CREATE_SEGMENT;

	return instruccion;
}


void serializar_memoria(int socket_cliente, void* memoria, int tam_memoria)
{
	t_buffer *buffer = malloc(sizeof(t_buffer));

	buffer->size = tam_memoria;

	void *stream = malloc(buffer->size);

	memcpy(stream,memoria,tam_memoria);

	buffer->stream = stream;

	void *a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar, buffer, 0, 1);

	send(socket_cliente, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	free(buffer->stream);
	free(buffer);
	free(a_enviar);
}

uint32_t calcular_tam_instruc_mov(t_instruc_mov* instruccion){
	uint32_t size = 0;

	size = sizeof(contexto_estado_t) +
			sizeof(uint32_t) +
			sizeof(uint32_t) + instruccion->param1_length +
			sizeof(uint32_t) + instruccion->param2_length +
			sizeof(uint32_t) + instruccion->param3_length;

	return size;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
