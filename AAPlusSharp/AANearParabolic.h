/*
Module : AANearParabolic.h
Purpose: Implementation for the algorithms for a near parabolic orbit
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

#ifndef __AANEARPARABOLIC_H__
#define __AANEARPARABOLIC_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Includes /////////////////////////////////////////////////

#include "AA3DCoordinate.h"


//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public value class AAPLUS_EXT_CLASS CAANearParabolicObjectElements
{
public:
//Member variables
  double q;
  double i;
  double w;
  double omega;
  double JDEquinox;
  double T;
  double e;
};

public value class AAPLUS_EXT_CLASS CAANearParabolicObjectDetails
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

public ref class AAPLUS_EXT_CLASS CAANearParabolic
{
public:
//Static methods
  static CAANearParabolicObjectDetails Calculate(double JD, CAANearParabolicObjectElements% elements, bool bHighPrecision);
  static double cbrt(double x);
  static void CalculateTrueAnomalyAndRadius(double JD, CAANearParabolicObjectElements% elements, double% v, double% r);
};


}
#endif //#ifndef __AANEARPARABOLIC_H__
