/*******************************************************************************
*
* Calculate Mandelbrot set.
*
* The Mandelbrot set consists of a set of points, c = x + iy, in the complex
* plane. These points must meet the condition that the magnitude of the complex
* number z does not grow without bound when the following operation is performed
* a large number of times:
*    z(n+1) = z(n)*z(n) + c, where z(0) = 0 + i0.
* When the data are plotted, colors are assigned to values of c according to the
* number of times the above operation had to be performed to cause the magnitude
* of z to exceed a specified value, in this case 2. The color black is assigned
* to points in the Mandelbrot set.
*
* Compile: gcc -Wall mandelbrot.c -o mandelbrot -lm
*
*******************************************************************************/
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

int main ( int argc, char *argv[] )
   {
   /* Declare variables */
   unsigned int i, j, nx, ny, count[500][500], count_max;
   double xmin, xmax, ymin, ymax, x[500], y[500], rmax;
   _Complex double c, z;
   char fname[] = "./jlp_mandelbrot.txt";
   FILE *fid;

   /* Define limits of calculation */
   xmin = -2.0;
   xmax =  0.5;
   ymin = -1.25;
   ymax =  1.25;

   /* Calculate x and y values for plotting area 433 pixels by 433 pixels */
   nx = 433;
   for ( i=1; i<=nx; i++ )
      x[i-1] = xmin + ( (double) (i - 1)/ (double) (nx - 1))*(xmax - xmin);

   ny = 433;
   for ( j=1; j<=ny; j++ )
      y[j-1] = ymin + ( (double) (j - 1)/ (double) (ny - 1))*(ymax - ymin);

   /* Set values of count to 0 for each value of x and y */
   for ( i=1; i<=nx; i++ )
      for ( j=1; j<=ny; j++ )
         count[i-1][j-1] = 0;

   /* Calculate Mandelbrot-set data */
   rmax = 2.0;
   count_max = 10000;
   for ( i=1; i<=nx; i++ )
      for ( j=1; j<=ny; j++ )
         {
         c = x[i-1] + I*y[j-1];
         z = 0.0;
         while ( (cabs(z) <= rmax) && (count[i-1][j-1] <= count_max) )
            {
            z = z*z + c;
            count[i-1][j-1]++;
            }
         if ( count[i-1][j-1] > 0 )
            count[i-1][j-1]--;
         }

   /* Save data to file */
   fid = fopen(fname, "w");
   fprintf(fid, "%u %u", nx, ny);
   fprintf(fid, "\n");
   for ( i=1; i<=nx; i++ )
      fprintf(fid, "%+10.7f ", x[i-1]);
   fprintf(fid, "\n");
   for ( j=1; j<=ny; j++ )
      fprintf(fid, "%+10.7f ", y[j-1]);
   fprintf(fid, "\n");
   for ( i=1; i<=nx; i++ )
      {
      for ( j=1; j<=ny; j++ )
         {
         if ( count[i-1][j-1] == count_max )
            fprintf(fid, "%5d ", -1);
         else
            fprintf(fid, "%5u ", count[i-1][j-1]);
         }
      fprintf(fid, "\n");
      }
   fclose(fid);

   return 0;
   }
