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

#if !defined(ALIZE_TestFeatureInputStreamModifier_cpp)
#define ALIZE_TestFeatureInputStreamModifier_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestFeatureInputStreamModifier.h"
#include "FeatureInputStreamModifier.h"
#include "FeatureMultipleFileReader.h"
#include "alizeString.h"
#include "Config.h"
#include "FeatureFileReaderHTK.h"
#include "Feature.h"
#include "FeatureFileReader.h"
#include <cstdio>
#include <iostream>

using namespace alize;
using namespace std;
//-------------------------------------------------------------------------
TestFeatureInputStreamModifier::TestFeatureInputStreamModifier()
:Test() {}
//-------------------------------------------------------------------------
bool TestFeatureInputStreamModifier::exec()
{
  typedef FeatureInputStreamModifier M;
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
      c.setParam("bigEndian", "true");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      FeatureFileReaderHTK r(file, c);
      test(M(r).getClassName() == "FeatureInputStreamModifier", __LINE__);
    }

    // test method : FeatureInputStreamModifier(FeatureInputStream& is));
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      FeatureFileReaderHTK r(file, c);
      M m(r);
      test(m._pInput == &r, __LINE__);
      M m1(r, "1,2");
      test(m1._pInput == &r, __LINE__);
      test(m1._mask == "1,2", __LINE__);
      M m2(*new FeatureFileReaderHTK(file, c), "1,2", true);
      // true = owns feature stream and deletes it at the end
    }

    // test method : virtual void setMask(const String& mask)
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      FeatureFileReaderHTK r(file, c);
      M m(r);
      
      // test valid masks
      try
      {
        m.setMask("1");
        test(m._tmpMask == "01", __LINE__);
        test(m._selectionSize == 1, __LINE__);

        m.setMask("1-2,10-11,24,26,33-35");
        test(m._tmpMask == "011000000011000000000000101000000111", __LINE__);
        test(m._selectionSize == 9, __LINE__);

        m.setMask("0-35");
        test(m._tmpMask == "111111111111111111111111111111111111", __LINE__);
        test(m._selectionSize == 36, __LINE__);

        m.setMask("0-0");
        test(m._tmpMask == "1", __LINE__);
        test(m._selectionSize == 1, __LINE__);

        m.setMask("2-5,2-3");
        test(m._tmpMask == "001111", __LINE__);
        test(m._selectionSize == 4, __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }

       // test invalid masks
      try { m.setMask("-"); showError(__LINE__); }
      catch (Exception&) { }
      try { m.setMask("z"); showError(__LINE__); }
      catch (Exception&) { }
      try { m.setMask("1-2-4"); showError(__LINE__); }
      catch (Exception&) { }
      try { m.setMask(" "); showError(__LINE__); }
      catch (Exception&) { }
      try { m.setMask(","); showError(__LINE__); }
      catch (Exception&) { }
      try { m.setMask("1,,2"); showError(__LINE__); }
      catch (Exception&) { }
      try { m.setMask("2-1"); showError(__LINE__); }
      catch (Exception&) { }

      //test setMask("NO_MASK")
      m.setMask("NO_MASK");
      test(m._useMask == false, __LINE__);
    }
    
    // test method : virtual unsigned long getFeatureCount()
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      FeatureFileReaderHTK r(file, c);
      M m(r);
      test(r.getFeatureCount() == 89, __LINE__);
    }

    // test method : virtual void reset();
     {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      FeatureFileReaderHTK r(file, c);
      M m(r);
      Feature f, g;
      m.readFeature(f);
      m.readFeature(g);
      m.reset();
      test(m.readFeature(g) && g == f, __LINE__);
    }


    // test method : virtual unsigned long getVectSize();
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      FeatureFileReaderHTK r(file, c);
      FeatureFileReaderHTK r1(file, c);
      M m(r);
      test(m.getVectSize() == r1.getVectSize(), __LINE__);
      
      m.setMask("1");
      test(m.getVectSize() == 1, __LINE__);

      m.setMask("1-2,10-11,24,26,33-35");
      test(m.getVectSize() == 9, __LINE__);

      m.setMask("0-35");
      test(m.getVectSize() == 36, __LINE__);
    }

    // test method : virtual const FeatureFlags& getFeatureFlags();
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      FeatureFileReaderHTK r(file, c);
      FeatureFileReaderHTK r1(file, c);
      M m(r);
      test(r1.getFeatureFlags() == m.getFeatureFlags(), __LINE__);
    }

    // test method : virtual real_t getSampleRate()
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      FeatureFileReaderHTK r(file, c);
      FeatureFileReaderHTK r1(file, c);
      M m(r);
      test(r1.getSampleRate() == m.getSampleRate(), __LINE__);
    }

    // test method : virtual unsigned long getFeatureCount()
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      FeatureFileReaderHTK r(file, c);
      FeatureFileReaderHTK r1(file, c);
      M m(r);
      test(r1.getFeatureCount() == m.getFeatureCount(), __LINE__);
    }

    // test method : virtual bool readFeature(Feature&, unsigned long step)
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      FeatureFileReaderHTK r(file, c);
      M m(r);
      FeatureFileReaderHTK r1(file, c);
      unsigned long i;

      // without mask
      Feature f, g;
      try
      {
        for (i=0; i<r1.getFeatureCount(); i++)
          test(r1.readFeature(f) && m.readFeature(g) && (f == g), __LINE__);
      }
      catch (EOFException&) { showError(__LINE__); }

      // with mask
      try
      {
        m.reset();
        r1.reset();
        m.setMask("1-2,10-11,24,26,33-35");
        //cout << m.toString() << endl;
        for (i=1; i<r1.getFeatureCount(); i++)
        {
          Feature fsource, ftarget, g;
          r1.readFeature(fsource);
          m.readFeature(ftarget);
          test(ftarget.getVectSize() == 9, __LINE__);
          test(fsource[1] == ftarget[0], __LINE__);
          test(fsource[2] == ftarget[1], __LINE__);
          test(fsource[10] == ftarget[2], __LINE__);
          test(fsource[11] == ftarget[3], __LINE__);
          test(fsource[24] == ftarget[4], __LINE__);
          test(fsource[26] == ftarget[5], __LINE__);
          test(fsource[33] == ftarget[6], __LINE__);
          test(fsource[34] == ftarget[7], __LINE__);
          test(fsource[35] == ftarget[8], __LINE__);
        }
        m.reset();
        r1.reset();
        m.setMask("");
        test(m.readFeature(g) && g.getVectSize() == 0, __LINE__);

        m.setMask("0-36");
        // mask too large -> readFeature(...) throws an exception
        try { m.readFeature(g); showError(__LINE__); }
        catch (Exception&) { }
      }
      catch (EOFException&) { showError(__LINE__); }
      
    }
    // test method : virtual bool writeFeature(Feature&, unsigned long step)
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      c.setParam("featureServerMode", "FEATURE_WRITABLE");
      c.setParam("loadFeatureFileMemAlloc", "10000000");
      FeatureFileReaderHTK r(file, c);
      M m(r);
      unsigned long i, j;

      // without mask
      unsigned long vectSize = m.getVectSize();
      Feature f(vectSize), g;
      f.setValidity(true);
      for (i=0; i<vectSize; i++)
        f[i] = i;
      test(m.getFeatureCount() != 0, __LINE__);
      try
      {
        for (i=0; i<m.getFeatureCount(); i++)
        {
          m.writeFeature(f, 0);
          m.readFeature(g);
          test(f.getVectSize() == g.getVectSize(), __LINE__);
          test(f == g, __LINE__);
        }
      }
      catch (Exception&) { showError(__LINE__); }

      // test with a feature too large or too small
      Feature h(vectSize+1), h1(vectSize-1);
      m.reset();
      try { m.writeFeature(h, 0); showError(__LINE__); }
      catch (Exception&) {}
      try { m.writeFeature(h1, 0); showError(__LINE__); }
      catch (Exception&) {}


      // with mask
      try
      {
        m.reset();
        m.setMask("1-2,10-11,24,26,33-35");
        Feature ff(m.getVectSize());
        for (j=0; j<ff.getVectSize(); j++)
          ff[j] = j;
        ff.setValidity(true);
        //cout << m.toString() << endl;
        for (i=1; i<m.getFeatureCount(); i++)
        {
          m.writeFeature(ff, 0);
          m.readFeature(g);
          test(ff == g, __LINE__);
        }
        //
        m.reset();
        m.setMask("");
        test(m.readFeature(g) && g.getVectSize() == 0, __LINE__);
        //
        m.reset();
        m.setMask("NO_MASK");
        Feature fff(m.getVectSize());
        for (j=0; j<fff.getVectSize(); j++)
          fff[j] = j;
        fff.setValidity(true);
        //cout << m.toString() << endl;
        for (i=1; i<m.getFeatureCount(); i++)
        {
          m.writeFeature(fff, 0);
          m.readFeature(g);
          test(fff == g, __LINE__);
        }
      }
      catch (EOFException&) { showError(__LINE__); }
      
    }
    // test seekFeature(...) suivi d'un readFeature(...)
    {
      Config c;
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("bigEndian", "true");
      FeatureFileReaderHTK r(file, c), r1(file,c);
      M m(r);
      Feature f0(r.getVectSize());
      Feature f5(r.getVectSize());
      Feature f11(r.getVectSize());
      Feature g;
      unsigned long i=0;
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
      test(r.readFeature(g) && g == f0, __LINE__);
      r.seekFeature(5);
      test(r.readFeature(g) && g == f5, __LINE__);
      r.seekFeature(11);
      test(r.readFeature(g) && g == f11, __LINE__);
      r.seekFeature(89);
      test(!r.readFeature(g), __LINE__);
      r.seekFeature(123456);
      test(!r.readFeature(g), __LINE__);
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
      XLine l;
      FeatureMultipleFileReader r0(l, c);
      M s0(r0);
      try { s0.getNameOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      l.addElement(file);
      FeatureMultipleFileReader r1(l, c);
      M s1(r1);
      try { test(s1.getNameOfASource(0) == file, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getNameOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      FeatureMultipleFileReader r2(l, c);
      M s2(r2);
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
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      XLine l;
      FeatureMultipleFileReader r0(l, c);
      M s0(r0);
      try { s0.getFeatureCountOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      l.addElement(file);
      FeatureMultipleFileReader r1(l, c);
      M s1(r1);
      try { test(s1.getFeatureCountOfASource(0) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFeatureCountOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      FeatureMultipleFileReader r2(l, c);
      M s2(r2);
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
      XLine l;
      FeatureMultipleFileReader r0(l, c);
      M s0(r0);
      try { s0.getFeatureCountOfASource(file); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      l.addElement(file);
      FeatureMultipleFileReader r1(l, c);
      M s1(r1);
      try { test(s1.getFeatureCountOfASource(file) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFeatureCountOfASource(file2); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      FeatureMultipleFileReader r2(l, c);
      M s2(r2);
      try { test(s2.getFeatureCountOfASource(file) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFeatureCountOfASource(file2) == FeatureFileReader(file2,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFeatureCountOfASource("mlklkl"); showError(__LINE__); }
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
      FeatureFileReader r1(c);
      test(M(r1).getSourceCount() == 0, __LINE__);
      // 1 file
      FeatureFileReader r2(f1, c);
      test(M(r2).getSourceCount() == 1, __LINE__);
      // 2 files
      XLine l;
      l.addElement(f1).addElement(f2);
      FeatureFileReader r3(l, c);
      test(M(r3).getSourceCount() == 2, __LINE__);
    }
    // test method : unsigned long getFirstFeatureIndexOfASource(
    //                                          unsigned long srcIdx) const;
    {
      Config c;
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      // with no file
      XLine l;
      FeatureMultipleFileReader r0(l, c);
      M s0(r0);
      try { s0.getFirstFeatureIndexOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      l.addElement(file);
      FeatureMultipleFileReader r1(l, c);
      M s1(r1);
      try { test(s1.getFirstFeatureIndexOfASource(0) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      FeatureMultipleFileReader r2(l, c);
      M s2(r2);
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
      c.setParam("bigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      // with no file
      XLine l;
      FeatureMultipleFileReader r0(l, c);
      M s0(r0);
      try { s0.getFirstFeatureIndexOfASource(file); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      l.addElement(file);
      FeatureMultipleFileReader r1(l, c);
      M s1(r1);
      try { test(s1.getFirstFeatureIndexOfASource(file) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(file2); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      FeatureMultipleFileReader r2(l, c);
      M s2(r2);
      try { test(s2.getFirstFeatureIndexOfASource(file) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFirstFeatureIndexOfASource(file2) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFirstFeatureIndexOfASource("kljkljk"); showError(__LINE__); }
      catch (Exception&) {}
    }

  } // local objects are deleted here 

  return endTest();
}
//-------------------------------------------------------------------------
String TestFeatureInputStreamModifier::getClassName() const
{ return "TestFeatureInputStreamModifier"; }
//-------------------------------------------------------------------------
TestFeatureInputStreamModifier::~TestFeatureInputStreamModifier() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFeatureInputStreamModifier_cpp)
