/*
    Inicializar conexión
    --------------------
    - crea un socket
    - ejecuta bind
    - ejecuta listen
    Parámetros:
        modo_ejecucion: modo de ejecución de la conexión
        puerto: puerto que utilizará el socket
        cantidad_clientes_maxima: cantidad máxima de clientes simultáneos
    Retorna:
        1: conexión inicializada
        2: ERROR: no se pudo crear el socket
        3: ERROR: no se pudo ejecutar bind
        4: ERROR: no se pudo ejecutar listen
*/
int inicializar_conexion(int modo_ejecucion, int puerto, int cantidad_clientes_maxima);

/*
    Comienza a atender solicitudes de los clientes
    ----------------------------------------------
    manejador de conexiones de clientes
    la concurrencia de conexiones se maneja con threads 
    Salida:
        muestra en pantalla los mensajes de las solicitudes   
*/
void atender_solicitudes();