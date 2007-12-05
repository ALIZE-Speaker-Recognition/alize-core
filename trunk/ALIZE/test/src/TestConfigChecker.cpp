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

#if !defined(ALIZE_TestConfigChecker_cpp)
#define ALIZE_TestConfigChecker_cpp

#include "TestConfigChecker.h"
#include "Config.h"
#include "ConfigChecker.h"
#include <iostream>

using namespace alize;
using namespace std;
#define MANDATORY true
#define OPTIONAL false
#define ARG_REQUIRED true
#define ARG_OPTIONAL false

//-------------------------------------------------------------------------
TestConfigChecker::TestConfigChecker()
:Test() {}
//-------------------------------------------------------------------------
bool TestConfigChecker::exec()
{
  typedef ConfigChecker CC;

  Test::exec();
  {
    // test method : String getClassName() const
    {
      test(CC().getClassName() == "ConfigChecker", __LINE__);
    }
    // test constructor
    {
      CC cc;
      test(cc._vect.size() == 0, __LINE__);
    }
    // test : void addIntegerParam(const String& name, bool mandatory)
    {
      CC cc;
      cc.addIntegerParam("name", OPTIONAL, ARG_REQUIRED, "description");
      test(cc._vect.size() == 1, __LINE__);
      CC::Param& param = cc._vect.getObject(0);
      test(param.name == "name", __LINE__);
      test(param.mandatory == OPTIONAL, __LINE__);
      test(param.type == PARAMTYPE_INTEGER, __LINE__);
      test(param.argIsRequired == ARG_REQUIRED, __LINE__);
      test(param.description == "description", __LINE__);

      cc.addIntegerParam("name2", MANDATORY, ARG_OPTIONAL, "dddd");
      test(cc._vect.size() == 2, __LINE__);
      CC::Param& param2 = cc._vect.getObject(1);
      test(param2.name == "name2", __LINE__);
      test(param2.mandatory == MANDATORY, __LINE__);
      test(param2.type == PARAMTYPE_INTEGER, __LINE__);
      test(param2.argIsRequired == ARG_OPTIONAL, __LINE__);
      test(param2.description == "dddd", __LINE__);
    }
    // test : void addFloatParam(const String& name, bool mandatory)
    {
      CC cc;
      cc.addFloatParam("name", OPTIONAL, ARG_REQUIRED, "description");
      test(cc._vect.size() == 1, __LINE__);
      CC::Param& param = cc._vect.getObject(0);
      test(param.name == "name", __LINE__);
      test(param.mandatory == OPTIONAL, __LINE__);
      test(param.argIsRequired == ARG_REQUIRED, __LINE__);
      test(param.type == PARAMTYPE_FLOAT, __LINE__);
      test(param.description == "description", __LINE__);

      cc.addFloatParam("name2", MANDATORY, ARG_OPTIONAL, "dddd");
      test(cc._vect.size() == 2, __LINE__);
      CC::Param& param2 = cc._vect.getObject(1);
      test(param2.name == "name2", __LINE__);
      test(param2.mandatory == MANDATORY, __LINE__);
      test(param2.type == PARAMTYPE_FLOAT, __LINE__);
      test(param2.argIsRequired == ARG_OPTIONAL, __LINE__);
      test(param2.description == "dddd", __LINE__);
    }
    // test : void addBooleanParam(const String& name, bool mandatory)
    {
      CC cc;
      cc.addBooleanParam("name", OPTIONAL, ARG_REQUIRED, "description");
      test(cc._vect.size() == 1, __LINE__);
      CC::Param& param = cc._vect.getObject(0);
      test(param.name == "name", __LINE__);
      test(param.mandatory == OPTIONAL, __LINE__);
      test(param.argIsRequired == ARG_REQUIRED, __LINE__);
      test(param.type == PARAMTYPE_BOOLEAN, __LINE__);
      test(param.description == "description", __LINE__);

      cc.addBooleanParam("name2", MANDATORY, ARG_OPTIONAL, "dddd");
      test(cc._vect.size() == 2, __LINE__);
      CC::Param& param2 = cc._vect.getObject(1);
      test(param2.name == "name2", __LINE__);
      test(param2.mandatory == MANDATORY, __LINE__);
      test(param2.type == PARAMTYPE_BOOLEAN, __LINE__);
      test(param2.argIsRequired == ARG_OPTIONAL, __LINE__);
      test(param2.description == "dddd", __LINE__);
    }
    // test : void addStringParam(const String& name, bool mandatory)
    {
      CC cc;
      cc.addStringParam("name", OPTIONAL, ARG_REQUIRED, "description");
      test(cc._vect.size() == 1, __LINE__);
      CC::Param& param = cc._vect.getObject(0);
      test(param.name == "name", __LINE__);
      test(param.mandatory == OPTIONAL, __LINE__);
      test(param.argIsRequired == ARG_REQUIRED, __LINE__);
      test(param.type == PARAMTYPE_STRING, __LINE__);
      test(param.description == "description", __LINE__);

      cc.addStringParam("name2", MANDATORY, ARG_OPTIONAL, "dddd");
      test(cc._vect.size() == 2, __LINE__);
      CC::Param& param2 = cc._vect.getObject(1);
      test(param2.name == "name2", __LINE__);
      test(param2.mandatory == MANDATORY, __LINE__);
      test(param2.type == PARAMTYPE_STRING, __LINE__);
      test(param2.argIsRequired == ARG_OPTIONAL, __LINE__);
      test(param2.description == "dddd", __LINE__);
    }
    // test : void check(const Config& cc)
    {
      Config c;
      CC cc;
      cc.addIntegerParam("int1", OPTIONAL, ARG_OPTIONAL);
      try { cc.check(c); }
      catch (ConfigCheckException&) { showError(__LINE__); }
      c.setParam("int1", "");
      try { cc.check(c); showError(__LINE__);}
      catch (ConfigCheckException&) {} 
    }
    {
      Config c;
      CC cc;
      cc.addIntegerParam("int1", MANDATORY, ARG_REQUIRED);
      try { cc.check(c); showError(__LINE__);}
      catch (ConfigCheckException&) {} 
      c.setParam("int1", "");
      try { cc.check(c); showError(__LINE__);}
      catch (ConfigCheckException&) {} 
    }
    {
      Config c;
      CC cc;
      cc.addIntegerParam("int1", MANDATORY, ARG_REQUIRED);
      try { cc.check(c); showError(__LINE__);}
      catch (ConfigCheckException&) {} 
      c.setParam("int1", "xxx"); // not an integer
      try { cc.check(c); showError(__LINE__);}
      catch (ConfigCheckException&) {} 
      c.setParam("int1", "123"); // an integer
      try { cc.check(c); }
      catch (ConfigCheckException&) { showError(__LINE__); }

      cc.addFloatParam("float1", MANDATORY, ARG_REQUIRED);
      try { cc.check(c); showError(__LINE__);}
      catch (ConfigCheckException&) {} 
      c.setParam("float1", "xxx"); // not a float
      try { cc.check(c); showError(__LINE__);}
      catch (ConfigCheckException&) {} 
      c.setParam("float1", "123.4"); // a float
      try { cc.check(c); }
      catch (ConfigCheckException&) { showError(__LINE__); }

      cc.addBooleanParam("bool1", MANDATORY, ARG_REQUIRED);
      try { cc.check(c); showError(__LINE__);}
      catch (ConfigCheckException&) {} 
      c.setParam("bool1", "xxx"); // not a bool
      try { cc.check(c); showError(__LINE__);}
      catch (ConfigCheckException&) {} 
      c.setParam("bool1", "true"); // a bool
      try { cc.check(c); }
      catch (ConfigCheckException&) { showError(__LINE__); }
      c.setParam("bool1", ""); // a bool ("" = "true")
      try { cc.check(c); }
      catch (ConfigCheckException&) { showError(__LINE__); }



    }

  } // local objects are deleted here

  return endTest();
}
//-------------------------------------------------------------------------
String TestConfigChecker::getClassName() const { return "TestConfigChecker"; }
//-------------------------------------------------------------------------
TestConfigChecker::~TestConfigChecker() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestConfigChecker_cpp)

