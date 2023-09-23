/*
Module : AAMoonPhases2.h
Purpose: Implementation for the algorithms which obtain the dates for the phases of the Moon (revised version)
Created: PJN / 01-01-2020

Copyright (c) 2020 - 2023 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

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

#ifndef __AAMOONPHASES2_H__
#define __AAMOONPHASES2_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Includes /////////////////////////////////////////////////




//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public value class AAPLUS_EXT_CLASS CAAMoonPhasesDetails2
{
public:
//Enums
  enum class Type
  {
    NotDefined = 0,
    NewMoon = 1,
    FirstQuarter = 2,
    FullMoon = 3,
    LastQuarter = 4
  };

//Member variables
  Type type; //The type of the event which has occurred
  double JD; //When the event occurred in TT
};

public ref class AAPLUS_EXT_CLASS CAAMoonPhases2
{
public:
//Enums
  enum class Algorithm
  {
    MeeusTruncated = 0
#ifndef AAPLUS_NO_ELP2000
    ,
    ELP2000 = 1
#endif //#ifndef AAPLUS_NO_ELP2000
#ifndef AAPLUS_NO_ELPMPP02
    ,
    ELPMPP02Nominal = 2,
    ELPMPP02LLR = 3,
    ELPMPP02DE405 = 4,
    ELPMPP02DE406 = 5
#endif //#ifndef AAPLUS_NO_ELPMPP02
  };

//Static methods
  static System::Collections::Generic::List<CAAMoonPhasesDetails2>^ Calculate(double StartJD, double EndJD, double StepInterval, Algorithm algorithm);
  static System::Collections::Generic::List<CAAMoonPhasesDetails2>^ Calculate(double StartJD, double EndJD) { return Calculate(StartJD, EndJD, 0.007, Algorithm::MeeusTruncated); }
  static System::Collections::Generic::List<CAAMoonPhasesDetails2>^ Calculate(double StartJD, double EndJD, double StepInterval) { return Calculate(StartJD, EndJD, StepInterval, Algorithm::MeeusTruncated); }

};


}
#endif //#ifndef __AAMOONNODES2_H__