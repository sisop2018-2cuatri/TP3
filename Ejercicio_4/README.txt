Se tiene una base de datos de notas de alumnos en un archivo de texto, y se necesita que los
profesores puedan actualizar la misma en forma remota y concurrente (varios profesores a la vez).
La identificaci�n del alumno se hace a trav�s de su n�mero de DNI, el archivo tiene un formato
separado por comas con los siguientes campos:

DNI,Nombre_Materia,Evaluacion,Nota

Las evaluaciones pueden ser Primer Parcial, Segundo Parcial o Recuperatorio.

Para esto se solicita que creen un servidor que atienda las solicitudes generadas desde un proceso
cliente que se le entrega a cada docente que se encuentran en distintas m�quinas de la red de la
universidad. Al ingresar el docente informa por par�metro la materia a la que pertenece y la
direcci�n del servidor. El sistema debe permitir cargar una nota y consultar el promedio de notas del
alumno (tanto general como por materia); pero no se puede modificar una nota ya cargada.

Mientras se est� realizando una transacci�n (incluso la consulta de promedios) el sistema no debe
permitir realizar otra transacci�n para garantizar el resultado de las consultas.

No es necesario controlar duplicidad de registros ni consistencia de los datos de la base, como
tampoco la identificaci�n o login del docente.

-----------------------------------------------------------

Criterios de correcci�n:

CONTROL 									CRITICIDAD
Compila sin errores con el script entregado 					Obligatorio
Funciona correctamente seg�n enunciado 						Obligatorio
Existe alg�n tipo de ayuda para la ejecuci�n del proceso 			Obligatorio
No deja procesos zombies o hu�rfanos. 						Obligatorio
Valida correctamente los par�metros 						Obligatorio
Funciona tanto localmente como a trav�s de la red desde distintas m�quinas 	Obligatorio
Utiliza sockets y threads 							Obligatorio
Maneja correctamente el cierre de los puertos al finalizar 			Deseable
Tiene archivo de configuraci�n (deben entregarlo) 				Deseable
Se conecta con nombre de m�quinas 						Deseable
Soporta m�ltiples clientes (se probar� con m�s de 5) 				Obligatorio