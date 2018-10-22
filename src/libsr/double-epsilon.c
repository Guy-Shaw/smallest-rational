
/*
 * Compute properties of double-precision floating-point arithmetic.
 *
 * Compute machine epsilon.
 */

extern double double_radix(void);

double
double_epsilon(void)
{
    double radix;
    double delta, epsilon;

    radix = double_radix();
    delta = 1.0;
    while (1.0 + delta != 1.0) {
        epsilon = delta;
        delta /= radix;
    }
    return (epsilon);
}
