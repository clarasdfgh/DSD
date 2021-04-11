/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "calculadora.h"

bool_t
xdr_vec (XDR *xdrs, vec *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->vec_val, (u_int *) &objp->vec_len, ~0,
		sizeof (double), (xdrproc_t) xdr_double))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_matr (XDR *xdrs, matr *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->matr_val, (u_int *) &objp->matr_len, ~0,
		sizeof (vec), (xdrproc_t) xdr_vec))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_calc_resultado (XDR *xdrs, calc_resultado *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->err))
		 return FALSE;
	switch (objp->err) {
	case 0:
		 if (!xdr_double (xdrs, &objp->calc_resultado_u.resultado))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_calc_resultado_vec (XDR *xdrs, calc_resultado_vec *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->err))
		 return FALSE;
	switch (objp->err) {
	case 0:
		 if (!xdr_vec (xdrs, &objp->calc_resultado_vec_u.resultado))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_calc_resultado_matr (XDR *xdrs, calc_resultado_matr *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->err))
		 return FALSE;
	switch (objp->err) {
	case 0:
		 if (!xdr_matr (xdrs, &objp->calc_resultado_matr_u.resultado))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_operandos (XDR *xdrs, operandos *objp)
{
	register int32_t *buf;

	 if (!xdr_double (xdrs, &objp->a))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->b))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_vsuma_2_argument (XDR *xdrs, vsuma_2_argument *objp)
{
	 if (!xdr_vec (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_vec (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_vresta_2_argument (XDR *xdrs, vresta_2_argument *objp)
{
	 if (!xdr_vec (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_vec (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_vmultiplicacion_2_argument (XDR *xdrs, vmultiplicacion_2_argument *objp)
{
	 if (!xdr_vec (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_vec (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_vdivision_2_argument (XDR *xdrs, vdivision_2_argument *objp)
{
	 if (!xdr_vec (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_vec (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_msuma_3_argument (XDR *xdrs, msuma_3_argument *objp)
{
	 if (!xdr_matr (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_matr (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mresta_3_argument (XDR *xdrs, mresta_3_argument *objp)
{
	 if (!xdr_matr (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_matr (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mmultiplicacion_3_argument (XDR *xdrs, mmultiplicacion_3_argument *objp)
{
	 if (!xdr_matr (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_matr (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}
