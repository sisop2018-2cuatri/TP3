#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../headers/conexion.h"
#include "../headers/configuracion.h"

int socket_id = -1;          // socket de conexión
struct sockaddr_in servidor; // estructura del socket

/*
    ver conexion.h
*/
void inicializar_conexion(char *materia, char *servidor_direccion, int socket_puerto, int recordar)
{
    int estado = -1;     // estado de la conexión
    char str_puerto[12]; // valor del puerto como string

    // cerrar la conexión por si ya fue inicializada previamente
    finalizar_conexion();

    while (estado != 1)
    {
        printf("\n");

        // si la materia está vacia ú ocurrió un error de conexión 4
        if (strlen(materia) == 0 || estado != -1)
        {
            printf("Ingrese la materia a la que pertenece: ");
            fflush(stdin);
            scanf("%s", materia);
        }

        // si la dirección es vacía ú ocurrió un error de conexión
        if (strlen(servidor_direccion) == 0 || estado != -1)
        {
            printf("Ingrese la dirección del servidor: ");
            fflush(stdin);
            scanf("%s", servidor_direccion);
        }

        // si el puerto es incorrecto ú ocurrió un error de conexión
        if (socket_puerto <= 1024 || socket_puerto > 65535 || estado != -1)
        {
            do
            {
                printf("Ingrese el puerto de conexión: ");
                fflush(stdin);
                scanf("%d", &socket_puerto);

                if (socket_puerto <= 1024 || socket_puerto > 65535)
                {
                    printf("\nERROR: el puerto debe ser mayor a 1024 y menor a 65536\n");
                }
            } while (socket_puerto <= 1024 || socket_puerto > 65535);
        }

        // si al finalizar es distinto de 1, ocurrió un error
        estado = 1;

        // crear socket: protocolo IPv4, TCP
        socket_id = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_id == -1)
        {
            printf("ERROR: no se pudo crear el socket\n");
            estado = 2;
        }

        // configurar socket
        servidor.sin_addr.s_addr = inet_addr(servidor_direccion);
        servidor.sin_family = AF_INET;
        servidor.sin_port = htons(socket_puerto);
        bzero(&(servidor.sin_zero), 8); // completar con ceros

        // conectar con el servidor
        if (connect(socket_id, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
        {
            printf("ERROR: no se pudo conectar a [%s:%d]\n", servidor_direccion, socket_puerto);
            estado = 3;
        }
    }

    // si hay que recordar la última conexión
    if (recordar)
    {
        // guardar variables de conexión en la configuración para el próximo inicio
        modificar_variable("SERVER_DIRECCION", servidor_direccion);
        sprintf(str_puerto, "%d", socket_puerto);
        modificar_variable("SERVER_PUERTO", str_puerto);
        modificar_variable("MATERIA_PROFESOR", materia);
    }

    printf("conectado a [%s:%d]\n", servidor_direccion, socket_puerto);
    printf("materia [%s]\n", materia);
}

/*
    ver conexion.h
*/
void finalizar_conexion(void)
{
    // si el socket esta en uso
    if (socket_id != -1)
    {
        // cerrar el socket
        close(socket_id);
    }
}

/*
    ver conexion.h
*/
int enviar_mensaje(char *mensaje, char *respuesta)
{
    int estado_send;          // estado de la funcion send
    int tam_respuesta_server; // tamaño de la respuesta del server

    // enviar solicitud al servidor
    // MSG_NOSIGNAL: mantener el proceso si se pierde la conexión
    estado_send = send(socket_id, mensaje, strlen(mensaje), MSG_NOSIGNAL);
    if (estado_send < 0)
    {
        respuesta[0] = '\0';
        return 2; // ERROR: al enviar
    }

    // respuesta del servidor
    tam_respuesta_server = recv(socket_id, respuesta, 1024, 0);
    if (tam_respuesta_server < 0)
    {
        respuesta[0] = '\0';
        return 3; // ERROR: al recibir
    }

    // ajustar tamaño del buffer de respuesta, al de la respuesta real
    respuesta[tam_respuesta_server] = '\0';

    return 1; // el servidor respondió algo
}