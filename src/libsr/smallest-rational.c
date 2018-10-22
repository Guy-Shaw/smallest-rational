/*
 * From: ressler@cs.cornell.edu (Gene Ressler)
 * Newsgroups: comp.programming
 * Subject: Re: Printing float as a fraction?
 * Date: 13 Mar 93 06:27:37 GMT
 * Organization: Cornell Univ. CS Dept, Ithaca NY 14853
 * 
 * The following function has the nice property of finding
 * _the_ shortest fraction in the given interval (subject to
 * floating point error in the loop termination criterion). 
 * See ACMCG 92 Proceedings "A Rational Rotation Method for Robust
 * Geometric Algorithms" by John Canny, Bruce Donald, and myself
 * for an informal explanation.  This also shows how to compute
 * ``short perfect'' sine/cosine pairs using the function below
 * as a subroutine.
 */

#include <stdbool.h>
#include <stdio.h>
#include <math.h>	// Import floor(), ceil()
#include <rational.h>

static rational_t rational_zero = { 0, 1 };

/*
 * Print smallest rational in interval [x0 .. x1],
 * where 0 <= x0 <= x1 <= 1
 */

rational_t
smallest_rational(double x0, double x1, double epsilon, bool trace)
{
    double p0, p1, q0, q1, e0, e1, f0, f1;
    double near1;
    rational_t ret;
    unsigned int cnt;

    if (x0 == 0) {
        return (rational_zero);
    }

    near1 = 1.0 + epsilon;
    p0 = 0;
    q0 = 1;
    p1 = 1;
    q1 = 1;
    e0 = 1 - x0;
    e1 = x0;
    f0 = 1 - x1;
    f1 = x1;

    cnt = 0;
    while (p0 * near1 < x0 * q0 || p0 > x1 * q0 * near1) {
        double i0, i1, t0, u0, v0, v1, r;

        ++cnt;
        i0 = floor(e0 / e1);
        i1 = ceil(f0 / f1);
        r  = i0 < i1 ? i0 : i1;
        t0 = p1 + r * p0;
        u0 = q1 + r * q0;
        p1 = p0;
        q1 = q0;
        p0 = t0;
        q0 = u0;
        v0 = f1;
        v1 = f0 - r * f1;
        f0 = e1;
        f1 = e0 - r * e1;
        e0 = v0;
        e1 = v1;
        if (trace) {
            fprintf(stderr, "trace: %u: %g/%g\n", cnt, p0, q0);
        }
    }

    if (trace) {
        fprintf(stderr, "trace: smallest_rational: loop count = %u\n", cnt);
    }

    ret.n = (unsigned long long)p0;
    ret.d = (unsigned long long)q0;
    return (ret);
}
