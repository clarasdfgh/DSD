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
