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

#if !defined(ALIZE_Score_cpp)
#define ALIZE_Score_cpp

#include <iostream>
#include "Score.h"

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
Score::Score(Config& config)
{
  unsigned long i;
  try
  {
    cout << endl;
    cout << "********************************************" << endl;
    cout << "* Compute the log-likelihood ratio between *" << endl;
    cout << "* data and client/world models       *" << endl;
    cout << "********************************************" << endl;

    cout << endl << "Create a label server (optional)" << endl;

    LabelServer labelServer;

    cout << "Build the mixture server" << endl;

    MixtureServer ms(config);

    cout << "Load and store world mixture in the server" << endl;
    
    Mixture& world = ms.loadMixture("WORLD_hom");

    cout << endl;
    cout << "---------------------------" << endl;
    cout << " Test with 1 speaker       " << endl;
    cout << "---------------------------" << endl;

    cout << endl << "Load and store a speaker mixture in the server" << endl;

    Mixture& spk = ms.loadMixtureGD("6149");

    cout << "Create a list of feature files to test" << endl;

    XLine prmList;
    prmList.addElement("5036").addElement("5036bis");

    cout << "Create a feature server for the features files" << endl;
    
    FeatureServer fs(config, prmList, labelServer);

    cout << "Build a statistic server";
    cout << " connected to the mixture server" << endl;

    StatServer ss(config, ms);
    MixtureStat& worldStat = ss.createAndStoreMixtureStat(world);
    MixtureStat& spkStat = ss.createAndStoreMixtureStat(spk);

    cout << "Reset likelihood accumulators";
    cout << " for mixtures world and spk" << endl;

    worldStat.resetLLK();
    spkStat.resetLLK();

    cout << "Compute and accumulate likelihood";
    cout << " for mixtures world and spk" << endl;

    Feature f;

    while ( fs.readFeature(f))
    {
      unsigned long labelCode = f.getLabelCode();
      if (f.isValid() && labelCode == 0 || labelCode == 1)
      {
        worldStat.computeAndAccumulateLLK(f, 1.0, DETERMINE_TOP_DISTRIBS);
        spkStat.computeAndAccumulateLLK(f, 1.0, USE_TOP_DISTRIBS);
      }
    }

    cout << "Get mean likelihood" << endl;

    lk_t llWorld = worldStat.getMeanLLK();
    lk_t llSpk   = spkStat.getMeanLLK();

    cout << endl;
    cout << "ll world   = " << llWorld     << endl;
    cout << "ll spk1    = " << llSpk     << endl;
    cout << "spk1 - world = " << llSpk-llWorld << endl;



    cout << endl;
    cout << "--------------------------------------------" << endl;
    cout << "Test with several speakers          " << endl;
    cout << "(use a feature server to store all the    " << endl;
    cout << " features to test in memory         " << endl;
    cout << "--------------------------------------------" << endl;
    
    
    cout << endl;

    cout << "Create a list of mixture files" << endl;
    
    XLine mixtureFileList;
    mixtureFileList.addElement("6137")
                   .addElement("6143")
                   .addElement("6149");
    
    cout << "Create a featureServer and load";
    cout << " ALL the features inside the server" << endl;
    
    FeatureServer fs2(config, prmList, labelServer);

    cout << "Load all the mixtures in the mixture server" << endl;

    unsigned long firstMix = ms.loadMixture(mixtureFileList);

    MixtureServerFileWriter ww("./testSaveMixtureServer.xml", config);
    ww.writeMixtureServer(ms);
    
    for (i=firstMix; i<ms.getMixtureCount(); i++)
      ss.createAndStoreMixtureStat(ms.getMixture(i));
    for (i=firstMix; i<ss.getMixtureStatCount(); i++)
    {
      MixtureStat& m = ss.getMixtureStat(i);

      cout << "Spk " << i-firstMix << " :" << endl;
      cout << "  > Reset Likelihood accumulator" << endl;
      m.resetLLK();
      cout << "  > compute and accumulate likelihood" << endl;
      fs2.seekFeature(0);
      while (fs2.readFeature(f))
        m.computeAndAccumulateLLK(f);

      cout << "  Spk - world = " << m.getMeanLLK() - llWorld << endl;
    }

    
    cout << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "OTHER POSSIBILITY                " << endl;
    cout << "For each new feature, we compute the likelihood  " << endl;
    cout << " for all the dictionnary of distributions    " << endl;
    cout << "-------------------------------------------------" << endl; 

    cout << endl;
    cout << "Reset the likelihood accumulator of each mixture" << endl;
    
    for (i=firstMix; i<ss.getMixtureStatCount(); i++)
      ss.getMixtureStat(i).resetLLK();

    cout << "Compute and accumulate Likelihood" << endl;

    fs2.seekFeature(0);
    while (fs2.readFeature(f))
      {
      ss.computeAllDistribLK(f);
    
      for (i=firstMix; i<ss.getMixtureStatCount(); i++)
        ss.getMixtureStat(i).computeAndAccumulateLLK();
    }

    cout << endl;
    for (i=firstMix; i<ss.getMixtureStatCount(); i++)
    {
      cout << "spk" << i-firstMix << " - world = "
         << ss.getMixtureStat(i).getMeanLLK() - llWorld << endl;
    }
    cout << endl;
  }
  catch (Exception& e)
  {
    cout << e.toString() << endl;
  }
}
//-------------------------------------------------------------------------
Score::~Score() {}
//-------------------------------------------------------------------------
#endif // !defined(ALIZE_Score_cpp)
