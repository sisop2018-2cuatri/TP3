Se desea generar mediante el uso del syscall fork el siguiente escenario:
 *2 procesos hijos
 *3 procesos nietos
 *5 procesos bisnietos
 *2 procesos zombies, en cualquier nivel
 *3 procesos demonios, que deben quedar activos
Cada proceso no podr� tener m�s de 2 hijos.

Cada proceso deber� mostrar por pantalla la siguiente informaci�n:
Soy el proceso con PID �� y pertenezco a la generaci�n N� ��.
Pid: ��� Pid padre: �.. Parentesco/Tipo: [nieto, hijo, zombie]

Hacer una espera hasta que se presione una tecla antes de finalizar, para poder verificar con el
comando ps o pstree toda la jerarqu�a de procesos generada.

-----------------------------------------------

Criterios de correcci�n:
CONTROL								CRITICIDAD
Compila sin errores con el script entregado 			Obligatorio
Funciona correctamente seg�n enunciado 				Obligatorio
Existe alg�n tipo de ayuda para la ejecuci�n del proceso 	Obligatorio

