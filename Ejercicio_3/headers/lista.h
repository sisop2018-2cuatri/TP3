typedef struct registro
{
    char patente[8]; // ej: AA000AA\0
    float cobrado;   // cobrado por estadia
} t_registro;

typedef struct _nodo
{
    t_registro registro;
    struct _nodo *siguiente;
    struct _nodo *anterior;
} tipoNodo;

typedef tipoNodo *pNodo;
typedef tipoNodo *Lista;

int lista_insertar(t_registro registro);
void lista_mostrar();
void lista_guardar(FILE *f_factura);
void vaciar_lista();