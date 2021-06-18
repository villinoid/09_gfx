#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"


/*============================================
  IMPORANT NOTE
  Ambient light is represeneted by a color value
  Point light sources are 2D arrays of doubles.
       - The fist index (LOCATION) represents the vector to the light.
       - The second index (COLOR) represents the color.
  Reflection constants (ka, kd, ks) are represened as arrays of
  doubles (red, green, blue)
  ============================================*/


//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  color a=calculate_ambient(alight,areflect);
  color d=calculate_diffuse(light,dreflect,normal);
  color s=calculate_specular(light,sreflect,view,normal);
  i.red=s.red+d.red+a.red;
  i.blue=s.blue+d.blue+a.blue;
  i.green=s.green+d.green+a.green;
 // printf("i:\n");

  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red=areflect[RED]*alight.red;
  a.blue=areflect[BLUE]*alight.blue;
  a.green=areflect[GREEN]*alight.green;
   // printf("a:\n");

    limit_color(&a);
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  normalize(normal);
  normalize(light[LOCATION]);
  double nl=dot_product(normal,light[LOCATION]);
   if (nl<0)
	  nl*=0;
  d.red=dreflect[RED] * light[COLOR][RED] * nl;
  d.blue=dreflect[BLUE] * light[COLOR][BLUE] * nl;
  d.green=dreflect[GREEN] * light[COLOR][GREEN] * nl;
  limit_color(&d);
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  normalize(normal);
  normalize(light[LOCATION]);
  normalize(view);
  double nl=dot_product(normal,light[LOCATION]);
  double R[3];
  double exponent=4;
  R[0]=2*normal[0]*nl-light[LOCATION][0];
  R[1]=2*normal[1]*nl-light[LOCATION][1];
  R[2]=2*normal[2]*nl-light[LOCATION][2];
  normalize(R);
  double RV=dot_product(R,view);
  if (RV<0)
	  RV=0;
  double cosa=pow(RV, exponent);
  s.red=sreflect[RED]*light[COLOR][RED] * cosa;
  s.green=sreflect[GREEN]*light[COLOR][GREEN] * cosa;
  s.blue=sreflect[BLUE]*light[COLOR][BLUE] * cosa;
   limit_color(&s);
  return s;
}

//limit each component of c to a max of 255
void limit_color( color * c ) {
	//printf("%d %d %d\n",c->red,c->green,c->blue); 
	if (c->red>255)
		c->red=255;
	if (c->red<0)
		c->red=0;
	if (c->blue>255)
		c->blue=255;
	if (c->blue<0)
		c->blue=0;
	if (c->green>255)
		c->green=255;
	if (c->green<0)
		c->green=0;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude;
  magnitude = sqrt( vector[0] * vector[0] +
                    vector[1] * vector[1] +
                    vector[2] * vector[2] );
  int i;
  for (i=0; i<3; i++) {
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


//Calculate the surface normal for the triangle whose first
//point is located at index i in polygons
double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}