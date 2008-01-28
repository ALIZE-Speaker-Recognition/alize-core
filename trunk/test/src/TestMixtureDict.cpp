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

#if !defined(ALIZE_TestMixtureDict_cpp)
#define ALIZE_TestMixtureDict_cpp

#include "TestMixtureDict.h"
#include "MixtureDict.h"
#include "MixtureGD.h"
#include "MixtureGF.h"
#include "Exception.h"

using namespace alize;

//-------------------------------------------------------------------------
TestMixtureDict::TestMixtureDict()
:Test() {}
//-------------------------------------------------------------------------
bool TestMixtureDict::exec()
{
  Test::exec();
  {
    // test method : virtual String getClassName() const;

    test(MixtureDict().getClassName() == "MixtureDict", __LINE__);

    // test method : MixtureDict()

    test(MixtureDict()._vect.size() == 0, __LINE__);

    // test method : unsigned long addMixture(Mixture&)
    {
      Mixture& m1 = MixtureGD::create(K::k, "", 1);
      Mixture& m2 = MixtureGF::create(K::k, "", 1);
      Mixture& m3 = MixtureGD::create(K::k, "", 1);
      MixtureDict v;
      v.addMixture(m1);
      v.addMixture(m2);
      v.addMixture(m3);
      test(v._vect.size() == 3, __LINE__);
      test(v._vect.getObject(0).isSameObject(m1), __LINE__);
      test(v._vect.getObject(1).isSameObject(m2), __LINE__);
      test(v._vect.getObject(2).isSameObject(m3), __LINE__);
    }

    // test method : Mixture& getMixture(unsigned long index) const
    {
      Mixture& m1 = MixtureGD::create(K::k, "", 1);
      Mixture& m2 = MixtureGD::create(K::k, "", 1);
      Mixture& m3 = MixtureGF::create(K::k, "", 1);
      MixtureDict v;
      v.addMixture(m1);
      v.addMixture(m2);
      v.addMixture(m3);
      try { v.getMixture(0); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      try { v.getMixture(3); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      test(v.getMixture(0).isSameObject(m1), __LINE__);
      test(v.getMixture(1).isSameObject(m2), __LINE__);
      test(v.getMixture(2).isSameObject(m3), __LINE__);
    }

    // test method : void clear();
    {
      Mixture& m1 = MixtureGD::create(K::k, "", 1);
      Mixture& m2 = MixtureGD::create(K::k, "", 1);
      Mixture& m3 = MixtureGF::create(K::k, "", 1);
      MixtureDict v;
      v.addMixture(m1);
      v.addMixture(m2);
      v.addMixture(m3);
      v.clear();
      test(v._vect.size() == 0, __LINE__);
    }

    // test method : unsigned long size() const;
    {
      MixtureDict v;
      test(v.size() == 0, __LINE__);
      Mixture& m1 = MixtureGF::create(K::k, "", 1);
      v.addMixture(m1);
      test(v.size() == 1, __LINE__);
      Mixture& m2 = MixtureGD::create(K::k, "", 1);
      v.addMixture(m2);
      test(v.size() == 2, __LINE__);
    }

    // test method : long getIndexOfId(const String& id) const
    {
      Mixture& m1 = MixtureGD::create(K::k, "a", 1);
      Mixture& m2 = MixtureGF::create(K::k, "b", 1);
      Mixture& m3 = MixtureGD::create(K::k, "c", 1);
      MixtureDict v;
      v.addMixture(m1);
      v.addMixture(m2);
      v.addMixture(m3);
      test(v.getIndexOfId("a") == 0, __LINE__);
      test(v.getIndexOfId("b") == 1, __LINE__);
      test(v.getIndexOfId("z") == -1, __LINE__);
    }
    // test method : void deleteMixtures(unsigned long first,
    //                   unsigned long last) 
    {
      {
        Mixture& m1 = MixtureGD::create(K::k, "1", 1);
        Mixture& m2 = MixtureGF::create(K::k, "2", 1);
        Mixture& m3 = MixtureGD::create(K::k, "3", 1);
        Mixture& m4 = MixtureGF::create(K::k, "4", 1);
        Mixture& m5 = MixtureGD::create(K::k, "5", 1);
        Mixture& m6 = MixtureGD::create(K::k, "6", 1);
        MixtureDict v;
        v.addMixture(m1);
        v.addMixture(m2);
        v.addMixture(m3);
        v.addMixture(m4);
        v.addMixture(m5);
        v.addMixture(m6);
        v.deleteMixtures(1, 3);
        test(v.size() == 3, __LINE__);
        test(v.getMixture(0).isSameObject(m1), __LINE__);
        test(v.getMixture(1).isSameObject(m5), __LINE__);
        test(v.getMixture(2).isSameObject(m6), __LINE__);
        test(v.getIndexOfId("1") == 0, __LINE__);
        test(v.getIndexOfId("2") < 0, __LINE__);
        test(v.getIndexOfId("3") < 0, __LINE__);
        test(v.getIndexOfId("4") < 0, __LINE__);
        test(v.getIndexOfId("5") == 1, __LINE__);
        test(v.getIndexOfId("6") == 2, __LINE__);
      }
      {
        Mixture& m1 = MixtureGD::create(K::k, "1", 1);
        Mixture& m2 = MixtureGF::create(K::k, "2", 1);
        Mixture& m3 = MixtureGD::create(K::k, "3", 1);
        Mixture& m4 = MixtureGF::create(K::k, "4", 1);
        Mixture& m5 = MixtureGD::create(K::k, "5", 1);
        MixtureDict v;
        v.addMixture(m1);
        v.addMixture(m2);
        v.addMixture(m3);
        v.addMixture(m4);
        v.addMixture(m5);
        v.deleteMixtures(1, 8);
        test(v.size() == 1, __LINE__);
        test(v.getMixture(0).isSameObject(m1), __LINE__);
        test(v.getIndexOfId("1") == 0, __LINE__);
        test(v.getIndexOfId("2") < 0, __LINE__);
        test(v.getIndexOfId("3") < 0, __LINE__);
        test(v.getIndexOfId("4") < 0, __LINE__);
        test(v.getIndexOfId("5") < 0, __LINE__);
      }
      {
        Mixture& m1 = MixtureGF::create(K::k, "1", 1);
        Mixture& m2 = MixtureGD::create(K::k, "2", 1);
        MixtureDict v;
        v.addMixture(m1);
        v.addMixture(m2);
        v.deleteMixtures(2, 1); // pas d'effet
        test(v.size() == 2, __LINE__);
        test(v.getMixture(0).isSameObject(m1), __LINE__);
        test(v.getMixture(1).isSameObject(m2), __LINE__);
        test(v.getIndexOfId("1") == 0, __LINE__);
        test(v.getIndexOfId("2") == 1, __LINE__);
      }
    }
    // test method : void deleteMixture(const Mixture&) 
    {
      Mixture& m1 = MixtureGD::create(K::k, "1", 1);
      Mixture& m2 = MixtureGF::create(K::k, "2", 1);
      Mixture& m3 = MixtureGD::create(K::k, "3", 1);
      Mixture& m4 = MixtureGF::create(K::k, "4", 1);
      Mixture& m5 = MixtureGD::create(K::k, "5", 1);
      MixtureDict v;
      v.addMixture(m1);
      v.addMixture(m2);
      v.addMixture(m3);
      v.addMixture(m4);
      v.addMixture(m5);

      v.deleteMixture(m5);
      test(v.size() == 4, __LINE__);
      test(v.getMixture(0).isSameObject(m1), __LINE__);
      test(v.getMixture(1).isSameObject(m2), __LINE__);
      test(v.getMixture(2).isSameObject(m3), __LINE__);
      test(v.getMixture(3).isSameObject(m4), __LINE__);

      v.deleteMixture(m2);
      test(v.size() == 3, __LINE__);
      test(v.getMixture(0).isSameObject(m1), __LINE__);
      test(v.getMixture(1).isSameObject(m3), __LINE__);
      test(v.getMixture(2).isSameObject(m4), __LINE__);

      v.deleteMixture(m1);
      test(v.size() == 2, __LINE__);
      test(v.getMixture(0).isSameObject(m3), __LINE__);
      test(v.getMixture(1).isSameObject(m4), __LINE__);

      v.deleteMixture(m3);
      v.deleteMixture(m4);
      test(v.size() == 0, __LINE__);

      // distribs are automatically destroyed with each mixture destruction
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestMixtureDict::getClassName() const
{ return "TestMixtureDict"; }
//-------------------------------------------------------------------------
TestMixtureDict::~TestMixtureDict() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestMixtureDict_cpp)
