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

#if !defined(ALIZE_TestFeature_cpp)
#define ALIZE_TestFeature_cpp

#include "TestFeature.h"
#include "Feature.h"
#include "Config.h"

using namespace alize;

//-------------------------------------------------------------------------
TestFeature::TestFeature()
:Test() {}
//-------------------------------------------------------------------------
bool TestFeature::exec()
{
  Test::exec();
  {
    Feature aa;
    test(aa._vectSize == 0, __LINE__);
    Feature f(128);
    Config config;
    unsigned long i;

    // TEST getClassName()
    test(f.getClassName() == "Feature", __LINE__);

    // test getVectSize()
    test(f.getVectSize() == 128, __LINE__);

    // test getDataVector()
    test(f.getDataVector() != NULL, __LINE__);

    // test methodes setValidity(bool) et isValid()
    test(f.isValid() == false, __LINE__);
    f.setValidity(true);
    test(f.isValid(), __LINE__);

    // test operator[]
    for (i=0; i<f.getVectSize(); i++)
    {
      f[i] = i+1;
      if (f[i] != i+1)
      {
        showError(__LINE__);
        break;
      }
    }

    try
    {
      f[129];
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}


    // test methodes getLabelCode() et setLabelCode(const Label&)
    test(f.getLabelCode() == 0, __LINE__);
    f.setLabelCode(1);
    test(f.getLabelCode() == 1, __LINE__);

    // test methode Feature(const Feature&)
    f.setValidity(true);
    Feature ff(f);
    test(ff.getVectSize() == f.getVectSize(), __LINE__);
    test(ff.getLabelCode() == f.getLabelCode(), __LINE__);
    test(ff.isValid() == true, __LINE__);
    f.setValidity(false);
    test(Feature(f).isValid() == false, __LINE__);

    for (i=0; i<f.getVectSize(); i++)
    {
      if (ff[i] != f[i])
      {
        showError(__LINE__);
        break;
      }
    }

    // test methode reset();
    f.setValidity(true);
    f.setLabelCode(2);
    f.reset();
    for (i=0; i<f.getVectSize(); i++)
    {
      if (f[i] != 0.0)
      {
        showError(__LINE__);
        break;
      }
    }
    test(f.getLabelCode() == 0, __LINE__);
    test(!f.isValid(), __LINE__);

    // test operator=(const Feature&)
    for (i=0; i<f.getVectSize(); i++)
      f[i] = (Feature::data_t)(i+1);
    f.setLabelCode(4);
    f.setValidity(true);

    Feature hh(150); // vectSize different de f.getVectSize()
    Feature h(f.getVectSize());
    try
    {
      hh = f;
      showError(__LINE__);
    } catch (Exception&) {}
    try
    {
      h = f;
    } catch (Exception&) { showError(__LINE__); }

    for (i=0; i<f.getVectSize(); i++)
      if (f[i] != h[i])
      {
        showError(__LINE__);
        break;
      }
    test(f.getLabelCode() == h.getLabelCode(), __LINE__);
    test(h.isValid(), __LINE__);
    f.setValidity(false);
    Feature m(f.getVectSize());
    m = f;
    test(!m.isValid(), __LINE__);

    // test method : bool operator==(const Feature&) const
    //       and bool operator!=(const Feature&) const
    {
      Feature f1(1); // vectSize = 1
      f1[0] = 1.2;
      Feature f2(2); // vectSize = 2
      f2[0] = 1.2;
      f2[0] = 1.2;
      test(!(f1 == f2), __LINE__); 
      test(f1 != f2, __LINE__);
      f2[0] = 3.4;
      Feature f3(f2);
      test(f3 == f2, __LINE__); 
      test(!(f3 != f2), __LINE__);
      f3[1] = 5.6;
      test(!(f3 == f2), __LINE__); 
      test(f3 != f2, __LINE__);
      f3[1] = 3.4;
      f3[0] = 7.8;
      test(!(f3 == f2), __LINE__); 
      test(f3 != f2, __LINE__);
    }
    // test method : void setVectSize(const K&, unsigned long s)
    {
      Feature f;
      f.setVectSize(K::k, 10);
      test(f.getVectSize() == 10, __LINE__);
      f.setVectSize(K::k, 0);
      test(f.getVectSize() == 0, __LINE__);
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestFeature::getClassName() const
{
  return "TestFeature";
}
//-------------------------------------------------------------------------
TestFeature::~TestFeature() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFeature_cpp)

