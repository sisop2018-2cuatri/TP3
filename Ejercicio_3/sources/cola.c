#include "../headers/cola.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nodo
{
    t_dato info;
    struct nodo *sig;
};

struct nodo *raiz = NULL;
struct nodo *fondo = NULL;

int vacia()
{
    if (raiz == NULL)
        return 1;
    else
        return 0;
}

void insertar(t_dato x)
{
    struct nodo *nuevo;
    nuevo = malloc(sizeof(struct nodo));
    nuevo->info = x;
    nuevo->sig = NULL;
    if (vacia())
    {
        raiz = nuevo;
        fondo = nuevo;
    }
    else
    {
        fondo->sig = nuevo;
        fondo = nuevo;
    }
}

int extraer(t_dato *informacion)
{
    if (!vacia())
    {
        *informacion = raiz->info;
        struct nodo *bor = raiz;
        if (raiz == fondo)
        {
            raiz = NULL;
            fondo = NULL;
        }
        else
        {
            raiz = raiz->sig;
        }
        free(bor);
        return 1;
    }
    else
        return -1;
}

void imprimir()
{
    struct nodo *reco = raiz;
    printf("Entradas registradas\n");
    while (reco != NULL)
    {
        mostrar(reco->info);
        reco = reco->sig;
    }
    printf("\n");
}

void mostrar(t_dato x)
{
    printf("[%s] [%s] [%d:%d]",
           DIRECCION(x.dir),
           x.patente,
           x.hora,
           x.minuto);

    if (x.dir == 'S' || x.dir == 's')
    {
        printf(" [%.2f]", x.precio);
    }

    printf("\n");
}

void vaciar_cola()
{
    struct nodo *reco = raiz;
    struct nodo *bor;
    while (reco != NULL)
    {
        bor = reco;
        reco = reco->sig;
        free(bor);
    }
}

int anterior(char *patente, t_dato *x)
{
    x->patente[0] = '\0';

    struct nodo *reco = raiz;
    while (reco != NULL)
    {
        if (strcmp(reco->info.patente, patente) == 0)
        {
            *x = reco->info;
        }

        reco = reco->sig;
    }

    // 1: si hay un registro anterior, 0 si no
    return x->patente[0] != '\0';
}

int siguiente(char *patente, t_dato *x)
{
    x->patente[0] = '\0';

    struct nodo *reco = raiz;
    while (reco != NULL)
    {
        if (strcmp(reco->info.patente, patente) == 0)
        {
            *x = reco->info;

            // registro siguiente encontrado
            return 1;
        }

        reco = reco->sig;
    }

    // no hay registro siguiente
    return 0;
}