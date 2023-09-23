/*
Module : AAInterpolate.h
Purpose: Implementation for the algorithms for Interpolation
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

#ifndef __AAINTERPOLATE_H__
#define __AAINTERPOLATE_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public ref class AAPLUS_EXT_CLASS CAAInterpolate
{
public:
//Static methods

#ifdef _MSC_VER
  #pragma warning(suppress : 26497)
#endif //#ifdef _MSC_VER
  static double Interpolate(double n, double Y1, double Y2, double Y3)
  {
    double a{Y2 - Y1};
    double b{Y3 - Y2};
    double c{Y1 + Y3 - (2*Y2)};

    return Y2 + (n/2*(a + b + (n*c)));
  }

#ifdef _MSC_VER
  #pragma warning(suppress : 26497)
#endif //#ifdef _MSC_VER
  static double Interpolate(double n, double Y1, double Y2, double Y3, double Y4, double Y5)
  {
    double A{Y2 - Y1};
    double B{Y3 - Y2};
    double C{Y4 - Y3};
    double D{Y5 - Y4};
    double E{B - A};
    double F{C - B};
    double G{D - C};
    double H{F - E};
    double J{G - F};
    double K{J - H};
    double N2{n*n};
    double N3{N2*n};
    double N4{N3*n};

    return Y3 + (n*(((B + C)/2) - ((H + J)/12))) + (N2*((F/2) - (K/24))) + (N3*((H + J)/12)) + (N4*(K/24));
  }

  static double InterpolateToHalves(double Y1, double Y2, double Y3, double Y4)
  {
    return ((9*(Y2 + Y3)) - Y1 - Y4)/16;
  }

  static double LagrangeInterpolate(double X, int n, double* pX, double* pY);
  static double Extremum(double Y1, double Y2, double Y3, double% nm);
  static double Extremum(double Y1, double Y2, double Y3, double Y4, double Y5, double% nm, double epsilon);
  static double Extremum(double Y1, double Y2, double Y3, double Y4, double Y5, double% nm) { return Extremum(Y1, Y2, Y3, Y4, Y5, nm, 1e-12); }

  static double Zero(double Y1, double Y2, double Y3, double epsilon);
  static double Zero(double Y1, double Y2, double Y3) { return Zero(Y1, Y2, Y3, 1e-12); }

  static double Zero(double Y1, double Y2, double Y3, double Y4, double Y5, double epsilon);
  static double Zero(double Y1, double Y2, double Y3, double Y4, double Y5) { return Zero(Y1, Y2, Y3, Y4, Y5, 1e-12); }

  static double Zero2(double Y1, double Y2, double Y3, double epsilon);
  static double Zero2(double Y1, double Y2, double Y3) { return Zero2(Y1, Y2, Y3, 1e-12); }

  static double Zero2(double Y1, double Y2, double Y3, double Y4, double Y5, double epsilon);
  static double Zero2(double Y1, double Y2, double Y3, double Y4, double Y5) { return Zero2(Y1, Y2, Y3, Y4, Y5, 1e-12); }

};


}
#endif //#ifndef __AAINTERPOLATE_H__
