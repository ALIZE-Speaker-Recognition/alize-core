/*
Alize is a free, open tool for speaker recognition

Alize is a development project initiated by the ELISA consortium
  [www.lia.univ-avignon.fr/heberges/ALIZE/ELISA] and funded by the
  French Research Ministry in the framework of the
  TECHNOLANGUE program [www.technolangue.net]
  [www.technolangue.net]

The Alize project team wants to highlight the limits of voice 
  authentication in a forensic context.
  The following paper proposes a good overview of this point:
  [Bonastre J.F., Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., 
  Magrin-chagnolleau I., Person  Authentification by Voice: A Need of 
  Caution, Eurospeech 2003, Genova]
  The conclusion of the paper of the paper is proposed bellow:
  [Currently, it is not possible to completely determine whether the 
  similarity between two recordings is due to the speaker or to other 
  factors, especially when: (a) the speaker does not cooperate, (b) there 
  is no control over recording equipment, (c) recording conditions are not 
  known, (d) one does not know whether the voice was disguised and, to a 
  lesser extent, (e) the linguistic content of the message is not 
  controlled. Caution and judgment must be exercised when applying speaker 
  recognition techniques, whether human or automatic, to account for these 
  uncontrolled factors. Under more constrained or calibrated situations, 
  or as an aid for investigative purposes, judicious application of these 
  techniques may be suitable, provided they are not considered as infallible.
  At the present time, there is no scientific process that enables one to 
  uniquely characterize a person=92s voice or to identify with absolute 
  certainty an individual from his or her voice.]
  Contact Jean-Francois Bonastre for more information about the licence or
  the use of Alize

Copyright (C) 2003-2005
  Laboratoire d'informatique d'Avignon [www.lia.univ-avignon.fr]
  Frederic Wils [frederic.wils@lia.univ-avignon.fr]
  Jean-Francois Bonastre [jean-francois.bonastre@lia.univ-avignon.fr]
      
This file is part of Alize.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if !defined(ALIZE_TestFrameAccGD_cpp)
#define ALIZE_TestFrameAccGD_cpp

#include "TestFrameAccGD.h"
#include "FrameAccGD.h"
#include "Feature.h"
#include "Exception.h"
#include <iostream>
#include <cmath>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestFrameAccGD::TestFrameAccGD()
:Test() {}
//-------------------------------------------------------------------------
bool TestFrameAccGD::exec()
{
  Test::exec();
  {
    // test method : String getClassName() const
    {
      test(FrameAccGD().getClassName() == "FrameAccGD", __LINE__);
    }
    // test constructor
    {
      FrameAccGD a;
      test(a._accVect.size() == 0.0, __LINE__);
      test(a._xaccVect.size() == 0.0, __LINE__);
      test(a._stdVect.size() == 0.0, __LINE__);
      test(a._covVect.size() == 0.0, __LINE__);
      test(a._count == 0, __LINE__);
      test(a._vectSizeDefined == false, __LINE__);
      test(a._computed == false, __LINE__);
    }
    // test copy constructor
    {
      FrameAccGD a;
      a._vectSize = 3;
      a._vectSizeDefined = true;
      a._count = 5;
      a._computed = true;
      a._stdComputed = true;
      a._accVect.setSize(a._vectSize);
      a._accVect[0] = 1.0;
      a._accVect[1] = 2.0;
      a._accVect[2] = 3.0;
      a._meanVect.setSize(a._vectSize);
      a._meanVect[0] = 4.0;
      a._meanVect[1] = 5.0;
      a._meanVect[2] = 6.0;
      a._covVect.setSize(a._vectSize);
      a._covVect[0] = 7.0;
      a._covVect[1] = 8.0;
      a._covVect[2] = 9.0;
      a._stdVect.setSize(a._vectSize);
      a._stdVect[0] = 10.0;
      a._stdVect[1] = 11.0;
      a._stdVect[2] = 12.0;
      a._xaccVect.setSize(a._vectSize);
      a._xaccVect[0] = 13.0;
      a._xaccVect[1] = 14.0;
      a._xaccVect[2] = 15.0;
      
      FrameAccGD b(a);

      test(a._vectSize == b._vectSize, __LINE__);
      test(a._vectSizeDefined == b._vectSizeDefined, __LINE__);
      test(a._count == b._count, __LINE__);
      test(a._computed == b._computed, __LINE__);
      test(a._stdComputed == b._stdComputed, __LINE__);
      test(a._accVect == b._accVect, __LINE__);
      test(a._meanVect == b._meanVect, __LINE__);
      test(a._covVect == b._covVect, __LINE__);
      test(a._stdVect == b._stdVect, __LINE__);
      test(a._xaccVect == b._xaccVect, __LINE__);

      a._vectSizeDefined = false;
      a._computed = false;
      a._stdComputed = false;
      
      FrameAccGD c(a);

      test(a._vectSizeDefined == c._vectSizeDefined, __LINE__);
      test(a._computed == c._computed, __LINE__);
      test(a._stdComputed == c._stdComputed, __LINE__);
    }
    // test operator=
    {
      FrameAccGD a;
      a._vectSize = 3;
      a._vectSizeDefined = true;
      a._count = 5;
      a._computed = true;
      a._stdComputed = true;
      a._accVect.setSize(a._vectSize);
      a._accVect[0] = 1.0;
      a._accVect[1] = 2.0;
      a._accVect[2] = 3.0;
      a._meanVect.setSize(a._vectSize);
      a._meanVect[0] = 4.0;
      a._meanVect[1] = 5.0;
      a._meanVect[2] = 6.0;
      a._covVect.setSize(a._vectSize);
      a._covVect[0] = 7.0;
      a._covVect[1] = 8.0;
      a._covVect[2] = 9.0;
      a._stdVect.setSize(a._vectSize);
      a._stdVect[0] = 10.0;
      a._stdVect[1] = 11.0;
      a._stdVect[2] = 12.0;
      a._xaccVect.setSize(a._vectSize);
      a._xaccVect[0] = 13.0;
      a._xaccVect[1] = 14.0;
      a._xaccVect[2] = 15.0;
      
      FrameAccGD b;
      b = a;

      test(a._vectSize == b._vectSize, __LINE__);
      test(a._vectSizeDefined == b._vectSizeDefined, __LINE__);
      test(a._count == b._count, __LINE__);
      test(a._computed == b._computed, __LINE__);
      test(a._stdComputed == b._stdComputed, __LINE__);
      test(a._accVect == b._accVect, __LINE__);
      test(a._meanVect == b._meanVect, __LINE__);
      test(a._covVect == b._covVect, __LINE__);
      test(a._stdVect == b._stdVect, __LINE__);
      test(a._xaccVect == b._xaccVect, __LINE__);

      a._vectSizeDefined = false;
      a._computed = false;
      a._stdComputed = false;
      
      FrameAccGD c;
      c = a;

      test(a._vectSizeDefined == c._vectSizeDefined, __LINE__);
      test(a._computed == c._computed, __LINE__);
      test(a._stdComputed == c._stdComputed, __LINE__);
    }
    // test method : void reset()
    {
      FrameAccGD a;
      a._count = 2;
      a._vectSizeDefined = true;
      a.reset();
      test(a._count == 0, __LINE__);
      test(a._vectSizeDefined == false, __LINE__);
      test(a._computed == false, __LINE__);
    }
    // test method : unsigned long getCount() const
    {
      FrameAccGD a;
      a._count = 2;
      test(a.getCount() == 2, __LINE__);
      a._count = 0;
      test(a.getCount() == 0, __LINE__);
    }
    // test method : const DoubleVector& getAccVect() const
    {
      FrameAccGD a;
      test(&a.getAccVect() == &a._accVect, __LINE__);
    }
    // test method : const DoubleVector& getxAccVect() const
    {
      FrameAccGD a;
      test(&a.getxAccVect() == &a._xaccVect, __LINE__);
    }
    // test method : void accumulate(const Feature&);
    {
      FrameAccGD a;
      a._vectSize = 45;
      Feature f0(0);
      a.accumulate(f0);
      test(a._count == 1, __LINE__);
      test(a._vectSizeDefined == true, __LINE__);
      test(a._vectSize == 0, __LINE__);
      test(a._accVect.size() == 0, __LINE__);
      test(a._xaccVect.size() == 0, __LINE__);
      test(a._computed == false, __LINE__);

      Feature f1(1);
      try { a.accumulate(f1); showError(__LINE__); }
      catch (Exception&){}
      
      a.reset();
      test(a._computed == false, __LINE__);
      f1[0] = 5.0;
      a.accumulate(f1);
      test(a._count == 1, __LINE__);
      test(a._vectSizeDefined == true, __LINE__);
      test(a._vectSize == 1, __LINE__);
      test(a._accVect.size() == 1, __LINE__);
      test(a._xaccVect.size() == 1, __LINE__);
      test(a._accVect[0] == 5.0, __LINE__);
      test(a._xaccVect[0] == 5.0*5.0, __LINE__);
      test(a._computed == false, __LINE__);

      Feature f2(1);
      f2[0] = 6.0;
      a.accumulate(f2);
      test(a._count == 2, __LINE__);
      test(a._vectSizeDefined == true, __LINE__);
      test(a._vectSize == 1, __LINE__);
      test(a._accVect.size() == 1, __LINE__);
      test(a._xaccVect.size() == 1, __LINE__);
      test(a._accVect[0] == 5.0+6.0, __LINE__);
      test(a._xaccVect[0] == 5.0*5.0+6.0*6.0, __LINE__);
      test(a._computed == false, __LINE__);
      
      a.reset();
      test(a._computed == false, __LINE__);
      Feature f3(2);
      f3[0] = 5.0;
      f3[1] = 7.0;
      a.accumulate(f3);
      test(a._count == 1, __LINE__);
      test(a._vectSizeDefined == true, __LINE__);
      test(a._vectSize == 2, __LINE__);
      test(a._accVect.size() == 2, __LINE__);
      test(a._xaccVect.size() == 2, __LINE__);
      test(a._accVect[0] == 5.0, __LINE__);
      test(a._xaccVect[0] == 5.0*5.0, __LINE__);
      test(a._accVect[1] == 7.0, __LINE__);
      test(a._xaccVect[1] == 7.0*7.0, __LINE__);
      test(a._computed == false, __LINE__);

      Feature f4(2);
      f4[0] = 6.0;
      f4[1] = 8.0;
      a.accumulate(f4);
      test(a._count == 2, __LINE__);
      test(a._vectSizeDefined == true, __LINE__);
      test(a._vectSize == 2, __LINE__);
      test(a._accVect.size() == 2, __LINE__);
      test(a._xaccVect.size() == 2, __LINE__);
      test(a._accVect[0] == 5.0+6.0, __LINE__);
      test(a._xaccVect[0] == 5.0*5.0+6.0*6.0, __LINE__);
      test(a._accVect[1] == 7.0+8.0, __LINE__);
      test(a._xaccVect[1] == 7.0*7.0+8.0*8.0, __LINE__);
      test(a._computed == false, __LINE__);
    }
    // test method : const DoubleVector& getMeanVect()
    // test method : const DoubleVector& getCovVect()
    // test method : const DoubleVector& getStdVect()
    {
      FrameAccGD a;
      try { a.getMeanVect(); showError(__LINE__); }
      catch (Exception&) {}
      try
      {
        Feature f0(2), f1(2), f2(2);
        f0[0] = 1.0;
        f0[1] = 2.0;
        f1[0] = 3.0;
        f1[1] = 4.0;
        f2[0] = 8.0;
        f2[1] = 9.0;
        a.accumulate(f0);
        {
          const DoubleVector& v = a.getMeanVect();
          test(v.size() == 2, __LINE__);
          test(v[0] == 1.0, __LINE__);
          test(v[1] == 2.0, __LINE__);
          test(a._computed == true, __LINE__);
        }
        {
          const DoubleVector& v = a.getCovVect();
          test(v.size() == 2, __LINE__);
          test(v[0] == 0.0, __LINE__);
          test(v[1] == 0.0, __LINE__);
          test(a._computed == true, __LINE__);
        }
        {
          const DoubleVector& v = a.getStdVect();
          test(v.size() == 2, __LINE__);
          test(v[0] == 0.0, __LINE__);
          test(v[1] == 0.0, __LINE__);
          test(a._computed == true, __LINE__);
        }
        a.accumulate(f1);
        {
          const DoubleVector& v = a.getMeanVect();
          test(v.size() == 2, __LINE__);
          test(v[0] == (1.0+3.0)/2.0, __LINE__);
          test(v[1] == (2.0+4.0)/2.0, __LINE__);
          test(a._computed == true, __LINE__);
        }
        {
          const DoubleVector& v = a.getCovVect();
          test(v.size() == 2, __LINE__);
          test(v[0] == 1.0, __LINE__);
          test(v[1] == 1.0, __LINE__);
          test(a._computed == true, __LINE__);
        }
        {
          const DoubleVector& v = a.getStdVect();
          test(v.size() == 2, __LINE__);
          test(v[0] == 1.0, __LINE__);
          test(v[1] == 1.0, __LINE__);
          test(a._computed == true, __LINE__);
        }
        a.accumulate(f2);
        {
          const DoubleVector& v = a.getMeanVect();
          test(v.size() == 2, __LINE__);
          test(v[0] == (1.0+3.0+8.0)/3.0, __LINE__);
          test(v[1] == (2.0+4.0+9.0)/3.0, __LINE__);
          test(a._computed == true, __LINE__);
        }
        {
          const DoubleVector& v = a.getCovVect();
          test(v.size() == 2, __LINE__);
          test(fabs(v[0]-26.0/3.0) < 1e-14, __LINE__);
          test(fabs(v[1]-26.0/3.0) < 1e-14, __LINE__);
          test(a._computed == true, __LINE__);
        }
        {
          const DoubleVector& v = a.getStdVect();
          test(v.size() == 2, __LINE__);
          test(fabs(v[0]-sqrt(26.0/3.0)) < 1e-14, __LINE__);
          test(fabs(v[1]-sqrt(26.0/3.0)) < 1e-14, __LINE__);
          test(a._computed == true, __LINE__);
        }
      }
      catch (Exception&) { showError(__LINE__); }
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestFrameAccGD::getClassName() const { return "TestFrameAccGD"; }
//-------------------------------------------------------------------------
TestFrameAccGD::~TestFrameAccGD() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFrameAccGD_cpp)

