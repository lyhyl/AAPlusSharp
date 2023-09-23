/*
Module : AARiseTransitSet2.h
Purpose: Implementation for the algorithms which obtain the Rise, Transit and Set times (revised version)
Created: PJN / 29-12-2003

Copyright (c) 2019 - 2023 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

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

#ifndef __AARISETRANSITSET2_H__
#define __AARISETRANSITSET2_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Includes /////////////////////////////////////////////////

#include "AA2DCoordinate.h"



//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public value class AAPLUS_EXT_CLASS CAARiseTransitSetDetails2
{
public:
//Enums
  enum class Type
  {
    NotDefined = 0,
    Rise = 1,
    Set = 2,
    SouthernTransit = 3,
    NorthernTransit = 4,
    CivilDusk = 5,
    NauticalDusk = 6,
    AstronomicalDusk = 7,
    AstronomicalDawn = 8,
    NauticalDawn = 9,
    CivilDawn = 10
  };

//Member variables
  Type type; //The type of the event which has occurred
  double JD; //When the event occurred in TT
  double Bearing; //Applicable for rise or sets only, this will be the bearing (degrees west of south) of the event
  double GeometricAltitude; //For transits only, this will contain the geometric altitude in degrees of the center of the object not including correction for refraction
  bool bAboveHorizon; //For transits only, this will be true if the transit is visible
};

public ref class AAPLUS_EXT_CLASS CAARiseTransitSet2
{
public:
//Enums
  enum class Object
  {
    SUN,
    MOON,
    MERCURY,
    VENUS,
    MARS,
    JUPITER,
    SATURN,
    URANUS,
    NEPTUNE,
    STAR
  };

  enum class MoonAlgorithm
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
  static System::Collections::Generic::List<CAARiseTransitSetDetails2>^ Calculate(double StartJD, double EndJD, Object object, double Longitude, double Latitude, double h0, double StepInterval, bool bHighPrecision);
  static System::Collections::Generic::List<CAARiseTransitSetDetails2>^ Calculate(double StartJD, double EndJD, Object object, double Longitude, double Latitude, double h0) { return Calculate(StartJD, EndJD, object, Longitude, Latitude, h0, 0.007, false); }
  static System::Collections::Generic::List<CAARiseTransitSetDetails2>^ Calculate(double StartJD, double EndJD, Object object, double Longitude, double Latitude, double h0, double StepInterval) { return Calculate(StartJD, EndJD, object, Longitude, Latitude, h0, StepInterval, false); }

  static System::Collections::Generic::List<CAARiseTransitSetDetails2>^ CalculateMoon(double StartJD, double EndJD, double Longitude, double Latitude, double RefractionAtHorizon, double StepInterval, MoonAlgorithm algorithm);
  static System::Collections::Generic::List<CAARiseTransitSetDetails2>^ CalculateMoon(double StartJD, double EndJD, double Longitude, double Latitude) { return CalculateMoon(StartJD, EndJD, Longitude, Latitude, -0.5667, 0.007, MoonAlgorithm::MeeusTruncated); }
  static System::Collections::Generic::List<CAARiseTransitSetDetails2>^ CalculateMoon(double StartJD, double EndJD, double Longitude, double Latitude, double RefractionAtHorizon) { return CalculateMoon(StartJD, EndJD, Longitude, Latitude, RefractionAtHorizon, 0.007, MoonAlgorithm::MeeusTruncated); }
  static System::Collections::Generic::List<CAARiseTransitSetDetails2>^ CalculateMoon(double StartJD, double EndJD, double Longitude, double Latitude, double RefractionAtHorizon, double StepInterval) { return CalculateMoon(StartJD, EndJD, Longitude, Latitude, RefractionAtHorizon, StepInterval, MoonAlgorithm::MeeusTruncated); }

  static System::Collections::Generic::List<CAARiseTransitSetDetails2>^ CalculateStationary(double StartJD, double EndJD, double Alpha, double Delta, double Longitude, double Latitude, double h0, double StepInterval);
  static System::Collections::Generic::List<CAARiseTransitSetDetails2>^ CalculateStationary(double StartJD, double EndJD, double Alpha, double Delta, double Longitude, double Latitude) { return CalculateStationary(StartJD, EndJD, Alpha, Delta, Longitude, Latitude, -0.5667, 0.007); }
  static System::Collections::Generic::List<CAARiseTransitSetDetails2>^ CalculateStationary(double StartJD, double EndJD, double Alpha, double Delta, double Longitude, double Latitude, double h0) { return CalculateStationary(StartJD, EndJD, Alpha, Delta, Longitude, Latitude, h0, 0.007); }


protected:
  static void AddEvents(System::Collections::Generic::List<CAARiseTransitSetDetails2>^% events, double LastAltitudeForDetectingRiseSet, double AltitudeForDetectingRiseSet,
                        double LastAltitudeForInterpolation, double h0, CAA2DCoordinate% Horizontal, double LastJD, double StepInterval, double LastBearing,
                        Object object, double LastAltitudeForDetectingTwilight, double AltitudeForTwilight);
};


}
#endif //#ifndef __AARISETRANSITSET2_H__
