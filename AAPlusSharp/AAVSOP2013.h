/*
Module : VSOP2013.h
Purpose: Implementation for the algorithms for VSOP2013
Created: PJN / 01-10-2021

Copyright (c) 2021 - 2023 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

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

#ifndef __AAVSOP2013_H__
#define __AAVSOP2013_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif //#ifndef AAPLUS_EXT_CLASS


//////////////////// Includes /////////////////////////////////////////////////

#include <array>

#include <cstdint>
#include <exception>
#include <filesystem>
#include "AAElliptical.h"


//////////////////// Classes //////////////////////////////////////////////////

//The exception class which can be thrown by CAAVSOP2013::Calculate or CAAVSOP2013::CalculateOrbit
class AAPLUS_EXT_CLASS CAAVSOP2013Exception : public std::exception
{
public:
//Enums
  enum class REASON
  {
    UNDEFINED = 0,
    FAILED_TO_OPEN_FILE = 1,
    PLANET_IS_INVALID = 2,
    DATE_IS_INVALID = 3,
    COULD_NOT_LOAD_BINARY_FILE = 4
  };

//Constructors / Destructors
  CAAVSOP2013Exception() = default;
  CAAVSOP2013Exception(REASON reason) : m_Reason(reason)
  {
  }
  CAAVSOP2013Exception(CAAVSOP2013Exception&) = default;
  CAAVSOP2013Exception(CAAVSOP2013Exception&&) = default;
  ~CAAVSOP2013Exception() override = default;

//Methods
  CAAVSOP2013Exception& operator=(CAAVSOP2013Exception&) = default;
  CAAVSOP2013Exception& operator=(CAAVSOP2013Exception&&) = default;

//Member variables
  REASON m_Reason{REASON::UNDEFINED}; //The reason for the exception
};

//Represents a single VSOP2013 ephemerides file
namespace AAPlusSharp
{
public ref class AAPLUS_EXT_CLASS CAAVSOP2013EphemeridesFile
{
public:
//Enums
  enum
  {
    SIZE_BASIC_INTERVAL = 32,
    IDENTIFICATION_INDEX = 2013,
    CHEBYSHEV_TABLES = 17122,
    COEFFICIENTS_PER_TABLE = 978
  };

//Methods
  bool ReadTextFile(std::filesystem::path::value_type* pszFilename);
  bool WriteBinaryFile(std::filesystem::path::value_type* pszFilename);
  bool ReadBinaryFile(std::filesystem::path::value_type* pszFilename);

  bool operator!=(CAAVSOP2013EphemeridesFile& file)
  {
    return !operator==(file);
  }

  bool operator==(CAAVSOP2013EphemeridesFile& file)
  {
    if (m_fStartJD != file.m_fStartJD)
      return false;
    if (m_fEndJD != file.m_fEndJD)
      return false;
    for (size_t i{0}; i<m_FirstCoefficientRank.size(); i++)
    {
#ifdef _MSC_VER
#pragma warning(suppress : 26446 26482)
#endif //#ifdef _MSC_VER
      if (m_FirstCoefficientRank[i] != file.m_FirstCoefficientRank[i])
        return false;
    }
    for (size_t i{0}; i<m_CoefficientsPerCoordinate.size(); i++)
    {
#ifdef _MSC_VER
#pragma warning(suppress : 26446 26482)
#endif //#ifdef _MSC_VER
      if (m_CoefficientsPerCoordinate[i] != file.m_CoefficientsPerCoordinate[i])
        return false;
    }
    for (size_t i{0}; i<m_SubIntervals.size(); i++)
    {
#ifdef _MSC_VER
#pragma warning(suppress : 26446 26482)
#endif //#ifdef _MSC_VER
      if (m_SubIntervals[i] != file.m_SubIntervals[i])
        return false;
    }
    if (m_ChebyshevTables.size() != file.m_ChebyshevTables.size())
      return false;
    for (size_t i{0}; i<file.m_ChebyshevTables.size(); i++)
    {
#ifdef _MSC_VER
#pragma warning(suppress : 26446)
#endif //#ifdef _MSC_VER
      if (m_ChebyshevTables[i] != file.m_ChebyshevTables[i])
        return false;
    }
    return true;
  }

//Member variables
  double m_fStartJD{0};
  double m_fEndJD{0};
  std::array<int32_t, 9> m_FirstCoefficientRank{};
  std::array<int32_t, 9> m_CoefficientsPerCoordinate{};
  std::array<int32_t, 9> m_SubIntervals{};
  std::vector<std::array<double, COEFFICIENTS_PER_TABLE + 2>> m_ChebyshevTables;
};

//Represents a single elements term
public value class AAPLUS_EXT_CLASS CAAVSOP2013ElementsTerm
{
public:
//Member variables
  std::array<int32_t, 17> m_iphi{};
  double m_fS;
  double m_fC;
};

//Represents a series of elements term records
public value struct AAPLUS_EXT_CLASS CAAVSOP2013ElementsSeries
{
//Member variables
  uint8_t m_ip; //planet index 1=Mercury...9=Pluto
  uint8_t m_iv; //variable index 1=a, 2=lambda, 3=k, 4=h, 5=q, 6=p
  uint8_t m_it; //time power
  System::Collections::Generic::List<CAAVSOP2013ElementsTerm>^ m_Terms = gcnew System::Collections::Generic::List<CAAVSOP2013ElementsTerm>(0);
};

//Represents a single VSOP2013 elements file
public ref class AAPLUS_EXT_CLASS CAAVSOP2013ElementsFile
{
public:
//Methods
  bool ReadTextFile(std::filesystem::path::value_type* pszFilename);
  bool WriteBinaryFile(std::filesystem::path::value_type* pszFilename);
  bool ReadBinaryFile(std::filesystem::path::value_type* pszFilename);

  bool operator!=(CAAVSOP2013ElementsFile& file)
  {
    return !operator==(file);
  }

  bool operator==(CAAVSOP2013ElementsFile& file)
  {
    if (m_AllSeries.size() != file.m_AllSeries.size())
      return false;
    for (size_t i{0}; i<file.m_AllSeries.size(); i++)
    {
#ifdef _MSC_VER
#pragma warning(suppress : 26446)
#endif //#ifdef _MSC_VER
      auto& series1{m_AllSeries[i]};
#ifdef _MSC_VER
#pragma warning(suppress : 26446)
#endif //#ifdef _MSC_VER
      auto& series2{file.m_AllSeries[i]};
      if (series1.m_ip != series2.m_ip)
        return false;
      if (series1.m_it != series2.m_it)
        return false;
      if (series1.m_iv != series2.m_iv)
        return false;
      if (series1.m_Terms.size() != series2.m_Terms.size())
        return false;
      for (size_t j{0}; j<series1.m_Terms.size(); j++)
      {
#ifdef _MSC_VER
#pragma warning(suppress : 26446)
#endif //#ifdef _MSC_VER
        auto& record1{series1.m_Terms[j]};
#ifdef _MSC_VER
#pragma warning(suppress : 26446)
#endif //#ifdef _MSC_VER
        auto& record2{series2.m_Terms[j]};
        if (record1.m_iphi != record2.m_iphi)
          return false;
        if (record1.m_fS != record2.m_fS)
          return false;
        if (record1.m_fC != record2.m_fC)
          return false;
      }
    }
    return true;
  }

//Member variables
  System::Collections::Generic::List<CAAVSOP2013ElementsSeries>^ m_AllSeries = gcnew System::Collections::Generic::List<CAAVSOP2013ElementsSeries>(0);
};

//Contains the results from CAAVSOP2013::Calculate
public value class AAPLUS_EXT_CLASS CAAVSOP2013Position
{
public:
//Member variables
  double X;
  double Y;
  double Z;
  double X_DASH;
  double Y_DASH;
  double Z_DASH;
};

//Contains the results from CAAVSOP2013::CalculateOrbit
public value class AAPLUS_EXT_CLASS CAAVSOP2013Orbit
{
public:
//Member variables
  double a;
  double lambda;
  double k;
  double h;
  double q;
  double p;
};

//The main class which implements the VSOP2013 algorithms
public ref class AAPLUS_EXT_CLASS CAAVSOP2013
{
public:
//Enums
  enum class Planet
  {
    MERCURY = 0,
    VENUS = 1,
    EARTH_MOON_BARYCENTER = 2,
    MARS = 3,
    JUPITER = 4,
    SATURN = 5,
    URANUS = 6,
    NEPTUNE = 7,
    PLUTO = 8
  };

//Methods
  void SetBinaryFilesDirectory(std::filesystem::path::value_type* pszBinaryFilesDirectory);
  [[nodiscard]] std::filesystem::path::value_type* GetBinaryFilesDirectory();
  CAAVSOP2013Position Calculate(Planet planet, double JD);
  CAAVSOP2013Orbit CalculateOrbit(Planet planet, double JD);
  static double CalculateMeanMotion(Planet planet, double a);
  static CAAEllipticalObjectElements OrbitToElements(double JD, Planet planet, CAAVSOP2013Orbit% orbit);
  static CAAVSOP2013Position Ecliptic2Equatorial(CAAVSOP2013Position% value);

protected:
//Methods
  void CalculateLambdas(double T, std::array<double, 17>% lambdas);

//Member variables
  std::array<double, 7> m_DateRange{ { 77294.5, 625198.5, 1173102.5, 1721006.5, 2268910.5, 2816814.5, 3364718.5 } };
#ifdef _WIN32
  std::array<std::filesystem::path::value_type*, 6> m_EphemeridesFilenames{ { L"VSOP2013.M4000.bin", L"VSOP2013.M2000.bin", L"VSOP2013.M1000.bin", L"VSOP2013.P1000.bin", L"VSOP2013.P2000.bin", L"VSOP2013.P4000.bin" } };
#else
  std::array<std::filesystem::path::value_type*, 6> m_EphemeridesFilenames{ { "VSOP2013.M4000.bin", "VSOP2013.M2000.bin", "VSOP2013.M1000.bin", "VSOP2013.P1000.bin", "VSOP2013.P2000.bin", "VSOP2013.P4000.bin" } };
#endif //#ifdef _WIN32
#ifdef _WIN32
  std::array<std::filesystem::path::value_type*, 9> m_ElementsFilenames{ { L"VSOP2013p1.bin", L"VSOP2013p2.bin", L"VSOP2013p3.bin", L"VSOP2013p4.bin", L"VSOP2013p5.bin", L"VSOP2013p6.bin", L"VSOP2013p7.bin", L"VSOP2013p8.bin", L"VSOP2013p9.bin" } };
#else
  std::array<std::filesystem::path::value_type*, 9> m_ElementsFilenames{ { "VSOP2013p1.bin", "VSOP2013p2.bin", "VSOP2013p3.bin", "VSOP2013p4.bin", "VSOP2013p5.bin", "VSOP2013p6.bin", "VSOP2013p7.bin", "VSOP2013p8.bin", "VSOP2013p9.bin" } };
#endif //#ifdef _WIN32
  std::array<CAAVSOP2013EphemeridesFile, 6> m_EphemerideFiles{};
  std::array<CAAVSOP2013ElementsFile, 9> m_ElementsFiles{};
#ifdef _WIN32
  std::filesystem::path::value_type* m_pszBinaryFilesDirectory = L".";
#else
  std::filesystem::path::value_type* m_pszBinaryFilesDirectory = ".";
#endif //#ifdef _WIN32
};


}
#endif //#ifndef __AAVSOP2013_H__
