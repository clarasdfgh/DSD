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
