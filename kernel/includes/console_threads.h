#ifndef CONSOLE_THREADS_H_
#define CONSOLE_THREADS_H_

#include <commons/string.h>
#include <commons/collections/queue.h>

#include "server_utils.h"
#include "client_utils.h"
#include "../../shared/includes/tad.h"

void atender_consola(int socket_servidor);
void iterator(char* value);

extern t_log* logger;

#endif /* CONSOLE_THREADS_H_ */
