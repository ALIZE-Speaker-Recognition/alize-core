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

#if !defined(ALIZE_TestMixtureGFStat_cpp)
#define ALIZE_TestMixtureGFStat_cpp

#include <cmath>
#include "TestMixtureGFStat.h"
#include "MixtureGFStat.h"
#include "LKVector.h"
#include "Feature.h"
#include "MixtureGF.h"
#include "AutoDestructor.h"
#include "Config.h"
#include "MixtureServer.h"
#include <iostream>
#include "StatServer.h"

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestMixtureGFStat::TestMixtureGFStat()
:Test() {}
//-------------------------------------------------------------------------
bool TestMixtureGFStat::exec()
{
  Test::exec();
  {
    { // 12/02/2004
      Feature f(1);
      f[0] = 10;
      MixtureGF& mix = MixtureGF::create(K::k, "", 1, 1);
      AutoDestructor ad(mix);
      mix.getDistrib(0).setCov(1.5, 0, 0);
      mix.getDistrib(0).setMean(5.0, 0);
      mix.weight(0) = 1.0;
      mix.computeAll();
      Config config;
      config.setParam("minLLK", "-100");
      config.setParam("maxLLK", "100");
      StatServer ss(config);
      MixtureGFStat mixStat(K::k, ss, mix, config);
      mixStat.resetEM();
      mixStat.computeAndAccumulateEM(f);
      mix = mixStat.getEM();
    }

    unsigned long i;
    MixtureGF& mix = MixtureGF::create(K::k, "un", 3, 5);
    AutoDestructor ad(mix);

    Config config;
    config.setParam("minLLK", "-1e10");
    config.setParam("maxLLK", "1e10");
    StatServer ss(config);
    MixtureGFStat mixStat(K::k, ss, mix, config);
    test(mixStat.getClassName() == "MixtureGFStat", __LINE__);

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
    for (unsigned long c=0; c<mixStat.getMixture().getDistribCount(); c++)
    {
      DistribGF& d = mix.getDistrib(c);
      d.setCov(3.0, 0, 0);
      d.setCov(2.0, 1, 0);
      d.setCov(1.0, 2, 0);
      d.setCov(2.0, 0, 1);
      d.setCov(2.0, 1, 1);
      d.setCov(1.0, 2, 1);
      d.setCov(1.0, 0, 2);
      d.setCov(1.0, 1, 2);
      d.setCov(1.0, 2, 2);
      for (i=0; i<d.getVectSize(); i++)
        d.setMean((real_t)(i+c+2), i);
    }
    mix.computeAll();

    Feature feature(3);
    for (i=0; i<feature.getVectSize(); i++)
      feature[i] = (Feature::data_t)(i+1);
    LKVector lkVect;
    mixStat.computeAndAccumulateLLK(feature, 1.0f,
            TOP_DISTRIBS_NO_ACTION);
    lk_t lk = mixStat.getAccumulatedLLK();
    test(fabs(lk - (-4.6495264805421)) <  1e-12, __LINE__);
    test(lk == mixStat.getLLK(), __LINE__);

    try
    {
      test(fabs(mixStat.getMeanLLK()
            - (-4.6495264805421)) <  1e-12, __LINE__);
    }
    catch (Exception&) { showError(__LINE__); }
    mixStat.computeAndAccumulateLLK(feature, 1.0f,
        TOP_DISTRIBS_NO_ACTION); // twice
    try
    {
      test(fabs(mixStat.getMeanLLK()
            - (-4.6495264805421)) <  1e-12, __LINE__);
    }
    catch (Exception&) { showError(__LINE__); }
    test(lk*2 == mixStat.getAccumulatedLLK(), __LINE__);

    mixStat.computeAndAccumulateOcc(feature);
    real_t o;
    
    test(fabs((o = occVect[0]) - 0.80514971981758  ) <  1e-14, __LINE__);
    test(fabs((o = occVect[1]) - 0.17965318592636  ) <  1e-14, __LINE__);
    test(fabs((o = occVect[2]) - 0.014746831519544 ) <  1e-14, __LINE__);
    test(fabs((o = occVect[3]) - 0.00044531572566001) <  1e-14, __LINE__);
    test(fabs((o = occVect[4]) - 4.9470108547819e-006) <  1e-14, __LINE__);

    test(fabs(accOccVect[0] - 0.80514971981758  ) <  1e-14, __LINE__);
    test(fabs(accOccVect[1] - 0.17965318592636  ) <  1e-14, __LINE__);
    test(fabs(accOccVect[2] - 0.014746831519544) <  1e-14, __LINE__);
    test(fabs(accOccVect[3] - 0.00044531572566001) <  1e-14, __LINE__);
    test(fabs(accOccVect[4] - 4.9470108547819e-006) <  1e-14, __LINE__);

    DoubleVector& meanOccVect = mixStat.getMeanOccVect();
    test(fabs(meanOccVect[0] - 0.80514971981758  ) <  1e-14, __LINE__);
    test(fabs(meanOccVect[1] - 0.17965318592636  ) <  1e-14, __LINE__);
    test(fabs(meanOccVect[2] - 0.014746831519544) <  1e-14, __LINE__);
    test(fabs(meanOccVect[3] - 0.00044531572566001) <  1e-14, __LINE__);
    test(fabs(meanOccVect[4] - 4.9470108547819e-006) <  1e-14, __LINE__);

    mixStat.computeAndAccumulateOcc(feature); // twice
    
    test(fabs(accOccVect[0] - 0.80514971981758  *2) <  1e-14, __LINE__);
    test(fabs(accOccVect[1] - 0.17965318592636  *2) <  1e-14, __LINE__);
    test(fabs(accOccVect[2] - 0.014746831519544*2) <  1e-14, __LINE__);
    test(fabs(accOccVect[3] - 0.00044531572566001*2) <  1e-14, __LINE__);
    test(fabs(accOccVect[4] - 4.9470108547819e-006*2) <  1e-14, __LINE__);

    test(fabs(meanOccVect[0] - 0.80514971981758  ) <  1e-14, __LINE__);
    test(fabs(meanOccVect[1] - 0.17965318592636  ) <  1e-14, __LINE__);
    test(fabs(meanOccVect[2] - 0.014746831519544) <  1e-14, __LINE__);
    test(fabs(meanOccVect[3] - 0.00044531572566001) <  1e-14, __LINE__);
    test(fabs(meanOccVect[4] - 4.9470108547819e-006) <  1e-14, __LINE__);

   /* 
    // test EM simplifie : 1 iteration
    mixStat.resetEM();
    //for (i=0; i<100; i++)
    for (i=0; i<1; i++)
      mixStat.computeAndAccumulateEM(feature); // always the same feature !
      // TODO : faire un test plus complet
    
    mixStat.getEM();

    // TODO : tester avec distribLKVect

*/
    // test method occt_t computeAndAccumulateEM(const Feature& f, real_t w)
    // with weight != 0.0
    if(false){
      Config co;
      co.setParam("vectSize", "3");
      co.setParam("minLLK", "-300");
      co.setParam("maxLLK", "300");
      MixtureServer ms(co);
      MixtureGF& m1 = ms.createMixtureGF(2);
      m1.weight(0) = 0.5;
      m1.weight(1) = 0.2;
      m1.weight(1) = 0.3;
    for (unsigned long c=0; c<m1.getDistribCount(); c++)
    {
      DistribGF& d = m1.getDistrib(c);
      d.setCov(3.0, 0, 0);
      d.setCov(2.0, 1, 0);
      d.setCov(1.0, 2, 0);
      d.setCov(2.0, 0, 1);
      d.setCov(2.0, 1, 1);
      d.setCov(1.0, 2, 1);
      d.setCov(1.0, 0, 2);
      d.setCov(1.0, 1, 2);
      d.setCov(1.0, 2, 2);
      for (i=0; i<d.getVectSize(); i++)
        d.setMean((real_t)(i+c+2), i);
    }
    cout << m1.getDistrib(0)._covMatr.toString() << endl;
    m1.computeAll();
    cout << m1.getDistrib(0)._covInvMatr.toString() << endl;

      MixtureGF& m2 = ms.duplicateMixtureGF(m1); //copy
      test(m2 == m1, __LINE__);

      StatServer ss(config);
      MixtureGFStat& mixStat1 = ss.createAndStoreMixtureStat(m1);
      MixtureGFStat& mixStat2 = ss.createAndStoreMixtureStat(m2);
      Feature f1(3);
      f1[0] = 1.0;
      f1[1] = 1.1;
      f1[2] = 1.2;
      Feature f2(3);
      f2[0] = 1.05;
      f2[1] = 1.15;
      f2[2] = 1.25;

      mixStat1.resetEM();
      mixStat1.computeAndAccumulateEM(f1); // weight = 1.0
      mixStat1.computeAndAccumulateEM(f2); // weight = 1.0

      mixStat2.resetEM();
      mixStat2.computeAndAccumulateEM(f1, 1.0);
      mixStat2.computeAndAccumulateEM(f2, 0.25);
      mixStat2.computeAndAccumulateEM(f2, 0.75);

      m1 = mixStat1.getEM();
      m2 = mixStat2.getEM();
      MixtureGFStat& mixStat1a = ss.createAndStoreMixtureStat(m1);
      MixtureGFStat& mixStat2a = ss.createAndStoreMixtureStat(m2);
      test(fabs(mixStat1a.computeLLK(f1)-mixStat2a.computeLLK(f1)) <1e-13, __LINE__);
    }


  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestMixtureGFStat::getClassName() const
{ return "TestMixtureGFStat"; }
//-------------------------------------------------------------------------
TestMixtureGFStat::~TestMixtureGFStat() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestMixtureGFStat_cpp)
