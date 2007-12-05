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

#if !defined(ALIZE_TestRefVector_cpp)
#define ALIZE_TestRefVector_cpp

#include "TestRefVector.h"
#include "RefVector.h"
#include "alizeString.h"
#include "Exception.h"
#include "Feature.h"

using namespace alize;

//-------------------------------------------------------------------------
TestRefVector::TestRefVector()
:Test() {}
//-------------------------------------------------------------------------
bool TestRefVector::exec()
{
  Test::exec();
  {
    typedef const String T;
    typedef RefVector<T> V;
    unsigned long i;

    // test method : String getClassname() const
    
    test(V().getClassName() == "RefVector", __LINE__);

    // test method : explicit V(unsigned long capacity = 1)

    V v;
    test(V()._size == 0, __LINE__);
    test(V()._capacity == 1, __LINE__);
    test(V(0)._capacity == 1, __LINE__);
    test(V()._array != NULL, __LINE__);
    test(V(5)._size == 0, __LINE__);
    test(V(5)._capacity == 5, __LINE__);
    test(V(5)._array != NULL, __LINE__);
  
    // test method : unsigned long addObject(Object& o)
    {
      V v(2);
      T s1, s2, s3;

      v.addObject(s1);
      test(v._size == 1, __LINE__);
      test(v._capacity == 2, __LINE__);

      v.addObject(s2);
      test(v._size == 2, __LINE__);
      test(v._capacity == 2, __LINE__);

      v.addObject(s3);
      test(v._size == 3, __LINE__);
      test(v._capacity == 4, __LINE__); // 2+2

      test(v._array[0] == &s1, __LINE__);
      test(v._array[1] == &s2, __LINE__);
      test(v._array[2] == &s3, __LINE__);
    }

    // test method : V(const V& o);
    {
      test(V(V())._size == 0, __LINE__);
      test(V(V())._capacity == 1, __LINE__);
      test(V(V())._array != NULL, __LINE__);
      V v(5);
      T t[5]; // 5 objects
      for (i=0; i<5; i++)
        v.addObject(t[i]);
      V vbis(v);
      test(vbis._size == 5, __LINE__);
      test(vbis._capacity == 5, __LINE__);
      test(vbis._array != NULL, __LINE__);
      for (i=0; i<5; i++)
        if (v._array[i] != vbis._array[i])
        {
          showError(__LINE__);
          break;
        }
    }

    // test method : const V& operator=(const V&)
    {
      V v(5);
      T t[5]; // 5 objects
      for (i=0; i<5; i++)
        v.addObject(t[i]);
      V vbis; // capacity = 1
      vbis = v;
      test(vbis._size == 5, __LINE__);
      test(vbis._capacity == 5, __LINE__);
      test(vbis._array != NULL, __LINE__);
      for (i=0; i<5; i++)
        if (v._array[i] != vbis._array[i])
        {
          showError(__LINE__);
          break;
        }
      V vter(10);
      vter = v;
      test(vter._size == 5, __LINE__);
      test(vter._capacity == 10, __LINE__);
      test(vter._array != NULL, __LINE__);
      for (i=0; i<5; i++)
        if (v._array[i] != vter._array[i])
        {
          showError(__LINE__);
          break;
        }
    }

    // test method : unsigned long size() const
    {
      T s;
      V v1;
      V v2(5);
      v1.addObject(s);
      v2.addObject(s);
      test(V().size() == 0, __LINE__);
      test(v1.size() == 1, __LINE__);
      test(v2.size() == 1, __LINE__);
    }
  
    // test method : void setObject(Object& o, unsigned long index) const
    {
      V v(4);
      T s1, s2, s3, s4;
      try { v.setObject(s1, 0); showError(__LINE__);}
      catch (IndexOutOfBoundsException&) {}
      v.addObject(s1);
      try { v.setObject(s1, 0);}
      catch (IndexOutOfBoundsException&) { showError(__LINE__);}
      v.addObject(s2);
      v.addObject(s3);
      v.setObject(s4, 1);
      test(v._array[0] == &s1, __LINE__);
      test(v._array[1] == &s4, __LINE__);
      test(v._array[2] == &s3, __LINE__);
    }
  
    // test method : Object& getObject(unsigned long index) const
    // test method : Object& operator[](unsigned long index)
    // test method : const Object& operator[](unsigned long index) const
    {
      V v(4);
      T s1, s2, s3, s4;
      v.addObject(s1);
      v.addObject(s2);
      v.addObject(s3);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s2), __LINE__);
      test(v.getObject(2).isSameObject(s3), __LINE__);
      test(v[0].isSameObject(s1), __LINE__);
      test(v[1].isSameObject(s2), __LINE__);
      test(v[2].isSameObject(s3), __LINE__);
      const V v1(v);
      test(v1[0].isSameObject(s1), __LINE__);
      test(v1[1].isSameObject(s2), __LINE__);
      test(v1[2].isSameObject(s3), __LINE__);
    }

    // test method : Object** getArray() const
    {
      V v;
      test(v._array == v.getArray(), __LINE__);
    }

    // test method : void deleteAllObjects(unsigned long first)
    {
      V v;
      v.addObject(*new String());
      v.addObject(*new String());
      v.addObject(*new String());
      v.addObject(*new String());
      v.deleteAllObjects(3);
      test(v.size() == 3, __LINE__);
      v.deleteAllObjects();
      test(v.size() == 0, __LINE__);
    }

    // test method : void RefVector::removeObjects(unsigned long
    //             first, unsigned long last, DeleteFlag fl) 
    {
      T s1, s2, s3, s4, s5, s6, s7, s8;
      V v, v0;
      v0.addObject(s1);
      v0.addObject(s2);
      v0.addObject(s3);
      v0.addObject(s4);
      v0.addObject(s5);

      v = v0;
      v.removeObjects(1,3);
      test(v.size() == 2, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s5), __LINE__);
      v.clear();

      v = v0;
      v.removeObjects(1,4);
      test(v.size() == 1, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      v.clear();

      v = v0;
      v.removeObjects(1,2);
      test(v.size() == 3, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s4), __LINE__);
      test(v.getObject(2).isSameObject(s5), __LINE__);
      v.clear();

      v = v0;
      v.removeObjects(1,1);
      test(v.size() == 4, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s3), __LINE__);
      test(v.getObject(2).isSameObject(s4), __LINE__);
      test(v.getObject(3).isSameObject(s5), __LINE__);
      v.clear();

      v = v0;
      v.removeObjects(1,99);
      test(v.size() == 1, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      v.clear();

      v = v0;
      v.removeObjects(1,0); // sans effet 
      test(v.size() == 5, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s2), __LINE__);
      test(v.getObject(2).isSameObject(s3), __LINE__);
      test(v.getObject(3).isSameObject(s4), __LINE__);
      test(v.getObject(4).isSameObject(s5), __LINE__);
      v.clear();

      v = v0;
      v.removeObjects(0, 99);
      test(v.size() == 0, __LINE__);

      v = v0;
      v.removeObjects(4, 4);
      test(v.size() == 4, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s2), __LINE__);
      test(v.getObject(2).isSameObject(s3), __LINE__);
      test(v.getObject(3).isSameObject(s4), __LINE__);
      v.clear();

      v.addObject(s1);
      v.addObject(*new String("11"));
      v.addObject(*new String("11"));
      v.addObject(s2);
      v.removeObjects(1, 2, DELETE);
    }
    // test method : Object& RefVector::removeObject(unsigned long i)
    {
      T s1, s2, s3;
      V v, v0;
      v0.addObject(s1);
      v0.addObject(s2);
      v0.addObject(s3);

      v = v0;
      test(v.removeObject(1).isSameObject(s2), __LINE__);
      test(v.size() == 2, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s3), __LINE__);
      test(v.removeObject(1).isSameObject(s3), __LINE__);
      test(v.size() == 1, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.removeObject(0).isSameObject(s1), __LINE__);
      test(v.size() == 0, __LINE__);
    }

    // test method : void clear()
    {
      T s;
      V v;
      v.addObject(s);
      v.clear();
      test(v.size() == 0, __LINE__);
      test(v._capacity == 1, __LINE__);
      test(v._array[0] == &s, __LINE__);
    }
    // test method unsigned long addObject(Object& o, unsigned long i)
    {
      T s1, s2, s3, s4, s5;
      V v;
      v.addObject(s2);
      v.addObject(s3);
      v.addObject(s1, 0);
      test(v.size() == 3, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s2), __LINE__);
      test(v.getObject(2).isSameObject(s3), __LINE__);
      v.addObject(s4, 2);
      test(v.size() == 4, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s2), __LINE__);
      test(v.getObject(2).isSameObject(s4), __LINE__);
      test(v.getObject(3).isSameObject(s3), __LINE__);
      v.addObject(s4, 999);
      test(v.size() == 5, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s2), __LINE__);
      test(v.getObject(2).isSameObject(s4), __LINE__);
      test(v.getObject(3).isSameObject(s3), __LINE__);
      test(v.getObject(4).isSameObject(s4), __LINE__);
      v.addObject(s5, 1);
      test(v.size() == 6, __LINE__);
      test(v.getObject(0).isSameObject(s1), __LINE__);
      test(v.getObject(1).isSameObject(s5), __LINE__);
      test(v.getObject(2).isSameObject(s2), __LINE__);
      test(v.getObject(3).isSameObject(s4), __LINE__);
      test(v.getObject(4).isSameObject(s3), __LINE__);
      test(v.getObject(5).isSameObject(s4), __LINE__);
    }
    // test method : void removeObject(Object& o)
    {
      T s1, s2, s3, s4, s5, s6;
      V v;
      v.addObject(s1);
      v.addObject(s2);
      v.addObject(s3);
      v.addObject(s4);
      v.addObject(s3);
      v.addObject(s5);
      v.addObject(s6);
      v.removeObject(s1);
      test(v.size() == 6, __LINE__);
      test(v.getObject(0).isSameObject(s2), __LINE__);
      test(v.getObject(1).isSameObject(s3), __LINE__);
      test(v.getObject(2).isSameObject(s4), __LINE__);
      test(v.getObject(3).isSameObject(s3), __LINE__);
      test(v.getObject(4).isSameObject(s5), __LINE__);
      test(v.getObject(5).isSameObject(s6), __LINE__);
      try { v.removeObject(s1); showError(__LINE__); }
      catch (Exception&) {}
      test(v.size() == 6, __LINE__);
      v.removeObject(s6);
      test(v.size() == 5, __LINE__);
      test(v.getObject(0).isSameObject(s2), __LINE__);
      test(v.getObject(1).isSameObject(s3), __LINE__);
      test(v.getObject(2).isSameObject(s4), __LINE__);
      test(v.getObject(3).isSameObject(s3), __LINE__);
      test(v.getObject(4).isSameObject(s5), __LINE__);
      try { v.removeObject(s6); showError(__LINE__); }
      catch (Exception&) {}
      test(v.size() == 5, __LINE__);
      v.removeObject(s3);
      test(v.size() == 4, __LINE__);
      test(v.getObject(0).isSameObject(s2), __LINE__);
      test(v.getObject(1).isSameObject(s4), __LINE__);
      test(v.getObject(2).isSameObject(s3), __LINE__);
      test(v.getObject(3).isSameObject(s5), __LINE__);
    }
    // test method unsigned long getObjectIndex(const Object& o) const
    {
      V v;
      T s0, s1, s2;
      v.addObject(s0);
      v.addObject(s2);
      try 
      {
        test(v.getObjectIndex(s0) == 0, __LINE__);
        test(v.getObjectIndex(s2) == 1, __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
      try 
      {
        v.getObjectIndex(s1);
        showError(__LINE__);
      }
      catch (Exception&) {}

    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestRefVector::getClassName() const
{ return "TestRefVector"; }
//-------------------------------------------------------------------------
TestRefVector::~TestRefVector() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestRefVector_cpp)
