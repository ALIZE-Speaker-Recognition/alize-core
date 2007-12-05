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

#if !defined(ALIZE_WorldTraining_cpp)
#define ALIZE_WorldTraining_cpp

//#include <iostream>
#include <cmath>
#include "WorldTraining.h"

using namespace alize;
using namespace std;

static bool verbose = true;

//-------------------------------------------------------------------------
bool baggedFrame(float baggedFrameProbability)
{
  // return (drand48()< baggedFrameProbability);
  return ((rand()/RAND_MAX) < baggedFrameProbability);
}
//-------------------------------------------------------------------------
float setItParameter(float begin, float end, int nbIt, int it)
{
  if (nbIt<2) return begin;
  float itVal=(begin-end/(nbIt-1));
  return begin-(itVal*it);
}
//-------------------------------------------------------------------------
void varianceControl(MixtureGD& model,float flooring,float ceiling)
{
  unsigned long vectSize   = model.getVectSize();
  unsigned long distribCount = model.getDistribCount();
  unsigned long cptFlooring  = 0;
  unsigned long cptCeiling   = 0;

  for (unsigned long c=0; c<distribCount; c++)
  {
    DistribGD& d = model.getDistrib(c);
    for (unsigned long v=0; v<vectSize; v++)
    {
      float cov = d.getCov(v); 
      if (cov < flooring) { cov = flooring; cptFlooring++; }
      if (cov > ceiling)  { cov = ceiling;  cptCeiling++; }
      d.setCov(cov, v);
    }
  }
  model.computeAll();
  if (verbose) cout << " total variance flooring = " << cptFlooring
    << " ceiling = " << cptCeiling << endl;
}
//-------------------------------------------------------------------------
WorldTraining::WorldTraining(Config& config)
{
  // parameters get in the config, from command line or config file
  // input feature file  filename
  const String inputPrmFilename = "5036";
  // input worldmodel file filename
  const String inputWorldFilename = "WORLD_hom";
  // output worldmodel file filename
  const String outputWorldFilename = "OutputWorld";
  // baggedFrameProbability
  float baggedFrameProbability = 0.2f;
  // variance control parameters
  float initVarianceFlooring  = 0.8f;
  float initVarianceCeiling   = 4.0;
  float finalVarianceFlooring = 0.01f;
  float finalVarianceCeiling  = 4.0;   
  // number of train it (including the final it)
  int nbTrainIt = 2; // 20;
  // number of final it 
  int nbTrainFinalIt = 5;
  // numer of iteration for the EM algorithm (total number=nbTainIt*nbEmIt)
  int nbEmIt = 5;
  // label for selected frames
  unsigned long labelSelectedFrame = 0;
   
  // information levels verbose and debug
  bool debug = false;
   
   
  try
  {
    cout << endl;
    cout << "***********************" << endl;
    cout << "* Train a world model *" << endl;
    cout << "***********************" << endl;


    if (verbose) cout << "Create a featureServer to load the"
              << " features inside the server" << endl;    
    FeatureServer fs(config, inputPrmFilename);
	  // the file could be a list of prmfile with the extension .lst
 
    if (verbose) cout << "Build the mixture server" << endl;
    MixtureServer ms(config);

    if (verbose) cout << "Build a statistic server" << endl;
    StatServer ss(config, ms);

    // adds the label file(s) loading, we will use it for training
    // the world model
  	LabelServer labelServer;
	
    if (verbose) cout << "Load initial world model" << endl;
    MixtureGD& worldModel = ms.loadMixtureGD(inputWorldFilename);
    MixtureStat& worldStat = ss.createAndStoreMixtureStat(worldModel);

    if (verbose) cout << "Train world model" << endl;    

    for (int trainIt=0; trainIt<nbTrainIt; trainIt++)
    {
      Feature f;
	    float varianceFlooring = initVarianceFlooring;
	    float varianceCeiling  = initVarianceCeiling;

      // variance control parameters (if we are not in the final iterations)
      if (trainIt<nbTrainIt-nbTrainFinalIt)
      {
         varianceFlooring = setItParameter(initVarianceFlooring,
           finalVarianceFlooring, nbTrainIt - nbTrainFinalIt, trainIt);
         varianceCeiling  = setItParameter(initVarianceCeiling,
           finalVarianceFlooring, nbTrainIt - nbTrainFinalIt, trainIt);
      }

	
      if (verbose) cout << "Train iteration " << trainIt << endl;	  
      
	    // Em iterations
	    for (unsigned long emIt=0; emIt<nbEmIt; emIt++)
	    {
	  	  if (verbose) cout << "  > Reset EM accumulators" << endl;  
	      worldStat.resetEM();
        if (verbose) cout << "  > Compute and accumulate EM for"
                  << " each feature" << endl;
		    // reset the reader at the begin of the input stream
		    fs.reset();
    	  while (fs.readFeature(f))
    	  {
      	    unsigned long labelCode = f.getLabelCode();
      	    if (f.isValid() && (labelCode == labelSelectedFrame))
      	      if (baggedFrame(baggedFrameProbability))
	              worldStat.computeAndAccumulateEM(f);
		    }
        if (verbose) cout << "  > Get the result" << endl;
		    worldModel = worldStat.getEM();
  	  }
	    if (verbose) cout << "  > Control the variance (flooring and"
                << " ceiling)" << endl;
        varianceControl(worldModel,varianceFlooring,varianceCeiling);
		
	    // compute the loglikelihood of the data, if needed
	    if (verbose)
	    {
	      // reset the reader at the begin of the input strem
	      fs.reset();
        while (fs.readFeature(f))
        {
          unsigned long labelCode = f.getLabelCode();
          if (f.isValid() && (labelCode == labelSelectedFrame))      	  
	        worldStat.computeAndAccumulateLLK(f);
        }
	      cout << "It: " << trainIt << " ll world = "
           << worldStat.getMeanLLK() << endl;
      }      
    }

    if (verbose) cout << "Save world model" << endl;
    worldModel.save(outputWorldFilename, config);
  }
  catch (Exception& e)
  {
    cout << e.toString() << endl;
  }
}
//-------------------------------------------------------------------------
WorldTraining::~WorldTraining() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_WorldTraining_cpp)
