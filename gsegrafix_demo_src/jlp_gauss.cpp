/***********************************************************************
* jlp_gauss.cpp
*
* Calculates data for gaussian distribution 
* From a gsegraf-demo octave routine
*
* JLP
* Version 03/11/2016
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>  // srand()
#include <math.h>
#include <string.h>

#ifndef PI
#define PI 3.14159
#endif

static double rand_normal(double mean, double stddev);
static int jlp_write_to_file1(char *fname, int nx, double *x0);
static int jlp_write_to_file2(char *fname, int nx, double *x0, double *y0);

int main(int argc, char *argv[])
{
double mean, sigma, binwidth;
double *x0, *x1, *y1, ww0, ww1, ww2, wwx, wwy, f1;
int nx, i, status, iseed;
char fname[128];

// 1. Calculate random data with Gaussian distribution
mean = 0.0;
sigma = 1.0;
nx = 1000;

x0 = new double[nx];

// Initialize random generator
iseed = 1234;
srand(iseed);

// Random Gaussian distribution
for(i = 0; i < nx; i++) {
  x0[i] = rand_normal(mean, sigma); 
  }

strcpy(fname, "jlp_gauss_data.txt");
status = jlp_write_to_file1(fname, nx, x0);

// 2. Calculate theoretical expectation with a Gaussian distribution

#ifdef TTTR
// Calculate histogram bin width
// [xsort, indices] = sort(x);
q = zeros(3,1);
for ( i = 1:3 )
   index = round(i*0.25*npts);
   q(i) = xsort(index);
end
// See section on bin_width keyword for formulas and references below.
// binwidth = 3.49*sigma/npts^(1.0/3.0);   // Scott
binwidth = 2.0*(q(3) - q(1))/npts^(1.0/3.0);   // Freedman, Diaconis
#else
// JLP2016: approximation only...
binwidth = 0.30;
#endif

// Calculate theoretical gaussian distribution
ww1 = (mean - 3.5 * sigma);
ww2 = (mean + 3.5 * sigma);
f1 = 1.0 / (sigma * sqrt(2.0 * PI));
nx = 1000;
x1 = new double[nx];
y1 = new double[nx];
for(i = 0; i < nx; i++) {
// Spanning the range [0,1]:
  ww0 = (double)i * 1. / ((double) nx - 1);
  wwx = ww1 + ww0 * (ww2 - ww1);
  x1[i] = wwx;
  wwy = f1 * exp(- (wwx - mean) * (wwx - mean) / (2.0 * sigma * sigma));
  y1[i] = 100.0 * binwidth * wwy;   // percent occurrences
  }

strcpy(fname, "jlp_gauss_theory.txt");
status = jlp_write_to_file2(fname, nx, x1, y1);

return(status);
}
/***********************************************************************
*
***********************************************************************/
static int jlp_write_to_file1(char *fname, int nx, double *x0)
{
int i;
FILE *fp;

if((fp = fopen(fname, "w")) == NULL) {
  fprintf(stderr, " jlp_write_to_file/error opening %s\n", fname);
  return(-1);
  }

for(i = 0; i < nx; i++) {
  fprintf(fp, "%10.5f \n", x0[i]);
  }

fclose(fp);

return(0);
}
/***********************************************************************
*
***********************************************************************/
static int jlp_write_to_file2(char *fname, int nx, double *x0, double *y0)
{
int i;
FILE *fp;

if((fp = fopen(fname, "w")) == NULL) {
  fprintf(stderr, " jlp_write_to_file/error opening %s\n", fname);
  return(-1);
  }

for(i = 0; i < nx; i++) {
  fprintf(fp, "%10.5f %10.5f\n", x0[i], y0[i]);
  }

fclose(fp);

return(0);
}
/***********************************************************************
* Random Gaussian distribution with the Box muller method
*
***********************************************************************/
static double rand_normal(double mean, double stddev)
{
double xx, yy, rr, dd, n1, result;

static double n2 = 0.0;
static int n2_cached = 0;
// rand() is a pseudo-random generator between 0 and RAND_MAX:
  if (!n2_cached) {
        do {
            xx = 2.0 * rand() / (double)RAND_MAX - 1;
            yy = 2.0 * rand() / (double)RAND_MAX - 1;

            rr = xx*xx + yy*yy;
        } while (rr == 0.0 || rr > 1.0);
        {
            dd = sqrt(-2.0*log(rr)/rr);
            n1 = xx*dd;
            n2 = yy*dd;
            result = n1*stddev + mean;
            n2_cached = 1;
            return result;
        }
    } else {
        result = n2*stddev + mean;
        n2_cached = 0;
        return result;
    }
}
