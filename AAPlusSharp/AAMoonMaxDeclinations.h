/*
Module : AAMoonMaxDeclinations.h
Purpose: Implementation for the algorithms which obtain the dates and values for maximum declination of the Moon
Created: PJN / 13-01-2004

Copyright (c) 2004 - 2023 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

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

#ifndef __AAMOONMAXDECLINATIONS_H__
#define __AAMOONMAXDECLINATIONS_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public ref class AAPLUS_EXT_CLASS CAAMoonMaxDeclinations
{
public:
//Static methods
  static double K(double Year)
  {
    return 13.3686*(Year - 2000.03);
  }

#ifdef _MSC_VER
  #pragma warning(suppress : 26497)
#endif //#ifdef _MSC_VER
  static double MeanGreatestDeclination(double k, bool bNortherly)
  {
    //convert from K to T
    double T{k/1336.86};
    double T2{T*T};
    double T3{T2*T};

    double value{bNortherly ? 2451562.5897 : 2451548.9289};
    return value + (27.321582247*k) + (0.000119804*T2) - (0.000000141*T3);
  }

#ifdef _MSC_VER
  #pragma warning(suppress : 26497)
#endif //#ifdef _MSC_VER
  static double MeanGreatestDeclinationValue(double k)
  {
    //convert from K to T
    double T{k/1336.86};
    return 23.6961 - (0.013004*T);
  }

  static double TrueGreatestDeclination(double k, bool bNortherly);
  static double TrueGreatestDeclinationValue(double k, bool bNortherly);
};


}
#endif //#ifndef __AAMOONMAXDECLINATIONS_H__
