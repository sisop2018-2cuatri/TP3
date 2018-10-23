Se desea generar mediante el uso del syscall fork el siguiente escenario:
 *2 procesos hijos
 *3 procesos nietos
 *5 procesos bisnietos
 *2 procesos zombies, en cualquier nivel
 *3 procesos demonios, que deben quedar activos
Cada proceso no podrá tener más de 2 hijos.

Cada proceso deberá mostrar por pantalla la siguiente información:
Soy el proceso con PID …… y pertenezco a la generación Nº …….
Pid: ……… Pid padre: ….. Parentesco/Tipo: [nieto, hijo, zombie]

Hacer una espera hasta que se presione una tecla antes de finalizar, para poder verificar con el
comando ps o pstree toda la jerarquía de procesos generada.

-----------------------------------------------

Criterios de corrección:
CONTROL								CRITICIDAD
Compila sin errores con el script entregado 			Obligatorio
Funciona correctamente según enunciado 				Obligatorio
Existe algún tipo de ayuda para la ejecución del proceso 	Obligatorio

