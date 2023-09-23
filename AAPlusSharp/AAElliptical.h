/*
Module : AAElliptical.h
Purpose: Implementation for the algorithms for an elliptical orbit
Created: PJN / 29-12-2003

Copyright (c) 2003 - 2023 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


//////////////////// Macros / Defines /////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif //#if _MSC_VER > 1000

#ifndef __AAELLIPTICAL_H__
#define __AAELLIPTICAL_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Includes /////////////////////////////////////////////////

#include "AA3DCoordinate.h"


//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public value class AAPLUS_EXT_CLASS CAAEllipticalObjectElements
{
public:
//Member variables
  double a;
  double e;
  double i;
  double w;
  double omega;
  double JDEquinox;
  double T;
};

public value class AAPLUS_EXT_CLASS CAAEllipticalPlanetaryDetails
{
public:
//Member variables
  double ApparentGeocentricEclipticalLongitude;
  double ApparentGeocentricEclipticalLatitude;
  double ApparentGeocentricDistance;
  double ApparentLightTime;
  double ApparentGeocentricRA;
  double ApparentGeocentricDeclination;
  CAA3DCoordinate TrueGeocentricRectangularEcliptical;
  double TrueHeliocentricEclipticalLongitude;
  double TrueHeliocentricEclipticalLatitude;
  double TrueHeliocentricDistance;
  double TrueGeocentricEclipticalLongitude;
  double TrueGeocentricEclipticalLatitude;
  double TrueGeocentricDistance;
  double TrueLightTime;
  double TrueGeocentricRA;
  double TrueGeocentricDeclination;
};

public value class AAPLUS_EXT_CLASS CAAEllipticalObjectDetails
{
public:
//Member variables
  CAA3DCoordinate HeliocentricRectangularEquatorial;
  CAA3DCoordinate HeliocentricRectangularEcliptical;
  double HeliocentricEclipticLongitude;
  double HeliocentricEclipticLatitude;
  double TrueGeocentricRA;
  double TrueGeocentricDeclination;
  double TrueGeocentricDistance;
  double TrueGeocentricLightTime;
  double AstrometricGeocentricRA;
  double AstrometricGeocentricDeclination;
  double AstrometricGeocentricDistance;
  double AstrometricGeocentricLightTime;
  double Elongation;
  double PhaseAngle;
};

public ref class AAPLUS_EXT_CLASS CAAElliptical
{
public:
//Enums
  enum class Object
  {
    SUN,
    MERCURY,
    VENUS,
    MARS,
    JUPITER,
    SATURN,
    URANUS,
    NEPTUNE
  };

//Static methods

  static double DistanceToLightTime(double Distance)
  {
    return Distance*0.0057755183;
  }

  static CAAEllipticalPlanetaryDetails Calculate(double JD, Object object, bool bHighPrecision);

  static double SemiMajorAxisFromPerihelionDistance(double q, double e)
  {
    return q/(1 - e);
  }

  static double MeanMotionFromSemiMajorAxis(double a);
  static CAAEllipticalObjectDetails Calculate(double JD, CAAEllipticalObjectElements% elements, bool bHighPrecision);
  static double InstantaneousVelocity(double r, double a);
  static double VelocityAtPerihelion(double e, double a);
  static double VelocityAtAphelion(double e, double a);
  static double LengthOfEllipse(double e, double a);
  static double CometMagnitude(double g, double delta, double k, double r);
  static double MinorPlanetMagnitude(double H, double delta, double G, double r, double PhaseAngle);
};


}
#endif //#ifndef __AAELLIPTICAL_H__
