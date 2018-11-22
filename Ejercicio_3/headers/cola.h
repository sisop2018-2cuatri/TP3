#define DIRECCION(x) ((x == 'E' || x == 'e') ? "Entrada" : "Salida")

typedef struct dato
{
    char dir;        // E: entrada, S: salida
    char patente[8]; // ej: AA000AA\0
    int hora;        // hora del evento
    int minuto;      // minuto del evento
    float precio;    // precio por hora
} t_dato;

void insertar(t_dato x);
void mostrar(t_dato x);
int extraer(t_dato *informacion);
void imprimir();
void vaciar_cola();
int vacia();
int anterior(char *patente, t_dato *x);
int siguiente(char *patente, t_dato *x);