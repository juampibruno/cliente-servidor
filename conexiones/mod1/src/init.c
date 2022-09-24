#include "../include/init.h"

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	if((nuevo_config = config_create("./cliente.config")) == NULL){
		printf("No pude leer la config");
		exit(2);
	}
	return nuevo_config;
}

bool generar_conexiones(t_log* logger,t_config* config , int* fd_mod2) {

	    //Leemos la config
	    char* ip;
	    char* puerto;

		ip = config_get_string_value(config,"IP");
		puerto = config_get_string_value(config,"PUERTO");

		// Loggeamos el valor de config
		log_info(logger,"Lei la ip %s, el puerto %s \n",ip,puerto);

    // No hardcodear, levantar de config
   // char* port_mod2 = "6969";
   //char* ip_mod2 = "0.0.0.0";

    *fd_mod2 = crear_conexion(
        logger,
        "SERVER MOD2",
        ip,
        puerto
    );

    return *fd_mod2 != 0;
}

void cerrar_programa(t_log* logger,t_config* config) {
    log_destroy(logger);
    config_destroy(config);
}
