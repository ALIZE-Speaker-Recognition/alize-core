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

#if !defined(ALIZE_TestSegCluster_cpp)
#define ALIZE_TestSegCluster_cpp

#include "TestSegCluster.h"
#include "SegServer.h"
#include "AutoDestructor.h"
#include <iostream>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestSegCluster::TestSegCluster()
:Test() {}
//-------------------------------------------------------------------------
bool TestSegCluster::exec()
{
  Test::exec();
  {
    // test method : String getClassName() const
    {
      SegServer server;
      test(SegCluster(server, 3,"","").getClassName() == "SegCluster", __LINE__);
    }
    // test method : explicit SegCluster(unsigned long b, unsigned long e,
    //       unsigned long lc, const String& s, const String& sn)
    {
      try
      {
        SegServer server;
        SegCluster s1(server, 3, "aa", "bb");
        s1.list().addLine().addElement("123");
        test (s1._labelCode == 3, __LINE__);
        test (s1._string == "aa", __LINE__);
        test (s1._srcName == "bb", __LINE__);
        test (s1._pServer == &server, __LINE__);
        test (s1._list.getLine(0).getElement(0) == "123", __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
    }
  /*    // test method : SegCluster(const SegCluster&)
    {
      SegCluster s1(server, 3, "aa", "bb");
      SegCluster s2(s1);
      test (s2._begin == s1._begin, __LINE__);
      test (s2._end == s1._end, __LINE__);
      test (s2._labelCode == s1._labelCode, __LINE__);
      test (s2._string == s1._string, __LINE__);
      test (s2._srcName == s1._srcName, __LINE__);
    }*/
    // test method : const SegCluster& operator=(const SegCluster& c)
    /*{
      SegCluster s1(server, 3, "aa", "bb");
      SegCluster s2(server, 9,"jhgj", "jh");
      s2 = s1;
      test (s2._begin == s1._begin, __LINE__);
      test (s2._end == s1._end, __LINE__);
      test (s2._labelCode == s1._labelCode, __LINE__);
      test (s2._string == s1._string, __LINE__);
      test (s2._srcName == s1._srcName, __LINE__);
    }*/
    // test method : virtual unsigned long begin() const;
    {
      SegServer ss;
      SegCluster& cl1 = ss.createCluster();
      // pas de bornes définies -> exception
      try { cl1.begin(); showError(__LINE__); }
      catch (Exception&) {}
      try
      {
        cl1.add(ss.createSeg(5,10));
        test(cl1.begin() == 5, __LINE__);
        cl1.add(ss.createSeg(6,8));
        test(cl1.begin() == 5, __LINE__);
        cl1.add(ss.createSeg(4,8));
        test(cl1.begin() == 4, __LINE__);
        cl1.add(ss.createSeg(6,11));
        test(cl1.begin() == 4, __LINE__);
        cl1.add(ss.createSeg(3,12));
        test(cl1.begin() == 3, __LINE__);

        SegCluster& cl2 = ss.createCluster();
        cl2.add(cl1);
        test(cl2.begin() == 3, __LINE__);
        ss.removeAllSegs();
        try { cl2.begin(); showError(__LINE__); }
        catch (Exception&) {}
      }
      catch (Exception&) { showError(__LINE__); }
    }
    // test method : virtual unsigned long length() const;
    {
      SegServer ss;
      SegCluster& cl1 = ss.createCluster();
      // pas de bornes définies -> exception
      try { cl1.length(); showError(__LINE__); }
      catch (Exception&) {}
      try
      {
        cl1.add(ss.createSeg(5,5));
        test(cl1.length() == 5, __LINE__);
        cl1.add(ss.createSeg(6,2));
        test(cl1.length() == 5, __LINE__);
        cl1.add(ss.createSeg(4,4));
        test(cl1.length() == 6, __LINE__);
        cl1.add(ss.createSeg(6,5));
        test(cl1.length() == 7, __LINE__);
        cl1.add(ss.createSeg(3,9));
        test(cl1.length() == 9, __LINE__);

        SegCluster& cl2 = ss.createCluster();
        cl2.add(cl1);
        test(cl2.length() == 9, __LINE__);
        ss.removeAllSegs();
        try { cl2.length(); showError(__LINE__); }
        catch (Exception&) {}
      }
      catch (Exception&) { showError(__LINE__); }
    }
    // test method : unsigned long labelCode() const
    {
      SegServer server;
      SegCluster s1(server, 3, "aa", "bb");
      test (s1._labelCode == s1.labelCode(), __LINE__);
    }
    // test method : const String& string() const
    {
      SegServer server;
      SegCluster s1(server, 3, "aa", "bb");
      test (s1._string == s1.string(), __LINE__);
    }
    // test method : const String& sourceName() const
    {
      SegServer server;
      SegCluster s1(server, 3, "aa", "bb");
      test (s1._srcName == s1.sourceName(), __LINE__);
    }
    // test method : void setLabelCode(unsigned long lc)
    {
      SegServer server;
      SegCluster s1(server, 3, "aa", "bb");
      s1.setLabelCode(12);
      test (s1._labelCode == 12, __LINE__);
    }
    // test method : void setString(const String& s)
    {
      SegServer server;
      SegCluster s1(server, 3, "aa", "bb");
      s1.setString("12");
      test (s1._string == "12", __LINE__);
    }
    // test method : void setSourceName(const String& sn)
    {
      SegServer server;
      SegCluster s1(server, 3, "aa", "bb");
      s1.setSourceName("12");
      test (s1._srcName == "12", __LINE__);
    }

    // test method : void setId(const K&, unsigned long id)
    {
      SegServer server;
      SegCluster s1(server, 3, "aa", "bb");
      s1.setId(K::k, 456);
      test(s1._id == 456, __LINE__);
    }
    // test method : unsigned long getId() const
    {
      SegServer server;
      SegCluster s1(server, 3, "aa", "bb");
      s1.setId(K::k, 456);
      test(s1.getId() == 456, __LINE__);
    }
    // test method : add(SegAbstract& s)
    {
      SegServer server;
      Seg& s1 = server.createSeg();
      SegCluster sc1(server);
      SegCluster sc2(server);
      SegCluster sc3(server);
       /*
          sc3 -
          /   \   \
        /     \   \
         sc1    sc2   \
        | \   /  |   |
        |   \  |   |
        | /   \  |   |
         s1     s2  s3
       */
      try 
      {
        sc1.add(s1);
        test(sc1._vect.size() == 1, __LINE__);
        test(s1._ownersVect.size() == 1, __LINE__);
        test(s1._ownersVect.getObject(0).isSameObject(sc1), __LINE__);
        sc2.add(s1);
        test(sc2._vect.size() == 1, __LINE__);
        test(s1._ownersVect.size() == 2, __LINE__);
        test(s1._ownersVect.getObject(1).isSameObject(sc2), __LINE__);
        Seg& s2 = server.createSeg();
        sc1.add(s2);
        test(sc1._vect.size() == 2, __LINE__);
        test(s2._ownersVect.size() == 1, __LINE__);
        test(s2._ownersVect.getObject(0).isSameObject(sc1), __LINE__);
        sc2.add(s2);
        test(sc2._vect.size() == 2, __LINE__);
        test(s2._ownersVect.size() == 2, __LINE__);
        test(s2._ownersVect.getObject(1).isSameObject(sc2), __LINE__);
        sc3.add(sc1);
        test(sc3._vect.size() == 1, __LINE__);
        test(sc1._ownersVect.size() == 1, __LINE__);
        test(sc1._ownersVect.getObject(0).isSameObject(sc3), __LINE__);
        sc3.add(sc2);
        test(sc3._vect.size() == 2, __LINE__);
        test(sc2._ownersVect.size() == 1, __LINE__);
        test(sc2._ownersVect.getObject(0).isSameObject(sc3), __LINE__);
        sc3.add(s2);
        test(sc3._vect.size() == 3, __LINE__);
        test(s2._ownersVect.size() == 3, __LINE__);
        test(s2._ownersVect.getObject(2).isSameObject(sc3), __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }

      SegServer server2;
      Seg& sx = server2.createSeg();
      try { sc1.add(sx); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : SegAbstract& get(unsigned long i) const;
    {
      SegServer server;
      Seg s1(server);
      Seg s2(server);
      Seg s3(server);
      Seg s4(server);
      SegCluster sc(server, 4,"", "");
      sc.add(s1);
      sc.add(s2);
      sc.add(s3);
      sc.add(s4);
      try
      {
        for (unsigned long i=0; i<4; i++)
          test(sc._vect.getObject(i).isSameObject(sc.get(i)) , __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
      try { sc.get(4); showError(__LINE__); }
      catch (Exception&) {}
    }
    // test method : add(SegAbstract& s, unsigned long pos)
    {
      SegServer server;
      Seg s1(server);
      Seg s2(server);
      Seg s3(server);
      Seg s4(server);
      SegCluster sc(server);
      sc.add(s1);
      sc.add(s2,0);
      sc.add(s3,1);
      sc.add(s4,3);
      test(sc._vect.size() == 4, __LINE__);
      test(sc.get(0).isSameObject(s2) , __LINE__);
      test(sc.get(1).isSameObject(s3) , __LINE__);
      test(sc.get(2).isSameObject(s1) , __LINE__);
      test(sc.get(3).isSameObject(s4) , __LINE__);
    }
    // test method : SegAbstract& duplicate() const
    {
      SegServer server;
      SegCluster s1(server);
      SegServer server2;
      SegCluster& s2 = s1.duplicate(K::k, server2);
      s2.list().addLine().addElement("123");
      AutoDestructor ad(s2);
      test (s2._labelCode == s1._labelCode, __LINE__);
      test (s2._string == s1._string, __LINE__);
      test (s2._srcName == s1._srcName, __LINE__);
      test (s2._pServer == &server2, __LINE__);
      test (s2._list.getLine(0).getElement(0) == "123", __LINE__);

    }
    // test method : void remove(const SegAbstract& s)
    {
      SegServer ss;
      SegAbstract& s1 = ss.createSeg();
      SegAbstract& s2 = ss.createSeg();
      SegAbstract& s3 = ss.createSeg();
      SegAbstract& s4 = ss.createSeg();
      SegAbstract& s5 = ss.createSeg();
      SegCluster& cl = ss.createCluster();
      cl.add(s1);
      cl.add(s2);
      cl.add(s3);
      cl.add(s4);
      cl.add(s5);
      
      cl.remove(s5);
      test(cl.getCount() == 4, __LINE__);
      test(cl.get(0).isSameObject(s1), __LINE__);
      test(cl.get(1).isSameObject(s2), __LINE__);
      test(cl.get(2).isSameObject(s3), __LINE__);
      test(cl.get(3).isSameObject(s4), __LINE__);
      cl.remove(s1);
      test(cl.getCount() == 3, __LINE__);
      test(cl.get(0).isSameObject(s2), __LINE__);
      test(cl.get(1).isSameObject(s3), __LINE__);
      test(cl.get(2).isSameObject(s4), __LINE__);
      cl.remove(s3);
      test(cl.getCount() == 2, __LINE__);
      test(cl.get(0).isSameObject(s2), __LINE__);
      test(cl.get(1).isSameObject(s4), __LINE__);

      test(s1._ownersVect.size() == 0, __LINE__);
      test(s2._ownersVect.size() == 1, __LINE__);
      test(s3._ownersVect.size() == 0, __LINE__);
      test(s4._ownersVect.size() == 1, __LINE__);
      test(s5._ownersVect.size() == 0, __LINE__);
    }
    // test method : SegAbstract& remove(unsigned long idx)
    {
      SegServer ss;
      SegAbstract& s1 = ss.createSeg();
      SegAbstract& s2 = ss.createSeg();
      SegAbstract& s3 = ss.createSeg();
      SegAbstract& s4 = ss.createSeg();
      SegAbstract& s5 = ss.createSeg();
      SegCluster& cl = ss.createCluster();
      SegCluster& cl2 = ss.createCluster();
      cl.add(s1);
      cl.add(s2);
      cl.add(s3);
      cl.add(s4);
      cl.add(s5);
      cl2.add(s1);
      
      test(cl.remove(4).isSameObject(s5), __LINE__);
      test(cl.getCount() == 4, __LINE__);
      test(cl.get(0).isSameObject(s1), __LINE__);
      test(cl.get(1).isSameObject(s2), __LINE__);
      test(cl.get(2).isSameObject(s3), __LINE__);
      test(cl.get(3).isSameObject(s4), __LINE__);
      test(cl.remove(0).isSameObject(s1), __LINE__);
      test(cl.getCount() == 3, __LINE__);
      test(cl.get(0).isSameObject(s2), __LINE__);
      test(cl.get(1).isSameObject(s3), __LINE__);
      test(cl.get(2).isSameObject(s4), __LINE__);
      test(cl.remove(1).isSameObject(s3), __LINE__);
      test(cl.getCount() == 2, __LINE__);
      test(cl.get(0).isSameObject(s2), __LINE__);
      test(cl.get(1).isSameObject(s4), __LINE__);

      test(s1._ownersVect.size() == 1, __LINE__);
      test(s2._ownersVect.size() == 1, __LINE__);
      test(s3._ownersVect.size() == 0, __LINE__);
      test(s4._ownersVect.size() == 1, __LINE__);
      test(s5._ownersVect.size() == 0, __LINE__);
    }
    // test method : void removeAll()
    {
      SegServer ss;
      SegAbstract& s1 = ss.createSeg();
      SegAbstract& s2 = ss.createSeg();
      SegAbstract& s3 = ss.createSeg();
      SegAbstract& s4 = ss.createSeg();
      SegAbstract& s5 = ss.createSeg();
      SegCluster& cl = ss.createCluster();
      SegCluster& cl2 = ss.createCluster();
      cl.add(s1);
      cl.add(s2);
      cl.add(s3);
      cl.add(s4);
      cl.add(s5);
      cl2.add(s1);
      
      cl.removeAll();

      test(cl.getCount() == 0, __LINE__);
      test(s1._ownersVect.size() == 1, __LINE__);
      test(s2._ownersVect.size() == 0, __LINE__);
      test(s3._ownersVect.size() == 0, __LINE__);
      test(s4._ownersVect.size() == 0, __LINE__);
      test(s5._ownersVect.size() == 0, __LINE__);
    }
    // test method : rewind() const
    {
      SegServer ss;
      SegCluster& cl = ss.createCluster();
      cl._current = 5;
      cl.rewind();
      test(cl._pCurrentSubSeg == NULL, __LINE__);
      test(cl._current == 0, __LINE__);
    }
    // test method : Seg* getSeg()
    {
      SegServer ss;
      SegCluster& cl = ss.createCluster();
      test(cl.getSeg() == NULL, __LINE__);

      SegCluster& cl1 = ss.createCluster();
      SegCluster& cl2 = ss.createCluster();
      SegCluster& cl3 = ss.createCluster();
      cl1.add(ss.createSeg(1,2,0,"1"));
      cl1.add(ss.createSeg(1,2,0,"2"));
      cl1.add(ss.createSeg(1,2,0,"3"));
      cl2.add(ss.createSeg(1,2,0,"4"));
      cl1.add(cl2);
      cl1.add(cl3);
      test(ss.getSeg(0)._current == 0, __LINE__);
      SegAbstract* p = ss.getSeg(0).getSeg();
      test(p != NULL && p->string() == "1", __LINE__);
      test(ss.getSeg(0).getSeg() == NULL, __LINE__);
      ss.getSeg(0).rewind();
      p = ss.getSeg(0).getSeg();
      test(p != NULL && p->string() == "1", __LINE__);
      test(cl1._current == 0, __LINE__);
      p = cl1.getSeg();
      test(p != NULL && p->string() == "1", __LINE__);
      p = cl1.getSeg();
      test(p != NULL && p->string() == "2", __LINE__);
      p = cl1.getSeg();
      test(p != NULL && p->string() == "3", __LINE__);
      p = cl1.getSeg();
      test(p != NULL && p->string() == "4", __LINE__);
      p = cl1.getSeg();
      test(p == NULL, __LINE__);
      cl1.rewind();
      p = cl1.getSeg();
      test(p != NULL && p->string() == "1", __LINE__);
      p = cl1.getSeg();
      test(p != NULL && p->string() == "2", __LINE__);
      p = cl1.getSeg();
      test(p != NULL && p->string() == "3", __LINE__);
      p = cl1.getSeg();
      test(p != NULL && p->string() == "4", __LINE__);
      p = cl1.getSeg();
      test(p == NULL, __LINE__);
    }
    // test methods : bool getFeatureLabelCode(unsigned long n,
       //      unsigned long& lc, bool& isFirst, bool& isLast)
    // and      bool getFeatureLabelCode(unsigned long n,
    //         unsigned long& lc,)
    {
      SegServer ss;
      SegCluster& cl = ss.createCluster();
      bool b, e;
      unsigned long lc;
      Seg& s0 = ss.createSeg(10, 10, 1);
      Seg& s1 = ss.createSeg(20, 5, 2);
      Seg& s2 = ss.createSeg(30, 8, 3);
      Seg& s3 = ss.createSeg(40, 1, 4);
      cl.add(s0);
      cl.add(s1);
      cl.add(s2);
      cl.add(s3);
      test(!cl.getFeatureLabelCode(0, lc), __LINE__);
      test(!cl.getFeatureLabelCode(9, lc), __LINE__);
      test(cl.getFeatureLabelCode(10, lc, b, e), __LINE__);
      test(lc == 1 && b && !e, __LINE__);
      test(cl.getFeatureLabelCode(11, lc, b, e), __LINE__);
      test(lc == 1 && !b && !e, __LINE__);
      test(cl.getFeatureLabelCode(19, lc, b, e), __LINE__);
      test(lc == 1 && !b && e, __LINE__);
      test(cl.getFeatureLabelCode(20, lc, b, e), __LINE__);
      test(lc == 2 && b && !e, __LINE__);
      test(cl.getFeatureLabelCode(21, lc, b, e), __LINE__);
      test(lc == 2 && !b && !e, __LINE__);
      test(cl.getFeatureLabelCode(24, lc, b, e), __LINE__);
      test(lc == 2 && !b && e, __LINE__);
      test(!cl.getFeatureLabelCode(25, lc), __LINE__);
      test(!cl.getFeatureLabelCode(29, lc), __LINE__);
      test(cl.getFeatureLabelCode(30, lc, b, e), __LINE__);
      test(lc == 3 && b && !e, __LINE__);
      test(cl.getFeatureLabelCode(31, lc, b, e), __LINE__);
      test(lc == 3 && !b && !e, __LINE__);
      test(cl.getFeatureLabelCode(37, lc, b, e), __LINE__);
      test(lc == 3 && !b && e, __LINE__);
      test(!cl.getFeatureLabelCode(38, lc), __LINE__);
      test(cl.getFeatureLabelCode(40, lc, b, e), __LINE__);
      test(lc == 4 && b && e, __LINE__);
    }
    // test method : Seg& addCopy(const Seg& s)
    {
      SegServer server;
      Seg& s1 = server.createSeg();
      SegCluster& cl = server.createCluster();
      Seg& s = cl.addCopy(s1);
      test(s.isSameObject(cl.get(0)), __LINE__);
      test(server.getSegCount() == 2, __LINE__);
      test(server.getSeg(1).isSameObject(cl.get(0)), __LINE__);
    }
    // test method : Seg& addCopy(const Seg& s, unsigned long pos)
    {
      SegServer server;
      Seg& s1 = server.createSeg();
      SegCluster& cl = server.createCluster();
      cl.add(s1);
      Seg& s = cl.addCopy(s1, 0);
      test(s.isSameObject(cl.get(0)), __LINE__);
      test(server.getSegCount() == 2, __LINE__);
      test(server.getSeg(1).isSameObject(cl.get(0)), __LINE__);
      test(cl.get(2).isSameObject(cl.addCopy(s1, 2)), __LINE__);
    }
    // test method : Seg& addNewSeg(unsigned long b = 0,
    //               unsigned long l = 0, unsigned long lc = 0,
    //               const String& s = "", const String& s  yn = "");
    {
      SegServer ss;
      SegCluster& cl = ss.createCluster();
      Seg& s1 = cl.addNewSeg(1, 1, 3, "4", "5");
      test(ss.getSegCount() == 1, __LINE__);
      test(ss.getSeg(0).isSameObject(cl.get(0)), __LINE__);
      test(s1._pServer == &ss, __LINE__);
      test(s1._begin == 1, __LINE__);
      test(s1._length == 1, __LINE__);
      test(s1.labelCode() == 3, __LINE__);
      test(s1.string() == "4", __LINE__);
      test(s1.sourceName() == "5", __LINE__);
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestSegCluster::getClassName() const { return "TestSegCluster"; }
//-------------------------------------------------------------------------
TestSegCluster::~TestSegCluster() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestSegCluster_cpp)

