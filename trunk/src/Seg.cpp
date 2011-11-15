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

#if !defined(ALIZE_Seg_cpp)
#define ALIZE_Seg_cpp

#include "Seg.h"
#include "Exception.h"
#include <new>

using namespace alize;

//-------------------------------------------------------------------------
Seg::Seg(SegServer& ss, unsigned long b, unsigned long l, unsigned long lc,
     const String& s, const String& sn)  //private method
:SegAbstract(ss, lc, s, sn), _begin(b), _length(l) {}
//-------------------------------------------------------------------------
Seg& Seg::create(const K&, SegServer& ss, unsigned long b, unsigned long l,
    unsigned long lc, const String& s, const String& sn) // static
{
  Seg* p = new (std::nothrow) Seg(ss, b, l, lc, s, sn);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
Seg& Seg::duplicate(const K&, SegServer& ss) const
{
  Seg& s = create(K::k, ss, _begin, _length, _labelCode, _string, _srcName);
  s.list() = _list;
  return s;
}
//-------------------------------------------------------------------------
void Seg::setBegin(unsigned long b) { _begin = b; }
//-------------------------------------------------------------------------
void Seg::setLength(unsigned long l) { _length = l; }
//-------------------------------------------------------------------------
Seg* Seg::getSeg() const
{
  if (_current == 0)
  {
      _current++;
      return const_cast<Seg*>(this);
  }
  return NULL;
}
//-------------------------------------------------------------------------
void Seg::getExtremeBoundaries(const K&, unsigned long& b,
                  unsigned long& e, bool& isDefined) const
{
  if (!isDefined)
  {
    b = _begin;
    e = _begin+_length;
    isDefined = true;
  }
  else
  {
    if (_begin < b)
      b = _begin;
    if (_begin+_length > e)
      e = _begin+_length;
  }
}
//-------------------------------------------------------------------------
unsigned long Seg::length() const{ return _length; }
//-------------------------------------------------------------------------
unsigned long Seg::begin() const { return _begin; }
//-------------------------------------------------------------------------
Seg& Seg::merge(Seg& s)
{
  unsigned long begin = _begin;
  unsigned long end = _begin+_length;
  if (s._begin < _begin)
    begin = s._begin;
  if (s._begin + s._length > end)
    end = s._begin + s._length;
  _begin = begin;
  _length = end-begin;
  if (_string != s._string)
    _string += " " + s._string;
  if (_srcName != s._srcName)
    _srcName += " " + s._srcName;
  // TODO : what to do with s._labelCode ?
  const XList& l = s.list();
  for (unsigned long i=0; i<l.getLineCount(); i++)
  { _list.addLine() = l.getLine(i); }
  getServer().remove(s);
  return *this;
}
//-------------------------------------------------------------------------
Seg& Seg::duplicate() const {return getServer().duplicateSeg(*this);}
//-------------------------------------------------------------------------
Seg& Seg::split(unsigned long i)
{
  if (i >= _begin+_length)
    throw Exception("Cannot split a segment(begin="
       + String::valueOf(_begin) + " length="
       + String::valueOf(_length) + ") from feature #"
       + String::valueOf(i), __FILE__, __LINE__);
  Seg& newSeg = duplicate();
  newSeg.setBegin(i);
  newSeg.setLength(_length-i+_begin);
  _length = i-_begin;
  return newSeg;
}
//-------------------------------------------------------------------------
String Seg::getClassName() const { return "Seg"; }
//-------------------------------------------------------------------------
String Seg::toString() const
{
  return Object::toString()
    + "\n  begin = " + String::valueOf(_begin)
    + "\n  length = " + String::valueOf(_length)
    + "\n  labelCode = " + String::valueOf(_labelCode)
    + "\n  string = '" + _string + "'"
    + "\n  sourceName = '" + _srcName + "'";
  // TODO : ajouter l'affichage de _list
}
//-------------------------------------------------------------------------
Seg::~Seg() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_Seg_cpp)
