#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define MIN(lhs, rhs) (((lhs) < (rhs)) ? (lhs) : (rhs))

/// Convert seconds to milliseconds
#define SEC_TO_MS(sec) ((sec)* 1e3)
/// Convert seconds to microseconds
#define SEC_TO_US(sec) ((sec)* 1e6)
/// Convert seconds to nanoseconds
#define SEC_TO_NS(sec) ((sec)* 1e9)

/// Convert nanoseconds to seconds
#define NS_TO_SEC(ns)   ((ns) * 1e-9)
/// Convert nanoseconds to milliseconds
#define NS_TO_MS(ns)    ((ns) * 1e-6)
/// Convert nanoseconds to microseconds
#define NS_TO_US(ns)    ((ns) * 1e-3)

/// mmul i for loop
#define fori for (size_t i = 0; i < lhs.m; i++)
/// mmul j for loop
#define forj for (size_t j = 0; j < rhs.n; j++)
/// mmul k for loop
#define fork for (size_t k = 0; k < lhs.n; k++)
// apply mmul
#define __mmul__ values[i * mul.n + j] += lhs.values[i * lhs.n + k] * rhs.values[k * rhs.n + j];

#define display 0

const int ENOMEM = -1;
const int ENODIMMISMATCH = -2;
const int ENOPARSEARGS = -3;
const int ENONULLPTR = -4;

const size_t NBOPS = (1ull << 30ull);

double seconds(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    const double s = ((double) ts.tv_sec) + NS_TO_SEC((double)ts.tv_nsec);
    return s;
}

double millis(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    const double ms = SEC_TO_MS((double)ts.tv_sec) + NS_TO_MS((double)ts.tv_nsec);
    return ms;
}

double micros(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    const double us = SEC_TO_US((double)ts.tv_sec) + NS_TO_US((double)ts.tv_nsec);
    return us;
}

double nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    const double ns = SEC_TO_NS((double)ts.tv_sec) + ((double) ts.tv_nsec);
    return ns;
}

void check_ptr_creation(const void *const ptr) {
    if (ptr == NULL) {
        exit(ENOMEM);
    }
}

typedef struct {
    uint64_t v;
} Rand;

uint64_t next_uint64(Rand *rng) {
    rng->v ^= rng->v >> 21;
    rng->v ^= rng->v << 35;
    rng->v ^= rng->v >> 4;
    return rng->v * 2685821657736338717;
}

uint32_t next_uint32(Rand *rng) {
    return (uint32_t) next_uint64(rng);
}

double next_double(Rand *rng) {
    return 5.42101086242752217E-20 * next_uint64(rng);
}

Rand rand_from_seed(const uint64_t seed) {
    Rand rng = {.v = 4101842887655102017};
    rng.v ^= seed;
    rng.v = next_uint64(&rng);
    return rng;
}

Rand rand_from_entropy(void) {
    return rand_from_seed((uint64_t) time(NULL));
}

typedef struct {
    double *values;
    size_t m;
    size_t n;
} matrix_of_double;

matrix_of_double create_matrix_of_double(const size_t m, const size_t n) {
    double *values = calloc(sizeof(double), m * n);
    check_ptr_creation(values);
    const matrix_of_double matrix = {
        .values = values, .m = m, .n = n
    };
    return matrix;
}

void fill_rands(const matrix_of_double m, Rand *rng) {
    double *const values = m.values;
    if (values == NULL) exit(ENONULLPTR);
    for (size_t i = 0; i < m.m; i++) {
        for (size_t j = 0; j < m.n; j++) {
            values[i * m.n + j] = next_double(rng);
        }
    }
}

void fill_zeroes(const matrix_of_double m) {
    double *const values = m.values;
    if (values == NULL) exit(ENONULLPTR);
    for (size_t i = 0; i < m.m; i++) {
        for (size_t j = 0; j < m.n; j++) {
            values[i * m.n + j] = 0;
        }
    }
}

void free_matrix_of_double(matrix_of_double m) {
    free(m.values);
}

void repr(const matrix_of_double m) {
    const double *const values = m.values;
    if (values == NULL) exit(ENONULLPTR);
    for (size_t i = 0; i < m.m; i++) {
        for (size_t j = 0; j < m.n; j++) {
            printf("%.2lf ", values[i * m.n + j]);
        }
        printf("\n");
    }
}

void check_dimensions_for_mmul(const matrix_of_double lhs, const matrix_of_double rhs, const matrix_of_double mul) {
    if (lhs.n != rhs.m || mul.m != lhs.m || mul.n != rhs.n) {
        exit(ENODIMMISMATCH);
    }
}

static inline
matrix_of_double mmul_ijk(const matrix_of_double lhs, const matrix_of_double rhs, const matrix_of_double mul) {
    check_dimensions_for_mmul(lhs, rhs, mul);
    double *values = mul.values;
    if (values == NULL) exit(ENONULLPTR);
    fori
        forj
            fork
                __mmul__
    return mul;
}

static inline
matrix_of_double mmul_jik(const matrix_of_double lhs, const matrix_of_double rhs, const matrix_of_double mul) {
    check_dimensions_for_mmul(lhs, rhs, mul);
    double *values = mul.values;
    if (values == NULL) exit(ENONULLPTR);
    forj
        fori
            fork
                __mmul__
    return mul;
}

static inline
matrix_of_double mmul_kij(const matrix_of_double lhs, const matrix_of_double rhs, const matrix_of_double mul) {
    check_dimensions_for_mmul(lhs, rhs, mul);
    double *values = mul.values;
    if (values == NULL) exit(ENONULLPTR);
    fork
        fori
            forj
                __mmul__
    return mul;
}

static inline
matrix_of_double mmul_ikj(const matrix_of_double lhs, const matrix_of_double rhs, const matrix_of_double mul) {
    check_dimensions_for_mmul(lhs, rhs, mul);
    double *values = mul.values;
    if (values == NULL) exit(ENONULLPTR);
    fori
        fork
            forj
                __mmul__
    return mul;
}

static inline
matrix_of_double mmul_jki(const matrix_of_double lhs, const matrix_of_double rhs, const matrix_of_double mul) {
    check_dimensions_for_mmul(lhs, rhs, mul);
    double *values = mul.values;
    if (values == NULL) exit(ENONULLPTR);
    forj
        fork
            fori
                __mmul__
    return mul;
}

static inline
matrix_of_double mmul_kji(const matrix_of_double lhs, const matrix_of_double rhs, const matrix_of_double mul) {
    check_dimensions_for_mmul(lhs, rhs, mul);
    double *values = mul.values;
    if (values == NULL) exit(ENONULLPTR);
    fork
        forj
            fori
                __mmul__
    return mul;
}

size_t ipow(const size_t x, const size_t n) {
    if (n == 0) return 1;
    if (n & 1) {
        return x * ipow(x * x, (n - 1) >> 1);
    }
    return ipow(x * x, n >> 1);
}

void benchmark(const char* const name, matrix_of_double (* const mmul)(matrix_of_double, matrix_of_double, matrix_of_double),
               const matrix_of_double m, const matrix_of_double n) {
    const matrix_of_double mul = create_matrix_of_double(m.m, n.n);
    fill_zeroes(mul);
    const size_t nb_iter = MIN(NBOPS, (NBOPS / (m.m * m.n * n.n)) + 1);
    const double start = micros();
    for (size_t i = 0; i < nb_iter; i++) {
        mmul(m, n, mul);
    }
    const double end = micros();
    fprintf(stderr, "%lf\n", mul.values[(mul.m - 1) * mul.n + mul.n - 1]);

    if (display) {
        // ReSharper disable once CppDFAUnreachableCode
        repr(mul);
    }

    free_matrix_of_double(mul);
    printf("%s:%lfμs:%lu\n", name, end - start, nb_iter);
}

int main(const int argc, char **argv) {
    size_t N;
    const char *names[] = {"ijk", "jik", "kij", "ikj", "jki", "kji"};
    if (argc < 2) {
        printf("missing N\n");
        printf("usage: mmul N <ALGORITHMS>\n");
        exit(ENOPARSEARGS);
    }
    if (sscanf(argv[1], "%zu", &N) != 1) {
        exit(ENOPARSEARGS);
    }
    bool algorithms[] = {true, true, true, true, true, true};
    if (argc == 3) {
        for (size_t i = 0; i < 6; i++) {
            algorithms[i] = false;
        }
        char *token = strtok(argv[2], ",");
        while (token) {
            size_t active = 0;
            while (active < 6 && strcmp(token, names[active]) != 0) {
                active += 1;
            }
            if (active < 6) {
                fprintf(stderr, "enabling: '%s'\n", token);
                algorithms[active] = true;
            } else {
                fprintf(stderr, "ignored invalid algorithm: '%s'\n", token);
            }
            token = strtok(NULL, ",");
        }
    }

    Rand rng = rand_from_entropy();
    const matrix_of_double m = create_matrix_of_double(N, N);
    fill_rands(m, &rng);
    const matrix_of_double n = create_matrix_of_double(N, N);
    fill_rands(n, &rng);

    // ijk
    if (algorithms[0])
        benchmark("ijk", mmul_ijk, m, n);

    // jik
    if (algorithms[1])
        benchmark("jik", mmul_jik, m, n);

    // kij
    if (algorithms[2])
        benchmark("kij", mmul_kij, m, n);

    // ikj
    if (algorithms[3])
        benchmark("ikj", mmul_ikj, m, n);

    // jki
    if (algorithms[4])
        benchmark("jki", mmul_jki, m, n);

    // kji
    if (algorithms[5])
        benchmark("kji", mmul_kji, m, n);

    free_matrix_of_double(n);
    free_matrix_of_double(m);
    return 0;
}
