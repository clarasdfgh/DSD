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
	result.calc_resultado_u.resultado = arg1.a*1.0 + arg1.b*1.0;

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

calc_resultado_vec *
vsuma_2_svc(vec arg1, vec arg2,  struct svc_req *rqstp)
{
	static calc_resultado_vec  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_vec, &result);

	int len  = arg1.vec_len;
	result.calc_resultado_vec_u.resultado.vec_len = len;
	result.calc_resultado_vec_u.resultado.vec_val = malloc(len * sizeof(double));


	for(int i = 0; i < len; i++){
		result.calc_resultado_vec_u.resultado.vec_val[i] = arg1.vec_val[i] + arg2.vec_val[i];
	}

	return &result;
}

calc_resultado_vec *
vresta_2_svc(vec arg1, vec arg2,  struct svc_req *rqstp)
{
	static calc_resultado_vec  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_vec, &result);

	int len  = arg1.vec_len;
	result.calc_resultado_vec_u.resultado.vec_len = len;
	result.calc_resultado_vec_u.resultado.vec_val = malloc(len * sizeof(double));


	for(int i = 0; i < len; i++){
		result.calc_resultado_vec_u.resultado.vec_val[i] = arg1.vec_val[i] - arg2.vec_val[i];
	}

	return &result;
}

calc_resultado_vec *
vmultiplicacion_2_svc(vec arg1, vec arg2,  struct svc_req *rqstp)
{
	static calc_resultado_vec  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_vec, &result);

	int len  = arg1.vec_len;
	result.calc_resultado_vec_u.resultado.vec_len = len;
	result.calc_resultado_vec_u.resultado.vec_val = malloc(len * sizeof(double));


	for(int i = 0; i < len; i++){
		result.calc_resultado_vec_u.resultado.vec_val[i] = arg1.vec_val[i] * arg2.vec_val[i];
	}

	return &result;
}

calc_resultado_vec *
vdivision_2_svc(vec arg1, vec arg2,  struct svc_req *rqstp)
{
	static calc_resultado_vec  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_vec, &result);

	int len  = arg1.vec_len;
	result.calc_resultado_vec_u.resultado.vec_len = len;
	result.calc_resultado_vec_u.resultado.vec_val = malloc(len * sizeof(double));


	for(int i = 0; i < len; i++){
		result.calc_resultado_vec_u.resultado.vec_val[i] = arg1.vec_val[i] / arg2.vec_val[i];
	}

	return &result;
}


calc_resultado_matr *
msuma_3_svc(matr arg1, matr arg2,  struct svc_req *rqstp)
{
	static calc_resultado_matr  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_matr, &result);

	int len  = arg1.matr_len;

	result.calc_resultado_matr_u.resultado.matr_len = len;
	result.calc_resultado_matr_u.resultado.matr_val = malloc(len * len * sizeof(double));

	for(int i = 0; i < len; i++){
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_len = len;
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_val = malloc(len * len * sizeof(double));
	}


	for(int i = 0; i < len; i++){
		for(int j = 0; j < len; j++){
			result.calc_resultado_matr_u.resultado.matr_val[i].vec_val[j] = arg1.matr_val[i].vec_val[j] + arg2.matr_val[i].vec_val[j];
		}
	}


	return &result;
}

calc_resultado_matr *
mresta_3_svc(matr arg1, matr arg2,  struct svc_req *rqstp)
{
	static calc_resultado_matr  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_matr, &result);

	int len  = arg1.matr_len;

	result.calc_resultado_matr_u.resultado.matr_len = len;
	result.calc_resultado_matr_u.resultado.matr_val = malloc(len * len * sizeof(double));

	for(int i = 0; i < len; i++){
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_len = len;
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_val = malloc(len * len * sizeof(double));
	}


	for(int i = 0; i < len; i++){
		for(int j = 0; j < len; j++){
			result.calc_resultado_matr_u.resultado.matr_val[i].vec_val[j] = arg1.matr_val[i].vec_val[j] - arg2.matr_val[i].vec_val[j];
		}
	}


	return &result;
}

calc_resultado_matr *
mmultiplicacion_3_svc(matr arg1, matr arg2,  struct svc_req *rqstp)
{
	static calc_resultado_matr  result;

	xdr_free((xdrproc_t)xdr_calc_resultado_matr, &result);

	int len  = arg1.matr_len;

	result.calc_resultado_matr_u.resultado.matr_len = len;
	result.calc_resultado_matr_u.resultado.matr_val = malloc(len * len * sizeof(double));

	for(int i = 0; i < len; i++){
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_len = len;
		result.calc_resultado_matr_u.resultado.matr_val[i].vec_val = malloc(len * len * sizeof(double));
	}


	for(int i = 0; i < len; i++){
		for(int j = 0; j < len; j++){
			int suma = 0;
			for(int k = 0; k < len; k++){
				suma += arg1.matr_val[j].vec_val[k] * arg2.matr_val[k].vec_val[i];
			}
			result.calc_resultado_matr_u.resultado.matr_val[j].vec_val[i] = suma;
		}
	}


	return &result;
}
