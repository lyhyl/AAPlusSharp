/*
Module : AAMoonPhases.h
Purpose: Implementation for the algorithms which obtain the dates for the phases of the Moon
Created: PJN / 11-01-2004

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

#ifndef __AAMOONPHASES_H__
#define __AAMOONPHASES_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public ref class AAPLUS_EXT_CLASS CAAMoonPhases
{
public:
//Static methods
  static double K(double Year)
  {
    return 12.3685*(Year - 2000);
  }

#ifdef _MSC_VER
  #pragma warning(suppress : 26497)
#endif //#ifdef _MSC_VER
  static double MeanPhase(double k)
  {
    //convert from K to T
    double T{k/1236.85};
    double T2{T*T};
    double T3{T2*T};
    double T4{T3*T};

    return 2451550.09766 + (29.530588861*k) + (0.00015437*T2) - (0.000000150*T3) + (0.00000000073*T4);
  }

  static double TruePhase(double k);
};


}
#endif //#ifndef __AAMOONPHASES_H__
