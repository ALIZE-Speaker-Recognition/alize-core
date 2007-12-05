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

#if !defined(ALIZE_TestXList_cpp)
#define ALIZE_TestXList_cpp

#include "TestXList.h"
#include "XList.h"
#include "AutoDestructor.h"
#include "Config.h"
#include <cstdio>

using namespace alize;

//-------------------------------------------------------------------------
TestXList::TestXList()
:Test() {}
//-------------------------------------------------------------------------
bool TestXList::exec()
{
    Test::exec();
    {
        // Test method : virtual String getClassName() const;
        {
            test(XList().getClassName() == "XList", __LINE__);
        }

        // Test method : XList();
        {
            XList l;
            test(l._vector.size() == 0, __LINE__);
            test(l._current == 0, __LINE__);
        }

        // Test method : static XList& create();
        {
            XList& l = XList::create();
            AutoDestructor ad(l);
            test(l._vector.size() == 0, __LINE__);
            test(l._current == 0, __LINE__);
        }

        // Test method : void addLine();
        {
            XList l;
            l.addLine();
            test(l._vector.size() == 1, __LINE__);
            test(l._vector.getObject(0).getClassName()=="XLine", __LINE__);
            test(l._current == 0, __LINE__);
            l.addLine();
            test(l._vector.size() == 2, __LINE__);
            test(l._current == 1, __LINE__);
            l.addLine();
            test(l._vector.size() == 3, __LINE__);
            test(l._current == 2, __LINE__);
        }

        // Test method : XLine& getLine(unsigned long idx) const;
        {
            XList l;
            try { l.getLine(0); showError(__LINE__);}
            catch (IndexOutOfBoundsException&) {}
            l.addLine();
            try 
            {
                test(&l.getLine(0) == &l._vector.getObject(0), __LINE__);
            }
            catch (IndexOutOfBoundsException&) { showError(__LINE__); }
            try { l.getLine(1); showError(__LINE__); }
            catch (IndexOutOfBoundsException&) {}
            test(l._current == 0, __LINE__);
            l.addLine();
            try
            {
                test(&l.getLine(0) == &l._vector.getObject(0), __LINE__);
                test(&l.getLine(1) == &l._vector.getObject(1), __LINE__);
            }
            catch (IndexOutOfBoundsException&) { showError(__LINE__); }
            try { l.getLine(2); showError(__LINE__); }
            catch (IndexOutOfBoundsException&) {}
            test(l._current == 1, __LINE__);
            l.addLine();
            try
            {
                test(&l.getLine(0) == &l._vector.getObject(0), __LINE__);
                test(&l.getLine(1) == &l._vector.getObject(1), __LINE__);
                test(&l.getLine(2) == &l._vector.getObject(2), __LINE__);
            }
            catch (IndexOutOfBoundsException&) { showError(__LINE__); }
            try { l.getLine(3); showError(__LINE__); }
            catch (IndexOutOfBoundsException&) {}
            test(l._current == 2, __LINE__);
        }

        // Test method : XList(const XList&);
        {
            XList l;
            XList l1(l);
            test(l1._vector.size() == 0, __LINE__);
            test(l1._current == 0, __LINE__);
            l.addLine().addElement("aa");
            l.addLine().addElement("bb");
            XList l2(l);
            test(l2._vector.size() == 2, __LINE__);
            test(l2._current == 0, __LINE__);
            test(l2.getLine(0).getElement(0) == "aa", __LINE__);
            test(l2.getLine(1).getElement(0) == "bb", __LINE__);
        }

        // Test method : const XList& operator=(const XList& c);
        {
            XList l;
            XList l1;
            l1 = l;
            test(l1._vector.size() == 0, __LINE__);
            test(l1._current == 0, __LINE__);
            l.addLine().addElement("aa");
            l.addLine().addElement("bb");
            XList l2;
            l2 = l;
            test(l2._vector.size() == 2, __LINE__);
            test(l2._current == 0, __LINE__);
            test(l2.getLine(0).getElement(0) == "aa", __LINE__);
            test(l2.getLine(1).getElement(0) == "bb", __LINE__);
        }

        // Test method : bool operator==(const XList& c) const;
        //  and method : bool operator!=(const XList& c) const;
        {
            XList l;
            XList l1;
            test(l == l1, __LINE__);
            test(!(l != l1), __LINE__);
            l.addLine().addElement("aa");
            test(l != l1, __LINE__);
            test(!(l == l1), __LINE__);
            l1.addLine().addElement("aa");
            test(l == l1, __LINE__);
            test(!(l != l1), __LINE__);
            l.addLine().addElement("aa");
            l1.addLine().addElement("cc");
            test(l != l1, __LINE__);
            test(!(l == l1), __LINE__);
        }

        // Test method : void reset();
        {
            XList l;
            l.reset();
            test(l._vector.size() == 0, __LINE__);
            test(l._current == 0, __LINE__);
            l.addLine();
            l.addLine();
            l.reset();
            test(l._vector.size() == 0, __LINE__);
            test(l._current == 0, __LINE__);
        }

        // Test method : String* getLine();
        {
            XList l;
            test(l.getLine() == NULL, __LINE__);
            l.addLine().addElement("aa");
            XLine* p = l.getLine();
            test(p != NULL && p->getElement(0) == "aa", __LINE__);
            test(l.getLine() == NULL, __LINE__);
            l.reset();
            l.addLine().addElement("aa");
            l.addLine().addElement("bb");
            p = l.getLine();
            test(p != NULL && p->getElement(0) == "bb", __LINE__);
            test(l.getLine() == NULL, __LINE__);
            l.getLine(0); // set current to 0
            p = l.getLine();
            test(p != NULL && p->getElement(0) == "aa", __LINE__);
            p = l.getLine();
            test(p != NULL && p->getElement(0) == "bb", __LINE__);
            test(l.getLine() == NULL, __LINE__);
        }

        // Test method : unsigned long getLineCount() const;
        {
            XList l;
            test(l.getLineCount() == 0, __LINE__);
            l.addLine();
            test(l.getLineCount() == 1, __LINE__);
            l.addLine();
            test(l.getLineCount() == 2, __LINE__);
            l.reset();
            test(l.getLineCount() == 0, __LINE__);
        }

        // Test method : XList& getAllElements() const;
        {
            XList list;
            XLine& l1 = list.addLine();
            l1.addElement("aa");
            l1.addElement("bb");
            list.addLine();
            XLine& l2 = list.addLine();
            l2.addElement("cc");
            XLine line;
            line.addElement("aa");
            line.addElement("bb");
            line.addElement("cc");
            test(list.getAllElements() == line, __LINE__);
        }

        // Test method : void rewind() const;
        {
            XList l;
            l.addLine().addElement("a");
            l.addLine();
            l.getLine(1);
            l.rewind();
            XLine* p = l.getLine();
            test(p!=NULL && p->getElement(0)=="a", __LINE__);
        }

        // test method : void load(const FileName& f, const Config& c)
        {
            char file[] = "./TestXListFileReader.tmp";
            FILE* p = fopen(file, "w");
            char s[] = "aa";
            fwrite(&s, sizeof(s)-1, 1, p);
            fclose(p);

            XList l1, l2;
            Config c;
            l1.load(file, c);
            l2.addLine().addElement("aa");
            test(l1 == l2, __LINE__);
            // for more tests, see file TestXListFileReader.cpp
            remove(file);
        }

        // test method : XList(const FileName& f, const Config& c)
        {
            char file[] = "./TestXListFileReader.tmp";
            FILE* p = fopen(file, "w");
            char s[] = "aa";
            fwrite(&s, sizeof(s)-1, 1, p);
            fclose(p);
            Config c;
            XList l1(file, c), l2;
            l2.addLine().addElement("aa");
            test(l1 == l2, __LINE__);
            // for more tests, see file TestXListFileReader.cpp
            remove(file);
        }
        // test method XLine* findLine(const String& key,
        //                             unsigned long idx = 0) const
        {
            XList list;
            list.addLine().addElement("00").addElement("11").addElement("22");
            list.addLine().addElement("33").addElement("44").addElement("55");
            list.addLine().addElement("66").addElement("77");
            XLine* p = list.findLine("33");
            test (p != NULL && p->getElement(0) == "33" , __LINE__);
            p = list.findLine("33", 1);
            test (p == NULL, __LINE__);
            p = list.findLine("44", 1);
            test (p != NULL && p->getElement(1) == "44", __LINE__);
            p = list.findLine("77", 3);
            test (p == NULL, __LINE__);
        }

    } // local objects are deleted here

    return endTest();
}
//-------------------------------------------------------------------------
String TestXList::getClassName() const
{
    return "TestXList";
}
//-------------------------------------------------------------------------
TestXList::~TestXList() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestXList_cpp)
