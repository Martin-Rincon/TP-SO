#ifndef CLIENT_UTILS_H_
#define CLIENT_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>

#include "../../shared/includes/tad.h"

int crear_conexion(char* ip, char* puerto);
void liberar_conexion(int socket_cliente);
void serializar_instrucciones(int socket,t_list* lista);
uint32_t calcular_tam_instrucciones(t_list* lista);
void copiar_instrucciones(void* stream, t_list* lista);
void crear_header(void* a_enviar, t_buffer* buffer, int lineas);
uint8_t recibir_respuesta(int socket_cliente);
t_instruc* crear_instruccion();

#endif
