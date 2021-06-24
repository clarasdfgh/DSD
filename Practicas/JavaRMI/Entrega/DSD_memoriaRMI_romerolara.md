# DSD 20-21, Práctica 3: Java RMI

###### Clara Mª Romero Lara - 3º GII, IS

## 1. Introducción

En esta práctica vamos a trabajar con Java RMI, un mecanismo de invocación remota de métodos. Es nuestro primer acercamiento a los objetos distribuidos, ya que las prácticas anteriores de RPC (Sun RPC y Thrift) solo cuentan con métodos primitivos.

La primera parte de la práctica consiste en demostrar el correcto funcionamiento de algunos ejemplos; y la segunda en el desarrollo de un sistema gestor de donaciones a una causa benéfica.

## 2. Ejemplos

### Ejemplo 1: invocación de un método remoto

El primer ejemplo consiste en un programa cliente-servidor simple en el que el servidor (`ejemplo.java`) exporta los métodos de la interfaz (`ejemplo_1.java`)  del objeto remoto instanciado como `ejemplo_1` de la clase definida en `ejemplo.java`. 

El cliente activa el gestor de seguridad, busca el objeto remoto, e invoca el método `escribir_mensaje()` con el argumento recibido por consola.

#### Compilación y ejecución

Para compilar:

```bash
# directorio: ejemplos/ejemplo1
javac *.java
```

Para ejecutar (cada bloque representa una terminal nueva):

```bash
# lanzar el ligador RMI
rmiregistry
```

```bash
# iniciar el servidor
java -cp . -Djava.rmi.server.codebase=file:./ -Djava.rmi.server.hostname=localhost -Djava.security.policy=server.policy Ejemplo
```

```bash
# iniciar el cliente (arg: localhost 0)
java -cp . -Djava.security.policy=server.policy Cliente_Ejemplo localhost 0
```

#### Capturas de pantalla

![image-20210512155631107](/home/clara/.config/Typora/typora-user-images/image-20210512155631107.png)

### Ejemplo 2: hebras

Este ejemplo es muy similar al anterior, pero en este caso lanzaremos varias hebras que accederan al método remoto.  

#### Compilación y ejecución

Para compilar:

```bash
# directorio: ejemplos/ejemplo2
javac *.java
```

Para ejecutar (cada bloque representa una terminal nueva):

```bash
# lanzamos el ligador RMI
rmiregistry
```

```bash
# iniciar el servidor
java -cp . -Djava.rmi.server.codebase=file:./ -Djava.rmi.server.hostname=localhost -Djava.security.policy=server.policy Ejemplo
```

```bash
# iniciar el cliente (arg: localhost 11)
java -cp . -Djava.security.policy=server.policy Cliente_Ejemplo_Multi_Threaded localhost 11
```

#### Capturas de pantalla

![image-20210512162718952](/home/clara/.config/Typora/typora-user-images/image-20210512162718952.png)

Como podemos comprobar, se pone a dormir en las hebras acabadas en 0.

### Ejemplo 3: contador

En este programa contamos con una clase contador a la que accede el cliente remotamente. El cliente inicializa el contador, lo incrementa 1000 veces y devuelve el valor del contador junto con el tiempo de respuesta medio del método `incrementar()`.

#### Compilación y ejecución

Para compilar:

```bash
# directorio: ejemplos/ejemplo3
javac *.java
```

Para ejecutar (cada bloque representa una terminal nueva):

```bash
# lanzamos el ligador RMI
rmiregistry
```

```bash
# iniciar el servidor
java -cp . -Djava.rmi.server.codebase=file:./ -Djava.rmi.server.hostname=localhost -Djava.security.policy=server.policy servidor
```

```bash
# iniciar el cliente (arg: localhost)
java -cp . -Djava.security.policy=server.policy cliente localhost
```

#### Capturas de pantalla

![image-20210512171334118](/home/clara/.config/Typora/typora-user-images/image-20210512171334118.png)

## 3. Servidor replicado: gestor de donaciones

Desarrollaremos un servidor con dos réplicas, cada una desplegada en una máquina diferente, que gestionará donaciones recibidas para una causa humanitaria. Debe incluir en su funcionalidad:

- Dos operaciones: registro de una entidad interesada (cliente), y depósito de una donación. No es posible depositar una donación sin registrarse primero.
- Cuando se trata de realizar un registro y se contacta a una de las réplicas, este ocurrirá realmente y de forma transparente en la réplica con menos entidades registradas. Es decir: el cliente se ha dirigido solo a una réplica, incluso si finalmente no se registra en ella.
  - A partir de ese momento, sus depósitos se dirigiran a la réplica en la que ha ocurrido el registro.
- Cada réplica del servidor mantendrá el subtotal de las donaciones realizadas en dicha replica. 
- Un cliente no podrá registrarse más de una vez, ni siquiera en replicas distintas. 
- Los servidores ofrecen una operación de consulta del total donado a la causa. Esta operación solo puede realizarse si el cliente se ha registrado y ha hecho un depósito previamente.
  - Cuando el cliente consulte la cantidad donada hasta el momento, solo se hará petición a su réplica: ésta es la encargada de consultar con la otra réplica y devolver el total.

### Código

```java
// Cliente.java
import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;
import java.util.ArrayList;
import java.lang.*;

public class Cliente {
    String host;
    String replica = "null";

    Cliente (String host){
      this.host = host;
    }

    void asignarReplica (String replica){
      this.replica = replica;
    }

  public static void main(String args[]) {
    try {
      String host           = "localhost";
      ArrayList<Cliente> clientes = new ArrayList<>();

      Registry reg           = LocateRegistry.getRegistry(host, 1099);
      idonaciones replica_inicial      = (idonaciones) reg.lookup("Replica0");

      Donaciones donaciones = new Donaciones(host, 0);

      Scanner entradaEscaner = new Scanner (System.in);
      int menu = -1;
      int entrada = -1;



      while (menu != 4) {
        System.out.println("\n\u001B[34m \t\t--- MENU --- \u001B[0m");
        System.out.println("\u001B[36m 1.Registrar entidad \u001B[0m");
        System.out.println("\u001B[36m 2.Hacer un deposito \u001B[0m");
        System.out.println("\u001B[36m 3.Consultar total recaudado \u001B[0m");
        System.out.println("\u001B[36m 4.Salir \u001B[0m");

        entradaEscaner = new Scanner (System.in);
        menu = entradaEscaner.nextInt();

        switch(menu) {
        	case 1:
            System.out.println("\tIniciando registro...");

            Cliente cliente = new Cliente(host);
            cliente.asignarReplica("" + donaciones.nextReplica());
            clientes.add(cliente);

            System.out.println("Gracias por registrarse!");
          break;

          case 2:
          System.out.print("Introduzca su ID: ");
          entradaEscaner = new Scanner (System.in);
          entrada = entradaEscaner.nextInt();

          if(donaciones.checkRegistro(entrada)){
            System.out.print("Introduzca la cantidad a depositar: ");
            entradaEscaner = new Scanner (System.in);
            int cantidad  = entradaEscaner.nextInt();

            if(entrada > 0){
              donaciones.depositar(entrada, cantidad);
              System.out.println("Depositado! Gracias por su donacion.");
            } else{
              System.out.println("Denegado: la donacion debe ser un entero positivo.");
            }
          } else{
            System.out.println("Denegado: el cliente no existe, registrese para poder donar.");
          }

          break;

          case 3:
          System.out.print("Introduzca su ID: ");
          entradaEscaner = new Scanner (System.in);
          entrada = entradaEscaner.nextInt();

          if(donaciones.checkRegistro(entrada) && donaciones.checkDeposito(entrada)){
            int total = donaciones.recaudado();
            System.out.println("El total recaudado es de: " + total + "€.");
          } else{
            System.out.println("Denegado: para consultar el total recaudado, registrese y realice un deposito.");
          }
          break;

          case 4:
            System.exit(0);
          break;
        }
      }
    } catch(Exception e) {
      System.out.println("Excepcion en el cliente: " + e.getMessage());
  }
  }
}
```

```java
// Servidor.java
import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.*;

public class Servidor {
  public static void main(String[] args) {
    int num_repl = 2;

    // Crea e instala el gestor de seguridad
    if (System.getSecurityManager() == null) {
      System.setSecurityManager(new SecurityManager());
    }

    try {
        System.out.println("Desplegando replicas...");

        for(int i = 0; i < num_repl; i++){
          Donaciones replica = new Donaciones("localhost", i);
          Naming.rebind("Replica" + i, replica);
          System.out.println("\tDesplegada replica " + i);
        }


    } catch (Exception e) {
        System.out.println("Excepcion en el servidor: " + e.getMessage());
    }
  }
}
```

```java
// idonaciones.java
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface idonaciones extends Remote {
  public boolean checkRegistro(int id_cliente)         throws RemoteException;
  public boolean checkDeposito(int id_cliente)         throws RemoteException;
  public int  numRegistrados()                         throws RemoteException;
  public int  nextReplica()                            throws RemoteException;
  public void registrar()                              throws RemoteException;
  public void depositar(int id_cliente, int cantidad)  throws RemoteException;
  public int  recaudado()                              throws RemoteException;
}
```

```java
// Donaciones.java
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;
import java.util.HashMap;
import java.rmi.registry.*;


public class Donaciones extends UnicastRemoteObject implements idonaciones {
  private HashMap<Integer, Integer> depositos = new HashMap<Integer, Integer>();
	private Registry reg;

  private static int num_repl = 2;
  private int id_replica;

  public Donaciones(String host, int id_replica) throws RemoteException {
    try {
      reg             = LocateRegistry.getRegistry(host, 1099);
      this.id_replica = id_replica;
    } catch(Exception e) {
      System.out.println("Excepcion en declaracion: " + e.getMessage());
    }
  }

  // Comprueba si el cliente id_cliente está registrado
  public boolean checkRegistro(int id_cliente) throws RemoteException {

    return depositos.containsKey(id_cliente);
  }

  //Comprueba si el cliente id_cliente ha realizado algún depósito
  public boolean checkDeposito(int id_cliente) throws RemoteException {

    return depositos.get(id_cliente) != 0;
  }

  //Devuelve el número de clientes registrados
  public int  numRegistrados() throws RemoteException{

    return depositos.size();
  }

  //Devuelve la réplica más vacía
  public int  nextReplica() throws RemoteException{
    int replica  = this.id_replica;
    int num_registrados_repl_actual = numRegistrados();
    idonaciones replica_candidata   = (idonaciones) this;

    for (int i = 0; i < num_repl; i++) {
      if (i != this.id_replica) {
        try{
          replica_candidata = (idonaciones) reg.lookup("Replica" + i);
        }catch(Exception e) {
          System.out.println("Excepcion en obtener replica: " + e.getMessage());
        }

        int num_registrados_repl_candidata = replica_candidata.numRegistrados();

        if (num_registrados_repl_candidata < num_registrados_repl_actual) {
          replica = i;
        }
      }
    }

    System.out.println("\tReplica con menos clientes: " + replica);

    try{
      if (replica != id_replica) {
        replica_candidata = (idonaciones) reg.lookup("Replica" + replica);
        replica_candidata.registrar();
      } else {
        registrar();
      }
    } catch(Exception e) {
      System.out.println("Excepcion en registro: " + e.getMessage());
    }

    return replica;
  }

  //Registra un nuevo cliente
  public void registrar() throws RemoteException {
    int siguiente = numRegistrados() + 1;

    depositos.put(siguiente, 0);

    System.out.println("\tSu ID de cliente: " + siguiente);
    System.out.println("\tSu replica: " + id_replica);
  }

  //Realiza un depósito de cantidad a nombre del cliente id_cliente
  public void depositar(int id_cliente, int cantidad) throws RemoteException {
    int sumatorio = depositos.get(id_cliente) + cantidad;

    depositos.put(id_cliente,sumatorio);
    System.out.println("\tCliente: "       + id_cliente);
    System.out.println("\tDeposito: "      + cantidad);
    System.out.println("\tSus depositos: " + sumatorio);
  }

  //Devuelve el total de dinero recaudado
  public int recaudado() throws RemoteException {
    int sumatorio = 0;

    for (int valor: depositos.values()) {
      sumatorio += valor;
    }

    return sumatorio;
  }
}
```

### Desarrollo

Contamos con la interfaz `idonaciones`, que define la clase principal `Donaciones` y es el punto de acceso remoto. 

En la clase `Donaciones` definimos métodos de comprobación para las restricciones indicadas: `checkRegistro` y `checkDeposito`. La estructura de datos en la que nos basamos es un *HashMap*, un diccionario que nos permite asociar `id_cliente` con la cantidad donada hasta el momento, que es 0 inicialmente. El método más interesante es `nextReplica` , que compara el `numRegistrados` de cada réplica y finalmente hace put mediante la réplica más vacía. 

En el servidor se desplegan las réplicas previa ejecución del cliente; y en el cliente un `ArrayList` de clientes asignan y llaman a la clase principal. El programa cuenta con un menú que nos permite elegir entre registrarnos, hacer una donación, consultar el total, y finalizar el programa.

### Compilación y ejecución

Para compilar:

```bash
# directorio: ejercicio
javac *.java
```

Para ejecutar (cada bloque representa una terminal nueva):

```bash
# lanzar el ligador RMI
rmiregistry
```

```bash
# iniciar el servidor
java -cp . -Djava.rmi.server.codebase=file:./ -Djava.rmi.server.hostname=localhost -Djava.security.policy=server.policy Servidor
```

```bash
# iniciar el cliente (arg: localhost)
java -cp . -Djava.security.policy=server.policy Cliente localhost
```

### Capturas de pantalla

![image-20210516170749944](/home/clara/.config/Typora/typora-user-images/image-20210516170749944.png)

Registrando dos clientes

![image-20210516170821665](/home/clara/.config/Typora/typora-user-images/image-20210516170821665.png)

Realización de un depósito (con y sin éxito)

![image-20210516170920250](/home/clara/.config/Typora/typora-user-images/image-20210516170920250.png)

Consulta del subtotal (con y sin éxito).

