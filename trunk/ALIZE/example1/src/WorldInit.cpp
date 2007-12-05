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

#if !defined(ALIZE_WorldInit_cpp)
#define ALIZE_WorldInit_cpp

#include <iostream>
#include "WorldInit.h"

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
WorldInit::WorldInit(Config& config)
{
  try
  {
    cout << endl;
    cout << "****************************" << endl;
    cout << "* Initialize a world model *" << endl;
    cout << "****************************" << endl;

    unsigned long n;

    // Default base directory
    
#ifdef WIN32
    String baseDir = "../../example1/";
#else
    String baseDir = "../";
#endif

    cout << endl;
    
    cout << "Create a featureServer" << endl;
    
    FeatureServer fs(config, "5036");

    cout << "Build the mixture server" << endl;

    MixtureServer ms(config);

    cout << "Create world model in the mixture server" << endl;

    MixtureGD& world = ms.createMixtureGD();

    cout << "Build a statistic server" << endl;

    StatServer ss(config, ms);

    cout << "Compute and set covariance/mean values in the distributions" << endl;
    
    unsigned long c, v;
    unsigned long nbTrames = fs.getFeatureCount();
    unsigned long vectSize = world.getVectSize();
    unsigned long distribCount = world.getDistribCount();

    unsigned long step = nbTrames / distribCount;
    unsigned long nb = 10;
    if (nb > nbTrames/(step+1))
      nb = nbTrames/(step+1);

    for (v=0; v<vectSize; v++)
    {
      real_t sum = 0.0;
      real_t sumsqr = 0.0;

      fs.seekFeature(0);
      Feature f;
      while (fs.readFeature(f))
      {
        real_t data = f[v];
        sum += data;
        sumsqr += data * data;
      }
      real_t cov  = sumsqr / nbTrames - (sum / nbTrames) * (sum / nbTrames);

      for (c=0; c < distribCount; c++)
      {
        DistribGD& d = world.getDistrib(c);
        real_t cumul = 0.0;
        fs.seekFeature(c*step);
        unsigned long nn = 0;
        for (n=0; n<nb; n++)
        {
          if (fs.readFeature(f))
          {
            cumul += f[v];
            nn++;
          }
        }
        d.setCov(cov, v); // cov must be 1.0
        d.setMean(cumul / nn, v);
      }
    }
    world.computeAll();

    cout << "Set distribution weights" << endl;
    
    world.equalizeWeights(); // set weight = 1/distribCount for each distrib

    cout << "Save world mixture on disc (world.xml)" << endl;

    world.save("world", config);
  }
  catch (Exception& e)
  {
    cout << e.toString() << endl;
  }
}
//-------------------------------------------------------------------------
WorldInit::~WorldInit() {}
//-------------------------------------------------------------------------
#endif // !defined(ALIZE_WorldInit_cpp)
