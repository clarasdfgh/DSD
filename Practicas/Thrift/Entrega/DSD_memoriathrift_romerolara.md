# DSD 20-21, Práctica 2: Calculadora Thrift

###### Clara Mª Romero Lara - 3º GII, IS

## 1. Introducción

Esta segunda práctica tenía el mismo planteamiento que la primera: desarrollar una calculadora funcional con un planteamiento cliente-servidor. En este caso, la herramienta para ello ha sido Apache Thrift y Python. 

Se exigían unas operaciones mínimas, y se animaba a extender su funcionalidad con estructuras de datos más complejas. En esta práctica se han implementado operaciones simples (suma, resta, multiplicación y división) y operaciones con vectores y matrices.

## 2. Uso

### 2.1. Prerrequisitos

Esta práctica se ha desarrollado en Arch Linux, versión 5.11.11-arch1-1. Para ejecutarla correctamente, se requieren los siguientes paquetes:

- thrift 0.14.1-1
- python 3.9.3-1

Adicionalmente se requieren módulos python:

- `pip install thrift `
- `pip install numpy `

### 2.2. Desplegado

En `calculadora.thrift` definimos las operaciones y estructuras de datos. Generamos todos los archivos para el sistema cliente-servidor de la siguiente forma:

```
> thrift -gen py calculadora.thrift
```

Esto genera múltiples archivos dentro de un directorio llamado ``gen-py``, dentro del cual debemos meter los archivos `cliente.py` y `servidor.py`.

### 2.3. Uso

Para ejecutar nuestra calculadora, debemos primero tener una instancia del servidor activa:

```
> python servidor.py
```

Y luego el cliente:

```
> python servidor.py
```

El programa nos guía para seleccionar el tipo de operación. En el caso de vectores y matrices, nos guía también a la hora de declararlos. En cualquier caso, ante una entrada incorrecta, la calculadora devolverá un error.

Algunos ejemplos:

![image-20210424113506227](/home/clara/.config/Typora/typora-user-images/image-20210424113506227.png)

![image-20210424113605099](/home/clara/.config/Typora/typora-user-images/image-20210424113605099.png)

![image-20210424113723362](/home/clara/.config/Typora/typora-user-images/image-20210424113723362.png)

## 3. Calculadora

### 3.1. Calculadora.thrift

Thrift es un IDL (*Interface Definition Language*) multilenguaje. Cuenta con un compilador que genera el código del sistema, y librerías para ejecutarlo en multitud de lenguajes.

En `calculadora.thift` definimos las operaciones y estructuras de datos que vamos a usar.

#### Estructuras de datos

Thrift cuenta con tipos de dato double para el punto flotante (son compatibles con float de python) y soporta el uso de contenedores como listas.

- `typedef list<double> vec`: lista de double para los vectores
- `typedef list< list<double>> matr`: lista de vectores para las matrices

#### Operaciones

- **Operaciones básicas:**

  ```c
     double suma(1:double num1, 2:double num2),
     double resta(1:double num1, 2:double num2),
     double multiplicacion(1:double num1, 2:double num2),
     double division(1:double num1, 2:double num2),
  ```

- **Operaciones con vectores:**

  ```c
     vec vsuma(1:vec v1, 2:vec v2),
     vec vresta(1:vec v1, 2:vec v2),
     vec vmultiplicacion(1:vec v1, 2:vec v2),
     vec vdivision(1:vec v1, 2:vec v2),
  ```

- **Operaciones con matrices:**

  ```c
     matr msuma(1:matr mat1, 2:matr mat2),
     matr mresta(1:matr mat1, 2:matr mat2),
     matr mmultiplicacion(1:matr mat1, 2:matr mat2),
  ```

También existe una función `ping()`, para comprobar que la conexión cliente-servidor es correcta.

### 3.2. Cliente

En el cliente se inicializa la conexión al servidor (todo en local) y se comprueba con un ping.

```python
transport = TSocket.TSocket("localhost", 9090)
transport = TTransport.TBufferedTransport(transport)
protocol = TBinaryProtocol.TBinaryProtocol(transport)

client = Calculadora.Client(protocol)

print("Ping!")
client.ping()
```

A partir de este punto, se solicita una entrada para escoger el tipo de operación:

```python
print("Seleccione el tipo de operación: ")
print(" 1.Operación simple")
print(" 2.Operación con vectores ")
print(" 3.Operación con matrices")
entrada = int(input())

#-------------------------------------------------------------------------------
if entrada == 1:

    arg1, operador, arg2 = input("Introduzca su operación simple (use espacios): ").split()
    arg1 = float(arg1)
    arg2 = float(arg2)

    resultado = operacion_simple(arg1, operador, arg2)
    print(str(arg1) + " " + str(operador) + " " + str(arg2) + " = " + str(resultado))

#-------------------------------------------------------------------------------
elif entrada == 2:
    tam = int(input("Introduzca la longitud de los vectores: "))

    vecA = []
    vecB = []

    for x in range(tam):
        elemento = float(input("Introduzca elemento " + str(x) + " del vector A (quedan " + str(tam-x) + "): "))
        vecA.append(elemento)

    print("")

    for x in range(tam):
        elemento = float(input("Introduzca elemento " + str(x) + " del vector B (quedan " + str(tam-x) + "): "))
        vecB.append(elemento)

    print("")
    operador = input("Intoduzca su operador: ")

    resultado = operacion_vec(vecA, operador, vecB)

    print(str(vecA) + " " + operador )
    print(str(vecB) + " = ")
    print(resultado)


#-------------------------------------------------------------------------------
elif entrada == 3:
    tam = int(input("Introduzca el tamaño de las matrices (solo opera en matrices cuadradas): "))

    matrA = np.zeros((tam, tam))
    matrB = np.zeros((tam, tam))

    for x in range(tam):
        for y in range(tam):
            elemento = float(input("Introduzca elemento " + str(x) + str(y) + " de la matriz A: "))
            matrA[x,y] = float(elemento)

    print("")

    for x in range(tam):
        for y in range(tam):
            elemento = float(input("Introduzca elemento " + str(x) + str(y) + " de la matriz B: "))
            matrB[x,y] = float(elemento)

    print("")
    operador = input("Intoduzca su operador: ")

    resultado = np.zeros((tam,tam))
    resultado = operacion_matr(matrA, operador, matrB)

    print(str(matrA) + " " + operador )
    print(str(matrB) + " = ")
    print("")
    print(str(resultado));

else:
    print("Entrada inválida")
    exit(1)

```

En el caso de las matrices, se declaran con un `ndarray` de numpy inicializado a ceros para facilitar su manejo.

Cada una de las funciones `operacion` selecciona la llamada correcta al servidor basándose en el operador. Las tres (`operacion_simple`, `operacion_vec`, `operacion_matr`) son prácticamente iguales:

```python
def operacion_simple(arg1, operador, arg2):
    if operador == "+":
        resultado = client.suma(arg1,arg2)
    elif operador == "-":
        resultado = client.resta(arg1,arg2)
    elif operador == "*":
        resultado = client.multiplicacion(arg1,arg2)
    elif operador == "/":
        resultado = client.division(arg1,arg2)
    else:
        resultado = "Error en la operación"
    return resultado
```

 Finalmente, se cierra la conexión con `transport.close()` .

### 3.3. Servidor

Se definen las operaciones dentro de `CalculadoraHandler`. Aquí las operaciones simples:

```python
def suma(self, n1, n2):
        print("sumando " + str(n1) + " con " + str(n2))
        return n1 + n2

    def resta(self, n1, n2):
        print("restando " + str(n1) + " con " + str(n2))
        return n1 - n2

    def multiplicacion(self, n1, n2):
        print("multiplicando " + str(n1) + " con " + str(n2))
        return n1 * n2

    def division(self, n1, n2):
        print("dividiendo " + str(n1) + " con " + str(n2))
        return n1 / n2
```

Las operaciones con vectores. Hay que tener en cuenta que no son técnicamente operaciones con vectores, sino operaciones con listas de números. 

Hemos empleado numpy para más facilidad:

```python
def vsuma(self, n1, n2):
        print("sumando vector A con vector B")
        v1 = np.array(n1)
        v2 = np.array(n2)
        return v1 + v2

    def vresta(self, n1, n2):
        print("restando vector A con vector B")
        v1 = np.array(n1)
        v2 = np.array(n2)
        return v1 - v2

    def vmultiplicacion(self, n1, n2):
        print("multiplicando vector A con vector B")
        v1 = np.array(n1)
        v2 = np.array(n2)
        return v1 * v2

    def vdivision(self, n1, n2):
        print("dividiendo vector A con vector B")
        v1 = np.array(n1)
        v2 = np.array(n2)
        return v1 / v2
```

Y las operaciones con matrices. No existe la división de matrices como tal, ya que esta se define como la multiplicación por la matriz inversa. 

Las matrices las definimos con numpy, pero las calculamos manualmente ya que esta funcionalidad de numpy ha sido *deprecated* recientemente. Al uso, son `ndarray`.

```python
def msuma(self, n1, n2):
        print("sumando matriz A con matriz B")

        m1 = np.matrix(n1, float, 1)
        m2 = np.matrix(n2, float, 1)
        tam = len(m1)
        resultado = np.zeros((tam, tam))

        for x in range(tam):
            for y in range(tam):
                resultado[x,y] = m1[x,y] + m2[x,y]
        return resultado

    def mresta(self, n1, n2):
        print("restando matriz A con matriz B")

        m1 = np.matrix(n1, float, 1)
        m2 = np.matrix(n2, float, 1)
        tam = len(m1)
        resultado = np.zeros((tam, tam))

        for x in range(tam):
            for y in range(tam):
                resultado[x,y] = m1[x,y] - m2[x,y]
        return resultado

    def mmultiplicacion(self, n1, n2):
        print("multiplicando matriz A con matriz B")

        m1 = np.matrix(n1, float, 1)
        m2 = np.matrix(n2, float, 1)
        tam = len(m1)
        resultado = np.zeros((tam, tam))

        for x in range(tam):
            for y in range(tam):
                for z in range(tam):
                    resultado[x,y] += m1[x,z] * m2[z,y]
        return resultado
```

Y se inicializa la conexión del servidor.

```python
if __name__ == "__main__":
    handler = CalculadoraHandler()
    processor = Calculadora.Processor(handler)
    transport = TSocket.TServerSocket(host="127.0.0.1", port=9090)
    tfactory = TTransport.TBufferedTransportFactory()
    pfactory = TBinaryProtocol.TBinaryProtocolFactory()

    server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)

    print("iniciando servidor...")
    server.serve()
    print("fin")

```

