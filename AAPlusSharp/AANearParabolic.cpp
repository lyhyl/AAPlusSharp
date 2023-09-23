/*
Module : AANearParabolic.cpp
Purpose: Implementation for the algorithms for a near parabolic orbit
Created: PJN / 21-11-2006
History: PJN / 16-03-2009 1. Fixed a bug in CAANearParabolic::Calculate(double JD, 
                          CAANearParabolicObjectElements& elements) in the calculation of the 
                          heliocentric rectangular ecliptical, the heliocentric ecliptical latitude and 
                          the heliocentric ecliptical longitude coordinates. The code incorrectly used the 
                          value "omega" instead of "w" in its calculation of the value "u". Unfortunately 
                          there is no worked examples in Jean Meeus's book for these particular values, 
                          hence resulting in my coding errors. Thanks to Jay Borseth for reporting this bug.
         PJN / 10-05-2010 1. The CAANearParabolicObjectDetails::AstrometricGeocenticRA value is now known as
                          AstrometricGeocentricRA. Thanks to Scott Marley for reporting this spelling mistake
         PJN / 08-09-2013 1. Fixed a bug in the calculation of HeliocentricEclipticLongitude and 
                          HeliocentricEclipticLatitude in CAANearParabolic::Calculate. Thanks to Joe Novak 
                          for reporting this problem.
         PJN / 16-09-2015 1. CAANearParabolic::Calculate now includes a "bool bHighPrecision" parameter which 
                          if set to true means the code uses the full VSOP87 theory rather than the truncated
                          theory as presented in Meeus's book.
         PJN / 18-08-2019 1. Fixed some further compiler warnings when using VC 2019 Preview v16.3.0 Preview 2.0
         PJN / 29-06-2022 1. Updated all the code in AANearParabolic.cpp to use C++ uniform initialization for
                          all variable declarations.
         PJN / 28-01-2023 1. Renamed CAANearParabolic::CalculateTrueAnnomalyAndRadius to 
                          CAANearParabolic::CalculateTrueAnomalyAndRadius.

Copyright (c) 2006 - 2023 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code.

*/


//////////////////// Includes /////////////////////////////////////////////////

#include "stdafx.h"
#include "AANearParabolic.h"
#include "AACoordinateTransformation.h"
#include "AASun.h"
#include "AANutation.h"
#include "AAElliptical.h"
#include <cmath>
using namespace AAPlusSharp;

//////////////////// Implementation ///////////////////////////////////////////

double CAANearParabolic::cbrt(double x)
{
  return pow(x, 1.0/3);
}

void CAANearParabolic::CalculateTrueAnomalyAndRadius(double JD, CAANearParabolicObjectElements% elements, double% v, double% r)
{
  double k{0.01720209895};
  double a{0.75*(JD - elements.T)*k*sqrt((1 + elements.e)/(elements.q*elements.q*elements.q))};
  double b{sqrt(1 + (a*a))};
  double W{cbrt(b + a) - cbrt(b - a)};
  double W2{W*W};
  double W4{W2*W2};
  double f{(1 - elements.e)/(1 + elements.e)};
  double a1{(2.0/3) + (0.4*W2)};
  double a2{(7.0/5) + ((33.0/35)*W2) + ((37.0/175)*W4)};
  double a3{W2*((432.0/175) + ((956.0/1125)*W2) + ((84.0/1575)*W4))};
  double C{W2/(1 + W2)};
  double g{f*C*C};
  double w{W*(1 + (f*C*(a1 + (a2*g) + (a3*g*g))))};
  double w2{w*w};
  v = 2*atan(w);
  r = elements.q*(1 + w2)/(1 + (w2*f));
}

CAANearParabolicObjectDetails CAANearParabolic::Calculate(double JD, CAANearParabolicObjectElements% elements, bool bHighPrecision)
{
  double Epsilon{CAANutation::MeanObliquityOfEcliptic(elements.JDEquinox)};

  double JD0{JD};

  //What will be the return value
  CAANearParabolicObjectDetails details;

  Epsilon = CAACoordinateTransformation::DegreesToRadians(Epsilon);
  double omega{CAACoordinateTransformation::DegreesToRadians(elements.omega)};
  double w{CAACoordinateTransformation::DegreesToRadians(elements.w)};
  double i{CAACoordinateTransformation::DegreesToRadians(elements.i)};

  double sinEpsilon{sin(Epsilon)};
  double cosEpsilon{cos(Epsilon)};
  double sinOmega{sin(omega)};
  double cosOmega{cos(omega)};
  double cosi{cos(i)};
  double sini{sin(i)};

  double F{cosOmega};
  double G{sinOmega*cosEpsilon};
  double H{sinOmega*sinEpsilon};
  double P{-sinOmega*cosi};
  double Q{(cosOmega*cosi*cosEpsilon) - (sini*sinEpsilon)};
  double R{(cosOmega*cosi*sinEpsilon) + (sini*cosEpsilon)};
  double a{sqrt((F*F) + (P*P))};
  double b{sqrt((G*G) + (Q*Q))};
  double c{sqrt((H*H) + (R*R))};
  double A{atan2(F, P)};
  double B{atan2(G, Q)};
  double C{atan2(H, R)};

  CAA3DCoordinate SunCoord{CAASun::EquatorialRectangularCoordinatesAnyEquinox(JD, elements.JDEquinox, bHighPrecision)};
  for (int j{0}; j<2; j++)
  {
    double v{0};
    double r{0};
    CalculateTrueAnomalyAndRadius(JD0, elements, v, r);
    double r2{r*r};

    double x{r*a*sin(A + w + v)};
    double y{r*b*sin(B + w + v)};
    double z{r*c*sin(C + w + v)};

    if (j == 0)
    {
      details.HeliocentricRectangularEquatorial.X = x;
      details.HeliocentricRectangularEquatorial.Y = y;
      details.HeliocentricRectangularEquatorial.Z = z;

      //Calculate the heliocentric ecliptic coordinates also
      double u{w + v};
      double cosu{cos(u)};
      double sinu{sin(u)};

      details.HeliocentricRectangularEcliptical.X = r*((cosOmega*cosu) - (sinOmega*sinu*cosi));
      details.HeliocentricRectangularEcliptical.Y = r*((sinOmega*cosu) + (cosOmega*sinu*cosi));
      details.HeliocentricRectangularEcliptical.Z = r*sini*sinu;

      details.HeliocentricEclipticLongitude = CAACoordinateTransformation::MapTo0To360Range(CAACoordinateTransformation::RadiansToDegrees(atan2(details.HeliocentricRectangularEcliptical.Y, details.HeliocentricRectangularEcliptical.X)));
      details.HeliocentricEclipticLatitude = CAACoordinateTransformation::RadiansToDegrees(asin(details.HeliocentricRectangularEcliptical.Z/r));
    }

    double psi{SunCoord.X + x};
    double psi2{psi*psi};
    double nu{SunCoord.Y + y};
    double nu2{nu*nu};
    double sigma{SunCoord.Z + z};

    double Alpha{atan2(nu, psi)};
    Alpha = CAACoordinateTransformation::RadiansToDegrees(Alpha);
    double Delta{atan2(sigma, sqrt(psi2 + nu2))};
    Delta = CAACoordinateTransformation::RadiansToDegrees(Delta);
    double Distance{sqrt(psi2 + nu2 + (sigma*sigma))};
    double Distance2{Distance*Distance};

    if (j == 0)
    {
      details.TrueGeocentricRA = CAACoordinateTransformation::MapTo0To24Range(Alpha/15);
      details.TrueGeocentricDeclination = Delta;
      details.TrueGeocentricDistance = Distance;
      details.TrueGeocentricLightTime = CAAElliptical::DistanceToLightTime(Distance);
    }
    else
    {
      details.AstrometricGeocentricRA = CAACoordinateTransformation::MapTo0To24Range(Alpha/15);
      details.AstrometricGeocentricDeclination = Delta;
      details.AstrometricGeocentricDistance = Distance;
      details.AstrometricGeocentricLightTime = CAAElliptical::DistanceToLightTime(Distance);

      double RES{sqrt((SunCoord.X*SunCoord.X) + (SunCoord.Y*SunCoord.Y) + (SunCoord.Z*SunCoord.Z))};
      double RES2{RES*RES};

      details.Elongation = CAACoordinateTransformation::RadiansToDegrees(acos((RES2 + Distance2 - r2) / (2*RES*Distance)));
      details.PhaseAngle = CAACoordinateTransformation::RadiansToDegrees(acos((r2 + Distance2 - RES2) / (2*r*Distance)));
    }

    if (j == 0) //Prepare for the next loop around
      JD0 = JD - details.TrueGeocentricLightTime;
  }

  return details;
}
