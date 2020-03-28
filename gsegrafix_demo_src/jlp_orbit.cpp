/***********************************************************************
* jlp_orbit.cpp
*
* Calculates orbit coordinates for specified orbit parameters.
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
// conversion from degrees to radians (rad/deg)
#define DEG2RAD (PI/180)           

static int compute_orbit_data(double eccen, double period_hr,
                              double *theta0, double *rad0, double *time0,
                              double *x_orbit0, double *y_orbit0,
                              double *x_ground0, double *y_ground0,
                              int npts);
static int jlp_write_to_file1(char *fname, int npts, double *x_orbit0, 
                              double *y_orbit0, double *x_ground0, 
                              double *y_ground0);
static int jlp_interp1(double *time0, double *x_orbit0, double *time_interp0, 
                       double *x_orbit_interp0, int ntime, int npts);

int main(int argc, char *argv[])
{
double eccen, period_hr, wstart, wend, wstep, ww0, ww1;
double *theta0, *rad0, *time0, *x_orbit0, *y_orbit0, *x_ground0, *y_ground0;
double *time_interp0;
double *x_orbit_interp0, *y_orbit_interp0, *x_ground_interp0, *y_ground_interp0;
int ntime, npts = 256;
int i, j, status;
char fname[128];

// theta0: mean anomaly angle from perigee (rad)
theta0 = new double[npts];
// rad0: radius (m) from focus of elliptical orbit
rad0 = new double[npts];
// time0: time as a function of angle from the perigee
time0 = new double[npts];
x_orbit0 = new double[npts]; 
y_orbit0 = new double[npts]; 
x_ground0 = new double[npts]; 
y_ground0 = new double[npts]; 

// Filling the theta0 array, spanning the intervall [0., 1.]
wstart = 0.;
wend = +1.;
wstep = (wend - wstart) / ((double)npts - 1.);
for(i = 0; i < npts; i++) {
  ww0 = wstart + (double)i * wstep;
  ww1 = 2.0 * PI * ww0; 
  theta0[i] = ww1; 
  }

// Specify orbit parameters
// eccentricity
 eccen = 0.7;                
// period in hours:
 period_hr = 12.;

 status = compute_orbit_data(eccen, period_hr, theta0, time0, rad0, 
                             x_orbit0, y_orbit0, x_ground0, y_ground0, npts);

// Save orbit and ground-track data to file
strcpy(fname, "jlp_orbit.txt");
status = jlp_write_to_file1(fname, npts, x_orbit0, y_orbit0,
                             x_ground0, y_ground0);


// Interpolate to find angle from perigee for equal time increments of 10 mn
wstep = 600.;
ntime = (period_hr * 3600.) / wstep;
time_interp0 = new double[ntime];
for(i = 0; i < ntime; i++) {
  time_interp0[i] = (double)i * wstep; 
  }
x_orbit_interp0 = new double[ntime]; 
y_orbit_interp0 = new double[ntime]; 
x_ground_interp0 = new double[ntime]; 
y_ground_interp0 = new double[ntime]; 

 jlp_interp1(time0, x_orbit0,  time_interp0, x_orbit_interp0, ntime, npts);
 jlp_interp1(time0, y_orbit0,  time_interp0, y_orbit_interp0, ntime, npts);
 jlp_interp1(time0, x_ground0, time_interp0, x_ground_interp0, ntime, npts);
 jlp_interp1(time0, y_ground0, time_interp0, y_ground_interp0, ntime, npts);

#if 0
strcpy(fname, "jlp_orbit_lines.txt");
% Save data for lines connecting corresponding points on orbit and ground track
fname = "/home/sab/gsegrafix/data_files/orbit_lines.txt";
fid = fopen(fname, "w");
npts_interp = length(time_interp);
for ( i = 1:npts_interp )
   fprintf(fid, "%15.5e %15.5e\n", x_orbit_interp(i)/1000,  y_orbit_interp(i)/1000);
   fprintf(fid, "%15.5e %15.5e\n", x_ground_interp(i)/1000, y_ground_interp(i)/1000);
   fprintf(fid, "%15s %15s\n", '*', '*');
end
fclose(fid);
#endif

delete[] rad0;
delete[] theta0;
delete[] time0;
delete[] x_orbit0;
delete[] y_orbit0;
delete[] x_ground0;
delete[] y_ground0;
return(status);
}
/*****************************************************************************
*
*****************************************************************************/
static int jlp_interp1(double *time0, double *x_orbit0, double *time_interp0, 
                       double *x_orbit_interp0, int ntime, int npts)
{
return(0);
}
/***********************************************************************
* Save orbit and ground-track data to file
*
***********************************************************************/
static int jlp_write_to_file1(char *fname, int npts, double *x_orbit0, 
                              double *y_orbit0, double *x_ground0, 
                              double *y_ground0)
{
int i;
FILE *fp;

if((fp = fopen(fname, "w")) == NULL) {
  fprintf(stderr, " jlp_write_to_file/error opening %s\n", fname);
  return(-1);
  }

// Save orbit and ground-track data to file
// Conversion from m to km
for(i = 0; i < npts; i++) {
   fprintf(fp, "%15.5e %15.5e %15.5e %15.5e\n", 
            x_orbit0[i]/1000.,  y_orbit0[i]/1000., 
            x_ground0[i]/1000., y_ground0[i]/1000.);
  }

fclose(fp);

return(0);
}
/**************************************************************************
* Orbit equations of satellites orbiting around the Earth
*
* Reference:
*    Roger R. Bate, Donald D. Mueller, Jerry E. White,
*    Fundamentals of Astrodynamics, Dover, 1971 (BMW)
*
* rr = pp/[1 + eccen*cos(theta)]           (BMW, p. 20, eq. 1.5-4)
* pp = aa*(1 - ee*ee)                    (BMW, p. 24, eq. 1.5-6)
* pp = hh*hh/mu                         (BMW, p. 26, eq. 1.6-1)
* dt = (rr*rr/hh)*dtheta                (BMW, p. 31, eq. 1.7-6)
* period = [2*pi/sqrt(mu)]*aa^(3/2)   (BMW, p. 33, eq. 1.7-9)
* where:
*    rr     = radius (m) from focus of elliptical orbit
*    theta  = angle (rad) from perigee
*    aa     = semi-major axis (m) of elliptical orbit
*    eccen  = eccentricity of elliptical orbit
*    pp     = semi-latus rectum (m)
*    hh     = specific angular momentum (m^2/sec)
*    mu     = earth gravitational constant (m^3/sec^2)
*    dt     = time differential (sec)
*    dtheta = angle differential (rad)
*    period = orbit period (sec)
*
* INPUT:
*  eccen : eccentricity
*  period_hr : orbital period in hours
*  theta0[npts] : mean anomaly in radians
*  rad0[npts] : radius, i.e., distance of the satellite from the Earth 
*  npts : number of points of theta0[] and rad0[]
**************************************************************************/
static int compute_orbit_data(double eccen, double period_hr,
                              double *theta0, double *rad0, double *time0,
                              double *x_orbit0, double *y_orbit0,
                              double *x_ground0, double *y_ground0,
                              int npts)
{
double period;             // period (in seconds)
double aa, pp, hh, theta, rr, ww0, ww1, ww2, ww3;
int i;

// Define constants
double Rsphere = 6.37100079e+06;   // spherical-earth radius (m)
double mu      = 3.986005e+14;     // earth gravitational constant (m^3/sec^2)

// Conversion of the period in hours to seconds:
period = period_hr * 3600;  // period (sec)

// Calculate orbit in polar coordinates

// aa: semi-major axis (m)
aa = pow((period/(2 * PI / sqrt(mu))),(2./3.));   

printf("semi-major-axis: a=%f (km)\n", aa/1000.);

// pp: semi-latus rectum (m)
pp = aa * (1.0 - eccen * eccen);                      
printf("semi-latus rectum: p=%f (km)\n", pp/1000.);

// hh: specific angular momentum (m^2/sec)
hh = sqrt(mu * pp);                       

// theta: mean anomaly angle from perigee (rad)
// rr: radius (m) from focus of elliptical orbit
for( i = 0; i < npts; i++) {
  rad0[i] = pp / (1.0 + eccen * cos(theta0[i]));            
  }

/*
* Calculate time as a function of angle from perigee
*    dt = (r*r/h)*dtheta
*       = (p*p/h)*d(theta)/[1 + e*cos(theta)]^2
*    t = (p*p/h)*[e*sin(theta)/((e^2 - 1)*(1 + e*cos(theta))) - ...
*                 (1/(e^2 - 1))*(2/(sqrt(1 - e^2)))*atan((1 - e)* ...
*                 tan(theta/2)/sqrt(1 - e^2))]
*/


for(i = 0; i < npts; i++) { 
   ww0 = ((pp * pp) / hh) * (eccen * sin(theta0[i]));
   ww1 = (eccen * eccen - 1.0) * (1.0 + eccen * cos(theta0[i])); 
   ww3 = (1.0 /(eccen * eccen - 1.0)) * (2./sqrt(1.0 - eccen * eccen))
         * atan((1.0 - eccen) * tan(theta0[i]/2.0) / sqrt(1 - eccen * eccen));
   time0[i] = ww0 / ww1 - ww2 - ww3; 
  }
for(i = ((npts + 1)/2); i < npts; i++) { 
  time0[i] = 2.0 * time0[npts/2] - time0[npts/2 - i];
  }

time0[npts - 1] = period;

// Calculate orbit in rectangular coordinates
for(i = 0; i < npts; i++) {
  x_orbit0[i] = -rad0[i] * cos(theta0[i]);
  y_orbit0[i] = -rad0[i] * sin(theta0[i]);
  }

// Calculate ground track in rectangular coordinates
for(i = 0; i < npts; i++) {
  x_ground0[i] = -Rsphere * cos(theta0[i]);
  y_ground0[i] = -Rsphere * sin(theta0[i]);
  }

return(0);
}
