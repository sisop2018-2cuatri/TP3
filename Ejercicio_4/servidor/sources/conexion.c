#include <stdio.h> // FIXME: usar en modo debugg
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
        cliente_socket: socket a traves del cuál se recibe la solicitud
        solicitud: mensaje recibido desde el cliente
    Output:
        Un valor según el origen de la solicitud, el cuál
        debe ser interpretado por el cliente.
*/
void procesar_solicitud(int cliente_socket, char *solicitud);

/*
    ver conexion.c
*/
void procesar_solicitud(int cliente_socket, char *solicitud)
{
    char metodo[100];        // acción para ejecutar en el servidor
    char parametro_uno[100]; // primer parametro, varia según la solicitud
    char *p_ini = solicitud; // puntero auxiliar
    int i;                   // indice auxiliar
    char respuesta[1024];    // respuesta para emitir al cliente

    respuesta[0] = '\0'; // inicializar respuesta
    metodo[0] = '\0';    // inicilizar metodo

    // leer que método fue solicitado
    i = 0;
    while (*p_ini != '\0' && *p_ini != '\n' && *p_ini != ',')
    {
        metodo[i] = *p_ini;
        p_ini++;
        i++;
    }
    metodo[i] = '\0';

    if (G_MODO_EJECUCION == DEBUG)
    {
        printf("ejecutar [%s] en el servidor\n", metodo);
    }

    // Ejecutar una acción según el método
    if (strcmp(metodo, "get_promedio_general") == 0)
    {
        // el primer parámetro debería ser el dni
        if (*p_ini == ',')
        {
            p_ini++;
            i = 0;
            while (*p_ini != '\0' && *p_ini != '\n')
            {
                parametro_uno[i] = *p_ini;
                p_ini++;
                i++;
            }
            parametro_uno[i] = '\0';

            // obtener promedio general según el dni
            sprintf(respuesta, "%.2f", get_promedio_general(atoi(parametro_uno)));
        }
        else
        {
            // ERROR: parámetro dni no encontrado
            sprintf(respuesta, "%s", "-1");
        }

        if (G_MODO_EJECUCION == DEBUG)
        {
            printf("el promedio general de [%s] es [%s]\n", parametro_uno, respuesta);
        }

        // enviar mensaje de respuesta al cliente
        write(cliente_socket, respuesta, strlen(respuesta));
    }
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
                    printf("recibe [%s] desde [%s]\n", cliente_mensaje, inet_ntoa(s_cliente.sin_addr));
                }

                // ejecutar una función según la solicitud del cliente
                procesar_solicitud(cliente_socket, cliente_mensaje);
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

    // TODO: ver como cerrar el socket del server
}