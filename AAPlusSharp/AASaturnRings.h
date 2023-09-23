/*
Module : AASaturnRings.h
Purpose: Implementation for the algorithms which calculate various parameters related to the Rings of Saturn
Created: PJN / 08-01-2004

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

#ifndef __AASATURNRINGS_H__
#define __AASATURNRINGS_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public value class AAPLUS_EXT_CLASS CAASaturnRingDetails
{
public:
//Member variables
  double B;
  double Bdash;
  double P;
  double a;
  double b;
  double DeltaU;
  double U1;
  double U2;
};

public ref class AAPLUS_EXT_CLASS CAASaturnRings
{
public:
//Static methods
  static CAASaturnRingDetails Calculate(double JD, bool bHighPrecision);
};


}
#endif //#ifndef __AASATURNRINGS_H__
