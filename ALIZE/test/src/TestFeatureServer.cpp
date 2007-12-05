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

#if !defined(ALIZE_TestFeatureServer_cpp)
#define ALIZE_TestFeatureServer_cpp

#include "TestFeatureServer.h"
#include "FeatureFileReader.h"
#include "FeatureServer.h"
#include "FeatureFlags.h"
#include "alizeString.h"
#include "Config.h"
#include "Feature.h"
#include "LabelServer.h"
#include "FeatureFileReaderHTK.h"
#include "FeatureMultipleFileReader.h"
#include "FeatureFileReaderSPro3.h"
#include <iostream>
#include <cmath>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestFeatureServer::TestFeatureServer()
:Test() {}
//-------------------------------------------------------------------------
bool TestFeatureServer::exec()
{
  Test::exec();
  {
#ifdef WIN32
    String dataDir = "./../../data/";
#else
    String dataDir = "./../data/";
#endif
    FeatureFlags flags;
    Config config;
    config.setParam("vectSize", "32");
    config.setParam("featureFlags", flags.getString());
    FeatureServer server(config);
    
    // TEST getClassName()
    test(server.getClassName() == "FeatureServer", __LINE__);

    //test(server.getBufferSize() == 10, __LINE__);

    Feature* previousFeature = NULL;
    try
    {
      Feature g;
      while (server.readFeature(g))
      {
        if (&g == previousFeature)
        {
          showError(__LINE__);
          break;
        }
        previousFeature = &g;
      }
    }
    catch (IndexOutOfBoundsException&) { showError(__LINE__); }
    /*try
    {
      server.getFeature(server.getBufferSize());
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}*/

    /*server.reset();
    if (server.getFeatureCount() != 0)
      showError(__LINE__);
*/

    // test l'utilisation d'un masque dans la config
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("featureServerMask", "1,3,5-7");
      c.setParam("loadFeatureFileFormat", "HTK");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      LabelServer ls;

      // f1 = feature qu'on est sensé obtenir
      Feature f1, g, f(5);
      FeatureFileReader(file, c).readFeature(f1);
      f.setValidity(true);
      f[0] = f1[1];
      f[1] = f1[3];
      f[2] = f1[5];
      f[3] = f1[6];
      f[4] = f1[7];

      test(FeatureServer(c, file).readFeature(g) && g == f, __LINE__);
      test(FeatureServer(c, file, ls).readFeature(g) && g == f, __LINE__);

      XLine line;
      line.addElement(file);
      test(FeatureServer(c, line).readFeature(g) && g == f, __LINE__);
      test(FeatureServer(c, line, ls).readFeature(g) && g == f, __LINE__);

      FeatureFileReader r2(file, c);
      FeatureServer x(c, r2);
      test(FeatureServer(c, r2).readFeature(g) && g == f, __LINE__);
    }

      // test seekFeature(...) suivi d'un readFeature(...)
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("loadFeatureFileFormat", "HTK");
      FeatureFileReaderHTK r(file, c);
      FeatureServer s(c, file);
      test(s.getFeatureCount() == r.getFeatureCount(), __LINE__);
      r.seekFeature(0);
      s.seekFeature(0);
      Feature g, h;
      s.readFeature(g);
      test(r.readFeature(h) && h == g, __LINE__);
      test(g.isValid(), __LINE__);
      /*test(*p == s.getFeature(s.getBufferSize()-1), __LINE__);
      for (unsigned long i=0; i<s.getBufferSize()-1; i++)
        if (s.getFeature(i).isValid())
        {
          showError(__LINE__);
          break;
        }*/
    }
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("loadFeatureFileFormat", "HTK");
      FeatureFileReaderHTK r(file, c);
      FeatureServer s(c, file);
      test(s.getFeatureCount() == r.getFeatureCount(), __LINE__);
      r.seekFeature(10);
      s.seekFeature(10);
      Feature g, g2;
      s.readFeature(g);
      r.readFeature(g2);
      test(g == g2, __LINE__);
      /*r.seekFeature(0);
      for (i=s.getBufferSize()-11; i<s.getBufferSize(); i++)
        if (s.getFeature(i) != *r.readFeature())
        {
          showError(__LINE__);
          break;
        }
      for (i=0; i<s.getBufferSize()-11; i++)
        if (s.getFeature(i).isValid())
        {
          showError(__LINE__);
          break;
        }
      for (; i<s.getBufferSize(); i++)
        if (!s.getFeature(i).isValid())
        {
          showError(__LINE__);
          break;
        }*/
    }
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("loadFeatureFileFormat", "HTK");
      //c.setParam("featureServerBufferSize", "10");
      FeatureFileReaderHTK r(file, c);
      FeatureServer s(c, file);
      test(s.getFeatureCount() == r.getFeatureCount(), __LINE__);
      r.seekFeature(5);
      s.seekFeature(5);
      Feature g, h;
      s.readFeature(g);
      test(r.readFeature(h) && h == g, __LINE__);
      /*r.seekFeature(0);
      for (i=s.getBufferSize()-6; i<s.getBufferSize(); i++)
        if (s.getFeature(i) != *r.readFeature())
        {
          showError(__LINE__);
          break;
        }
      for (i=0; i<s.getBufferSize()-6; i++)
        if (s.getFeature(i).isValid())
        {
          showError(__LINE__);
          break;
        }
      for (; i<s.getBufferSize(); i++)
        if (!s.getFeature(i).isValid())
        {
          showError(__LINE__);
          break;
        }*/
    }
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file(dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      c.setParam("loadFeatureFileFormat", "HTK");
      //c.setParam("featureServerBufferSize", "10");
      //c.setParam("vectSize", "2");
      FeatureFileReaderHTK r(file, c);
      FeatureServer s(c, file);
      test(s.getFeatureCount() == r.getFeatureCount(), __LINE__);
      r.seekFeature(15);
      s.seekFeature(15);
      Feature g, h;
      unsigned long i=0;
      while (r.readFeature(g))
      {
        i++;
        if (!s.readFeature(h) || h != g)
        {
          showError(__LINE__);
          break;
        }
      }
      /*for (i=0; i<s.getBufferSize(); i++)
        if (!s.getFeature(i).isValid())
        {
          showError(__LINE__);
          break;
        }*/
    }
    // test method : const String& getNameOfASource(unsigned long srcIdx)
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      FeatureServer s0(c);
      try { s0.getNameOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      XLine l;
      l.addElement(file);
      FeatureServer s1(c, l);
      try { test(s1.getNameOfASource(0) == file, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getNameOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      FeatureServer s2(c, l);
      try { test(s2.getNameOfASource(0) == file, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getNameOfASource(1) == file2, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getNameOfASource(2); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file out of a list -> not possible
      FeatureServer s20(c, file);
      try { test(s20.getNameOfASource(0) == file, __LINE__); }
      catch (Exception&) { showError(__LINE__); }
      try { s20.getNameOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : unsigned long getFeatureCountOfASource(unsigned long srcIdx) const;
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      FeatureServer s0(c);
      try { s0.getFeatureCountOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      XLine l;
      l.addElement(file);
      FeatureServer s1(c, l);
      try { test(s1.getFeatureCountOfASource(0) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFeatureCountOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      FeatureServer s2(c, l);
      try { test(s2.getFeatureCountOfASource(0) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFeatureCountOfASource(1) == FeatureFileReader(file2,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFeatureCountOfASource(2); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file out of a list
      FeatureServer s20(c, file);
      try { test(s20.getFeatureCountOfASource(0) == FeatureFileReader(file,c).getFeatureCount(), __LINE__); }
      catch (Exception&) { showError(__LINE__); }
      FeatureServer s21(c, file);
      try { s21.getFeatureCountOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : unsigned long getFeatureCountOfASource(const String& srcName) const;
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      FeatureServer s0(c);
      try { s0.getFeatureCountOfASource(file); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      XLine l;
      l.addElement(file);
      FeatureServer s1(c, l);
      try { test(s1.getFeatureCountOfASource(file) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFeatureCountOfASource("mlklm"); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      FeatureServer s2(c, l);
      try { test(s2.getFeatureCountOfASource(file) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFeatureCountOfASource(file2) == FeatureFileReader(file2,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFeatureCountOfASource("klkljk"); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file out of a list
      FeatureServer s20(c, file);
      try { test(s20.getFeatureCountOfASource(file) == FeatureFileReader(file,c).getFeatureCount(), __LINE__); }
      catch (Exception&) { showError(__LINE__); }
      FeatureServer s21(c, file);
      try { s21.getFeatureCountOfASource("kljk"); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : virtual unsigned long getSourceCount()
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      c.setParam("loadFeatureFileFormat", "HTK");
      const String f1 = dataDir + "prm/TestFeatureFileReaderHTK_PLP.plp";
      const String f2 = dataDir + "prm/TestFeatureFileReaderHTK_PLP.plp";
      // no file
      test(FeatureServer(c).getSourceCount() == 0, __LINE__);
      // 1 file
      test(FeatureServer(c, f1).getSourceCount() == 1, __LINE__);
      // 2 files
      XLine l;
      l.addElement(f1).addElement(f2);
      test(FeatureServer(c, l).getSourceCount() == 2, __LINE__);
    }
    // test method : unsigned long getFirstFeatureIndexOfASource(
    //                                        unsigned long srcIdx) const;
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      FeatureServer s0(c);
      try { s0.getFirstFeatureIndexOfASource(0); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      XLine l;
      l.addElement(file);
      FeatureServer s1(c, l);
      try { test(s1.getFirstFeatureIndexOfASource(0) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      FeatureServer s2(c, l);
      try { test(s2.getFirstFeatureIndexOfASource(0) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFirstFeatureIndexOfASource(1) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFirstFeatureIndexOfASource(2); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file out of a list
      FeatureServer s20(c, file);
      try { test(s20.getFirstFeatureIndexOfASource(0) == 0, __LINE__); }
      catch (Exception&) { showError(__LINE__); }
      FeatureServer s21(c, file);
      try { s21.getFirstFeatureIndexOfASource(1); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : unsigned long getFirstFeatureIndexOfASource(
    //                                        const String& srcName) const
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      //c.setParam("featureServerBufferSize", "10");
      // with no file
      FeatureServer s0(c);
      try { s0.getFirstFeatureIndexOfASource(file); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file
      XLine l;
      l.addElement(file);
      FeatureServer s1(c, l);
      try { test(s1.getFirstFeatureIndexOfASource(file) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s1.getFirstFeatureIndexOfASource(file2); showError(__LINE__); }
      catch (Exception&) {}
      // with 2 files
      l.addElement(file2);
      FeatureServer s2(c, l);
      try { test(s2.getFirstFeatureIndexOfASource(file) == 0, __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { test(s2.getFirstFeatureIndexOfASource(file2) == FeatureFileReader(file,c).getFeatureCount(), __LINE__);}
      catch (Exception&) { showError(__LINE__); }
      try { s2.getFirstFeatureIndexOfASource("jknk"); showError(__LINE__); }
      catch (Exception&) {}
      // with 1 file out of a list
      FeatureServer s20(c, file);
      try { test(s20.getFirstFeatureIndexOfASource(file) == 0, __LINE__); }
      catch (Exception&) { showError(__LINE__); }
      FeatureServer s21(c, file);
      try { s21.getFirstFeatureIndexOfASource(file2); showError(__LINE__); }
      catch (Exception&) {}
    }

    // test method : bool writeFeature(const Feature& f, unsigned long step)
    // with 1 file
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      c.setParam("featureServerMode", "FEATURE_WRITABLE");
      c.setParam("featureServerMemAlloc","7104"); // 74*24*4
      FeatureServer s(c, file);
      FeatureFileReaderHTK r(file, c);
      Feature f, g;
      while (s.readFeature(f, 0))
      {
        f[0] = 128;
        s.writeFeature(f);
      }
      s.reset();
      while (s.readFeature(f))
      {
        r.readFeature(g);
        g[0] = 128;
        test (f == g, __LINE__);
        //cout << f.toString() << endl;
        //cout << g.toString() << endl;
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
      c.setParam("featureServerMemAlloc","21312"); // 74*3*24*4
      XLine l;
      l.addElement(file).addElement(file2).addElement(file);
      FeatureServer s(c, l);
      FeatureMultipleFileReader r(l, c);
      Feature f, g;
      unsigned long i=0;
      while (s.readFeature(f, 0))
      {
        if (!r.readFeature(g))
        {
          showError(__LINE__);
          break;
        }
          //cout << g.toString() << endl;
          //cout << f.toString() << endl;
        if (g != f)
        {
          showError(__LINE__);
          break;
        }
        f[0] = 128;
        test(s.writeFeature(f), __LINE__);
        i++;
      }
      s.reset();
      r.reset();
      i = 0;
      while (s.readFeature(f))
      {
        r.readFeature(g);
        g[0] = 128;
        test (f == g, __LINE__);
        i++;
      }
      test(i == r.getFeatureCount(), __LINE__);
    }

    // TODO : faire des test plus complets !

    // test readFeature with historic = LIMITED and 1 file
    {
      /* <FRANCAIS>dans cette configuration, on simule une lecture sur
         un flux continu sans retour en arrière possible (mode micro) sauf
         dans la limite de l'historique */
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      c.setParam("featureServerBufferSize", "10");
      FeatureServer fs(c, file);
      Feature f;
      fs.seekFeature(50);
      test(fs.readFeature(f) && !f.isValid(), __LINE__);
      for (unsigned long i=0; i<50; i++) // read 50 features
        fs.readFeature(f);
      fs.seekFeature(40); // go back 10 steps
      test(fs.readFeature(f) && f.isValid(), __LINE__);
      // try to seek before the historic
      fs.seekFeature(39); // try to go back 11 steps
      test(fs.readFeature(f) && !f.isValid(), __LINE__);
      fs.seekFeature(50); // do not move
      test(fs.readFeature(f) && f.isValid(), __LINE__);
      // try to seek after the historic
      fs.seekFeature(51+1);
      test(fs.readFeature(f) && !f.isValid(), __LINE__);
    }
    // test writeFeature with historic = LIMITED and more than 1 file
    {
      /* <FRANCAIS>dans cette configuration, on simule une lecture sur
         un flux continu sans retour en arrière possible (mode micro) sauf
         dans la limite de l'historique */
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureServerMode", "FEATURE_WRITABLE");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      c.setParam("featureServerBufferSize", "10");
      c.setParam("featureServerMemAlloc",String::valueOf(74*24*4*2));
      XLine l;
      l.addElement(file).addElement(file); // 2 files
      FeatureServer fs(c, l);
      Feature f(fs.getVectSize());
      fs.seekFeature(50);
      try { fs.writeFeature(f); showError(__LINE__); }
      catch (Exception&) {};
      for (unsigned long i=0; i<50; i++) // read 50 features
        try { fs.writeFeature(f); }
        catch (Exception&) { showError(__LINE__); break; }
      fs.seekFeature(40); // go back 10 steps
      try { fs.writeFeature(f); }
      catch (Exception&) { showError(__LINE__); }
      // try to seek before the historic
      fs.seekFeature(39); // try to go back 11 steps
      try {fs.writeFeature(f); showError(__LINE__); }
      catch (Exception&) {};
      fs.seekFeature(50); // do not move
      try { fs.writeFeature(f); }
      catch (Exception&) { showError(__LINE__); }
      // try to seek after the historic
      fs.seekFeature(51+1);
      try {fs.writeFeature(f); showError(__LINE__); }
      catch (Exception&) {};
    }

    // test methods for default server (no stream & no config)
    {
      FeatureServer s;
      try { s.close(); } catch (Exception&) { showError(__LINE__); }
      try { s.getFeatureCount(); } catch (Exception&) { showError(__LINE__); }
      try { s.getFeatureFlags(); showError(__LINE__); } catch (Exception&) {}
      try { s.getSampleRate(); showError(__LINE__); } catch (Exception&) {}
      try { s.getVectSize(); showError(__LINE__); } catch (Exception&) {}
      try { Feature f; s.readFeature(f); } catch (Exception&) { showError(__LINE__); }
      try { s.reset(); } catch (Exception&) { showError(__LINE__); }
      try { Feature f; s.writeFeature(f); showError(__LINE__); } catch (Exception&) {}
      try { s.seekFeature(1); } catch (Exception&) { showError(__LINE__); }
      try { s.seekFeature(1, "jkk"); } catch (Exception&) { showError(__LINE__); }
      try { s.getSourceCount(); } catch (Exception&) { showError(__LINE__); }
      try { s.getFeatureCountOfASource(0); showError(__LINE__); } catch (Exception&) {}
      try { s.getFeatureCountOfASource("hh"); showError(__LINE__); } catch (Exception&) {}
      try { s.getFirstFeatureIndexOfASource(0); showError(__LINE__); } catch (Exception&) {}
      try { s.getFirstFeatureIndexOfASource("jj"); showError(__LINE__); } catch (Exception&) {}
    }

    // test methods for default server (no stream but a config)
    {
      Config c;
      FeatureServer s(c);
      try { s.close(); } catch (Exception&) { showError(__LINE__); }
      try { s.getFeatureCount(); } catch (Exception&) { showError(__LINE__); }
      try { s.getFeatureFlags(); showError(__LINE__); } catch (Exception&) {}
      try { s.getSampleRate(); showError(__LINE__); } catch (Exception&) {}
      try { s.getVectSize(); showError(__LINE__); } catch (Exception&) {}
      try { Feature f; s.readFeature(f);} catch (Exception&) { showError(__LINE__); }
      try { s.reset(); } catch (Exception&) { showError(__LINE__); }
      try { Feature f; s.writeFeature(f); showError(__LINE__); } catch (Exception&) {}
      try { s.seekFeature(1); } catch (Exception&) { showError(__LINE__); }
      try { s.seekFeature(1, "jkk"); } catch (Exception&) { showError(__LINE__); }
      try { s.getSourceCount(); } catch (Exception&) { showError(__LINE__); }
      try { s.getFeatureCountOfASource(0); showError(__LINE__); } catch (Exception&) {}
      try { s.getFeatureCountOfASource("hh"); showError(__LINE__); } catch (Exception&) {}
      try { s.getFirstFeatureIndexOfASource(0); showError(__LINE__); } catch (Exception&) {}
      try { s.getFirstFeatureIndexOfASource("jj"); showError(__LINE__); } catch (Exception&) {}
    }

    // test method init(const Config&)
    {
      Config c, c1;
      FeatureServer fs(c);
      fs.init(c1);
      test(fs.getConfig().isSameObject(c1), __LINE__);
    }
    // test method init(const Config&, FeatureInputStream&)
    {
      Config c;
      c.setParam("loadFeatureFileFormat", "SPRO3");
      FeatureFileReaderSPro3 r0("./file0.prm", c);
      Config c1(c);
      FeatureFileReaderSPro3 r1("./file1.prm",c1);
      FeatureServer fs(c, r0);
      fs.init(c1, r1);
      test(fs.getConfig().isSameObject(c1), __LINE__);
      test(fs._pInputStream == &r1, __LINE__);
    }
    // test method init(const Config&, const FileName&)
    {
      Config c;
      c.setParam("loadFeatureFileFormat", "SPRO3");
      const String file0 = "./file0.prm";
      Config c1(c);
      const String file1 = "./file1.prm";
      FeatureServer fs(c, file0);
      fs.init(c1, file1);
      test(fs.getConfig().isSameObject(c1), __LINE__);
      test(fs.getNameOfASource(0) == file1, __LINE__);
      test(fs._pLabelServer == NULL, __LINE__);
    }
    // test method init(const Config&, const FileName&, LabelServer&)
    {
      Config c;
      c.setParam("loadFeatureFileFormat", "SPRO3");
      const String file0 = "./file0.prm";
      LabelServer ls0, ls1;
      Config c1(c);
      const String file1 = "./file1.prm";
      FeatureServer fs(c, file0, ls0);
      fs.init(c1, file1, ls1);
      test(fs.getConfig().isSameObject(c1), __LINE__);
      test(fs.getNameOfASource(0) == file1, __LINE__);
      test(fs._pLabelServer == &ls1, __LINE__);
    }
    // test method init(const Config&, const XLine&)
    {
      Config c;
      c.setParam("loadFeatureFileFormat", "SPRO3");
      XLine l0;
      l0.addElement("./file0.prm");
      Config c1(c);
      XLine l1;
      const String file1 = "./file1.prm";
      l1.addElement(file1);
      FeatureServer fs(c, l0);
      fs.init(c1, l1);
      test(fs.getConfig().isSameObject(c1), __LINE__);
      test(fs.getNameOfASource(0) == file1, __LINE__);
      test(fs._pLabelServer == NULL, __LINE__);
    }
    // test method init(const Config&, const XLine&, LabelServer&)
    {
      Config c;
      c.setParam("loadFeatureFileFormat", "SPRO3");
      XLine l0;
      l0.addElement("./file0.prm");
      LabelServer ls0, ls1;
      Config c1(c);
      XLine l1;
      const String file1 = "./file1.prm";
      l1.addElement(file1);
      FeatureServer fs(c, l0, ls0);
      fs.init(c1, l1, ls1);
      test(fs.getConfig().isSameObject(c1), __LINE__);
      test(fs.getNameOfASource(0) == file1, __LINE__);
      test(fs._pLabelServer == &ls1, __LINE__);
    }
    // test avec lecture d'un fichier sphere
    {
      Config c, c1;
      c.setParam("loadFeatureFileBigEndian", "false");
      c.setParam("loadFeatureFileFormat", "SPRO4");
      XLine listPRM;
      listPRM.addElement(dataDir+"/prm/testFeatureComputer1.prm");
      listPRM.addElement(dataDir+"/prm/testFeatureComputer1.prm");
      FeatureServer fs(c, listPRM);

      c1.setParam("loadAudioFileBigEndian", "true");
      c1.setParam("featureComputerEnergy", "true");
      c1.setParam("loadFeatureFileFormat", "SPHERE");
      c1.setParam("featureServerMode", "FEATURE_WRITABLE");
      c1.setParam("featureServerMemAlloc", String::valueOf(fs.getFeatureCount()*fs.getVectSize()*sizeof(float)*2));
      XLine list;
      list.addElement(dataDir+"/audio/TestAudioFileReader_pcm16_1channel.sph");
      list.addElement(dataDir+"/audio/TestAudioFileReader_pcm16_1channel.sph");
      FeatureServer fs1(c1, list);

      Feature f, f1;
      unsigned long n=0;
      while (fs.readFeature(f) && fs1.readFeature(f1, 0))
      {
        fs1.writeFeature(f1); // default step = 1
        n++;
        test(f.getVectSize() == f1.getVectSize(), __LINE__);
        test(f.isValid(), __LINE__);
        for (unsigned long i=0; i<f.getVectSize(); i++)
        {
          if (fabs(f[i]-f1[i]) > 1e-6)
          {
            showError(__LINE__);
            break;
          }
        }
      }
      test(fs1.getFeatureCount() == n, __LINE__);
    }

    // test param featureServerMemAlloc usage
    {
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      const String file2 = "TestFeatureFileReaderHTK_PLP_E";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");

      Config c2(c);
      c.setParam("featureServerMemAlloc","10000000");

      XLine l;
      l.addElement(file).addElement(file).addElement(file);
      FeatureServer s(c, l);
      //cout << s.getFeatureCount() << endl;
      FeatureServer s2(c2, l); // without buffering
      Feature f, g;
      //unsigned long i=0;
      unsigned long nb = s.getFeatureCount();
      //cout << nb << endl;
      for (unsigned long t=0; t<100; t++) // t tests
      {
        unsigned long p = (unsigned long)((double)rand()*(nb-1))/RAND_MAX;
        //unsigned long p = nb-t;
        s.seekFeature(p);
        s2.seekFeature(p);
        while (s.readFeature(f))
        {
          if (!s2.readFeature(g))
          {
            showError(__LINE__);
            break;
          }
          if (g != f)
          {
            //cout << f.toString() << endl;
            //cout << g.toString() << endl;
            cout << p << endl;
            showError(__LINE__);
            break;
          }
        }
      }
    }
    // XXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    if (false){
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true");
      const String file = "TestFeatureFileReaderHTK_PLP_D";
      c.setParam("loadFeatureFileFormat", "HTK");
      c.setParam("featureFilesPath", dataDir+"prm/");
      c.setParam("loadFeatureFileExtension", ".plp");
      FeatureFileReader fr(file, c);
      Config c2(c);
      int nbFiles=30000;
      XLine l;
      for (int y=0;y<nbFiles;y++)
        l.addElement(file);
      unsigned long mem = fr.getFeatureCount()*sizeof(float)*fr.getVectSize()*(nbFiles);
      cout << mem << endl;
      c.setParam("featureServerMemAlloc",String::valueOf(mem));
      FeatureServer s(c, l);
      cout << s.getFeatureCount() << endl;
      Feature f, g;
      //unsigned long i=0;
      unsigned long nb = s.getFeatureCount();
      //cout << nb << endl;
      unsigned long p=0;
      while(p<nb)
      {
        s.seekFeature(p);
        s.readFeature(f);
        p+=nb/nbFiles;
      }
    }

    /*{
      Config c;
      c.setParam("loadFeatureFileBigEndian", "true"); // or false ?
      c.setParam("loadFeatureFileFormat", "HTK");
      FeatureServer s(c, dataDir + "prm/TestFeatureFileReaderHTK_PLP_A_D.plp");
      Feature f;
      cout << "Feature count: " << s.getFeatureCount() << endl;
      while (s.readFeature(f)) // if end of file return false
      {
        if (f.isValid())
        {
        // use f



        }
      }
    }*/


  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestFeatureServer::getClassName() const {return "TestFeatureServer";}
//-------------------------------------------------------------------------
TestFeatureServer::~TestFeatureServer() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFeatureServer_cpp)
