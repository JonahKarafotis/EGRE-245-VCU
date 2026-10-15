#include "complex.h"
#include <stdio.h>
#include <stdlib.h>

struct ComplexType {
    double real;
    double imag;
};

// Create a new complex number with the given real and imaginary parts.
complex CreatComplex(double real, double imag) {
    complex c = (complex)malloc(sizeof(struct ComplexType));
    if (c == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    c->real = real;
    c->imag = imag;
    return c;
}

// Free the memory used by a complex number.
void DestroyComplex(complex c) {
    free(c);
}

// Return the real part of a complex number.
double GetReal(const complex c) {
    return c->real;
}

// Return the imaginary part of a complex number.
double GetImaginary(const complex c) {
    return c->imag;
}

// Compute the magnitude of a complex number.
// The magnitude is defined as sqrt(real^2 + imag^2).
double ComplexMagnitude(const complex c) {
    return sqrt(c->real * c->real + c->imag * c->imag);
}

// Add two complex numbers together.
// The result is a new complex number.
complex AddComplex(const complex c1, const complex c2) {
    return CreatComplex(c1->real + c2->real, c1->imag + c2->imag);
}

// Subtract the second complex number from the first.
// The result is a new complex number.
complex SubtractComplex(const complex c1, const complex c2) {
    return CreatComplex(c1->real - c2->real, c1->imag - c2->imag);
}

// Multiply two complex numbers.
// The result is a new complex number.
// This can be found by using the formula: (a + bi)(c + di) = (ac - bd) + (ad + bc)i
complex MultiplyComplex(const complex c1, const complex c2) {
    double real = c1->real * c2->real - c1->imag * c2->imag;
    double imag = c1->real * c2->imag + c1->imag * c2->real;
    return CreatComplex(real, imag);
}

// Divide the first complex number by the second.
// The result is a new complex number.
// This can be found by using the formula: (a + bi) / (c + di) = ((ac + bd) / (c^2 + d^2)) + ((bc - ad) / (c^2 + d^2))i
complex DivideComplex(const complex c1, const complex c2) {
    double denominator = c2->real * c2->real + c2->imag * c2->imag;
    if (denominator == 0) {
        fprintf(stderr, "Division by zero\n");
        exit(EXIT_FAILURE);
    }
    double real = (c1->real * c2->real + c1->imag * c2->imag) / denominator;
    double imag = (c1->imag * c2->real - c1->real * c2->imag) / denominator;
    return CreatComplex(real, imag);
}

// Return the complex conjugate of a complex number.
// The result is a new complex number.
// The conjugate of a complex number a + bi is defined as a - bi.
complex ConjugateComplex(const complex c) {
    return CreatComplex(c->real, -c->imag);
}

// Square a complex number by multiplying it by itself.
// The result is a new complex number.
// This can be found by using the formula: (a + bi)^2 = (a^2 - b^2) + (2ab)i
complex SquareComplex(const complex c) {
    return MultiplyComplex(c, c); // reuses MultiplyComplex to square the complex number
}

// Print a complex number in a readable format.
void DisplayComplex(const complex c1) {
    // As long as the imaginary part is non-negative, we can print it in the form "a + bi".
    if (c1->imag >= 0) {
        // Print the complex number in the form "a + bi"
        // Use %.2f to format the real and imaginary parts to two decimal places
        printf("%.2f + %.2fi\n", c1->real, c1->imag);
    } else {
        // Print the complex number in the form "a - bi"
        // Use %.2f to format the real and imaginary parts to two decimal places
        printf("%.2f - %.2fi\n", c1->real, -c1->imag);
    }
}
