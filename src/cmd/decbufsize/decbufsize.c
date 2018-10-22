#include <stdlib.h>	// Import exit();
#include <stdio.h>
#include <math.h>	// Import log(), Ceil()

typedef unsigned int uint_t;

void
main(int argc, char * const *argv)
{
    uint_t w;

    for (w = 1; w <= 64; ++w) {
        double fdigits;
        uint_t digits;

        fdigits = ceil(log(2)/log(10) * (double)w);
        digits = fdigits;
        printf("%2u %2u\n", w, digits);
    }
    exit(0);
}
