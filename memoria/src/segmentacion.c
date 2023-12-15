#include "../includes/segmentacion.h"

void crear_segmento(int id, int direccion_base, int tamanio, int pid) {
	segmento_t *segmento = malloc(sizeof(segmento_t));
	segmento->ids = id;
	segmento->direccion_base = direccion_base;
	segmento->tamanio = tamanio;
	tabla_segmentos_t *tabla = buscar_tabla(pid);
	list_add(tabla->segmentos,segmento);
}

tabla_segmentos_t* buscar_tabla(int pid)
{
	for (int i = 0; i < list_size(lista_de_tablas); i++)
	{
		tabla_segmentos_t *tabla = (tabla_segmentos_t*) list_get(lista_de_tablas, i);
		if (tabla->pid - pid == 0)
		{
			return tabla;
		}
	}
	return NULL;
}
