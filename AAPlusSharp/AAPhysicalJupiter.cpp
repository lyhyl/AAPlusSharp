/*
Module : AAPhysicalJupiter.cpp
Purpose: Implementation for the algorithms which obtain the physical parameters of Jupiter
Created: PJN / 29-12-2003
History: PJN / 16-09-2015 1. CAAPhysicalJupiter::Calculate now includes a "bool bHighPrecision" parameter
                          which if set to true means the code uses the full VSOP87 theory rather than the
                          truncated theory as presented in Meeus's book. 
         PJN / 18-08-2019 1. Fixed some further compiler warnings when using VC 2019 Preview v16.3.0 Preview 2.0
         PJN / 02-07-2022 1. Updated all the code in AAPhysicalJupiter.cpp to use C++ uniform initialization for
                          all variable declarations.

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
#include "AAPhysicalJupiter.h"
#include "AAJupiter.h"
#include "AAEarth.h"
#include "AANutation.h"
#include "AACoordinateTransformation.h"
#include <cmath>
using namespace AAPlusSharp;

//////////////////// Implementation ///////////////////////////////////////////

CAAPhysicalJupiterDetails CAAPhysicalJupiter::Calculate(double JD, bool bHighPrecision)
{
  //What will be the return value
  CAAPhysicalJupiterDetails details;

  //Step 1
  double d{JD - 2433282.5};
  double T1{d/36525};
  double alpha0{268.00 + (0.1061*T1)};
  double alpha0rad{CAACoordinateTransformation::DegreesToRadians(alpha0)};
  double delta0{64.50 - (0.0164*T1)};
  double delta0rad{CAACoordinateTransformation::DegreesToRadians(delta0)};
  double cosdelta0rad{cos(delta0rad)};
  double sindelta0rad{sin(delta0rad)};

  //Step 2
  double W1{CAACoordinateTransformation::MapTo0To360Range(17.710 + (877.90003539*d))};
  double W2{CAACoordinateTransformation::MapTo0To360Range(16.838 + (870.27003539*d))};

  //Step 3
  double l0{CAAEarth::EclipticLongitude(JD, bHighPrecision)};
  double l0rad{CAACoordinateTransformation::DegreesToRadians(l0)};
  double cosl0rad{cos(l0rad)};
  double sinl0rad{sin(l0rad)};
  double b0{CAAEarth::EclipticLatitude(JD, bHighPrecision)};
  double b0rad{CAACoordinateTransformation::DegreesToRadians(b0)};
  double sinb0rad{sin(b0rad)};
  double R{CAAEarth::RadiusVector(JD, bHighPrecision)};

  //Step 4
  double l{CAAJupiter::EclipticLongitude(JD, bHighPrecision)};
  double lrad{ CAACoordinateTransformation::DegreesToRadians(l)};
  double coslrad{cos(lrad)};
  double sinlrad{sin(lrad)};
  double b{CAAJupiter::EclipticLatitude(JD, bHighPrecision)};
  double brad{CAACoordinateTransformation::DegreesToRadians(b)};
  double cosbrad{cos(brad)};
  double sinbrad{sin(brad)};
  double r{CAAJupiter::RadiusVector(JD, bHighPrecision)};

  //Step 5
  double x{(r*cosbrad*coslrad) - (R*cosl0rad)};
  double x2{x*x};
  double y{(r*cosbrad*sinlrad) - (R*sinl0rad)};
  double y2{y*y};
  double z{(r*sinbrad) - (R*sinb0rad)};
  double z2{z*z};
  double DELTA{sqrt(x2 + y2 + z2)};

  //Step 6
  l -= 0.012990*DELTA/(r*r);
  lrad = CAACoordinateTransformation::DegreesToRadians(l);
  sinlrad = sin(lrad);
  coslrad = cos(lrad);

  //Step 7
  x = (r*cosbrad*coslrad) - (R*cosl0rad);
  y = (r*cosbrad*sinlrad) - (R*sinl0rad);
  z = (r*sinbrad) - (R*sinb0rad);
  DELTA = sqrt(x2 + y2 + (z2));

  //Step 8
  double e0{CAANutation::MeanObliquityOfEcliptic(JD)};
  double e0rad{CAACoordinateTransformation::DegreesToRadians(e0)};
  double cose0rad{cos(e0rad)};
  double sine0rad{sin(e0rad)};

  //Step 9
  double alphas{atan2((cose0rad*sinlrad) - (sine0rad*tan(brad)), coslrad)};
  double deltas{asin((cose0rad*sinbrad) + (sine0rad*cosbrad*sinlrad))};

  //Step 10
  details.DS = CAACoordinateTransformation::RadiansToDegrees(asin(-sindelta0rad*sin(deltas) - cosdelta0rad*cos(deltas)*cos(alpha0rad - alphas)));

  //Step 11
  double u{(y*cose0rad) - (z*sine0rad)};
  double v{(y*sine0rad) + (z*cose0rad)};
  double alpharad{atan2(u, x)};
  double alpha{CAACoordinateTransformation::RadiansToDegrees(alpharad)};
  double deltarad{atan2(v, sqrt((x*x) + (u*u)))};
  double sindeltarad{sin(deltarad)};
  double cosdeltarad{cos(deltarad)};
  double delta{CAACoordinateTransformation::RadiansToDegrees(deltarad)};
  double xi{atan2((sindelta0rad*cosdeltarad*cos(alpha0rad - alpharad)) - (sindeltarad*cosdelta0rad), cosdeltarad*sin(alpha0rad - alpharad))};

  //Step 12
  details.DE = CAACoordinateTransformation::RadiansToDegrees(asin((-sindelta0rad*sindeltarad) - (cosdelta0rad*cosdeltarad*cos(alpha0rad - alpharad))));

  //Step 13
  details.Geometricw1 = CAACoordinateTransformation::MapTo0To360Range(W1 - CAACoordinateTransformation::RadiansToDegrees(xi) - (5.07033*DELTA));
  details.Geometricw2 = CAACoordinateTransformation::MapTo0To360Range(W2 - CAACoordinateTransformation::RadiansToDegrees(xi) - (5.02626*DELTA));

  //Step 14
  double C{57.2958*((2*r*DELTA) + (R*R) - (r*r) - (DELTA*DELTA))/(4*r*DELTA)};
  if (sin(lrad - l0rad) > 0)
  {
    details.Apparentw1 = CAACoordinateTransformation::MapTo0To360Range(details.Geometricw1 + C);
    details.Apparentw2 = CAACoordinateTransformation::MapTo0To360Range(details.Geometricw2 + C);
  }
  else
  {
    details.Apparentw1 = CAACoordinateTransformation::MapTo0To360Range(details.Geometricw1 - C);
    details.Apparentw2 = CAACoordinateTransformation::MapTo0To360Range(details.Geometricw2 - C);
  }

  //Step 15
  double NutationInLongitude{CAANutation::NutationInLongitude(JD)};
  double NutationInObliquity{CAANutation::NutationInObliquity(JD)};
  e0 += NutationInObliquity/3600;
  e0rad = CAACoordinateTransformation::DegreesToRadians(e0);
  cose0rad = cos(e0rad);

  //Step 16
  double cosalpharad{cos(alpharad)};
  double sinalpharad{sin(alpharad)};
  alpha += 0.005693*((cosalpharad*cosl0rad*cose0rad) + (sinalpharad*sinl0rad))/cosdeltarad;
  alpha = CAACoordinateTransformation::MapTo0To360Range(alpha);
  delta += 0.005693*(cosl0rad*cose0rad*(tan(e0rad)*cosdeltarad - sinalpharad*sindeltarad) + cosalpharad*sindeltarad*sinl0rad);

  //Step 17
  double NutationRA{CAANutation::NutationInRightAscension(alpha/15, delta, e0, NutationInLongitude, NutationInObliquity)};
  double alphadash{alpha + (NutationRA/3600)};
  double alphadashrad{CAACoordinateTransformation::DegreesToRadians(alphadash)};
  double NutationDec{CAANutation::NutationInDeclination(alpha/15, e0, NutationInLongitude, NutationInObliquity)};
  double deltadash{delta + (NutationDec/3600)};
  double deltadashrad{CAACoordinateTransformation::DegreesToRadians(deltadash)};
  NutationRA = CAANutation::NutationInRightAscension(alpha0/15, delta0, e0, NutationInLongitude, NutationInObliquity);
  double alpha0dash{alpha0 + (NutationRA/3600)};
  double alpha0dashrad{CAACoordinateTransformation::DegreesToRadians(alpha0dash)};
  NutationDec = CAANutation::NutationInDeclination(alpha0 / 15, e0, NutationInLongitude, NutationInObliquity);
  double delta0dash{delta0 + (NutationDec/3600)};
  double delta0dashrad{CAACoordinateTransformation::DegreesToRadians(delta0dash)};
  double cosdelta0dashrad{cos(delta0dashrad)};

  //Step 18
  details.P = CAACoordinateTransformation::MapTo0To360Range(CAACoordinateTransformation::RadiansToDegrees(atan2(cosdelta0dashrad*sin(alpha0dashrad - alphadashrad), sin(delta0dashrad)*cos(deltadashrad) - cosdelta0dashrad*sin(deltadashrad)*cos(alpha0dashrad - alphadashrad))));

  return details;
}
