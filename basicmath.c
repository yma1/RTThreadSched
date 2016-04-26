#include "main.h"

double rad2deg(double rad)
{
  double res = 0;
  int i = 0;
  for (i = 0; i < MAX_LOOPS; i++)
    res = (180.0 * rad / (PI));
  return res;
}

double deg2rad(double deg)
{
  double res = 0;
  int i = 0;
  for (i = 0; i < MAX_LOOPS; i++)
    res = (PI * deg / 180.0);
  return res;
}

void solveCubic(double  a,
                double  b,
                double  c,
                double  d,
                int    *solutions,
                double *x)
{
  long double    a1 = b/a, a2 = c/a, a3 = d/a;
  long double    Q = (a1*a1 - 3.0*a2)/9.0;
  long double R = (2.0*a1*a1*a1 - 9.0*a1*a2 + 27.0*a3)/54.0;
  double    R2_Q3 = R*R - Q*Q*Q;

  double    theta;

  if (R2_Q3 <= 0)
  {
    *solutions = 3;
    theta = acos(R/sqrt(Q*Q*Q));
    x[0] = -2.0*sqrt(Q)*cos(theta/3.0) - a1/3.0;
    x[1] = -2.0*sqrt(Q)*cos((theta+2.0*PI)/3.0) - a1/3.0;
    x[2] = -2.0*sqrt(Q)*cos((theta+4.0*PI)/3.0) - a1/3.0;
  }
  else
  {
    *solutions = 1;
    x[0] = pow(sqrt(R2_Q3)+fabs(R), 1/3.0);
    x[0] += Q/x[0];
    x[0] *= (R < 0.0) ? 1 : -1;
    x[0] -= a1/3.0;
  }
}
