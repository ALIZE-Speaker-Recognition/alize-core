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

#if !defined(ALIZE_TestFeatureFileReaderHTK_cpp)
#define ALIZE_TestFeatureFileReaderHTK_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestFeatureFileReaderHTK.h"
#include "FeatureFileReaderHTK.h"
#include "alizeString.h"
#include "Config.h"
#include "AutoDestructor.h"
#include "Feature.h"
#include "FileReader.h"
#include <cstdio>
#include <iostream>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestFeatureFileReaderHTK::TestFeatureFileReaderHTK()
:Test() {}
//-------------------------------------------------------------------------
bool TestFeatureFileReaderHTK::exec()
{
  typedef FeatureFileReaderHTK R;

  Test::exec();
  {
#ifdef WIN32
    String dataDir = "./../../data/";
#else
    String dataDir = "./../data/";
#endif

    // test method : String getClassName() const
    {
      Config c;
      c.setParam("bigEndian", "true");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      test(R(file, c).getClassName()
                  == "FeatureFileReaderHTK", __LINE__);
    }

    // test method : explicit FeatureFileReaderHTK(const FileName& f,
    //         const Config& c, LabelServer* ls = NULL);
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      R r(file, c);
    }
  
    // test method : static FeatureFileReaderHTK& create(const FileName& f,
    //         const Config& c, LabelServer* ls = NULL);
    {
      Config c;
      c.setParam("bigEndian", "true");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      FeatureInputStream& r = R::create(file, c);
      AutoDestructor ad(r);
    }

    // test method : virtual bool readFeature(Feature&, unsigned long step)
    {
      Config c;
      c.setParam("bigEndian", "true");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      R r(file, c);
      unsigned long cpt=0;
      Feature f;
      try
      {
        while (r.readFeature(f))
        {
          //std::cout << p->toString() << std::endl;
          test(f.getVectSize() == r.getVectSize(), __LINE__);
          test(f.isValid(), __LINE__);
          cpt++;
        }
      }
      catch (EOFException&) { showError(__LINE__); }
      test(cpt == r.getFeatureCount(), __LINE__);
    }

    // test seekFeature(...) suivi d'un readFeature(...)
    {
      Config c;
      c.setParam("bigEndian", "true");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      R r(file, c);
      Feature f0(r.getVectSize());
      Feature f5(r.getVectSize());
      Feature f11(r.getVectSize());
      Feature f;
      unsigned long i=0;
      while (r.readFeature(f))
      {
        if (i==0)
          f0 = f;
        else if (i==5)
          f5 = f;
        else if (i==11)
          f11 = f;
        i++;
      }
      r.seekFeature(0);
      test(r.readFeature(f) && f == f0, __LINE__);
      r.seekFeature(5);
      test(r.readFeature(f) && f == f5, __LINE__);
      r.seekFeature(11);
      test(r.readFeature(f) && f == f11, __LINE__);
      r.seekFeature(89);
      test(!r.readFeature(f), __LINE__);
      r.seekFeature(123456);
      test(!r.readFeature(f), __LINE__);
    }

    // test method : virtual void reset();
    {
      Config c;
      c.setParam("bigEndian", "true");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      FeatureInputStream& r = R::create(file, c);
      AutoDestructor ad(r);
      Feature f, g;
      r.readFeature(f);
      r.readFeature(g);
      r.reset();
      test(r.readFeature(g) && g == f, __LINE__);
    }

    // test method : virtual unsigned long getFeatureCount();
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      R r(file, c);
      test(r.getFeatureCount() == 89, __LINE__);
    }

    // test method : virtual unsigned long getVectSize();
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      R r(file, c);
      test(r.getVectSize() == 36, __LINE__);
    }

    // test method : virtual const FeatureFlags& getFeatureFlags();
    {
      Config c;
      c.setParam("bigEndian", "true");
      R r1(dataDir + "prm/TestFeatureFileReaderHTK_PLP.plp", c);
      R r2(dataDir + "prm/TestFeatureFileReaderHTK_PLP_E.plp", c);
      R r3(dataDir + "prm/TestFeatureFileReaderHTK_PLP_D.plp", c);
      R r4(dataDir + "prm/TestFeatureFileReaderHTK_PLP_E_D.plp", c);
      R r5(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp", c);
      test(r1.getFeatureFlags().getString() == "100000", __LINE__);
      test(r2.getFeatureFlags().getString() == "110000", __LINE__);
      test(r3.getFeatureFlags().getString() == "101000", __LINE__);
      test(r4.getFeatureFlags().getString() == "111100", __LINE__);
      test(r5.getFeatureFlags().getString() == "101010", __LINE__);
    }
    // test method : const String& getNameOfASource(unsigned long srcIdx)
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      c.setParam("bigEndian", "true");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      R s1(file, c);
      try { test(s1.getNameOfASource(0) == file, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : unsigned long getFeatureCountOfASource(unsigned long srcIdx) const;
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      c.setParam("bigEndian", "true");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      R s1(file, c);
      try { test(s1.getFeatureCountOfASource(0) == R(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFeatureCountOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : unsigned long getFeatureCountOfASource(const String& srcName) const;
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      c.setParam("bigEndian", "true");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      R s1(file, c);
      try { test(s1.getFeatureCountOfASource(file) == R(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFeatureCountOfASource("kljkj"); showError(__LINE__); }
      catch (Exception&) {}
    }

    // test method : virtual unsigned long getSourceCount()
    {
      Config c;
      c.setParam("bigEndian", "true");
      const String f1 = dataDir + "prm/TestFeatureFileReaderHTK_PLP.plp";
      const String f2 = dataDir + "prm/TestFeatureFileReaderHTK_PLP_E.plp";
      test(R(f1, c).getSourceCount() == 1, __LINE__);
    }
    // test method : virtual unsigned long getFirstFeatureIndexOfASource(
    //                                      unsigned long srcIdx) const
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("bigEndian", "true");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      R s1(file, c);
      try { test(s1.getFirstFeatureIndexOfASource(0) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : virtual unsigned long getFirstFeatureIndexOfASource(
    //                                      const FileName&) const
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      c.setParam("bigEndian", "true");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      R s1(file, c);
      try { test(s1.getFirstFeatureIndexOfASource(file) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource("kjhjkh"); showError(__LINE__); }
      catch (Exception&) {}
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
String TestFeatureFileReaderHTK::getClassName() const
{ return "TestFeatureFileReaderHTK"; }
//-------------------------------------------------------------------------
TestFeatureFileReaderHTK::~TestFeatureFileReaderHTK() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFeatureFileReaderHTK_cpp)
