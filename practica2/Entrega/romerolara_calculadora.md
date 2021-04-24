# DSD 20-21, Práctica 1: Calculadora RPC Sun

###### Clara Mª Romero Lara - 3º GII, IS

## 1. Introducción

En esta primera práctica se pedía crear una calculadora que realice operaciones básicas mediante Sun RPC. Se solicitaban como mínimo unas operaciones básicas (suma, resta, multiplicación y división), con ánimo a extender sus funcionalidades a otras estructuras de datos y operaciones más complejas.

En nuestra práctica hemos implementado estas operaciones básicas y operaciones con vectores y matrices.

## 2. Uso de la calculadora

### 2.1. Prerrequisitos

Esta práctica se ha desarrollado en Arch Linux, versión 5.11.11-arch1-1. Hemos tenido que instalar previamente los siguientes paquetes RPC:

- libtirpc 1.3.1-1
- rpcbind 1.2.5-3
- rpcsvc-proto 1.4.2-1

### 2.2. Desplegado

Tras definir las operaciones de la calculadora en `calculadora.x`, ejecutamos:

```
>rpcbind
>rpcinfo
>rpcgen -aCN calculadora.x
```

El comando `rpcbind` nos permite hacer uso de las funcionalidades de rpcgen. Con `rpcinfo` podremos comprobar si ciertamente existe una instancia de `rpcbind` - En este caso es obvio que sí, pero es un comando útil para comprobaciones futuras.

Mediante la orden `rpcgen` se generan, entre otros, los archivos correspondientes al cliente y al servidor de la calculadora. Las opciones -aCN hacen lo siguiente:

- -a: generar el código para los archivos del lado cliente y servidor
- -C: permite compilar con el compilador de C++
- -N: habilita el 'newstyle' de RPCgen, que permite múltiples parámetros en las operaciones

![image-20210412091716617](/home/clara/.config/Typora/typora-user-images/image-20210412091716617.png)

### 2.3. Compilación y ejecución

En nuestro caso, al tratarse de Arch Linux, la biblioteca tirpc se encuentra en una ruta distinta a la que asume rpc. Es por eso que no podremos compilar directamente con `make`: hay que especificar manualmente la ruta de la biblioteca.

```
> make -f Makefile.calculadora CFLAGS+="-I/usr/include/tirpc -ltirpc"
```

![image-20210412092047164](/home/clara/.config/Typora/typora-user-images/image-20210412092047164.png)

### 2.4. Uso

Una vez compilado, para ejecutar abriremos dos terminales: una para el server y otra para el cliente.

En primer lugar, ejecutamos como usuario root el server:

```
> sudo ./calculadora_server
```

Y el cliente. Debemos pasarle como parámetros el host (en el caso de esta práctica, '*localhost*') y el tipo de operación que deseamos realizar: 1 para operaciones simples, 2 para operaciones con vectores, y 3 para operaciones con matrices. Si los parámetros introducidos son incorrectos (como en la foto), el programa nos mostrará instrucciones de uso.

```
> ./calculadora_client localhost
```

![image-20210412092218557](/home/clara/.config/Typora/typora-user-images/image-20210412092218557.png)

Algunos ejemplos de operaciones:

<img src="/home/clara/.config/Typora/typora-user-images/image-20210412092537236.png" alt="image-20210412092537236" style="zoom:40%;" /><img src="/home/clara/.config/Typora/typora-user-images/image-20210412092650966.png" alt="image-20210412092650966" style="zoom:40%;" /> 

## 3. Calculadora

### 3.1. Calculadora.x

El formato .x correspone a archivos RPC. Aquí definiremos las estructuras de datos y operaciones de la calculadora.

#### Estructuras de datos

- `operandos`: un struct consistente en dos double, *a* y *b*. Se pasa como parámetro a las funciones correspondientes a las operaciones simples.
- `vec<>`: un vector de tamaño indefinido de doubles. Cuenta con los atributos vec_len (tamaño) y vec_val (valores).
- `matr<>`: un vector de tamaño infefinido de `vec`. Cuenta con los atributos matr_len (tamaño) y matr_val, desde el cual se accede a los vectores internos.
- `calc_resultado`: funciona como filtro de errores, en caso de que haya algún error (`err != 0`), no devolverá nada. El resultado es un double si no hay errores.
- `calc_resultado_vec`:  funciona como filtro de errores, en caso de que haya algún error (`err != 0`), no devolverá nada. El resultado es un `vec` si no hay errores.
- `calc_resultado_matr`:  funciona como filtro de errores, en caso de que haya algún error (`err != 0`), no devolverá nada. El resultado es un `matr` si no hay errores.

#### Operaciones

- **Versión 1:** en esta versión se encuentran las operaciones simples.
  - `suma(operandos)`
  - `resta(operandos)`
  - `multiplicacion(operandos)`
  - `division(operandos)`
- **Versión 2:** en esta versión se encuentran las operaciones con vectores.
  - `vsuma(vec, vec)`
  - `vresta(vec, vec)`
  - `vmultiplicacion(vec, vec)`
  - `vdivision(vec, vec)`
- **Versión 3:** en esta versión se encuentran las operaciones con matrices.
  - `msuma(matr, matr)`
  - `mresta(matr, matr)`
  - `mmultiplicacion(matr, matr)`



### 3.2. Cliente

El main del cliente consiste en inicializar los operadores y llamar a la función correspondiente.

Al haber definido versiones en nuestro .x, cada tipo de operación tiene su propia función `calculadora_n()` (siendo *n* las versiones 1 a 3). Accedemos a la correspondiente mediante un switch-case sobre la variable `tipo`, introducida como parámetro al ejecutar.

```c++
	switch (tipo) {
/******************************************************************************/
		case 1:
			printf("Introduzca su operación. FORMATO: x operador y \n");
			scanf("%lf %c %lf", &mis_operandos.a, &operador, &mis_operandos.b );

			calculadora_1 (host, mis_operandos.a, operador, mis_operandos.b);
		break;

/******************************************************************************/
		case 2:
			printf("Introduzca la longitud de los vectores: ");
			scanf("%d", &len );
			vector_A.vec_len = len;
			vector_A.vec_val = malloc(len * sizeof(double));


		//[[[[INICIALIZACIÓN DE LOS VECTORES AQUÍ, OMITIDA PARA MEJOR LECTURA]]]]
			

			printf("\nIntroduzca su operador: ");
			scanf(" %c", &operador);

			calculadora_2 (host, vector_A, operador, vector_B);
			free(vector_A.vec_val);
			free(vector_B.vec_val);
		break;

/******************************************************************************/
		case 3:
			printf("Introduzca el tamaño de las matrices (solo opera en matrices cuadradas): ");
			scanf("%d", &len );

      matriz_A.matr_len = len;
      matriz_A.matr_val = malloc(len * len * sizeof(double));

			for(int i = 0; i < len; i++){
				matriz_A.matr_val[i].vec_len = len;
				matriz_A.matr_val[i].vec_val = malloc(len * len * sizeof(double));
			}

			
		//[[[[INICIALIZACIÓN DE LAS MATRICES AQUÍ, OMITIDA PARA MEJOR LECTURA]]]]
			
			
			printf("\nIntroduzca su operador: ");
			scanf(" %c", &operador);

			calculadora_3 (host, matriz_A, operador, matriz_B);

      for(int i = 0; i < len; i++){
    			free(matriz_A.matr_val[i].vec_val);
    			free(matriz_B.matr_val[i].vec_val);
      }

			free(matriz_A.matr_val);
			free(matriz_B.matr_val);
		break;

		default:
				printf ("uso: ./calculadora_cliente server_host tipo\n");
				printf ("\tTipos de operación:  \n");
				printf ("\t 1.Operaciones básicas\n");
				printf ("\t 2.Operaciones con vectores \n");
                printf ("\t 3.Operaciones con matrices \n");
				exit (1);
	}
```

Como vemos, el proceso es siempre el mismo: recibir por `scanf` los operadores, alojar memoria en caso de matrices y vectores, recibir el operador, y llamar a la calculadora.

Dentro de cada función `calculadora_n` definimos un puntero a la respuesta correspondiente (simple, vector, o matriz) y nos encontramos, de nuevo, con un switch. Este ejecuta la operación correspondiente al operador introducido.  Adjuntamos el switch de la versión 1 (operaciones simples) ya que las tres funciones  `calculadora_n` son exactamente iguales; solo difieren en el tipo de dato (`operandos`, `vec` y `matr`) y las funciones asociadas a estos:

```c++
switch (operador) {
 	case '+':
 		result_1 = suma_1(mis_operandos, clnt);
		pinta_operacion(mis_operandos, operador, result_1);
 	break;

 	case '-':
 		result_1 = resta_1(mis_operandos, clnt);
 		pinta_operacion(mis_operandos, operador, result_1);
 	break;

 	case 'x':
 		result_1 = multiplicacion_1(mis_operandos, clnt);
 		pinta_operacion(mis_operandos, operador, result_1);
 	break;

 	case '/':
 		result_1 = division_1(mis_operandos, clnt);
 		pinta_operacion(mis_operandos, operador, result_1);
 	break;

	default:
			printf ("Operadores disponibles:\n");
			printf (" Suma: +\n");
			printf (" Resta: -\n");
			printf (" Multiplicacion: x\n");
			printf (" Division: +\n");
			exit(1);
 }
```

Como podemos ver, tras la llamada al servidor, el resultado aparecerá en pantalla mediante una llamada a la función que le corresponda por tipo: `pinta_operacion()`, `pinta_operacion_vec()`,  `pinta_operacion_matr()`.

### 3.3. Server

#### Operaciones simples

Procedimientos *straightforward* de suma, resta, multiplicación y división:

```c++
calc_resultado *
suma_1_svc(operandos arg1,  struct svc_req *rqstp)
{
	static calc_resultado  result;

	xdr_free((xdrproc_t)xdr_calc_resultado, &result);
	result.calc_resultado_u.resultado = arg1.a + arg1.b;

	return &result;
}

calc_resultado *
resta_1_svc(operandos arg1,  struct svc_req *rqstp)
{
	static calc_resultado  result;

	xdr_free((xdrproc_t)xdr_calc_resultado, &result);
	result.calc_resultado_u.resultado = arg1.a - arg1.b;

	return &result;
}

calc_resultado *
multiplicacion_1_svc(operandos arg1,  struct svc_req *rqstp)
{
	static calc_resultado  result;

	xdr_free((xdrproc_t)xdr_calc_resultado, &result);
	result.calc_resultado_u.resultado = arg1.a * arg1.b;

	return &result;
}

calc_resultado *
division_1_svc(operandos arg1,  struct svc_req *rqstp)
{
	static calc_resultado  result;

	xdr_free((xdrproc_t)xdr_calc_resultado, &result);
	result.calc_resultado_u.resultado = arg1.a / arg1.b;

	return &result;
}
```

#### Operaciones con vectores

Las operaciones con vectores primero alojan el espacio necesario para el resultado. Se ha optado por operaciones literales: es decir, más que operaciones con vectores, son operaciones con listas de números. 

```c++
calc_resultado_vec *
vsuma_2_svc(vec arg1, vec arg2,  struct svc_req *rqstp)
{
	static calc_resultado_vec  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_vec, &result);

	int len  = arg1.vec_len;
	result.calc_resultado_vec_u.resultado.vec_len = len;
	result.calc_resultado_vec_u.resultado.vec_val = malloc(len * sizeof(double));


	for(int i = 0; i < len; i++){
		result.calc_resultado_vec_u.resultado.vec_val[i] = arg1.vec_val[i] + arg2.vec_val[i];
	}

	return &result;
}

calc_resultado_vec *
vresta_2_svc(vec arg1, vec arg2,  struct svc_req *rqstp)
{
	static calc_resultado_vec  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_vec, &result);

	int len  = arg1.vec_len;
	result.calc_resultado_vec_u.resultado.vec_len = len;
	result.calc_resultado_vec_u.resultado.vec_val = malloc(len * sizeof(double));


	for(int i = 0; i < len; i++){
		result.calc_resultado_vec_u.resultado.vec_val[i] = arg1.vec_val[i] - arg2.vec_val[i];
	}

	return &result;
}

calc_resultado_vec *
vmultiplicacion_2_svc(vec arg1, vec arg2,  struct svc_req *rqstp)
{
	static calc_resultado_vec  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_vec, &result);

	int len  = arg1.vec_len;
	result.calc_resultado_vec_u.resultado.vec_len = len;
	result.calc_resultado_vec_u.resultado.vec_val = malloc(len * sizeof(double));


	for(int i = 0; i < len; i++){
		result.calc_resultado_vec_u.resultado.vec_val[i] = arg1.vec_val[i] * arg2.vec_val[i];
	}

	return &result;
}

calc_resultado_vec *
vdivision_2_svc(vec arg1, vec arg2,  struct svc_req *rqstp)
{
	static calc_resultado_vec  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_vec, &result);

	int len  = arg1.vec_len;
	result.calc_resultado_vec_u.resultado.vec_len = len;
	result.calc_resultado_vec_u.resultado.vec_val = malloc(len * sizeof(double));


	for(int i = 0; i < len; i++){
		result.calc_resultado_vec_u.resultado.vec_val[i] = arg1.vec_val[i] / arg2.vec_val[i];
	}

	return &result;
}

```

#### Operaciones con matrices

Las operaciones con matrices primero alojan el espacio necesario para la matriz resultado. Cuenta con suma, resta y multiplicación, ya que la división de matrices no existe como tal, sino que se considera como la multiplicación por el inverso. Hemos considerado matrices estrictamente cuadradas para más claridad en la práctica.

```c++
calc_resultado_matr *
msuma_3_svc(matr arg1, matr arg2,  struct svc_req *rqstp)
{
	static calc_resultado_matr  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_matr, &result);

	int len  = arg1.matr_len;

	result.calc_resultado_matr_u.resultado.matr_len = len;
	result.calc_resultado_matr_u.resultado.matr_val = malloc(len * len * sizeof(double));

	for(int i = 0; i < len; i++){
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_len = len;
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_val = malloc(len * len * sizeof(double));
	}


	for(int i = 0; i < len; i++){
		for(int j = 0; j < len; j++){
			result.calc_resultado_matr_u.resultado.matr_val[i].vec_val[j] = arg1.matr_val[i].vec_val[j] + arg2.matr_val[i].vec_val[j];
		}
	}


	return &result;
}

calc_resultado_matr *
mresta_3_svc(matr arg1, matr arg2,  struct svc_req *rqstp)
{
	static calc_resultado_matr  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_matr, &result);

	int len  = arg1.matr_len;

	result.calc_resultado_matr_u.resultado.matr_len = len;
	result.calc_resultado_matr_u.resultado.matr_val = malloc(len * len * sizeof(double));

	for(int i = 0; i < len; i++){
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_len = len;
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_val = malloc(len * len * sizeof(double));
	}


	for(int i = 0; i < len; i++){
		for(int j = 0; j < len; j++){
			result.calc_resultado_matr_u.resultado.matr_val[i].vec_val[j] = arg1.matr_val[i].vec_val[j] - arg2.matr_val[i].vec_val[j];
		}
	}


	return &result;
}

calc_resultado_matr *
mmultiplicacion_3_svc(matr arg1, matr arg2,  struct svc_req *rqstp)
{
	static calc_resultado_matr  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_matr, &result);

	int len  = arg1.matr_len;

	result.calc_resultado_matr_u.resultado.matr_len = len;
	result.calc_resultado_matr_u.resultado.matr_val = malloc(len * len * sizeof(double));

	for(int i = 0; i < len; i++){
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_len = len;
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_val = malloc(len * len * sizeof(double));
	}


	for(int i = 0; i < len; i++){
		for(int j = 0; j < len; j++){
			int suma = 0;
			for(int k = 0; k < len; k++){
				suma += arg1.matr_val[j].vec_val[k] * arg2.matr_val[k].vec_val[i];
			}
			result.calc_resultado_matr_u.resultado.matr_val[j].vec_val[i] = suma;
		}
	}


	return &result;
}
```

