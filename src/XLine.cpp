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

#if !defined(ALIZE_XLine_cpp)
#define ALIZE_XLine_cpp

#include <new>
#include "XLine.h"
#include "Exception.h"

using namespace alize;

//-------------------------------------------------------------------------
XLine::XLine()
:Object(), _current(0), _pLine(NULL) {}
//-------------------------------------------------------------------------
XLine& XLine::create()
{
  XLine* p = new (std::nothrow) XLine();
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
/* Modified 19/07/07 - richard.dufour@lium.univ-lemans.fr
 */
XLine& XLine::create(String& key, String& value)
{
  XLine& l = XLine::create();
  l.addElement(key);
  l.addElement(value);
  l.rewind();
  return l;
}
//-------------------------------------------------------------------------
XLine::XLine(const XLine& l)
:Object(), _current(0), _pLine(NULL)
{
  for (unsigned long i=0; i<l._vector.size(); i++)
    _vector.addObject(l._vector.getObject(i).duplicate());
}
//-------------------------------------------------------------------------
XLine& XLine::duplicate() const
{
  XLine* p = new (std::nothrow) XLine(*this);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
const XLine& XLine::operator=(const XLine& l)
{
  if (this != &l)
  {
    reset();
    for (unsigned long i=0; i<l._vector.size(); i++)
      addElement(l._vector.getObject(i)); 
  }
  _current = 0;
  return *this;
}  
//-------------------------------------------------------------------------
bool XLine::operator==(const XLine& l) const
{
  if (_vector.size() != l._vector.size())
    return false;
  for (unsigned long i=0; i<l._vector.size(); i++)
    if (_vector.getObject(i) != l._vector.getObject(i))
      return false;
  return true;
}  
//-------------------------------------------------------------------------
bool XLine::operator!=(const XLine& l) const { return !(*this==l); }
//-------------------------------------------------------------------------
void XLine::deleteElement(const String& e)
{ delete &_vector.removeObject(e);}
//-------------------------------------------------------------------------
void XLine::rewind() const { _current = 0; }
//-------------------------------------------------------------------------
String& XLine::getElement(const unsigned long i,
                          const bool becomeCurrent) const
{
  String& s = _vector.getObject(i);
  // getObject(i) can throw IndexOutOfBoundsException
  if (becomeCurrent)
    _current = i;
  return s;
}
//-------------------------------------------------------------------------
long XLine::getIndex(const String& e) const
{
  unsigned long size = _vector.size();
  for (unsigned long i=0; i<size; i++)
    if (_vector.getObject(i) == e)
      return (long)i;
  return -1;
}
//-------------------------------------------------------------------------
String* XLine::getElement() const
{
  if (_current >= _vector.size())
    return NULL;
  _current++;
  return &_vector.getObject(_current-1);
}
//-------------------------------------------------------------------------
const XLine& XLine::getElements() const
{
  if (_pLine == NULL)
    _pLine = &XLine::create();
  else
    _pLine->reset();
  for (unsigned long i=_current; i<_vector.size(); i++)
    _pLine->_vector.addObject(_vector.getObject(i).duplicate());
  return *_pLine;
}
//-------------------------------------------------------------------------
XLine& XLine::addElement(const String& e) 
{
  _current = _vector.addObject(e.duplicate());
  return *this;
}
//-------------------------------------------------------------------------
void XLine::reset()
{
  _vector.deleteAllObjects();
  _current = 0;
}
//-------------------------------------------------------------------------
unsigned long XLine::getElementCount() const { return _vector.size(); }
//-------------------------------------------------------------------------
String XLine::getClassName() const { return "XLine"; }
//-------------------------------------------------------------------------
String XLine::toString() const
{
  String s;
  for (unsigned long i=0; i<_vector.size(); i++)
    s += " " + _vector.getObject(i); 
  return Object::toString() + s;
}
//-------------------------------------------------------------------------
XLine::~XLine()
{
  _vector.deleteAllObjects();
  if (_pLine != NULL)
    delete _pLine;
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_XLine_cpp)

