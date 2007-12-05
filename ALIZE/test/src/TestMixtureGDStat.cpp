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

#if !defined(ALIZE_TestMixtureGDStat_cpp)
#define ALIZE_TestMixtureGDStat_cpp

#include <cmath>
#include "TestMixtureGDStat.h"
#include "MixtureGDStat.h"
#include "MixtureServer.h"
#include "LKVector.h"
#include "Feature.h"
#include "MixtureGD.h"
#include "AutoDestructor.h"
#include "Config.h"
#include <iostream>
#include "StatServer.h"

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestMixtureGDStat::TestMixtureGDStat()
:Test() {}
//-------------------------------------------------------------------------
bool TestMixtureGDStat::exec()
{
  Test::exec();
  {
    { // 12/02/2004
      Feature f(1);
      f[0] = 10;
      MixtureGD& mix = MixtureGD::create(K::k, "", 1, 1);
      AutoDestructor ad(mix);
      mix.getDistrib(0).setCov(3.0, 0);
      mix.getDistrib(0).setMean(5.0, 0);
      mix.weight(0) = 1.0;
      mix.computeAll();
      Config config;
      config.setParam("minLLK", "-300");
      config.setParam("maxLLK", "300");
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, mix, config);
      mixStat.resetEM();
      occ_t sum = mixStat.computeAndAccumulateEM(f);
      test(fabs(sum - 0.0035709938084786) <  1e-11, __LINE__);
      mix = mixStat.getEM();
    }

    unsigned long i, c;
    MixtureGD& mix = MixtureGD::create(K::k, "un", 32, 5);
    AutoDestructor ad(mix);

    Config config;
    config.setParam("minLLK", "-300");
    config.setParam("maxLLK", "300");
    config.setParam("topDistribsCount", "64");
    StatServer ss(config);
    MixtureGDStat mixStat(K::k, ss, mix, config);
    test(mixStat.getClassName() == "MixtureGDStat", __LINE__);

    test(mixStat.getMixture().isSameObject(mix), __LINE__);

    mixStat.resetLLK();
    test(mixStat.getLLK() == 0.0, __LINE__);
    test(mixStat.getAccumulatedLLK() == 0.0, __LINE__);

    try
    {
      mixStat.getMeanLLK();
      showError(__LINE__);
    }
    catch (Exception&) { /* si featureCounter == 0 */}

    mixStat.resetOcc();
    DoubleVector& occVect = mixStat.getOccVect();
    DoubleVector& accOccVect = mixStat.getAccumulatedOccVect();
    for (i=0; i<mixStat.getMixture().getDistribCount(); i++)
    {
      /*if (occVect[i] != 0.0)
      {
        showError(__LINE__);
        break;
      }*/
      if (accOccVect[i] != 0.0)
      {
        showError(__LINE__);
        break;
      }
    }
    try
    {
      mixStat.getMeanOccVect();
      showError(__LINE__);
    }
    catch (Exception&) { /* if featureCounter == 0 */}
    
    //

    mix.equalizeWeights();
    for (c=0; c<mixStat.getMixture().getDistribCount(); c++)
    {
      DistribGD& d = mix.getDistrib(c);
      for (i=0; i<d.getVectSize(); i++)
      {
        d.setCov((real_t)(i+c+1), i);
        d.setMean((real_t)(i+c+2), i);
      }
    }
    mix.computeAll();

    Feature feature(32);
    for (i=0; i<feature.getVectSize(); i++)
      feature[i] = (Feature::data_t)(i+1);
    //LKVector lkVect;
    lk_t lk0 = mixStat.computeAndAccumulateLLK(feature, 1.0f,
            TOP_DISTRIBS_NO_ACTION);
    test(fabs(lk0 - (-73.820950855469)) <  1e-12, __LINE__);
    lk_t lk = mixStat.getAccumulatedLLK();
    test(fabs(lk - (-73.820950855469)) <  1e-12, __LINE__);
    test(lk == mixStat.getLLK(), __LINE__);

    try
    {
      test(fabs(mixStat.getMeanLLK()
            - (-73.820950855469)) <  1e-12, __LINE__);
    }
    catch (Exception&) { showError(__LINE__); }
    mixStat.computeAndAccumulateLLK(feature, 1.0f,
        TOP_DISTRIBS_NO_ACTION); // twice
    try
    {
      test(fabs(mixStat.getMeanLLK()
            - (-73.820950855469)) <  1e-12, __LINE__);
    }
    catch (Exception&) { showError(__LINE__); }
    test(lk*2 == mixStat.getAccumulatedLLK(), __LINE__);

    mixStat.computeAndAccumulateOcc(feature);
    
    test(fabs(occVect[0] - 0.99725632548375  ) <  1e-14, __LINE__);
    test(fabs(occVect[1] - 0.0027412258180277  ) <  1e-16, __LINE__);
    test(fabs(occVect[2] - 2.4478382806130e-006) <  1e-14, __LINE__);
    test(fabs(occVect[3] - 8.5980725644553e-010) <  1e-14, __LINE__);
    test(fabs(occVect[4] - 1.3881090707711e-013) <  1e-14, __LINE__);

    test(fabs(accOccVect[0] - 0.99725632548375  ) <  1e-14, __LINE__);
    test(fabs(accOccVect[1] - 0.0027412258180277  ) <  1e-16, __LINE__);
    test(fabs(accOccVect[2] - 2.4478382806130e-006) <  1e-14, __LINE__);
    test(fabs(accOccVect[3] - 8.5980725644553e-010) <  1e-14, __LINE__);
    test(fabs(accOccVect[4] - 1.3881090707711e-013) <  1e-14, __LINE__);

    DoubleVector& meanOccVect = mixStat.getMeanOccVect();
    test(fabs(meanOccVect[0] - 0.99725632548375  ) <  1e-14, __LINE__);
    test(fabs(meanOccVect[1] - 0.0027412258180277  ) <  1e-16, __LINE__);
    test(fabs(meanOccVect[2] - 2.4478382806130e-006) <  1e-14, __LINE__);
    test(fabs(meanOccVect[3] - 8.5980725644553e-010) <  1e-14, __LINE__);
    test(fabs(meanOccVect[4] - 1.3881090707711e-013) <  1e-14, __LINE__);

    mixStat.computeAndAccumulateOcc(feature); // twice
    
    test(fabs(accOccVect[0] - 0.99725632548375  *2) <  1e-14, __LINE__);
    test(fabs(accOccVect[1] - 0.0027412258180277  *2) <  1e-16, __LINE__);
    test(fabs(accOccVect[2] - 2.4478382806130e-006*2) <  1e-14, __LINE__);
    test(fabs(accOccVect[3] - 8.5980725644553e-010*2) <  1e-14, __LINE__);
    test(fabs(accOccVect[4] - 1.3881090707711e-013*2) <  1e-14, __LINE__);

    test(fabs(meanOccVect[0] - 0.99725632548375  ) <  1e-14, __LINE__);
    test(fabs(meanOccVect[1] - 0.0027412258180277  ) <  1e-16, __LINE__);
    test(fabs(meanOccVect[2] - 2.4478382806130e-006) <  1e-14, __LINE__);
    test(fabs(meanOccVect[3] - 8.5980725644553e-010) <  1e-14, __LINE__);
    test(fabs(meanOccVect[4] - 1.3881090707711e-013) <  1e-14, __LINE__);

    
    // test EM simplifie : 1 iteration
    mixStat.resetEM();
    //for (i=0; i<100; i++)
    for (i=0; i<1; i++)
      mixStat.computeAndAccumulateEM(feature); // always the same feature !
      // TODO : faire un test plus complet
    
    mixStat.getEM();

    // TODO : tester avec distribLKVect

    // ----------------------------------------------------------

    // test method Mixture& getMixture();
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      MixtureGD& m = ms.createMixtureGD(10);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, c);
      test(mixStat.getMixture().isSameObject(m), __LINE__);
    }
    // test method void resetLLK();
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      MixtureGD& m = ms.createMixtureGD(10);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, c);
      Feature f(c);
      mixStat.computeAndAccumulateLLK(f);
      test(mixStat._featureCounterForAccumulatedLK != 0, __LINE__);
      mixStat._accumulatedLLK = 1.2f;
      mixStat.resetLLK();
      test(mixStat._featureCounterForAccumulatedLK == 0, __LINE__);
      test(mixStat._accumulatedLLK == 0.0, __LINE__);
    }
    // test method : double getLLK() const
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      MixtureGD& m = ms.createMixtureGD(10);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, c);
      mixStat._llk = 1.2345f;
      test(mixStat.getLLK() == mixStat._llk, __LINE__);
    }
    // test method : void accumulateLLK(lk_t v, double a)
    //         and : double getAccumulatedLLK()
    //         and : double getMeanLLK()
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      MixtureGD& m = ms.createMixtureGD(10);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, c);
      mixStat._accumulatedLLK = 5.0f;
      test(mixStat.getAccumulatedLLK() == mixStat._accumulatedLLK, __LINE__);
      mixStat.accumulateLLK(2.0f);
      test(mixStat.getAccumulatedLLK() == 7.0f, __LINE__);

      mixStat.resetLLK();
      test(mixStat.getAccumulatedLLK() == 0, __LINE__);
      double l0 = 1.23;
      mixStat.accumulateLLK(l0, 0.5);
      test(mixStat.getAccumulatedLLK() == l0*0.5, __LINE__);
      test(mixStat.getMeanLLK() == l0, __LINE__);
      double l1 = 6.78;
      mixStat.accumulateLLK(l1);
      test(fabs(mixStat.getAccumulatedLLK()-(l0*0.5+l1))<1e-14, __LINE__);
      test(fabs(mixStat.getMeanLLK()-(l0*0.5+l1)/1.5)<1e-14, __LINE__);

      mixStat.resetLLK();
      double l2 = 1.23;
      double w2 = 0.8;
      mixStat.accumulateLLK(l2, w2);
      test(fabs(mixStat.getAccumulatedLLK()-l2*w2)<1e-14, __LINE__);
      test(fabs(mixStat.getMeanLLK()-l2*w2/w2)<1e-14, __LINE__);
      double l3 = 6.78;
      double w3 = 0.1;
      mixStat.accumulateLLK(l3, w3);
      test(fabs(mixStat.getAccumulatedLLK()-(l2*w2+l3*w3))<1e-14, __LINE__);
      test(fabs(mixStat.getMeanLLK()-(l2*w2+l3*w3)/(w2+w3))<1e-14, __LINE__);
    }
    // test method double getAccumulatedLLKFeatureCount() const
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      MixtureGD& m = ms.createMixtureGD(10);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, c);
      mixStat.resetLLK();
      Feature f(c);
      test(mixStat.getAccumulatedLLKFeatureCount() == 0.0, __LINE__);
      mixStat.computeAndAccumulateLLK(f);
      test(mixStat.getAccumulatedLLKFeatureCount() == 1.0, __LINE__);
      mixStat.computeAndAccumulateLLK(f);
      test(mixStat.getAccumulatedLLKFeatureCount() == 2.0, __LINE__);
    }
    // test method lk_t computeLLK(const Feature& f)
    {
      Config config;
      config.setParam("vectSize", "32");
      config.setParam("minLLK", "-300");
      config.setParam("maxLLK", "300");
      MixtureServer ms(config);
      MixtureGD& m = ms.createMixtureGD(5);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, config);
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

      lk_t llk = mixStat.computeLLK(f);
      lk_t llk2 = mixStat.computeLLK(f, 0);
      test(fabs(llk - (-73.820950855469)) <  1e-12, __LINE__);
      test(fabs(llk2 - (-72.177173580491)) <  1e-12, __LINE__);
    }
    // test method lk_t computeLLK(const Feature& f, unsigned long idx)
    {
      Config config;
      config.setParam("vectSize", "1");
      config.setParam("minLLK", "-300");
      config.setParam("maxLLK", "300");
      MixtureServer ms(config);
      MixtureGD& m = ms.createMixtureGD(5);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, config);
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

      lk_t llk = mixStat.computeLLK(f);
      try
      {
        mixStat.computeLLK(f, 1); // idx 1 > vectSize !
        showError(__LINE__);
      }
      catch (IndexOutOfBoundsException&) {}
      try
      {
        lk_t llk2 = mixStat.computeLLK(f, 0);
        test(fabs(llk-llk2) < 1e-15, __LINE__);
      }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
    }
    // test method lk_t computeAndAccumulateLLK(const Feature& f,
    //                  const double w = 1.0,
    //                  const TopDistribsAction& a = TOP_DISTRIBS_NO_ACTION)
    //        and  lk_t getMeanLLK();
    {
      Config config;
      config.setParam("vectSize", "32");
      config.setParam("minLLK", "-300");
      config.setParam("maxLLK", "300");
      MixtureServer ms(config);
      MixtureGD& m = ms.createMixtureGD(5);
      MixtureGD& m1 = ms.duplicateMixtureGD(m);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, config);
      MixtureGDStat mixStat1(K::k, ss, m1, config);
      for (c=0; c<m.getDistribCount(); c++)
      {
        DistribGD& d = m.getDistrib(c);
        DistribGD& d1 = m1.getDistrib(c);
        for (i=0; i<d.getVectSize(); i++)
        {
          d.setCov((real_t)(i+c+1), i);
          d.setMean((real_t)(i+c+2), i);
          d1.setCov((real_t)(i+c+1.1), i);
          d1.setMean((real_t)(i+c+2.1), i);
        }
      }
      m.computeAll();
      m.equalizeWeights();
      m1.computeAll();
      m1.equalizeWeights();
      Feature f(32), f1(32);
      for (i=0; i<f.getVectSize(); i++)
        f1[i] = (f[i] = (Feature::data_t)(i+1))+0.1;
      mixStat.resetLLK();
      try { mixStat.getMeanLLK(); showError(__LINE__); }
      catch (Exception&) {}
      lk_t llk = mixStat.computeAndAccumulateLLK(f);
      test(fabs(llk - (-73.820950855469)) <  1e-12, __LINE__);
      llk = mixStat.computeAndAccumulateLLK(f1, 0.5);
      lk_t mean = (mixStat.computeLLK(f) + mixStat.computeLLK(f1)*0.5)/1.5;
      try
      {
        test(fabs(mixStat.getMeanLLK() - mean) <  1e-12, __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }

      // test with param a = DETERMINE_TOP_DISTRIBS and USE_TOP_DISTRIBS
      config.setParam("topDistribsCount", "5"); // 5 = m.getDistribCount()
      config.setParam("computeLLKWithTopDistribs", "PARTIAL");
      mixStat.resetLLK();
      llk = mixStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);
      test(fabs(llk - (-73.820950855469)) <  1e-12, __LINE__);
      const LKVector& v = ss.getTopDistribIndexVector();
      test(v.size() == m.getDistribCount(), __LINE__);
      lk = v[0].lk;
      for (i=0; i<v.size(); i++)
      {
        if (fabs(v[i].lk - m.getDistrib(v[i].idx).computeLK(f)*m.weight(v[i].idx)) > 1e-44)
        {
          showError(__LINE__);
          break;
        }
        if (i>0 && v[i].lk > lk) // test tri du vecteur
        {
          showError(__LINE__);
          break;
        }
        lk = v[i].lk;
      }
      llk = mixStat.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      // avec le meme modele et nTop = 5 on obtient la meme valeur
      test(fabs(llk - (-73.820950855469)) <  1e-12, __LINE__);

      // Meme test mais avec nTop different de 5 et un le même modèle de locuteur
      config.setParam("topDistribsCount", "2");
      mixStat.resetLLK();
      mixStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);
      llk = mixStat.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      test(fabs(llk - (-72.904662572297)) <  1e-12, __LINE__);

      // Meme test mais avec avec COMPLETE a la place de PARTIAL
      config.setParam("computeLLKWithTopDistribs", "COMPLETE");
      mixStat.resetLLK();
      mixStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);
      llk = mixStat.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      // avec le meme modele on obtient la meme valeur
      test(fabs(llk - (-73.820950855469)) <  1e-12, __LINE__);

      // Meme test mais avec nTop different de 5 et un 2ème modèle de locuteur
      config.setParam("computeLLKWithTopDistribs", "PARTIAL");
      config.setParam("topDistribsCount", "2");
      mixStat.resetLLK();
      mixStat1.resetLLK();
      mixStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);
      llk = mixStat1.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      test(fabs(llk - (-73.439207762004)) <  1e-12, __LINE__);

      // Meme test mais avec COMPLETE a la place de PARTIAL
      config.setParam("computeLLKWithTopDistribs", "COMPLETE");
      mixStat.resetLLK();
      mixStat1.resetLLK();
      mixStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);
      llk = mixStat1.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      test(fabs(llk - (-74.355494314753)) <  1e-12, __LINE__);
    }

    // test method setTopDistribIndexVector(const ULongVector& indexVect,
    //                              real_t w, real_t l)
    {
      Config config;
      config.setParam("vectSize", "32");
      config.setParam("minLLK", "-300");
      config.setParam("maxLLK", "300");
      MixtureServer ms(config);
      MixtureGD& m = ms.createMixtureGD(5);
      MixtureGD& m1 = ms.duplicateMixtureGD(m);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, config);
      MixtureGDStat mixStat1(K::k, ss, m1, config);
      for (c=0; c<m.getDistribCount(); c++)
      {
        DistribGD& d = m.getDistrib(c);
        DistribGD& d1 = m1.getDistrib(c);
        for (i=0; i<d.getVectSize(); i++)
        {
          d.setCov((real_t)(i+c+1), i);
          d.setMean((real_t)(i+c+2), i);
          d1.setCov((real_t)(i+c+1.1), i);
          d1.setMean((real_t)(i+c+2.1), i);
        }
      }
      m.computeAll();
      m.equalizeWeights();
      m1.computeAll();
      m1.equalizeWeights();
      Feature f(32), f1(32);
      for (i=0; i<f.getVectSize(); i++)
        f1[i] = (f[i] = (Feature::data_t)(i+1))+0.1;
      mixStat.resetLLK();
      try { mixStat.getMeanLLK(); showError(__LINE__); }
      catch (Exception&) {}
      lk_t llk = mixStat.computeAndAccumulateLLK(f);
      test(fabs(llk - (-73.820950855469)) <  1e-12, __LINE__);
      llk = mixStat.computeAndAccumulateLLK(f1, 0.5);
      lk_t mean = (mixStat.computeLLK(f) + mixStat.computeLLK(f1)*0.5)/1.5;
      try
      {
        test(fabs(mixStat.getMeanLLK() - mean) <  1e-12, __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }

      // test with param a = DETERMINE_TOP_DISTRIBS and USE_TOP_DISTRIBS
      config.setParam("topDistribsCount", "5"); // 5 = m.getDistribCount()
      config.setParam("computeLLKWithTopDistribs", "PARTIAL");
      mixStat.resetLLK();
      llk = mixStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);
      test(fabs(llk - (-73.820950855469)) <  1e-12, __LINE__);
      const LKVector& v = ss.getTopDistribIndexVector();
      test(v.size() == m.getDistribCount(), __LINE__);
      lk = v[0].lk;
      for (i=0; i<v.size(); i++)
      {
        if (fabs(v[i].lk - m.getDistrib(v[i].idx).computeLK(f)*m.weight(v[i].idx)) > 1e-44)
        {
          showError(__LINE__);
          break;
        }
        if (i>0 && v[i].lk > lk) // test tri du vecteur
        {
          showError(__LINE__);
          break;
        }
        lk = v[i].lk;
      }

      // sauve les index des top distribs

      ULongVector save;
      for (i=0; i<config.getParam("topDistribsCount").toLong(); i++)
        save.addValue(v[i].idx);
      double aa = ss._topDistribsVect.sumNonTopDistribLK;
      double bb = ss._topDistribsVect.sumNonTopDistribWeights;
      
      // on met n'importe quoi à la place

      for (i=0;i<ss._topDistribsVect.size(); i++)
        ss._topDistribsVect.getArray()[i].idx = 1;
      ss._topDistribsVect.sumNonTopDistribLK =4564;
      ss._topDistribsVect.sumNonTopDistribWeights = 478787;

      // on remet les données 

      ss.setTopDistribIndexVector(save, aa, bb);

      llk = mixStat.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      // avec le meme modele et nTop = 5 on obtient la meme valeur
      test(fabs(llk - (-73.820950855469)) <  1e-12, __LINE__);

      // Meme test mais avec nTop different de 5 et un le même modèle de locuteur
      config.setParam("topDistribsCount", "2");
      mixStat.resetLLK();
      mixStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);


      // sauve les index des top distribs

      save.clear();
      for (i=0; i<config.getParam("topDistribsCount").toLong(); i++)
        save.addValue(v[i].idx);
      aa = ss._topDistribsVect.sumNonTopDistribLK;
      bb = ss._topDistribsVect.sumNonTopDistribWeights;
      
      // on met n'importe quoi à la place

      for (i=0;i<ss._topDistribsVect.size(); i++)
        ss._topDistribsVect.getArray()[i].idx = 1;
      ss._topDistribsVect.sumNonTopDistribLK =4564;
      ss._topDistribsVect.sumNonTopDistribWeights = 478787;

      // on remet les données 

      ss.setTopDistribIndexVector(save, aa, bb);


      llk = mixStat.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      test(fabs(llk - (-72.904662572297)) <  1e-12, __LINE__);

      // Meme test mais avec avec COMPLETE a la place de PARTIAL
      config.setParam("computeLLKWithTopDistribs", "COMPLETE");
      mixStat.resetLLK();
      mixStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);
      llk = mixStat.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      // avec le meme modele on obtient la meme valeur
      test(fabs(llk - (-73.820950855469)) <  1e-12, __LINE__);

      // Meme test mais avec nTop different de 5 et un 2ème modèle de locuteur
      config.setParam("computeLLKWithTopDistribs", "PARTIAL");
      config.setParam("topDistribsCount", "2");
      mixStat.resetLLK();
      mixStat1.resetLLK();
      mixStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);
      llk = mixStat1.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      test(fabs(llk - (-73.439207762004)) <  1e-12, __LINE__);

      // Meme test mais avec COMPLETE a la place de PARTIAL
      config.setParam("computeLLKWithTopDistribs", "COMPLETE");
      mixStat.resetLLK();
      mixStat1.resetLLK();
      mixStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);
      llk = mixStat1.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      test(fabs(llk - (-74.355494314753)) <  1e-12, __LINE__);
    }


    // -----------------
    //      Occ
    // -----------------

    // test method void resetOcc();
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      MixtureGD& m = ms.createMixtureGD(10); // 10 distribs
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, c);
      Feature f(c);
      mixStat.computeAndAccumulateOcc(f);
      test(mixStat._featureCounterForAccumulatedOcc != 0.0, __LINE__);
      mixStat._accumulatedOccVect[0] = 1.2f;
      mixStat.resetOcc();
      test(mixStat._featureCounterForAccumulatedOcc == 0.0, __LINE__);
      test(mixStat._accumulatedOccVect[0] == 0.0, __LINE__);
    }

    // test method real_t getAccumulatedOccFeatureCount() const
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      MixtureGD& m = ms.createMixtureGD(10);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, c);
      mixStat.resetLLK();
      Feature f(c);
      test(mixStat.getAccumulatedOccFeatureCount() == 0, __LINE__);
      mixStat.computeAndAccumulateOcc(f);
      test(mixStat.getAccumulatedOccFeatureCount() == 1, __LINE__);
      mixStat.computeAndAccumulateOcc(f);
      test(mixStat.getAccumulatedOccFeatureCount() == 2, __LINE__);
    }

    // -----------------
    //      EM 
    // -----------------

    // test method unsigned long getEMFeatureCount() const
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      MixtureGD& m = ms.createMixtureGD(10);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, c);
      mixStat.resetEM();
      Feature f(c);
      test(mixStat.getEMFeatureCount() == 0.0, __LINE__);
      mixStat.computeAndAccumulateEM(f);
      test(mixStat.getEMFeatureCount() == 1.0, __LINE__);
      mixStat.computeAndAccumulateEM(f);
      test(mixStat.getEMFeatureCount() == 2.0, __LINE__);
    }
    // test method : MixtureGD& getInternalAccumEM()
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("minLLK", "-300");
      c.setParam("maxLLK", "300");
      MixtureServer ms(c);
      MixtureGD& m = ms.createMixtureGD(10);
      StatServer ss(config);
      MixtureGDStat mixStat(K::k, ss, m, c);
      try
      { 
        mixStat.getInternalAccumEM(); // resetEM() pas fait avant
        showError(__LINE__);
      }
      catch (Exception&) {}
      mixStat.resetEM();
      try
      { 
        MixtureGD& tmp = mixStat.getInternalAccumEM();
        test(tmp.isSameObject(*mixStat._pMixForAccumulation), __LINE__);
        test(tmp.getDistribCount() == m.getDistribCount(), __LINE__);
        test(tmp.getVectSize() == m.getVectSize(), __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
    }
    // test method occt_t computeAndAccumulateEM(const Feature& f, real_t w)
    // with weight != 0.0
    {
      Config co;
      co.setParam("vectSize", "2");
      co.setParam("minLLK", "-300");
      co.setParam("maxLLK", "300");
      MixtureServer ms(co);
      MixtureGD& m1 = ms.createMixtureGD(2);
      m1.weight(0) = 0.8;
      m1.weight(1) = 0.2;
      m1.getDistrib(0).setCov(0.1,0);
      m1.getDistrib(0).setCov(0.2,1);
      m1.getDistrib(1).setCov(0.3,0);
      m1.getDistrib(1).setCov(0.4,1);
      m1.getDistrib(0).setMean(0.1,0);
      m1.getDistrib(0).setMean(0.2,1);
      m1.getDistrib(1).setMean(0.3,0);
      m1.getDistrib(1).setMean(0.4,1);
      m1.computeAll();

      MixtureGD& m2 = ms.duplicateMixtureGD(m1); //copy
      test(m2 == m1, __LINE__);

      StatServer ss(config);
      MixtureGDStat& mixStat1 = ss.createAndStoreMixtureStat(m1);
      MixtureGDStat& mixStat2 = ss.createAndStoreMixtureStat(m2);
      Feature f1(2);
      f1[0] = 1.0;
      f1[1] = 1.1;
      Feature f2(2);
      f2[0] = 1.05;
      f2[1] = 1.15;

      mixStat1.resetEM();
      mixStat1.computeAndAccumulateEM(f1); // weight = 1.0
      mixStat1.computeAndAccumulateEM(f2); // weight = 1.0

      mixStat2.resetEM();
      mixStat2.computeAndAccumulateEM(f1, 1.0);
      mixStat2.computeAndAccumulateEM(f2, 0.25);
      mixStat2.computeAndAccumulateEM(f2, 0.75);

      m1 = mixStat1.getEM();
      m2 = mixStat2.getEM();
      MixtureGDStat& mixStat1a = ss.createAndStoreMixtureStat(m1);
      MixtureGDStat& mixStat2a = ss.createAndStoreMixtureStat(m2);
      test(fabs(mixStat1a.computeLLK(f1)-mixStat2a.computeLLK(f1)) <1e-13, __LINE__);
    }
    // test method : void addAccEM(const MixtureStat&)
    {
      Config co;
      co.setParam("vectSize", "2");
      co.setParam("minLLK", "-300");
      co.setParam("maxLLK", "300");
      MixtureServer ms(co);
      MixtureGD& m1 = ms.createMixtureGD(2);
      m1.weight(0) = 0.8;
      m1.weight(1) = 0.2;
      m1.getDistrib(0).setCov(0.1,0);
      m1.getDistrib(0).setCov(0.2,1);
      m1.getDistrib(1).setCov(0.3,0);
      m1.getDistrib(1).setCov(0.4,1);
      m1.getDistrib(0).setMean(0.1,0);
      m1.getDistrib(0).setMean(0.2,1);
      m1.getDistrib(1).setMean(0.3,0);
      m1.getDistrib(1).setMean(0.4,1);
      m1.computeAll();
      MixtureGD& m2 = ms.createMixtureGD(2);
      m2.weight(0) = 0.85;
      m2.weight(1) = 0.25;
      m2.getDistrib(0).setCov(0.15,0);
      m2.getDistrib(0).setCov(0.25,1);
      m2.getDistrib(1).setCov(0.35,0);
      m2.getDistrib(1).setCov(0.45,1);
      m2.getDistrib(0).setMean(0.15,0);
      m2.getDistrib(0).setMean(0.25,1);
      m2.getDistrib(1).setMean(0.35,0);
      m2.getDistrib(1).setMean(0.45,1);
      m2.computeAll();
      MixtureGD& m3 = ms.duplicateMixtureGD(m1); //copy
      test(m3 == m1, __LINE__);

      StatServer ss(config);
      MixtureGDStat& mixStat1 = ss.createAndStoreMixtureStat(m1);
      MixtureGDStat& mixStat2 = ss.createAndStoreMixtureStat(m2);
      MixtureGDStat& mixStat3 = ss.createAndStoreMixtureStat(m3);
      Feature f1(2);
      f1[0] = 1.0;
      f1[1] = 1.1;
      Feature f2(2);
      f2[0] = 1.05;
      f2[1] = 1.15;

      mixStat1.resetEM();
      mixStat1.computeAndAccumulateEM(f1);
      mixStat1.computeAndAccumulateEM(f1);

      mixStat2.resetEM();
      mixStat2.computeAndAccumulateEM(f1);
      mixStat2.computeAndAccumulateEM(f2);

      // 3 = 1 + 2
      mixStat3.resetEM();
      mixStat3.computeAndAccumulateEM(f1); // copy of mixStat1
      mixStat3.computeAndAccumulateEM(f1);
      mixStat3.addAccEM(mixStat2);

      DoubleVector v = mixStat1.getAccumulatedOccVect();
      v += mixStat2.getAccumulatedOccVect();

      test(mixStat3.getEMFeatureCount() ==
          mixStat1.getEMFeatureCount() + 
          mixStat2.getEMFeatureCount(), __LINE__);
      test(mixStat3.getAccumulatedOccFeatureCount() ==
          mixStat1.getAccumulatedOccFeatureCount() +
          mixStat2.getAccumulatedOccFeatureCount(), __LINE__);
      test(v == mixStat3.getAccumulatedOccVect(), __LINE__);
      for (unsigned long c=0; c<m1.getDistribCount(); c++)
      {
        const DistribGD& d1 = mixStat1.getInternalAccumEM().getDistrib(c);
        const DistribGD& d2 = mixStat2.getInternalAccumEM().getDistrib(c);
        const DistribGD& d3 = mixStat3.getInternalAccumEM().getDistrib(c);
        v = d1.getCovVect();
        v += d2.getCovVect();
        test(d3.getCovVect() == v, __LINE__);
        v = d1.getMeanVect();
        v += d2.getMeanVect();
        test(d3.getMeanVect() == v, __LINE__);
      }

    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestMixtureGDStat::getClassName() const
{ return "TestMixtureGDStat"; }
//-------------------------------------------------------------------------
TestMixtureGDStat::~TestMixtureGDStat() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestMixtureGDStat_cpp)

