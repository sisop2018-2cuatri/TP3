/*
    Mostrar ayuda en pantalla
*/
void mostrar_ayuda(void);

/*
    Muestra el menú de opciones
*/
void mostrar_menu(void);

/*
    Indica al sistema que vamos a manejar las señales
    de fin de programa que sean recibidas
*/
void atrapar_seniales(void);

/*
    Adminsitra la señal de fin de ejecución
    al ser recibida
*/
void fin_ejecucion(int senial);

/*
    Informar al usuario como se calcula una materia rendida
*/
void mensaje_materia_rendida(void);

//  ====================================
//          FUNCIONES DEL MENÚ
//  ====================================

/*
    Cargar nueva nota para un alumno
    --------------------------------
    Solicita al usuario los datos para cargar
    una nueva nota a un alumno.
    Salida:
        se muestra en pantalla un estado
        según la respuesta del servidor o en caso de fallo
        se mostrará un mensaje de error
*/
void cargar_nueva_nota(void);

/*
    Obtener promedio del alumno en la materia
    -----------------------------------------
    Consulta en el servidor el promedio del alumno
    en la materia con la cual se registro el usuario
    en la sesión acual.
    Salida:
        se muestra en pantalla el promedio del alumno
        en la materia, o un mensaje de error en caso de fallo
*/
void obtener_promedio_materia(void);

/*
    Obtener promedio general del alumno
    -----------------------------------
    Servidor retorna (char[1024]):
        -1: ocurrió un error obteniendo el promedio general
        0: el alumno no rindió ninguna materia
        mayor a cero: promedio general del alumno
*/
void obtener_promedio_general(void);

/*
    Cierra la conexión actual y 
    solicita al usuario nuevos parámetros de conexión
*/
void cambiar_la_conexion(void);

/*
    Elimina los datos de la conexión actual
    modificando el archivo de configuración
*/
void borrar_datos_conexion(void);