/***********************************************************************
* jlp_dipole.cpp
*
* Calculates normalized gain pattern of half-wavelength dipole antenna 
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
double *theta0, *gain0, *gain_dB0, wstart, wend, wstep, ww0, ww1;
double lambda0, l2, kk, f1;
int nx, i, j, status;
char fname[128];

nx = 360.; 
theta0 = new double[nx];
gain0 = new double[nx];
gain_dB0 = new double[nx];

// Calculate dipole antenna gain pattern
lambda0 = 1.0;
l2 = lambda0 / 2.0;
kk = 2.0 * PI / lambda0;
f1 = kk * l2 / 2.0;

for(i = 0; i < nx; i++) {
  ww0 = (double)i * 360. / (nx - 1);
  theta0[i] = ww0 * DEG2RAD;
  }
 
gain0[0] = DBLE_MINI;
gain0[359] = DBLE_MINI;
for(i = 1; i < 359; i++) {
  if(i == 180) { 
    gain0[180] = DBLE_MINI;
  } else {
    ww1 = (cos(f1*cos(theta0[i])) - cos(f1))
              / sin(theta0[i]);
    gain0[i] = ww1 * ww1; 
  }
}

for(i = 1; i < 360; i++) {
  gain_dB0[i] = 10.0 * log10 (gain0[i]);
  }

/* Calculate 3-dB beam width
theta_3dB = interp1(gain_dB(1:181), theta(1:181), -3)/deg2rad;
beamwidth_3dB = 2*(90 - theta_3dB);
*/

// Save data
strcpy(fname, "jlp_dipole.txt");
status = jlp_write_to_file3(fname, nx, theta0, gain0, gain_dB0);

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
  fprintf(fp, "%13.5e %13.5e %13.5e\n", x0[i], y0[i], z0[i]);
  }

fclose(fp);

return(0);
}
