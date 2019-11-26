/// \file Random.h
/// \brief Header for the pseudo-random number generator CRandom.

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

#if !defined(__Random__)
#define __Random__

/// \brief Pseudorandom number generator (PRNG for short).
///
/// A pseudorandom number generator based on xorshift128. It seeds itself
/// with the C standard rand() function, which it assumes has been seeded
/// with something that is different each time that the program is run
/// such as the Windows API function timeGetTime().

class CRandom{
  private: 
    unsigned m_uState[4] = {0}; ///< Current state.

  public:
    CRandom(); ///< Constructor.
    
    void srand(); ///< Seed the random number generator.

    unsigned randn(); ///< Get a random unsigned integer.
    unsigned randn(unsigned i, unsigned j); ///< Get random number in \f$[i,j]\f$.
    float randf(); ///< Get a random floating point number.
}; //CRandom

#endif
