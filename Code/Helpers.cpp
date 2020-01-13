/// \file Helpers.cpp
/// \brief Code for helper functions..

// MIT License
//
// Copyright (c) 2019 Ian Parberry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "Helpers.h"

#include "Graph.h"
#include "Random.h"
#include "Defines.h"
#include "Includes.h"
#include "Board.h"

#if !defined(_MSC_VER)
  /// Implementation of fopen_s when we're not under Windows.
  /// \param stream Output stream.
  /// \param name File name.
  /// \param fmt File format.

  void fopen_s(FILE** stream, const char* name, const char* fmt){
    *stream = fopen(name, fmt);
  } //fopen_s

  #include <sys/time.h>

  /// Well, it kinda-sorta is like the Windows version of timeGetTime(), but
  /// not really. Since it will be used to seed a PRNG, all that really matters
  /// is that it returns something different each time is is called. It can be
  /// slow but hopefully we'll only used it a few times during initialization.
  /// \return Number of milliseconds since the start of the current epoch.

  UINT timeGetTime(){
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec/1000;
  } //timeGetTime
#endif

/// \brief Move deltas for all possible knight's moves.
///
/// Move deltas (horizontal and vertical displacements) for the 8 knight's
/// moves indexed counterclockwise from \f$(2, -1)\f$ (the origin is at top 
/// left of the board, positive \f$x\f$ is rightwards, and positive \f$y\f$
/// is downwards.
///
/// \image html movedeltas.png

MoveDeltas g_vecDeltas = {
  MoveDelta( 2, -1), //move 0
  MoveDelta( 1, -2), //move 1
  MoveDelta(-1, -2), //move 2
  MoveDelta(-2, -1), //move 3
  MoveDelta(-2,  1), //move 4
  MoveDelta(-1,  2), //move 5
  MoveDelta( 1,  2), //move 6
  MoveDelta( 2,  1)  //move 7
}; //g_vecDeltas

/// Make the base of a file name (without the extension) based on a tourney.
/// dwscription. If the generator type is GeneratorType::Unknown, then the
/// file name base returned will be "Unknown". If the generator type or cycle
/// type is not one of the ones hard-coded in the switch statements in this
/// function, then the file name base returned will be "Error".
/// \param t Tourney descriptor.
/// \param w Board width (defaults to -1, which means "none".
/// \return String that describes the tourney.

std::string MakeFileNameBase(const CTourneyDesc& t, int w){
  const GeneratorType gentype = t.m_eGenerator;
  const CycleType cycletype = t.m_eCycle;

  std::string s = "Error"; //file name string, should never see this default

  switch(gentype){ //prefix represents the generator type
    case GeneratorType::Warnsdorff:       s = "Warnsd"; break;
    case GeneratorType::TakefujiLee:      s = "Neural"; break;
    case GeneratorType::DivideAndConquer: s = "Divide"; break;
    case GeneratorType::ConcentricBraid:  s = "Braid";  break;
    case GeneratorType::FourCover:        s = "Cover4"; break;
    case GeneratorType::Unknown:          s = "Unknown"; break;
  } //switch
  
  switch(cycletype){ //suffix represents the cycle type
    case CycleType::Tour:            s += "Tour"; break;
    case CycleType::TourFromTourney: s += "Join"; break;
    case CycleType::Tourney:         s += "Trny"; break;
  } //switch

  if(t.m_bBlur)s += "Blur"; //blurred
  if(w > 0)s += std::to_string(w); //board width and height

  return s;
} //MakeFileNameBase

/// Make a compact string representation of a floating point number, that is,
/// if it's close enough to an integer, then don't use decimal point notation,
/// otherwise use 1 digit after the decimal point. This saves space when
/// rendering massive knight's tours to an SVG file. The floating point number
/// is assumed to be positive, otherwise the null string is returned.
/// \param x The number to be processed.
/// \return Compact string form of x.

std::string NumString(float x){
  std::string s; //return string
  if(x < 0)return s;

  const UINT intpart =  (UINT)std::floor(x);
  const float fracpart = x - intpart;

  if(fracpart < 0.1f)
    s = std::to_string(intpart);

  else{
    const UINT intfracpart = (UINT)std::round(fracpart*10.0f);
    s = std::to_string(intpart) + "." + std::to_string(intfracpart);
  } //else

  return s;
} //NumString

/// Convert color in HSV format to RGB format. This is a helper function for
/// generating a pseudorandom color. All parameters are floating point
/// values in \f$[0,1]\f$.
/// \param h [in] Hue.
/// \param s [in] Saturation.
/// \param v [in] Value.
/// \param rgb [out] Array of three RGB values.

void HSVtoRGB(float h, float s, float v, float rgb[3]){
  const UINT i = UINT(6.0f*h);

  const float f = 6.0f*h - i;
  const float p = v*(1.0f - s);
  const float q = v*(1.0f - s*f);
  const float t = v*(1.0f - s*(1.0f - f));

  //set red, green, and blue channels in different ways depending on h

  switch(i%6){ 
    case 0: rgb[0] = v; rgb[1] = t; rgb[2] = p; break;
    case 1: rgb[0] = q; rgb[1] = v; rgb[2] = p; break;
    case 2: rgb[0] = p; rgb[1] = v; rgb[2] = t; break;
    case 3: rgb[0] = p; rgb[1] = q; rgb[2] = v; break;
    case 4: rgb[0] = t; rgb[1] = p; rgb[2] = v; break;
    case 5: rgb[0] = v; rgb[1] = p; rgb[2] = q; break;
  } //switch
} //HSVtoRGB