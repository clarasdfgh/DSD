typedef double vec<>;

typedef vec matr<>;

union calc_resultado switch(int err) {
	case 0:
		double resultado;
	default:
		void;
};

union calc_resultado_vec switch(int err) {
	case 0:
		vec resultado;
	default:
		void;
};

union calc_resultado_matr switch(int err) {
	case 0:
		matr resultado;
	default:
		void;
};

struct operandos{
	double a;
	double b;
};

program CALCULADORA{
        version CALC_VERS_1 {
                calc_resultado SUMA(operandos) = 1;
                calc_resultado RESTA(operandos) = 2;
                calc_resultado MULTIPLICACION(operandos) = 3;
                calc_resultado DIVISION(operandos) = 4;
        } = 1;

				version CALC_VERS_2 {
                calc_resultado_vec VSUMA(vec, vec) = 1;
                calc_resultado_vec VRESTA(vec, vec) = 2;
                calc_resultado_vec VMULTIPLICACION(vec, vec) = 3;
                calc_resultado_vec VDIVISION(vec, vec) = 4;
        } = 2;

				version CALC_VERS_3 {
                calc_resultado_matr MSUMA(matr, matr) = 1;
                calc_resultado_matr MRESTA(matr, matr) = 2;
                calc_resultado_matr MMULTIPLICACION(matr, matr) = 3;
        } = 3;

} = 0x20202020;
