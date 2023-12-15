/*
 * initial_setup.h
 *
 *  Created on: Apr 6, 2023
 *      Author: utnso
 */

#ifndef INITIAL_SETUP_H_
#define INITIAL_SETUP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

int initial_setup(void);
void failed_initial_setup(char* key);

extern char* memoria_ip;
extern char* memoria_port;

extern char* server_port;

extern int retardo_instruc;

extern int tam_max_segmento;

extern t_config* config;

extern t_log* logger;

#endif /* INITIAL_SETUP_H_ */
