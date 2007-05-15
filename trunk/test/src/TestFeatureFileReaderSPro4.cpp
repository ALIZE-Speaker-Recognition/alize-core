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

#if !defined(ALIZE_TestFeatureFileReaderSPro4_cpp)
#define ALIZE_TestFeatureFileReaderSPro4_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestFeatureFileReaderSPro4.h"
#include "FeatureFileReaderSPro4.h"
#include "alizeString.h"
#include "Feature.h"
#include "Config.h"
#include "FileReader.h"
#include <cstdio>
#include <iostream>

using namespace alize;

//-------------------------------------------------------------------------
TestFeatureFileReaderSPro4::TestFeatureFileReaderSPro4()
:Test() {}
//-------------------------------------------------------------------------
bool TestFeatureFileReaderSPro4::exec()
{
  typedef FeatureFileReaderSPro4 R;

#ifdef WIN32
    String dataDir = "./../../data/";
#else
    String dataDir = "./../data/";
#endif
    const String file(dataDir + "prm/testSPro4.prm");
  Test::exec();
  {
    // test method : String getClassName() const
    {
      Config c;
      c.setParam("bigEndian", "false");
      test(R(file, c).getClassName() == "FeatureFileReaderSPro4", __LINE__);
    }

    // test method : explicit FeatureFileReaderSPro4(const String& f,
    //         const Config& c, LabelServer* ls = NULL);
    {
      Config c;
      c.setParam("bigEndian", "false");
      R r(file, c);
    }
  
    // test method : static FeatureFileReaderSPro4& create(const FileName& f,
    //         const Config& c, LabelServer* ls = NULL);
    {
      Config c;
      c.setParam("bigEndian", "false");
      R r(file, c);
    }

    // test method : virtual bool readFeature(Feature&, unsigned long step)
    {
      Config c;
      c.setParam("bigEndian", "false");
      R r(file, c);
      //for (unsigned long i=0; i<5000; i++)
      {
        r.reset();
        unsigned long cpt=0;
        Feature g;
        try
        {
          while (r.readFeature(g))
          {
            test(g.getVectSize() == r.getVectSize(), __LINE__);
            test(g.isValid(), __LINE__);
            cpt++;
          }
        }
        catch (EOFException&) { showError(__LINE__); }
        test(cpt == r.getFeatureCount(), __LINE__);
      }
    }
    // test seekFeature(...) suivi d'un readFeature(...)
    {
      Config c;
      c.setParam("bigEndian", "false");
      R r(file, c);
      Feature f0(r.getVectSize());
      Feature f5(r.getVectSize());
      Feature f11(r.getVectSize());
      Feature g, t;
      unsigned long i=0;
      r.seekFeature(0);
      while (r.readFeature(g))
      {
        if (i==0)
          f0 = g;
        else if (i==5)
          f5 = g;
        else if (i==11)
          f11 = g;
        i++;
      }
      r.seekFeature(0);
      test(r.readFeature(t) && t == f0, __LINE__);
      r.seekFeature(5);
      test(r.readFeature(g) && g == f5, __LINE__);
      r.seekFeature(11);
      test(r.readFeature(g) && g == f11, __LINE__);
      r.seekFeature(r.getFeatureCount());
      test(!r.readFeature(g), __LINE__);
      r.seekFeature(12456789);
      test(!r.readFeature(g), __LINE__);
    }

    // test method : virtual void reset();
    {
      Config c;
      c.setParam("bigEndian", "false");
      R r(file, c);
      Feature f, g;
      r.readFeature(f);
      r.readFeature(g);
      r.reset();
      test(r.readFeature(g) && g == f, __LINE__);
    }

    // test method : virtual unsigned long getFeatureCount();
    {
      Config c;
      c.setParam("bigEndian", "false");
      R r(file, c);
      test(r.getFeatureCount() == 2000, __LINE__);
    }

    // test method : virtual unsigned long getVectSize();
    {
      Config c;
      c.setParam("bigEndian", "false");
      R r(file, c);
      test(r.getVectSize() == 37, __LINE__);
    }

    // test method : virtual const FeatureFlags& getFeatureFlags();
    {
      Config c;
      c.setParam("bigEndian", "false");
      FeatureFlags f("110000");
      R r(file, c);
      test(r.getFeatureFlags() == f, __LINE__);



      /*c.setParam("saveFeatureFileFormat", "SPRO3");
      c.setParam("saveFeatureFileSPro3DataKind", "LPCEPSTRA");
      c.setParam("loadFeatureFileFormat", "SPRO3");
      c.setParam("featureFlags", "110000");
      FeatureFileWriter w(dataDir + "prm/testSPro3.prm", c);
      for (unsigned long i=0; i<r.getFeatureCount(); i++)
        w.writeFeature(*r.readFeature());*/
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

  } // local objects are deleted here 

  return endTest();
}
//-------------------------------------------------------------------------
String TestFeatureFileReaderSPro4::getClassName() const
{ return "TestFeatureFileReaderSPro4"; }
//-------------------------------------------------------------------------
TestFeatureFileReaderSPro4::~TestFeatureFileReaderSPro4() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFeatureFileReaderSPro4_cpp)
