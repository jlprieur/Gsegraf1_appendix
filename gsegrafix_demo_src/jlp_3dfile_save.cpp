/*************************************************************************
* jlp_3dfile_save.c
*
* Write an example of a 3D-file compatible with gsegraf program
*
* JLP
* Version 31/10/2016
**************************************************************************/
#include <stdio.h>
#include <math.h>

void data_C_save(char *fname, int nx, int ny, double *x, double *y, double *z);

int main(int argc, char *argv[]){
double *x0, *y0, *z0, rr;
int nx = 64, ny = 64, ixc, iyc;
int i, j;

x0 = new double[nx];
y0 = new double[ny];
z0 = new double[nx * ny];

ixc = nx /2;
iyc = nx /2;
for(i = 0; i < nx; i++) x0[i] = (double) (i - ixc);
for(j = 0; j < ny; j++) y0[j] = (double) (j - iyc);

for(j = 0; j < ny; j++) {
  for(i = 0; i < nx; i++) { 
   rr = (double)(i - ixc)*(double)(i - ixc) +
             (double)(j - iyc)*(double)(j - iyc);
   if(rr != 0.) {
     rr = sqrt(rr);
     z0[i + j * nx] = sin(rr) / rr; 
     } else {
     z0[i + j * nx] = 1.; 
     }

   }
  }

data_C_save("./jlp_sinc3d.txt", nx, ny, x0, y0, z0);

delete[] x0;
delete[] y0;
delete[] z0;
return(0);
}

/****************************************************************************
*
* Function writes a 3d data-file for 3d contour, color, and mesh plots, and
* 2d contour and color plots.
*
* Input
*    fname: 3d data file-name (including path)
*    nx:    number of values in x-coordinates array
*    ny:    number of values in y-coordinates array
*    x:     array of x coordinates
*    y:     array of y coordinates
*    z:     2-dimensional array of z coordinates
*           z[i][j] = value of z corresponding to point (x[i], y[j])
*
* An example of how to call data_C_save is:
*
*    data_C_save("./sinc3d.txt", nx, ny, &x[0], &y[0], &z[0][0]);
*
* License:
*    This document is released into the public domain.
*
****************************************************************************/
void data_C_save(char *fname, int nx, int ny, double *x, double *y, double *z)
   {

   /* Declare variables */
   unsigned int i, j;
   FILE *fid;

   /* Write data file */
   fid = fopen(fname, "w");
   fprintf(fid, "%13d %13d", nx, ny);
   fprintf(fid, "\n");
   for ( i=1; i<=nx; i++ )
      fprintf(fid, "%+13.6e ", x[i-1]);
   fprintf(fid, "\n");
   for ( j=1; j<=ny; j++ )
      fprintf(fid, "%+13.6e ", y[j-1]);
   fprintf(fid, "\n");
   for ( i=1; i<=nx; i++ )
      {
      for ( j=1; j<=ny; j++ )
         fprintf(fid, "%+13.6e ", z[ny*(i-1)+j-1]);
      fprintf(fid, "\n");
      }
   fclose(fid);

   return;
   }
