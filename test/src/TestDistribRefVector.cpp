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

#if !defined(ALIZE_TestDistribRefVector_cpp)
#define ALIZE_TestDistribRefVector_cpp

#include "TestDistribRefVector.h"
#include "DistribRefVector.h"
#include "alizeString.h"
#include "DistribGD.h"
#include "DistribGF.h"
#include "AutoDestructor.h"
#include "Exception.h"
#include <iostream>

using namespace alize;
using namespace std;

typedef DistribRefVector V;

//-------------------------------------------------------------------------
TestDistribRefVector::TestDistribRefVector()
:Test() {}
//-------------------------------------------------------------------------
bool TestDistribRefVector::exec()
{

  Test::exec();
  {
    unsigned long i;
    V v;
    // test method : String getClassname() const
    
    test(v.getClassName() == "DistribRefVector", __LINE__);

    // test method : explicit V(unsigned long capacity = 1)

//cout << v._size << v._capacity << endl;

    test(v._size == 0, __LINE__);
    test(v._capacity == 1, __LINE__);
    test(V(0)._capacity == 1, __LINE__);
    test(v._array != NULL, __LINE__);
    test(V(5)._size == 0, __LINE__);
    test(V(5)._capacity == 5, __LINE__);
    test(V(5)._array != NULL, __LINE__);
  
    // test method : unsigned long addDistrib(Distrib& o)
    {
      Distrib& d1 = DistribGD::create(K::k, 1);
      Distrib& d2 = DistribGF::create(K::k, 1);
      Distrib& d3 = DistribGD::create(K::k, 1);
      d1.refCounter(K::k)++;
      {
        V v(2);
        v.addDistrib(d1);
        test(v._size == 1, __LINE__);
        test(v._capacity == 2, __LINE__);
        test(d1._refCounter == 2, __LINE__);

        v.addDistrib(d2);
        test(v._size == 2, __LINE__);
        test(v._capacity == 2, __LINE__);
        test(d2._refCounter == 1, __LINE__);

        v.addDistrib(d3);
        test(v._size == 3, __LINE__);
        test(v._capacity == 4, __LINE__); // 2+2
        test(d3._refCounter == 1, __LINE__);

        test(v._array[0] == &d1, __LINE__);
        test(v._array[1] == &d2, __LINE__);
        test(v._array[2] == &d3, __LINE__);
        } // here v is destroyed
      test(d1._refCounter == 1, __LINE__);
      delete &d1;
    }

    // test method : V(const V& o);
    {
      test(V(v)._size == 0, __LINE__);
      test(V(v)._capacity == 1, __LINE__);
      test(V(v)._array != NULL, __LINE__);
      V vv(5);
      vv.addDistrib(DistribGD::create(K::k, 1));
      vv.addDistrib(DistribGD::create(K::k, 1));
      vv.addDistrib(DistribGD::create(K::k, 1));
      vv.addDistrib(DistribGD::create(K::k, 1));
      vv.addDistrib(DistribGD::create(K::k, 1));
 
      V vbis(vv);
      test(vbis._size == 5, __LINE__);
      test(vbis._capacity == 5, __LINE__);
      test(vbis._array != NULL, __LINE__);
      for (i=0; i<5; i++)
      {
        if (vv._array[i] != vbis._array[i])
        {
          showError(__LINE__);
          break;
        }
        if (vbis.getDistrib(i).refCounter(K::k) != 2)
        {
          showError(__LINE__);
          break;
        }
      }
    }

    // test method : unsigned long size() const
    {

      DistribGD& ss = DistribGD::create(K::k, 1);

      AutoDestructor as1(ss);
      ss.refCounter(K::k)++;
      V v1;
      V v2(5);
      v1.addDistrib(ss);
      v2.addDistrib(ss);
      test(v.size() == 0, __LINE__);
      test(v1.size() == 1, __LINE__);
      test(v2.size() == 1, __LINE__);
    }
  
    // test method : void setDistrib(Distrib& o, unsigned long index) const
    {
      V v(4);
      DistribGD& d1 = DistribGD::create(K::k, 1);
      AutoDestructor as1(d1);
      DistribGD& d2 = DistribGD::create(K::k, 1);
      AutoDestructor as2(d2);
      DistribGD& d3 = DistribGD::create(K::k, 1);
      AutoDestructor as3(d3);
      DistribGD& d4 = DistribGD::create(K::k, 1);
      AutoDestructor as4(d4);
      d1.refCounter(K::k)++;
      d2.refCounter(K::k)++;
      d3.refCounter(K::k)++;
      d4.refCounter(K::k)++;
      try { v.setDistrib(d1, 0); showError(__LINE__);}
      catch (IndexOutOfBoundsException&) {}
        v.addDistrib(d1);
      try { v.setDistrib(d1, 0);}
      catch (IndexOutOfBoundsException&) { showError(__LINE__);}
      v.addDistrib(d2);
      v.addDistrib(d3);
      v.setDistrib(d4, 1);
      test(v._array[0] == &d1, __LINE__);
      test(v._array[1] == &d4, __LINE__);
      test(v._array[2] == &d3, __LINE__);
      test(d2._refCounter == 1, __LINE__);
      test(d4._refCounter == 2, __LINE__);
    }

    // test method : Distrib& getDistrib(unsigned long index) const
    {
      V v(4);
      //cout << "avant" << endl;
      DistribGD& d1 = DistribGD::create(K::k, 1);
      //cout << "après" << endl;
      AutoDestructor as1(d1);
      DistribGD& d2 = DistribGD::create(K::k, 1);
      AutoDestructor as2(d2);
      DistribGD& d3 = DistribGD::create(K::k, 1);
      AutoDestructor as3(d3);
      DistribGD& d4 = DistribGD::create(K::k, 1);
      AutoDestructor as4(d4);
      d1.refCounter(K::k)++;
      d2.refCounter(K::k)++;
      d3.refCounter(K::k)++;
      d4.refCounter(K::k)++;
      v.addDistrib(d1);
      v.addDistrib(d2);
      v.addDistrib(d3);
      test(v.getDistrib(0).isSameObject(d1), __LINE__);
      test(v.getDistrib(1).isSameObject(d2), __LINE__);
      test(v.getDistrib(2).isSameObject(d3), __LINE__);
    }

    // test method : Distrib** getArray() const
    {
      V v;
      test(v._array == v.getArray(), __LINE__);
    }

    // test method : void clear()
    {
      DistribGD& s = DistribGD::create(K::k, 1);
      AutoDestructor as1(s);
      s.refCounter(K::k)++;
      V v;
      v.addDistrib(s);
      v.clear();
      test(v.size() == 0, __LINE__);
      test(v._capacity == 1, __LINE__);
      test(s._refCounter == 1, __LINE__);
    }

    // test method : void deleteUnreferencedDistribs()
    {
      {
        V v;
        v.addDistrib(DistribGD::create(K::k, 10));
        v.addDistrib(DistribGD::create(K::k, 10));
        v.addDistrib(DistribGD::create(K::k, 10));
        v.deleteUnreferencedDistribs();
        test(v.size() == 0, __LINE__);
      }
      {
        DistribGD& d0 = DistribGD::create(K::k, 10);
        DistribGD& d1 = DistribGD::create(K::k, 10);
        DistribGD& d2 = DistribGD::create(K::k, 10);
        V v;
        v.addDistrib(d0);
        v.addDistrib(d1);
        v.addDistrib(d2);
        d0.refCounter(K::k)++;
        d2.refCounter(K::k)++;
        v.deleteUnreferencedDistribs();
        test(v.size() == 2, __LINE__);
        test(v.getDistrib(0).isSameObject(d0), __LINE__);
        test(v.getDistrib(1).isSameObject(d2), __LINE__);
        d0.refCounter(K::k)--;
        d2.refCounter(K::k)--;
      }
      {
        DistribGD& d0 = DistribGD::create(K::k, 10);
        DistribGD& d1 = DistribGD::create(K::k, 10);
        DistribGD& d2 = DistribGD::create(K::k, 10);
        V v;
        v.addDistrib(d0);
        v.addDistrib(d1);
        v.addDistrib(d2);
        d1.refCounter(K::k)++;
        v.deleteUnreferencedDistribs();
        test(v.size() == 1, __LINE__);
        test(v.getDistrib(0).isSameObject(d1), __LINE__);
        d1.refCounter(K::k)--;
      }
      {
        DistribGD& d0 = DistribGD::create(K::k, 10);
        V v;
        v.addDistrib(d0);
        d0.refCounter(K::k)++;
        v.deleteUnreferencedDistribs();
        test(v.size() == 1, __LINE__);
        d0.refCounter(K::k)--;
        v.deleteUnreferencedDistribs();
        test(v.size() == 0, __LINE__);
      }
      {
        DistribGD& d0 = DistribGD::create(K::k, 10);
        DistribGD& d1 = DistribGD::create(K::k, 10);
        DistribGD& d2 = DistribGD::create(K::k, 10);
        V v;
        v.addDistrib(d0);
        v.addDistrib(d1);
        v.addDistrib(d2);
        d0.refCounter(K::k)++;
        d1.refCounter(K::k)++;
        d2.refCounter(K::k)++;
        v.deleteUnreferencedDistribs();
        test(v.size() == 3, __LINE__);
        test(v.getDistrib(0).isSameObject(d0), __LINE__);
        test(v.getDistrib(1).isSameObject(d1), __LINE__);
        test(v.getDistrib(2).isSameObject(d2), __LINE__);
        d0.refCounter(K::k)--;
        d1.refCounter(K::k)--;
        d2.refCounter(K::k)--;
        v.deleteUnreferencedDistribs();
        test(v.size() == 0, __LINE__);
      }
    }
    // test method : void deleteUnreferencedDistrib(const Distrib&)
    {
      DistribGD& d0 = DistribGD::create(K::k, 10);
      DistribGD& d1 = DistribGD::create(K::k, 10);
      DistribGD& d2 = DistribGD::create(K::k, 10);
      DistribGD& d3 = DistribGD::create(K::k, 10);
      V v;
      v.addDistrib(d0);
      v.addDistrib(d1);
      v.addDistrib(d2);
      v.addDistrib(d3);
      d0.refCounter(K::k)++;
      d1.refCounter(K::k)++;
      d2.refCounter(K::k)++;
      d3.refCounter(K::k)++;
      v.deleteUnreferencedDistrib(d1); // sans effet normalement
      test(v.size() == 4, __LINE__);
      test(v.getDistrib(0).isSameObject(d0), __LINE__);
      test(v.getDistrib(1).isSameObject(d1), __LINE__);
      test(v.getDistrib(2).isSameObject(d2), __LINE__);
      test(v.getDistrib(3).isSameObject(d3), __LINE__);
      d1.refCounter(K::k)--;
      v.deleteUnreferencedDistrib(d1); // delete d1
      test(v.size() == 3, __LINE__);
      test(v.getDistrib(0).isSameObject(d0), __LINE__);
      test(v.getDistrib(1).isSameObject(d2), __LINE__);
      test(v.getDistrib(2).isSameObject(d3), __LINE__);
      d3.refCounter(K::k)--;
      v.deleteUnreferencedDistrib(d3); // delete d3
      test(v.size() == 2, __LINE__);
      test(v.getDistrib(0).isSameObject(d0), __LINE__);
      test(v.getDistrib(1).isSameObject(d2), __LINE__);
      d0.refCounter(K::k)--;
      d2.refCounter(K::k)--;
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestDistribRefVector::getClassName() const
{ return "TestDistribRefVector"; }
//-------------------------------------------------------------------------
TestDistribRefVector::~TestDistribRefVector() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestDistribRefVector_cpp)
