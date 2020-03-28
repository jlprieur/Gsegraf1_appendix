/***********************************************************************
* jlp_logz.cpp
*
* Calculates data for magnitude of log(z) plot, where z = x + i*y.
* From a gsegraf-demo octave routine
*
* JLP
* Version 31/10/2016
**************************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

static int jlp_write_to_file(char *fname, int nx, int ny, double *x0, 
                             double *y0, double *z0);

int main(int argc, char *argv[])
{
double *x0, *y0, *z0, wstart, wend, wstep, zz, rho, theta;
int nx = 256, ny = 256;
int i, j, k, status;
char fname[128];

x0 = new double[nx];
y0 = new double[ny];
z0 = new double[nx * ny];

wstart = -3.;
wend = +3.;
wstep = (wend - wstart) / ((double)nx - 1.);
for(i = 0; i < nx; i++) x0[i] = wstart + (double)i * wstep;

wstart = -3.;
wend = +3.;
wstep = (wend - wstart) / ((double)ny - 1.);
for(j = 0; j < ny; j++) y0[j] = wstart + (double)j * wstep; 

for(i = 0; i < nx; i++) {
  for(j = 0; j < ny; j++) {
// Modulus:
    rho = x0[i] * x0[i] + y0[j] * y0[j]; 
    if(rho != 0.) rho = sqrt(rho);
// Argument in [-pi, +pi] : using atan2(y,x)
    theta = atan2(y0[j], x0[i]);
// log(rho exp i theta) = log(rho) + i theta;
// We take here the modulus of the complex logarithm:
    if(rho != 0.) {
     z0[i + j * nx] = log(rho)*log(rho) + theta *theta;
     } else {
     z0[i + j * nx] = 0.;
     }

   }
  }

strcpy(fname, "jlp_logz.txt");
status = jlp_write_to_file(fname, nx, ny, x0, y0, z0);

return(status);
}
/***************************************************************************
*
***************************************************************************/
static int jlp_write_to_file(char *fname, int nx, int ny, double *x0, 
                             double *y0, double *z0)
{
FILE *fp;
int i, j;

if((fp = fopen(fname, "w")) == NULL) {
  fprintf(stderr, " jlp_write_to_file/error opening %s\n", fname);
  return(-1);
  }

fprintf(fp, "%10d%10d", nx, ny);
fprintf(fp, "\n");
for(i = 0; i < nx; i++) fprintf(fp, "%10.3f ", x0[i]);
fprintf(fp, "\n");
for(j = 0; j < ny; j++) fprintf(fp, "%10.3f ", y0[j]);
fprintf(fp, "\n");
for(i = 0; i < nx; i++) {
  for(j = 0; j < ny; j++) {
     fprintf(fp, "%15.5e ", z0[i + j * nx]);
     }
// End each line by "\n"
  fprintf(fp, "\n");
  }

fclose(fp);

return(0);
}
