/*
	This file is part of ALIZE which is an open-source tool for 
	speaker recognition.

    ALIZE is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as 
    published by the Free Software Foundation, either version 3 of 
    the License, or any later version.

    ALIZE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public 
    License along with ALIZE.
    If not, see <http://www.gnu.org/licenses/>.
        
	ALIZE is a development project initiated by the ELISA consortium
	[alize.univ-avignon.fr/] and funded by the French Research 
	Ministry in the framework of the TECHNOLANGUE program 
	[www.technolangue.net]

	The ALIZE project team wants to highlight the limits of voice
	authentication in a forensic context.
	The "Person  Authentification by Voice: A Need of Caution" paper 
	proposes a good overview of this point (cf. "Person  
	Authentification by Voice: A Need of Caution", Bonastre J.F., 
	Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., Magrin-
	chagnolleau I., Eurospeech 2003, Genova].
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
	the use of ALIZE

	Copyright (C) 2003-2010
	Laboratoire d'informatique d'Avignon [lia.univ-avignon.fr]
	ALIZE admin [alize@univ-avignon.fr]
	Jean-Francois Bonastre [jean-francois.bonastre@univ-avignon.fr]
*/

#if !defined(ALIZE_ConfigChecker_cpp)
#define ALIZE_ConfigChecker_cpp

#include "ConfigChecker.h"
#include "Config.h"
#include "Exception.h"
#include <new>

using namespace alize;
typedef ConfigChecker CC;

//-------------------------------------------------------------------------
CC::ConfigChecker()
:Object() {}
//-------------------------------------------------------------------------
void CC::check(const Config& config)
{
  unsigned long i;
  for (i=0; i<_vect.size(); i++)
  {
    const Param& c = _vect.getObject(i);
    if (!config.existsParam(c.name))
    {
      if (c.mandatory)
        err("Parameter '" + c.name + "' mandatory in the config");
    }
    else
    {
      const String& content = config.getParam(c.name);
      if (c.type != PARAMTYPE_BOOLEAN && c.argIsRequired
          && content.getToken(0) == "")
        err("Parameter '" + c.name + "' argument required");

      if (c.type == PARAMTYPE_INTEGER)
      {
        try { content.toLong(); }
        catch (Exception&)
        {
          err("Value '" + content + "' of parameter '"
               + c.name + "' is not an integer");
        }
      }
      else if (c.type == PARAMTYPE_FLOAT)
      {
        try { content.toDouble(); }
        catch (Exception&)
        {
          err("Value '" + content + "' of parameter '"
               + c.name + "' is not a float");
        }
      }
      else if (c.type == PARAMTYPE_BOOLEAN)
      {
        try
        {
          String l = content;
          if (l != "")
            l.toBool();
        }
        catch (Exception&)
        {
          err("Value '" + content + "' of parameter '"
               + c.name + "' is not '' nor 'true' nor 'false'");
        }
      }
      else if (c.type == PARAMTYPE_STRING)
      {
        // no test
      }
    }
  }
}
//-------------------------------------------------------------------------
void CC::err(const String& e) const
{ throw ConfigCheckException(e, __FILE__, __LINE__); }
//-------------------------------------------------------------------------
void CC::checkName(const String& name) const
{
  if (name.getToken(0) == "")
    throw Exception("parameter name cannot be empty", __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
void CC::addIntegerParam(const String& name, bool mandatory,
                         bool argIsRequired, const String& desc)
{
  checkName(name);
  Param* p = new (std::nothrow) Param();
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  p->name = name;
  p->type = PARAMTYPE_INTEGER;
  p->mandatory = mandatory;
  p->argIsRequired = argIsRequired;
  p->description = desc;
  _vect.addObject(*p);
}
//-------------------------------------------------------------------------
void CC::addFloatParam(const String& name, bool mandatory,
                       bool argIsRequired, const String& desc)
{
  checkName(name);
  Param* p = new (std::nothrow) Param();
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  p->name = name;
  p->type = PARAMTYPE_FLOAT;
  p->mandatory = mandatory;
  p->argIsRequired = argIsRequired;
  p->description = desc;
  _vect.addObject(*p);
}
//-------------------------------------------------------------------------
void CC::addBooleanParam(const String& name, bool mandatory,
                         bool argIsRequired, const String& desc)
{
  checkName(name);
  Param* p = new (std::nothrow) Param();
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  p->name = name;
  p->type = PARAMTYPE_BOOLEAN;
  p->mandatory = mandatory;
  p->argIsRequired = argIsRequired;
  p->description = desc;
  _vect.addObject(*p);
}
//-------------------------------------------------------------------------
void CC::addStringParam(const String& name, bool mandatory,
                        bool argIsRequired, const String& desc)
{
  checkName(name);
  Param* p = new (std::nothrow) Param();
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  p->name = name;
  p->type = PARAMTYPE_STRING;
  p->mandatory = mandatory;
  p->argIsRequired = argIsRequired;
  p->description = desc;
  _vect.addObject(*p);
}
//-------------------------------------------------------------------------
String CC::getParamList()
{
  unsigned long i;
  unsigned long length = 9;
  for (i=0; i<_vect.size(); i++)
  {
    unsigned long l = 2+_vect.getObject(i).name.length();
    if (l > length)
      length = l;
  }
  String s, n;
  n = "--help";
  while (n.length() < length) n += " ";
  s += n + " Show this help\n";
  n = "--version";
  while (n.length() < length) n += " ";
  s += n + " Show version information\n";

  for (i=0; i<_vect.size(); i++)
  {
    const Param& c = _vect.getObject(i);
    n = "--" + c.name;
    while (n.length() < length) n += " ";
    s += n + " "
//      +  String(c.mandatory?"(mandatory)":"(optional)") + " "
      +  c.description + " "
      +  "<" + getParamTypeName(c.type) + ">"
      +  "\n";
  }
  return s;
}
//-------------------------------------------------------------------------
//unsigned long CC::getCount() const { return _vect.size(); }
//-------------------------------------------------------------------------
String CC::getClassName() const { return "ConfigChecker";}
//-------------------------------------------------------------------------
CC::~ConfigChecker() { _vect.deleteAllObjects(); }
//-------------------------------------------------------------------------
String CC::Param::getClassName() const { return "Param"; }
//-------------------------------------------------------------------------
#endif // !defined(ALIZE_ConfigChecker_cpp)
