/***********************************************************************
* jlp_sinc3d.cpp
*
* Calculates data for sin(r)/r plot, where r = sqrt(x^2 + y^2)
* From a gsegraf-demo octave routine
*
* JLP
* Version 01/11/2016
**************************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef PI
#define PI 3.14159
#endif

static int jlp_write_to_file(char *fname, int nx, int ny, double *x0, 
                             double *y0, double *z0);

int main(int argc, char *argv[])
{
double *x0, *y0, *z0, wstart, wend, wstep, rr;
int nx, ny;
int i, j, status;
char fname[128];

wstart = -10.;
wend = +10.;
wstep = 0.5; 
nx = (wend - wstart) / wstep;
ny = nx; 

x0 = new double[nx];
y0 = new double[ny];
z0 = new double[nx * ny];

for(i = 0; i < nx; i++) {
  x0[i] = wstart + (double)i * wstep + 0.00001;
  y0[i] = x0[i]; 
  }

for(i = 0; i < nx; i++) {
   for(j = 0; j < ny; j++) {
     rr = sqrt(x0[i] * x0[i] + y0[j] * y0[j]);
     z0[i + j * nx] = sin(rr) / rr;
   }
}

strcpy(fname, "jlp_sinc3d.txt");
status = jlp_write_to_file(fname, nx, ny, x0, y0, z0);

return(status);
}
/***********************************************************************
*
***********************************************************************/
static int jlp_write_to_file(char *fname, int nx, int ny, double *x0, 
                             double *y0, double *z0)
{
int i, j;
FILE *fp;

if((fp = fopen(fname, "w")) == NULL) {
  fprintf(stderr, " jlp_write_to_file/error opening %s\n", fname);
  return(-1);
  }

fprintf(fp, "%10d%10d ", nx, ny);
fprintf(fp, "\n");

for(i = 0; i < nx; i++) {
  fprintf(fp, "%10.2f ", x0[i]);
  }
fprintf(fp, "\n");

for(i = 0; i < ny; i++) {
  fprintf(fp, "%10.2f ", y0[i]);
  }
fprintf(fp, "\n");

for(i = 0; i < nx; i++) {
   for(j = 0; j < ny; j++) {
     fprintf(fp, "%10.5f ", z0[i + j * nx]);
     }
   fprintf(fp, "\n");
}

fclose(fp);

return(0);
}
