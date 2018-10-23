Se requiere realizar la facturación al final del día de un garaje de autos que está automatizado. El
control de entrada y salida de vehículos se hace con dos cámaras inteligentes que leen la patente
del auto y envían la información al proceso facturador vía FIFO.

Las cámaras inteligentes envían una letra indicando el sentido (Entrada/Salida) seguido de la
patente del auto.

Ej:
EIPH898 -> indica entrada
SAA234BB -> indica salida

El proceso debe ir mostrando por pantalla las entradas (hora y vehículo) e ir almacenando en un
archivo de log (con la fecha en el nombre del mismo). Cuando se procesa una salida, debe además
mostrar el valor a cobrarle, para poder hacer el cálculo debe pedir por parámetro el valor de la hora,
y se fracciona cada 15 minutos.

Nota: las patentes no pueden repetirse para 2 entradas simultaneas salvo que entre medio tenga
una salida. Por ejemplo:

Movimientos_20180820.txt:

Entrada AAA999 9:12
Entrada III111 9:00
Salida AAA999 14:30
Entrada AAA999 17:40
Entrada BBB000 18:00
Entrada CAB000 18:00
Salida BBB000 20:00
Salida AAA999 20:30
Entrada BBB000 21:05
Salida III111 22:00

En el cierre del día, cuando finalizan el proceso facturador y cierran el estacionamiento se debe
generar el archivo de facturación (Facturacion_AAAAMMDD.txt) con los precios cobrados por
patente y el total del día.

Nota: si una patente al momento de hacer el cierre no posee una salida se debería incluir la leyenda
“Aún en el garaje”.

Entonces el archivo Facturacion_AAAAMMDD.txt quedaría:

AAA999 $255
BBB000 $60
BBB000 “Aun en el garaje”
CAB000 “Aun en el garaje”
III111 $250

---------------------------------------------

Criterios de corrección:

CONTROL 								CRITICIDAD
Compila sin errores con el script entregado 				Obligatorio
Funciona correctamente según enunciado 					Obligatorio
Existe algún tipo de ayuda para la ejecución del proceso 		Obligatorio
No deja procesos zombies o huérfanos. 					Obligatorio
Valida correctamente los parámetros 					Obligatorio
Genera los archivos de salida en formato legible 			Obligatorio
Incluyen como parte del ejercicio un script para automatizar la prueba 	Deseable