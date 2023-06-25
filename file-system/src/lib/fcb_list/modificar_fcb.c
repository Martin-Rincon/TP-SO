#include "../../../includes/fcb_list.h"

void modificar_fcb_id(int id,fcb_prop_t llave, uint32_t valor){
	fcb_t* fcb = get_fcb(id);

	modificar_fcb(fcb,llave,valor);
}

void modificar_fcb(fcb_t* fcb, fcb_prop_t llave, uint32_t valor){
	t_config* fcb_fisico = config_create(fcb->ruta_archivo);
	char* valor_string = string_itoa(valor);

	switch(llave){
		case TAMANIO_ARCHIVO:
			fcb->tamanio_archivo = valor;
			config_set_value(fcb_fisico, "TAMANIO_ARCHIVO", valor_string);
			break;
		case PUNTERO_DIRECTO:
			fcb->puntero_directo = valor;
			config_set_value(fcb_fisico, "PUNTERO_DIRECTO", valor_string);
			break;
		case PUNTERO_INDIRECTO:
			fcb->puntero_indirecto = valor;
			config_set_value(fcb_fisico, "PUNTERO_INDIRECTO", valor_string);
			break;
		case PUNTERO_ARCHIVO:
			fcb->puntero_archivo = valor;
			config_set_value(fcb_fisico, "PUNTERO_ARCHIVO", valor_string);
			break;
		default:
			break;
	}

	config_save(fcb_fisico);

	config_destroy(fcb_fisico);
	free(valor_string);
}