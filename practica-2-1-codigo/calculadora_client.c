/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calculadora.h"


void
calculadora_1(char *host, operandos mis_operandos, int operador)
{
	CLIENT *clnt;
	calc_resultado  *result_1;

	xdr_free((xdrproc_t)xdr_calc_resultado, &result_1);

#ifndef	DEBUG
	clnt = clnt_create (host, CALCULADORA, CALC_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	switch (operador) {
		case 1:
			result_1 = suma_1(mis_operandos, clnt);
			printf("%i + %i = %lf\n", mis_operandos.a, mis_operandos.b, result_1->calc_resultado_u.resultado);
		break;

		case 2:
			result_1 = resta_1(mis_operandos, clnt);
			printf("%i - %i = %lf\n", mis_operandos.a, mis_operandos.b, result_1->calc_resultado_u.resultado);
		break;

		case 3:
			result_1 = multiplicacion_1(mis_operandos, clnt);
			printf("%i * %i = %lf\n", mis_operandos.a, mis_operandos.b, result_1->calc_resultado_u.resultado);
		break;

		case 4:
			result_1 = division_1(mis_operandos, clnt);
			printf("%i / %i = %lf\n", mis_operandos.a, mis_operandos.b, result_1->calc_resultado_u.resultado);
		break;
	}

	if (result_1 == (calc_resultado *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	xdr_free((xdrproc_t)xdr_calc_resultado, result_1);

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	operandos mis_operandos;
	int mi_operador;

	if (argc != 5) {
		printf ("usage: %s server_host a b operator_id\n", argv[0]);
		exit (1);
	}

	host = argv[1];
	mis_operandos.a = atoi(argv[2]);
	mis_operandos.b = atoi(argv[3]);
	mi_operador = atoi(argv[4]);

				printf("asignados argumentos host:%s a:%u b:%u operador:%u\n",host,mis_operandos.a,mis_operandos.b, mi_operador);
				int sum, rest, mult, div;
				sum = mis_operandos.a +mis_operandos.b;
				rest = mis_operandos.a -mis_operandos.b;
				mult = mis_operandos.a *mis_operandos.b;
				div = mis_operandos.a /mis_operandos.b;
				printf("sum:%i rest:%i mult:%i div:%i\n",sum, rest, mult, div );

	calculadora_1 (host, mis_operandos, mi_operador);
exit (0);
}