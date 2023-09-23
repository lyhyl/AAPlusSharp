/*
Module : AABinaryStar.cpp
Purpose: Implementation for the algorithms for a binary star system
Created: PJN / 29-12-2003
History: PJN / 18-08-2019 1. Fixed some further compiler warnings when using VC 2019 Preview v16.3.0 Preview 2.0
         PJN / 22-11-2021 1. Reworked the CAABinaryStar::Calculate method to use the method as detailed in 
                          chapter 7 in the book "Observing and Measuring Visual Double Stars". This helps fix
                          issues detected with the algorithms as presented in Chapter 57 of Meeus's book. These
                          issues were detected when developing a program to provide a simple animation of a 
                          sample binary star system. The observed bug was causing the orbits to not be calculated
                          as projected ellipses when the inclination value (i) approached 90 degrees.
                          2. Updated CAABinaryStarDetails class to include rectangular coordinates as well as 
                          the existing polar coordinates.
         PJN / 12-06-2022 1. Updated all the code in AABinaryStar.cpp to use C++ uniform initialization for all 
                          variable declarations.

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
#include "AABinaryStar.h"
#include "AAKepler.h"
#include "AACoordinateTransformation.h"
#include <cmath>
using namespace AAPlusSharp;

//////////////////// Implementation ///////////////////////////////////////////

CAABinaryStarDetails CAABinaryStar::Calculate(double t, double P, double T, double e, double a, double i, double omega, double w)
{
  double n{360 / P};
  double M{CAACoordinateTransformation::MapTo0To360Range(n*(t - T))};
  double E{CAACoordinateTransformation::DegreesToRadians(CAAKepler::Calculate(M, e))};
  i = CAACoordinateTransformation::DegreesToRadians(i);
  w = CAACoordinateTransformation::DegreesToRadians(w);
  omega = CAACoordinateTransformation::DegreesToRadians(omega);
  double cosi{cos(i)};
  double cosomega{cos(omega)};
  double sinomega{sin(omega)};
  double cosw{cos(w)};
  double sinw{sin(w)};
  //Use the Thiele-Innes elements for calculating the rectangular as well as polar coordinates
  //as taken from Chapter 7 of the book "Observing and Measuring Visual Double Stars"
  double A{a*((cosw*cosomega) - (sinw*sinomega*cosi))};
  double B{a*((cosw*sinomega) + (sinw*cosomega*cosi))};
  double F{a*((-sinw*cosomega) - (cosw*sinomega*cosi))};
  double G{a*((-sinw*sinomega) + (cosw*cosomega*cosi))};
  double cosE{cos(E)};
  double X{cosE - e};
  double Y{sqrt(1 - (e*e))*sin(E)};
  CAABinaryStarDetails details;
  details.x = A*X + F*Y;
  details.y = B*X + G*Y;
  details.r = a*(1 - e* cosE);
  details.Theta = CAACoordinateTransformation::MapTo0To360Range(CAACoordinateTransformation::RadiansToDegrees(atan2(details.y, details.x)));
  details.Rho = sqrt((details.x*details.x) + (details.y*details.y));
  return details;
}

double CAABinaryStar::ApparentEccentricity(double e, double i, double w)
{
  i = CAACoordinateTransformation::DegreesToRadians(i);
  w = CAACoordinateTransformation::DegreesToRadians(w);

  double cosi{cos(i)};
  double cosw{cos(w)};
  double sinw{sin(w)};
  double esquared{e*e};
  double A{(1 - (esquared*cosw*cosw))*cosi*cosi};
  double B{esquared*sinw*cosw*cosi};
  double C{1 - (esquared*sinw*sinw)};
  double D{(A-C)*(A-C) + (4*B*B)};

  double sqrtD{sqrt(D)};
  return sqrt((2*sqrtD)/(A+C+sqrtD));
}
