/*
 * Filename: src/cmd/smallest-rational.c
 * Project: smallest-rational
 * Brief: "smallest" rational that approximates a given floating-point number.
 *
 * Copyright (C) 2018 Guy Shaw
 * Written by Guy Shaw <gshaw@acm.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
    // Import isalnum()
    // Import islower()
    // Import isprint()
    // Import isupper()
#include <math.h>
    // Import floor()
#include <stdbool.h>
    // Import type bool
    // Import constant false
    // Import constant true
#include <stddef.h>
    // Import constant NULL
#include <stdio.h>
    // Import constant EOF
    // Import type FILE
    // Import fclose()
    // Import fgetc()
    // Import fopen()
    // Import fputc()
    // Import fputs()
    // Import printf()
    // Import snprintf()
    // Import var stdin
    // Import var stdout
#include <stdlib.h>
    // Import abort()
    // Import exit()
    // Import strtod()
#include <string.h>
    // Import strcmp()
#include <strings.h>
    // Import strncasecmp()
#include <unistd.h>
    // Import getopt_long()
    // Import type size_t

#define IMPORT_FVH
#include <cscript.h>

#include <rational.h>

static inline size_t
int_to_size(int i)
{
    if (i < 0) {
        abort();
    }
    return ((size_t)i);
}

const char *program_path;
const char *program_name;

size_t filec;               // Count of elements in filev
char **filev;               // Non-option elements of argv

bool debug      = false;
bool verbose    = false;
bool opt_argv   = false;
bool opt_over   = false;
bool opt_under  = false;


/*
 * cmp: filter results.
 *   -1 : Show only rationals that are <= the given number.
 *    1 : Show only rationals that are >= the given number.
 *    0 : Show all rationals.  No filtering.
 */

static int cmp = 0;

FILE *errprint_fh = NULL;
FILE *dbgprint_fh = NULL;

static struct option long_options [] = {
    { "help",    no_argument, 0, 'h' },
    { "version", no_argument, 0, 'V' },
    { "verbose", no_argument, 0, 'v' },
    { "debug",   no_argument, 0, 'd' },
    { "argv",    no_argument, 0, 'A' },
    { "over",    no_argument, 0, 'O' },
    { "under",   no_argument, 0, 'U' },
    { 0,         0,           0,  0 }
};

static const char usage_text[] =
    "Options:\n"
    "  --help|-h|-?    Show this help message and exit\n"
    "  --version       Show version information and exit\n"
    "  --debug|-d      debug\n"
    "  --verbose|-v    verbose\n"
    "  --argv|-A       Date from command-line arguments, not files\n"
    "  --over|-O       Show only rationals that are >= |n|\n"
    "  --under|-U      Show only rationals that are <= |n|\n"
    ;

static const char version_text[] =
    "0.1\n"
    ;

static const char copyright_text[] =
    "Copyright (C) 2018 Guy Shaw\n"
    "Written by Guy Shaw\n"
    ;

static const char license_text[] =
    "License GPLv3+: GNU GPL version 3 or later"
    " <http://gnu.org/licenses/gpl.html>.\n"
    "This is free software: you are free to change and redistribute it.\n"
    "There is NO WARRANTY, to the extent permitted by law.\n"
    ;

static void
fshow_program_version(FILE *f)
{
    fputs(version_text, f);
    fputc('\n', f);
    fputs(copyright_text, f);
    fputc('\n', f);
    fputs(license_text, f);
    fputc('\n', f);
}

static void
show_program_version(void)
{
    fshow_program_version(stdout);
}

static void
usage(void)
{
    eprintf("usage: %s [ <options> ]\n", program_name);
    eprint(usage_text);
}


#ifdef OPT_VARIANT_STRCMP

enum variant {
    VARIANT_WORDS,
    VARIANT_ACRONYM,
};

/*
 * Like strcmp(), except that several variations of case and punctuation
 * are allowed.  For example, all the following compare equal:
 *
 *   my-option  my_option  MyOption MY-OPTION My-Option ...
 *
 * but not
 *
 *   my-option == myoption
 *
 * because 'my-option' is two "words" but 'myoption' is just one word.
 *
 * Word boundaries are either punctuation ('-' or '_')
 * or a CamelCase transition from lowercase to uppercase.
 *
 * Status: Experimental.  Needs work.
 *
 */

static int
variant_strcmp(const char *var_str, const char *ref_str, int flags)
{
    const char *vp;
    const char *rp;
    const char *vw;
    const char *rw;

    (void)flags;
    vp = var_str;
    rp = ref_str;
    while (true) {
        size_t vlen;
        size_t rlen;
        size_t cmplen;
        int cmp;

        vw = vp;
        while (*vp && isalnum(*vp) && !(vp > vw && islower(vp[-1]) && isupper(*vp))) {
            ++vp;
        }
        vlen = vp - vw;
        rw = rp;
        while (*rp && isalnum(*rp) && !(rp > rw && islower(rp[-1]) && isupper(*rp))) {
            ++rp;
        }
        rlen = rp - rw;

        cmplen = vlen <= rlen ? vlen : rlen;
        cmp = strncasecmp(vw, rw, cmplen);
        if (cmp != 0) {
            return (cmp);
        }
        if (vlen < rlen) {
            return (- *rp);
        }
        else if (vlen > rlen) {
            return (*vp);
        }
        while (*vp && !isalnum(*vp)) {
            ++vp;
        }
        while (*rp && !isalnum(*rp)) {
            ++rp;
        }
        if (*vp == 0 && *rp == 0) {
            return (0);
        }
    }
}

#endif /* OPT_VARIANT_STRCMP */

static inline bool
is_long_option(const char *s)
{
    return (s[0] == '-' && s[1] == '-');
}

static inline char *
vischar_r(char *buf, size_t sz, int c)
{
    if (isprint(c)) {
        buf[0] = c;
        buf[1] = '\0';
    }
    else {
        snprintf(buf, sz, "\\x%02x", c);
    }
    return (buf);
}

extern double double_radix(void);
extern double double_epsilon(void);

extern rational_t smallest_rational(double, double, double, bool);

static double fp_radix;
static double fp_epsilon;

void
set_fp_properties_double(void)
{
    fp_radix   = double_radix();
    fp_epsilon = double_epsilon();
}

void
show_fp_properties_double(void)
{
    set_fp_properties_double();
    printf("double radix   = %g\n", fp_radix);
    printf("double epsilon = %g\n", fp_epsilon);
}

int
show_smallest_rational(FILE *dstf, double x, double tolerance, double epsilon, int cmp)
{
    double whole, frac;
    double diff, abs_diff, rel_err, pcnt_err;
    char sbuf[40];
    rational_t r;
    double approx;
    char *sgn;
    int rc;
    bool print;

    whole = floor(x);
    frac = x - whole;
    r = smallest_rational(frac - tolerance, frac + tolerance, epsilon, verbose);
    if (whole > 0) {
        r.n += whole * r.d;
    }
    // fprintf(stderr, "tolerance=%g\n", tolerance);
    rc = snprintf(sbuf, sizeof(sbuf), "%llu/%llu", r.n, r.d);
    if (rc < 0) {
        return (rc);
    }
    approx = (double)r.n / (double)r.d;
    diff = approx - x;
    abs_diff = (diff >= 0) ? diff : -diff;
    rel_err = abs_diff / x;
    pcnt_err = rel_err * 100;
    print = true;
    if (diff == 0.0) {
        sgn = "=";
    }
    else if (diff > 0.0) {
        if (cmp < 0) {
            print = false;
        }
        sgn = ">";
    }
    else {
        if (cmp > 0) {
            print = false;
        }
        sgn = "<";
    }

    if (print) {
        fprintf(dstf, "%-22s %12g  %s  %12g %12g",
            sbuf, tolerance, sgn, abs_diff, pcnt_err);
        // fprintf(dstf, "diff=%10g, abs_diff=%g, rel_err=%g, pcnt_err=%g",
        //     diff, abs_diff, rel_err, pcnt_err);
        fprintf(dstf, "\n");
    }

    return (0);
}

void
show_smallest_rational_series(FILE *dstf, double x, double epsilon, int cmp)
{
    double tolerance;
    printf("smallest_rational(%g)\n", x);

    printf("numerator/denominator  tolerance    cmp     abs-diff    pcnt-diff\n");
    printf("---------------------- ------------ --- ------------ ------------\n");

    for (tolerance = 1e-10; tolerance < 0.1; tolerance *= 10.0) {
        (void) show_smallest_rational(dstf, x, tolerance, epsilon, cmp);
    }
}

static int
do_stream(fvh_t *fvp, FILE *dstf, double epsilon, int cmp)
{
    char numstr[64];
    int c;

    while ((c = fgetc(fvp->fh)) != EOF) {
        if (c != ' ' && c != '\n' && c != '\t') {
            char *p;
            p = numstr;
            while ((c = fgetc(fvp->fh)) != EOF && c != ' ' && c != '\n' && c != '\t')  {
                if ((size_t)(p - numstr) < sizeof (numstr) - 1) {
                    *p++ = c;
                }
            }

            if (p > numstr) {
                double x;

                *p = '\0';
                x = strtod(numstr, NULL);
                show_smallest_rational_series(dstf, x, epsilon, cmp);
            }
        }
    }

    return (0);
}

static int
do_filev(size_t filec, char **filev, FILE *dstf, double epsilon, int cmp)
{
    fvh_t fv;
    FILE *srcf;
    int rv;

    fv.filec = filec;
    fv.filev = filev;
    fv.glnr = 0;
    rv = 0;
    for (fv.fnr = 0; fv.fnr < filec; ++fv.fnr) {
        fv.fname = fv.filev[fv.fnr];
        if (strcmp(fv.fname, "-") == 0) {
            srcf = stdin;
        }
        else {
            srcf = fopen(fv.fname, "r");
        }
        if (srcf == NULL) {
            rv = 2;
            break;
        }
        fv.fh = srcf;
        rv = do_stream(&fv, dstf, epsilon, cmp);
        fclose(srcf);
    }

    return (rv);
}

int
main(int argc, char **argv)
{
    extern char *optarg;
    extern int optind, opterr, optopt;
    int option_index;
    int err_count;
    int optc;
    int rv;

    set_eprint_fh();
    program_path = *argv;
    program_name = sname(program_path);
    option_index = 0;
    err_count = 0;
    opterr = 0;

    while (true) {
        int this_option_optind;

        if (err_count > 10) {
            eprintf("%s: Too many option errors.\n", program_name);
            break;
        }

        this_option_optind = optind ? optind : 1;
        optc = getopt_long(argc, argv, "+hVdvEHw:", long_options, &option_index);
        if (optc == -1) {
            break;
        }

        rv = 0;
        if (optc == '?' && optopt == '?') {
            optc = 'h';
        }

        switch (optc) {
        case 'V':
            show_program_version();
            exit(0);
            break;
        case 'h':
            fputs(usage_text, stdout);
            exit(0);
            break;
        case 'd':
            debug = true;
            set_debug_fh(NULL);
            break;
        case 'v':
            verbose = true;
            break;
        case 'A':
            opt_argv = true;
            break;
        case 'O':
            opt_over = true;
            break;
        case 'U':
            opt_under = true;
            break;
        case '?':
            eprint(program_name);
            eprint(": ");
            if (is_long_option(argv[this_option_optind])) {
                eprintf("unknown long option, '%s'\n",
                    argv[this_option_optind]);
            }
            else {
                char chrbuf[10];
                eprintf("unknown short option, '%s'\n",
                    vischar_r(chrbuf, sizeof (chrbuf), optopt));
            }
            ++err_count;
            break;
        default:
            eprintf("%s: INTERNAL ERROR: unknown option, '%c'\n",
                program_name, optopt);
            exit(64);
            break;
        }
    }

    if (opt_over && opt_under) {
        eprintf("%s: WARNING\n", program_name);
        eprintf("  Options  --over and --under both specified.\n");
        eprintf("  All results will be shown.\n");
    }
    else if (opt_over) {
        cmp = 1;
    }
    else if (opt_under) {
        cmp = -1;
    }

    if (debug) {
        verbose = true;
    }

    if (optind < argc) {
        filec = (size_t) (argc - optind);
        filev = argv + optind;
    }
    else {
        filec = 0;
        filev = NULL;
    }

    if (verbose) {
        fshow_str_array(errprint_fh, filec, filev);
    }

    if (verbose && optind < argc) {
        int i;

        eprintf("non-option ARGV-elements:\n");
        i = optind;
        while (i < argc) {
            eprintf("    %s\n", argv[i]);
            ++i;
        }
    }

    if (err_count != 0) {
        usage();
        exit(2);
    }

    set_fp_properties_double();
    show_fp_properties_double();

    if (opt_argv) {
        double x;

        x = strtod(argv[optind], NULL);
        show_smallest_rational_series(stdout, x, fp_epsilon, cmp);
    }
    else if (filec) {
        rv = filev_probe(filec, filev);
        if (rv != 0) {
            exit(rv);
        }

        rv = do_filev(int_to_size(filec), filev, stdout, fp_epsilon, cmp);
    }
    else {
        char *fv_stdin = { "-" };
        rv = do_filev(1, &fv_stdin, stdout, fp_epsilon, cmp);
    }

    if (rv != 0) {
        exit(rv);
    }

    exit(0);
}
