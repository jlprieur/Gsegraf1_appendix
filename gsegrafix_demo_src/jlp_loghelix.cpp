/***********************************************************************
* jlp_dipole.cpp
*
* Calculates data for 3d logarithmic helix plot
* From a gsegraf-demo octave routine
*
* JLP
* Version 03/11/2016
**************************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef PI
#define PI 3.14159
#endif
#define DEG2RAD (PI / 180.) 
#define DBLE_MINI 1.e-32

static int jlp_write_to_file3(char *fname, int nx, double *x0, double *y0,
                              double *z0);

int main(int argc, char *argv[])
{
double *x0, *y0, *z0, rr0, theta0;
int nx, i, status;
char fname[128];

nx = 1000.; 
x0 = new double[nx];
y0 = new double[nx];
z0 = new double[nx];

for(i = 0; i < nx; i++) {
// Spanning the range [0, 20 * PI] (i.e. 10 turns)
  theta0 = 20. * PI *(double)i / (nx - 1);
  rr0 = exp(0.05 * theta0);
  x0[i] = rr0 * cos(theta0);
  y0[i] = rr0 * sin(theta0);
  z0[i] = theta0;
  }

// Save data
strcpy(fname, "jlp_loghelix.txt");
status = jlp_write_to_file3(fname, nx, x0, y0, z0);

delete[] x0;
delete[] y0;
delete[] z0;

return(status);
}
/***********************************************************************
*
***********************************************************************/
static int jlp_write_to_file3(char *fname, int nx, double *x0, double *y0,
                              double *z0)
{
int i;
FILE *fp;

if((fp = fopen(fname, "w")) == NULL) {
  fprintf(stderr, " jlp_write_to_file3/error opening %s\n", fname);
  return(-1);
  }

for(i = 0; i < nx; i++) {
  fprintf(fp, "%10.5f %10.5f %10.5f\n", x0[i], y0[i], z0[i]);
  }

fclose(fp);

return(0);
}
