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

#if !defined(ALIZE_TestLabelSet_cpp)
#define ALIZE_TestLabelSet_cpp

#include "TestLabelSet.h"
#include "LabelSet.h"
#include "Exception.h"
#include "Config.h"
#include <cstdio>

using namespace alize;

//-------------------------------------------------------------------------
TestLabelSet::TestLabelSet()
:Test() {}
//-------------------------------------------------------------------------
bool TestLabelSet::exec()
{
  Test::exec();
  {
    // test method : LabelSet();

    test(LabelSet()._nameVect.size() == 0, __LINE__);

    // Test method : void addLabel(real_t b, real_t e, const String& n);
    {
      LabelSet l;
      l.addLabel(1.1, 2.2, "aaaa");
      l.addLabel(3.3, 4.4, "bbbb");
      test(l._nameVect.getObject(0) == "aaaa", __LINE__);
      test(l._nameVect.getObject(1) == "bbbb", __LINE__);
      test(l._beginVect[0] == 1.1, __LINE__);
      test(l._endVect[0] == 2.2, __LINE__);
      test(l._beginVect[1] == 3.3, __LINE__);
      test(l._endVect[1] == 4.4, __LINE__);
    }

    //Test method : unsigned long size() const;
    {
      LabelSet l;
      test(l.size() == 0, __LINE__);
      l.addLabel(1.1, 2.2, "aaaa");
      test(l.size() == 1, __LINE__);
      l.addLabel(3.3, 4.4, "bbbb");
      test(l.size() == 2, __LINE__);
    }
    
    // Test method : const String& getName(unsigned long i) const;
    {
      LabelSet l;
      l.addLabel(1.1, 2.2, "aaaa");
      l.addLabel(3.3, 4.4, "bbbb");
      try { test(l.getName(0) == "aaaa", __LINE__); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      test(l.getName(1) == "bbbb", __LINE__);
      try { l.getName(2); showError(__LINE__);}
      catch (IndexOutOfBoundsException&) {}
    }

    // Test method : real_t getBegin(unsigned long i) const;
    {
      LabelSet l;
      l.addLabel(1.1, 2.2, "aaaa");
      l.addLabel(3.3, 4.4, "bbbb");
      try { test(l.getBegin(0) == 1.1, __LINE__); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      test(l.getBegin(1) == 3.3, __LINE__);
      try { l.getBegin(2); showError(__LINE__);}
      catch (IndexOutOfBoundsException&) {}
    }

    // Test method : real_t getEnd(unsigned long i) const;
    {
      LabelSet l;
      l.addLabel(1.1, 2.2, "aaaa");
      l.addLabel(3.3, 4.4, "bbbb");
      try { test(l.getEnd(0) == 2.2, __LINE__); }
      catch (IndexOutOfBoundsException&) { showError(__LINE__); }
      test(l.getEnd(1) == 4.4, __LINE__);
      try { l.getEnd(2); showError(__LINE__);}
      catch (IndexOutOfBoundsException&) {}
    }

    // Test method : LabelSet(const LabelSet&);
    {
      LabelSet l;
      l.addLabel(1.1, 2.2, "aaaa");
      l.addLabel(3.3, 4.4, "bbbb");
      LabelSet l2(l);
      test(l2.size() == l.size(), __LINE__);
      test(l2.getName(0) == l.getName(0), __LINE__);
      test(l2.getName(1) == l.getName(1), __LINE__);
      test(l2.getBegin(0) == l.getBegin(0), __LINE__);
      test(l2.getBegin(1) == l.getBegin(1), __LINE__);
      test(l2.getEnd(0) == l.getEnd(0), __LINE__);
      test(l2.getEnd(1) == l.getEnd(1), __LINE__);
    }
    // Test method : const LabelSet& operator=(const LabelSet&);
    { 
      LabelSet l;
      l.addLabel(1.1, 2.2, "aaaa");
      l.addLabel(3.3, 4.4, "bbbb");
      LabelSet l2;
      l2 = l;
      test(l2.size() == l.size(), __LINE__);
      test(l2.getName(0) == l.getName(0), __LINE__);
      test(l2.getName(1) == l.getName(1), __LINE__);
      test(l2.getBegin(0) == l.getBegin(0), __LINE__);
      test(l2.getBegin(1) == l.getBegin(1), __LINE__);
      test(l2.getEnd(0) == l.getEnd(0), __LINE__);
      test(l2.getEnd(1) == l.getEnd(1), __LINE__);
    }
    // Test method : bool operator==(const LabelSet&) const
    //       and bool operator!=(const LabelSet&) const;
    {
      LabelSet l;
      LabelSet l2(l);
      test(l == l2, __LINE__);
      test(!(l != l2), __LINE__);

      l.addLabel(1.1, 2.2, "aaaa");
      test(l != l2, __LINE__);
      test(!(l == l2), __LINE__);

      l2.addLabel(1.1, 2.2, "aaaa");
      test(l == l2, __LINE__);
      test(!(l != l2), __LINE__);

      l.addLabel(1.1, 2.2, "aaaa");
      l.addLabel(1.1, 2.2, "bbbb");
      test(l != l2, __LINE__);
      test(!(l == l2), __LINE__);

      LabelSet l3;
      l3.addLabel(1.1, 2.2, "aaaa");
      LabelSet l4;
      l4.addLabel(8, 2.2, "aaaa");
      test(l3 != l4, __LINE__);
      test(!(l3 == l4), __LINE__);

      LabelSet l5;
      l5.addLabel(1.1, 2.2, "aaaa");
      LabelSet l6;
      l6.addLabel(1.1, 8, "aaaa");
      test(l5 != l6, __LINE__);
      test(!(l5 == l6), __LINE__);
    }

    // Test method : void clear();
    {
      LabelSet l;
      l.addLabel(1.1, 2.2, "aaaa");
      l.addLabel(3.3, 4.4, "bbbb");
      l.clear();
      test(l.size() == 0, __LINE__);
    }

    // test method : void load(const String&, const Config&);
    {
      char file[] = "./TestLabelFileReader.tmp";
      LabelSet set, set2;
      FILE* f = fopen(file, "w");
      char s[] = "1.1 3.3 l1\n 2.2 4.4 Label2\n \n 5 6E10 xxx";
      fwrite(&s, sizeof(s)-1, 1, f);
      fclose(f);
      Config c;
      set.load(file, c);
      set2.addLabel(1.1, 3.3, "l1");
      set2.addLabel(2.2, 4.4, "Label2");
      set2.addLabel(5, 6E10, "xxx");
      test(set == set2, __LINE__);
      remove(file);
    }

    // test method : LabelSet(const String&, const Config&);
    {
      char file[] = "./TestLabelFileReader.tmp";
      FILE* f = fopen(file, "w");
      char s[] = "1.1 3.3 l1\n 2.2 4.4 Label2\n \n 5 6E10 xxx";
      fwrite(&s, sizeof(s)-1, 1, f);
      fclose(f);
      Config c;
      LabelSet set(file, c), set2;
      set2.addLabel(1.1, 3.3, "l1");
      set2.addLabel(2.2, 4.4, "Label2");
      set2.addLabel(5, 6E10, "xxx");
      test(set == set2, __LINE__);
      remove(file);
    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestLabelSet::getClassName() const { return "TestLabelSet"; }
//-------------------------------------------------------------------------
TestLabelSet::~TestLabelSet() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestLabelSet_cpp)
