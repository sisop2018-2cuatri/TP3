Se pide realizar una versi�n local del sistema de notas que se comunique v�a memoria compartida,
utilizando la misma base de datos que el sistema remoto.

------------------------------------------------------

Criterios de correcci�n:

CONTROL 									CRITICIDAD
Compila sin errores con el script entregado					Obligatorio
Funciona correctamente seg�n enunciado 						Obligatorio
Existe alg�n tipo de ayuda para la ejecuci�n del proceso 			Obligatorio
No deja procesos zombies o hu�rfanos. 						Obligatorio
Valida correctamente los par�metros						Obligatorio
Utiliza shared memory y sem�foros 						Obligatorio
Maneja correctamente el cierre de los recursos 					Deseable
Tiene archivo de configuraci�n (deben entregarlo) 				Deseable
Se puede utilizar al mismo tiempo que el servidor socket, ambos sobre el
mismo archivo de base y manteniendo la premisa de bloqueo al hacer una
operaci�n
										Deseable
Soporta m�ltiples clientes (se probar� con m�s de 5) 				Obligatorio