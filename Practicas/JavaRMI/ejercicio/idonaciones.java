import java.rmi.Remote;
import java.rmi.RemoteException;

public interface idonaciones extends Remote {
  public boolean checkRegistro(int id_cliente)            throws RemoteException;
  public boolean checkDeposito(int id_cliente)            throws RemoteException;
  public int  numRegistrados()                         throws RemoteException;
  public int  nextReplica()                            throws RemoteException;
  public void registrar()                              throws RemoteException;
  public void depositar(int id_cliente, int cantidad)  throws RemoteException;
  public int  recaudado()                              throws RemoteException;
}
