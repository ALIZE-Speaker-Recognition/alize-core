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

#if !defined(ALIZE_TestMixtureGD_cpp)
#define ALIZE_TestMixtureGD_cpp

#include "TestMixtureGD.h"
#include "MixtureGD.h"
#include "AutoDestructor.h"
#include "Exception.h"
#include "ULongVector.h"

using namespace alize;

//-------------------------------------------------------------------------
TestMixtureGD::TestMixtureGD()
:Test() {}
//-------------------------------------------------------------------------
bool TestMixtureGD::exec()
{
  Test::exec();
  {
    unsigned long i;

    // TEST getClassName()
    MixtureGD& m1 = MixtureGD::create(K::k, "un", 10);
    AutoDestructor ad1(m1);

    test(m1.getClassName() == "MixtureGD", __LINE__);
    
    // test construction
    if (m1.getId() != "un")
      showError(__LINE__);
    else
    {
      m1.setId(K::k, "aaaa");
      test(m1.getId() == "aaaa", __LINE__);
    }
    test(m1.getDistribCount() == 0, __LINE__);
    test(m1.getVectSize() == 10, __LINE__);
    test(m1.getTabDistrib() != NULL, __LINE__);

    // test addDistrib()
    // comme la mixture n'appartient a aucun serveur, ses distributions
    // seront automatiquement supprimee en meme temps que la mixture

    DistribGD& d0 = DistribGD::create(K::k, m1.getVectSize()+1);
    AutoDestructor jkjjjiji(d0);
    try
    {
      m1.addDistrib(K::k, d0);
      showError(__LINE__);
    }
    catch (Exception&) {}     
    
    DistribGD& d1 = DistribGD::create(K::k, m1.getVectSize());
    DistribGD& d2 = DistribGD::create(K::k, m1.getVectSize());
    m1.addDistrib(K::k, d1, 1.23);
    m1.addDistrib(K::k, static_cast<Distrib&>(d2), 4.55);
    test(d1._refCounter == 1, __LINE__);
    test(d2._refCounter == 1, __LINE__);

    // test getDistrib()
    test(m1.getDistrib(0).isSameObject(d1), __LINE__);
    test(m1.getDistrib(1).isSameObject(d2), __LINE__);
    test(m1.getDistribCount() == 2, __LINE__);

    // test Exception dans getDistrib()
    try
    {
      m1.getDistrib(0);
      m1.getDistrib(1);
    }
    catch (IndexOutOfBoundsException&) { showError(__LINE__); }
    try
    {
      m1.getDistrib(99);
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}
  
    // test weight(unsigned long index)
    test(m1.weight(0) == 1.23, __LINE__);
    test(m1.weight(1) == 4.55, __LINE__);
    m1.weight(0) = 1.11;
    test(m1.weight(0) == 1.11, __LINE__);

    // test setDistrib() et distrib.refCounter
    m1.addDistrib(K::k, d2, 1.2345);
    test(d2._refCounter == 2, __LINE__);
    m1.setDistrib(K::k, d1, 1);
    test(m1.getDistrib(1).isSameObject(d1), __LINE__);
    test(d1._refCounter == 2, __LINE__);
    test(d2._refCounter == 1, __LINE__);

    // test getTabWeight, getTabDistrib
    for (i=0; i<m1.getDistribCount(); i++)
    {
      if (m1.getTabWeight()[i] != m1.weight(i))
      {
        showError(__LINE__);
        break;
      }
      if (m1.getTabDistrib()[i] != &m1.getDistrib(i))
      {
        showError(__LINE__);
        break;
      }
    }

    // test creation mixture avec 5 distributions

    MixtureGD& mx = MixtureGD::create(K::k, "un", 10, 5);
    AutoDestructor admx(mx);
    test(mx.getDistribCount() == 5, __LINE__);
    for (i=0; i<mx.getDistribCount(); i++)
    {
      if (mx.getTabWeight()[i] != 1.0/5)
      {
        showError(__LINE__);
        break;
      }
      if (mx.getTabDistrib()[i] == NULL)
      {
        showError(__LINE__);
        break;
      }
    }

    // test methods : bool operator==(const Mixture&) const
    //        bool operator==(const MixtureGD&) const
    //        bool operator!=(const Mixture&) const
    //        bool operator!=(const MixtureGD&) const
    {
      {
        MixtureGD& m1 = MixtureGD::create(K::k, "", 5, 3);
        AutoDestructor adm1(m1);
        MixtureGD& m2 = MixtureGD::create(K::k, "", 5, 3);
        AutoDestructor adm2(m2);
        for (i=0; i<3; i++)
          m2.getDistrib(i) = m1.getDistrib(i);

        test(m1 == m2 , __LINE__);
        test(static_cast<Mixture&>(m1) == m2 , __LINE__);
        test(m1 == static_cast<Mixture&>(m2) , __LINE__);
        test(static_cast<Mixture&>(m1) == static_cast<Mixture&>(m2) , __LINE__);
        test(!(m1 != m2) , __LINE__);
        test(!(static_cast<Mixture&>(m1) != m2) , __LINE__);
        test(!(m1 != static_cast<Mixture&>(m2)) , __LINE__);
        test(!(static_cast<Mixture&>(m1) != static_cast<Mixture&>(m2)) , __LINE__);

        real_t covInv = m2.getDistrib(0).getCovInv(0);
        m2.getDistrib(0)._covInvVect[0] = 0.123456;
        test(m1 != m2 , __LINE__);
        test(!(m1 == m2) , __LINE__);
        m2.getDistrib(0)._covInvVect[0] = covInv;
        real_t mean = m2.getDistrib(0).getMean(0);
        m2.getDistrib(0).setMean(0.123456, 0);
        test(m1 != m2 , __LINE__);
        test(!(m1 == m2) , __LINE__);
        m2.getDistrib(0).setMean(mean, 0);
      }
      {
        MixtureGD& m1 = MixtureGD::create(K::k, "", 5, 3);
        AutoDestructor adm1(m1);
        MixtureGD& m2 = MixtureGD::create(K::k, "", 4, 3);
        AutoDestructor adm2(m2);
        test(m1 != m2 , __LINE__);
        test(!(m1 == m2) , __LINE__);
      }
      {
        MixtureGD& m1 = MixtureGD::create(K::k, "", 5, 3);
        AutoDestructor adm1(m1);
        MixtureGD& m2 = MixtureGD::create(K::k, "", 5, 4);
        AutoDestructor adm2(m2);
        test(m1 != m2 , __LINE__);
        test(!(m1 == m2) , __LINE__);
      }
    }
    
    // test duplicate
    ULongVector ref;
    for (i=0; i<m1.getDistribCount(); i++)
      ref.addValue(m1.getDistrib(i)._refCounter);
    MixtureGD& m2 = static_cast<MixtureGD&>(m1).duplicate(K::k, SHARE_DISTRIB);
    AutoDestructor fsdqvdsgdsfsd(m2);
    test(m1.getDistribCount() == m2.getDistribCount(), __LINE__);
    test(m1.getId() == m2.getId(), __LINE__);
    test(m1.getVectSize() == m2.getVectSize(), __LINE__);
    test(!m1.getTabWeight().isSameObject(m2.getTabWeight()), __LINE__);
    test(m1.getTabDistrib() != m2.getTabDistrib(), __LINE__);
    for (i=0; i<m2.getDistribCount(); i++)
    {
      test(m1.getDistrib(i).isSameObject(m2.getDistrib(i)), __LINE__);
      test(m2.getDistrib(i)._refCounter >= ref[i] + 1, __LINE__);
      test(m1.weight(i) == m2.weight(i), __LINE__);
    }
    // duplicate distrib too
    MixtureGD& m3 = static_cast<MixtureGD&>(m1).duplicate(K::k, DUPL_DISTRIB);
    AutoDestructor dfgdsfgfsdgs(m3);
    test(m1.getDistribCount() == m3.getDistribCount(), __LINE__);
    test(m1.getId() == m3.getId(), __LINE__);
    test(m1.getVectSize() == m3.getVectSize(), __LINE__);
    test(!m1.getTabWeight().isSameObject(m3.getTabWeight()), __LINE__);
    test(m1.getTabDistrib() != m3.getTabDistrib(), __LINE__);
    for (i=0; i<m3.getDistribCount(); i++)
    {
      test(!m1.getDistrib(i).isSameObject(m3.getDistrib(i)), __LINE__);
      test(m3.getDistrib(i)._refCounter == 1, __LINE__);
      test(m1.weight(i) == m3.weight(i), __LINE__);
    }

    // test operator=(const Mixture&)

    {
      MixtureGD& tt = MixtureGD::create(K::k, "", 1);
      AutoDestructor adm1(tt);
      tt = tt;
      static_cast<Mixture&>(tt) = tt;
      tt = static_cast<Mixture&>(tt);
      static_cast<Mixture&>(tt) = static_cast<Mixture&>(tt);
    }

    try
    {
      MixtureGD& tt = MixtureGD::create(K::k,
           "oijiji", m3.getVectSize()+1, m3.getDistribCount());
      AutoDestructor ad(tt);
      tt = m3;  // vectSize are different
      showError(__LINE__);
    }
    catch (Exception&) {}
    try
    {
      MixtureGD& tt = MixtureGD::create(K::k, "oijiji", m3.getVectSize(), m3.getDistribCount()+1);
      AutoDestructor ad(tt);
      tt = m3;  // distribCount are different
      showError(__LINE__);
    }
    catch (Exception&) {}
    MixtureGD& tt = MixtureGD::create(K::k, "oijiji", m3.getVectSize(), m3.getDistribCount());
    AutoDestructor ad(tt);
    tt = m3;
    for (i=0; i<m3.getDistribCount(); i++)
    {
      test(!tt.getDistrib(i).isSameObject(m3.getDistrib(i)), __LINE__);
      test(tt.weight(i) == m3.weight(i), __LINE__);
      test(m3.getDistrib(i) == tt.getDistrib(i), __LINE__);
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestMixtureGD::getClassName() const
{
  return "TestMixtureGD";
}
//-------------------------------------------------------------------------
TestMixtureGD::~TestMixtureGD() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestMixtureGD_cpp)
