Se necesita un proceso que analice archivos en directorios con mucha cantidad de archivos,
generando una estad�stica de los siguientes valores, agrupados por cada extensi�n de archivo
distinta (Ej, todos los .doc, los .mp3, los .c, etc.)
* Cantidad de archivos totales
* Por cada extensi�n
	o Cantidad de archivos
	o Tama�o del archivo m�s chico
	o Tama�o del archivo m�s grande
	o Usuario due�o de la mayor cantidad de archivos y cu�ntos son

Para optimizar el tiempo de ejecuci�n, se deber� generar un thread por cada extensi�n distinta, para
poder paralelizar el an�lisis.

------------------------------------------

Criterios de correcci�n:

CONTROL									CRITICIDAD
Compila sin errores con el script entregado 				Obligatorio
Funciona correctamente seg�n enunciado 					Obligatorio
Existe alg�n tipo de ayuda para la ejecuci�n del proceso 		Obligatorio
Valida correctamente los par�metros 					Obligatorio
Acepta correctamente paths absolutos y relativos 			Obligatorio
Funciona correctamente con directorios con m�s de 100 archivos 		Obligatorio