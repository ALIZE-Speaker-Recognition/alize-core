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

#if !defined(ALIZE_TestFeatureFileReaderSPro3_cpp)
#define ALIZE_TestFeatureFileReaderSPro3_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestFeatureFileReaderSPro3.h"
#include "FeatureFileReaderSPro3.h"
#include "alizeString.h"
#include "Config.h"
#include "Feature.h"
#include "FileReader.h"
#include <cstdio>

using namespace alize;

//-------------------------------------------------------------------------
TestFeatureFileReaderSPro3::TestFeatureFileReaderSPro3()
:Test() {}
//-------------------------------------------------------------------------
bool TestFeatureFileReaderSPro3::exec()
{
  typedef FeatureFileReaderSPro3 R;

#ifdef WIN32
    String dataDir = "./../../data/";
#else
    String dataDir = "./../data/";
#endif

  Test::exec();
  {
    const String file(dataDir + "prm/testSPro3.prm");
    {
    
      // test method : String getClassName() const
      {
        Config c;
        test(R(file, c).getClassName() == "FeatureFileReaderSPro3", __LINE__);
      }

      // test method : explicit FeatureFileReaderSPro3(const FileName& f,
      //         const Config& c, LabelServer* ls = NULL);
      {
        Config c;
        R r(file, c);
      }
    
      // test method : static FeatureFileReaderSPro3& create(const FileName& f,
      //         const Config& c, LabelServer* ls = NULL);
      {
        Config c;
        R r(file, c);
      }

      // test method : virtual bool readFeature(Feature&, unsigned long step)
      {
        FileReader rr(file, "", "", false);
        rr.readChar();
        rr.close();
        Config c;
        R r(file, c);
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
          test(cpt == r.getFeatureCount(), __LINE__);
        }
        catch (EOFException&) { showError(__LINE__); }
      }
      // test seekFeature(...) suivi d'un readFeature(...)
      {
        Config c;
        R r(file, c);
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
        r.seekFeature(r.getFeatureCount());
        test(!r.readFeature(g), __LINE__);
        r.seekFeature(12456789);
        test(!r.readFeature(g), __LINE__);
      }

      // test method : virtual void reset();
      {
        Config c;
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
        R r(file, c);
        test(r.getFeatureCount() == 2000, __LINE__);
      }

      // test method : virtual unsigned long getVectSize();
      {
        Config c;
        R r(file, c);
        test(r.getVectSize() == 37, __LINE__);
      }

      // test method : virtual const FeatureFlags& getFeatureFlags();
      {
        Config c;
        FeatureFlags f("110000");
        R r(file, c);
        test(r.getFeatureFlags() == f, __LINE__);
      }
    } // end if (file.exists())

    // test method SPRO3DataKind getSPro3DataKind(const String& name)
    {
      try
      {
        test(getSPro3DataKind("OTHER") == SPRO3DataKind_OTHER, __LINE__);
        test(getSPro3DataKind("FBANK") == SPRO3DataKind_FBANK, __LINE__);
        test(getSPro3DataKind("FBCEPSTRA") == SPRO3DataKind_FBCEPSTRA, __LINE__);
        test(getSPro3DataKind("LPCEPSTRA") == SPRO3DataKind_LPCEPSTRA, __LINE__);
        test(getSPro3DataKind("LPCOEFF") == SPRO3DataKind_LPCOEFF, __LINE__);
        test(getSPro3DataKind("PARCOR") == SPRO3DataKind_PARCOR, __LINE__);
        test(getSPro3DataKind("LAR") == SPRO3DataKind_LAR, __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
      try
      { 
        getSPro3DataKind("NIMPORTEQUOI");
        showError(__LINE__);
      }
      catch (Exception&) {}
    }

  } // local objects are deleted here 

  return endTest();
}
//-------------------------------------------------------------------------
String TestFeatureFileReaderSPro3::getClassName() const
{ return "TestFeatureFileReaderSPro3"; }
//-------------------------------------------------------------------------
TestFeatureFileReaderSPro3::~TestFeatureFileReaderSPro3() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFeatureFileReaderSPro3_cpp)
