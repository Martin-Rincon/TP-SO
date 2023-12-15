#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include "../../shared/includes/tad.h"

t_log* iniciar_logger(void);
t_config* iniciar_config(char * path_config);
t_instruc* crear_instruccion();
void instrucciones_destroy(t_instruc* instruccion);
#endif /* CLIENT_H_ */
