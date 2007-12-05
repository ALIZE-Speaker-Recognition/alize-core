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

#if !defined(ALIZE_TestSegServerFileReaderRaw_cpp)
#define ALIZE_TestSegServerFileReaderRaw_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestSegServerFileReaderRaw.h"
#include "SegServerFileReaderRaw.h"
#include "SegServer.h"
#include "Config.h"
#include <cstdio>
#include <iostream>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestSegServerFileReaderRaw::TestSegServerFileReaderRaw()
:Test() {}
//-------------------------------------------------------------------------
bool TestSegServerFileReaderRaw::exec()
{
  Test::exec();

  {
    Config c;
    c.setParam("saveSegServerFileFormat", "RAW");
    const String file("./TestSegServerFileReaderRaw.raw");

    // test method : String getClassName()
    {
      test(SegServerFileReaderRaw(file, c).getClassName()
                  == "SegServerFileReaderRaw", __LINE__);
    }

    // test method : explicit SegServerFileReaderRaw(const String& f)
    {
      SegServerFileReaderRaw r(file, c);
    }

    // test method : virtual void readSegServer(SegServer&, Config&);
    {
      SegServer ss;
      ss.setServerName("serverName");
      Seg& s0 = ss.createSeg(1,2,3,"seg 0","bb");
      ss.createSeg(4,5,6,"seg 1","dd");
      ss.createSeg(7,8,9,"seg 2","ff");
      ss.createSeg(10,11,12,"seg 3","hh");
      ss.createSeg(13,14,15,"seg 4","jj");
      SegCluster& cl1 = ss.createCluster(16,"cl 0", "ll");
      SegCluster& cl2 = ss.createCluster(17,"cl 1", "nnnn");
      cl1.add(s0);
      cl1.add(ss.getSeg(1));
      cl2.add(cl1);
      cl2.add(ss.getSeg(3));
      s0.list().addLine().addElement("element0").addElement("element1");
      s0.list().addLine().addElement("element2");
      cl1.list().addLine().addElement("element3").addElement("element4");
      cl1.list().addLine().addElement("element5");

      FILE* f = fopen(file.c_str(), "wb");
      unsigned long i, j, k, l;
      i = ss.getServerName().length(); fwrite(&i, sizeof(i), 1, f);
      fwrite(ss.getServerName().c_str(), i, 1, f);
      i = ss.getSegCount(); fwrite(&i, sizeof(i), 1, f);
      for (j=0; j<ss.getSegCount(); j++)
      {
        const Seg& seg = ss.getSeg(j);
        i = seg.begin(); fwrite(&i, sizeof(i), 1, f);
        i = seg.length(); fwrite(&i, sizeof(i), 1, f);
        i = seg.labelCode(); fwrite(&i, sizeof(i), 1, f);
        i = seg.string().length(); fwrite(&i, sizeof(i), 1, f);
        fwrite(seg.string().c_str(), i, 1, f);
        i = seg.sourceName().length(); fwrite(&i, sizeof(i), 1, f);
        fwrite(seg.sourceName().c_str(), i, 1, f);
        const XList& list = seg.list();
        i = list.getLineCount(); fwrite(&i, sizeof(i), 1, f);
        for (k=0; k<list.getLineCount(); k++)
        {
          const XLine& line = list.getLine(k);
          i = line.getElementCount(); fwrite(&i, sizeof(i), 1, f);
          for (l=0; l<line.getElementCount(); l++)
          {
            const String& elem = line.getElement(l);
            i = elem.length(); fwrite(&i, sizeof(i), 1, f);
            fwrite(elem.c_str(), i, 1, f);
          }
        }
      }
      i = ss.getClusterCount(); fwrite(&i, sizeof(i), 1, f);
      for (j=0; j<ss.getClusterCount(); j++)
      {
        const SegCluster& cl = ss.getCluster(j);
        i = cl.getId(); fwrite(&i, sizeof(i), 1, f);
        i = cl.labelCode(); fwrite(&i, sizeof(i), 1, f);
        i = cl.string().length(); fwrite(&i, sizeof(i), 1, f);
        fwrite(cl.string().c_str(), i, 1, f);
        i = cl.sourceName().length(); fwrite(&i, sizeof(i), 1, f);
        fwrite(cl.sourceName().c_str(), i, 1, f);
        const XList& list = cl.list();
        i = list.getLineCount(); fwrite(&i, sizeof(i), 1, f);
        for (k=0; k<list.getLineCount(); k++)
        {
          const XLine& line = list.getLine(k);
          i = line.getElementCount(); fwrite(&i, sizeof(i), 1, f);
          for (l=0; l<line.getElementCount(); l++)
          {
            const String& elem = line.getElement(l);
            i = elem.length(); fwrite(&i, sizeof(i), 1, f);
            fwrite(elem.c_str(), i, 1, f);
          }
        }
        i = cl.getCount(); fwrite(&i, sizeof(i), 1, f);
        for (k=0; k<cl.getCount(); k++)
        {
          char c = cl.get(k).getClassName() == "Seg"?'s':'c';
          fwrite(&c, sizeof(c), 1, f);
          i = ss.getIndex(cl.get(k)); fwrite(&i, sizeof(i), 1, f);
          //cout << cl.getId() << "-" << c << "-" << k << endl;
        }
      }

      fclose(f);
      //
      SegServer ss2;
      SegServerFileReaderRaw(file, c).readSegServer(ss2);
      test(ss.getServerName() == ss2.getServerName(), __LINE__);
      test(ss.getSegCount() == ss2.getSegCount(), __LINE__);
      test(ss.getClusterCount() == ss2.getClusterCount(), __LINE__);
      for (i=0; i<ss.getSegCount(); i++)
      {
        const Seg& s1 = ss.getSeg(i);
        const Seg& s2 = ss2.getSeg(i);
        test(s1.begin() == s2.begin(), __LINE__);
        test(s1.length() == s2.length(), __LINE__);
        test(s1.labelCode() == s2.labelCode(), __LINE__);
        test(s1.string() == s2.string(), __LINE__);
        test(s1.sourceName() == s2.sourceName(), __LINE__);
        test(s1.list() == s2.list(), __LINE__);
      }
      for (i=0; i<ss.getClusterCount(); i++)
      {
        const SegCluster& cl1 = ss.getCluster(i);
        const SegCluster& cl2 = ss2.getCluster(i);
        test(cl1.labelCode() == cl2.labelCode(), __LINE__);
        test(cl1.string() == cl2.string(), __LINE__);
        test(cl1.sourceName() == cl2.sourceName(), __LINE__);
        test(cl1.list() == cl2.list(), __LINE__);
        test(cl1.getCount() == cl2.getCount(), __LINE__);
        for (unsigned long j=0; j<cl1.getCount(); j++)
        {
          test(cl1.get(j).getClassName() == cl2.get(j).getClassName(), __LINE__);
          test(ss.getIndex(cl1.get(j)) == ss2.getIndex(cl2.get(j)), __LINE__);
          //cout << ss.getIndex(cl1.get(j)) << " " << ss2.getIndex(cl2.get(j)) << endl;
        }
      }

      remove(file.c_str());
    }
  } // local objects are deleted here 
  return endTest();
}
//-------------------------------------------------------------------------
String TestSegServerFileReaderRaw::getClassName() const
{ return "TestSegServerFileReaderRaw"; }
//-------------------------------------------------------------------------
TestSegServerFileReaderRaw::~TestSegServerFileReaderRaw() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestSegServerFileReaderRaw_cpp)
