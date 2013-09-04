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

#if !defined(ALIZE_XList_cpp)
#define ALIZE_XList_cpp

#include <new>
#include "XList.h"
#include "Exception.h"
#include "XListFileReader.h"
#include "Config.h"
#include <fstream>
#include <LKVector.h>

using namespace alize;

//-------------------------------------------------------------------------
XList::XList()
:Object(), _current(0) {}
//-------------------------------------------------------------------------
XList::XList(const FileName& f)
:Object(), _current(0) { load(f, Config()); }
//-------------------------------------------------------------------------
XList::XList(const FileName& f, const Config& c)
:Object(), _current(0) { load(f, c); }
//-------------------------------------------------------------------------
XList& XList::create() // static method
{
  XList* p = new (std::nothrow) XList();
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
XList::XList(const XList& l)
:Object(), _current(0)
{
  for (unsigned long i=0; i<l._vector.size(); i++)
    _vector.addObject(l._vector.getObject(i).duplicate());
}
//-------------------------------------------------------------------------
const XList& XList::operator=(const XList& l)
{
  if (this != &l)
  {
    _vector.deleteAllObjects();
    for (unsigned long i=0; i<l._vector.size(); i++)
      _vector.addObject(l._vector.getObject(i).duplicate());
    _current = 0;
  }
  return *this;
}  
//-------------------------------------------------------------------------
bool XList::operator==(const XList& l) const
{
  if (_vector.size() != l._vector.size())
    return false;
  for (unsigned long i=0; i<l._vector.size(); i++)
    if (_vector.getObject(i) != l._vector.getObject(i))
      return false;
  return true;
}  
//-------------------------------------------------------------------------
bool XList::operator!=(const XList& l) const { return !(*this==l); }
//-------------------------------------------------------------------------
void XList::load(const FileName& f, const Config& c)
{
  XListFileReader(f, c).readList(*this);
  // can throw FileNotFoundException, IOException
}
//-------------------------------------------------------------------------
void XList::save(const FileName& f) const { save(f, Config()); }
//-------------------------------------------------------------------------
void XList::save(const FileName& f, const Config& c) const
{
  std::ofstream ff(f.c_str(), std::ios::out);
  if (!ff)
    throw IOException("Cannot open file", __FILE__, __LINE__, f);
  for (unsigned long i=0; i<getLineCount(); i++)
  {
    XLine& line = _vector.getObject(i);
    unsigned long  count = line.getElementCount();
    for (unsigned long j=0; j<count; j++)
    {
      ff << line.getElement(j, false); // false = does not change current seg
      if (j+1<count)
        ff << " ";
    }
    ff << std::endl;
  }
}
//-------------------------------------------------------------------------
XLine& XList::addLine()
{
  XLine& l = XLine::create();
  _current = _vector.addObject(l);
  return l;
}
//-------------------------------------------------------------------------
/* Modified 19/07/07 - richard.dufour@lium.univ-lemans.fr
 */
XLine& XList::addLine(String& key, String& value)
{
  XLine& l = XLine::create(key,value);
  _current = _vector.addObject(l);
  return l;
}
//-------------------------------------------------------------------------
void XList::rewind() const { _current = 0; }
//-------------------------------------------------------------------------
XLine& XList::getLine(unsigned long i) const
{
  XLine& line = _vector.getObject(i);
  // getObject(i) can throw IndexOutOfBoundsException
  _current = i;
  line.rewind();
  return line;
}
//-------------------------------------------------------------------------
XLine* XList::getLine() const
{
  if (_current >= _vector.size())
    return NULL;
  _current++;
  XLine& line = _vector.getObject(_current-1);
  line.rewind();
  return &line;
}
//-------------------------------------------------------------------------
XLine* XList::findLine(const String& key, unsigned long idx) const
{
  for (unsigned long i=0; i<_vector.size(); i++)
  {
    XLine& line = _vector.getObject(i);
    if (idx < line.getElementCount() && line.getElement(idx) == key)
    {
      _current = i;
      return &line;
    }
  }
  return NULL;
}
//-------------------------------------------------------------------------
XLine& XList::getAllElements() const
{
  _line.reset();
  for (unsigned long i=0; i<_vector.size(); i++)
  {
    XLine& l = _vector.getObject(i);
    for (unsigned long j=0; j<l.getElementCount(); j++)
      _line.addElement(l.getElement(j, false));
  }
  return _line; 
}
//-------------------------------------------------------------------------
XLine& XList::getAllUniqueElements() const
{
  _line.reset();
  for (unsigned long i=0; i<_vector.size(); i++)
  {
    XLine& l = _vector.getObject(i);
    for (unsigned long j=0; j<l.getElementCount(); j++)
		if(_line.getIndex(l.getElement(j, false)) == -1)
			_line.addElement(l.getElement(j, false));
  }
  return _line; 
}
//-------------------------------------------------------------------------
void XList::sortByElementNumber(String order){

	// Get the number of sessions per speaker
	LKVector spk(0,0);
	for(unsigned long i=0;i<_vector.size();i++){
		LKVector::type sps;
		sps.idx = i;
		sps.lk = _vector.getObject(i).getElementCount();
		spk.addValue(sps);
	}
	// Sort Xlines of the temporary XList by element number
	spk.descendingSort();

	// Copy the current RefVector<XLine> into a temporary one
	RefVector<XLine> tmpX;
	for(unsigned long i=0;i<_vector.size();i++){
		XLine *ll = new XLine(_vector.getObject(i));
		tmpX.addObject(*ll);
	}

	// Remove all elements from the XList
	_vector.deleteAllObjects();

	// Fill the XList according to the number of elements
	if(order == "descend"){
		for(unsigned long i=0;i<tmpX.size();i++){
			_vector.addObject(tmpX.getObject(spk[i].idx));
		}
	}
	else if(order == "ascend"){
		for(long i=tmpX.size()-1;i>=0;i--){
			_vector.addObject(tmpX.getObject(spk[i].idx));
		}
	}
}
//-------------------------------------------------------------------------
void XList::reset()
{
  _vector.deleteAllObjects();
  _current = 0;
}
//-------------------------------------------------------------------------
unsigned long XList::getLineCount() const { return _vector.size(); }
//-------------------------------------------------------------------------
String XList::getClassName() const { return "XList"; }
//-------------------------------------------------------------------------
/* Modified 19/07/07 - richard.dufour@lium.univ-lemans.fr
 */
String XList::searchValue(String& index)
{
  String result = "";
  for (unsigned long i=0; i<_vector.size(); i++)
  {
    XLine& l = _vector.getObject(i);
    if (l.getElement(0) == index) {
      return l.getElement(1);
      break;
    }
  }
  return result;
}
//-------------------------------------------------------------------------
String XList::toString() const
{
  String s;
  for (unsigned long i=0; i<_vector.size(); i++)
  {
    s += "\n";
    XLine& l = _vector.getObject(i);
    for (unsigned long j=0; j<l.getElementCount(); j++)
    { s += " " + l.getElement(j); }
  }
  return Object::toString() + s;
}
//-------------------------------------------------------------------------
XList::~XList() { _vector.deleteAllObjects(); }
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_XList_cpp)

