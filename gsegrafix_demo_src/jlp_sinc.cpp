/***********************************************************************
* jlp_sinc.cpp
*
* Calculates data for y = sin(x)/x plot.
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

static int jlp_write_to_file(char *fname, int nx, double *x0, double *y0);

int main(int argc, char *argv[])
{
double *x0, *y0, wstart, wend, wstep, ww0, ww1;
int nx = 256;
int i, j, status;
char fname[128];

x0 = new double[nx];
y0 = new double[nx];

wstart = -1.;
wend = +1.;
wstep = (wend - wstart) / ((double)nx - 1.);
for(i = 0; i < nx; i++) {
  ww0 = wstart + (double)i * wstep;
  ww1 = 6. * PI * ww0 + 0.00001; 
  x0[i] = ww1; 
  y0[i] = sin(ww1) / ww1;
  }

strcpy(fname, "jlp_sinc.txt");
status = jlp_write_to_file(fname, nx, x0, y0);

return(status);
}
/***********************************************************************
*
***********************************************************************/
static int jlp_write_to_file(char *fname, int nx, double *x0, double *y0)
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
