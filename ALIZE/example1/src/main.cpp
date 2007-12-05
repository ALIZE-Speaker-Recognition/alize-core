/*
Alize is a free, open tool for speaker recognition

Alize is a development project initiated by the ELISA consortium
  [www.lia.univ-avignon.fr/heberges/ALIZE/ELISA] and funded by the
  French Research Ministry in the framework of the
  TECHNOLANGUE program [www.technolangue.net]

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

//#include <iostream> // pour cin

#include "WorldTraining.h"
#include "Score.h"
#include "WorldInit.h"

int main(int argc, char* argv[])
{
  using namespace std;
  using namespace alize;

  try
  {
    // Declares options for command line

    #define MANDATORY true
    #define OPTIONAL false
    #define ARG_REQUIRED true
    #define ARG_IS_OPTIONAL false

    ConfigChecker cc;
    cc.addStringParam("config", OPTIONAL, ARG_REQUIRED, "Default config file name ");
    cc.addIntegerParam("mixtureDistribCount", MANDATORY, ARG_REQUIRED, "Distribution count for a mixture");
    cc.addIntegerParam("topDistribsCount", MANDATORY, ARG_REQUIRED);
    cc.addStringParam("computeLLKWithTopDistribs", MANDATORY, ARG_REQUIRED);

    cc.addStringParam("featureFilesPath", MANDATORY, ARG_IS_OPTIONAL);
    cc.addStringParam("loadFeatureFileFormat", MANDATORY, ARG_REQUIRED);
    cc.addStringParam("loadFeatureFileExtension", MANDATORY, ARG_IS_OPTIONAL);
    cc.addIntegerParam("featureServerMemAlloc", MANDATORY, ARG_REQUIRED);

    cc.addStringParam("mixtureFilesPath", MANDATORY, ARG_IS_OPTIONAL);
    cc.addStringParam("loadMixtureFileFormat", MANDATORY, ARG_REQUIRED);
    cc.addStringParam("loadMixtureFileExtension", MANDATORY, ARG_IS_OPTIONAL);
    cc.addStringParam("saveMixtureFileFormat", MANDATORY, ARG_REQUIRED);
    cc.addStringParam("saveMixtureFileExtension", MANDATORY, ARG_IS_OPTIONAL);

    cc.addFloatParam("minLLK", MANDATORY, ARG_REQUIRED);
    cc.addFloatParam("maxLLK", MANDATORY, ARG_REQUIRED);
    cc.addStringParam("distribType", MANDATORY, ARG_REQUIRED);

    // Gets options on command line

    CmdLine cmdLine(argc, argv);

    // Deals with option --help

    if (cmdLine.displayHelpRequired())
      cout << "test 1.0" << endl
           << "Usage: myprog [option]..." << endl << endl
           << "       myprog is a speech signal processing toolkit which provides" << endl
           << "       runtime commands implementing standard feature extraction" << endl
           << "       algorithms for speech and speaker recognition applications" << endl
           << "       and a C library to implement new algorithms and to use myprog" << endl
           << "       files within your own programs" << endl << endl
           << cc.getParamList();

    // Deals with option --version

    else if (cmdLine.displayVersionRequired())
      cout << "test\n 1.0" << endl;

    // 

    else
    {
      // Looks for default configuration
      
      Config tmp;
      cmdLine.copyIntoConfig(tmp);
      Config config;
      if (tmp.existsParam("config")) // --config="aConfigfile.xml"
        config.load(tmp.getParam("config"));

      // writes or overwrites config parameters with command line parameters

      cmdLine.copyIntoConfig(config);
      cc.check(config); // throws a ConfigCheckException exception if an error occurs
      if (config.getParam_debug())
        cout << "mode debug" << endl;
      
      // performs job

      cout << endl << "-------------- Start example1 -----------" << endl << endl;

      #if !defined NDEBUG
      cout << "debug mode" << endl << endl;
      #else
      cout << "release mode" << endl << endl;
      #endif

      WorldInit     init(config);
      Score         score(config);
      WorldTraining train(config);

      cout << endl << "-------------- End example1 -------------" << endl;
    }
  }
  catch (ConfigCheckException& e)
  {
    cout << e.msg << endl
         << "Try test --help for more informations" << endl;
  } 
  catch (Exception& e)
  {
    cout << e.toString() << endl;
  }

#ifdef WIN32
  int xx;
  cin >> xx;
#endif
  return 0;
}

      /*config.setParam("mixtureDistribCount", "128");
      config.setParam("topDistribsCount", "64");
      config.setParam("computeLLKWithTopDistribs", "PARTIAL"); // or "PARTIAL"
      config.setParam("featureFilesPath", baseDir + "data/prm/");
      config.setParam("loadFeatureFileFormat", "SPRO3");
      config.setParam("loadFeatureFileExtension", ".prm");
      config.setParam("featureServerMemAlloc", "10000000");
      config.setParam("mixtureFilesPath", baseDir + "data/gmm/");
      config.setParam("loadMixtureFileFormat", "AMIRAL");
      config.setParam("loadMixtureFileExtension", ".gmm");
      config.setParam("saveMixtureFileFormat", "XML");
      config.setParam("saveMixtureFileExtension", ".xml");
      config.setParam("minLLK", "-100");
      config.setParam("maxLLK", "100");
      config.setParam("distribType", "GD");*/

    /*FeatureFileReaderSPro4 r("./../../../../xx.prm", config);
    Feature f;
    while (r.readFeature(f))
      cout << f.toString() << endl;*/
