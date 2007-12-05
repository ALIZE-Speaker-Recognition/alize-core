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

#if !defined(ALIZE_TestSeg_cpp)
#define ALIZE_TestSeg_cpp

#include "TestSeg.h"
#include "SegServer.h"
#include "AutoDestructor.h"
#include <iostream>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestSeg::TestSeg()
:Test() {}
//-------------------------------------------------------------------------
bool TestSeg::exec()
{
    SegServer server;

    Test::exec();
    {
        // test method : String getClassName() const
        {
            test(Seg(server, 1,2,3,"","").getClassName() == "Seg", __LINE__);
        }
        // test method : explicit Seg(unsigned long b, unsigned long e,
        //           unsigned long lc, const String& s, const String& sn)
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            test(s1._begin == 1, __LINE__);
            test(s1._length == 2, __LINE__);
            test(s1._labelCode == 3, __LINE__);
            test(s1._string == "aa", __LINE__);
            test(s1._srcName == "bb", __LINE__);
            Seg s2(server, 5,5,4, "", "");
            s2.list().addLine().addElement("123");
            test(s2._begin == 5, __LINE__);
            test(s2._length == 5, __LINE__);
            test(s2._labelCode == 4, __LINE__);
            test(s2._string == "", __LINE__);
            test(s2._srcName == "", __LINE__);
            test(s2._list.getLine(0).getElement(0) == "123", __LINE__);
        }
        
/*        // test method : Seg(const Seg&)
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            Seg s2(s1);
            test(s2._begin == s1._begin, __LINE__);
            test(s2._length == s1._length, __LINE__);
            test(s2._labelCode == s1._labelCode, __LINE__);
            test(s2._string == s1._string, __LINE__);
            test(s2._srcName == s1._srcName, __LINE__);
        }
        // test method : const Seg& operator=(const Seg& c)
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            Seg s2(server, 4,8,9,"jhgj", "jh");
            s2 = s1;
            test(s2._begin == s1._begin, __LINE__);
            test(s2._length == s1._length, __LINE__);
            test(s2._labelCode == s1._labelCode, __LINE__);
            test(s2._string == s1._string, __LINE__);
            test(s2._srcName == s1._srcName, __LINE__);
        }*/
        // test method : virtual SegAbstract& duplicate() const
        {
            SegServer ss;
            Seg s1(server, 1,2,3, "aa", "bb");
            s1.list().addLine().addElement("123");
            Seg& s2 = s1.duplicate(K::k, ss);
            AutoDestructor ad(s2);
            test(s2._begin == s1._begin, __LINE__);
            test(s2._length == s1._length, __LINE__);
            test(s2._labelCode == s1._labelCode, __LINE__);
            test(s2._string == s1._string, __LINE__);
            test(s2._srcName == s1._srcName, __LINE__);
            test(s2._pServer == &ss, __LINE__);
            test(s2._list == s1._list, __LINE__);
        }
        // test method : virtual Seg& duplicate() const
        {
            SegServer ss;
            Seg& s1 = ss.createSeg(1,2,3, "aa", "bb");
            s1.list().addLine().addElement("123");
            Seg& s2 = s1.duplicate();
            test(s2._begin == s1._begin, __LINE__);
            test(s2._length == s1._length, __LINE__);
            test(s2._labelCode == s1._labelCode, __LINE__);
            test(s2._string == s1._string, __LINE__);
            test(s2._srcName == s1._srcName, __LINE__);
            test(s2._pServer == s1._pServer, __LINE__);
            test(s2._list == s1._list, __LINE__);
        }
        // test method : virtual unsigned long begin() const
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            test(s1.begin() == 1, __LINE__);
        }
        // test method : virtual unsigned long length() const
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            test(s1.length() == 2, __LINE__);
        }
        // test method : void setBegin(unsigned long b)
        {
            Seg s1(server, 2,3,5, "", "");
            s1.setBegin(8);
            test(s1._begin == 8, __LINE__);
        }
        // test method : void setLength(unsigned long l)
        {
            Seg s1(server, 2,3,5, "", "");
            s1.setLength(8);
            test(s1._length == 8, __LINE__);
        }
        // test method : unsigned long labelCode() const
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            test(s1._labelCode == s1.labelCode(), __LINE__);
        }
        // test method : const String& string() const
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            test(s1._string == s1.string(), __LINE__);
        }
        // test method : const String& sourceName() const
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            test(s1._srcName == s1.sourceName(), __LINE__);
        }
        // test method : void setLabelCode(unsigned long lc)
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            s1.setLabelCode(12);
            test(s1._labelCode == 12, __LINE__);
        }
        // test method : void setString(const String& s)
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            s1.setString("12");
            test(s1._string == "12", __LINE__);
        }
        // test method : void setSourceName(const String& sn)
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            s1.setSourceName("12");
            test(s1._srcName == "12", __LINE__);
        }
        // test method : bool belongsToSameSegServer(const SegAbstract&) const
        {
            Seg s1(server, 1,2,3, "aa", "bb");
            Seg s2(server, 1,2,3, "aa", "bb");
            SegServer server2;
            Seg s3(server2, 1,2,3, "aa", "bb");

            test(s1.getServer().isSameObject(s2.getServer()), __LINE__);
            test(!s1.getServer().isSameObject(s3.getServer()), __LINE__);
        }
        // test method : void SegAbstract::removeAllOwners(const K&)
        {
            SegServer ss;
            Seg& s1 = ss.createSeg(5, 10);
            SegCluster& cl1 = ss.createCluster();
            SegCluster& cl2 = ss.createCluster();
            cl1.add(s1);
            cl2.add(s1);
            s1.removeAllOwners(K::k);
            test(s1._ownersVect.size() == 0, __LINE__);
            test(cl1.getCount() == 0, __LINE__);
            test(cl2.getCount() == 0, __LINE__);
        }
        // test method void getExtremeBoundaries(const K&, unsigned long& b,
        //   (private method)       unsigned long& e, bool& isDefined) const
        {
            SegServer ss;
            Seg s1(ss, 5, 5);
            unsigned long b = 6, l = 8;
            bool isDefined = false;
            s1.getExtremeBoundaries(K::k, b, l, isDefined);
            test(b == 5, __LINE__);
            test(l == 10, __LINE__);
            test(isDefined == true, __LINE__);
            s1.setBegin(7);
            s1.setLength(0);
            s1.getExtremeBoundaries(K::k, b, l, isDefined);
            test(b == 5, __LINE__);
            test(l == 10, __LINE__);
            s1.setBegin(1);
            s1.setLength(6);
            s1.getExtremeBoundaries(K::k, b, l, isDefined);
            test(b == 1, __LINE__);
            test(l == 10, __LINE__);
            s1.setBegin(1);
            s1.setLength(14);
            s1.getExtremeBoundaries(K::k, b, l, isDefined);
            test(b == 1, __LINE__);
            test(l == 15, __LINE__);
        }
        // test method Seg& merge(Seg& s)
        {
            SegServer ss;
            Seg& s1 = ss.createSeg(10, 10, 1, "seg1", "file1");
            s1.list().addLine().addElement("element1").addElement("element2");
            s1.list().addLine().addElement("element3");
            Seg& s2 = ss.createSeg(1, 2, 2, "seg2", "file2");
            s2.list().addLine().addElement("element4").addElement("element5");
            s2.list().addLine().addElement("element6");
            s1.merge(s2);
            test(ss.getSegCount() == 1, __LINE__);
            test(s1.begin() == 1, __LINE__);
            test(s1.length() == 19, __LINE__);
            test(s1.labelCode() == 1, __LINE__);
            test(s1.string() == "seg1 seg2", __LINE__);
            test(s1.sourceName() == "file1 file2", __LINE__);
            const XList& l = s1.list();
            test(l.getLineCount() == 4, __LINE__);
            test(l.getLine(0).getElementCount() == 2, __LINE__);
            test(l.getLine(1).getElementCount() == 1, __LINE__);
            test(l.getLine(2).getElementCount() == 2, __LINE__);
            test(l.getLine(3).getElementCount() == 1, __LINE__);
            test(l.getLine(0).getElement(0) == "element1", __LINE__);
            test(l.getLine(0).getElement(1) == "element2", __LINE__);
            test(l.getLine(1).getElement(0) == "element3", __LINE__);
            test(l.getLine(2).getElement(0) == "element4", __LINE__);
            test(l.getLine(2).getElement(1) == "element5", __LINE__);
            test(l.getLine(3).getElement(0) == "element6", __LINE__);
        }
        // test method : Seg& split(unsigned long i)
        {
            SegServer ss;
            Seg& s0 = ss.createSeg(2,10);
            try { s0.split(12); showError(__LINE__); }
            catch (Exception&) {}
            try
            {
                Seg& s1 = s0.split(5);
                test(ss.getSegCount() == 2, __LINE__);
                test(s0.begin() == 2, __LINE__);
                test(s0.length() == 3, __LINE__);
                test(s1.begin() == 5, __LINE__);
                test(s1.length() == 7, __LINE__);
                Seg& s2 = s0.split(4);
                test(s0.begin() == 2, __LINE__);
                test(s0.length() == 2, __LINE__);
                test(s2.begin() == 4, __LINE__);
                test(s2.length() == 1, __LINE__);
                Seg& s3 = s0.split(2);
                test(s0.begin() == 2, __LINE__);
                test(s0.length() == 0, __LINE__);
                test(s3.begin() == 2, __LINE__);
                test(s3.length() == 2, __LINE__);
            }
            catch (Exception&) { showError(__LINE__); }
        }

    } // local objects are deleted here

    return endTest();
}
//-------------------------------------------------------------------------
String TestSeg::getClassName() const { return "TestSeg"; }
//-------------------------------------------------------------------------
TestSeg::~TestSeg() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestSeg_cpp)

