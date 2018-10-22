/*
 * Compute properties of double-precision floating-point arithmetic.
 *
 * Compute floating-point radix.
 */

double
double_radix(void)
{
    double radix;
    double a, b;

    a = 2.0;
    while ((a + 1.0) - a == 1.0) {
        a *= 2.0;
    }

    b = 2.0;
    while (a + b - a == 0.0) {
        b *= 2.0;
    }
    radix = (a + b) - a;
    return (radix);
}
