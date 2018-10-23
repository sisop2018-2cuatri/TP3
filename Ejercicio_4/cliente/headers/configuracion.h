/*
    CONFIGURACIÓN DEFAULT
    ---------------------
*/
#define MODO_EJEC ((int)1)
#define MOSTRAR_AYUDA ((int)1)
#define SERVER_IP ((char *)"127.0.0.1")
#define SERVER_PUERTO ((int)55444)
#define RECORDAR_ULTIMA_CONEXION ((int)1)
#define MATERIA_PROFESOR ((char *)"")

enum MODO_EJECUCION
{
    RELEASE = 0,
    DEBUG = 1
};

typedef struct Configuracion
{
    enum MODO_EJECUCION modo_ejecucion; // un valor de MODO_EJECUCION
    int mostrar_ayuda;                  // mostrar ayuda al iniciar
    char server_direccion[1000];        // IP del servidor
    int socket_puerto;                  // Puerto del socket
    int recordar_ultima_conexion;       // si se conecta recuerda IP y puerto
    char materia_profesor[1000];        // materia dictada por el profesor
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

/*
    Modifica el valor de una variable
    ---------------------------------
    itera sobre el archivo hasta encontrar la variable para modificar,
    si la encuentra la modifica.
    Parametros:
        nombre_variable: nombre de la variable que se desea modificar.
        valor_nuevo: valor que se le debe poner a la variable.
    Retorna:
        1: si se pudo modificar
        2: ERROR: no se pudo crear el archivo temporal
        3: ERROR: archivo de configuracion no encontrado
        4: ERROR: no se pudo actualizar el archivo
        5: variable no encontrada
*/
int modificar_variable(char *nombre_variable, char *valor_nuevo);