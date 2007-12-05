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

#if !defined(ALIZE_TestFeatureFileReader_cpp)
#define ALIZE_TestFeatureFileReader_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestFeatureFileReader.h"
#include "FeatureFileReader.h"
#include "alizeString.h"
#include "Config.h"
#include "AutoDestructor.h"
#include "Feature.h"
#include "FeatureFileReaderRaw.h"
#include "FileReader.h"
#include <cstdio>
#include <iostream>

using namespace alize;
using namespace std;
typedef FeatureFileReader R;

//-------------------------------------------------------------------------
TestFeatureFileReader::TestFeatureFileReader()
:Test() {}
//-------------------------------------------------------------------------
bool TestFeatureFileReader::exec()
{
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
      c.setParam("loadFeatureFileFormat", "HTK");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      test(R(file, c).getClassName() == "FeatureFileReader", __LINE__);
    }

    // test method : explicit FeatureFileReader(const FileName& f,
    //         const Config& c, LabelServer* ls = NULL);
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      R r(file, c);
    }
  
    // test method : static FeatureFileReader& create(const FileName& f,
    //         const Config& c, LabelServer* ls = NULL);
    {
      Config c;
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      FeatureInputStream& r = R::create(file, c);
      AutoDestructor ad(r);
    }

    // test method : virtual bool readFeature(Feature&, unsigned long step)
    {
      Config c;
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      FeatureInputStream& r = R::create(file, c);
      AutoDestructor ad(r);
      unsigned long cpt=0;
      Feature f;
      try
      {
        while (r.readFeature(f))
        {
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
      c.setParam("loadFeatureFileFormat", "HTK");
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
      c.setParam("loadFeatureFileFormat", "HTK");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      FeatureInputStream& r = R::create(file, c);
      AutoDestructor ad(r);
      Feature f, f1, f2;
      r.readFeature(f);
      r.readFeature(f1);
      r.reset();
      test(r.readFeature(f2) && f2 == f, __LINE__);
    }

    // test method : virtual unsigned long getFeatureCount();
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      R r(file, c);
      test(r.getFeatureCount() == 89, __LINE__);
    }

    // test method : virtual unsigned long getVectSize();
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      R r(file, c);
      test(r.getVectSize() == 36, __LINE__);
    }

    // test method : virtual const FeatureFlags& getFeatureFlags();
    {
      Config c;
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
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
    // test method : unsigned long getFeatureCountOfASource(unsigned long srcIdx) const;
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      R s0(c);
      try { s0.getFeatureCountOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      R s1(file, c);
      try { test(s1.getFeatureCountOfASource(0) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFeatureCountOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      XLine l;
      l.addElement(file);
      l.addElement(file2);
      R s2(l, c);
      try { test(s2.getFeatureCountOfASource(0) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFeatureCountOfASource(1) == FeatureFileReader(file2,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFeatureCountOfASource(2); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : unsigned long getFeatureCountOfASource(const String& srcName) const;
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      R s0(c);
      try { s0.getFeatureCountOfASource(file); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      R s1(file, c);
      try { test(s1.getFeatureCountOfASource(file) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFeatureCountOfASource(file2); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      XLine l;
      l.addElement(file);
      l.addElement(file2);
      R s2(l, c);
      try { test(s2.getFeatureCountOfASource(file) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFeatureCountOfASource(file2) == FeatureFileReader(file2,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFeatureCountOfASource("kljj"); showError(__LINE__); }
      catch (Exception&) {}
    }

    // test method : virtual unsigned long getSourceCount()
    {
      Config c;
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      const String f1 = dataDir + "prm/TestFeatureFileReaderHTK_PLP.plp";
      const String f2 = dataDir + "prm/TestFeatureFileReaderHTK_PLP_E.plp";
      // no file
      test(R(c).getSourceCount() == 0, __LINE__);
      // 1 file
      test(R(f1, c).getSourceCount() == 1, __LINE__);
      // 2 files
      XLine l;
      l.addElement(f1).addElement(f2);
      test(R(l, c).getSourceCount() == 2, __LINE__);
    }
    // test method : const String& getNameOfASource(unsigned long srcIdx)
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      R s0(c);
      try { s0.getFirstFeatureIndexOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      R s1(file, c);
      try { test(s1.getFirstFeatureIndexOfASource(0) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      XLine l;
      l.addElement(file);
      l.addElement(file2);
      R s2(l, c);
      try { test(s2.getFirstFeatureIndexOfASource(0) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFirstFeatureIndexOfASource(1) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFirstFeatureIndexOfASource(2); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : unsigned long getFirstFeatureIndexOfASource(
    //                                       unsigned long srcIdx) const
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      R s0(c);
      try { s0.getFirstFeatureIndexOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      R s1(file, c);
      try { test(s1.getFirstFeatureIndexOfASource(0) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      XLine l;
      l.addElement(file);
      l.addElement(file2);
      R s2(l, c);
      try { test(s2.getFirstFeatureIndexOfASource(0) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFirstFeatureIndexOfASource(1) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFirstFeatureIndexOfASource(2); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : unsigned long getFirstFeatureIndexOfASource(
    //                                       const FileName&) const
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      R s0(c);
      try { s0.getFirstFeatureIndexOfASource(file); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      FeatureFileReader s1(file, c);
      try { test(s1.getFirstFeatureIndexOfASource(file) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(file2); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      XLine l;
      l.addElement(file);
      l.addElement(file2);
      R s2(l, c);
      try { test(s2.getFirstFeatureIndexOfASource(file) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFirstFeatureIndexOfASource(file2) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFirstFeatureIndexOfASource("hhghhg"); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test le choix de big/little endian
    #define CAST dynamic_cast<FeatureFileReaderSingle*>
    for (unsigned long i=0; i<3; i++)
    {
      String format;
      if (i==0) format = "RAW";
      if (i==1) format = "SPRO4";
      if (i==2) format = "HTK";
      {
        Config c;
        c.setParam("loadFeatureFileFormat", "RAW");
        R r1("./bidon", c);
        test((CAST(r1._pFeatureReader))->_pReader->_swap == false, __LINE__);
        R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
        test((CAST(r2._pFeatureReader))->_pReader->_swap == false, __LINE__);
        R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
        test((CAST(r3._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
        test((CAST(r4._pFeatureReader))->_pReader->_swap == false, __LINE__);
      }
      {
        Config c;
        c.setParam("loadFeatureFileFormat", "RAW");
        c.setParam("loadFeatureFileBigEndian", "true");
        R r1("./bidon", c);
        test((CAST(r1._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
        test((CAST(r2._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
        test((CAST(r3._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
        test((CAST(r4._pFeatureReader))->_pReader->_swap == false, __LINE__);
      }
      {
        Config c;
        c.setParam("loadFeatureFileFormat", "RAW");
        c.setParam("loadFeatureFileBigEndian", "false");
        R r1("./bidon", c);
        test((CAST(r1._pFeatureReader))->_pReader->_swap == false, __LINE__);
        R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
        test((CAST(r2._pFeatureReader))->_pReader->_swap == false, __LINE__);
        R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
        test((CAST(r3._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
        test((CAST(r4._pFeatureReader))->_pReader->_swap == false, __LINE__);
      }
      {
        Config c;
        c.setParam("loadFeatureFileFormat", "RAW");
        c.setParam("bigEndian", "true");
        R r1("./bidon", c);
        test((CAST(r1._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
        test((CAST(r2._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
        test((CAST(r3._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
        test((CAST(r4._pFeatureReader))->_pReader->_swap == false, __LINE__);
      }
      {
        Config c;
        c.setParam("loadFeatureFileFormat", "RAW");
        c.setParam("bigEndian", "true");
        c.setParam("loadFeatureFileBigEndian", "false");
        R r1("./bidon", c);
        test((CAST(r1._pFeatureReader))->_pReader->_swap == false, __LINE__);
        R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
        test((CAST(r2._pFeatureReader))->_pReader->_swap == false, __LINE__);
        R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
        test((CAST(r3._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
        test((CAST(r4._pFeatureReader))->_pReader->_swap == false, __LINE__);
      }
      {
        Config c;
        c.setParam("loadFeatureFileFormat", "RAW");
        c.setParam("bigEndian", "true");
        c.setParam("loadFeatureFileBigEndian", "true");
        R r1("./bidon", c);
        test((CAST(r1._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
        test((CAST(r2._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
        test((CAST(r3._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
        test((CAST(r4._pFeatureReader))->_pReader->_swap == false, __LINE__);
      }
      {
        Config c;
        c.setParam("loadFeatureFileFormat", "RAW");
        c.setParam("bigEndian", "false");
        c.setParam("loadFeatureFileBigEndian", "false");
        R r1("./bidon", c);
        test((CAST(r1._pFeatureReader))->_pReader->_swap == false, __LINE__);
        R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
        test((CAST(r2._pFeatureReader))->_pReader->_swap == false, __LINE__);
        R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
        test((CAST(r3._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
        test((CAST(r4._pFeatureReader))->_pReader->_swap == false, __LINE__);
      }
      {
        Config c;
        c.setParam("loadFeatureFileFormat", format);
        c.setParam("bigEndian", "false");
        c.setParam("loadFeatureFileBigEndian", "true");
        R r1("./bidon", c);
        test((CAST(r1._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r2("./bidon", c, NULL, BIGENDIAN_AUTO);
        test((CAST(r2._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r3("./bidon", c, NULL, BIGENDIAN_TRUE);
        test((CAST(r3._pFeatureReader))->_pReader->_swap == true, __LINE__);
        R r4("./bidon", c, NULL, BIGENDIAN_FALSE);
        test((CAST(r4._pFeatureReader))->_pReader->_swap == false, __LINE__);
      }
    }
    // test method : bool writeFeature(const Feature& f, unsigned long step)
    // test whether feature vectSize != file vectSize
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      c.setParam("featureServerMode", "FEATURE_WRITABLE");
      c.setParam("loadFeatureFileMemAlloc","7104"); // 74*24*4
      XLine l;
      FeatureFileReader r(file, c);
      Feature f(r.getVectSize()+1);
      try { r.writeFeature(f); showError(__LINE__); }
      catch (Exception&) {}
    }
    // other tests for method : bool writeFeature(const Feature& f,
    //                                            unsigned long step)
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      c.setParam("featureServerMode", "FEATURE_WRITABLE");
      c.setParam("loadFeatureFileMemAlloc","7104"); // 74*24*4
      XLine l;
      FeatureFileReader r(file, c);
      Feature f;
      unsigned long i=0, j;
      while (r.readFeature(f,0))
      {
        for (j=0; j<f.getVectSize(); j++)
          f[j] = i+j;
        r.writeFeature(f);
        i++;
      }
      r.reset();
      i = 0;
      while (r.readFeature(f))
      {
        for (j=0; j<f.getVectSize(); j++)
          if (f[j] != i+j)
          {
            showError(__LINE__);
            goto sortie;
          }
        i++;
      }
sortie:
      ;
    }

  } // local objects are deleted here 

  return endTest();
}
//-------------------------------------------------------------------------
String TestFeatureFileReader::getClassName() const
{ return "TestFeatureFileReader"; }
//-------------------------------------------------------------------------
TestFeatureFileReader::~TestFeatureFileReader() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFeatureFileReader_cpp)
