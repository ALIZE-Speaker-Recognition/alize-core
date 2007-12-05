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

#if !defined(ALIZE_TestFeatureFileReaderRaw_cpp)
#define ALIZE_TestFeatureFileReaderRaw_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestFeatureFileReaderRaw.h"
#include "FeatureFileReaderRaw.h"
#include "alizeString.h"
#include "Config.h"
#include "FileReader.h"
#include <cstdio>
#include <iostream>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestFeatureFileReaderRaw::TestFeatureFileReaderRaw()
:Test() {}
//-------------------------------------------------------------------------
bool TestFeatureFileReaderRaw::exec()
{
  typedef FeatureFileReaderRaw R;

  Test::exec();

  {
    const String file("./TestFeatureFileReaderRaw.tmp");
    // test getClassName()
    Config c;
    c.setParam("vectSize", "2");
    c.setParam("sampleRate", "101");
    c.setParam("featureFlags", "100000");
    test(R(file, c).getClassName() == "FeatureFileReaderRaw", __LINE__);

    // test method : explicit FeatureFileReaderRaw(const FileName& f,
    //                          const Config&);
    {
      try { c.setParam("vectSize", "0"); showError(__LINE__); }
      catch (Exception&) {};
      c.setParam("vectSize", "1");
      R r1(file, c);
      c.setParam("vectSize", "2");
      R r2(file, c);
      // a completer
    }

    // test method : virtual bool readFeature(Feature&, unsigned long step)
    {
      FILE* f = fopen(file.c_str(), "w");
      float v1 = 1.2f;
      float v2 = 3.4f;
      fwrite(&v1, sizeof(v1), 1, f);
      fwrite(&v2, sizeof(v2), 1, f);
      fclose(f);
      //
      c.setParam("vectSize", "2");
      R r1(file, c);
      Feature g;
      bool ok = r1.readFeature(g);
      test(r1.getError() == FeatureInputStream::NO_ERROR, __LINE__);
      if (ok)
      {
        test(g.isValid(), __LINE__);
        test(g[0] == v1, __LINE__);
        test(g[1] == v2, __LINE__);
      }
      else
        showError(__LINE__);
      // try to read an other feature 
      test(!r1.readFeature(g), __LINE__);
      r1.close();
      // try to read feature with vectSize = 3
      c.setParam("vectSize", "3");
      R r2(file, c);
      try { r2.readFeature(g); showError(__LINE__); }
      catch (InvalidDataException&) {}
      r2.close();

      remove(file.c_str());
    }
    // test seekFeature(...) suivi d'un readFeature(...)
    {
      FILE* f = fopen(file.c_str(), "w");
      Feature g;
      float v1 = 1.0f;
      float v2 = 2.0f;
      float v3 = 3.0f;
      float v4 = 4.0f;
      float v5 = 5.0f;
      float v6 = 6.0f;
      fwrite(&v1, sizeof(v1), 1, f);
      fwrite(&v2, sizeof(v2), 1, f);
      fwrite(&v3, sizeof(v3), 1, f);
      fwrite(&v4, sizeof(v4), 1, f);
      fwrite(&v5, sizeof(v5), 1, f);
      fwrite(&v6, sizeof(v6), 1, f);
      fclose(f);
      //
      c.setParam("vectSize", "2");
      R r1(file, c);
      r1.seekFeature(0);
      test(r1.readFeature(g) && g[0] == 1.0f, __LINE__);
      test(r1.readFeature(g) && g[0] == 3.0f, __LINE__);
      test(r1.readFeature(g) && g[0] == 5.0f, __LINE__);
      r1.seekFeature(1);
      test(r1.readFeature(g) && g[0] == 3.0f, __LINE__);
      test(r1.readFeature(g) && g[0] == 5.0f, __LINE__);
      r1.seekFeature(2);
      test(r1.readFeature(g) && g[0] == 5.0f, __LINE__);
      r1.seekFeature(3);
      test(!r1.readFeature(g), __LINE__);
      r1.seekFeature(123456);
      test(!r1.readFeature(g), __LINE__);

      r1.close();
      remove(file.c_str());
    }

    // test method : virtual unsigned long getFeatureCount();
    {
      FILE* f = fopen(file.c_str(), "w");
      float v1 = 1.2f;
      float v2 = 3.4f;
      fwrite(&v1, sizeof(v1), 1, f);
      fwrite(&v2, sizeof(v2), 1, f);
      fwrite(&v1, sizeof(v1), 1, f);
      fwrite(&v2, sizeof(v2), 1, f);
      fclose(f);
      c.setParam("vectSize", "2"); // Must define vectSize
      R r(file, c);
      test(r.getFeatureCount() == 2, __LINE__);
      r.close();
      remove(file.c_str());
    }
    // test le choix de big/little endian
    {
      Config c;
      R r1("./bidon", c);
      test(r1._pReader->_swap == false, __LINE__);
      R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
      test(r2._pReader->_swap == false, __LINE__);
      R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
      test(r3._pReader->_swap == true, __LINE__);
      R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
      test(r4._pReader->_swap == false, __LINE__);
    }
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      R r1("./bidon", c);
      test(r1._pReader->_swap == true, __LINE__);
      R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
      test(r2._pReader->_swap == true, __LINE__);
      R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
      test(r3._pReader->_swap == true, __LINE__);
      R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
      test(r4._pReader->_swap == false, __LINE__);
    }
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "false");
      R r1("./bidon", c);
      test(r1._pReader->_swap == false, __LINE__);
      R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
      test(r2._pReader->_swap == false, __LINE__);
      R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
      test(r3._pReader->_swap == true, __LINE__);
      R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
      test(r4._pReader->_swap == false, __LINE__);
    }
    {
      Config c;
      c.setParam("bigEndian", "true");
      R r1("./bidon", c);
      test(r1._pReader->_swap == true, __LINE__);
      R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
      test(r2._pReader->_swap == true, __LINE__);
      R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
      test(r3._pReader->_swap == true, __LINE__);
      R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
      test(r4._pReader->_swap == false, __LINE__);
    }
    {
      Config c;
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileBigEndian", "false");
      R r1("./bidon", c);
      test(r1._pReader->_swap == false, __LINE__);
      R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
      test(r2._pReader->_swap == false, __LINE__);
      R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
      test(r3._pReader->_swap == true, __LINE__);
      R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
      test(r4._pReader->_swap == false, __LINE__);
    }
    {
      Config c;
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileBigEndian", "true");
      R r1("./bidon", c);
      test(r1._pReader->_swap == true, __LINE__);
      R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
      test(r2._pReader->_swap == true, __LINE__);
      R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
      test(r3._pReader->_swap == true, __LINE__);
      R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
      test(r4._pReader->_swap == false, __LINE__);
    }
    {
      Config c;
      c.setParam("bigEndian", "false");
      c.setParam("loadFeatureFileBigEndian", "false");
      R r1("./bidon", c);
      test(r1._pReader->_swap == false, __LINE__);
      R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
      test(r2._pReader->_swap == false, __LINE__);
      R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
      test(r3._pReader->_swap == true, __LINE__);
      R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
      test(r4._pReader->_swap == false, __LINE__);
    }
    {
      Config c;
      c.setParam("bigEndian", "false");
      c.setParam("loadFeatureFileBigEndian", "true");
      R r1("./bidon", c);
      test(r1._pReader->_swap == true, __LINE__);
      R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
      test(r2._pReader->_swap == true, __LINE__);
      R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
      test(r3._pReader->_swap == true, __LINE__);
      R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
      test(r4._pReader->_swap == false, __LINE__);
    }

    // test with mask
    {
      FILE* f = fopen(file.c_str(), "w");
      Feature g;
      float v1 = 1.0f;
      float v2 = 2.0f;
      float v3 = 3.0f;
      float v4 = 4.0f;
      float v5 = 5.0f;
      float v6 = 6.0f;
      fwrite(&v1, sizeof(v1), 1, f);
      fwrite(&v2, sizeof(v2), 1, f);
      fwrite(&v3, sizeof(v3), 1, f);
      fwrite(&v4, sizeof(v4), 1, f);
      fwrite(&v5, sizeof(v5), 1, f);
      fwrite(&v6, sizeof(v6), 1, f);
      fclose(f);
      try
      {
        Config c;
        c.setParam("loadFeatureFileVectSize" , "6");
        c.setParam("vectSize" , "6");
        R r(file, c);
        Feature f;
        while (r.readFeature(f))
          ;//cout << f.toString() << endl;

        // test à finir
      }
      catch (Exception&)
      {
        remove(file.c_str());
        throw; // do not use 'throw e'
      }
      remove(file.c_str());
    }

  } // local objects are deleted here 

  return endTest();
}
//-------------------------------------------------------------------------
String TestFeatureFileReaderRaw::getClassName() const
{ return "TestFeatureFileReaderRaw"; }
//-------------------------------------------------------------------------
TestFeatureFileReaderRaw::~TestFeatureFileReaderRaw() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFeatureFileReaderRaw_cpp)
