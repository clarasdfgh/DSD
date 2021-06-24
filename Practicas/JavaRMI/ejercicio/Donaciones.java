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
