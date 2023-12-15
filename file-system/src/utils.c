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
/*
void imprimir_datos(){
	int offset = tam_memoria;
	int lineas = tam_memoria / 64;
	int b = 0;
	char string[tam_memoria + lineas + 1];
	char nueva_linea = '\n';
	char final = '\0';

	for(int i = 0; i<(offset + lineas); i++){
		if((i % 64) == 0){
			memcpy(string + i,&nueva_linea, sizeof(char));
			continue;
		}
		memcpy(string + i,memoria + b, sizeof(char));
		if(string[i] == '\0') string[i] = '0';
		b++;
	}
	memcpy(string + (offset + lineas),&final, sizeof(char));
	log_info(logger,"%s",string);
}

void inicializar_datos_memoria(){
	int offset = tam_memoria;
	char caracter = '0';

	for(int i = 0; i<offset; i++){
		memcpy(memoria + i,&caracter, sizeof(char));
	}

}
*/

void inicializar_datos_memoria(){
	int offset = tam_memoria_file_system;
	char caracter = '0';

	for(int i = 0; i<offset; i++){
		memcpy(memoria_file_system + i,&caracter, sizeof(char));
	}

}

t_list* armar_lista_offsets(int id_fcb, int tam_a_leer, int p_seek){
	t_list* lista_offsets = list_create();

	int bloque_apuntado = ceil((double)(p_seek + 1) / tamanio_de_bloque);
	int nro_bloque = 0;
	t_list* lista_bloques = obtener_lista_de_bloques(id_fcb,p_seek,tam_a_leer);
	int cant_bloques = list_size(lista_bloques);

	while(nro_bloque < cant_bloques){
		offset_fcb_t* nuevo_offset = malloc(sizeof(offset_fcb_t));
		offset_fcb_t* bloque = list_get(lista_bloques, nro_bloque);

		nuevo_offset->offset = bloque->id_bloque * tamanio_de_bloque;
		nuevo_offset->id_bloque = bloque->id_bloque;
		nuevo_offset->tamanio = tamanio_de_bloque;

		if(tam_a_leer < tamanio_de_bloque){
			nuevo_offset->offset = nuevo_offset->offset + (p_seek - ((bloque_apuntado - 1) * tamanio_de_bloque));
			nuevo_offset->tamanio = tam_a_leer;
		} else if (nro_bloque + 1 == cant_bloques){
			nuevo_offset->tamanio = (p_seek + tam_a_leer) - ((bloque_apuntado - 1) * tamanio_de_bloque);
		} else if(nro_bloque == 0){
			nuevo_offset->offset = nuevo_offset->offset + (p_seek - ((bloque_apuntado - 1) * tamanio_de_bloque));
			nuevo_offset->tamanio = (bloque_apuntado * tamanio_de_bloque) - p_seek;
		}

		nro_bloque++;
		bloque_apuntado = ceil(((double)(p_seek + 1) + (tamanio_de_bloque * nro_bloque)) / tamanio_de_bloque);

		list_add(lista_offsets,nuevo_offset);
	}

	return lista_offsets;
}

void* list_pop(t_list* list)
{
	int last_element_index = (list->elements_count - 1);
	return list_remove(list, last_element_index);
}
