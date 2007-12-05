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

#if !defined(ALIZE_TestFeatureMultipleFileReader_cpp)
#define ALIZE_TestFeatureMultipleFileReader_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "TestFeatureMultipleFileReader.h"
#include "FeatureMultipleFileReader.h"
#include "alizeString.h"
#include "Config.h"
#include "AutoDestructor.h"
#include "Feature.h"
#include "XLine.h"
#include "FeatureFileReader.h"

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestFeatureMultipleFileReader::TestFeatureMultipleFileReader()
:Test() {}
//-------------------------------------------------------------------------
bool TestFeatureMultipleFileReader::exec()
{
  typedef FeatureMultipleFileReader R;
#ifdef WIN32
    String dataDir = "./../../data/";
#else
    String dataDir = "./../data/";
#endif
  Test::exec();
  {
    // test method : String getClassName() const
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      XLine fileList;
      test(R(fileList, c).getClassName()
                  == "FeatureMultipleFileReader", __LINE__);
    }

    // test method : explicit FeatureMultipleFileReader(const XLine&
    //                  fileList, const Config& c, LabelServer* ls = NULL);
    {
      Config c;
      XLine fileList;
      c.setParam("loadFeatureFileBigEndian", "true");
      R r(fileList, c);
    }
  
    // test method : static FeatureMultipleFileReader& create(const FileName& f,
    //         const Config& c, LabelServer* ls = NULL);
    {
      Config c;
      XLine fileList;
      FeatureInputStream& r = R::create(fileList, c);
      AutoDestructor ad(r);
    }

    // test method : virtual bool readFeature(Feature&, unsigned long step)

    // test for 0 file
    {
      Config c;
      XLine fileList;
      R r(fileList, c);
      Feature g;
      test (!r.readFeature(g), __LINE__);
    }
    // test for 1 file
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine fileList;
      const String& file = dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp"; 
      fileList.addElement(file);
      FeatureFileReader r1(file, c);
      R r(fileList, c);
      test(r.getFeatureCount() == r1.getFeatureCount(), __LINE__);
      test(r.getVectSize() == r1.getVectSize(), __LINE__);
      Feature g, f;
      try
      {
        while (r.readFeature(g))
        {
          test(g.isValid(), __LINE__);
          test(r1.readFeature(f) && g == f, __LINE__);
        }
      }
      catch (EOFException&) { showError(__LINE__); }
    }
    // test for 2 files
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine fileList;
      const String& file = dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp"; 
      const String& file2 = dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp"; 
      fileList.addElement(file);
      fileList.addElement(file2);
      FeatureFileReader r1(file, c);
      FeatureFileReader r2(file, c);
      Config c2(c);
      c2.setParam("loadFeatureFileMemAlloc", "1");
      R r(fileList, c2);
      test(r.getFeatureCount() == (r1.getFeatureCount()
                                 + r2.getFeatureCount()), __LINE__);
      test(r.getVectSize() == r1.getVectSize(), __LINE__);
      Feature f, g;
      try
      {
        while (r1.readFeature(g))
          test(r.readFeature(f) && g == f, __LINE__);
        while (r2.readFeature(g))
          test(r.readFeature(f) && g == f, __LINE__);
      }
      catch (EOFException&) { showError(__LINE__); }
    }

    // test seekFeature(...) suivi d'un readFeature(...)

    // test for 0 file
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine fileList;
      R r(fileList, c);
      r.seekFeature(0);
      Feature g;
      test(!r.readFeature(g), __LINE__);
    }

    // test for 1 file
    {
      Config c;
      unsigned long i;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine fileList;
      fileList.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      R r(fileList, c);
      Feature f0(r.getVectSize());
      Feature f5(r.getVectSize());
      Feature f11(r.getVectSize());
      Feature f88(r.getVectSize());
      Feature g;
      for (i=0; i<r.getFeatureCount(); i++)
      {
        r.readFeature(g);
        if (i==0)
          f0 = g;
        else if (i==5)
          f5 = g;
        else if (i==11)
          f11 = g;
        else if (i==88)
          f88 = g;
      }
      r.seekFeature(0);
      test(r.readFeature(g) && g == f0, __LINE__);
      r.seekFeature(5);
      test(r.readFeature(g) && g == f5, __LINE__);
      r.seekFeature(11);
      test(r.readFeature(g) && g == f11, __LINE__);
      r.seekFeature(88);
      test(r.readFeature(g) && g == f88, __LINE__);
      r.seekFeature(89);
      test(!r.readFeature(g), __LINE__);
    }
    // test for 2 files
    {
      Config c;
      unsigned long i;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine fileList;
      fileList.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      fileList.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      R r(fileList, c);
      Feature f0(r.getVectSize());
      Feature f5 = f0;
      Feature f11 = f0;
      Feature f88 = f0;
      Feature f89 = f0;
      Feature f90 = f0;
      Feature f177 = f0;
      Feature g;
      for (i=0; i<r.getFeatureCount(); i++)
      {
        r.readFeature(g);
        if (i==0)
          f0 = g;
        else if (i==5)
          f5 = g;
        else if (i==11)
          f11 = g;
        else if (i==88)
          f88 = g;
        else if (i==89)
          f89 = g;
        else if (i==90)
          f90 = g;
        else if (i==177)
          f177 = g;
      }
      r.seekFeature(0);
      test(r.readFeature(g) && g == f0, __LINE__);
      r.seekFeature(5);
      test(r.readFeature(g) && g == f5, __LINE__);
      r.seekFeature(11);
      test(r.readFeature(g) && g == f11, __LINE__);
      r.seekFeature(88);
      test(r.readFeature(g) && g == f88, __LINE__);
      r.seekFeature(89);
      test(r.readFeature(g) && g == f89, __LINE__);
      r.seekFeature(90);
      test(r.readFeature(g) && g == f90, __LINE__);
      r.seekFeature(177);
      test(r.readFeature(g) && g == f177, __LINE__);
      r.seekFeature(5);
      test(r.readFeature(g) && g == f5, __LINE__);
      r.seekFeature(88);
      test(r.readFeature(g) && g == f88, __LINE__);
      r.seekFeature(0);
      test(r.readFeature(g) && g == f0, __LINE__);
      r.seekFeature(178);
      test(!r.readFeature(g), __LINE__);
      test(!r.readFeature(g), __LINE__); // twice
      r.seekFeature(12345678);
      test(!r.readFeature(g), __LINE__);
    }

    // test method : virtual void reset();
    // for 0 file
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine fileList;
      R r(fileList, c);
      r.reset();
      Feature f;
      test(!r.readFeature(f), __LINE__);
    }
    // for 1 file
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine fileList;
      fileList.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      R r(fileList, c);
      Feature f, g;
      r.readFeature(f);
      r.readFeature(g);
      r.reset();
      test(r.readFeature(g) && g == f, __LINE__);
    }
    // for 2 files
    {
      Config c;
      unsigned long i;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine fileList;
      fileList.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      fileList.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      R r(fileList, c);
      Feature f, g;
      r.readFeature(f);
      r.reset();
      test(r.readFeature(g) && g == f, __LINE__);
      for (i=0; i<r.getFeatureCount()/2+5; i++)
        r.readFeature(g);
      r.reset();
      test(r.readFeature(g) && g == f, __LINE__);
    }

    // test method : virtual unsigned long getFeatureCount();
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine fileList;
      fileList.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      R r(fileList, c);
      test(r.getFeatureCount() == 89, __LINE__);
    }

    // test method : virtual unsigned long getVectSize();
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine fileList;
      fileList.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      R r(fileList, c);
      test(r.getVectSize() == 36, __LINE__);
      // empty list
      XLine l;
      R r1(l, c);
      try { r1.getVectSize(); showError(__LINE__); }
      catch (Exception&) {}

    }

    // test method : virtual const FeatureFlags& getFeatureFlags();
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      XLine l1, l2, l3, l4, l5;
      l1.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP.plp");
      l2.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_E.plp");
      l3.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_D.plp");
      l4.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_E_D.plp");
      l5.addElement(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      R r1(l1, c), r2(l2, c), r3(l3, c), r4(l4, c), r5(l5, c);
      test(r1.getFeatureFlags().getString() == "100000", __LINE__);
      test(r2.getFeatureFlags().getString() == "110000", __LINE__);
      test(r3.getFeatureFlags().getString() == "101000", __LINE__);
      test(r4.getFeatureFlags().getString() == "111100", __LINE__);
      test(r5.getFeatureFlags().getString() == "101010", __LINE__);
      // empty list
      XLine l;
      R rr(l, c);
      try { rr.getVectSize(); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : const String& getNameOfASource(unsigned long srcIdx)
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      // with no file
      XLine l;
      R s0(l, c);
      try { s0.getNameOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      l.addElement(file);
      R s1(l, c);
      try { test(s1.getNameOfASource(0) == file, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getNameOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      R s2(l, c);
      try { test(s2.getNameOfASource(0) == file, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getNameOfASource(1) == file2, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getNameOfASource(2); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : unsigned long getFeatureCountOfASource(unsigned long srcIdx) const;
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      c.setParam("vectSize", "2"); // just for FeatureServer s0(c);
      // with no file
      XLine l;
      R s0(l, c);
      try { s0.getFeatureCountOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      l.addElement(file);
      R s1(l,c);
      try { test(s1.getFeatureCountOfASource(0) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFeatureCountOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
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
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      c.setParam("vectSize", "2"); // just for FeatureServer s0(c);
      // with no file
      XLine l;
      R s0(l, c);
      try { s0.getFeatureCountOfASource(file); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      l.addElement(file);
      R s1(l,c);
      try { test(s1.getFeatureCountOfASource(file) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFeatureCountOfASource("uihuhu"); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      R s2(l, c);
      try { test(s2.getFeatureCountOfASource(file) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFeatureCountOfASource(file2) == FeatureFileReader(file2,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFeatureCountOfASource("lkklk"); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : virtual unsigned long getSourceCount()
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      const String f1 = dataDir + "prm/TestFeatureFileReaderHTK_PLP.plp";
      const String f2 = dataDir + "prm/TestFeatureFileReaderHTK_PLP_E.plp";
      XLine l;
      // 0 file
      test(R(l, c).getSourceCount() == 0, __LINE__);
      // 1 file
      l.addElement(f1);
      test(R(l, c).getSourceCount() == 1, __LINE__);
      // 2 files
      l.addElement(f2);
      test(R(l, c).getSourceCount() == 2, __LINE__);
    }
    // test method : unsigned long getFirstFeatureIndexOfASource(
    //                                          unsigned long srcIdx) const;
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      // with no file
      XLine l;
      R s0(l, c);
      try { s0.getFirstFeatureIndexOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      l.addElement(file);
      R s1(l, c);
      try { test(s1.getFirstFeatureIndexOfASource(0) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
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
    //                                    const FileName&) const;
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      // with no file
      XLine l;
      R s0(l, c);
      try { s0.getFirstFeatureIndexOfASource(file); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      l.addElement(file);
      R s1(l, c);
      try { test(s1.getFirstFeatureIndexOfASource(file) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(file2); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      R s2(l, c);
      try { test(s2.getFirstFeatureIndexOfASource(file) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFirstFeatureIndexOfASource(file2) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFirstFeatureIndexOfASource("kljkljk"); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test le choix de big/little endian
    {
      Config c;
      XLine l;
      l.addElement("./khjkhj");
      R r1(l, c, NULL, BIGENDIAN_TRUE);
      test(r1._bigEndian == BIGENDIAN_TRUE, __LINE__);
      R r2(l, c, NULL, BIGENDIAN_FALSE);
      test(r2._bigEndian == BIGENDIAN_FALSE, __LINE__);
      R r3(l, c, NULL, BIGENDIAN_AUTO);
      test(r3._bigEndian == BIGENDIAN_AUTO, __LINE__);
    }

    // test la gestion des buffers internes
    {
      srand(time(NULL));
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "SPRO3");
      c.setParam("loadFeatureFileMemAlloc", "200000000");
      XLine l;
      const String file = dataDir + "prm/testSPro3.prm"; 
      for (unsigned long i=0; i<8 /*800*/; i++)
        l.addElement(file);
      FeatureMultipleFileReader r(l, c);
      Feature g;
      for (unsigned long j=0; j<50 /*500000*/;j++)
      {
        unsigned long i = (unsigned long)((float)rand()*r.getFeatureCount())/RAND_MAX;
        r.seekFeature(i);
        r.readFeature(g);
      }
    }
    // with n files
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      c.setParam("featureServerMode", "FEATURE_WRITABLE");
      c.setParam("loadFeatureFileMemAlloc","21312"); // 74*3*24*4
      XLine l;
      l.addElement(file).addElement(file2).addElement(file);
      FeatureMultipleFileReader s(l, c);
      FeatureFileReader r0(l.getElement(0), c);
      FeatureFileReader r1(l.getElement(1), c);
      FeatureFileReader r2(l.getElement(2), c);
      Feature f, g;
      unsigned long i=0;
      while (r0.readFeature(f))
      {
        if (!s.readFeature(g,0))
        {
          showError(__LINE__);
          break;
        }
        if (g != f)
        {
          showError(__LINE__);
          break;
        }
        g[0] = 128;
        test(s.writeFeature(g), __LINE__);
        i++;
      }
      while (r1.readFeature(f))
      {
        if (!s.readFeature(g,0))
        {
          showError(__LINE__);
          break;
        }
        if (g != f)
        {
          showError(__LINE__);
          break;
        }
        g[0] = 128;
        test(s.writeFeature(g), __LINE__);
        i++;
      }
      while (r2.readFeature(f))
      {
        if (!s.readFeature(g,0))
        {
          showError(__LINE__);
          break;
        }
        if (g != f)
        {
          showError(__LINE__);
          break;
        }
        g[0] = 128;
        test(s.writeFeature(g), __LINE__);
        i++;
      }
      s.reset();
      r0.reset();
      r1.reset();
      r2.reset();
      i = 0;
      while (r0.readFeature(f))
      {
        s.readFeature(g);
        f[0] = 128;
        test (f == g, __LINE__);
        i++;
      }
      while (r1.readFeature(f))
      {
        s.readFeature(g);
        f[0] = 128;
        test (f == g, __LINE__);
        i++;
      }
      while (r2.readFeature(f))
      {
        s.readFeature(g);
        f[0] = 128;
        test (f == g, __LINE__);
        i++;
      }
      test(i == r0.getFeatureCount()+r1.getFeatureCount()+r2.getFeatureCount(), __LINE__);
    }


  } // local objects are deleted here 

  return endTest();
}
//-------------------------------------------------------------------------
String TestFeatureMultipleFileReader::getClassName() const
{ return "TestFeatureMultipleFileReader"; }
//-------------------------------------------------------------------------
TestFeatureMultipleFileReader::~TestFeatureMultipleFileReader() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFeatureMultipleFileReader_cpp)
