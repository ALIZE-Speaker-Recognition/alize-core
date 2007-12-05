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

#if !defined(ALIZE_TestStatServer_cpp)
#define ALIZE_TestStatServer_cpp

#include <cmath>
#include <iostream>

#include "TestStatServer.h"
#include "StatServer.h"
#include "MixtureServer.h"
#include "Config.h"
#include "Feature.h"
#include "FrameAcc.h"
#include "FrameAccGD.h"
#include "FrameAccGF.h"
#include "MixtureGDStat.h"
#include "MixtureGFStat.h"

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestStatServer::TestStatServer()
:Test() {}
//-------------------------------------------------------------------------
bool TestStatServer::exec()
{
  Test::exec();
  {
  Config config;
  config.setParam("vectSize", "32");
  config.setParam("minLLK", "-100");
  config.setParam("maxLLK", "100");
  MixtureServer ms(config);
  //
  Mixture& world  = ms.createMixtureGD(5);
  Mixture& speaker1 = ms.createMixtureGD(10);
  //
  StatServer server(config, ms);

  // TEST getClassName()
  test(server.getClassName() == "StatServer", __LINE__);

  Feature feature(32);

  server.reset();
  server.resetLLK(world);
  test(server._mixtureStatVect.size() == 1, __LINE__);
  server.resetLLK(speaker1);
  test(server._mixtureStatVect.size() == 2, __LINE__);

  try
  {
    server.computeAllDistribLK(Feature(31));
    showError(__LINE__);
  }
  catch (Exception&) { }

  try { server.computeAllDistribLK(feature); }
  catch (Exception&) { showError(__LINE__); }

  try
  {
    server.computeAndAccumulateLLK(world, feature);
    server.computeAndAccumulateLLK(world);
    server.getMeanLLK(world);
  }
  catch (Exception&) {}
  
  server.getLLK(world);

  // occ
  
  server.resetOcc(world);
  server.computeAndAccumulateOcc(world, feature);
  
  try { server.getMeanOccVect(world); }
  catch (Exception&) {}
  
  // EM

  server.resetEM(world);
  try { server.computeAndAccumulateEM(world, feature); }
  catch (Exception&) {}

  server.getEM(world);


  // test method : ViterbiAccum& createViterbiAccum()   28 mai 2004
  {
    Config config;
    config.setParam("vectSize", "32");
    config.setParam("minLLK", "-100");
    config.setParam("maxLLK", "100");
    MixtureServer ms(config);
    StatServer ss(config, ms);

    ViterbiAccum& va  = ss.createViterbiAccum();
    ViterbiAccum& va2 = ss.createViterbiAccum();
    test(ss._viterbiAccumVect.size() == 2, __LINE__);
    test(ss._viterbiAccumVect.getObject(0).isSameObject(va), __LINE__);
    test(ss._viterbiAccumVect.getObject(1).isSameObject(va2), __LINE__);
  }

  // test method : FrameAccGD& createFrameAccGD()
  {
    FrameAccGD a  = StatServer::createFrameAccGD();
    FrameAccGF a2 = StatServer::createFrameAccGF();
  }




  // TODO : faire des test serieux !


  /*{
    Config c;
    c.setParam("vectSize", "1");
    c.setParam("minLLK", "-200");
    c.setParam("maxLLK", "200");
    MixtureServer ms(c);
    MixtureGD& m = ms.createMixtureGD(1);
    m.getDistrib(0).setCov(1.0, 0);
    m.getDistrib(0).setMean(0.0, 0);
    m.getDistrib(0).computeAll();
    m.weight(0) = 1.0;

    Feature f1(1);
    f1[0] = 0.1;
    Feature f2(1);
    f2[0] = 0.3;

    StatServer ss(c, ms);

    ss.resetEM(m);
    for (int i=0; i<20; i++)
    {
    ss.computeAndAccumulateEM(m, f1);
    ss.computeAndAccumulateEM(m, f2);
    m = ss.getEM(m);
    cout << m.getDistrib(0).toString() << endl;  
    }
  }*/

    // test method : const LKVector& getTopDistribIndexVector() const
    {
      Config c;
      c.setParam("minLLK", "-100");
      c.setParam("maxLLK", "100");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      const LKVector& v = ss.getTopDistribIndexVector();
      test(&v == &ss._topDistribsVect, __LINE__);
    }

    // test method : void accumulateLLK(lk_t v, double a)
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      MixtureGD& m = ms.createMixtureGD(10);
      ss.resetLLK(m);
      double l0 = 1.23;
      ss.accumulateLLK(m, l0);
      test(ss.getMeanLLK(m) == l0, __LINE__);
      double l1 = 6.78;
      ss.accumulateLLK(m, l1);
      test(fabs(ss.getMeanLLK(m)-(l0+l1)/2.0)<1e-15, __LINE__);

      ss.resetLLK(m);
      double l2 = 1.23;
      double w2 = 0.8;
      ss.accumulateLLK(m, l2, w2);
      test(fabs(ss.getMeanLLK(m)-l2*w2/w2)<1e-15, __LINE__);
      double l3 = 6.78;
      double w3 = 0.1;
      ss.accumulateLLK(m, l3, w3);
      test(fabs(ss.getMeanLLK(m)-(l2*w2+l3*w3)/(w2+w3))<1e-15, __LINE__);
    }
    // test method llk_t computeAndAccumulateLLK(const Mixture& m,
    //                   const Feature& f, const TopDistribsAction& a)
    {
      // TODO


    }
    // test method llk_t computeAndAccumulateLLK(const Mixture& m,
    //                   const Feature& f, double w, const TopDistribsAction& a)
    // just to test parameter w
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      MixtureGD& m = ms.createMixtureGD(10);
      Feature f(m.getVectSize());
      Feature f2(m.getVectSize());
      for (unsigned long i=0; i<f.getVectSize(); i++)
      {
        f[i] = i;
        f2[i] = i*2;
        for (unsigned long c=0; c<m.getDistribCount(); c++)
        {
          m.getDistrib(c).setCov((double)i, i);
          m.getDistrib(c).setMean((double)i, i);
        }
      }
      m.computeAll();
      ss.resetLLK(m);
      ss.computeAndAccumulateLLK(m, f); // without w
      ss.resetLLK(m);
      double llk2 = ss.computeAndAccumulateLLK(m, f, 0.8); // with w
      double llk3 = ss.computeAndAccumulateLLK(m, f2, 0.7); // with w
      test(fabs(ss.getMeanLLK(m)-(llk2*0.8+llk3*0.7)/(0.7+0.8))<1e-14, __LINE__);
    }
    // test method : MixtureStat& createAndStoreMixtureStat(const Mixture&)
    // 7 juin 2005
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      MixtureGD& m = ms.createMixtureGD(10);
      MixtureGF& m2 = ms.createMixtureGF(10);

      MixtureStat& acc = ss.createAndStoreMixtureStat(m);
      test(ss._mixtureStatVect.size() == 1, __LINE__);
      test(ss._mixtureStatVect.getObject(0).isSameObject(acc), __LINE__);
      test(acc.getClassName() == "MixtureGDStat", __LINE__);

      MixtureStat& acc2 = ss.createAndStoreMixtureStat(m2);
      test(ss._mixtureStatVect.size() == 2, __LINE__);
      test(ss._mixtureStatVect.getObject(0).isSameObject(acc), __LINE__);
      test(ss._mixtureStatVect.getObject(1).isSameObject(acc2), __LINE__);
      test(acc2.getClassName() == "MixtureGFStat", __LINE__);
    }
    // test method : MixtureGDStat& createAndStoreMixtureStat(MixtureGD& m)
    // 8 juin 2005
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      MixtureGD& m = ms.createMixtureGD(10);

      MixtureStat& acc = ss.createAndStoreMixtureStat(m);
      test(ss._mixtureStatVect.size() == 1, __LINE__);
      test(ss._mixtureStatVect.getObject(0).isSameObject(acc), __LINE__);
      test(acc.getClassName() == "MixtureGDStat", __LINE__);
    }
    // test method : MixtureGFStat& createAndStoreMixtureStat(MixtureGF& m)
    // 8 juin 2005
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      MixtureGF& m = ms.createMixtureGF(10);

      MixtureStat& acc = ss.createAndStoreMixtureStat(m);
      test(ss._mixtureStatVect.size() == 1, __LINE__);
      test(ss._mixtureStatVect.getObject(0).isSameObject(acc), __LINE__);
      test(acc.getClassName() == "MixtureGFStat", __LINE__);
    }
    // test method : MixtureGDStat& createAndStoreMixtureGDStat(Mixture& m)
    // 8 juin 2005
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      Mixture& m = ms.createMixtureGD(10);

      try
      {
        MixtureStat& acc = ss.createAndStoreMixtureGDStat(m);
        test(ss._mixtureStatVect.size() == 1, __LINE__);
        test(ss._mixtureStatVect.getObject(0).isSameObject(acc), __LINE__);
        test(acc.getClassName() == "MixtureGDStat", __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
      try
      {
        ss.createAndStoreMixtureGDStat(ms.createMixtureGF(5));
        showError(__LINE__);
      }
      catch (Exception&) {  }
    }
    // test method : MixtureGFStat& createAndStoreMixtureGFStat(Mixture& m)
    // 8 juin 2005
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      Mixture& m = ms.createMixtureGF(10);

      try
      {
        MixtureStat& acc = ss.createAndStoreMixtureGFStat(m);
        test(ss._mixtureStatVect.size() == 1, __LINE__);
        test(ss._mixtureStatVect.getObject(0).isSameObject(acc), __LINE__);
        test(acc.getClassName() == "MixtureGFStat", __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
      try
      {
        ss.createAndStoreMixtureGFStat(ms.createMixtureGD(5));
        showError(__LINE__);
      }
      catch (Exception&) {  }
    }

    // test method : unsigned long getMixtureStatCount() const
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      test(ss.getMixtureStatCount() == 0, __LINE__);
      ss.createAndStoreMixtureStat(ms.createMixtureGF(10));
      test(ss.getMixtureStatCount() == 1, __LINE__);
      ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      test(ss.getMixtureStatCount() == 2, __LINE__);
    }
    // test method : MixtureStat& getMixtureStat(unsigned long idx)
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      ss.createAndStoreMixtureStat(ms.createMixtureGF(10));
      ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      test(ss.getMixtureStat(0).getMixture().isSameObject(ms.getMixture(0)), __LINE__);
      test(ss.getMixtureStat(1).getMixture().isSameObject(ms.getMixture(1)), __LINE__);
    }
    // test method : MixtureGDStat& getMixtureGDStat(unsigned long idx)
    // test method : MixtureGFStat& getMixtureGFStat(unsigned long idx)
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      ss.createAndStoreMixtureStat(ms.createMixtureGF(10));
      ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      try
      {
        test(ss.getMixtureGDStat(1).getMixture().isSameObject(ms.getMixture(1)), __LINE__);
        test(ss.getMixtureGFStat(0).getMixture().isSameObject(ms.getMixture(0)), __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
      try
      {
        ss.getMixtureGDStat(0);
        showError(__LINE__);
      }
      catch (Exception&) {  }
      try
      {
        ss.getMixtureGFStat(1);
        showError(__LINE__);
      }
      catch (Exception&) {  }
    }
    // test method : void deleteMixtureStat(const MixtureStat& m)
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      MixtureStat& acc0 = ss.createAndStoreMixtureStat(ms.createMixtureGF(10));
      MixtureStat& acc1 = ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      try
      {
        ss.deleteMixtureStat(acc0);
        test(ss.getMixtureStatCount() == 1, __LINE__);
        test(ss.getMixtureStat(0).isSameObject(acc1), __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
    }
    // test method : void deleteMixtureStat(unsigned long b, unsigned long e)
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      MixtureStat& acc0 = ss.createAndStoreMixtureStat(ms.createMixtureGF(10));
      ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      MixtureStat& acc3 = ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      try
      {
        ss.deleteMixtureStat(1,2);
        test(ss.getMixtureStatCount() == 2, __LINE__);
        test(ss.getMixtureStat(0).isSameObject(acc0), __LINE__);
        test(ss.getMixtureStat(1).isSameObject(acc3), __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
    }
    // test method : void deleteAllMixtureStat()
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      ss.createAndStoreMixtureStat(ms.createMixtureGF(10));
      ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      try
      {
        ss.deleteAllMixtureStat();
        test(ss.getMixtureStatCount() == 0, __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
    }
    // test method : unsigned long getMixtureStatIndex(const MixtureStat& m)
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      StatServer ss(c, ms);
      MixtureStat& s0 = ss.createAndStoreMixtureStat(ms.createMixtureGF(10));
      MixtureStat& s1 = ss.createAndStoreMixtureStat(ms.createMixtureGF(10));
      MixtureStat& s2 = ss.createAndStoreMixtureStat(ms.createMixtureGD(10));
      ss.deleteMixtureStat(s1);
      try 
      {
        test(ss.getMixtureStatIndex(s0) == 0, __LINE__);
        test(ss.getMixtureStatIndex(s2) == 1, __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
      try 
      {
        ss.getMixtureStatIndex(s1);
        showError(__LINE__);
      }
      catch (Exception&) {}
    }

        // test method lk_t computeLLK(const Feature& f)
    {
      unsigned long c, i;
      config.setParam("vectSize", "32");
      config.setParam("minLLK", "-300");
      config.setParam("maxLLK", "300");
      MixtureServer ms(config);
      MixtureGD& m = ms.createMixtureGD(5);
      StatServer ss(config);
      for (c=0; c<m.getDistribCount(); c++)
      {
        DistribGD& d = m.getDistrib(c);
        for (i=0; i<d.getVectSize(); i++)
        {
          d.setCov((real_t)(i+c+1), i);
          d.setMean((real_t)(i+c+2), i);
        }
      }
      m.computeAll();
      m.equalizeWeights();
      Feature f(32);
      for (i=0; i<f.getVectSize(); i++)
        f[i] = (Feature::data_t)(i+1);

      lk_t llk = ss.computeLLK(m, f);
      lk_t llk2 =ss.computeLLK(m, f, 0);
      test(fabs(llk - (-73.820950855469)) <  1e-12, __LINE__);
      test(fabs(llk2 - (-72.177173580491)) <  1e-12, __LINE__);
    }
    // test method lk_t computeLLK(const Feature& f, unsigned long idx)
    {
      unsigned long c, i;
      Config config;
      config.setParam("vectSize", "1");
      config.setParam("minLLK", "-300");
      config.setParam("maxLLK", "300");
      MixtureServer ms(config);
      MixtureGD& m = ms.createMixtureGD(5);
      StatServer ss(config);
      for (c=0; c<m.getDistribCount(); c++)
      {
        DistribGD& d = m.getDistrib(c);
        for (i=0; i<d.getVectSize(); i++)
        {
          d.setCov((real_t)(i+c+1), i);
          d.setMean((real_t)(i+c+2), i);
        }
      }
      m.computeAll();
      m.equalizeWeights();
      Feature f(1);
      for (i=0; i<f.getVectSize(); i++)
        f[i] = (Feature::data_t)(i+1);

      lk_t llk = ss.computeLLK(m, f);
      try
      {
        ss.computeLLK(m, f, 1); // idx 1 > vectSize !
        showError(__LINE__);
      }
      catch (IndexOutOfBoundsException&) {}
      try
      {
        lk_t llk2 = ss.computeLLK(m, f, 0);
        test(fabs(llk-llk2) < 1e-15, __LINE__);
      }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
    }


  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestStatServer::getClassName() const { return "TestStatServer"; }
//-------------------------------------------------------------------------
TestStatServer::~TestStatServer() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestStatServer_cpp)
