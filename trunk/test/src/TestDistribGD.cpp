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

#if !defined(ALIZE_TestDistribGD_cpp)
#define ALIZE_TestDistribGD_cpp

#include "TestDistribGD.h"
#include "DistribGD.h"
#include "DistribGF.h"
#include "Config.h"
#include "alizeString.h"
#include "AutoDestructor.h"
#include "Feature.h"

using namespace alize;


//-------------------------------------------------------------------------
TestDistribGD::TestDistribGD()
:Test() {}
//-------------------------------------------------------------------------
bool TestDistribGD::exec()
{
  Test::exec();

  {
    unsigned long i;
    Config config;
    config.setParam("vectSize", "5");

    // TEST getClassName()
    test(DistribGD(config).getClassName() == "DistribGD", __LINE__);

    // TEST constructor

    DistribGD d1(config);

    test(d1.getVectSize() == 5,  __LINE__);
    //test(d1.getCst() == 0.0, __LINE__);
    
    // test setcov(0) -> remplace 0 par MIN_COV
    d1.setCov(0, 0);
    test(d1.getCov(0) == MIN_COV, __LINE__);
    // on force 0
    d1._covVect[0] = MIN_COV;
    d1.computeAll();
    // Cette ligne pose probleme avec gcc <FRANCAIS>
    // les 2 valeurs ne sont pas exactement egales !
    //test(d1._covInvVect[0] == 1.0/MIN_COV, __LINE__);
    
    // test debordements
    d1.setCov(0.0, 0);
    try
    {
      d1.getCov(0);
      d1.getCov(d1.getVectSize()-1);
      d1.getCovInv(0);
      d1.getCovInv(d1.getVectSize()-1);
      d1.getMean(0);
      d1.getMean(d1.getVectSize()-1);
      d1.setCov(d1.getCov(0), 0);
      d1.setMean(d1.getMean(0), 0);
    }
    catch (IndexOutOfBoundsException&) { showError(__LINE__); }
    try
    {
      d1.getCov(d1.getVectSize());
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}
    try
    {
      d1.getCovInv(d1.getVectSize());
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}
    try
    {
      d1.getMean(d1.getVectSize());
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}
    try
    {
      d1.setCov(12111211.0, d1.getVectSize());
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}
    try
    {
      d1.setMean(4415415.0, d1.getVectSize());
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}

    // test setCov, setMean, setCovInv
    for (i=0; i<d1.getVectSize(); i++)
    {
      d1.setCov((real_t)(i+1), i);
      d1.setMean((real_t)(i+2), i);
      d1.setCovInv(K::k, (real_t)(i+3), i);
      test(d1.getCov(i) == (real_t)(i+1), __LINE__);
      test(d1.getCovInv(i) == (real_t)(i+3), __LINE__);
    }

    // test computeAll()
    d1.computeAll();
    test(d1.getDet() == 120.0, __LINE__);
    for (i=0; i<d1.getVectSize(); i++)
    {
      if (d1.getCovInv(i) - 1.0/((real_t)(i+1)) >  0.00000000000001 ||
        d1.getCovInv(i) - 1.0/((real_t)(i+1)) < -0.00000000000001)
        showError(__LINE__);
    }
    if (d1.getCst() - 0.00092248583478473 >  0.00000000000000001 ||
      d1.getCst() - 0.00092248583478473 < -0.00000000000000001)
        showError(__LINE__);

    test(d1._refCounter == 0, __LINE__);
    d1.refCounter(K::k)++;
    d1.refCounter(K::k)++;
    test(d1._refCounter == 2, __LINE__);
    d1.refCounter(K::k)--;
    test(d1._refCounter == 1, __LINE__);
  
    if (d1.dictIndex(K::k) != 0)
      showError(__LINE__);
    else
    {
      d1.dictIndex(K::k) = 125;
      test(d1.dictIndex(K::k) == 125, __LINE__);
    }
    
    // test duplicate()

    DistribGD& d3 = d1.duplicate(K::k);
    AutoDestructor dwzaezaxdfz(d3);
    test(d3._refCounter == 0, __LINE__);
    test(d3.dictIndex(K::k) == 0, __LINE__);
    test(d3._refCounter == 0, __LINE__);
    test(d3.dictIndex(K::k) == 0, __LINE__);
    test(d3.getVectSize() == d1.getVectSize(), __LINE__);
    test(d3.getCst() == d1.getCst(), __LINE__);
    test(!d3.getMeanVect().isSameObject(d1.getMeanVect()), __LINE__);
    test(!d3.getCovVect().isSameObject(d1.getCovVect()), __LINE__);
    test(!d3.getCovInvVect().isSameObject(d1.getCovInvVect()), __LINE__);
    for (i=0; i<d3.getVectSize(); i++)
    {
      if (d3.getMean(i) != d1.getMean(i) )
      {
        showError(__LINE__);
        break;
      }
      if (d3._covVect.size() != 0)
        if (d3.getCov(i) != d1.getCov(i))
        {
          showError(__LINE__);
          break;
        }
      if (d3.getCovInv(i) != d1.getCovInv(i))
      {
        showError(__LINE__);
        break;
      }
    }
  
    test(d3.getDet() == d1.getDet(), __LINE__);

    // test operator=(const Distrib&)
    {
      DistribGD x(1), y(1);
      DistribGD xx(1);
      x = xx;
      static_cast<Distrib&>(x) = xx;
      x = static_cast<Distrib&>(xx);
      static_cast<Distrib&>(x) = static_cast<Distrib&>(xx);
      DistribGF yy(3);
      try { y = yy; showError(__LINE__); }
      catch (Exception&) { }
      try { static_cast<Distrib&>(y) = yy; showError(__LINE__); }
      catch (Exception&) { }
      try { y = static_cast<Distrib&>(yy); showError(__LINE__); }
      catch (Exception&) { }
      try { static_cast<Distrib&>(y) = static_cast<Distrib&>(yy); showError(__LINE__); }
      catch (Exception&) { }
    }

    try
    {
      DistribGD dd(5);
      dd = DistribGD(6); // vectSize different
      showError(__LINE__);
    }
    catch (Exception&) {}
    try
    {
      DistribGD d2(d1.getVectSize());
      unsigned long refCounter = d2._refCounter;
      unsigned long dictIndex = d2.dictIndex(K::k);
      d2 = d1; // vectSize identiques
      test(refCounter == d2._refCounter, __LINE__);
      test(dictIndex == d2.dictIndex(K::k), __LINE__);
      test(d1.getCst() == d2.getCst(), __LINE__);
      test(d1.getDet() == d2.getDet(), __LINE__);
      for (i=0; i<d1.getVectSize(); i++)
      {
        if (d1._covVect.size() != 0)
          test(d1.getCov(i) == d2.getCov(i), __LINE__);
        test(d1.getCovInv(i) == d2.getCovInv(i), __LINE__);
        test(d1.getMean(i) == d2.getMean(i), __LINE__);
      }

      // test operator== et !=
      DistribGF x(3);
      // TODO : c'est un peu leger comme test ...
      test(!(d2==x), __LINE__);
      test(!(x==d2), __LINE__);
      test(static_cast<DistribGD&>(d2)==static_cast<DistribGD&>(d2), __LINE__);
      test(!(static_cast<DistribGD&>(d2)!=static_cast<DistribGD&>(d2)), __LINE__);
      test(static_cast<DistribGD&>(d1)==static_cast<DistribGD&>(d2), __LINE__);
      test(!(static_cast<DistribGD&>(d1)!=static_cast<DistribGD&>(d2)), __LINE__);
      test(static_cast<DistribGD&>(d2)==static_cast<Distrib&>(d2), __LINE__);
      test(!(static_cast<DistribGD&>(d2)!=static_cast<Distrib&>(d2)), __LINE__);
      test(static_cast<DistribGD&>(d1)==static_cast<Distrib&>(d2), __LINE__);
      test(!(static_cast<DistribGD&>(d1)!=static_cast<Distrib&>(d2)), __LINE__);
      test(static_cast<Distrib&>(d2)==static_cast<DistribGD&>(d2), __LINE__);
      test(!(static_cast<Distrib&>(d2)!=static_cast<DistribGD&>(d2)), __LINE__);
      test(static_cast<Distrib&>(d1)==static_cast<DistribGD&>(d2), __LINE__);
      test(!(static_cast<Distrib&>(d1)!=static_cast<DistribGD&>(d2)), __LINE__);
      test(static_cast<Distrib&>(d2)==static_cast<Distrib&>(d2), __LINE__);
      test(!(static_cast<Distrib&>(d2)!=static_cast<Distrib&>(d2)), __LINE__);
      test(static_cast<Distrib&>(d1)==static_cast<Distrib&>(d2), __LINE__);
      test(!(static_cast<Distrib&>(d1)!=static_cast<Distrib&>(d2)), __LINE__);
    }
    catch (Exception&) { showError(__LINE__); }

    // test computeLK
    Feature feature(5);
    for (i=0; i<feature.getVectSize(); i++)
    { feature[i] = (Feature::data_t)(i+1); }
    lk_t lk = d1.computeLK(feature);
    // computeLK can throw Exception if something goes wrong
    if (lk - 0.000294537 >  0.000000001 ||
      lk - 0.000294537 < -0.000000001)
        showError(__LINE__);

    // test method : const DoubleVector& getMeanVect() const
    {
      DistribGD d(5);
      test(d._meanVect.isSameObject(d.getMeanVect()), __LINE__);
    }
    // test method : void setMeanVect(const DoubleVector&)
    {
      DoubleVector v;
      v.addValue(1);
      v.addValue(2);
      v.addValue(3);
      try
      {
        DistribGD(0).setMeanVect(v);
        DistribGD(1).setMeanVect(v);
        DistribGD(2).setMeanVect(v);
        DistribGD(4).setMeanVect(v);
        showError(__LINE__);
      }
      catch (Exception&) {}
      try
      {
        DistribGD d(3);
        d.setMean(0.0, 0);
        d.setMean(0.0, 1);
        d.setMean(0.0, 2);
        d.setMeanVect(v);
        test(v == d.getMeanVect(), __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestDistribGD::getClassName() const { return "TestDistribGD"; }
//-------------------------------------------------------------------------
TestDistribGD::~TestDistribGD() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestDistribGD_cpp)
