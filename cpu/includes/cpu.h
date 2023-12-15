/*
 * memoria.h
 *
 *  Created on: Apr 3, 2023
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <readline/readline.h>

#include "server_utils.h"
#include "client_utils.h"
#include "utils.h"
#include "initial_setup.h"
#include "comm_thread.h"
#include "../../shared/includes/tad.h"

void terminar_programa();

// ___ CONFIG VARIABLES ____

t_log* logger;
t_config* config;

int server_connection;
int memoria_connection;
char* memoria_ip;
char* memoria_port;
char* server_port;
int retardo_instruc;
int tam_max_segmento;

// _____________________

// ___ CONTEXTO ____

//Declaramos los registros de proposito general
uint16_t ip;
char ax[5];
char bx[5];
char cx[5];
char dx[5];
char eax[9];
char ebx[9];
char ecx[9];
char edx[9];
char rax[17];
char rbx[17];
char rcx[17];
char rdx[17];
char* contexto_param;
uint32_t contexto_estado;

// ________


#endif /* CPU_H_ */
