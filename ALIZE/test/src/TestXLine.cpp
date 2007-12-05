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

#if !defined(ALIZE_TestXLine_cpp)
#define ALIZE_TestXLine_cpp

#include "TestXLine.h"
#include "XLine.h"
#include "AutoDestructor.h"
using namespace alize;

//-------------------------------------------------------------------------
TestXLine::TestXLine()
:Test() {}
//-------------------------------------------------------------------------
bool TestXLine::exec()
{
  Test::exec();
  {
    // Test method : virtual String getClassName() const;
    {
      test(XLine().getClassName() == "XLine", __LINE__);
    }

    // Test method : XLine();
    {
      XLine l;
      test(l._vector.size() == 0, __LINE__);
      test(l._current == 0, __LINE__);
    }

    // Test method : static XLine& create();
    {
      XLine& l = XLine::create();
      AutoDestructor ad(l);
      test(l._vector.size() == 0, __LINE__);
      test(l._current == 0, __LINE__);
    }

    // Test method : void addElement(const String& e);
    {
      XLine l;
      l.addElement("coucou");
      test(l._vector.size() == 1, __LINE__);
      test(l._vector.getObject(0).getClassName()=="String", __LINE__);
      test(l._vector.getObject(0)=="coucou", __LINE__);
      test(l._current == 0, __LINE__);
      l.addElement("aaaa");
      test(l._vector.size() == 2, __LINE__);
      test(l._vector.getObject(1)=="aaaa", __LINE__);
      test(l._current == 1, __LINE__);
      l.addElement("");
      test(l._vector.size() == 3, __LINE__);
      test(l._vector.getObject(2)=="", __LINE__);
      test(l._current == 2, __LINE__);
    }

    // Test method : String& getElement(const unsigned long idx,
    //                  const bool becomeCurrent = true) const;
    {
      XLine l;
      try { l.getElement(0); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      l.addElement("aa");
      try { test(l.getElement(0) == "aa", __LINE__); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      try { l.getElement(1); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      test(l._current == 0, __LINE__);
      l.addElement("bb");
      try { test(l.getElement(0) == "aa", __LINE__); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      try { test(l.getElement(1) == "bb", __LINE__); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      try { l.getElement(2); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      test(l._current == 1, __LINE__);
      l.addElement("cc");
      try { test(l.getElement(0) == "aa", __LINE__); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      try { test(l.getElement(1) == "bb", __LINE__); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      try { test(l.getElement(2) == "cc", __LINE__); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      try { l.getElement(3); showError(__LINE__); }
      catch (IndexOutOfBoundsException&) {}
      l.getElement(0) = "123";
      test(l.getElement(0) == "123", __LINE__);
      
      l.rewind();
      test(l._current == 0, __LINE__);
      l.getElement(2, false); // does not change current
      test(l._current == 0, __LINE__);
      l.getElement(2, true); // change current
      test(l._current == 2, __LINE__);
    }

    // Test method : XLine(const XLine&);
    {
      XLine l;
      XLine l1(l);
      test(l1._vector.size() == 0, __LINE__);
      test(l1._current == 0, __LINE__);
      l.addElement("aa");
      l.addElement("bb");
      XLine l2(l);
      test(l2._vector.size() == 2, __LINE__);
      test(l2._current == 0, __LINE__);
      test(l2.getElement(0) == "aa", __LINE__);
      test(l2.getElement(1) == "bb", __LINE__);
    }

    // Test method : XLine& duplicate() const;
    {
      XLine l;
      XLine& l1 = l.duplicate();
      AutoDestructor ad1(l1);
      test(l1._vector.size() == 0, __LINE__);
      test(l1._current == 0, __LINE__);
      l.addElement("aa");
      l.addElement("bb");
      XLine& l2 = l.duplicate();
      AutoDestructor ad2(l2);
      test(l2._vector.size() == 2, __LINE__);
      test(l2._current == 0, __LINE__);
      test(l2.getElement(0) == "aa", __LINE__);
      test(l2.getElement(1) == "bb", __LINE__);
    }

    // Test method : const XLine& operator=(const XLine& c);
    {
      XLine l;
      XLine l1;
      l1 = l;
      test(l1._vector.size() == 0, __LINE__);
      test(l1._current == 0, __LINE__);
      l.addElement("aa");
      l.addElement("bb");
      XLine l2;
      l2 = l;
      test(l2._vector.size() == 2, __LINE__);
      test(l2._current == 0, __LINE__);
      test(l2.getElement(0) == "aa", __LINE__);
      test(l2.getElement(1) == "bb", __LINE__);
    }

    // Test method : bool operator==(const XLine& c) const;
    //  and method : bool operator!=(const XLine& c) const;
    {
      XLine l;
      XLine l1;
      test(l == l1, __LINE__);
      test(!(l != l1), __LINE__);
      l.addElement("aa");
      test(l != l1, __LINE__);
      test(!(l == l1), __LINE__);
      l1.addElement("aa");
      test(l == l1, __LINE__);
      test(!(l != l1), __LINE__);
      l.addElement("aa");
      l1.addElement("cc");
      test(l != l1, __LINE__);
      test(!(l == l1), __LINE__);
    }

    // Test method : void reset();
    {
      XLine l;
      l.reset();
      test(l._vector.size() == 0, __LINE__);
      test(l._current == 0, __LINE__);
      l.addElement("aa");
      l.addElement("bb");
      l.reset();
      test(l._vector.size() == 0, __LINE__);
      test(l._current == 0, __LINE__);
    }

    // Test method : String* getElement();
    {
      XLine l;
      test(l.getElement() == NULL, __LINE__);
      l.addElement("aa");
      String* p = l.getElement();
      test(p != NULL && *p == "aa", __LINE__);
      test(l.getElement() == NULL, __LINE__);
      l.reset();
      l.addElement("aa");
      l.addElement("bb");
      p = l.getElement();
      test(p != NULL && *p == "bb", __LINE__);
      test(l.getElement() == NULL, __LINE__);
      l.getElement(0); // set current to 0
      p = l.getElement();
      test(p != NULL && *p == "aa", __LINE__);
      p = l.getElement();
      test(p != NULL && *p == "bb", __LINE__);
      test(l.getElement() == NULL, __LINE__);
    }

    // Test method : unsigned long getElementCount() const;
    {
      XLine l;
      test(l.getElementCount() == 0, __LINE__);
      l.addElement("aa");
      test(l.getElementCount() == 1, __LINE__);
      l.addElement("bb");
      test(l.getElementCount() == 2, __LINE__);
      l.reset();
      test(l.getElementCount() == 0, __LINE__);
    }

    // Test method : const XLine& getElements() const;
    {
      XLine l, l1;
      test(l.getElements().getElementCount() == 0, __LINE__);
      l.addElement("aa");
      l.getElement(0); // set current element to 0
      l1 = l.getElements();
      test(l1.getElementCount() == 1, __LINE__);
      test(l1.getElement(0) == "aa", __LINE__);
      l.addElement("bb");
      l1 = l.getElements();
      test(l1.getElementCount() == 1, __LINE__);
      test(l1.getElement(0) == "bb", __LINE__);
      l.addElement("cc");
      l.getElement(1); // set current element to 1
      l1 = l.getElements();
      test(l1.getElementCount() == 2, __LINE__);
      test(l1.getElement(0) == "bb", __LINE__);
      test(l1.getElement(1) == "cc", __LINE__);
    }

    // Test method : void rewind() const;
    {
      XLine l;
      l.addElement("a");
      l.addElement("b");
      l.getElement(1);
      l.rewind();
      String* p = l.getElement();
      test(p!=NULL && (*p)=="a", __LINE__);
    }
    // Test method : void deleteElement(const String& e);
    {
      XLine l;
      l.addElement("a");
      l.addElement("b");
      l.addElement("c");
      try { l.deleteElement("b"); showError(__LINE__); }
      catch (Exception&) {}
      try
      {
        l.deleteElement(l.getElement(1));
        test(l.getElementCount() == 2, __LINE__);
        test(l.getElement(0) == "a", __LINE__);
        test(l.getElement(1) == "c", __LINE__);
      }
      catch (Exception&) { showError(__LINE__); }
    }
    // Test method : long getIndex(const String& e) const;
    {
      XLine l;
      l.addElement("a").addElement("bb").addElement("c");
      test(l.getIndex("a") == 0, __LINE__);
      test(l.getIndex("b") == -1, __LINE__);
      test(l.getIndex("bb") == 1, __LINE__);
      test(l.getIndex("C") == -1, __LINE__);
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestXLine::getClassName() const
{
  return "TestXLine";
}
//-------------------------------------------------------------------------
TestXLine::~TestXLine() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestXLine_cpp)
