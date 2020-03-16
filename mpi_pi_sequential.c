/*
MPI Scatter Gather testing program
    testing scatter and gather local and global time with big buffer
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <zconf.h>

double rand_0_1() {
    return (double) rand() / (double) RAND_MAX;
}

double vec_length(double x, double y) {
    return sqrt(x * x + y * y);
}

int main(int argc, char **argv) {
    double x, y;
    unsigned long in_circle, n, n_max, times, times_max;
    n_max = 1000000;
    times_max = 10;

    time_t tt;
    int seed = time(&tt);
    srand(seed);

    FILE *fp;
    if ((fp = fopen("data.csv", "w")) == NULL) {
        printf("Can't open data.csv in write mode!\n");
        exit(1);
    }

    for (n = 10; n <= n_max; n *= 10) {
        fprintf(fp, "n%u", n);
        for (times = 1; times < times_max; times++) {
            in_circle = 0;
            for (long i = 0; i < n; ++i) {
                x = rand_0_1();
                y = rand_0_1();
                if (vec_length(x, y) <= 1) {
                    in_circle++;
                }
            }
            double pi = (double) 4.0 * in_circle / n;
            printf("n = %u, pi = %f\n", n, pi);
            fprintf(fp, ",%f", pi);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}
