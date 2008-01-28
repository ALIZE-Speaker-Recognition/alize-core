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

#if !defined(ALIZE_TestLabel_cpp)
#define ALIZE_TestLabel_cpp

#include "TestLabel.h"
#include "Label.h"
#include "AutoDestructor.h"

using namespace alize;

//-------------------------------------------------------------------------
TestLabel::TestLabel()
:Test() {}
//-------------------------------------------------------------------------
bool TestLabel::exec()
{
   Test::exec();

    {
        // test String& getClassName() const
    
        test(Label().getClassName() == "Label", __LINE__);

        // test method : Label(const String& = "")

        test(Label()._string == "", __LINE__);
        test(Label()._srcName == "", __LINE__);
        test(Label("")._string == "", __LINE__);
        test(Label("")._srcName == "", __LINE__);
        test(Label("abc")._string == "abc", __LINE__);
        test(Label("abc")._srcName == "", __LINE__);

        // test method : static Label& create(const String& = "")
        {
            Label& c1 = Label::create();
            Label& c2 = Label::create("");
            Label& c3 = Label::create("abc");
            AutoDestructor a1(c1);
            AutoDestructor a2(c2);
            AutoDestructor a3(c3);
            test(c1._string == "", __LINE__);
            test(c1._srcName == "", __LINE__);
            test(c2._string == "", __LINE__);
            test(c2._srcName == "", __LINE__);
            test(c3._string == "abc", __LINE__);
            test(c3._srcName == "", __LINE__);
        }
        
        // test method : Label(const Label&)
        {
            Label c1, c2("abc");
            c1._srcName = "111";
            c2._srcName = "222";
            test(Label(c1)._string == "", __LINE__);
            test(Label(c1)._srcName == "111", __LINE__);
            test(Label(c2)._string == "abc", __LINE__);
            test(Label(c2)._srcName == "222", __LINE__);
        }

        // test method : virtual Label& duplicate() const;
        {
            Label a1, a2("abc");
            a1._srcName = "111";
            a2._srcName = "222";
            Label& c1 = a1.duplicate();
            Label& c2 = a2.duplicate();
            AutoDestructor x1(c1);
            AutoDestructor x2(c2);
            test(c1._string == "", __LINE__);
            test(c1._srcName == "111", __LINE__);
            test(c2._string == "abc", __LINE__);
            test(c2._srcName == "222", __LINE__);
        }

        // test method : const Label& operator=(const Label&)
        {
            Label c0("123"), c1, c2("abc"), c3;
            c0._srcName = "000";
            c1._srcName = "111";
            c2._srcName = "222";
            c3._srcName = "333";
            c3 = c1 = c0;
            c2 = Label("");
            test(c1._string == "123", __LINE__);
            test(c1._srcName == "000", __LINE__);
            test(c2._string == "", __LINE__);
            test(c2._srcName == "", __LINE__);
            test(c3._string == "123", __LINE__);
            test(c3._srcName == "000", __LINE__);
        }

        // test method : bool operator==(const Label&) const
        {
            Label c0, c1;
            test(c0 == c0, __LINE__);
            test(c0 == c1, __LINE__);
            c1._string = "vvv";
            test(!(c0 == c1), __LINE__);
            c1._string = "";
            c1._srcName = "vvv";
            test(!(c0 == c1), __LINE__);
        }

        // test method : bool operator!=(const Label&) const
        {
            Label c0, c1;
            test(!(c0 != c0), __LINE__);
            test(!(c0 != c1), __LINE__);
            c1._string = "vvv";
            test(c0 != c1, __LINE__);
            c1._string = "";
            c1._srcName = "vvv";
            test(c0 != c1, __LINE__);
        }

        // test method : void reset()
        {
            Label c1, c2("123");
            c2._srcName = "1235415";
            c1.reset();
            c2.reset();
            test(c1._string == "", __LINE__);
            test(c2._string == "", __LINE__);
            test(c2._srcName == "", __LINE__);
        }

        // test method : void setString(const String&)
        {
            Label c1("121");
            c1.setString("444");
            test(c1._string == "444", __LINE__);
            c1.setString("");
            test(c1._string == "", __LINE__);
        }
   
        // test method : const String& getString() const
        
        test(Label("123").getString() == "123", __LINE__);

        // test method : void setSourceName(const String&)
        {
            Label c1;
            c1.setSourceName("444");
            test(c1._srcName == "444", __LINE__);
            c1.setSourceName("");
            test(c1._srcName == "", __LINE__);
        }
   
        // test method : const String& getSourceName() const
        
        {
            Label c1;
            c1.setSourceName("456");
            test(c1.getSourceName() == "456", __LINE__);
        }

    } // local objects are deleted here

    return endTest();
}
//-------------------------------------------------------------------------
String TestLabel::getClassName() const
{
    return "TestLabel";
}
//-------------------------------------------------------------------------
TestLabel::~TestLabel() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestLabel_cpp)
