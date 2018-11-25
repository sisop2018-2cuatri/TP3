#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "../headers/cola.h"
#include "../headers/lista.h"

#define ABS(x) ((x < 0) ? (-(x)) : (x))
#define FIFO_CAMARAS "/tmp/camaras"
#define FIFO_CAM_TAM ((128))

int f_camaras;
char log_name[1000];
char hoy[32];
float precio;

char *getFecha(char *hoy);
void getHora(char *msg, int *hora, int *minuto);
void generarLogDia();
void atrapar_seniales();
void facturar(int senial);
void proceso_facturador();

int main()
{
    FILE *f_log;
    t_dato entrada;
    t_dato r_anterior;
    char camara_msg[FIFO_CAM_TAM];
    int c_buffer; // para limpiar el buffer del teclado
    int i;

    printf("***************************************\n");
    printf("| Ejemplos de uso de FIFO:            |\n");
    printf("| - echo EAAA111 >> \"/tmp/camaras\"    |\n");
    printf("| - echo SAAA111 >> \"/tmp/camaras\"    |\n");
    printf("| ó puede ejecutar \"./camara.sh\"      |\n");
    printf("|                                     |\n");
    printf("| Para facturar el día use \"Ctrl + C\" |\n");
    printf("***************************************\n\n");

    printf("ingrese precio por hora: ");
    fflush(stdin);
    scanf("%f", &precio);

    // crear archivo de log
    generarLogDia();

    // atrapar señal de fin de ejecución
    atrapar_seniales();

    mkfifo(FIFO_CAMARAS, 0666);

    printf("leyendo cámaras (FIFO: \"/tmp/camaras\")\n");
    printf("--------------------------------------\n");
    while (1)
    {
        // limpiar buffer container
        for (i = 0; i < FIFO_CAM_TAM; i++)
            camara_msg[i] = '\0';

        // leer FIFO camaras
        f_camaras = open(FIFO_CAMARAS, O_RDONLY);
        read(f_camaras, camara_msg, FIFO_CAM_TAM);
        close(f_camaras);
        camara_msg[strcspn(camara_msg, "\r\n")] = 0;

        // solo se procesan entradas no vacias
        if (strlen(camara_msg) == 0)
            continue;

        // guardar datos cámara en memoria
        getHora(camara_msg, &entrada.hora, &entrada.minuto);
        entrada.dir = camara_msg[0];
        strcpy(entrada.patente, camara_msg + 1);

        printf("cámara [%s]\n", camara_msg);

        // si es una entrada y
        // si hay un registro anterior para esta patente
        if ((entrada.dir == 'E' || entrada.dir == 'e') &&
            anterior(entrada.patente, &r_anterior))
        {
            // si el anterior fue también una entrada
            if (r_anterior.dir == 'E' || r_anterior.dir == 'e')
            {
                printf("[%s] ya registra una entrada\n", entrada.patente);
                printf("--------------------------------------\n");

                // nueva entrada ignorada
                continue;
            }
        }
        else if (entrada.dir == 'S' || entrada.dir == 's')
        {
            // si hay un registro anterior debe ser una entrada
            if (anterior(entrada.patente, &r_anterior))
            {
                // si el anterior fue también una salida
                if (r_anterior.dir == 'S' || r_anterior.dir == 's')
                {
                    printf("[%s] ya registra una salida\n", entrada.patente);
                    printf("--------------------------------------\n");

                    // nueva salida ignorada
                    continue;
                }
            }
        }

        insertar(entrada);
        mostrar(entrada);
        printf("--------------------------------------\n");

        // agregar registro al log del día
        f_log = fopen(log_name, "a");
        fprintf(f_log, "%s ", DIRECCION(entrada.dir));
        if (entrada.dir == 'S' || entrada.dir == 's')
        {
            fprintf(f_log, " ");
        }
        fprintf(f_log, "%s ", entrada.patente);
        if (strlen(entrada.patente) < 7)
        {
            fprintf(f_log, " ");
        }
        fprintf(f_log, "%02d:%02d\n", entrada.hora, entrada.minuto);
        fclose(f_log);
    }

    return 0;
}

char *getFecha(char *hoy)
{
    struct tm *to;
    time_t t;
    t = time(NULL);
    to = localtime(&t);
    strftime(hoy, 32, "%Y%m%d", to);

    return hoy;
}

void getHora(char *msg, int *hora, int *minuto)
{
    struct tm *to;
    time_t t;
    char *dots;
    int dots_index;
    char str_hora[3];
    char str_minuto[3];

    dots = strchr(msg, ':');
    if (dots != NULL && dots > msg)
    {
        // el mensaje contiene fecha hora, los usamos
        dots_index = dots - msg;
        str_hora[0] = msg[dots_index - 2];
        str_hora[1] = msg[dots_index - 1];
        str_hora[2] = '\0';
        str_minuto[0] = msg[dots_index + 1];
        str_minuto[1] = msg[dots_index + 2];
        str_minuto[2] = '\0';
        *hora = atoi(str_hora);
        *minuto = atoi(str_minuto);
        msg[dots_index - 3] = '\0';
    }
    else
    {
        // el mensaje no contiene HH:mm usamos los del servidor
        t = time(NULL);
        to = localtime(&t);
        *hora = to->tm_hour;
        *minuto = to->tm_min;
    }
}

void generarLogDia()
{
    FILE *f_log;

    log_name[0] = '\0';
    getFecha(hoy);

    strcpy(log_name, "./outputs/Movimientos_");
    strcat(log_name, hoy);
    strcat(log_name, ".txt");

    f_log = fopen(log_name, "w");

    if (f_log == NULL)
    {
        printf("ERROR: al generar el archivo de log [%s]\n", log_name);
        exit(1);
    }
    fclose(f_log);
    printf("log [%s] creado\n", log_name);
}

void atrapar_seniales()
{

    if (signal(SIGTERM, facturar) == SIG_ERR)
    {
        printf("ADVERTENCIA: no se podrá procesar SIGTERM\n");
        printf("evite utilizar esta señal\n");
    }
    if (signal(SIGINT, facturar) == SIG_ERR)
    {
        printf("ADVERTENCIA: no se podrá procesar SIGINT\n");
        printf("evite utilizar esta señal\n");
    }

    // SIGKILL y SIGSTOP nunca pueden atraparse
}

void facturar(int senial)
{
    printf("\n");

    close(f_camaras);
    printf("fifo cámaras cerrado\n");

    proceso_facturador();

    // rutina de cerrado correcto
    vaciar_cola();
    vaciar_lista();
    printf("memoria liberada\n");

    exit(0);
}

void proceso_facturador()
{
    FILE *f_factura;
    t_dato dato;
    t_dato r_siguiente;
    t_registro registro;
    char factura_name[1000];
    float cobrado;
    float total = 0;
    int minutos;

    strcpy(factura_name, "./outputs/Facturacion_");
    strcat(factura_name, hoy);
    strcat(factura_name, ".txt");

    f_factura = fopen(factura_name, "w");
    if (f_factura == NULL)
    {
        printf("ERROR: al generar el archivo de facturación [%s]\n", factura_name);
        exit(1);
    }

    while (!vacia())
    {
        extraer(&dato);

        // ignorar si es salida
        if (dato.dir == 'S' || dato.dir == 's')
            continue;

        // si es una entrada, buscar la fecha de salida
        if (siguiente(dato.patente, &r_siguiente))
        {
            // si hay siguiente, ver si es una salida
            if (r_siguiente.dir == 'S' || r_siguiente.dir == 's')
            {
                // calcular cobrado (precio por hora, fracción cada 15 minutos)
                cobrado = (r_siguiente.hora - dato.hora) * precio;

                // fracción cada 15 min
                minutos = ABS(r_siguiente.minuto - dato.minuto);
                while (minutos - 15 > 0)
                {
                    minutos -= 15;
                    // precio cada 15 minutos
                    cobrado += precio / 4;
                }
                if (minutos > 0)
                    // precio cada 15 minutos
                    cobrado += precio / 4;

                // si la patente ya existía en la lista
                // se encarga de sumar lo "cobrado" para manter
                // un único registro por patente
                strcpy(registro.patente, dato.patente);
                registro.cobrado = cobrado;
                lista_insertar(registro);

                total += cobrado;
            }
        }
        else
        {
            // si no hay siguiente sigue en el garaje
            fprintf(f_factura, "%s", dato.patente);
            if (strlen(dato.patente) < 7)
            {
                fprintf(f_factura, " ");
            }
            fprintf(f_factura, " \"Aún en el garaje\"\n");
        }
    }

    // guardar la lista
    lista_guardar(f_factura);

    fclose(f_factura);
    printf("facturación [%s]\n", factura_name);

    // mostrar total del día
    printf("facturación total del día [%.2f]\n", total);
}