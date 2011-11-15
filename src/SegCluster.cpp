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

#if !defined(ALIZE_SegCluster_cpp)
#define ALIZE_SegCluster_cpp

#include "SegCluster.h"
#include "SegServer.h"
#include "Exception.h"
#include <new>
#include "limits.h"
#include <iostream>

using namespace alize;
using namespace std;
typedef SegCluster C;
//-------------------------------------------------------------------------
C::SegCluster(SegServer& ss, unsigned long lc, const String& s,
                                                          const String& sn)
:SegAbstract(ss, lc, s, sn), _pCurrentSeg(NULL) { rewind(); }
//-------------------------------------------------------------------------
SegCluster& C::create(const K&, SegServer& ss, unsigned long lc,
                      const String& s, const String& sn)
{
  SegCluster* p = new (std::nothrow) SegCluster(ss, lc, s, sn);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
/*C::SegCluster(const SegCluster& s)
:SegAbstract(*s._pServer, s._begin, s._end, s._labelCode, s._string,
 s._srcName) {}*/
//-------------------------------------------------------------------------
SegCluster& C::duplicate(const K&, SegServer& ss) const
{
  SegCluster& cl = create(K::k, ss, labelCode(), string(), sourceName());
  cl.list() = _list;
  return cl;
}
//-------------------------------------------------------------------------
unsigned long C::getId() const { return _id; }
//-------------------------------------------------------------------------
void C::setId(const K&, unsigned long id) { _id = id; }
//-------------------------------------------------------------------------
void C::rewind() const
{
  _current = 0;
  _pCurrentSubSeg = NULL;
}
//-------------------------------------------------------------------------
Seg* C::getSeg() const
{
  do
  {
    if (_pCurrentSubSeg == NULL)
    {
      if (_current >= _vect.size())
        return NULL;
      _pCurrentSubSeg = &get(_current);
      _pCurrentSubSeg->rewind();
    }
    Seg* p = _pCurrentSubSeg->getSeg();
    if (p != NULL)
      return p;
    _pCurrentSubSeg = NULL;
    _current++;
  }
  while (true);
}
//-------------------------------------------------------------------------
unsigned long C::begin() const
{
  bool isDefined = false;
  unsigned long b, e;
  getExtremeBoundaries(K::k, b, e, isDefined);
  if (!isDefined)
    throw Exception("Undefined begin value", __FILE__, __LINE__);
  return b;
}
//-------------------------------------------------------------------------
unsigned long C::length() const
{
  bool isDefined = false;
  unsigned long b, e;
  getExtremeBoundaries(K::k, b, e, isDefined);
  if (!isDefined)
    throw Exception("Undefined length value", __FILE__, __LINE__);
  return e-b;
}
//-------------------------------------------------------------------------
void C::add(SegAbstract& s)
{
  if (!getServer().isSameObject(s.getServer()))
    throw Exception("Cannot add a extern segment", __FILE__, __LINE__);
  _vect.addObject(s);
  s.addOwner(K::k, *this);
}
//-------------------------------------------------------------------------
void C::add(SegAbstract& s, unsigned long pos)
{
  if (!getServer().isSameObject(s.getServer()))
    throw Exception("Cannot add a extern segment", __FILE__, __LINE__);
  _vect.addObject(s, pos);
  s.addOwner(K::k, *this);
}
//-------------------------------------------------------------------------
Seg& C::addCopy(const Seg& s)
{
  Seg& seg = getServer().duplicateSeg(s);
  _vect.addObject(seg);
  seg.addOwner(K::k, *this);
  return seg;
}
//-------------------------------------------------------------------------
Seg& C::addCopy(const Seg& s, unsigned long pos)
{
  Seg& seg = getServer().duplicateSeg(s);
  _vect.addObject(seg, pos);
  seg.addOwner(K::k, *this);
  return seg;
}
//-------------------------------------------------------------------------
Seg& C::addNewSeg(unsigned long b, unsigned long l, unsigned long lc,
                  const String& s, const String& sn)
{
  Seg& seg = getServer().createSeg(b, l, lc, s, sn);
  _vect.addObject(seg);
  seg.addOwner(K::k, *this);
  return seg;
}
//-------------------------------------------------------------------------
void C::remove(SegAbstract& s)
{
  s.removeOwner(K::k, *this);
  _vect.removeObject(s);
}
//-------------------------------------------------------------------------
SegAbstract& C::remove(unsigned long i)
{
  SegAbstract& s = get(i);
  s.removeOwner(K::k, *this);
  _vect.removeObject(i);
  return s;
}
//-------------------------------------------------------------------------
void C::removeAll()
{
  for (unsigned long i=0; i<_vect.size(); i++)
    get(i).removeOwner(K::k, *this);
  _vect.clear(); // do not call deleteAllObjects()
}
//-------------------------------------------------------------------------
unsigned long C::getCount() const { return _vect.size(); }
//-------------------------------------------------------------------------
SegAbstract& C::get(unsigned long i) const { return _vect.getObject(i); }
//-------------------------------------------------------------------------
bool C::getFeatureLabelCode(unsigned long n, unsigned long& lc) const
{
  if (_pCurrentSeg != NULL && n >= _b && n < _e)
  {
    lc = _lc;
    return true;
  }
  bool f, l;
  return getFeatureLabelCode(n, lc, f, l);
}
//-------------------------------------------------------------------------
bool C::getFeatureLabelCode(unsigned long n, unsigned long& lc,
                            bool& isFirst, bool& isLast) const
{
  if (_pCurrentSeg != NULL && n >= _b && n < _e)
  {
    isFirst = (n == _b);
    isLast = (n == _e-1);
    lc = _lc;
    return true;
  }
  _pStartSeg = NULL;

  do
  {
    _pCurrentSeg = getSeg();

    if (_pCurrentSeg != NULL)
    {
      if (_pCurrentSeg == _pStartSeg)
        break;
      _b = _pCurrentSeg->begin();
      _e = _b + _pCurrentSeg->length();
      if (n >= _b && n < _e)
      {
        isFirst = (n == _b);
        isLast = (n == _e-1);
        lc = _lc = _pCurrentSeg->labelCode();
        return true;
      }
      if (_pStartSeg == NULL)
        _pStartSeg = _pCurrentSeg;
    }
    else
      rewind();
  }
  while (true);

  _pCurrentSeg = NULL;
  return false;
}
//-------------------------------------------------------------------------
void C::getExtremeBoundaries(const K&, unsigned long& b,
  /* methode recursive */     unsigned long& e, bool& isDefined) const
{
  for (unsigned long i=0; i<getCount(); i++)
  { get(i).getExtremeBoundaries(K::k, b, e, isDefined); }
}
//-------------------------------------------------------------------------
String C::getClassName() const { return "SegCluster"; }
//-------------------------------------------------------------------------
String C::toString() const
{
  String b("undefined");
  String l("undefined");
  try { b = String::valueOf(begin()); } catch (Exception&) {}
  try { l = String::valueOf(length()); } catch (Exception&) {}
  return Object::toString()
    + "\n  server name = '" + getServer().getServerName() + "'"
    + "\n  begin = " + b
    + "\n  length   = " + l
    + "\n  labelCode = " + String::valueOf(_labelCode)
    + "\n  string = '" + _string + "'"
    + "\n  sourceName = '" + _srcName + "'";
  // TODO : ajouter l'affichage de _list
}
//-------------------------------------------------------------------------
C::~SegCluster() { removeAll(); }
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_SegCluster_cpp)
