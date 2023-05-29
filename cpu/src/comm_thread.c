#include "../includes/comm_thread.h"

void conexion_kernel(int server_connection){

	log_info(logger, "Cpu lista para recibir al Kernel");
	int connection_fd = esperar_cliente(server_connection);
	log_info(logger,handshake(connection_fd));

	int exit_status = 0;

	while (exit_status == 0) {
		//Reservo memoria para el paquete
		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));

		t_contexto* contexto = contexto_create();

		//Recivo el header del paquete + el stream de datos
		deserializar_header(paquete, connection_fd);

		//Reviso el header para saber de que paquete se trata y deserealizo acorde
		switch(paquete->codigo_operacion){
			case 1:
				deserializar_contexto(paquete->buffer, paquete->lineas, contexto);
				int result = ejecutar_contexto(contexto, paquete->lineas);
				armar_contexto(contexto);
				log_info(logger, "Creación de Proceso PID: %d", contexto->pid);
				//log_info(logger, "El numero de estado es: %d", contexto->estado);
				//log_info(logger, "El parametro de interrupcion es: %s", contexto->param); //aca esta el error
				//log_info(logger,contexto->registros->ax);
				//log_info(logger,contexto->registros->ip);
				serializar_contexto(connection_fd,contexto);
				break;
			default:
				exit_status = 1;
				break;
		}

		contexto_destroy(contexto);

		free(paquete->buffer->stream);
	    free(paquete->buffer);
	    free(paquete);
	}
}
