#ifndef INIT_MOD1_H_
#define INIT_MOD1_H_

#include "../../shared/include/sockets.h"
#include "../../shared/include/protocolo.h"
#include <stdlib.h>
#include<stdio.h>
#include<readline/readline.h>

#include <commons/log.h>
#include<commons/string.h>
#include<commons/config.h>

t_config* iniciar_config(void);
bool generar_conexiones(t_log* logger,t_config* config, int* fd_mod2);
void cerrar_programa(t_log* logger,t_config* config);

#endif
