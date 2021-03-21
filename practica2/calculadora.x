union calc_resultado switch(int err) {
	case 0:
		int resultado;
	default:
		void;
};

struct operandos {
        int a;
        int b;
};

program CALCULADORA{
        version CALC_VERS {
                calc_resultado SUMA(operandos) = 1;
                calc_resultado RESTA(operandos) = 2;
                calc_resultado MULTIPLICACION(operandos) = 3;
                calc_resultado DIVISION(operandos) = 4;
        } = 1;
} = 0x20202020;
