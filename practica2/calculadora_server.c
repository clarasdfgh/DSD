/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calculadora.h"

calc_resultado *
suma_1_svc(operandos arg1,  struct svc_req *rqstp)
{
	static calc_resultado  result;

	xdr_free((xdrproc_t)xdr_calc_resultado, &result);
	result.calc_resultado_u.resultado = arg1.a + arg1.b;

	return &result;
}

calc_resultado *
resta_1_svc(operandos arg1,  struct svc_req *rqstp)
{
	static calc_resultado  result;

	xdr_free((xdrproc_t)xdr_calc_resultado, &result);
	result.calc_resultado_u.resultado = arg1.a - arg1.b;

	return &result;
}

calc_resultado *
multiplicacion_1_svc(operandos arg1,  struct svc_req *rqstp)
{
	static calc_resultado  result;

	xdr_free((xdrproc_t)xdr_calc_resultado, &result);
	result.calc_resultado_u.resultado = arg1.a * arg1.b;


	return &result;
}

calc_resultado *
division_1_svc(operandos arg1,  struct svc_req *rqstp)
{
	static calc_resultado  result;

	xdr_free((xdrproc_t)xdr_calc_resultado, &result);
	result.calc_resultado_u.resultado = arg1.a / arg1.b;


	return &result;
}