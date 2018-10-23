/*
    Inicializar conexión
    --------------------
    - crea un socket
    - ejecuta connect
    En caso de error solicitará otros parámetros de conexión
    Parámetros:
        materia: nombre de la materia del profesor
        servidor_direccion: dirección del servidor, no puede estar vacía
        socket_puerto: puerto del socket, mayor a 1024 y menor a 65536
        recordar: si es TRUE al conectar guarda la dirección y el puerto
            para utilizarlos la próxima vez
*/
void inicializar_conexion(char *materia, char *servidor_direccion, int socket_puerto, int recordar);

/*
    Finalizar la conexión con el server
    -----------------------------------
    finaliza la conexión cerrando el socket, si la conexión
    estaba cerrada, no hace nada.
*/
void finalizar_conexion(void);

/*
    Enviar mensaje al servidor
    --------------------------
    Parametros:
        mensaje: mensaje que se envia al servidor
        respuesta: almancena la respuesta del servidor
    Retorna:
        1: el servidor responde la solicitud
        2: ERROR: al enviar
        3: ERROR: al recibir
*/
int enviar_mensaje(char *mensaje, char *respuesta);