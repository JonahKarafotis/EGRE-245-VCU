/*
 * EGRE 246 - Complex ADT test driver
 * Compile this with complex.c and complex.h.
 * Submit only complex.c, as the assignment requires.
 */
#include "complex.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int total_tests = 0;
static int passed_tests = 0;

/* Floating-point answers should be compared with a tolerance. */
static int nearly_equal(double actual, double expected) {
    double scale = fmax(1.0, fmax(fabs(actual), fabs(expected)));
    return isfinite(actual) && fabs(actual - expected) <= 1e-9 * scale;
}

static void check_value(const char *name, double actual, double expected) {
    total_tests++;
    if (nearly_equal(actual, expected)) {
        passed_tests++;
        printf("[PASS] %-39s expected %.10g, got %.10g\n",
               name, expected, actual);
    } else {
        printf("[FAIL] %-39s expected %.10g, got %.10g\n",
               name, expected, actual);
    }
}

static void check_complex(const char *name, complex c,
                          double expected_real, double expected_imag) {
    total_tests++;
    if (c == NULL) {
        printf("[FAIL] %-39s returned NULL\n", name);
        return;
    }

    double actual_real = GetReal(c);
    double actual_imag = GetImaginary(c);
    if (nearly_equal(actual_real, expected_real) &&
        nearly_equal(actual_imag, expected_imag)) {
        passed_tests++;
        printf("[PASS] %-39s expected (%.10g, %.10g), got (%.10g, %.10g)\n",
               name, expected_real, expected_imag, actual_real, actual_imag);
    } else {
        printf("[FAIL] %-39s expected (%.10g, %.10g), got (%.10g, %.10g)\n",
               name, expected_real, expected_imag, actual_real, actual_imag);
    }
}

/* Arithmetic routines must return a NEW allocated complex number. */
static void check_result(const char *name, complex result,
                         double expected_real, double expected_imag,
                         complex input1, complex input2) {
    check_complex(name, result, expected_real, expected_imag);

    total_tests++;
    if (result != NULL && result != input1 && result != input2) {
        passed_tests++;
        printf("[PASS] %-39s returned a new object\n", name);
        DestroyComplex(result);
    } else {
        printf("[FAIL] %-39s did not return a new object\n", name);
        /* Do not free an aliased input: it is still needed below. */
    }
}

static void test_division_by_zero(void) {
    printf("Division-by-zero behavior is not specified by the assignment.\n");
    printf("Your implementation may exit the program or return NULL.\n");
    fflush(stdout);

    complex numerator = CreatComplex(1, 1);
    complex zero = CreatComplex(0, 0);
    if (numerator == NULL || zero == NULL) {
        fprintf(stderr, "Allocation failed in division-by-zero test.\n");
        exit(EXIT_FAILURE);
    }

    complex result = DivideComplex(numerator, zero);
    if (result == NULL) {
        printf("DivideComplex returned NULL for a zero divisor.\n");
    } else {
        printf("DivideComplex returned a value for a zero divisor; inspect this.\n");
        DestroyComplex(result);
    }
    DestroyComplex(numerator);
    DestroyComplex(zero);
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--divide-by-zero") == 0) {
        test_division_by_zero();
        return 0;
    }

    complex z1 = CreatComplex(3.0, 2.0);      /* 3 + 2i */
    complex z2 = CreatComplex(1.0, -4.0);     /* 1 - 4i */
    complex zero = CreatComplex(0.0, 0.0);
    complex real_only = CreatComplex(-5.0, 0.0);
    complex imag_only = CreatComplex(0.0, 6.0);
    complex fractional = CreatComplex(-1.25, 2.5);
    complex three_four = CreatComplex(3.0, 4.0);

    if (!z1 || !z2 || !zero || !real_only ||
        !imag_only || !fractional || !three_four) {
        fprintf(stderr, "ERROR: CreatComplex returned NULL.\n");
        DestroyComplex(z1);
        DestroyComplex(z2);
        DestroyComplex(zero);
        DestroyComplex(real_only);
        DestroyComplex(imag_only);
        DestroyComplex(fractional);
        DestroyComplex(three_four);
        return EXIT_FAILURE;
    }

    printf("\n========== CONSTRUCTOR, GETTERS, AND EDGE VALUES ==========\n");
    check_complex("CreatComplex(3, 2)", z1, 3, 2);
    check_complex("CreatComplex(1, -4)", z2, 1, -4);
    check_complex("CreatComplex(0, 0)", zero, 0, 0);
    check_complex("Real-only number", real_only, -5, 0);
    check_complex("Imaginary-only number", imag_only, 0, 6);
    check_complex("Fractional components", fractional, -1.25, 2.5);
    check_value("GetReal(z1)", GetReal(z1), 3);
    check_value("GetImaginary(z1)", GetImaginary(z1), 2);
    check_value("GetReal(z2)", GetReal(z2), 1);
    check_value("GetImaginary(z2)", GetImaginary(z2), -4);

    printf("\n======================= MAGNITUDE ===========================\n");
    check_value("Magnitude(3 + 2i)", ComplexMagnitude(z1), sqrt(13.0));
    check_value("Magnitude(1 - 4i)", ComplexMagnitude(z2), sqrt(17.0));
    check_value("Magnitude(3 + 4i)", ComplexMagnitude(three_four), 5);
    check_value("Magnitude(0)", ComplexMagnitude(zero), 0);
    check_value("Magnitude(-5)", ComplexMagnitude(real_only), 5);
    check_value("Magnitude(6i)", ComplexMagnitude(imag_only), 6);

    printf("\n======================== ADDITION ===========================\n");
    check_result("(3+2i) + (1-4i)", AddComplex(z1, z2), 4, -2, z1, z2);
    check_result("Reverse addition", AddComplex(z2, z1), 4, -2, z1, z2);
    check_result("Add zero", AddComplex(z1, zero), 3, 2, z1, zero);
    check_result("Add fractional values", AddComplex(fractional, z1), 1.75, 4.5,
                 fractional, z1);

    printf("\n====================== SUBTRACTION =========================\n");
    check_result("(3+2i) - (1-4i)", SubtractComplex(z1, z2), 2, 6, z1, z2);
    check_result("Reverse subtraction", SubtractComplex(z2, z1), -2, -6,
                 z2, z1);
    check_result("Subtract self", SubtractComplex(z1, z1), 0, 0, z1, z1);
    check_result("Zero minus number", SubtractComplex(zero, z1), -3, -2,
                 zero, z1);

    printf("\n===================== MULTIPLICATION =======================\n");
    check_result("(3+2i) * (1-4i)", MultiplyComplex(z1, z2), 11, -10,
                 z1, z2);
    check_result("Reverse multiplication", MultiplyComplex(z2, z1), 11, -10,
                 z2, z1);
    check_result("Multiply by zero", MultiplyComplex(z1, zero), 0, 0,
                 z1, zero);
    check_result("(6i) * (6i)", MultiplyComplex(imag_only, imag_only), -36, 0,
                 imag_only, imag_only);
    check_result("(-5) * (6i)", MultiplyComplex(real_only, imag_only), 0, -30,
                 real_only, imag_only);

    printf("\n======================== DIVISION ===========================\n");
    check_result("(3+2i) / (1-4i)", DivideComplex(z1, z2), -5.0/17.0,
                 14.0/17.0, z1, z2);
    check_result("(1-4i) / (3+2i)", DivideComplex(z2, z1), -5.0/13.0,
                 -14.0/13.0, z2, z1);
    check_result("Divide by self", DivideComplex(z1, z1), 1, 0, z1, z1);
    check_result("Zero divided by nonzero", DivideComplex(zero, z1), 0, 0,
                 zero, z1);
    check_result("(6i) / (-5)", DivideComplex(imag_only, real_only), 0, -1.2,
                 imag_only, real_only);

    printf("\n======================= CONJUGATE ===========================\n");
    check_result("Conjugate(3+2i)", ConjugateComplex(z1), 3, -2, z1, NULL);
    check_result("Conjugate(1-4i)", ConjugateComplex(z2), 1, 4, z2, NULL);
    check_result("Conjugate(0)", ConjugateComplex(zero), 0, 0, zero, NULL);
    check_result("Conjugate(6i)", ConjugateComplex(imag_only), 0, -6,
                 imag_only, NULL);

    printf("\n========================= SQUARE ============================\n");
    check_result("Square(3+2i)", SquareComplex(z1), 5, 12, z1, NULL);
    check_result("Square(1-4i)", SquareComplex(z2), -15, -8, z2, NULL);
    check_result("Square(6i)", SquareComplex(imag_only), -36, 0,
                 imag_only, NULL);
    check_result("Square(0)", SquareComplex(zero), 0, 0, zero, NULL);

    printf("\n====================== INPUT INTEGRITY ======================\n");
    check_complex("z1 unchanged after operations", z1, 3, 2);
    check_complex("z2 unchanged after operations", z2, 1, -4);
    check_complex("zero unchanged", zero, 0, 0);

    printf("\n======================= DISPLAY TEST ========================\n");
    printf("Check that each line shows the correct sign and components.\n");
    printf("Expected: 3 + 2i       Actual: ");  DisplayComplex(z1);
    printf("Expected: 1 - 4i       Actual: ");  DisplayComplex(z2);
    printf("Expected: 0 + 0i       Actual: ");  DisplayComplex(zero);
    printf("Expected: -5 + 0i      Actual: ");  DisplayComplex(real_only);
    printf("Expected: 0 + 6i       Actual: ");  DisplayComplex(imag_only);
    printf("Expected: -1.25 + 2.5i Actual: ");  DisplayComplex(fractional);
    printf("(Display formatting is a visual check, not in the count.)\n");

    printf("\n========================= CLEANUP ===========================\n");
    DestroyComplex(z1);
    DestroyComplex(z2);
    DestroyComplex(zero);
    DestroyComplex(real_only);
    DestroyComplex(imag_only);
    DestroyComplex(fractional);
    DestroyComplex(three_four);
    printf("All input complex numbers passed to DestroyComplex.\n");

    printf("\n========================= SUMMARY ===========================\n");
    printf("Passed: %d / %d automated checks\n", passed_tests, total_tests);
    printf("Failed: %d\n", total_tests - passed_tests);
    printf("Display output and memory leaks require separate inspection.\n");

    return passed_tests == total_tests ? EXIT_SUCCESS : EXIT_FAILURE;
}
