#include "../includes/utils.h"

t_config* iniciar_config(char * path_config)
{
	t_config* nuevo_config;
	if((nuevo_config = config_create(path_config))==NULL){
		printf("No pude leer la config \n");
		exit(2);
	}
	return nuevo_config;
}

fcb_t* iniciar_fcb(char* path_fcb){
	fcb_t* nuevo_fcb;
	if((nuevo_fcb = config_create(path_fcb))==NULL) {
		printf("No pude leer la fcb \n");
	}
	return nuevo_fcb;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("file-system.log", "FILE-SYSTEM", 1, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

t_instruc_file* inicializar_instruc_file()
{
	t_instruc_file* contexto = malloc(sizeof(t_instruc_file));
	contexto->pid=0;
	contexto->param1 = malloc(sizeof(char) * 2);
	memcpy(contexto->param1, "0", (sizeof(char) * 2));
	contexto->param1_length = sizeof(char) * 2;
	contexto->param2 = malloc(sizeof(char) * 2);
	memcpy(contexto->param2, "0", (sizeof(char) * 2));
	contexto->param2_length = sizeof(char) * 2;
	contexto->param3 = malloc(sizeof(char) * 2);
	memcpy(contexto->param3, "0", (sizeof(char) * 2));
	contexto->param3_length = sizeof(char) * 2;
	contexto->param4 = malloc(sizeof(char) * 2);
	memcpy(contexto->param4, "0", (sizeof(char) * 2));
	contexto->param4_length = sizeof(char) * 2;
	contexto->estado = CREATE_SEGMENT;

	return contexto;
}

t_instruc_mem* inicializar_instruc_mem()
{
	t_instruc_mem* contexto = malloc(sizeof(t_instruc_mem));
	contexto->pid=0;
	contexto->param1 = malloc(sizeof(char) * 2);
	memcpy(contexto->param1, "0", (sizeof(char) * 2));
	contexto->param1_length = sizeof(char) * 2;
	contexto->param2 = malloc(sizeof(char) * 2);
	memcpy(contexto->param2, "0", (sizeof(char) * 2));
	contexto->param2_length = sizeof(char) * 2;
	contexto->param3 = malloc(sizeof(char) * 2);
	memcpy(contexto->param3, "0", (sizeof(char) * 2));
	contexto->param3_length = sizeof(char) * 2;
	contexto->estado = CREATE_SEGMENT;

	return contexto;
}

void copiar_instruccion_mem(t_instruc_mem* instruccion, t_instruc_file* contexto){
	instruccion->estado = contexto->estado;
	instruccion->param1_length = contexto->param1_length;
	instruccion->param2_length = contexto->param2_length;
	instruccion->param3_length = contexto->param3_length;

	instruccion->param1 = realloc(instruccion->param1,instruccion->param1_length);
	instruccion->param2 = realloc(instruccion->param2,instruccion->param2_length);
	instruccion->param3 = realloc(instruccion->param3,instruccion->param3_length);

	memcpy(instruccion->param1,contexto->param1,instruccion->param1_length);
	memcpy(instruccion->param2,contexto->param2,instruccion->param2_length);
	memcpy(instruccion->param3,contexto->param3,instruccion->param3_length);
	memcpy(&(instruccion->pid), &(contexto->pid), sizeof(uint32_t));
}

fcb_t* inicializar_fcb(){
	fcb_t* new_fcb = malloc(sizeof(fcb_t));

	new_fcb->nombre_archivo = malloc(sizeof(char) * 2);
	memcpy(new_fcb->nombre_archivo,"0",sizeof(char) * 2);
	new_fcb->ruta_archivo = malloc(sizeof(char) * 2);
	memcpy(new_fcb->nombre_archivo,"0",sizeof(char) * 2);
	new_fcb->id = 0;
	new_fcb->puntero_archivo = 0;
	new_fcb->puntero_directo = 0;
	new_fcb->puntero_indirecto = 0;
	new_fcb->tamanio_archivo = 0;

	return new_fcb;
}
