# DSD 20-21, Práctica 4: NodeJs

###### Clara Mª Romero Lara - 3º GII, IS

## 1. Introducción

La primera parte de esta práctica consiste en la correcta ejecución de una serie de ejemplos, diseñados para ayudarnos a entender las funcionalidades que usaremos en el ejercicio posterior.

El ejercicio se basa en un sistema de domótica, es decir, un sistema de automatización de vivienda. Simularemos una serie de sensores, y unos actuadores que respondan en función a la información recibida por los sensores. Además, se podrá gestionar el sistema desde un panel de control 

Hemos optado por usar `yarn`  en vez de `npm`, ya que cuenta con la misma funcionalidad pero da menos problemas en nuestro sistema operativo.

## 2. Ejemplos

Para facilitar la ejecución, hemos introducido scripts dentro del archivo `package.json` de la siguiente manera:

```json
  "scripts": {
    	"helloworld": 		"node src/helloworld.js",
		"calculadora": 		"node src/calculadora.js",
		"calculadoraweb": 	"node src/calculadora-web.js",
		"connection": 		"node src/connections.js",
		"mongotest": 		"node src/mongo-test.js"
  }
```

Cabe destacar que el código fuente de algunos de estos ejemplos ha sido modificado para su correcta ejecución, más específicamente, para resolver problemas de rutas. En ningún caso se ha modificado el código proporcionado más que para este fin.

### 2.1. Ejemplo 1: hola mundo

Lanzamos el programa con yarn y uno de los scripts indicados anteriormente:

```
yarn helloworld
```

Y accedemos al puerto 8080 desde nuestro navegador (http://127.0.0.1:8080/). Este programa simplemente imprime el "Hola mundo".

![image-20210604093652514](/home/clara/.config/Typora/typora-user-images/image-20210604093652514.png)

### 2.2. Ejemplo 2: calculadora, calculadora web

Lanzamos el programa con yarn y su script correspondiente:

```
yarn calculadora
```

Y accedemos al puerto 8080 desde nuestro navegador, indicando en la ruta la operación, el operador 1 y el operador 2, por ejemplo: http://127.0.0.1:8080/sumar/2/2 . Esta es la forma más básica de pasar parámetros a nuestras aplicaciones Nodejs.

![image-20210604093900969](/home/clara/.config/Typora/typora-user-images/image-20210604093900969.png)

Luego existe la opción `calculadoraweb`. Nos lleva a un formulario que permite operar con la calculadora sin tener que modificar la ruta:

```
yarn calculadoraweb
```

Accedemos al puerto 8080 desde el navegador (http://127.0.0.1:8080). Desde ahí podremos operar con la calculadora libremente, ya que estamos cargando un formulario html que hace las peticiones  por nosotros:

![image-20210604094232366](/home/clara/.config/Typora/typora-user-images/image-20210604094232366.png)

### 2.3. Ejemplo 3: conexión

Lanzamos el programa con yarn y su script correspondiente:

```
yarn connection
```

Accedemos al puerto 8080 desde el navegador (http://127.0.0.1:8080). En este ejemplo contamos con una implementación de Socket.io que muestra una notificación que con las direcciones de todos los clientes conectados al propio servicio. La notificación se envía cada vez que un cliente se conecta o se desconecta. 

De esta forma, la página imprime una lista con los clientes conectados en ese instante. Como vemos en la captura, contamos con tres instancias distintas y, cuando cerramos una, la página deja de mostrar la dirección de ese cliente.

![image-20210604102353712](/home/clara/.config/Typora/typora-user-images/image-20210604102353712.png)

Además, el lado del servidor también se muestra: veamos qué pasa si cancelamos la ejecución mientras seguimos dentro de la página.

![image-20210604102848228](/home/clara/.config/Typora/typora-user-images/image-20210604102848228.png)

Hemos cancelado la ejecución y cerrado dos de las instancias de cliente. Como podemos ver, ya solo queda una instancia del cliente, pero en la página se siguen reflejando tres porque nos hemos desconectado y ya no se actualiza.

### 2.4. Ejemplo 4: mongo test

Para ejecutar el siguiente ejemplo vamos a necesitar inicializar los servicios de Mongodb. A continuación, lanzamos el programa con yarn y su script correspondiente:

```
>sudo systemctl start mongodb
>yarn mongotest
```

De forma similar al ejemplo anterior, al entrar al programa se registra nuestro acceso. La diferencia es que, en este caso, lo guarda en la base de datos de forma permanente.

![image-20210604105511567](/home/clara/.config/Typora/typora-user-images/image-20210604105511567.png) 



## 3. Ejercicio: sistema domótico

Se nos pide desarrollar un sistema domótico compuesto por dos sensores (luminosidad y temperatura), dos actuadores (persiana y aire acondicionado), y un servidor que actúa a modo de panel de control, mostrando el estado actual y dando opciones de uso de los actuadores.  El servidor cuenta con un agente con notificaciones y toma de decisiones básicas.

- **Los sensores **difunden información acerca de las medidas tomadas a través del servidor.
  - Estas medidas se simularán proporcionándolas manualmente mediante un formulario de entrada. 
  - La introducción en el formulario de una nueva medida conllevará la publicación del correspondiente evento con dicha medida. 
  - La misma página mostrará los cambios que se produzcan en el estado de los actuadores. 
- **El servidor **proporcionará el formulario de los sensores y la página de acceso del usuario.
  - Mantendrá las subscripciones a los eventos relacionados con luminosidad y temperatura de los usuarios que estén accediendo al sistema y del agente.
  - Guardará en la base de datos un historial de eventos (cambios en los sensores) producidos en el sistema junto con su marca de tiempo correspondiente.
- **Los usuarios** accederán al estado del sistema a través del **servidor**, mediante una página de acceso que éste le envía.
  - Esta página mostrará los estados de luminosidad y temperatura según se produzcan.
  - También contará con opciones de abrir/cerrar la persiana y encender/apagar el aire acondicionado.
- **El agente** detectará cuándo se sobrepasan ciertos umbrales (máximos y mínimos) de luminosidad y temperatura.
  - En tal caso, enviará alarmas a todos los usuarios conectados.
  - Además, si se sobrepasan simultáneamente los máximos de luminosidad y temperatura, cerrará la persiana automáticamente.

### Desarrollo

La luminosidad máxima es de 110000, la cantidad de lux (unidad del Sistema Internacional para iluminancia, equivalente a un lumen/m²) estimada para [luz diurna brillante](https://en.wikipedia.org/wiki/Daylight#Intensity_in_different_conditions). Cualquier cosa por debajo de 1 lux se considera noche. El mínimo es 0 lux.

La temperatura máxima es de 50ºC y la mínima de -30ºC (aunque en la tierra se han llegado a registrar temperaturas de -90ºC).



