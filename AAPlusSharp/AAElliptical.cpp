/*
Module : AAElliptical.cpp
Purpose: Implementation for the algorithms for an elliptical orbit
Created: PJN / 29-12-2003
History: PJN / 24-05-2004 1. Fixed a missing break statement in CAAElliptical::Calculate. Thanks to
                          Carsten A. Arnholm for reporting this bug. 
                          2. Also fixed an issue with the calculation of the apparent distance to 
                          the Sun.
         PJN / 31-12-2004 1. Fix for CAAElliptical::MinorPlanetMagnitude where the phase angle was
                          being incorrectly converted from Radians to Degrees when it was already
                          in degrees. Thanks to Martin Burri for reporting this problem.
         PJN / 05-06-2006 1. Fixed a bug in CAAElliptical::Calculate(double JD, EllipticalObject object)
                          where the correction for nutation was incorrectly using the Mean obliquity of
                          the ecliptic instead of the true value. The results from the test program now 
                          agree much more closely with the example Meeus provides which is the position 
                          of Venus on 1992 Dec. 20 at 0h Dynamical Time. I've also checked the positions
                          against the JPL Horizons web site and the agreement is much better. Because the
                          True obliquity of the Ecliptic is defined as the mean obliquity of the ecliptic
                          plus the nutation in obliquity, it is relatively easy to determine the magnitude
                          of error this was causing. From the chapter on Nutation in the book, and 
                          specifically the table which gives the cosine coefficients for nutation in 
                          obliquity you can see that the absolute worst case error would be the sum of the 
                          absolute values of all of the coefficients and would have been c. 10 arc seconds 
                          of degree, which is not a small amount!. This value would be an absolute worst 
                          case and I would expect the average error value to be much much smaller 
                          (probably much less than an arc second). Anyway the bug has now been fixed. 
                          Thanks to Patrick Wong for pointing out this rather significant bug. 
         PJN / 10-11-2008 1. Fixed a bug in CAAElliptical::Calculate(double JD, 
                          CAAEllipticalObjectElements& elements) in the calculation of the 
                          heliocentric rectangular ecliptical, the heliocentric ecliptical latitude and 
                          the heliocentric ecliptical longitude coordinates. The code incorrectly used the 
                          value "omega" instead of "w" in its calculation of the value "u". Unfortunately 
                          there is no worked examples in Jean Meeus's book for these particular values, 
                          hence resulting in my coding errors. Thanks to Carsten A. Arnholm for reporting 
                          this bug. 
         PJN / 10-05-2010 1. The CAAEllipticalObjectDetails::AstrometricGeocenticRA value is now known as
                          AstrometricGeocentricRA. Thanks to Scott Marley for reporting this spelling mistake
         PJN / 12-07-2015 1. Fixed a bug in CAAElliptical::Calculate when calculating values for the position
                          of the Sun. The values returned by this method are now consistent with those returned
                          using the methods of the CAASun class, the planetarium program SkyMap and the JPL
                          HORIZON's web site. The errors were of the order of 4 arc seconds in declination and
                          1.5 seconds of Right ascension for modern times. I have also taken the opportunity to
                          optimize the code in this method. With these changes the errors are now down to 0.5 
                          seconds of an angle in declination and right ascension. Thanks to Marko Peric for 
                          reporting this bug.
         PJN / 16-09-2015 1. CAAElliptical::Calculate now includes a "bool bHighPrecision" parameter which if 
                          set to true means the code uses the full VSOP87 theory rather than the truncated 
                          theory as presented in Meeus's book.
         PJN / 18-08-2019 1. Fixed some further compiler warnings when using VC 2019 Preview v16.3.0 Preview 2.0
         PJN / 03-10-2021 1. Renamed CAAElliptical::EllipticalObject type to Object.
         PJN / 19-06-2022 1. Updated all the code in AAElliptical.cpp to use C++ uniform initialization for all
                          variable declarations.
         PJN / 28-08-2022 1. Fixed an issue in CAAElliptical::MinorPlanetMagnitude where brackets were missing. 
                          Thanks to "Pavel" for reporting this issue.
         PJN / 01-10-2022 1. Updated the CAAElliptical::Calculate method which takes a Object parameter to return 
                          the following additional values: TrueGeocentricRectangularEcliptical, 
                          TrueHeliocentricEclipticalLongitude, TrueHeliocentricEclipticalLatitude,
                          TrueHeliocentricDistance, TrueGeocentricEclipticalLongitude, 
                          TrueGeocentricEclipticalLatitude, TrueGeocentricDistance, TrueLightTime, 
                          TrueGeocentricRA & TrueGeocentricDeclination.
                          2. Fixed an issue in CAAElliptical::Calculate method which takes a Object parameter 
                          when the Object parameter is the Sun and the code is attempting to calculate the apparent
                          position of the Sun.
                          3. Renamed CAAEllipticalPlanetaryDetails::ApparentGeocentricLongitude to 
                          ApparentGeocentricEclipticalLongitude.
                          4. Renamed CAAEllipticalPlanetaryDetails::ApparentGeocentricLatitude to 
                          ApparentGeocentricEclipticalLatitude.

Copyright (c) 2003 - 2023 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

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
#include "AAElliptical.h"
#include "AAAberration.h"
#include "AACoordinateTransformation.h"
#include "AASun.h"
#include "AAMercury.h"
#include "AAVenus.h"
#include "AAEarth.h"
#include "AAMars.h"
#include "AAJupiter.h"
#include "AASaturn.h"
#include "AAUranus.h"
#include "AANeptune.h"
#include "AAPluto.h"
#include "AAFK5.h"
#include "AANutation.h"
#include "AAKepler.h"
#include <cmath>
#include <cassert>
using namespace AAPlusSharp;

//////////////////// Implementation ///////////////////////////////////////////

CAAEllipticalPlanetaryDetails CAAElliptical::Calculate(double JD, Object object, bool bHighPrecision)
{
  //What will be the return value
  CAAEllipticalPlanetaryDetails details;

  //Calculate the position of the earth first
  double JD0{JD};
  double L0{CAAEarth::EclipticLongitude(JD0, bHighPrecision)};
  double B0{CAAEarth::EclipticLatitude(JD0, bHighPrecision)};
  double R0{CAAEarth::RadiusVector(JD0, bHighPrecision)};
  L0 = CAACoordinateTransformation::DegreesToRadians(L0);
  B0 = CAACoordinateTransformation::DegreesToRadians(B0);
  double cosB0{cos(B0)};

  //Iterate to find the positions adjusting for light-time correction if required
  double L{0};
  double B{0};
  double R{0};
  if (object != Object::SUN)
  {
    bool bRecalc{true};
    bool bFirstRecalc{true};
    double LPrevious{0};
    double BPrevious{0};
    double RPrevious{0};
    while (bRecalc)
    {
      switch (object)
      {
        case Object::MERCURY:
        {
          L = CAAMercury::EclipticLongitude(JD0, bHighPrecision);
          B = CAAMercury::EclipticLatitude(JD0, bHighPrecision);
          R = CAAMercury::RadiusVector(JD0, bHighPrecision);
          break;
        }
        case Object::VENUS:
        {
          L = CAAVenus::EclipticLongitude(JD0, bHighPrecision);
          B = CAAVenus::EclipticLatitude(JD0, bHighPrecision);
          R = CAAVenus::RadiusVector(JD0, bHighPrecision);
          break;
        }
        case Object::MARS:
        {
          L = CAAMars::EclipticLongitude(JD0, bHighPrecision);
          B = CAAMars::EclipticLatitude(JD0, bHighPrecision);
          R = CAAMars::RadiusVector(JD0, bHighPrecision);
          break;
        }
        case Object::JUPITER:
        {
          L = CAAJupiter::EclipticLongitude(JD0, bHighPrecision);
          B = CAAJupiter::EclipticLatitude(JD0, bHighPrecision);
          R = CAAJupiter::RadiusVector(JD0, bHighPrecision);
          break;
        }
        case Object::SATURN:
        {
          L = CAASaturn::EclipticLongitude(JD0, bHighPrecision);
          B = CAASaturn::EclipticLatitude(JD0, bHighPrecision);
          R = CAASaturn::RadiusVector(JD0, bHighPrecision);
          break;
        }
        case Object::URANUS:
        {
          L = CAAUranus::EclipticLongitude(JD0, bHighPrecision);
          B = CAAUranus::EclipticLatitude(JD0, bHighPrecision);
          R = CAAUranus::RadiusVector(JD0, bHighPrecision);
          break;
        }
        case Object::NEPTUNE:
        {
          L = CAANeptune::EclipticLongitude(JD0, bHighPrecision);
          B = CAANeptune::EclipticLatitude(JD0, bHighPrecision);
          R = CAANeptune::RadiusVector(JD0, bHighPrecision);
          break;
        }
        default:
        {
          assert(false);
          break;
        }
      }

      bool bFirstCalc = false;
      if (!bFirstRecalc)
      {
        bRecalc = ((fabs(L - LPrevious) > 0.00001) || (fabs(B - BPrevious) > 0.00001) || (fabs(R - RPrevious) > 0.000001));
        LPrevious = L;
        BPrevious = B;
        RPrevious = R;
      }
      else
      {
        bFirstCalc = true;
        bFirstRecalc = false;
        details.TrueHeliocentricEclipticalLongitude = L;
        details.TrueHeliocentricEclipticalLatitude = B;
        details.TrueHeliocentricDistance = R;
      }

      double Lrad{CAACoordinateTransformation::DegreesToRadians(L)};
      double Brad{CAACoordinateTransformation::DegreesToRadians(B)};
      double cosB{cos(Brad)};
      double cosL{cos(Lrad)};
      double x{(R*cosB*cosL) - (R0*cosB0*cos(L0))};
      double y{(R*cosB*sin(Lrad)) - (R0*cosB0*sin(L0))};
      double z{(R*sin(Brad)) - (R0*sin(B0))};
      double distance{sqrt((x*x) + (y*y) + (z*z))};
      if (bFirstCalc)
      {
        details.TrueGeocentricRectangularEcliptical.X = x;
        details.TrueGeocentricRectangularEcliptical.Y = y;
        details.TrueGeocentricRectangularEcliptical.Z = z;
      }

      //Prepare for the next loop around
      if (bRecalc)
        JD0 = JD - CAAElliptical::DistanceToLightTime(distance);
    }
  }
  else
  {
    bool bRecalc{true};
    bool bFirstRecalc{true};
    double LPrevious{0};
    double BPrevious{0};
    double RPrevious{0};
    while (bRecalc)
    {
      L = CAAEarth::EclipticLongitude(JD0, bHighPrecision);
      B = CAAEarth::EclipticLatitude(JD0, bHighPrecision);
      R = CAAEarth::RadiusVector(JD0, bHighPrecision);

      bool bFirstCalc = false;
      if (!bFirstRecalc)
      {
        bRecalc = ((fabs(L - LPrevious) > 0.00001) || (fabs(B - BPrevious) > 0.00001) || (fabs(R - RPrevious) > 0.000001));
        LPrevious = L;
        BPrevious = B;
        RPrevious = R;
      }
      else
      {
        bFirstCalc = true;
        bFirstRecalc = false;
      }

      if (bFirstCalc)
      {
        double Lrad{CAACoordinateTransformation::DegreesToRadians(L)};
        double Brad{CAACoordinateTransformation::DegreesToRadians(B)};
        double cosB{cos(Brad)};
        double cosL{cos(Lrad)};
        details.TrueGeocentricRectangularEcliptical.X = -R*cosB*cosL;
        details.TrueGeocentricRectangularEcliptical.Y = -R*cosB*sin(Lrad);
        details.TrueGeocentricRectangularEcliptical.Z = -R*sin(Brad);
      }

      //Prepare for the next loop around
      if (bRecalc)
        JD0 = JD - CAAElliptical::DistanceToLightTime(R);
    }
  }

  double x{0};
  double y{0};
  double z{0};
  if (object != Object::SUN)
  {
    double Lrad{CAACoordinateTransformation::DegreesToRadians(L)};
    double Brad{CAACoordinateTransformation::DegreesToRadians(B)};
    double cosB{cos(Brad)};
    double cosL{cos(Lrad)};

    x = (R*cosB*cosL) - (R0*cosB0*cos(L0));
    y = (R*cosB*sin(Lrad)) - (R0*cosB0*sin(L0));
    z = (R*sin(Brad)) - (R0*sin(B0));
  }
  else
  {
    double Lrad{CAACoordinateTransformation::DegreesToRadians(L)};
    double Brad{CAACoordinateTransformation::DegreesToRadians(B)};
    double cosB{cos(Brad)};
    double cosL{cos(Lrad)};

    x = -R*cosB*cosL;
    y = -R*cosB*sin(Lrad);
    z = -R*sin(Brad);
  }
  double x2{x*x};
  double y2{y*y};
  details.ApparentGeocentricEclipticalLatitude = CAACoordinateTransformation::RadiansToDegrees(atan2(z, sqrt(x2 + y2)));
  details.ApparentGeocentricDistance = sqrt(x2 + y2 + (z*z));
  details.ApparentGeocentricEclipticalLongitude = CAACoordinateTransformation::MapTo0To360Range(CAACoordinateTransformation::RadiansToDegrees(atan2(y, x)));
  details.ApparentLightTime = CAAElliptical::DistanceToLightTime(details.ApparentGeocentricDistance);

  x2 = {details.TrueGeocentricRectangularEcliptical.X*details.TrueGeocentricRectangularEcliptical.X};
  y2 = {details.TrueGeocentricRectangularEcliptical.Y*details.TrueGeocentricRectangularEcliptical.Y};
  details.TrueGeocentricEclipticalLatitude = CAACoordinateTransformation::RadiansToDegrees(atan2(details.TrueGeocentricRectangularEcliptical.Z, sqrt(x2 + y2)));
  details.TrueGeocentricDistance = sqrt(x2 + y2 + (details.TrueGeocentricRectangularEcliptical.Z*details.TrueGeocentricRectangularEcliptical.Z));
  details.TrueGeocentricEclipticalLongitude = CAACoordinateTransformation::MapTo0To360Range(CAACoordinateTransformation::RadiansToDegrees(atan2(details.TrueGeocentricRectangularEcliptical.Y, details.TrueGeocentricRectangularEcliptical.X)));
  details.TrueLightTime = CAAElliptical::DistanceToLightTime(details.TrueGeocentricDistance);

  //Adjust for Aberration
  CAA2DCoordinate Aberration{CAAAberration::EclipticAberration(details.ApparentGeocentricEclipticalLongitude, details.ApparentGeocentricEclipticalLatitude, JD, bHighPrecision)};
  details.ApparentGeocentricEclipticalLongitude += Aberration.X;
  details.ApparentGeocentricEclipticalLatitude += Aberration.Y;

  //convert to the FK5 system
  double DeltaLong{CAAFK5::CorrectionInLongitude(details.ApparentGeocentricEclipticalLongitude, details.ApparentGeocentricEclipticalLatitude, JD)};
  details.ApparentGeocentricEclipticalLatitude += CAAFK5::CorrectionInLatitude(details.ApparentGeocentricEclipticalLongitude, JD);
  details.ApparentGeocentricEclipticalLongitude += DeltaLong;

  //Correct for nutation
  double NutationInLongitude{CAANutation::NutationInLongitude(JD)};
  details.ApparentGeocentricEclipticalLongitude += CAACoordinateTransformation::DMSToDegrees(0, 0, NutationInLongitude);

  //Convert to RA and Dec
  double Epsilon{CAANutation::TrueObliquityOfEcliptic(JD)};
  CAA2DCoordinate ApparentEqu{CAACoordinateTransformation::Ecliptic2Equatorial(details.ApparentGeocentricEclipticalLongitude, details.ApparentGeocentricEclipticalLatitude, Epsilon)};
  details.ApparentGeocentricRA = ApparentEqu.X;
  details.ApparentGeocentricDeclination = ApparentEqu.Y;

  CAA2DCoordinate TrueEqu{CAACoordinateTransformation::Ecliptic2Equatorial(details.TrueGeocentricEclipticalLongitude, details.TrueGeocentricEclipticalLatitude, Epsilon)};
  details.TrueGeocentricRA = TrueEqu.X;
  details.TrueGeocentricDeclination = TrueEqu.Y;

  return details;
}

double CAAElliptical::MeanMotionFromSemiMajorAxis(double a)
{
  return 0.9856076686 / (a * sqrt(a));
}

CAAEllipticalObjectDetails CAAElliptical::Calculate(double JD, CAAEllipticalObjectElements% elements, bool bHighPrecision)
{
  double Epsilon{CAANutation::MeanObliquityOfEcliptic(elements.JDEquinox)};
  double JD0{JD};

  //What will be the return value
  CAAEllipticalObjectDetails details;

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
  double n{CAAElliptical::MeanMotionFromSemiMajorAxis(elements.a)};

  CAA3DCoordinate SunCoord{CAASun::EquatorialRectangularCoordinatesAnyEquinox(JD, elements.JDEquinox, bHighPrecision)};

  for (int j=0; j<2; j++)
  {
    double M{n*(JD0 - elements.T)};
    double E{CAAKepler::Calculate(M, elements.e)};
    E = CAACoordinateTransformation::DegreesToRadians(E);
    double v{2*atan(sqrt((1 + elements.e) / (1 - elements.e)) * tan(E/2))};
    double r{elements.a*(1 - (elements.e*cos(E)))};
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
      details.HeliocentricEclipticLatitude = CAACoordinateTransformation::RadiansToDegrees(asin(details.HeliocentricRectangularEcliptical.Z / r));
    }

    double psi{SunCoord.X + x};
    double nu{SunCoord.Y + y};
    double nusquared(nu*nu);
    double sigma{SunCoord.Z + z};
    double psisquared{psi*psi};

    double Alpha{atan2(nu, psi)};
    Alpha = CAACoordinateTransformation::RadiansToDegrees(Alpha);
    double Delta{atan2(sigma, sqrt(psisquared + nusquared))};
    Delta = CAACoordinateTransformation::RadiansToDegrees(Delta);
    double Distance{sqrt(psisquared + nusquared + (sigma*sigma))};

    if (j == 0)
    {
      details.TrueGeocentricRA = CAACoordinateTransformation::MapTo0To24Range(Alpha / 15);
      details.TrueGeocentricDeclination = Delta;
      details.TrueGeocentricDistance = Distance;
      details.TrueGeocentricLightTime = DistanceToLightTime(Distance);
    }
    else
    {
      details.AstrometricGeocentricRA = CAACoordinateTransformation::MapTo0To24Range(Alpha / 15);
      details.AstrometricGeocentricDeclination = Delta;
      details.AstrometricGeocentricDistance = Distance;
      details.AstrometricGeocentricLightTime = DistanceToLightTime(Distance);

      double RES{sqrt((SunCoord.X*SunCoord.X) + (SunCoord.Y*SunCoord.Y) + (SunCoord.Z*SunCoord.Z))};
      double rsquared(r*r);
      double Distancesquared(Distance*Distance);
      details.Elongation = acos(((RES*RES) + Distancesquared - rsquared)/(2*RES*Distance));
      details.Elongation = CAACoordinateTransformation::RadiansToDegrees(details.Elongation);

      details.PhaseAngle = acos((rsquared + Distancesquared - RES*RES)/(2*r*Distance));
      details.PhaseAngle = CAACoordinateTransformation::RadiansToDegrees(details.PhaseAngle);
    }

    if (j == 0) //Prepare for the next loop around
      JD0 = JD - details.TrueGeocentricLightTime;
  }

  return details;
}

double CAAElliptical::InstantaneousVelocity(double r, double a)
{
  return 42.1219*sqrt((1/r) - (1/(2*a)));
}

double CAAElliptical::VelocityAtPerihelion(double e, double a)
{
  return 29.7847/sqrt(a)*sqrt((1+e)/(1-e));
}

double CAAElliptical::VelocityAtAphelion(double e, double a)
{
  return 29.7847/sqrt(a)*sqrt((1-e)/(1+e));
}

double CAAElliptical::LengthOfEllipse(double e, double a)
{
  double b{a*sqrt(1 - (e*e))};
  return CAACoordinateTransformation::PI()*(3*(a+b) - sqrt((a+(3*b))*((3*a) + b)));
}

double CAAElliptical::CometMagnitude(double g, double delta, double k, double r)
{
  return g + (5*log10(delta)) + (k*log10(r));
}

double CAAElliptical::MinorPlanetMagnitude(double H, double delta, double G, double r, double PhaseAngle)
{
  //Convert from degrees to radians
  PhaseAngle = CAACoordinateTransformation::DegreesToRadians(PhaseAngle);

  double phi1{exp(-3.33*pow(tan(PhaseAngle/2), 0.63))};
  double phi2{exp(-1.87 * pow(tan(PhaseAngle / 2), 1.22))};

  return H + (5*log10(r*delta)) - (2.5*log10(((1 - G)*phi1) + (G*phi2)));
}
