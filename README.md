# path_sum: 
## Introducción
Las líneas de código aquí recogidas tenían como objetivo la resolución de los problemas 18,67 y 81 de <a HREF="https://projecteuler.net/about"> Project Euler </a>. Más concretamente, el objetivo final se trata de resolver el problema 81:

<img src="https://github.com/Lba-29/path_sum/blob/main/images/Enunciado.png" width="100%" height="100%">

Lo he enfocado a encontrar no sólo el coste del camino más corto, sino la ruta que debe seguirse. Para el ejemplo del enunciado, el programa debe proporcionar  ```coste = 2427``` y además el camino que sigue: ```M[0,0]```, ```M[1,0]```, ```M[1,1]```, ```M[1,2]```, ```M[2,2]```, ```M[2,3]```, ```M[3,3]```, ```M[4,3]``` y ```M[4,4]```
En los sucesivos apartados discuto qué algoritmos he utilizado y la estructura básica del programa.

## Algoritmos empleados

La aproximación más intuitiva es probar con todos los caminos posibles, calcular su coste y seleccionar de todos ellos el de menor valor. Este enfoque puede funcionar para matrices pequeñas, pero para un número de filas grande el número de casos que hay que probar es inmenso e inviable.
Podemos hacer el cálculo aplicado en el ejemplo del enunciado. Para una matriz de ```N=5``` filas, siendo el origen el primer elemento y el destino el último, se necesitan hacer 4 movimientos hacia abajo y 4 hacia la derecha, independientemente del camino. El problema puede verse como un problema de combinatoria de permutaciones con repetición, donde para ```N``` filas el número de caminos lo da:

<img src="https://github.com/Lba-29/path_sum/blob/main/images/permutaciones.png" width="100%" height="100%">

Y como puede verse en la tabla, el número de caminos a calcular crece exponencialmente.

Este problema es equivalente al problema 18 y 67 de Project Euler, y utilizo la misma idea para resolverlo.

<img src="https://github.com/Lba-29/path_sum/blob/main/images/67_Maximum%20path%20sum%20II.png" width="100%" height="100%">

Para explicar la resolución del triángulo consideramos el ejemplo anterior: situándonos en la base, penúltima fila, la modificamos por otra equivalente sumando el elemento de la fila inferior que satisface la condición (en el problema 18, se trata de el mayor valor):

<img src="https://github.com/Lba-29/path_sum/blob/main/images/abajo-arribapng.png" width="100%" height="100%">

Así, el triángulo va reduciéndose en número de filas por otro equivalente cuya base tiene pesos actualizados conservando la información de la ruta más óptima en caso de continuar por dicho elemento de la fila.

Para el caso de una matriz, el problema puede resolverse con la misma lógica si la dividimos en 2 triángulos:

<img src="https://github.com/Lba-29/path_sum/blob/main/images/resolucion_matriz.png" width="100%" height="100%">

Sin embargo, si aplicamos el algoritmo a ambos triángulos puede ocurrir que los caminos más óptimos para cada uno no coincidan en el elemento que pasa por la fila de la base que comparten. Es por ello que resolverlo así no hará que encontremos el camino más óptimo en conjunto ya que cuando se resuleve no se considera la información del otro triángulo.

La mananera de considerar esto es que antes de aplicar el algoritmo anterior, que resuelve el triángulo de la base a la punta; se resuelva el otro partiendo de la punta y llegando a la base, y se sustituya en el otro triángulo por la base obtenida. Aplicando en la matriz de arriba, en el triángulo ```down```, se resuelve primero de arriba a abajo:

<img src="https://github.com/Lba-29/path_sum/blob/main/images/arriba-abajo.png" width="100%" height="100%">

Ahora esta base se introduce en el triángulo ```up``` y se resuelve con el algoritmo que va de la base a la punta del triángulo, obteniéndose una única ruta y su coste:

<img src="https://github.com/Lba-29/path_sum/blob/main/images/abajo-arriba_matriz.png" width="100%" height="100%">

## Estructura del programa

La implementación de la idea descrita anteriormente se lleva a cabo en ```euler_81_path_sum.c```

<img src="https://github.com/Lba-29/path_sum/blob/main/images/estructura_programa.png" width="100%" height="100%">

El programa tiene 3 partes: Declaración de variables (reserva dinámica de memoria para que sea válido para cualquier ```N```, asignación de variables y obtención de datos de un fichero ```*.csv``` donde se encuentra la matriz en formato ```;```), aplicación del algoritmo ( ```caminos_base_triangulo()``` es la resolución de un triángulo desde la punta a la base y ```camino_menor_coste()``` resuelve el triángulo de la base a la punta) y finalmente unas líneas de código para unir información dada por las funciones de ambos algoritmos en una única variable.

El resultado del programa es un array con los índices de los elementos de la matriz del camino más óptimo y su coste. Para el ejemplo del problema 81:

<img src="https://github.com/Lba-29/path_sum/blob/main/images/N%3D5.png" width="60%" height="60%">


Y para el caso a resolver una matriz mayor:

<img src="https://github.com/Lba-29/path_sum/blob/main/images/N%3D80.png" width="100%" height="100%">
