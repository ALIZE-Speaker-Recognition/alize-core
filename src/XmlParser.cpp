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

#if !defined(ALIZE_XmlParser_cpp)
#define ALIZE_XmlParser_cpp

#include "XmlParser.h"
#include "MixtureGD.h"
#include "MixtureGF.h"

// see http://babel.alis.com/web_ml/xml/REC-xml.fr.html#NT-XMLDecl

using namespace alize;

//-------------------------------------------------------------------------
XmlParser::XmlParser()
:Object() {}
//-------------------------------------------------------------------------
void XmlParser::parse()
{
  // lecture 1er et seul element
  test(readNextChar() == "<", ": first character must be '<'");
  parseElement("", readOneChar());
}
//-------------------------------------------------------------------------
void XmlParser::parseElement(String path, String s)
{
  String tag, value;

  // read the opening tag
  test(s != ">" && s != "<" && s != "\"" && !isASeparator(s), "");
  while (s != "/" && s != ">" && !isASeparator(s))
  {
    tag += s;
    s = readOneChar();
  }
  eventOpeningElement(path += "<" + tag + ">");

  if (isASeparator(s))
    s = readNextChar();

  // read attributes

  while ( s != "/" && s != ">")
  {
    parseAttribute(path, s);
    s = readNextChar();
  }

  // fin element simple

  if (s == "/")
  {
    test(readOneChar() == ">", ": character '>' expected after '/'");
    eventClosingElement(path, value);
    return; // fin element simple
  }

  // element compose

  while (true)
  {
    while ( (s = readOneChar()) != "<")
    {
      if (s != "\r" && s != "\t" && s != "\n")
        value += s;
    }
    s = readOneChar();

    // closing tag

    if (s == "/")
    {
      s  = readOneChar();
      test(s != ">", ": a tag cannot be empty");
      String closingElement; // lecture balise de fermeture
      while (s != ">")
      {
        test(s != "/" && s != "\"" && s != "<" && !isASeparator(s),
          ": the tag contains an invalid character");
        closingElement += s;
        s  = readOneChar();
      }
      test(tag == closingElement, " : End tag <" + closingElement +
         "> does not match the start tag <" + tag  + ">");

      eventClosingElement(path, value);
      return; // fin element compose
    }
    parseElement(path, s);
  }
}
//-------------------------------------------------------------------------
void XmlParser::parseAttribute(String path, String s)
{
  String attribute, value;
  test(s != "\"" && s != "<" && s != "=", "");
  while (s != "=" && !isASeparator(s))
  {
    attribute += s;
    s = readOneChar();
    test(s != "/" && s != ">" && s != "<" && s != "\"" && s != "'",
              ": an attribute contain an invalid character");
  }
  eventOpeningElement(path += "<" + attribute + ">");
  if (isASeparator(s))
    test(readNextChar() == "=",
       ": Missing equals sign between attribute and attribute value");
  String quote = readNextChar();
  test(quote == "\"" || quote == "'", String(": a string literal was")
          + "expected, but no opening quote character was found");
  while ( (s = readOneChar()) != quote)
    value += s;
  eventClosingElement(path, value);
}
//-------------------------------------------------------------------------
// Return the next character of the file that is not a separator character
//-------------------------------------------------------------------------
const String& XmlParser::readNextChar()
{
  while(true) 
  {
    const String& s = readOneChar();
    if (!isASeparator(s))
      return s;
  }
  return readNextChar(); // never used
}
//-------------------------------------------------------------------------
bool XmlParser::isASeparator(String s) const
{ return s == " " || s == "\n" || s == "\t" ||s == "\r"; }
//-------------------------------------------------------------------------
void XmlParser::test(bool v, const String& msg) { if (!v) eventError(msg); }
//-------------------------------------------------------------------------
XmlParser::~XmlParser() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_XmlParser_cpp)

