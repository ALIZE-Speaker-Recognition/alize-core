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

#if !defined(ALIZE_TestFloatVector_cpp)
#define ALIZE_TestFloatVector_cpp

#include "TestFloatVector.h"
#include "FloatVector.h"
#include "alizeString.h"
#include "Exception.h"

using namespace alize;

//-------------------------------------------------------------------------
TestFloatVector::TestFloatVector()
:Test() {}
//-------------------------------------------------------------------------
bool TestFloatVector::exec()
{
    Test::exec();
    {
        typedef FloatVector V;

        // test method : String getClassName() const

        test(V().getClassName() == "FloatVector", __LINE__);

        // test method : explicit V(unsigned long size = 0);

        test(V()._size == 0, __LINE__);
        test(V()._capacity == 1, __LINE__);
        test(V()._array != NULL, __LINE__);
        test(V(5)._size == 0, __LINE__);
        test(V(5)._capacity == 5, __LINE__);

        // test method : void addValue(double);
        {
            V v;
            v.addValue(11);
            v.addValue(22);
            v.addValue(33);
            test(v._size == 3, __LINE__);
            test(v._capacity == 4, __LINE__);
            test(v._array[0] == 11, __LINE__);
            test(v._array[1] == 22, __LINE__);
            test(v._array[2] == 33, __LINE__);
        }

        // test method : void addValue(const ULongVector&) // 1 juin 2004
        {
            V v, v1;
            v.addValue(11);
            v.addValue(22);
            v.addValue(33);
            v.addValue(v1);
            test(v._size == 3, __LINE__);
            test(v._array[0] == 11, __LINE__);
            test(v._array[1] == 22, __LINE__);
            test(v._array[2] == 33, __LINE__);
            v1.addValue(1);
            v.addValue(v1);
            test(v._size == 4, __LINE__);
            test(v._array[0] == 11, __LINE__);
            test(v._array[1] == 22, __LINE__);
            test(v._array[2] == 33, __LINE__);
            test(v._array[3] == 1, __LINE__);
            v1.addValue(2);
            v.addValue(v1);
            test(v._size == 6, __LINE__);
            test(v._array[0] == 11, __LINE__);
            test(v._array[1] == 22, __LINE__);
            test(v._array[2] == 33, __LINE__);
            test(v._array[3] == 1, __LINE__);
            test(v._array[4] == 1, __LINE__);
            test(v._array[5] == 2, __LINE__);
        }

        // test method : unsigned long size() const;
        {
            test(V().size() == 0, __LINE__);
            V v;
            v.addValue(11);
            test(v.size() == 1, __LINE__);
        }

        // test method : double& operator[](unsigned long index);
        {
            V v;
            v.addValue(11);
            v.addValue(22);
            v.addValue(33);
            try { v[3]; showError(__LINE__); }
            catch (IndexOutOfBoundsException&) {}
            try { v[2]; }
            catch (IndexOutOfBoundsException&) { showError(__LINE__); }
            test(v[0] == 11, __LINE__);
            test(v[1] == 22, __LINE__);
            test(v[2] == 33, __LINE__);
            v[2] = 44;
            test(v[2] == 44, __LINE__);
        }

        // test method : V(const V&);
        {
            V v1;
            v1.addValue(11);
            v1.addValue(22);
            v1.addValue(33);
            const V v(v1);
            test(v._size == 3, __LINE__);
            test(v._capacity == 3, __LINE__);
            test(v[0] == 11, __LINE__);
            test(v[1] == 22, __LINE__);
            test(v[2] == 33, __LINE__);
        }

        // test method : double operator[](unsigned long index) const;
        {
            V v1;
            v1.addValue(11);
            v1.addValue(22);
            v1.addValue(33);
            const V v(v1);
            try { v[3]; showError(__LINE__); }
            catch (IndexOutOfBoundsException&) {}
            try { v[2]; }
            catch (IndexOutOfBoundsException&) { showError(__LINE__); }
            test(v[0] == 11, __LINE__);
            test(v[1] == 22, __LINE__);
            test(v[2] == 33, __LINE__);
        }

        // test method : void clear();
        {
            V v;
            v.addValue(11);
            v.addValue(22);
            v.addValue(33);
            v.clear();
            test(v._size == 0, __LINE__);
            test(v._capacity == 4, __LINE__);
        }

        // test method : double* getArray() const;
        {
            V v;
            test(v._array == v.getArray(), __LINE__);
        }

        // test method : bool operator==(const FloatVector&) const
        //  and method : bool operator!=(const FloatVector&) const
        {
            V v;
            v.addValue(11);
            v.addValue(22);
            v.addValue(33);
            V v1(v);
            test(v == v1, __LINE__);
            test(!(v != v1), __LINE__);

            v[0] = 0;
            test(!(v == v1), __LINE__);
            test(v != v1, __LINE__);
            v[0] = 11;

            v[1] = 0;
            test(!(v == v1), __LINE__);
            test(v != v1, __LINE__);
            v[1] = 22;

            v[2] = 0;
            test(!(v == v1), __LINE__);
            test(v != v1, __LINE__);
            v[2] = 33;

            v.addValue(44);
            test(!(v == v1), __LINE__);
            test(v != v1, __LINE__);
        }

        // test method : const FloatVector& FloatVector::operator=
        //                                (const FloatVector& v)
        {
            V v;
            v.addValue(11);
            v.addValue(22);
            v.addValue(33);
            V v1;
            V v2(v);
            v1 = v;
            test(v1 == v, __LINE__);
            v2 = v2; // auto affectation
            test(v2 == v, __LINE__);
        }


    } // local objects are deleted here

    return endTest();
}
//-------------------------------------------------------------------------
String TestFloatVector::getClassName() const
{
    return "TestFloatVector";
}
//-------------------------------------------------------------------------
TestFloatVector::~TestFloatVector() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFloatVector_cpp)

