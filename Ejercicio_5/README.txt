Se pide realizar una versión local del sistema de notas que se comunique vía memoria compartida,
utilizando la misma base de datos que el sistema remoto.

------------------------------------------------------

Criterios de corrección:

CONTROL 									CRITICIDAD
Compila sin errores con el script entregado					Obligatorio
Funciona correctamente según enunciado 						Obligatorio
Existe algún tipo de ayuda para la ejecución del proceso 			Obligatorio
No deja procesos zombies o huérfanos. 						Obligatorio
Valida correctamente los parámetros						Obligatorio
Utiliza shared memory y semáforos 						Obligatorio
Maneja correctamente el cierre de los recursos 					Deseable
Tiene archivo de configuración (deben entregarlo) 				Deseable
Se puede utilizar al mismo tiempo que el servidor socket, ambos sobre el
mismo archivo de base y manteniendo la premisa de bloqueo al hacer una
operación
										Deseable
Soporta múltiples clientes (se probará con más de 5) 				Obligatorio