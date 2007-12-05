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

#if !defined(ALIZE_TestConfigFileReader_cpp)
#define ALIZE_TestConfigFileReader_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestConfigFileReader.h"
#include "Config.h"
#include "ConfigFileReaderXml.h"
#include <cstdio>
#include "alizeString.h"
#include <iostream>

using namespace std;
using namespace alize;

//-------------------------------------------------------------------------
TestConfigFileReader::TestConfigFileReader()
:Test() {}
//-------------------------------------------------------------------------
bool TestConfigFileReader::exec()
{
   Test::exec();

    {
        const String file("./TestConfigFileReader.xml");

        // test method : String getClassName()
        {
            test(ConfigFileReaderXml(file).getClassName()
                                    == "ConfigFileReaderXml", __LINE__);
        }

        // test method : explicit ConfigFileReaderXml(const String& f)
        {
            ConfigFileReaderXml r(file);
        }

        // test method : virtual void readConfig(Config&);
        {
            FILE* f = fopen(file.c_str(), "w");
            char buffer[] =
"<config version=\"1\">\n\
  <param name=\"vectSize\">32</param>\n\
  <param name=\"mixtureDistribCount\">128</param>\n\
  <param name=\"featureFlags\">110000</param>\n\
  <param name=\"maxLLK\">1e10</param>\n\
  <param name=\"minLLK\">-1e10</param>\n\
  <param name=\"bigEndian\">false</param>\n\
  <param name=\"sampleRate\">100.0</param>\n\
  <param name=\"saveMixtureFileFormat\">XML</param>\n\
  <param name=\"loadFeatureFileFormat\">SPRO3</param>\n\
  <param name=\"loadMixtureFileFormat\">AMIRAL</param>\n\
</config>";
            fwrite(&buffer, sizeof(buffer)-1, 1, f);
            fclose(f);
            //
            ConfigFileReaderXml r(file);
            Config c, c1;
            c1.setParam("vectSize", "32");
            c1.setParam("mixtureDistribCount", "128");
            c1.setParam("featureFlags", "110000");
            c1.setParam("maxLLK", "1e10");
            c1.setParam("minLLK", "-1e10");
            c1.setParam("bigEndian", "false");
            c1.setParam("sampleRate", "100.0");
            c1.setParam("saveMixtureFileFormat", "XML");
            c1.setParam("loadFeatureFileFormat", "SPRO3");
            c1.setParam("loadMixtureFileFormat", "AMIRAL");
            r.readConfig(c);
            test(c1 == c, __LINE__);
            //cout << c1.toString() << endl;
            //cout << c.toString() << endl;
            remove(file.c_str());

            // TODO : more serious tests...

        }

    } // local objects are deleted here 

    return endTest();
}
//-------------------------------------------------------------------------
String TestConfigFileReader::getClassName() const
{ return "TestConfigFileReader"; }
//-------------------------------------------------------------------------
TestConfigFileReader::~TestConfigFileReader() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestConfigFileReader_cpp)
