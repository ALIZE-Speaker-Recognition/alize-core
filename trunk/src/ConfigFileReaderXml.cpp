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

#if !defined(ALIZE_ConfigFileReaderXml_cpp)
#define ALIZE_ConfigFileReaderXml_cpp

#include "ConfigFileReaderXml.h"
#include "Exception.h"
#include "Config.h"
#include "FileReader.h"

// see http://babel.alis.com/web_ml/xml/REC-xml.fr.html#NT-XMLDecl

using namespace alize;

//-------------------------------------------------------------------------
ConfigFileReaderXml::ConfigFileReaderXml(const FileName& f)
:ConfigFileReaderAbstract(f), XmlParser() {}
//-------------------------------------------------------------------------
void ConfigFileReaderXml::readConfig(Config& c)
{
  _pConfig = &c;
  _pConfig->reset();
  _line = 1;
  parse();
  assert(_pReader != NULL);
  _pReader->close();
}
//-------------------------------------------------------------------------
void ConfigFileReaderXml::eventOpeningElement(const String& path)
{
  if (path.endsWith("<config>"))
  {
  }
  else if (path.endsWith("<param>"))
  {
    _paramNameDefined = false;
  }
  else if (path.endsWith("<param><name>"))
  {
  }
  else if (path.endsWith("<version>"))
  {
  }
  else
    eventError("Unknown tag in the path " + path);
}
//-------------------------------------------------------------------------
void ConfigFileReaderXml::eventClosingElement(const String& path,
                           const String& value)
{
  if (path.endsWith("<config>"))
  {
  }
  else if (path.endsWith("<param>"))
  {
    if (_paramNameDefined)
    {
      _pConfig->setParam(_paramName, value);
    }
    else
      if (value != "")
        eventError("parameter name missing");
  }
  else if (path.endsWith("<param><name>"))
  {
    if (value.getToken(0) != "")
    {
      _paramName = value;
      _paramNameDefined = true;
    }
    else
      eventError("parameter name is required");
  }
  else if (path.endsWith("<version>"))
  {
    if (value != "1")
      eventError("invalid version");
  }
  // TODO : more serious checking
}
//-------------------------------------------------------------------------
void ConfigFileReaderXml::eventError(const String& msg)
{
  assert(_pReader != NULL);
  _pReader->close();
  throw InvalidDataException("Error line " + String::valueOf(_line)
    + " : " + msg, __FILE__, __LINE__, _pReader->getFullFileName());
}
//-------------------------------------------------------------------------
const String& ConfigFileReaderXml::readOneChar()
{
  assert(_pReader != NULL);
  const String& s = _pReader->readString(1);
  if (s == "\n")
    _line++;
  return s;
}
//-------------------------------------------------------------------------
String ConfigFileReaderXml::getClassName() const
{ return "ConfigFileReaderXml"; }
//-------------------------------------------------------------------------
ConfigFileReaderXml::~ConfigFileReaderXml() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_ConfigFileReaderXml_cpp)

