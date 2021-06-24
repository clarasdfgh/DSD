# DSD: Invocación Remota

###### CH5. Distributed Systems: Concepts and Design

## 0. Transparencias

### A. Paso de mensajes

#### A.1. Conceptos

- Formar (*marshalling*): poner una colección de datos en un formato adecuado para su transmisión por mensaje. Consiste en:
  - Poner en plano (*to flat*) las estructuras de datos en secuencias básicas
  - Traducir los elementos básicos a una representación de datos estándar (como XDR [*External Data Representation*] de SUN)
  - Las operaciones para formar mensajes se pueden generar automáticamente
- Canal: abstracción de una red de comunicación física que proporciona un camino de comunicación entre procesos y sincronización mediante dos primitivas: send y receive.

#### A.2. Notación

Las notaciones difieren en:

- Ámbito y denominación de los canales (ej. globales a procesos o asociados a subconjunto de ellos)

- Uso (ej. flujos de información bidireccionales o no)

- Cómo se sincroniza la comunicación (ej. asíncrona/bloqueante, asíncrona/no bloqueante con buffer limitado o asíncrona con buffer limitado)

- Operación de comunicación no bloqueante -> su ejecución nunca retrasa al proceso que la invoca

  - Normalmente no bloquea send, aunque también existe el receive no bloqueante

  La mayoría de las propuestas de notaciones son **equivalentes**, ya que un programa en una notación se puede escribir en otra; pero cada propuesta es más adecuada dependiendo del tipo de problema.

Notación aceptada:

- send <puerto o canal> (<mensaje>)
- receive <puerto> (<mensaje>)
- empty <puerto>
  - Para ver si la cola de un canal está vacía

#### A.3. Comunicación

Tipo de comunicación

- Síncrona: ambas primitivas son bloqueantes
- Asíncrona: normalmente solo es bloqueante receive
- Asíncrona con buffer finito: receive bloqueante y send cuando el buffer está lleno
  - Las implementaciones de receive no bloqueante son complejas, requieren sondeos o interrupciones.

El destino de los mensajes...

- Debe ser conocido por el emisor e independiente de la localización (transparencia)
- Tipos:
  - Proceso (*referencia directa*): comunicación punto a punto
  - Enlace (*link*): punto a punto con indirección
  - Puerto (*port*): muchos a uno con indirección
  - Buzón (*mailbox*): muchos a muchos con indirección
  - Difusión (*broadcast*): muchos a muchos con indirección
  - Selección (*multicast*): muchos a muchos con indirección

Protocolos de comunicación de grupos:

- Uso:
  - Tolerancia a fallos en servicios replicados
  - Localización de objetos en servicios distribuidos (ej. ficheros)
  - Mejor rendimiento con servicios replicados
  - Actualización múltiple notificando eventos a varios procesos a la vez

- Propiedades:
  - Atomicidad: el mensaje es recibido por todos o ninguno
  - Ordenación: ejecución de las operaciones en el mismo orden
- Fiabilidad:
  - Fiable: los datos que emite el cliente serán recibidos por el servidor sin errores y en el mismo orden que fueron emitidos
  - Se puede construir paso de mensajes fiable a partir de uno no fiable (ej. mediante acuses de recibo)

### B. Comunicación cliente-servidor 

#### B.1. Protocolo petición respuesta

- Comunicación típicamente asíncrona y segura/fiable (la respuesta del servidor sirve como acuse de recibo)
- Alternativas de implementación:
  - Primitivas de comunicación (send-receive). Algunos inconvenientes:
    - Sobrecarga (más canales utilizados explícitamente)
    - Correspondencia entre send-receive
    - Garantía de reparto de mensajes si los servicios de red no la proporcionan

> Ej. asignador de recursos: diseñar un asignador de recursos utilizando primitivas de paso de mensajes utilizando distintos canales de comunicación
>
> ```pseudocode
> //Algunas estructuras de datos y variables a utilizar por el Asignador 
> var unidades: set of int; 
> pendientes: queue of int; 
> disponibles: int:= MAX_UNIDADES; 
> type clase_op = enum (ADQUIRIR,LIBERAR); 
> port peticion; 
> indice: int; 
> clase: clase_op; 
> unidad; int;
> ```
>
> ```pseudocode
> chan peticion (int, clase_op, int) 
> chan respuesta [1..n](int);
> ```
>
> ```pseudocode
> Cliente[i:1..n]::var unidad:int 
> 	… 
> 	send peticion (i, ADQUIRIR, -1)… 
> 	receive respuesta[i](unidad); 
> 	//usar recurso… 
> 	send peticion (i, LIBERAR, unidad);
> ```

- Alternativas de implementación:
  - Operaciones de comunicación:
    - Combinan aspectos de monitores (exportación de operaciones) y paso de mensajes síncrono (las peticiones bloquean a los clientes)
    - Tres primitivas de comunicación: DoOperation, GetRequest, SendReply

![image-20210418102457729](/home/clara/.config/Typora/typora-user-images/image-20210418102457729.png)

> Ej. operaciones en protocolo petición-respuesta
>
> ```pseudocode
> public byte[] doOperation (RemoteObjectRef o, int methodId, byte[] arguments) 
> 
> public byte[] getRequest (); 
> 
> public void sendReply (byte[] reply, InetAddress clientHost, int clientPort);
> ```
>
> ![image-20210418102602823](/home/clara/.config/Typora/typora-user-images/image-20210418102602823.png)

Realmente, hay tres protocolos petición-respuesta con diferentes semánticas ante la presencia de fallos:

| Nombre  | Enviado por cliente | Enviado por servidor | Enviado por cliente |
| ------- | ------------------- | -------------------- | ------------------- |
| **R**   | Petición            |                      |                     |
| **RR**  | Petición            | Respuesta            |                     |
| **RRA** | Petición            | Respuesta            | Reconocimiento      |

##### Implementación del protocolo petición-respuesta (modelo de gestión de fallos)

Depende de las garantías de envío-entrega.

- **Plazo de timpo** (*time out*) tras DoOperation. Alternativas cuando se cumple:

  - Devolver fallo, aunque la operación finalmente se esté realizando
  - Repetir petición y, eventualmente, devolver error si es el caso

- **Filtrar mensajes de petición duplicados**. Formato:

  - Tipo: petición o respuesta
  - Identificador petición
  - Referencia a objeto remoto: serializada
  - Identificador procedimiento: pueden estar numerados, o empaquetarse/serializarse
  - Argumentos

  ![image-20210418103858089](/home/clara/.config/Typora/typora-user-images/image-20210418103858089.png)

- **Mensajes de respuesta perdidos**: operaciones idempotentes en servidores permiten reejecución proporcionando los mismos resultados
- **Gestión de históricos**: 
  - Retransmisión de respuestas sin reejecución de operaciones
  - Manejo eficiente de la estructura de historia:
    - Nueva respuesta como reconocimiento de la respuesta previa
    - Reconocimientos del cliente (*acknowledgment*) ayudan a descargar entradas en la historia
    - También se pueden descargar tras un cierto tiempo

- **Modelo de fallos petición-respuesta**: garantía de envío/información sobre errores con DoOperation:

| Garantía de envío: reintentar petición | Garantía de envío: Filtrar duplicados | Garantía de envío: reejecutar procedimiento/retransmitir respuesta | Semánticas de invocación |
| -------------------------------------- | ------------------------------------- | ------------------------------------------------------------ | ------------------------ |
| No                                     | No aplica                             | No aplica                                                    | **Quizás**               |
| Sí                                     | No                                    | Reejecutar procedimiento                                     | **Al menos una vez**     |
| Sí                                     | Sí                                    | Retransmitir respuesta                                       | **Exactamente una vez**  |

### C. RPC

#### C.1. Introducción

- En el modelo cliente-servidor los servicios proporcionan varias operaciones
- La comunicación cliente-servidor se basa en un protocolo petición-respuesta
- Los mecanismos RPC integran esta organización con lenguajes de programación procedurales convencionales
- Se modela y diseña como una llamada a un procedimiento local, pero esta se ejecuta remotamente
- El servidor es visto como un módulo con una interfaz que exporta operaciones y con un tiempo de vida distinto
- Existe biblioteca de soporte a servicios para aislar cuestiones como:
  - Diferencias entre procedimiento local y remoto
  - Localización del servidor
  - Mejora del redimiento mediante cachés

#### C.2. Semántica

- Parámetros de entrada-salida (comunicación bidireccional)
- Sólo uso de variables locales (no hay variables globales entre servidor y cliente)
- No tienen sentido los punteros, se hacen **clonaciones** (estructura interne oculta por modularidad)
- Los servidores pueden devolver referencias opacas que no necesitan ser interpretadas en el entorno del cliente (ej. cookies, datos de autenticación)

#### C.3. Cuestiones de diseño

- **Clases de sistemas o middlewares RPC:** mecanismo integrado en el lenguaje de programación (permite algunos requisitos, como las excepciones, se puedan tratar con construcciones del lenguaje [ej. Argus]) o de propósito general (no dependen de un entorno particular [ej. Sun RPC])
- **Características del lenguaje de definición de interfaces (IDL):**
  - Deben permitir especificar...
    - Nombres de procedimientos
    - Tipo de parámetros
    - Dirección (entrada, salida, E/S)
- **Manejo de excepciones:** notificar de errores debidos a distribución, *time outs* de comunicaciones, errores de ejecución...
- **Transparencia:** 
  - Manejar errores debido a que RPC...
    - Es más vulnerable (red, otra computadora, otro proceso)
    - Toma más tiempo que en local
  - Por tanto, no debería ser transparente al programador: debería ocultar detalles de bajo nivel de paso de mensajes, pero no retardos o fallos.

#### C.4. Implementación

![image-20210418110600037](/home/clara/.config/Typora/typora-user-images/image-20210418110600037.png)

##### 1. Procesamiento de la interfaz

- Integra el mecanismo RPC con programas cliente y servidor, formando y extrayendo argumentos y resultados
- Se compila una especificación escrita en un lenguaje de definición de interfaces (IDL) y genera cabeceras, plantillas y stubs
  - En el cliente convierte llamada local a remota
  - En el servidor selecciona y llama al procedimiento adecuado

##### 2. Módulo de comunicaciones

Implementa el protocolo petición-respuesta

##### 3. Servicio de ligadura (binding)

- Mecanismo de para localización del servidor
- Asociación de un nombre a un identificador de comunicación
- Mensaje de petición se dirige a un puerto concreto
- Se evalúa cada vez que el cliente lo requiera, ya que el servidor puede ser relocalizado
- Servicio del cual dependen otros, así que debe ser tolerante a fallos
- Los servidores exportan (registran) sus servicios, y los clientes los importan (buscan)

![image-20210418112731951](/home/clara/.config/Typora/typora-user-images/image-20210418112731951.png)

Algunas alternativas para localizar al ligador...

- Dirección conocida: el cliente-servidor han de ser recompilados cuando el ligador se relocaliza
- El sistema operativo proporciona información en tiempo de ejecución (ej. variables de entorno)
- Cuando cliente-servidor se lanzan, envían mensajes de difusión (broadcast) para que así el ligador o binder responda con la dirección

#### C.5. RPC Asíncrona

Requisitos comunes:

- El cliente envía muchas peticiones al servidor
- No se necesita una respuesta por petición

Ventajas:

- El servidor puede planificar operaciones más eficientemente
- El cliente trabaja en paralelo
- Se facilita el cálculo de peticiones paralelas en el caso de varios servidores

![image-20210418113525531](/home/clara/.config/Typora/typora-user-images/image-20210418113525531.png)

Optimizaciones:

- Varias peticiones en una sola comunicación, se almacenan mensajes hasta que:
  - Se cumple un plazo de tiempo
  - Se realiza una petición que requiere respuesta
- El cliente puede proceder si no espera una respuesta que puede obtener más tarde

### D. Citas

También conocidas como citas extendidas en entornos distribuidos. Las invocaciones remotas se sirven mediante una **instrucción de aceptación** (dos instrucciones call-in).

RPC es una comunicación intermódulo, a diferencia de las citas.

Las instrucciones de comunicación están limitadas: a menudo un proceso desea comunicarse con más de un proceso, quizá en puertos diferentes, y no se sabe el orden en el que los otros procesos desean comunicarse con él.

- No determinismo mediante instrucciones guardadas:

```pseudocode
Var a, b, c; 
a, b, c := A, B, C; 

do 
	a > b -> a, b := b, a; 
	b > c -> b, c := c, b;

od
```

- Comunicación no determinista = instrucciones guardadas +  instrucciones de comunicación

![image-20210418123743642](/home/clara/.config/Typora/typora-user-images/image-20210418123743642.png)

#### D.1. Semántica

```python
B ; C -> S
```

- Semántica de la guarda:
  - Éxito si B es verdad y la ejecución de C no produce retardo
  - Falla si B es falso
  - Bloqueo si B es verdad pero C no se puede ejecutar sin producir retardo

B no puede cambiar hasta ejecutar otras instrucciones de asignación, ya que no hay variables locales

Las guardas pueden incluir instrucciones de comunicación de entrada o salida.

Las instrucciones guardades de comunicación pueden combinarse en instrucciones...

- Alternativas (if)
  - Si al menos una guarda tiene éxito,  una se escoge de forma no determinista ejecutando C y S
  - Si todas las guardas fallan, el if falla o termina
  - Si no hay guardas con éxito y algunas están bloqueadas, la ejecución se retrasa hasta que la primera tenga éxito
- Repetitivas (do), igual que con if con ejecución iterativa hasta que todas las guardas fallen

> Ej. Servidor de ficheros (clientes)
>
> ```pseudocode
> Clientes[j:1..m]:: var 
> servidor:int… 
> 
> if send abrir (nombref,j) -> 
> 	receive respuesta_abrir[j](servidor); 
> fi 
> 
> #trabajo/procesamiento con el fichero, e.g.: 
> send leer[servidor](args); 
> receive respuesta[servidor](resultados); 
> …
> ```
>
> Descripción:
>
> - Hasta n ficheros abiertos a la vez por m clientes
> - El acceso a cada fichero se proporciona por un proceso servidor del fichero distinto
> - Paso de mensajes asíncrono
>
> Operaciones sobre los archivos:
>
> - Abrir
> - Leer
> - Escribir
> - Cerrar
>
> Canales de comunicación:
>
> - `mailbox abrir (string nombre fichero, int n_cliente)`
> - `chan respuesta_abrir [1..m](int n_serv_fichero)`
> - `chan leer [1:n](...)`
> - `chan escribir[1:n](...)`
> - `chan cerrar [1:n](...)`
> - `chan respuesta [1..m](...)`
>
> Modelar el servidor de ficheros:
>
> ```pseudocode
> Ficheros [i:1..n]:: 
> 
> var nombref:string; args: otros tipos; 
> 	índice_cliente:int; resultados:int; 
> 	fichero_abierto:bool; 
> 	buffer_local, caché, dirección_de_disco, …; 	
> do receive abrir (…)
> ```

#### D.2. Órdenes guardadas de comunicación

- Un proceso exporta operaciones de forma similar a RPC
- Otro proceso invoca operaciones exportadas
  - `call <nombre_proceso>.<nombre_operación>(<argumentos>)`
- El proceso servidor atiende invocaciones en su contexto de ejecución mediante **instrucciones de aceptación**
  - `in <nombre_operación> (<parámetros_formales>) -> S ni`
  -  `<nombre_operación> (<parámetros_formales>)` sería la guarda
- El ámbito de los parámetros formales es el de la operación guardada
- Una guarda de una operación tiene éxito cuando:
  - Se ha invocado la operación
  - La expresión lógica se evalúa a verdad
- La ejecución se retrasa hasta que una guarda tiene éxito, por tanto, es no determinista cuando hay varias

#### D.3. Citas extendidas

Construcción alternativa con órdenes guardadas de comunicación:

![image-20210421092242890](/home/clara/.config/Typora/typora-user-images/image-20210421092242890.png)

##### Características

- Operaciones en el contexto del proceso que especifican puntos de comunicación de muchos a uno
- Sin parámetros hay sincronización y no comunicación
- El servidor puede definir distintas guardas para la invoación de una misma operación exportada, y por tanto, producir efectos diferentes ante la invación de un mismo servicio
- Las invocaciones se sirven en los instantes que desee el servidor
- A diferencia de RPC, el servidor es un proceso activo que se está ejecutando antes y después de servir una invocación remota

> Ej. Buffer acotado en el problema del Productor-Consumidor
>
> ```pseudocode
> Bufferacotado:: 
> 	op poner (dato:T); 
> 	tomar (var resultado:T); 
> 
> var buffer[1:n]:T 
> 	cabecera:int:=1; cola:int:=1; contador:int:=0; 
> 
> do true -> 
> 	in poner (dato) and (contador < n) -> 
> 		buffer[cola]:=dato; 
> 		cola:=(cola mod n)+1; 
> 		contador:=contador+1; 
> 	
> 	□ tomar (resultado) and (contador > 0) -> 
> 		resultado:=buffer[cabecera]; 
> 		cabecera:=(cabecera mod n)+1; 
> 		contador:=contador-1; 
> 	ni; 
> od;
> ```
>
> 

> Ej. Citas extendidas en Ada 
>
> ```pseudocode
> task Buffer1 is 
> 	entry Escribir (Elem: TElemento); 
> 	entry Leer (Elem: out TElemento); 
> end Buffer1; 
> 
> task body Buffer1 is 
> 	ElemLocal: TElemento; nElementos: integer; 
> 	tamBuffer : constant := 1; 
> 
> begin 
> 	loop 
> 		select when (nElementos < tamBuffer) 
> 			accept Escribir (Elem: TElemento) do 
> 				ElemLocal:= Elem; -- Guarda el elemento. 
> 				nElementos:=nElementos+1; 
> 				Ada.Text_IO.Put_Line("Elemento escrito"); 
> 			end Escribir; 
> 		or when (nElementos > 0) 
> 			accept Leer (Elem: out TElemento) do 
> 				Elem := ElemLocal; -- Devuelve el elemento. 
> 				nElementos:=nElementos-1;
> 				Ada.Text_IO.Put_Line("Elemento leído"); 
> 			end Escribir; 
> 		end select; 
> 	end loop; 
> end Buffer1; 
> …
> ```
>
> Ejemplos de llamadas:
>
> ```pseudocode
> Buffer1.Escribir(elemento), 
> Buffer1.Leer(elemento)…
> ```
>
> Ejemplos de declaraciones:
>
> ```pseudocode
> Tam_máx : constant := 100; 
> subtype T_Rango is Positive range 1 .. Tam_máx; 
> cabecera, cola : T_Rango := 1; 
> Buffer : array (T_Rango) of TElemento;
> ```
>
> ```pseudocode
> -- Alternativamente
> Buffer : array (0 .. Tam_máx) of TElemento; 
> Buffer : array (0 .. 100) of TElemento;…
> ```
>
> ```pseudocode
> -- Otros ejemplos de declaraciones para variables (no necesario)
> nElementos : Natural range 0 .. Tam_máx := 0;
> ```
>
> Ejemplo de acceso a elementos de un array:
>
> ```pseudocode
> Buffer(3) := otroElemento; 
> for i in Buffer'Range loop 
> 	Buffer(i) := unElemento; 
> end loop;
> ```

## 