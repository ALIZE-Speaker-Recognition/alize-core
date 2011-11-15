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

#if !defined(ALIZE_LabelSet_cpp)
#define ALIZE_LabelSet_cpp

#include "LabelSet.h"
#include "alizeString.h"
#include "Exception.h"
#include "LabelFileReader.h"
#include "Config.h"

using namespace alize;

//-------------------------------------------------------------------------
LabelSet::LabelSet()
:Object() {}
//-------------------------------------------------------------------------
LabelSet::LabelSet(const FileName& f, const Config& c)
:Object() { load(f, c); }
//-------------------------------------------------------------------------
LabelSet::LabelSet(const LabelSet& s)
:Object(), _beginVect(s._beginVect), _endVect(s._endVect)
{
  for (unsigned long i=0; i<s._nameVect.size(); i++)
  { _nameVect.addObject(s.getName(i).duplicate()); }
}
//-------------------------------------------------------------------------
const LabelSet& LabelSet::operator=(const LabelSet& s)
{
  _beginVect = s._beginVect;
  _endVect = s._endVect;
  for (unsigned long i=0; i<s._nameVect.size(); i++)
  { _nameVect.addObject(s.getName(i).duplicate()); }
  return *this;
}
//-------------------------------------------------------------------------
bool LabelSet::operator==(const LabelSet& s)
{
  if (_beginVect != s._beginVect || _endVect != s._endVect)
    return false;
  assert(_nameVect.size() == s._nameVect.size());
  for (unsigned long i=0; i<_nameVect.size(); i++)
  {
    if (getName(i) != s.getName(i))
      return false;
  }
  return true;
}
//-------------------------------------------------------------------------
bool LabelSet::operator!=(const LabelSet& s) { return !(*this==s); }
//-------------------------------------------------------------------------
void LabelSet::addLabel(real_t begin, real_t end, const String& label)
{
  _nameVect.addObject(label.duplicate());
  _beginVect.addValue(begin);
  _endVect.addValue(end);
}
//-------------------------------------------------------------------------
void LabelSet::load(const FileName& f, const Config& c)
{
  LabelFileReader(f, c).readLabels(*this);
  // can throw FileNotFoundException, IOException
}
//-------------------------------------------------------------------------
const String& LabelSet::getName(unsigned long i) const
{
  return _nameVect.getObject(i);
  // can throw IndexOutOfBoundsException
}
//-------------------------------------------------------------------------
real_t LabelSet::getBegin(unsigned long i) const
{ return _beginVect[i]; /* can throw IndexOutOfBoundsException */ }
//-------------------------------------------------------------------------
real_t LabelSet::getEnd(unsigned long i) const
{ return _endVect[i]; /* can throw IndexOutOfBoundsException */ }
//-------------------------------------------------------------------------
String LabelSet::getClassName() const { return "LabelSet"; }
//-------------------------------------------------------------------------
unsigned long LabelSet::size() const { return _nameVect.size(); }
//-------------------------------------------------------------------------
String LabelSet::toString() const
{
  String s = Object::toString() + "\n  Nb  = " + String::valueOf(size());
  for (unsigned long i=0; i< size(); i++)
  {
    s += "\n  begin: " + String::valueOf(_beginVect[i])
      +  " end: " + String::valueOf(_endVect[i])
      +  " label: '" + getName(i);
  }
  return s;
}
//-------------------------------------------------------------------------
void LabelSet::clear()
{
  _nameVect.deleteAllObjects();
  _beginVect.clear();
  _endVect.clear();
}
//-------------------------------------------------------------------------
LabelSet::~LabelSet() { _nameVect.deleteAllObjects(); }
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_LabelSet_cpp)

