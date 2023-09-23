/*
Module : AAEclipses.h
Purpose: Implementation for the algorithms which obtain the principal characteristics of an eclipse of the Sun or the Moon
Created: PJN / 21-01-2004

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

#ifndef __AAECLIPSES_H__
#define __AAECLIPSES_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public value class AAPLUS_EXT_CLASS CAASolarEclipseDetails
{
public:
//Constants (used by Flags member variable)
  static unsigned int TOTAL_ECLIPSE = 0x01;
  static unsigned int ANNULAR_ECLIPSE = 0x02;
  static unsigned int ANNULAR_TOTAL_ECLIPSE = 0x04;
  static unsigned int CENTRAL_ECLIPSE = 0x08;
  static unsigned int PARTIAL_ECLIPSE = 0x10;
  static unsigned int NON_CENTRAL_ECLIPSE = 0x20;

//Member variables
  unsigned int Flags;
  double TimeOfMaximumEclipse;
  double F;
  double u;
  double gamma;
  double GreatestMagnitude;
};


public value class AAPLUS_EXT_CLASS CAALunarEclipseDetails
{
public:
//Member variables
  bool bEclipse;
  double TimeOfMaximumEclipse;
  double F;
  double u;
  double gamma;
  double PenumbralRadii;
  double UmbralRadii;
  double PenumbralMagnitude;
  double UmbralMagnitude;
  double PartialPhaseSemiDuration;
  double TotalPhaseSemiDuration;
  double PartialPhasePenumbraSemiDuration;
};

public ref class AAPLUS_EXT_CLASS CAAEclipses
{
public:
//Static methods
  static CAASolarEclipseDetails CalculateSolar(double k);
  static CAALunarEclipseDetails CalculateLunar(double k);

protected:
  static CAASolarEclipseDetails Calculate(double k, double% Mdash);
};


}
#endif //#ifndef __AAECLIPSES_H__
