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

#if !defined(ALIZE_TestViterbiAccum_cpp)
#define ALIZE_TestViterbiAccum_cpp

#include "TestViterbiAccum.h"
#include "ViterbiAccum.h"
#include "Config.h"
#include "MixtureServer.h"
#include "StatServer.h"
#include "AutoDestructor.h"
#include "Feature.h"
#include <iostream>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestViterbiAccum::TestViterbiAccum()
:Test() {}
//-------------------------------------------------------------------------
bool TestViterbiAccum::exec()
{
  Test::exec();
  {
    Config config;
    config.setParam("vectSize", "32");
    config.setParam("minLLK", "-1e10");
    config.setParam("maxLLK", "1e10");
    MixtureServer ms(config);
    Mixture& mix1 = ms.createMixtureGD(10);
    Mixture& mix2 = ms.createMixtureGF(10);
    StatServer ss(config, ms);

    // test method : String getClassName() const
    {
      StatServer ss(config);
      ViterbiAccum v(ss, config);
      test(v.getClassName() == "ViterbiAccum", __LINE__);
    }

    // test method : ViterbiAccum()
    {
      StatServer ss(config);
      ViterbiAccum va(ss, config);
      test(va._stateVect.size() == 0, __LINE__);
      test(va._transMatrix.size() == 0, __LINE__);
    }

    // test method : static ViterbiAccum& create(StatServer& ss, const K&)
    {
      StatServer ss(config);
      ViterbiAccum& va = ViterbiAccum::create(ss, config, K::k);
      AutoDestructor aa(va);
      test(va._stateVect.size() == 0, __LINE__);
      test(va._transMatrix.size() == 0, __LINE__);
    }

    // test method : void addState(const Mixture& m)
    {
      StatServer ss(config);
      ViterbiAccum va(ss, config);
      va.addState(mix1);
      test(va._stateVect.size() == 1, __LINE__);
      test(va._stateVect.getObject(0).isSameObject(mix1), __LINE__);
      va.addState(mix2);
      test(va._stateVect.size() == 2, __LINE__);
      test(va._stateVect.getObject(0).isSameObject(mix1), __LINE__);
      test(va._stateVect.getObject(1).isSameObject(mix2), __LINE__);
    }

    // test method : unsigned long getStateCount() const
    {
      StatServer ss(config);
      ViterbiAccum va(ss, config);
      test(va.getStateCount() == 0, __LINE__);
      va.addState(mix1);
      test(va.getStateCount() == 1, __LINE__);
      va.addState(mix2);
      test(va.getStateCount() == 2, __LINE__);
    }

    // test method : real_t& logTransition(unsigned long i1,
    //                  unsigned long i2)
    {
      {
        StatServer ss(config);
        ViterbiAccum va(ss, config);
        try { va.logTransition(0,0) = 1.0; showError(__LINE__); }
        catch (Exception&) {}
        va.addState(mix1);
        try { va.logTransition(0,1) = 1.0; showError(__LINE__); }
        catch (Exception&) {}
        try { va.logTransition(1,0) = 1.0; showError(__LINE__); }
        catch (Exception&) {}
        try { va.logTransition(0,0) = 1.23; }
        catch (Exception&) { showError(__LINE__); }
        test(va._transMatrix[0*0+0] == 1.23, __LINE__);
      }
      /*{
        ViterbiAccum va(config);
        va.addState(mix1);
        va.addState(mix2);
        va.logTransition(0,0) = 1.1;
        va.logTransition(0,1) = 2.2;
        va.logTransition(1,0) = 3.3;
        va.logTransition(1,1) = 4.4;
        test(va._transMatrix[0*2+0] == 1.1, __LINE__);
        test(va._transMatrix[1*2+0] == 2.2, __LINE__);
        test(va._transMatrix[0*2+1] == 3.3, __LINE__);
        test(va._transMatrix[1*2+1] == 4.4, __LINE__);
      }*/
      {
        StatServer ss(config);
        ViterbiAccum va(ss, config);
        va.addState(mix1);
        va.addState(mix2);
        va.logTransition(0,0) = 1.1;
        va.logTransition(0,1) = 2.2;
        va.logTransition(1,0) = 3.3;
        va.logTransition(1,1) = 4.4;
        try { va.logTransition(2, 0); showError(__LINE__); }
        catch (Exception&) {}
        try { va.logTransition(0, 2); showError(__LINE__); }
        catch (Exception&) {}
        test(va.logTransition(0, 0) == 1.1, __LINE__);
        test(va.logTransition(0, 1) == 2.2, __LINE__);
        test(va.logTransition(1, 0) == 3.3, __LINE__);
        test(va.logTransition(1, 1) == 4.4, __LINE__);
        //std::cout << va.toString() << std::endl;
      }
    }

    // test method : Mixture& getState(unsigned long i) const
    {
      StatServer ss(config);
      ViterbiAccum va(ss, config);
      va.addState(mix1);
      va.addState(mix2);
      try { va.getState(2); showError(__LINE__); }
      catch (Exception&) {}
      try { test(va.getState(0).isSameObject(mix1), __LINE__); }
      catch (Exception&) { showError(__LINE__); }
      try { test(va.getState(1).isSameObject(mix2), __LINE__); }
      catch (Exception&) { showError(__LINE__); }
    }

    //--------------------------------------------------------------------

    // test method : reset(unsigned long nbFeatures)
    {
      StatServer ss(config);
      ViterbiAccum va(ss, config);
      va.addState(mix1);
      va.addState(mix2);
      va.reset();
      test(va._tmpTab.size() == 0, __LINE__);
      test(!va._llpDefined, __LINE__);
      test(!va._pathDefined, __LINE__);
      test(va._featureCount == 0, __LINE__);
      // ... to finish
    }

    // test method : void computeAndAccumulate(const Feature& f)
    // and  method : const ULongVector getPath()
    {
      { // test avec des log transition identiques
        Config config;
        config.setParam("vectSize", "2");
        config.setParam("minLLK", "-1e10");
        config.setParam("maxLLK", "1e10");
        MixtureServer ms(config);

        MixtureGD& mix0 = ms.createMixtureGD(2);
        mix0.getDistrib(0).setCov(1, 0);
        mix0.getDistrib(0).setCov(2, 1);
        mix0.getDistrib(0).setMean(3, 0);
        mix0.getDistrib(0).setMean(4, 1);
        mix0.getDistrib(1).setCov(1.5, 0);
        mix0.getDistrib(1).setCov(2.2, 1);
        mix0.getDistrib(1).setMean(2, 0);
        mix0.getDistrib(1).setMean(1, 1);
        mix0.computeAll();
        ms.setMixtureId(mix0, "state0");

        MixtureGD& mix1 = ms.createMixtureGD(2);
        mix1.getDistrib(0).setCov(2, 0);
        mix1.getDistrib(0).setCov(3, 1);
        mix1.getDistrib(0).setMean(1, 0);
        mix1.getDistrib(0).setMean(5, 1);
        mix1.getDistrib(1).setCov(2.5, 0);
        mix1.getDistrib(1).setCov(1.2, 1);
        mix1.getDistrib(1).setMean(3, 0);
        mix1.getDistrib(1).setMean(1, 1);
        mix1.computeAll();
        ms.setMixtureId(mix1, "state1");

        MixtureGD& mix2 = ms.createMixtureGD(2);
        mix2.getDistrib(0).setCov(3, 0);
        mix2.getDistrib(0).setCov(1.5, 1);
        mix2.getDistrib(0).setMean(2, 0);
        mix2.getDistrib(0).setMean(2, 1);
        mix2.getDistrib(1).setCov(0.5, 0);
        mix2.getDistrib(1).setCov(1.2, 1);
        mix2.getDistrib(1).setMean(3, 0);
        mix2.getDistrib(1).setMean(2, 1);
        mix2.computeAll();
        ms.setMixtureId(mix2, "state2");

        StatServer ss(config, ms);
      
        Feature f0(2);
        f0[0] = 1;
        f0[1] = 5;
        Feature f1(2);
        f1[0] = 2;
        f1[1] = 6;
        Feature f2(2);
        f2[0] = 3;
        f2[1] = 4;
        Feature f3(2);
        f3[0] = 1;
        f3[1] = 3;

        StatServer ss2(config);
        ViterbiAccum va(ss2, config);
      
        va.addState(mix0);
        va.addState(mix1);
        va.addState(mix2);
        va.logTransition(0, 0) = -1.1;
        va.logTransition(0, 1) = -1.1;
        va.logTransition(0, 2) = -1.1;
        va.logTransition(1, 0) = -1.1;
        va.logTransition(1, 1) = -1.1;
        va.logTransition(1, 2) = -1.1;
        va.logTransition(2, 0) = -1.1;
        va.logTransition(2, 1) = -1.1;
        va.logTransition(2, 2) = -1.1;

        va.reset();
        va.computeAndAccumulate(f0);
        va.computeAndAccumulate(f1);
        va.computeAndAccumulate(f2);
        va.computeAndAccumulate(f3);
        ULongVector v;
        v.addValue(1).addValue(1).addValue(0).addValue(2);
        test(va.getPath() == v, __LINE__);
      }
      {
        Config config;
        config.setParam("vectSize", "2");
        config.setParam("minLLK", "-1e10");
        config.setParam("maxLLK", "1e10");
        MixtureServer ms(config);

        MixtureGD& mix0 = ms.createMixtureGD(2);
        mix0.getDistrib(0).setCov(1, 0);
        mix0.getDistrib(0).setCov(2, 1);
        mix0.getDistrib(0).setMean(3, 0);
        mix0.getDistrib(0).setMean(4, 1);
        mix0.getDistrib(1).setCov(1.5, 0);
        mix0.getDistrib(1).setCov(2.2, 1);
        mix0.getDistrib(1).setMean(2, 0);
        mix0.getDistrib(1).setMean(1, 1);
        mix0.computeAll();
        ms.setMixtureId(mix0, "state0");

        MixtureGD& mix1 = ms.createMixtureGD(2);
        mix1.getDistrib(0).setCov(2, 0);
        mix1.getDistrib(0).setCov(3, 1);
        mix1.getDistrib(0).setMean(1, 0);
        mix1.getDistrib(0).setMean(5, 1);
        mix1.getDistrib(1).setCov(2.5, 0);
        mix1.getDistrib(1).setCov(1.2, 1);
        mix1.getDistrib(1).setMean(3, 0);
        mix1.getDistrib(1).setMean(1, 1);
        mix1.computeAll();
        ms.setMixtureId(mix1, "state1");

        MixtureGD& mix2 = ms.createMixtureGD(2);
        mix2.getDistrib(0).setCov(3, 0);
        mix2.getDistrib(0).setCov(1.5, 1);
        mix2.getDistrib(0).setMean(2, 0);
        mix2.getDistrib(0).setMean(2, 1);
        mix2.getDistrib(1).setCov(0.5, 0);
        mix2.getDistrib(1).setCov(1.2, 1);
        mix2.getDistrib(1).setMean(3, 0);
        mix2.getDistrib(1).setMean(2, 1);
        mix2.computeAll();
        ms.setMixtureId(mix2, "state2");

        StatServer ss(config, ms);
      
        Feature f0(2);
        f0[0] = 1;
        f0[1] = 5;
        Feature f1(2);
        f1[0] = 2;
        f1[1] = 6;
        Feature f2(2);
        f2[0] = 3;
        f2[1] = 4;
        Feature f3(2);
        f3[0] = 1;
        f3[1] = 3;

        StatServer ss3(config);
        ViterbiAccum va(ss3, config);
      
        va.addState(mix0);
        va.addState(mix1);
        va.addState(mix2);
        va.logTransition(0, 0) = -1.1;
        va.logTransition(0, 1) = -1.1;
        va.logTransition(0, 2) = -1.1;
        va.logTransition(1, 0) = -1.1;
        va.logTransition(1, 1) = -1.1;
        va.logTransition(1, 2) = 5;//-1.1;
        va.logTransition(2, 0) = -1.1;
        va.logTransition(2, 1) = -1.1;
        va.logTransition(2, 2) = 5;//-1.1;

        va.reset();
        va.computeAndAccumulate(f0);
        va.computeAndAccumulate(f1);
        va.computeAndAccumulate(f2);
        va.computeAndAccumulate(f3);
        ULongVector v;
        v.addValue(1).addValue(2).addValue(2).addValue(2);
        test(va.getPath() == v, __LINE__);
        //cout << va.toString() << endl;
      }
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestViterbiAccum::getClassName() const { return "TestViterbiAccum"; }
//-------------------------------------------------------------------------
TestViterbiAccum::~TestViterbiAccum() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestViterbiAccum_cpp)
