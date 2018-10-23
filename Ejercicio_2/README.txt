Se necesita un proceso que analice archivos en directorios con mucha cantidad de archivos,
generando una estadística de los siguientes valores, agrupados por cada extensión de archivo
distinta (Ej, todos los .doc, los .mp3, los .c, etc.)
* Cantidad de archivos totales
* Por cada extensión
	o Cantidad de archivos
	o Tamaño del archivo más chico
	o Tamaño del archivo más grande
	o Usuario dueño de la mayor cantidad de archivos y cuántos son

Para optimizar el tiempo de ejecución, se deberá generar un thread por cada extensión distinta, para
poder paralelizar el análisis.

------------------------------------------

Criterios de corrección:

CONTROL									CRITICIDAD
Compila sin errores con el script entregado 				Obligatorio
Funciona correctamente según enunciado 					Obligatorio
Existe algún tipo de ayuda para la ejecución del proceso 		Obligatorio
Valida correctamente los parámetros 					Obligatorio
Acepta correctamente paths absolutos y relativos 			Obligatorio
Funciona correctamente con directorios con más de 100 archivos 		Obligatorio