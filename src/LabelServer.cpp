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

#if !defined(ALIZE_LabelServer_cpp)
#define ALIZE_LabelServer_cpp

#include "Object.h"
#include "LabelServer.h"
#include "RefVector.h"
#include "alizeString.h"
#include "Label.h"
#include "Exception.h"

using namespace alize;

//-------------------------------------------------------------------------
LabelServer::LabelServer(bool usePredefinedLabels)
:Object(), _first(0), _lastAdded(0)
{
  if (usePredefinedLabels)
  {
    addLabel(Label(""));
    addLabel(Label("SPEAKING"));
    addLabel(Label("MUSIC"));
    addLabel(Label("MUTE"));
  }
  _first = size();
}
//-------------------------------------------------------------------------
unsigned long LabelServer::addLabel(const Label& l, bool forceAdd)
{
  if (size() != 0 && !forceAdd)
  {
    // if l is identical to the last label added
    if (l == getLabel(_lastAdded)) // operator!= overloaded
      return _lastAdded;
    // search for an identical label
    unsigned long s = size();
    for (unsigned long i=0; i<s; i++)
    {
      if (getLabel(i) == l) // if an identical label exists
      {
        _lastAdded = i;
        return _lastAdded;
      }
    }
  }
  // adds a new label
  _vect.addObject(l.duplicate());
  _lastAdded = size()-1;
  return _lastAdded;
}
//-------------------------------------------------------------------------
void LabelServer::setLabel(const Label& l, unsigned long i) const
{
  delete &_vect.getObject(i); // can throw IndexOutOfBoundsException
  _vect.setObject(l.duplicate(), i);
}
//-------------------------------------------------------------------------
Label& LabelServer::getLabel(unsigned long index) const
{
  return _vect.getObject(index);
  // can throw IndexOutOfBoundsException
}
//-------------------------------------------------------------------------
long LabelServer::getLabelIndexByString(const String& s) const
{
  unsigned long size = _vect.size();
  for (unsigned long i=0; i<size; i++)
    if (_vect.getObject(i).getString() == s)
      return (long)i;
  return -1;
}
//-------------------------------------------------------------------------
void LabelServer::clear(bool deletePreDefined)
{
  if (deletePreDefined)
    _first = 0;
  _vect.deleteAllObjects(_first);
}
//-------------------------------------------------------------------------
unsigned long LabelServer::size() const { return _vect.size(); }
//-------------------------------------------------------------------------
String LabelServer::getClassName() const { return "LabelServer"; }
//-------------------------------------------------------------------------
LabelServer::~LabelServer() { clear(true); }
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_LabelServer_cpp)
