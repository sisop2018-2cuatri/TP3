#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/conexion.h"
#include "../headers/configuracion.h"
#include "../headers/notas.h"

int socket_id;               // socket servidor
struct sockaddr_in servidor; // configuración del socket servidor
int G_MODO_EJECUCION;        // modo de ejecución del server

/*
    Procesa la solicitud recibida del cliente
    -----------------------------------------
    Define que acciones tomar en el servidor según la 
    solicitud recibida desde el cliente.
    Parámetros:
        s_cliente: structura de conexión con el cliente
        cliente_socket: socket a traves del cuál se recibe la solicitud
        solicitud: mensaje recibido desde el cliente
    Salida:
        Un valor según el origen de la solicitud, el cuál
        debe ser interpretado por el cliente.
*/
void procesar_solicitud(struct sockaddr_in s_cliente, int cliente_socket, char *solicitud);

/*
    Parsea una solicitud
    --------------------
    en base a la solicitud obtiene el método que se debe ejecutar
    y sus respectivos parámetros de ejecución.
    Parámetros:
        s: solicitud que debe ser parseada
        m: método que fue solicitado para ejecutar en el servidor
        p1-4: parámetro del 1 al 4, según el método
    Retorna:
        mayor igual a 0: cantidad de parámetros encontrados (pueden ser cero)
        -1: si no se encuntra el método
*/
int parserar_solicitud(char *s, char *m, char *p1, char *p2, char *p3, char *p4);

/*
    ver conexion.c
*/
int parserar_solicitud(char *s, char *m, char *p1, char *p2, char *p3, char *p4)
{
    int cantidad_parametros = 0; // cantidad de parámetros encontrados
    char *p_aux = s;             // puntero auxiliar
    int i = 0;                   // indice auxiliar

    m[0] = '\0';  // inicilizar método
    p1[0] = '\0'; // inicilizar parámetro 1
    p2[0] = '\0'; // inicilizar parámetro 2
    p3[0] = '\0'; // inicilizar parámetro 3
    p4[0] = '\0'; // inicilizar parámetro 4

    // obtener método
    while (*p_aux != '\0' && *p_aux != '\n' && *p_aux != ',')
    {
        m[i] = *p_aux;
        p_aux++;
        i++;
    }
    m[i] = '\0';

    // si no hay método para ejecutar en la solicitud
    if (strlen(m) == 0)
    {
        return -1; // método no encontrado
    }

    // obtener parámetros del uno al cuatro
    while (*p_aux != '\0' && *p_aux != '\n')
    {
        // si encontramos un nuevo parámetro
        // podemos almacenar hasta 4 parámetros
        if (*p_aux == ',' && cantidad_parametros < 4)
        {
            p_aux++;
            i = 0;
            while (*p_aux != '\0' && *p_aux != '\n' && *p_aux != ',')
            {
                // según la cantidad de parámetros parseados
                // sabemos cuál estamos parseando
                switch (cantidad_parametros)
                {
                case 0:
                    p1[i] = *p_aux;
                    break;
                case 1:
                    p2[i] = *p_aux;
                    break;
                case 2:
                    p3[i] = *p_aux;
                    break;
                case 3:
                    p4[i] = *p_aux;
                    break;
                }
                p_aux++;
                i++;
            }
            // según la cantidad de parámetros parseados
            // sabemos cuál estamos parseando
            switch (cantidad_parametros)
            {
            case 0:
                p1[i] = '\0';
                break;
            case 1:
                p2[i] = '\0';
                break;
            case 2:
                p3[i] = '\0';
                break;
            case 3:
                p4[i] = '\0';
                break;
            }

            // contabilizar parámetro parseado
            cantidad_parametros++;
        }
        else
        {
            break; // no hay más parámetros para parsear
        }
    }

    return cantidad_parametros;
}

/*
    ver conexion.c
*/
void procesar_solicitud(struct sockaddr_in s_cliente, int cliente_socket, char *solicitud)
{
    char metodo[100];        // acción para ejecutar en el servidor
    char parametro_1[231];   // parámetro 1, varia según la solicitud
    char parametro_2[231];   // parámetro 2, varia según la solicitud
    char parametro_3[231];   // parámetro 3, varia según la solicitud
    char parametro_4[231];   // parámetro 4, varia según la solicitud
    char respuesta[1024];    // respuesta para emitir al cliente
    int cantidad_parametros; // cantidad de parámetros parseados
    int validar_parametros;  // la cantidad de parámetros parseados es correcta

    respuesta[0] = '\0';    // inicializar respuesta
    validar_parametros = 0; // vale uno si están todos los parámetros necesarios

    // parsear solicitud
    cantidad_parametros = parserar_solicitud(solicitud,
                                             metodo,
                                             parametro_1,
                                             parametro_2,
                                             parametro_3,
                                             parametro_4);
    if (cantidad_parametros == -1)
    {
        printf("[%s] ERROR: solicitud sin método\n", inet_ntoa(s_cliente.sin_addr));
    }
    else
    {
        if (G_MODO_EJECUCION == DEBUG)
        {
            printf("[%s] método [%s]\n",
                   inet_ntoa(s_cliente.sin_addr),
                   metodo);
            printf("[%s] parámetros [%s] [%s] [%s] [%s]\n",
                   inet_ntoa(s_cliente.sin_addr),
                   parametro_1,
                   parametro_2,
                   parametro_3,
                   parametro_4);
        }

        // ejecución del método solicitado
        // -------------------------------

        if (strcmp(metodo, "get_promedio_general") == 0)
        {
            // parametro_1 == DNI
            if (cantidad_parametros == 1)
            {
                // obtener promedio general según el dni
                sprintf(respuesta, "%.2f", get_promedio_general(atoi(parametro_1)));

                if (G_MODO_EJECUCION == DEBUG)
                {
                    printf("[%s] promedio general de [%s] es [%s]\n",
                           inet_ntoa(s_cliente.sin_addr),
                           parametro_1,
                           respuesta);
                }

                // están todos los parámetros necesarios
                validar_parametros = 1;
            }
        }
        else if (strcmp(metodo, "get_promedio") == 0)
        {
            // parametro_1 == DNI
            // parametro_2 == MATERIA
            if (cantidad_parametros == 2)
            {
                // obtener promedio de la materia según el dni
                sprintf(respuesta, "%.2f", get_promedio(atoi(parametro_1), parametro_2));

                if (G_MODO_EJECUCION == DEBUG)
                {
                    printf("[%s] promedio de [%s] en materia [%s] es [%s]\n",
                           inet_ntoa(s_cliente.sin_addr),
                           parametro_1,
                           parametro_2,
                           respuesta);
                }

                // están todos los parámetros necesarios
                validar_parametros = 1;
            }
        }
        else if (strcmp(metodo, "cargar_nota") == 0)
        {
            // parametro_1 == DNI
            // parametro_2 == MATERIA
            // parametro_3 == TIPO_EVALUACION
            // parametro_4 == NOTA
            if (cantidad_parametros == 4)
            {
                // cargar nota
                sprintf(respuesta,
                        "%d",
                        cargar_nota(atoi(parametro_1),
                                    parametro_2,
                                    atoi(parametro_3),
                                    atof(parametro_4)));

                if (G_MODO_EJECUCION == DEBUG)
                {
                    printf("[%s] cargar nota a dni [%s] materia [%s] evaluación [%s] nota [%s] estado [%s]\n",
                           inet_ntoa(s_cliente.sin_addr),
                           parametro_1,
                           parametro_2,
                           parametro_3,
                           parametro_4,
                           respuesta);
                }

                // están todos los parámetros necesarios
                validar_parametros = 1;
            }
        }

        // si no se recibieron todos los parametros necesarios
        if (!validar_parametros)
        {
            if (G_MODO_EJECUCION == DEBUG)
            {
                printf("[%s] ERROR: no se pudo ejecutar [%s] faltan parámetros\n",
                       inet_ntoa(s_cliente.sin_addr),
                       metodo);
            }
        }
    }

    // enviar mensaje de respuesta al cliente
    write(cliente_socket, respuesta, strlen(respuesta));
}

/*
    ver conexion.h
*/
int inicializar_conexion(int modo_ejecucion, int puerto, int cantidad_clientes_maxima)
{
    // crear socket: protocolo IPv4, TCP
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_id == -1)
    {
        return 2; // ERROR: no se pudo crear el socket
    }

    // peraparar la structura sockaddr_in del servidor
    servidor.sin_family = AF_INET;         // protocolo V4
    servidor.sin_addr.s_addr = INADDR_ANY; // IP automática
    servidor.sin_port = htons(puerto);     // puerto de red
    bzero(&(servidor.sin_zero), 8);        // completar con ceros

    // asignar socket al puerto del sistema
    if (bind(socket_id, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        return 3; // ERROR: al ejecutar bind
    }

    // socket en modo escucha
    if (listen(socket_id, cantidad_clientes_maxima) < 0)
    {
        return 4; // ERROR: al ejecutar listen
    }

    // inicializar modo de ejecución
    G_MODO_EJECUCION = modo_ejecucion;

    // Mostrar datos de conexión
    printf("\nServidor IP [%s] PUERTO [%d]\n", inet_ntoa(servidor.sin_addr), puerto);

    return 1; // conexión inicializada correctamente
}

/*
    ver conexion.h
*/
void atender_solicitudes()
{
    int cliente_socket;           // cliente socket
    int tam_cliente_struct;       // tamaño de la structura cliente
    int tam_mensaje;              // tamaño del mensaje del cliente
    struct sockaddr_in s_cliente; // configuración cliente
    char cliente_mensaje[1024];   // longitud del mensaje cliente

    // atender solicitudes indefinidamente
    while (1)
    {
        // TODO: usar un hilo por cliente
        // tamaño de la estructura de configuración del cliente
        tam_cliente_struct = sizeof(struct sockaddr_in);

        // aceptar conexión del cliente
        cliente_socket = accept(socket_id, (struct sockaddr *)&s_cliente, (socklen_t *)&tam_cliente_struct);
        if (cliente_socket < 0)
        {
            printf("ERROR: al conectar con [%s]\n", inet_ntoa(s_cliente.sin_addr));
        }
        else
        {
            printf("conexión [%s] aceptada\n", inet_ntoa(s_cliente.sin_addr));
            // mensajes de entrada desde el cliente
            while ((tam_mensaje = recv(cliente_socket, cliente_mensaje, 1024, 0)) > 0)
            {
                // descartar todo lo que no es el mensaje del cliente
                cliente_mensaje[tam_mensaje] = '\0';

                if (G_MODO_EJECUCION == DEBUG)
                {
                    printf("[%s] solicita [%s]\n", inet_ntoa(s_cliente.sin_addr), cliente_mensaje);
                }

                // ejecutar una función según la solicitud del cliente
                procesar_solicitud(s_cliente, cliente_socket, cliente_mensaje);
            }

            if (tam_mensaje == 0)
            {
                // TODO: atender
                // cliente desconectado
            }
            else if (tam_mensaje == -1)
            {
                // TODO: atender
                // error en la conexión con el cliente
            }

            // conexión cliente terminada
            close(cliente_socket);
            printf("conexión [%s] terminada\n", inet_ntoa(s_cliente.sin_addr));
        }
    }
}

/*
    ver conexion.h
*/
void finalizar_conexion(void)
{
    // si el socket esta en uso
    if (socket_id != -1)
    {
        // TODO: cerrar todas las conexiones cliente abiertas

        // cerrar el socket
        close(socket_id);

        printf("conexión finalizada\n");
    }
}