#include "include/main.h"

int main() {

    t_log* logger = log_create("mod1.log", "ClienteLogs", true, LOG_LEVEL_INFO);
	 t_config* config = iniciar_config();

    int fd_server = 0;
    if (!generar_conexiones(logger,config,&fd_server)){
        cerrar_programa(logger,config);
        return EXIT_FAILURE;
    }

    int a;
    scanf("%d", &a);
    enviar_mensaje("Se envia esto?",fd_server);

    //Queremos enviar el valor de CLAVE DE LA CONFIG por red

    char* clave = config_get_string_value(config,"CLAVE");

    //Por las dudas lo loogeamos para ver que se haya leido bien
    log_info(logger,"De la config lei el valor CLAVE: %s",clave);

    scanf("%d", &a);
    enviar_mensaje(clave,fd_server);

    scanf("%d", &a);
    send_mirar_netflix(fd_server, "Inception", 14);

    //scanf("%d", &a);
    //send_aprobar_operativos(fd_server, 7, 8);

    scanf("%d", &a);
    cerrar_programa(logger,config);
    return EXIT_SUCCESS;
}

//NOS QUEDA PENDIENTE LA PARTE DE GUARDAR LAS COSAS INGRESADAS POR CONSOLA, AGREGARLAS AL PAQUETE Y MANDARLA
//EL PROBLEMA LO TENES QUE NO SABES COMO GUARDAR LAS COSAS INGRESADAS POR CONSOLA.

