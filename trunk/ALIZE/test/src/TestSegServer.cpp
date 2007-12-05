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

#if !defined(ALIZE_TestSegServer_cpp)
#define ALIZE_TestSegServer_cpp

#include "TestSegServer.h"
#include "SegServer.h"
#include "AutoDestructor.h"
#include <iostream>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestSegServer::TestSegServer()
:Test() {}
//-------------------------------------------------------------------------
bool TestSegServer::exec()
{

    Test::exec();
    {
        // test method : String getClassName() const
        {
            test(SegServer().getClassName() == "SegServer", __LINE__);
        }
        // test method : explicit SegServer();
        {
            SegServer s2;
            test(s2._serverName == "", __LINE__);
            test(s2._segVect.size() == 0, __LINE__);
            test(s2._clusterVect.size() == 0, __LINE__);
            test(s2._nextClusterId == 0, __LINE__);
        }
        // test method : static SegServer& create(const String& serverName = "");
        {
            SegServer& s1 = SegServer::create();
            AutoDestructor ad1(s1);
            test(s1._serverName == "", __LINE__);
            SegServer& s2 = SegServer::create();
            AutoDestructor ad2(s2);
            test(s2._segVect.size() == 0, __LINE__);
            test(s2._clusterVect.size() == 0, __LINE__);
            test(s2._nextClusterId == 0, __LINE__);
        }
        // test method : const String& getServerName() const;
        {
            SegServer s2;
            s2._serverName = "123";
            test(s2.getServerName() == "123", __LINE__);
        }
        // test method : void setServerName(const String& s);
        {
            SegServer s2;
            s2.setServerName("567");
            test(s2.getServerName() == "567", __LINE__);
        }
        // test method : Seg& createSeg(unsigned long b = 0,
        //               unsigned long l = 0, unsigned long lc = 0,
        //               const String& s = "", const String& sn = "");
        {
            SegServer ss;
            Seg& s1 = ss.createSeg(1, 1, 3, "4", "5");
            test(ss._segVect.size() == 1, __LINE__);
            test(ss._segVect.getObject(0).isSameObject(s1), __LINE__);
            test(s1._pServer == &ss, __LINE__);
            test(s1._begin == 1, __LINE__);
            test(s1._length == 1, __LINE__);
            test(s1.labelCode() == 3, __LINE__);
            test(s1.string() == "4", __LINE__);
            test(s1.sourceName() == "5", __LINE__);
        }
        // test method : SegCluster& createCluster(unsigned long lc = 0,
        //               const String& s = "", const String& sn = "");
        {
            SegServer ss;
            SegCluster& s1 = ss.createCluster(3, "4", "5");
            test(ss._clusterVect.size() == 1, __LINE__);
            test(ss._clusterVect.getObject(0).isSameObject(s1), __LINE__);
            test(s1._pServer == &ss, __LINE__);
            test(s1.labelCode() == 3, __LINE__);
            test(s1.string() == "4", __LINE__);
            test(s1.sourceName() == "5", __LINE__);
            test(ss._nextClusterId == 1, __LINE__);
            test(s1.getId() == 0, __LINE__);

            // test auto define id
            SegCluster& s2 = ss.createCluster(3, "4", "5");
            test(s2.getId() == 1, __LINE__);
            test(ss._nextClusterId == 2, __LINE__);
            ss.setClusterId(s2, 2); // alter id of s2
            SegCluster& s3 = ss.createCluster(3, "4", "5");
            test(s3.getId() == 3, __LINE__);
            test(ss._nextClusterId == 4, __LINE__);
        }
        // test method : Seg& getSeg(unsigned long idx) const;
        {
            SegServer ss;
            try { ss.getSeg(0); showError(__LINE__); }
            catch (Exception&) {}
            try
            {
                Seg& s1 = ss.createSeg();
                try { ss.getSeg(1); showError(__LINE__); }
                catch (Exception&) {}
                test(ss._segVect.size() == 1, __LINE__);
                test(ss.getSeg(0).isSameObject(s1), __LINE__);
                Seg& s2 = ss.createSeg();
                test(ss.getSeg(0).isSameObject(s1), __LINE__);
                test(ss.getSeg(1).isSameObject(s2), __LINE__);
                test(ss._segVect.size() == 2, __LINE__);
                Seg& s3 = ss.createSeg();
                test(ss.getSeg(2).isSameObject(s3), __LINE__);
                test(ss._segVect.size() == 3, __LINE__);
            }
            catch (Exception&) { showError(__LINE__); }
        }
        // test method : unsigned long getSegCount() const;
        {
            SegServer ss;
            ss.createSeg();
            test(ss.getSegCount() == 1, __LINE__);
            ss.createSeg();
            test(ss.getSegCount() == 2, __LINE__);
            ss.createSeg();
            test(ss.getSegCount() == 3, __LINE__);
        }
        // test method : SegCluster& getCluster(unsigned long idx) const;
        {
            SegServer ss;
            try { ss.getCluster(0); showError(__LINE__); }
            catch (Exception&) {}
            try
            {
                SegCluster& s1 = ss.createCluster();
                try { ss.getCluster(1); showError(__LINE__); }
                catch (Exception&) {}
                test(ss._clusterVect.size() == 1, __LINE__);
                test(ss.getCluster(0).isSameObject(s1), __LINE__);
                SegCluster& s2 = ss.createCluster();
                test(ss.getCluster(0).isSameObject(s1), __LINE__);
                test(ss.getCluster(1).isSameObject(s2), __LINE__);
                test(ss._clusterVect.size() == 2, __LINE__);
                SegCluster& s3 = ss.createCluster();
                test(ss.getCluster(2).isSameObject(s3), __LINE__);
                test(ss._clusterVect.size() == 3, __LINE__);
            }
            catch (Exception&) { showError(__LINE__); }
        }
        // test method : unsigned long getClusterCount() const;
        {
            SegServer ss;
            ss.createCluster();
            test(ss.getClusterCount() == 1, __LINE__);
            ss.createCluster();
            test(ss.getClusterCount() == 2, __LINE__);
            ss.createCluster();
            test(ss.getClusterCount() == 3, __LINE__);
        }
        // test method : void setClusterId(SegCluster& s, unsigned long id);
        {
            SegServer ss;
            SegCluster& s1 = ss.createCluster(3, "4", "5");
            SegCluster& s2 = ss.createCluster(3, "4", "5");
            ss.setClusterId(s2, 2); // alter id of s2
            SegCluster& s3 = ss.createCluster(3, "4", "5");
            ss.setClusterId(s3, 45);
            test(s1.getId() == 0, __LINE__);
            test(s2.getId() == 2, __LINE__);
            test(s3.getId() == 45, __LINE__);
            try { ss.setClusterId(s3, 45); }
            catch (Exception&) { showError(__LINE__); }
            try { ss.setClusterId(s3, 0); showError(__LINE__); }
            catch (Exception&) {}
        }
        // test method : long getClusterIndexById(unsigned long id);
        {
            SegServer ss;
            ss.createCluster(3, "4", "5");
            SegCluster& s2 = ss.createCluster(3, "4", "5");
            ss.setClusterId(s2, 2); // alter id of s2
            ss.createCluster(3, "4", "5");
            test(ss.getClusterIndexById(0) == 0, __LINE__);
            test(ss.getClusterIndexById(1) == -1, __LINE__);
            test(ss.getClusterIndexById(2) == 1, __LINE__);
            test(ss.getClusterIndexById(3) == 2, __LINE__);
            test(ss.getClusterIndexById(4) == -1, __LINE__);
        }
        // test method : SegCluster& getClusterById(unsigned long id);
        {
            SegServer ss;
            SegCluster& s1 = ss.createCluster(3, "4", "5");
            SegCluster& s2 = ss.createCluster(3, "4", "5");
            ss.setClusterId(s2, 2); // alter id of s2
            SegCluster& s3 = ss.createCluster(3, "4", "5");
            try 
            {
                test(ss.getClusterById(0).isSameObject(s1), __LINE__);
                test(ss.getClusterById(2).isSameObject(s2), __LINE__);
                test(ss.getClusterById(3).isSameObject(s3), __LINE__);
            }
            catch (Exception&) { showError(__LINE__); }
            try { ss.getClusterById(1); showError(__LINE__); }
            catch (Exception&) {}
        }
        // test method : SegAbstract& matchingSegAbstract(
        //                                    const SegAbstract&) const;
        {
            SegServer ss1;
            SegAbstract& s1a = ss1.createSeg();
            SegAbstract& s1b = ss1.createSeg();
            SegAbstract& s1c = ss1.createCluster();
            SegAbstract& s1d = ss1.createCluster();
            SegServer ss2;
            SegAbstract& s2a = ss2.createSeg();
            SegAbstract& s2b = ss2.createSeg();
            SegAbstract& s2c = ss2.createCluster();
            SegAbstract& s2d = ss2.createCluster();
            test(ss2.matchingSegAbstract(s1a).isSameObject(s2a), __LINE__);
            test(ss2.matchingSegAbstract(s1b).isSameObject(s2b), __LINE__);
            test(ss2.matchingSegAbstract(s1c).isSameObject(s2c), __LINE__);
            test(ss2.matchingSegAbstract(s1d).isSameObject(s2d), __LINE__);
        }
        // test method : void assign(const SegServer&)
        // used by both copy constructor and assignment operator
        {
            SegServer ss1;
            Seg& s1a = ss1.createSeg(0,1,2,"a","b");
            Seg& s1b = ss1.createSeg(3,4,5,"c","d");
            Seg& s1c = ss1.createSeg(6,7,8,"e","f");
            SegCluster& cl1a = ss1.createCluster(9,"g","h");
            SegCluster& cl1b = ss1.createCluster(10,"i","j");
            SegCluster& cl1c = ss1.createCluster(11,"j","l");
            SegCluster& cl1d = ss1.createCluster(12,"m","n");
            cl1a.add(s1b);
            cl1a.add(s1c);
            cl1b.add(s1a);
            cl1b.add(s1b);
            cl1c.add(s1a);
            cl1c.add(cl1a);
            cl1d.add(cl1b);
            cl1d.add(cl1c);
            cl1d.add(cl1a);
            s1a.list().addLine().addElement("123");
            cl1a.list().addLine().addElement("123");

            SegServer ss2;
            ss2.assign(ss1);
            test(ss2.getClusterCount() == ss1.getClusterCount(), __LINE__);
            test(ss2.getSegCount() == ss1.getSegCount(), __LINE__);
            unsigned long i;
            for (i=0; i<ss2.getClusterCount(); i++)
            {
                SegCluster& x = ss1.getCluster(i);
                SegCluster& y = ss2.getCluster(i);
                test(x.labelCode() == y.labelCode(), __LINE__);
                test(x.string() == y.string(), __LINE__);
                test(x.sourceName() == y.sourceName(), __LINE__);
                test(x.list() == y.list(), __LINE__);
                test(x.getId() == y.getId(), __LINE__);
            }
            for (i=0; i<ss2.getSegCount(); i++)
            {
                Seg& x = ss1.getSeg(i);
                Seg& y = ss2.getSeg(i);
                test(x._begin == y._begin, __LINE__);
                test(x._length == y._length, __LINE__);
                test(x.labelCode() == y.labelCode(), __LINE__);
                test(x.string() == y.string(), __LINE__);
                test(x.sourceName() == y.sourceName(), __LINE__);
            }
            Seg& s2a = ss2.getSeg(0);
            Seg& s2b = ss2.getSeg(1);
            Seg& s2c = ss2.getSeg(2);
            SegCluster& cl2a = ss2.getCluster(0);
            SegCluster& cl2b = ss2.getCluster(1);
            SegCluster& cl2c = ss2.getCluster(2);
            SegCluster& cl2d = ss2.getCluster(3);
            test(cl2a.get(0).isSameObject(s2b), __LINE__);
            test(cl2a.get(1).isSameObject(s2c), __LINE__);
            test(cl2b.get(0).isSameObject(s2a), __LINE__);
            test(cl2b.get(1).isSameObject(s2b), __LINE__);
            test(cl2c.get(0).isSameObject(s2a), __LINE__);
            test(cl2c.get(1).isSameObject(cl2a), __LINE__);
            test(cl2d.get(0).isSameObject(cl2b), __LINE__);
            test(cl2d.get(1).isSameObject(cl2c), __LINE__);
            test(cl2d.get(2).isSameObject(cl2a), __LINE__);

            test(s2c._ownersVect.getObject(0).isSameObject(cl2a), __LINE__);
            test(s2b._ownersVect.getObject(0).isSameObject(cl2a), __LINE__);
            test(s2b._ownersVect.getObject(1).isSameObject(cl2b), __LINE__);
            test(s2a._ownersVect.getObject(0).isSameObject(cl2b), __LINE__);
            test(s2a._ownersVect.getObject(1).isSameObject(cl2c), __LINE__);
            test(cl2a._ownersVect.getObject(0).isSameObject(cl2c), __LINE__);
            test(cl2a._ownersVect.getObject(1).isSameObject(cl2d), __LINE__);
            test(cl2b._ownersVect.getObject(0).isSameObject(cl2d), __LINE__);
            test(cl2c._ownersVect.getObject(0).isSameObject(cl2d), __LINE__);
            test(cl2d._ownersVect.size() == 0, __LINE__);
        }
        // test method : void remove(Seg& s)
        // test method : void remove(SegCluster& s)
        {
            SegServer ss;

            try { ss.remove(SegServer().createSeg()); showError(__LINE__); }
            catch (Exception&) {}
            try
            {
                Seg& s1 = ss.createSeg();
                Seg& s2 = ss.createSeg();
                Seg& s3 = ss.createSeg();
                SegCluster& cl1 = ss.createCluster();
                SegCluster& cl2 = ss.createCluster();
                cl1.add(s1);
                cl1.add(s2);
                cl1.add(s3);
                cl2.add(s1);
                cl2.add(s2);
                cl2.add(s3);

                ss.remove(s2);
                test(ss.getSegCount() == 2, __LINE__);
                test(ss.getSeg(0).isSameObject(s1), __LINE__);
                test(ss.getSeg(1).isSameObject(s3), __LINE__);
                test(s1._ownersVect.size() == 2, __LINE__);
                test(s1._ownersVect.getObject(0).isSameObject(cl1), __LINE__);
                test(s1._ownersVect.getObject(1).isSameObject(cl2), __LINE__);
                test(s3._ownersVect.getObject(0).isSameObject(cl1), __LINE__);
                test(s3._ownersVect.getObject(1).isSameObject(cl2), __LINE__);
                test(cl1._vect.getObject(0).isSameObject(s1), __LINE__);
                test(cl1._vect.getObject(1).isSameObject(s3), __LINE__);
                test(cl2._vect.getObject(0).isSameObject(s1), __LINE__);
                test(cl2._vect.getObject(1).isSameObject(s3), __LINE__);
                test(ss.getClusterCount() == 2, __LINE__);
                test(ss.getCluster(0).isSameObject(cl1), __LINE__);
                test(ss.getCluster(1).isSameObject(cl2), __LINE__);

                ss.remove(cl2);
                test(ss.getSegCount() == 2, __LINE__);
                test(ss.getSeg(0).isSameObject(s1), __LINE__);
                test(ss.getSeg(1).isSameObject(s3), __LINE__);
                test(ss.getClusterCount() == 1, __LINE__);
                test(ss.getCluster(0).isSameObject(cl1), __LINE__);
                test(s1._ownersVect.getObject(0).isSameObject(cl1), __LINE__);
                test(s3._ownersVect.getObject(0).isSameObject(cl1), __LINE__);
                test(cl1._vect.getObject(0).isSameObject(s1), __LINE__);
                test(cl1._vect.getObject(1).isSameObject(s3), __LINE__);

                ss.remove(s3);
                test(ss.getSegCount() == 1, __LINE__);
                test(ss.getSeg(0).isSameObject(s1), __LINE__);
                test(ss.getClusterCount() == 1, __LINE__);
                test(ss.getCluster(0).isSameObject(cl1), __LINE__);
                test(s1._ownersVect.getObject(0).isSameObject(cl1), __LINE__);
                test(cl1._vect.getObject(0).isSameObject(s1), __LINE__);

                ss.remove(s1);
                test(ss.getSegCount() == 0, __LINE__);
                test(ss.getClusterCount() == 1, __LINE__);
                test(ss.getCluster(0).isSameObject(cl1), __LINE__);
                test(cl1._vect.size() == 0, __LINE__);

                ss.remove(cl1);
                test(ss.getSegCount() == 0, __LINE__);
                test(ss.getClusterCount() == 0, __LINE__);
            }
            catch (Exception&) { showError(__LINE__); }
        }
        // test method : void removeAllSegs()
        // test method : void removeAllClusters()
        {
            SegServer ss;
            ss.createSeg(10,20);
            ss.createSeg(8,25);
            ss.createSeg(9,30);
            ss.createSeg(3,4);
            ss.createCluster();
            ss.createCluster();
            ss.getCluster(0).add(ss.getSeg(0));
            ss.getCluster(0).add(ss.getSeg(1));
            ss.getCluster(0).add(ss.getSeg(2));
            ss.getCluster(1).add(ss.getSeg(3));
            ss.getCluster(1).add(ss.getCluster(0));
            ss.removeAllClusters();
            test(ss._nextClusterId == 0, __LINE__);
            ss.removeAllSegs();
            test(ss.getSegCount() == 0, __LINE__);
            test(ss.getClusterCount() == 0, __LINE__);
        }
        // test method : Seg& duplicateSeg(const K&, const Seg& s)
        {
            SegServer ss;
            Seg& s1 = ss.createSeg(1,2,3, "aa", "bb");
            s1.list().addLine().addElement("123");
            Seg& s2 = ss.duplicateSeg(s1);
            test(s2._begin == s1._begin, __LINE__);
            test(s2._length == s1._length, __LINE__);
            test(s2._labelCode == s1._labelCode, __LINE__);
            test(s2._string == s1._string, __LINE__);
            test(s2._srcName == s1._srcName, __LINE__);
            test(s2._pServer == &ss, __LINE__);
            test(s2._list == s1._list, __LINE__);
        }

        // test method : unsigned long getIndex(const SegAbstract& s)
        {
            SegServer ss, ss2;
            const Seg& s0 = ss.createSeg();
            const Seg& s1 = ss.createSeg();
            const Seg& s2 = ss.createSeg();
            SegCluster& cl0 = ss.createCluster();
            SegCluster& cl1 = ss.createCluster();
            ss2.createSeg();
            ss2.createCluster();
            try
            {
                ss.getIndex(ss2.getSeg(0)); showError(__LINE__);
                ss.getIndex(ss2.getCluster(0)); showError(__LINE__);
            }
            catch (Exception&) {}
            try
            {
                test(ss.getIndex(s0) == 0, __LINE__);
                test(ss.getIndex(s1) == 1, __LINE__);
                test(ss.getIndex(s2) == 2, __LINE__);
                test(ss.getIndex(cl0) == 0, __LINE__);
                test(ss.getIndex(cl1) == 1, __LINE__);
            }
            catch (Exception&) { showError(__LINE__); }


        }

    } // local objects are deleted here

    return endTest();
}
//-------------------------------------------------------------------------
String TestSegServer::getClassName() const { return "TestSegServer"; }
//-------------------------------------------------------------------------
TestSegServer::~TestSegServer() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestSegServer_cpp)

