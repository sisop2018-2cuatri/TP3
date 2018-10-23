/*
    CONFIGURACIÓN DEFAULT
    ---------------------
*/
#define MODO_EJEC ((int)1)
#define PATH_NOTAS ((char *)"./inputs/notas.txt")
#define MOSTRAR_AYUDA ((int)1)
#define SOCKET_PUERTO ((int)55444)
#define CANT_CLI_MAX ((int)10)

enum MODO_EJECUCION
{
    RELEASE = 0,
    DEBUG = 1
};

typedef struct Configuracion
{
    enum MODO_EJECUCION modo_ejecucion; // un valor de MODO_EJECUCION
    char path_notas[1000];              // path del archivo de notas
    int mostrar_ayuda;                  // mostrar ayuda al iniciar
    int socket_puerto;                  // puerto que utiliza el socket
    int cantidad_clientes_maxima;       // máxima cantidad de clientes en simultáneo
} Configuracion;

/*
    Obtener valores de configuración
    --------------------------------
    En caso de existir utilizará el archivo de entrada:
        - nombre archivo: "./inputs/configuracion.txt"
    Si el archivo no existe se crerá en la primera ejecución,
    siempre y cuando sea posible crearlo. Así mismo el archivo
    será inicializado con los valores default.
*/
void get_configuracion(Configuracion *configuracion);

/*
    Mostrar en pantalla los valores cargados
    ----------------------------------------
    No se debe llamar antes de get_configuracion
*/
void mostrar_configuracion(Configuracion *configuracion);