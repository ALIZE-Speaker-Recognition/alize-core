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

#if !defined(ALIZE_TestDoubleSquareMatrix_cpp)
#define ALIZE_TestDoubleSquareMatrix_cpp

#include "TestDoubleSquareMatrix.h"
#include "DoubleSquareMatrix.h"
#include "alizeString.h"
#include "Exception.h"
#include <cmath>
#include <iostream>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestDoubleSquareMatrix::TestDoubleSquareMatrix()
:Test() {}
//-------------------------------------------------------------------------
bool TestDoubleSquareMatrix::exec()
{
  Test::exec();
  {
    typedef DoubleSquareMatrix V;

    // test method : String getClassName() const

    test(V(1).getClassName() == "DoubleSquareMatrix", __LINE__);

    // test method : explicit V(unsigned long size);
    {
      V v;
      test(v._size == 0, __LINE__);
      test(V(0)._size == 0, __LINE__);
      test(V(1)._size == 1, __LINE__);
    }

    // test method : unsigned long size() const;
    {
      test(V(0).size() == 0, __LINE__);
      test(V(1).size() == 1, __LINE__);
      test(V(99).size() == 99, __LINE__);
    }

    // test method : double& operator()(unsigned long i, unsigned long j)
    {
      V v(2);
      try { v(3,0); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      try { v(0,3);  showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}

      v._array[0+0*v.size()]= 1.2;
      v._array[0+1*v.size()]= 3.4;
      v._array[1+0*v.size()]= 5.6;
      v._array[1+1*v.size()]= 7.8;
      test(v(0,0) == 1.2, __LINE__);
      test(v(0,1) == 3.4, __LINE__);
      test(v(1,0) == 5.6, __LINE__);
      test(v(1,1) == 7.8, __LINE__);

      v(0,0) = 10;
      v(0,1) = 11;
      v(1,0) = 12;
      v(1,1) = 13;
      test(v(0,0) == 10, __LINE__);
      test(v(0,1) == 11, __LINE__);
      test(v(1,0) == 12, __LINE__);
      test(v(1,1) == 13, __LINE__);
    }

    // test method : V(const V&);
    {
      V v1(2);
      v1(0,0) = 10;
      v1(0,1) = 11;
      v1(1,0) = 12;
      v1(1,1) = 13;
      const V v(v1);
      test(v.size() == v1.size(), __LINE__);
      test(v(0,0) == v1(0,0), __LINE__);
      test(v(0,1) == v1(0,1), __LINE__);
      test(v(1,0) == v1(1,0), __LINE__);
      test(v(1,1) == v1(1,1), __LINE__);
    }

    // test method : double operator()(unsigned long i,
    //                 unsigned long j) const;
    {
      V v1(2);
      v1(0,0) = 10;
      v1(0,1) = 11;
      v1(1,0) = 12;
      v1(1,1) = 13;
      const V v(v1);
      try { v(3,0); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      try { v(0,3); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}

      test(v(0,0) == v1(0,0), __LINE__);
      test(v(0,1) == v1(0,1), __LINE__);
      test(v(1,0) == v1(1,0), __LINE__);
      test(v(1,1) == v1(1,1), __LINE__);
    }

    // test method : double* getArray() const;
    {
      V v(2);
      test(v._array.getArray() == v.getArray(), __LINE__);
    }

    // test method : bool operator==(const DoubleSquareMatrix&) const
    //  and method : bool operator!=(const DoubleSquareMatrix&) const
    {
      V v(2);
      v(0,0) = 10;
      v(0,1) = 11;
      v(1,0) = 12;
      v(1,1) = 13;
      
      V v1(v);
      test(v == v1, __LINE__);
      test(!(v != v1), __LINE__);

      v(0,0) = 0;
      test(!(v == v1), __LINE__);
      test(v != v1, __LINE__);
      v(0,0) = 10;

      v(1,0) = 0;
      test(!(v == v1), __LINE__);
      test(v != v1, __LINE__);
    }

    // test method : const DoubleSquareMatrix&
    //     DoubleSquareMatrix::operator=(const DoubleSquareMatrix& v)
    {
      V v(2);
      v(0,0) = 10;
      v(0,1) = 11;
      v(1,0) = 12;
      v(1,1) = 13;
      try
      {
        V v1(2);
        V v2(v);
        v1 = v;
        test(v1 == v, __LINE__);
        v2 = v2; // auto affectation
        test(v2 == v, __LINE__);
        V v3(3);
        v3 = v;
        test(v3 == v, __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }      
    }

    // test method : real_t invert()
    {
      unsigned long ii, jj;
      // test matrice dim 0
      V m(0), mi(0);
      try { m.invert(mi); showError(__LINE__); } 
      catch (Exception&) {}
      // test with dim >= 1
      V m1(1), mi1(1), mi1b(1);
      double det = m1(0,0) = 1.5;
      test (fabs(m1.invert(mi1)-det) < 1e-15, __LINE__);
      mi1.invert(mi1b);
      test (mi1b(0,0)-m1(0,0) < 1e-15, __LINE__);

      V m2(2), mi2(2), mi2b(2);
      m2(0,0) = 30;
      m2(0,1) = 20;
      m2(1,0) = 20;
      m2(1,1) = 36;
      test (m2.invert(mi2) == 30*36-20*20, __LINE__);
      mi2.invert(mi2b);
      for (ii=0; ii<2; ii++)
        for (jj=0; jj<2; jj++)
          test (fabs(mi2b(ii,jj)-m2(ii,jj)) < 1e-13, __LINE__);

      V m3(3), mi3(3), mi3b(3);
      double a = m3(0,0) = 10;
      double b = m3(0,1) = 2;
      double c = m3(0,2) = 3;
      double d = m3(1,0) = 2;
      double e = m3(1,1) = 5;
      double f = m3(1,2) = 1;
      double g = m3(2,0) = 3;
      double h = m3(2,1) = 1;
      double i = m3(2,2) = 7;
      test (fabs(m3.invert(mi3)- a*e*i-a*f*h-b*d*i+b*f*g+c*d*h-c*e*g) < 1e20, __LINE__);
      mi3.invert(mi3b);
      for (ii=0; ii<3; ii++)
        for (jj=0; jj<3; jj++)
          test (fabs(mi3b(ii,jj)-m3(ii,jj)) < 1e-13, __LINE__);

      m3(0, 0) = 3;
      m3(1, 0) = 2;
      m3(2, 0) = 1;
      m3(0, 1) = 2;
      m3(1, 1) = 2;
      m3(2, 1) = 1;
      m3(0, 2) = 1;
      m3(1, 2) = 1;
      m3(2, 2) = 1;
      m3.invert(mi3);
      mi3.invert(mi3b);
      for (ii=0; ii<3; ii++)
        for (jj=0; jj<3; jj++)
          test (fabs(mi3b(ii,jj)-m3(ii,jj)) < 1e-13, __LINE__);

      // dimension = 4 // valeurs récupérées sur http://www.cs.ut.ee/~toomas_l/linalg/lin2/node24.html
      V m4(4), mi4(4), mi4b(4);
      m4(0,0) = 1;
      m4(1,0) = 2;
      m4(2,0) = -1;
      m4(3,0) = 2;
      m4(0,1) = 2;
      m4(1,1) = 8;
      m4(2,1) = 6;
      m4(3,1) = 0;
      m4(0,2) = -1;
      m4(1,2) = 6;
      m4(2,2) = 21;
      m4(3,2) = -2;
      m4(0,3) = 2;
      m4(1,3) = 0;
      m4(2,3) = -2;
      m4(3,3) = 25;
      m4.invert(mi4); // inverse
      mi4.invert(mi4b); // inverse de l'inverse
      for (ii=0; ii<3; ii++)
        for (jj=0; jj<3; jj++)
          test (fabs(mi4b(ii,jj)-m4(ii,jj)) < 1e-12, __LINE__);
    }
    // test method : void setSize(unsigned long size)
    {
      V v(2);
      v.setSize(10);
      test(v._size == 10, __LINE__);
      test(v._array.size() == 10*10, __LINE__);
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestDoubleSquareMatrix::getClassName() const
{ return "TestDoubleSquareMatrix"; }
//-------------------------------------------------------------------------
TestDoubleSquareMatrix::~TestDoubleSquareMatrix() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestDoubleSquareMatrix_cpp)
