typedef list<double> vec
typedef list<list<double>> matr

service Calculadora{
   void ping(),
   double suma(1:double num1, 2:double num2),
   double resta(1:double num1, 2:double num2),
   double multiplicacion(1:double num1, 2:double num2),
   double division(1:double num1, 2:double num2),

   vec vsuma(1:vec v1, 2:vec v2),
   vec vresta(1:vec v1, 2:vec v2),
   vec vmultiplicacion(1:vec v1, 2:vec v2),
   vec vdivision(1:vec v1, 2:vec v2),

   matr msuma(1:matr mat1, 2:matr mat2),
   matr mresta(1:matr mat1, 2:matr mat2),
   matr mmultiplicacion(1:matr mat1, 2:matr mat2),
}
