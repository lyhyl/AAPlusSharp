/*
Module : AAMoonPerigeeApogee.h
Purpose: Implementation for the algorithms which obtain the dates of Lunar Apogee and Perigee
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

#ifndef __AAMOONPERIGEEAPOGEE_H__
#define __AAMOONPERIGEEAPOGEE_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public ref class AAPLUS_EXT_CLASS CAAMoonPerigeeApogee
{
public:
//Static methods
  static double K(double Year)
  {
    return 13.2555*(Year - 1999.97);
  }

#ifdef _MSC_VER
  #pragma warning(suppress : 26497)
#endif //#ifdef _MSC_VER
  static double MeanPerigee(double k)
  {
    //convert from K to T
    double T{k/1325.55};
    double Tsquared{T*T};
    double Tcubed{Tsquared*T};
    double T4{Tcubed*T};

    return 2451534.6698 + (27.55454989*k) - (0.0006691*Tsquared) - (0.000001098*Tcubed) + (0.0000000052*T4);
  }

  static double MeanApogee(double k)
  {
    //Uses the same formula as MeanPerigee
    return MeanPerigee(k);
  }

  static double TruePerigee(double k);
  static double TrueApogee(double k);
  static double PerigeeParallax(double k);
  static double ApogeeParallax(double k);
};


}
#endif //#ifndef __AAMOONPERIGEEAPOGEE_H__
