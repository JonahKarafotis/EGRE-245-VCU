#ifndef COMPLEX_H
#define COMPLEX_H
#include <math.h>

typedef struct ComplexType *complex;

complex CreatComplex(double real, double imag);
void DestroyComplex(complex c);
double GetReal(const complex c);
double GetImaginary(const complex c);
double ComplexMagnitude(const complex c);
complex AddComplex(const complex c1, const complex c2);
complex SubtractComplex(const complex c1, const complex c2);
complex MultiplyComplex(const complex c1, const complex c2);
complex DivideComplex(const complex c1, const complex c2);
complex ConjugateComplex(const complex c);
complex SquareComplex(const complex c);
void DisplayComplex(const complex c1);

#endif


