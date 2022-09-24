#include "include/main.h"

t_log* logger;
int fd_mod2;

void sighandler(int s) {
    cerrar_programa(logger);
    exit(0);
}

int main() {
    signal(SIGINT, sighandler);  //Esto lo uso para salir de la consola con CTRL + C

    logger = log_create("mod2.log", "ServidorLog", true, LOG_LEVEL_INFO);

    // No hardcodear IP y puerto, leer de config
    //En este caso esta bien hardcodeado pq en el tp0 no nos piden que lo levantemos de una config
    fd_mod2 = iniciar_servidor(logger, "SERVER", "0.0.0.0", "6969");
    while (server_escuchar(logger, "MOD2", fd_mod2));

    cerrar_programa(logger);

    return 0;
}
