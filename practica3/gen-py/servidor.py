import glob
import sys

from calculadora import Calculadora

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

import numpy as np
import logging

logging.basicConfig(level=logging.DEBUG)


class CalculadoraHandler:
    def __init__(self):
        self.log = {}

    def ping(self):
        print("Pong!")

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

################################################################################

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

################################################################################

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


################################################################################

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
