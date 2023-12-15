#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>
#include "../../shared/includes/tad.h"
#include"calculos.h"
#include"datos.h"

extern t_log* logger;
extern bool cpu_conectada;
extern bool kernel_conectado;
extern bool fileSystem_conectado;
extern int cpu_connection;
extern int kernel_connection;
extern int fileSystem_connection;

int iniciar_servidor(char* puerto);
int esperar_cliente(int);
char* handshake(int);
void liberar_conexion(int socket_servidor);
void deserializar_header(t_paquete* paquete, int socket_cliente);
void deserializar_instruccion_memoria(t_instruc_mem* instruccion, t_buffer* buffer, int lineas);
void deserializar_instruccion_mov(t_instruc_mov* instruccion, t_buffer* buffer, int lineas);
t_resp_mem* deserializar_respuesta_memoria(t_buffer* buffer);


#endif /* SERVER_UTILS_H_ */
