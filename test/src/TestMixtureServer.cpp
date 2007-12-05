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

#if !defined(ALIZE_TestMixtureServer_cpp)
#define ALIZE_TestMixtureServer_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include <cstdio>
#include <iostream>

#include "TestMixtureServer.h"
#include "MixtureServer.h"
#include "MixtureServerFileWriter.h"
#include "AutoDestructor.h"

using namespace alize;
using namespace std;
typedef MixtureServer MS;

//-------------------------------------------------------------------------
TestMixtureServer::TestMixtureServer()
:Test() {}
//-------------------------------------------------------------------------
bool TestMixtureServer::exec()
{
  Test::exec();
  {
    //DistribGF g(1);
    unsigned long i, j;

    // Test method : String getClassName() const
    {
      Config c;
      test(MS(c).getClassName() == "MixtureServer", __LINE__);
    }

    // Test method : explicit MixtureServer(const Config& config);
    {
      Config c;
      MS ms(c);
      test(ms._config == c, __LINE__);
      test(ms._serverName == "", __LINE__);
      test(ms._distribDict.size() == 0, __LINE__);
      test(ms._mixtureDict.size() == 0, __LINE__);
      test(ms._lastMixtureId == 0, __LINE__);
    }

    // Test method : explicit MixtureServer(const FileName&, const Config&)
    {
      Config c;
      c.setParam("vectSize", "5");
      const String file("./TestMixtureServerFileReaderXml.xml");
      MS ms1(c);
      ms1.createMixtureGD(3);
      ms1.createMixtureGD(7);
      MixtureServerFileWriter w(file, c);
      w.writeMixtureServer(ms1);
      w.close();

      MS ms(file, c); // build and load
      test(ms.getMixture(0) == ms1.getMixture(0), __LINE__);
      test(ms.getMixture(1) == ms1.getMixture(1), __LINE__);

      remove(file.c_str());
    }

    // Test method : void setServerName(const String&);
    {
      Config c;
      MS ms(c);
      ms.setServerName("name");
      test(ms._serverName == "name", __LINE__);
    }

    // Test method : const String& getServerName() const;
    {
      Config c;
      MS ms(c);
      ms.setServerName("name");
      test(ms.getServerName() == "name", __LINE__);
    }

    // Test method : Distrib& createDistrib(DistribType, unsigned long vectSize)
    {
      Config c;
      c.setParam("vectSize", "32");
      MS ms(c);
      try
      {
        Distrib& d = ms.createDistrib(DistribType_GD, 32);
        test(d.getClassName() == "DistribGD", __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
      MS ms2(c);
      try
      {
        Distrib& d = ms2.createDistrib(DistribType_GF, 32);
        test(d.getClassName() == "DistribGF", __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
      test(ms._distribDict.size() == 1, __LINE__);
      DistribGD* p = dynamic_cast<DistribGD*>(&ms._distribDict.getDistrib(0));
      test(p != NULL, __LINE__);
      test(p->getVectSize() == c.getParam_vectSize(), __LINE__);
      test(p->dictIndex(K::k) == 0, __LINE__);
      Distrib& d = ms.createDistrib(DistribType_GD, 32);
      test(ms._distribDict.size() == 2, __LINE__);
      test(d.dictIndex(K::k) == 1, __LINE__);
      Distrib& d2 = ms.createDistrib(DistribType_GD, 32);
      test(ms._distribDict.size() == 3, __LINE__);
      test(d2.dictIndex(K::k) == 2, __LINE__);
    }
    // Test method : unsigned long getVectSize()
    {
      Config c;
      MS ms(c);
      try
      {
        ms.getVectSize();
        showError(__LINE__);
      }
      catch (Exception&) {}

      ms.createDistrib(DistribType_GD, 32);
      try { test(ms.getVectSize() == 32, __LINE__); }
      catch (Exception&) { showError(__LINE__); }
    }

    // Test method : Distrib& createDistrib();
    // uses private method : Distrib& createDistrib(DistribType)
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD"); // default type
      MS ms(c);
      try
      {
        Distrib& d = ms.createDistrib();
        test(d.getClassName() == "DistribGD", __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }

      c.setParam("distribType", "GF");
      MS ms2(c);
      try { ms2.createDistrib(); }
      catch (Exception&) { showError(__LINE__); }
    }

    // Test method : DistribGD& createDistribGD();
    // uses private method : Distrib& createDistrib(DistribType)
    {
      Config c;
      c.setParam("vectSize", "32");
      MS ms(c);
      test(ms.createDistribGD().getClassName() == "DistribGD", __LINE__);
    }

    // Test method : Distrib& getDistrib(index) const;
    {
      Config c;
      c.setParam("vectSize", "32");
      MS ms(c);
      Distrib& d1 = ms.createDistribGD();
      Distrib& d2 = ms.createDistribGD();
      Distrib& d3 = ms.createDistribGD();
      try { ms.getDistrib(0); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      try { ms.getDistrib(50); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      test(ms.getDistrib(0).isSameObject(d1), __LINE__);
      test(ms.getDistrib(1).isSameObject(d2), __LINE__);
      test(ms.getDistrib(2).isSameObject(d3), __LINE__);
    }

    // Test method : Distrib& getDistribGD(index) const;
    {
      Config c;
      c.setParam("vectSize", "32");
      MS ms(c);
      Distrib& d1 = ms.createDistribGD();
      Distrib& d2 = ms.createDistribGD();
      Distrib& d3 = ms.createDistribGD();
      try { ms.getDistribGD(0); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      try { ms.getDistribGD(50); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      test(ms.getDistribGD(0).isSameObject(d1), __LINE__);
      test(ms.getDistribGD(1).isSameObject(d2), __LINE__);
      test(ms.getDistribGD(2).isSameObject(d3), __LINE__);

      // TODO : test with DistribGF or and other type
    }

    // Test method : unsigned long getDistribCount() const;
    {
      Config c;
      c.setParam("vectSize", "32");
      MS ms(c);
      test(ms.getDistribCount() == 0, __LINE__);
      ms.createDistribGD();
      test(ms.getDistribCount() == 1, __LINE__);
      ms.createDistribGD();
      test(ms.getDistribCount() == 2, __LINE__);
      ms.createDistribGD();
      test(ms.getDistribCount() == 3, __LINE__);
    }

    // Test method : DistribGD& duplicateDistrib(const DistribGD& d);
    {
      Config c;
      c.setParam("vectSize", "32");
      MS ms(c);
      DistribGD& d = ms.createDistribGD();
      Distrib& d2 = ms.duplicateDistrib(d);
      test(d2 == d, __LINE__);
      test(ms.getDistrib(0).isSameObject(d), __LINE__);
      test(ms.getDistribCount() == 2, __LINE__);
    }

    // Test method : Distrib& duplicateDistrib(const Distrib& d);
    {
      Config c;
      c.setParam("vectSize", "32");
      MS ms(c);
      Distrib& d = ms.createDistribGD();
      Distrib& d2 = ms.duplicateDistrib(d);
      test(d2 == d, __LINE__);
      test(ms.getDistrib(0).isSameObject(d), __LINE__);
      test(ms.getDistribCount() == 2, __LINE__);
      // TODO : tester avec d'autres types de distrib
    }

    // Test method : DistribGD& duplicateDistribGD(const Distrib& d);
    /*{
      Config c;
      c.setParam("vectSize", "32");
      MS ms(c);
      Distrib& d = ms.createDistribGD();
      DistribGD& d1 = ms.createDistribGD();
      Distrib& d2 = ms.duplicateDistribGD(d);
      Distrib& d3 = ms.duplicateDistribGD(d1);
      test(d2 == d, __LINE__);
      test(d3 == d1, __LINE__);
      test(ms.getDistrib(0).isSameObject(d), __LINE__);
      test(ms.getDistrib(1).isSameObject(d1), __LINE__);
      test(ms.getDistrib(2).isSameObject(d2), __LINE__);
      test(ms.getDistrib(3).isSameObject(d3), __LINE__);
      test(ms.getDistribCount() == 4, __LINE__);

      // TODO : test with DistribGF
    }*/

    // Test private method : Mixture& createMixture(unsigned long dc,
    //                          DistribType);
    {
      Config c;
      c.setParam("vectSize", "32");
      MS ms(c);
      unsigned long dc = 5;
      try
      {
        Mixture& m = ms.createMixture(dc, DistribType_GD);
        test(m.getClassName() == "MixtureGD", __LINE__);
        test(m.getId() == "#1", __LINE__);
        test(ms.getDistribCount() == dc, __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }

      /*MS ms2(c);
      try
      {
         // for the future
        ms2.createMixture(dc, DistribType_GF);
        showError(__LINE__);
      }
      catch (Exception&) {}*/
      
      test(ms._mixtureDict.size() == 1, __LINE__);
      MixtureGD* p = dynamic_cast<MixtureGD*>
                    (&ms._mixtureDict.getMixture(0));
      test(p != NULL, __LINE__);
      test(p->getVectSize() == c.getParam_vectSize(), __LINE__);
      Mixture& m2 = ms.createMixture(dc, DistribType_GD);
      test(m2.getId() == "#2", __LINE__);
      test(ms._mixtureDict.size() == 2, __LINE__);
      test(ms.getDistribCount() == 2*dc, __LINE__);
      ms.createMixture(dc, DistribType_GD);
      test(ms._mixtureDict.size() == 3, __LINE__);
      test(ms.getDistribCount() == 3*dc, __LINE__);
      unsigned long t=0;
      for (i=0; i<ms._mixtureDict.size(); i++)
      {
        Mixture& m = ms._mixtureDict.getMixture(i);
        test(m.getDistribCount() == dc, __LINE__);
        for (j=0; j<m.getDistribCount(); j++)
        {
          test(m.getDistrib(j).isSameObject(ms.getDistrib(t)),
                              __LINE__);
          t++;
        }
      }
    }

    // Test method : Mixture& createMixture(unsigned long dc);
    // uses private method : Mixture& createMixture(unsigned long dc,
    //                DistribType);
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      c.setParam("mixtureDistribCount", "5");
      MS ms(c);
      Mixture& m = ms.createMixture(6);
      test(m.getClassName() == "MixtureGD", __LINE__);
      test(m.getDistribCount() == 6, __LINE__);
      test(m.getId() == "#1", __LINE__);
      test(ms.createMixture(6).getId() == "#2", __LINE__);
      /*// for the future
      c.setParam("distribType", "GF");
      MS ms2(c);
      try { ms2.createMixture(6); showError(__LINE__); }
      catch (Exception&) {}*/
    }

    // Test method : MixtureGD& createMixtureGD();
    // uses private method : Mixture& createMixture(unsigned long dc,
    //                DistribType);
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("mixtureDistribCount", "5");
      c.setParam("distribType", "GF");
      MS ms(c);
      MixtureGD& m = ms.createMixtureGD();
      test(m.getClassName() == "MixtureGD", __LINE__);
      test(m.getDistribCount() == 5, __LINE__);
    }

    // Test method : MixtureGD& createMixtureGD(unsigned long dc);
    // uses private method : Mixture& createMixture(unsigned long dc,
    //                DistribType);
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("mixtureDistribCount", "5");
      c.setParam("distribType", "GF");
      MS ms(c);
      MixtureGD& m = ms.createMixtureGD(6);
      test(m.getClassName() == "MixtureGD", __LINE__);
      test(m.getDistribCount() == 6, __LINE__);
    }

    // Test method : unsigned long getMixtureCount() const;
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      c.setParam("mixtureDistribCount", "2");
      MS ms(c);
      test(ms.getMixtureCount() == 0, __LINE__);
      ms.createMixture();
      test(ms.getMixtureCount() == 1, __LINE__);
      ms.createMixture();
      test(ms.getMixtureCount() == 2, __LINE__);
    }

    // Test method : long getMixtureIndex(const String& id) const;
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      c.setParam("mixtureDistribCount", "2");
      MS ms(c);
      ms.createMixture();
      ms.createMixture();
      ms.createMixture();
      test(ms.getMixtureIndex("#1") == 0, __LINE__);
      test(ms.getMixtureIndex("#2") == 1, __LINE__);
      test(ms.getMixtureIndex("#3") == 2, __LINE__);
      test(ms.getMixtureIndex("#4") == -1, __LINE__);
      test(ms.getMixtureIndex("") == -1, __LINE__);
    }

    // Test method : Mixture& getMixture(index) const;
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      c.setParam("mixtureDistribCount", "5");
      MS ms(c);
      Mixture& m0 = ms.createMixture();
      Mixture& m1 = ms.createMixture();
      Mixture& m2 = ms.createMixture();
      try
      {
        test(ms.getMixture(0).isSameObject(m0), __LINE__);
      }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      test(ms.getMixture(1).isSameObject(m1), __LINE__);
      test(ms.getMixture(2).isSameObject(m2), __LINE__);
      try { ms.getMixture(3); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
    }

    // Test method : MixtureGD& getMixtureGD(index) const;
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      c.setParam("mixtureDistribCount", "2");
      MS ms(c);
      MixtureGD& m0 = ms.createMixtureGD();
      MixtureGD& m1 = ms.createMixtureGD();
      MixtureGD& m2 = ms.createMixtureGD();
      try
      {
        test(ms.getMixtureGD(0).isSameObject(m0), __LINE__);
      }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      test(ms.getMixtureGD(1).isSameObject(m1), __LINE__);
      test(ms.getMixtureGD(2).isSameObject(m2), __LINE__);
      try { ms.getMixtureGD(3); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      
      // TODO : for the future : test with DistribGF
    }

    // Test method : void setMixtureId(Mixture& mix, const String& id);
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      c.setParam("mixtureDistribCount", "2");
      MS ms(c);
      Mixture& m = ms.createMixture();
      try { ms.setMixtureId(m, "mix"); }
      catch (IdAlreadyExistsException&) { showError(__LINE__); }
      test(m.getId() == "mix", __LINE__);
      try { ms.setMixtureId(ms.createMixture(), "mix"); showError(__LINE__); }
      catch (IdAlreadyExistsException&) {}
      try { ms.setMixtureId(ms.createMixture(), "#1"); }
      catch (IdAlreadyExistsException&) { showError(__LINE__); }
    }

    // Test method : void addDistribToMixture(Mixture& mix,
    //         Distrib& distr, weight_t w = 0.0);
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      MS ms(c);
      Mixture& m = ms.createMixtureGD(2);
      DistribGD& d = ms.createDistribGD();
      DistribGF& e = DistribGF::create(K::k, 1);
      AutoDestructor xxx(e);
      ms.addDistribToMixture(m, d);
      test(m.weight(2) == 0, __LINE__);
      test(m.getDistribCount() == 3, __LINE__);
      test(d._refCounter == 2, __LINE__);
      test(m.getDistrib(0)._refCounter == 2, __LINE__);
      test(m.getDistrib(1)._refCounter == 2, __LINE__);
      test(m.getDistrib(2).isSameObject(d), __LINE__);
      test(ms.getDistribCount() == 3, __LINE__);
      ms.addDistribToMixture(m, static_cast<Distrib&>(d));
      ms.addDistribToMixture(static_cast<Mixture&>(m), d);
      ms.addDistribToMixture(static_cast<Mixture&>(m),
                  static_cast<Distrib&>(d));
      test(d._refCounter == 5, __LINE__);
      try
      {
        ms.addDistribToMixture(static_cast<Mixture&>(m), e);
        showError(__LINE__);
      }
      catch (...) {}
      try { ms.addDistribToMixture(m, e); showError(__LINE__); }
      catch (Exception&) {}
      try
      {
        ms.addDistribToMixture(m, static_cast<Distrib&>(e));
        showError(__LINE__);
      }
      catch (Exception&) {}
      try
      {
        ms.addDistribToMixture(static_cast<Mixture&>(m),
                    static_cast<Distrib&>(e));
        showError(__LINE__);
      }
      catch (Exception&) {}

      ms.addDistribToMixture(m, d, 1.1);
      test(m.weight(m.getDistribCount()-1) == 1.1, __LINE__);
    }

    // Test method : void setDistribToMixture(Mixture& mix,
    //         Distrib& distr, weight_t w, index);
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      MS ms(c);
      Mixture& m = ms.createMixtureGD(3);
      Distrib& d1 = m.getDistrib(1);
      Distrib& d = ms.createDistribGD();
      DistribGF& e = DistribGF::create(K::k, 1);
      AutoDestructor aa(e);
      try { ms.setDistribToMixture(m, d, 1.1,3); showError(__LINE__); }
      catch (Exception&) {}
      try { ms.setDistribToMixture(m, d, 1.1, 1); }
      catch (Exception&) { showError(__LINE__); }
      test(m.getDistrib(1).isSameObject(d), __LINE__);
      test(d._refCounter == 2, __LINE__);
      test(d1._refCounter == 1, __LINE__);
      try { ms.setDistribToMixture(m, e, 1.1,1); showError(__LINE__); }
      catch (Exception&) {}
      try { ms.setDistribToMixture(static_cast<MixtureGD&>(m), d, 1.1, 1); }
      catch (Exception&) { showError(__LINE__); }
      try { ms.setDistribToMixture(static_cast<MixtureGD&>(m),
                      static_cast<DistribGD&>(d), 1.1, 1); }
      catch (Exception&) { showError(__LINE__); }
      try { ms.setDistribToMixture(m, static_cast<DistribGD&>(d), 1.1, 1); }
      catch (Exception&) { showError(__LINE__); }

      // TODO : test set d'une distribGF dans une mixtureGF
    }

    // Test method : Mixture& duplicateMixture(const Mixture& mix,
    //          DuplDistrib = Mixture::DUPL_DISTRIB);
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      MS ms(c);
      const Mixture& m1 = ms.createMixtureGD(3);
      const Mixture& m2 = ms.duplicateMixture(m1);
      test(!m1.isSameObject(m2), __LINE__);
      test(m1 == m2, __LINE__);
      test(ms.getDistribCount() == 6, __LINE__);
      test(ms.getMixtureCount() == 2, __LINE__);
      test(m2.getId() == "#1 #2", __LINE__);
      const Mixture& m3 = ms.duplicateMixture(m1, SHARE_DISTRIB);
      test(!m1.isSameObject(m3), __LINE__);
      test(m1 == m3, __LINE__);
      test(ms.getDistribCount() == 6, __LINE__); // inchange
      test(ms.getMixtureCount() == 3, __LINE__);
      test(m3.getId() == "#1 #3", __LINE__);
    }

    // Test method : MixtureGD& duplicateMixture(const MixtureGD& mix,
    //  DuplDistrib = Mixture::DUPL_DISTRIB);
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      MS ms(c);
      const MixtureGD& m1 = ms.createMixtureGD(3);
      const MixtureGD& m2 = ms.duplicateMixture(m1);
      test(!m1.isSameObject(m2), __LINE__);
      test(m1 == m2, __LINE__);
      test(ms.getDistribCount() == 6, __LINE__);
      test(ms.getMixtureCount() == 2, __LINE__);
      test(m2.getId() == "#1 #2", __LINE__);
      const Mixture& m3 = ms.duplicateMixture(m1, SHARE_DISTRIB);
      test(!m1.isSameObject(m3), __LINE__);
      test(m1 == m3, __LINE__);
      test(ms.getDistribCount() == 6, __LINE__); // inchange
      test(ms.getMixtureCount() == 3, __LINE__);
      test(m3.getId() == "#1 #3", __LINE__);
    }

    // Test method : MixtureGD& duplicateMixtureGD(const Mixture& mix,
    //          DuplDistrib = Mixture::DUPL_DISTRIB);
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      MS ms(c);
      const Mixture& m1 = ms.createMixtureGD(3);
      const Mixture& m2 = ms.duplicateMixtureGD(m1);
      test(!m1.isSameObject(m2), __LINE__);
      test(m1 == m2, __LINE__);
      test(ms.getDistribCount() == 6, __LINE__);
      test(ms.getMixtureCount() == 2, __LINE__);
      test(m2.getId() == "#1 #2", __LINE__);
      const Mixture& m3 = ms.duplicateMixtureGD(m1, SHARE_DISTRIB);
      test(!m1.isSameObject(m3), __LINE__);
      test(m1 == m3, __LINE__);
      test(ms.getDistribCount() == 6, __LINE__); // inchange
      test(ms.getMixtureCount() == 3, __LINE__);
      test(m3.getId() == "#1 #3", __LINE__);
    }

    // Test method : void reset();
    {
      Config c;
      c.setParam("vectSize", "32");
      c.setParam("distribType", "GD");
      MS ms(c);
      ms.createMixtureGD(3);
      ms.createMixtureGD(4);
      ms.reset();
      test(ms.getMixtureCount() == 0, __LINE__);
      test(ms.getDistribCount() == 0, __LINE__);
      test(ms._lastMixtureId == 0, __LINE__);
    }

    // test method : MixtureGD& loadMixtureGD(const String& f)
    {
      char file[] = "./TestMixtureFileReader.xml";
      FILE* p = fopen(file, "w");
      char s[] = "\
<MixtureGD id=\"mixtureName\">\
	<distribCount>2</distribCount>\
	<vectSize>2</vectSize>\
	<DistribGD i=\"0\">\
		<weight>1</weight>\
		<covInv i=\"0\">1</covInv>\
		<covInv i=\"1\">0.5</covInv>\
		<mean i=\"0\">4</mean>\
		<mean i=\"1\">5</mean>\
	</DistribGD>\
	<DistribGD i=\"1\">\
		<weight>6</weight>\
		<covInv i=\"0\">0.25</covInv>\
		<covInv i=\"1\">0.125</covInv>\
		<mean i=\"0\">9</mean>\
		<mean i=\"1\">10</mean>\
	</DistribGD>\
</MixtureGD>";
      fwrite(&s, sizeof(s)-1, 1, p);
      fclose(p);

      Config c;
      c.setParam("vectSize", "2");
      c.setParam("mixtureDistribCount", "2");
      MS ms(c);
      MixtureGD& m1 = ms.loadMixtureGD(file);
      remove(file);
      test(m1.getId() == file, __LINE__);
      MixtureGD& m2 = ms.createMixtureGD();
      m2.weight(0) = 1;
      m2.getDistrib(0).setCov(1, 0);
      m2.getDistrib(0).setCov(2, 1);
      m2.getDistrib(0).setMean(4, 0);
      m2.getDistrib(0).setMean(5, 1);
      m2.weight(1) = 6;
      m2.getDistrib(1).setCov(4, 0);
      m2.getDistrib(1).setCov(8, 1);
      m2.getDistrib(1).setMean(9, 0);
      m2.getDistrib(1).setMean(10, 1);
      m2.computeAll();

      test(m1 == m2, __LINE__);
    }

    // Test method : void load(const FileName&);
    {
      Config c;
      c.setParam("vectSize", "5");
      const String file("./TestMixtureServerFileReaderXml.xml");
      MS ms1(c), ms(c);
      ms1.createMixtureGD(3);
      ms1.createMixtureGD(7);
      MixtureServerFileWriter w(file, c);
      w.writeMixtureServer(ms1);
      w.close();

      ms.load(file);
      test(ms.getMixture(0) == ms1.getMixture(0), __LINE__);
      test(ms.getMixture(1) == ms1.getMixture(1), __LINE__);

      remove(file.c_str());
    }

    // Test method : void save(const FileName&) const;
    {
      Config c;
      c.setParam("vectSize", "5");
      c.setParam("saveMixtureServerFileFormat", "RAW");
      const String file("./TestMixtureServerFileWriter");
      MS ms1(c), ms(c);
      ms1.createMixtureGD(3);
      ms1.createMixtureGD(7);
      ms1.save(file);

      ms.load(file);
      test(ms.getMixture(0) == ms1.getMixture(0), __LINE__);
      test(ms.getMixture(1) == ms1.getMixture(1), __LINE__);

      remove(file.c_str());
    }

    // Test method : void deleteMixtures(unsigned long first,
    //                   unsigned long last);
    {
      Config c;
      c.setParam("vectSize", "5");
      c.setParam("mixtureDistribCount", "5");
      MS ms(c);
      ms.setMixtureId(ms.createMixtureGD(), "0");
      ms.setMixtureId(ms.createMixtureGD(), "1");
      ms.setMixtureId(ms.createMixtureGD(), "2");
      ms.setMixtureId(ms.createMixtureGD(), "3");
      ms.deleteMixtures(1, 2);
      test(ms.getMixtureCount() == 2, __LINE__);
      test(ms.getDistribCount() == 20, __LINE__);
      test(ms.getMixture(0).getId() == "0", __LINE__);
      test(ms.getMixture(1).getId() == "3", __LINE__);
      ms.deleteMixtures(0, 99);
      test(ms.getMixtureCount() == 0, __LINE__);
      test(ms.getDistribCount() == 20, __LINE__);
      for (unsigned long i=0; i<ms.getDistribCount(); i++)
        if (ms.getDistrib(i).refCounter(K::k) != 1)
        {
          showError(__LINE__);
          break;
        }
    }
    // Test method : void deleteMixture(const Mixture& m)
    {
      Config c;
      c.setParam("vectSize", "5");
      c.setParam("mixtureDistribCount", "5");
      MS ms(c);
      ms.setMixtureId(ms.createMixtureGD(), "0");
      ms.setMixtureId(ms.createMixtureGD(), "1");
      ms.setMixtureId(ms.createMixtureGD(), "2");
      ms.setMixtureId(ms.createMixtureGD(), "3");
      ms.deleteMixture(ms.getMixture(1));
      ms.deleteMixture(ms.getMixture(1));
      test(ms.getMixtureCount() == 2, __LINE__);
      test(ms.getDistribCount() == 20, __LINE__);
      test(ms.getMixture(0).getId() == "0", __LINE__);
      test(ms.getMixture(1).getId() == "3", __LINE__);
      ms.deleteMixture(ms.getMixture(1));
      ms.deleteMixture(ms.getMixture(0));
      test(ms.getMixtureCount() == 0, __LINE__);
      test(ms.getDistribCount() == 20, __LINE__);
      for (unsigned long i=0; i<ms.getDistribCount(); i++)
        if (ms.getDistrib(i).refCounter(K::k) != 1)
        {
          showError(__LINE__);
          break;
        }
      ms.deleteUnusedDistribs();
      test(ms.getDistribCount() == 0, __LINE__);
    }

    // Test method : void deleteUnusedDistribs());
    {
      Config c;
      c.setParam("vectSize", "5");
      c.setParam("mixtureDistribCount", "5");
      MS ms(c);
      ms.setMixtureId(ms.createMixtureGD(), "0");
      ms.setMixtureId(ms.createMixtureGD(), "1");
      ms.setMixtureId(ms.createMixtureGD(), "2");
      ms.setMixtureId(ms.createMixtureGD(), "3");
      ms.deleteMixtures(1, 2);
      ms.deleteUnusedDistribs();
      test(ms.getDistribCount() == 10, __LINE__);
      try { ms.getVectSize(); }
      catch (Exception&) { showError(__LINE__); }
      ms.deleteMixtures(0, 99);
      ms.deleteUnusedDistribs();
      test(ms.getDistribCount() == 0, __LINE__);
      try
      {
        ms.getVectSize();
        showError(__LINE__);
      }
      catch (Exception&) {}
    }
    if (false) { // test de charge
      Config c;
      c.setParam("vectSize", "16");
      c.setParam("mixtureDistribCount", "128");
      MS ms(c);
      for (j=0; j<100; j++)
      {
        for (i=0; i<50; i++)
        {
          ms.setMixtureId(ms.createMixtureGD(), String::valueOf(i)+" " + String::valueOf(j));
        }
      }
      for (j=0; j<100; j++)
      {
        for (i=0; i<50; i++)
        {
          try {
          ms.getMixtureIndex(String::valueOf(i)+String::valueOf(j));
          } catch (IndexOutOfBoundsException&)
          {
            i=i;
          }
        }
        ms.deleteMixtures(0,9999999);
        ms.deleteUnusedDistribs();
      }
      /*test(v.size() == 2, __LINE__);
      test(v.getMixture(0).isSameObject(m1), __LINE__);
      test(v.getMixture(1).isSameObject(m2), __LINE__);
      test(v.getIndexOfId("1") == 0, __LINE__);
      test(v.getIndexOfId("2") == 1, __LINE__);*/
    }

    // ---------------------------------------------------
    // old tests
    // ---------------------------------------------------


    // TEST getClassName()

    Config config;
    config.setParam("vectSize", "32");
    config.setParam("distribType", "GD");
    MS ms(config);

    // test construction
    test(ms._config == config, __LINE__);
    test(ms.getDistribCount() == 0, __LINE__);
    test(ms.getMixtureCount() == 0, __LINE__);

    ms.reset();
    test(ms._config == config, __LINE__);


    // test createDistribGD() et createDistrib()-----------------------------
    Distrib& d1 = ms.createDistrib();
    test(ms.getDistribCount() == 1, __LINE__);
    test(d1._refCounter == 1, __LINE__);
    test(d1.dictIndex(K::k) == 0, __LINE__);
    Distrib& d2 = ms.createDistribGD();
    test(ms.getDistribCount() == 2, __LINE__);
    test(d2.dictIndex(K::k) == 1, __LINE__);

    // test getDistrib()
    test(ms.getDistrib(0).isSameObject(d1), __LINE__);
    try { test(ms.getDistrib(1).isSameObject(d2), __LINE__); }
    catch (IndexOutOfBoundsException&) { showError(__LINE__); }
    try 
    {
      ms.getDistrib(99);
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}
    try 
    {
      ms.getDistrib(3);
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}

    // test createMixtureGD() ------------------------------------

    MixtureGD& m1 = ms.createMixtureGD(4); // 4 distributions
    test(m1.getId() == "#1", __LINE__);
    test(ms.getMixtureCount() == 1, __LINE__);
    test(m1.getDistribCount() == 4, __LINE__);
    for (j=0; j<m1.getDistribCount(); j++)
    {
      test(m1.getDistrib(j).isSameObject(ms.getDistrib(j+2)), __LINE__);
      test(m1.getDistrib(j)._refCounter == 2, __LINE__);
    }

    Mixture& m2 = ms.createMixtureGD(5); // 5 distributions
    test(m2.getId() == "#2", __LINE__);
    test(ms.getMixtureCount() == 2, __LINE__);


    // test getMixture()
    test(ms.getMixture(0).isSameObject(m1), __LINE__);
    try { test(ms.getMixture(1).isSameObject(m2), __LINE__); }
    catch (IndexOutOfBoundsException&) { showError(__LINE__); }
    try 
    {
      ms.getMixture(99);
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}
    try 
    {
      ms.getMixture(3);
      showError(__LINE__);
    }
    catch (IndexOutOfBoundsException&) {}

    // test getMixtureIndex()
    //if (ms.getMixtureIndex("loc1") < 0)
    //  showError(__LINE__);
    //else
    //  test(ms.getMixtureIndex("bhjhjgygy") < 0, __LINE__);

    // test creation mixture sans distributions
    ms.createMixtureGD(0);

    // essaye d'ajouter une 2eme mixture "world" -> interdit
    /*try
    {
      ms.createMixtureGD(5);
      showError(__LINE__);
    }
    catch (IdAlreadyExistsException&) {}*/

    // ----------------------------------------------------------------
    // test duplicateMixture(const Mixture&, const String& id, DuplDistrib)
    // create a mixture

    MixtureGD& m4 = ms.createMixtureGD(5);
    for (i=0; i<m4.getDistribCount(); i++)
    {
      DistribGD& d = m4.getDistrib(i);
      for (j=0; j<d.getVectSize(); j++)
      {
        d.setMean((j+1)*(i+1), j); // données bidon
        d.setCov((j+2)*(i+10), j); // données bidon
      }
      m4.weight(i) = (weight_t)(i+1); // données bidon
    }
    m4.computeAll();
    unsigned long nbMix = ms.getMixtureCount();
    unsigned long nbDis = ms.getDistribCount();
    // duplicate the mixture and the distributions too
    MixtureGD& m5 = ms.duplicateMixture(m4);
    ms.setMixtureId(m5, "otherblabla");
    test(ms.getMixtureCount()-nbMix == 1, __LINE__);
    test(ms.getDistribCount()-nbDis == 5, __LINE__);
    test(!m5.isSameObject(m4), __LINE__);
    test(m5.getDistribCount() == m4.getDistribCount(), __LINE__);
    /*test(m5.getId() == "#2", __LINE__);
    test(m5.getVectSize() == m4.getVectSize(), __LINE__);
    for (i=0; i<m4.getDistribCount(); i++)
    {
      DistribGD& d5 = m5.getDistrib(i);
      DistribGD& d4 = m4.getDistrib(i);
      test(m5.weight(i) == m5.weight(i), __LINE__);
      test(!m5.getDistrib(i).isSameObject(m4.getDistrib(i)), __LINE__);
      for (j=0; j<d4.getVectSize(); j++)
      {
        test(d4.getMean(j) == d5.getMean(j), __LINE__);
        test(d4.getCovInv(j) == d5.getCovInv(j), __LINE__);
        test(d4.getCst() == d5.getCst(), __LINE__);
        test(d4.getDet() == d5.getDet(), __LINE__);
        test(d4.getCov(j) == d5.getCov(j), __LINE__);
      }
    }
    // duplicate the mixture but not the distributions
    MixtureGD& m6 = ms.duplicateMixture(m4, "otherblabla xx", false);
    test(ms.getMixtureCount()-nbMix == 2, __LINE__);
    test(ms.getDistribCount()-nbDis == 5, __LINE__);
    test(!m6.isSameObject(m4), __LINE__);
    test(m6.getDistribCount() == m4.getDistribCount(), __LINE__);
    test(m6.getId() == "otherblabla xx", __LINE__);
    test(m6.getVectSize() == m4.getVectSize(), __LINE__);
    for (i=0; i<m4.getDistribCount(); i++)
    {
      DistribGD& d6 = m6.getDistrib(i);
      DistribGD& d4 = m4.getDistrib(i);
      test(m6.weight(i) == m6.weight(i), __LINE__);
      test(m6.getDistrib(i).isSameObject(m4.getDistrib(i)), __LINE__);
    }*/

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestMixtureServer::getClassName() const
{ return "TestMixtureServer"; }
//-------------------------------------------------------------------------
TestMixtureServer::~TestMixtureServer() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestMixtureServer_cpp)
