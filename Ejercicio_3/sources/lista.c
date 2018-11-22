#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../headers/lista.h"

// listado de registros en memoria
Lista l_factura = NULL;

int cmp_patente(char *m1, char *m2)
{
    const unsigned char *p1 = (const unsigned char *)m1;
    const unsigned char *p2 = (const unsigned char *)m2;
    int result;

    if (p1 == p2)
        return 0;

    // casteamos tolower a char para que compare bien los caracteres con tilde
    while ((result = (char)tolower(*p1) - (char)tolower(*p2++)) == 0)
        if (*p1++ == '\0')
            break;

    return result;
}

int comparar(t_registro a, t_registro b)
{
    if (cmp_patente(a.patente, b.patente) > 0)
    {
        return 1;
    }
    else if (cmp_patente(a.patente, b.patente) < 0)
    {
        return -1;
    }

    return 0; // iguales
}

int lista_insertar(t_registro registro)
{
    pNodo nuevo, actual;

    // crear un nuevo nodo
    nuevo = (pNodo)malloc(sizeof(tipoNodo));
    nuevo->registro = registro;

    // colocamos actual en la primera posición de la lista
    actual = l_factura;
    if (actual)
        while (actual->anterior)
            actual = actual->anterior;
    // si la lista está vacía o el primer miembro es mayor que el nuevo
    if (!actual || comparar(actual->registro, registro) > 0)
    {
        // añadimos la lista a continuación del nuevo nodo
        nuevo->siguiente = actual;
        nuevo->anterior = NULL;
        if (actual)
            actual->anterior = nuevo;
        if (!l_factura)
            l_factura = nuevo;
    }
    else
    {
        // avanzamos hasta el último elemento o hasta que el siguiente tenga
        // un valor mayor que el nuevo
        while (actual->siguiente && comparar(actual->siguiente->registro, registro) < 0)
            actual = actual->siguiente;

        // si el actual o el siguiente son iguales al nuevo
        if (comparar(actual->registro, registro) == 0 ||
            (actual->siguiente && comparar(actual->siguiente->registro, registro) == 0))
        {
            free(nuevo);

            // nodo repetido
            if (comparar(actual->registro, registro) == 0)
            {
                actual->registro.cobrado += registro.cobrado;
            }
            else if (actual->siguiente && comparar(actual->siguiente->registro, registro) == 0)
            {
                actual->siguiente->registro.cobrado += registro.cobrado;
            }

            return 1;
        }

        // insertamos el nuevo nodo después del nodo anterior
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
        nuevo->anterior = actual;
        if (nuevo->siguiente)
            nuevo->siguiente->anterior = nuevo;
    }

    return 1;
}

void lista_mostrar()
{
    Lista lista = l_factura;
    pNodo nodo = lista;

    if (!lista)
    {
        printf("listado vacío\n");
        return;
    }

    nodo = lista;
    while (nodo->anterior)
        nodo = nodo->anterior;
    printf("listado ascendente (patente/cobrado):\n");
    while (nodo)
    {
        printf("[%s] [%.2f]\n",
               nodo->registro.patente,
               nodo->registro.cobrado);

        nodo = nodo->siguiente;
    }
}

void lista_guardar(FILE *f_factura)
{
    Lista lista = l_factura;
    pNodo nodo = lista;

    if (!lista)
        return;

    nodo = lista;
    while (nodo->anterior)
        nodo = nodo->anterior;

    while (nodo)
    {
        fprintf(f_factura, "%s", nodo->registro.patente);
        if (strlen(nodo->registro.patente) < 7)
        {
            fprintf(f_factura, " ");
        }
        fprintf(f_factura, " $%.2f\n", nodo->registro.cobrado);

        nodo = nodo->siguiente;
    }
}

void vaciar_lista()
{
    pNodo nodo, actual;

    if (l_factura != NULL)
    {
        actual = l_factura;
        while (actual->anterior)
        {
            actual = actual->anterior;
        }

        while (actual)
        {
            nodo = actual;
            actual = actual->siguiente;
            free(nodo);
        }

        l_factura = NULL;
    }
}