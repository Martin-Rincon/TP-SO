/*
 * long_term_planification.h
 *
 *  Created on: May 4, 2023
 *      Author: utnso
 */

#ifndef LONG_TERM_PLANIFICATION_H_
#define LONG_TERM_PLANIFICATION_H_

#include "utils.h"
#include "semaphore.h"
#include "comm_Mem.h"

void estado_new();
void agregar_pcb_a_new(t_list* instrucciones, uint32_t socket);
pcb_t *crear_proceso(t_list* instrucciones, uint32_t socket);
void estado_exit();
void iniciar_planificador_largo_plazo();

extern t_lista_mutex* lista_tabla_segmentos;
extern int memoria_connection;

#endif /* LONG_TERM_PLANIFICATION_H_ */
