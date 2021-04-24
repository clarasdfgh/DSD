from calculadora import Calculadora

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

import numpy as np

transport = TSocket.TSocket("localhost", 9090)
transport = TTransport.TBufferedTransport(transport)
protocol = TBinaryProtocol.TBinaryProtocol(transport)

client = Calculadora.Client(protocol)

################################################################################

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

#-------------------------------------------------------------------------------

def operacion_vec(arg1, operador, arg2):
    resultado = []

    if operador == "+":
        resultado = client.vsuma(arg1,arg2)
    elif operador == "-":
        resultado = client.vresta(arg1,arg2)
    elif operador == "*":
        resultado = client.vmultiplicacion(arg1,arg2)
    elif operador == "/":
        resultado = client.vdivision(arg1,arg2)
    else:
        resultado = "Error en la operación"
    return resultado

#-------------------------------------------------------------------------------

def operacion_matr(arg1, operador, arg2):
    if operador == "+":
        resultado = client.msuma(arg1,arg2)
    elif operador == "-":
        resultado = client.mresta(arg1,arg2)
    elif operador == "*":
        resultado = client.mmultiplicacion(arg1,arg2)
    else:
        resultado = "Error en la operación"
    return resultado

transport.open()

################################################################################

print("Ping!")
client.ping()

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


transport.close()
