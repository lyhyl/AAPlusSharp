/*
Module : AAELPMPP02.h
Purpose: Implementation for the algorithms for ELP/MPP02
Created: PJN / 30-07-2017

Copyright (c) 2017 - 2023 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

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
#endif

#ifndef __AAELPMPP02_H__
#define __AAELPMPP02_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif


//////////////////// Includes /////////////////////////////////////////////////

#include "AA2DCoordinate.h"
#include "AA3DCoordinate.h"


//////////////////// Classes //////////////////////////////////////////////////

namespace AAPlusSharp
{
public ref class AAPLUS_EXT_CLASS CAAELPMPP02
{
public:
//Enums
  enum class Correction
  {
    Nominal = 0,
    LLR,
    DE405,
    DE406
  };

//Static methods
  static double EclipticLongitude(double JD, Correction correction, double* pDerivative);
  static double EclipticLongitude(double JD) { return EclipticLongitude(JD, Correction::LLR, nullptr); }
  static double EclipticLongitude(double JD, Correction correction) { return EclipticLongitude(JD, correction, nullptr); }

  static double EclipticLongitude(double* pT, int nTSize, Correction correction, double* pDerivative);
  static double EclipticLongitude(double* pT, int nTSize) { return EclipticLongitude(pT, nTSize, Correction::LLR, nullptr); }
  static double EclipticLongitude(double* pT, int nTSize, Correction correction) { return EclipticLongitude(pT, nTSize, correction, nullptr); }

  static double EclipticLatitude(double JD, Correction correction, double* pDerivative);
  static double EclipticLatitude(double JD) { return EclipticLatitude(JD, Correction::LLR, nullptr); }
  static double EclipticLatitude(double JD, Correction correction) { return EclipticLatitude(JD, correction, nullptr); }

  static double EclipticLatitude(double* pT, int nTSize, Correction correction, double* pDerivative);
  static double EclipticLatitude(double* pT, int nTSize) { return EclipticLatitude(pT, nTSize, Correction::LLR, nullptr); }
  static double EclipticLatitude(double* pT, int nTSize, Correction correction) { return EclipticLatitude(pT, nTSize, correction, nullptr); }

  static double RadiusVector(double JD, Correction correction, double* pDerivative);
  static double RadiusVector(double JD) { return RadiusVector(JD, Correction::LLR, nullptr); }
  static double RadiusVector(double JD, Correction correction) { return RadiusVector(JD, correction, nullptr); }

  static double RadiusVector(double* pT, int nTSize, Correction correction, double* pDerivative);
  static double RadiusVector(double* pT, int nTSize) { return RadiusVector(pT, nTSize, Correction::LLR, nullptr); }
  static double RadiusVector(double* pT, int nTSize, Correction correction) { return RadiusVector(pT, nTSize, correction, nullptr); }

  static CAA3DCoordinate EclipticRectangularCoordinates(double JD, Correction correction, CAA3DCoordinate^ pDerivative);
  static CAA3DCoordinate EclipticRectangularCoordinates(double JD) { return EclipticRectangularCoordinates(JD, Correction::LLR, nullptr); }
  static CAA3DCoordinate EclipticRectangularCoordinates(double JD, Correction correction) { return EclipticRectangularCoordinates(JD, correction, nullptr); }

  static CAA3DCoordinate EclipticRectangularCoordinates(double* pT, int nTSize, Correction correction, CAA3DCoordinate^ pDerivative);
  static CAA3DCoordinate EclipticRectangularCoordinates(double* pT, int nTSize) { return EclipticRectangularCoordinates(pT, nTSize, Correction::LLR, nullptr); }
  static CAA3DCoordinate EclipticRectangularCoordinates(double* pT, int nTSize, Correction correction) { return EclipticRectangularCoordinates(pT, nTSize, correction, nullptr); }

  static CAA3DCoordinate EclipticRectangularCoordinatesJ2000(double JD, Correction correction, CAA3DCoordinate^ pDerivative);
  static CAA3DCoordinate EclipticRectangularCoordinatesJ2000(double JD) { return EclipticRectangularCoordinatesJ2000(JD, Correction::LLR, nullptr); }
  static CAA3DCoordinate EclipticRectangularCoordinatesJ2000(double JD, Correction correction) { return EclipticRectangularCoordinatesJ2000(JD, correction, nullptr); }

  static CAA3DCoordinate EclipticRectangularCoordinatesJ2000(double* pT, int nTSize, Correction correction, CAA3DCoordinate^ pDerivative);
  static CAA3DCoordinate EclipticRectangularCoordinatesJ2000(double* pT, int nTSize) { return EclipticRectangularCoordinatesJ2000(pT, nTSize, Correction::LLR, nullptr); }
  static CAA3DCoordinate EclipticRectangularCoordinatesJ2000(double* pT, int nTSize, Correction correction) { return EclipticRectangularCoordinatesJ2000(pT, nTSize, correction, nullptr); }

};


}
#endif //#ifndef __AAELPMPP02_H__
