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

#if !defined(ALIZE_FeatureInputStreamModifier_cpp)
#define ALIZE_FeatureInputStreamModifier_cpp

#include <new>
#include "FeatureInputStreamModifier.h"
#include "Exception.h"
#include "LabelServer.h"
#include "Label.h"
#include "FeatureFlags.h"
#include "XLine.h"
#include "Config.h"

using namespace alize;
typedef FeatureInputStreamModifier M;

//-------------------------------------------------------------------------
M::FeatureInputStreamModifier(FeatureInputStream& is, const String& m,
                              bool ownStream)
:FeatureInputStream(is.getConfig()), _pInput(&is), _useMask(false),
_ownStream(ownStream) { setMask(m); }
//-------------------------------------------------------------------------
M& M::create(FeatureInputStream& is, const String& m, bool ownStream)
{
  M* p = new (std::nothrow) M(is, m, ownStream);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void M::setMask(const String& m)
{
  if (m == "NO_MASK")
  {
    _useMask = false;
    return;
  }
  _tmpMask.reset();
  unsigned long i = 0;
  String begin, end;

  if (m.isEmpty())
    goto xend;
  while (true)
  {
    if (m[i] < "0" || m[i] > "9")
      throw Exception("Invalid feature mask", __FILE__, __LINE__);
    begin += m[i++];
    end = begin;
    if (i == m.length())
    {
      updateMask(begin, end);
      goto xend;
    }
    if (m[i] == "-")
    {
      i++;
      if (i == m.length())
        throw Exception("Invalid feature mask", __FILE__, __LINE__);
      end.reset();
      do
      {
        if (m[i] < "0" || m[i] > "9")
          throw Exception("Invalid feature mask", __FILE__, __LINE__);
        end += m[i++];
        if (i == m.length())
        {
          updateMask(begin, end);
          goto xend;
        }
      }
      while (m[i] != ",");
    }
    if (m[i] == ",")
    {
      updateMask(begin, end);
      i++;
      if (i == m.length())
        throw Exception("Invalid feature mask", __FILE__, __LINE__);
      begin.reset();
      end.reset();
    }
  }
xend:
  _useMask = true;
  _mask = m;
  _selection.clear();
  char* p = const_cast<char*>(_tmpMask.c_str());
  for (i=0; i<_tmpMask.length(); i++)
    if (p[i] == '1')
     _selection.addValue(i);
  _selectionSize = _selection.size();
}
//-------------------------------------------------------------------------
void M::updateMask(const String& b, const String& e) // private
{
  unsigned long ee = e.toULong();
  unsigned long bb = b.toULong();
  if (ee < bb)
    throw Exception("Invalid feature mask", __FILE__, __LINE__);
  for (unsigned long i=bb; i<=ee; i++)
  {
    while (i >= _tmpMask.length())
      _tmpMask += "0";
    const_cast<char*>(_tmpMask.c_str())[i] = '1';
  }
}
//-------------------------------------------------------------------------
bool M::readFeature(Feature& f, unsigned long step)
{
  bool ok;
  if (!_useMask)
    ok = _pInput->readFeature(f, step);
  else if ( (ok = _pInput->readFeature(_feature, step)) )
  {
    f.setVectSize(K::k, _selectionSize);
    f.copySelectedData(_feature, _selection);
  }
  _error = _pInput->getError();
  return ok;
}
//-------------------------------------------------------------------------
bool M::addFeature(const Feature& f)
{
  bool ok;
  ok = _pInput->addFeature(f);
  return ok;
}
//-------------------------------------------------------------------------
bool M::writeFeature(const Feature& f, unsigned long step)
{
  bool ok;
  if (!_useMask)
    ok = _pInput->writeFeature(f, step);
  else
  {
    if (_selectionSize != f.getVectSize())
       throw Exception("Invalid feature mask : " + _mask,
                       __FILE__, __LINE__);
    _feature.setVectSize(K::k, _pInput->getVectSize());
    Feature::data_t* outputVect = _feature.getDataVector();
    Feature::data_t* inputVect = f.getDataVector();
    for (unsigned long i=0; i<_selectionSize; i++)
      outputVect[_selection[i]] = inputVect[i];
    ok = _pInput->writeFeature(_feature, step);
  }
  _error = _pInput->getError();
  return ok;
}
//-------------------------------------------------------------------------
unsigned long M::getFeatureCount() { return _pInput->getFeatureCount(); }
//-------------------------------------------------------------------------
unsigned long M::getVectSize() 
{
  if (_useMask)
    return _selectionSize;
  return _pInput->getVectSize();
}
//-------------------------------------------------------------------------
const FeatureFlags& M::getFeatureFlags() 
{ return _pInput->getFeatureFlags(); }
// TODO : que faire des flags si _useMask = true ?
//-------------------------------------------------------------------------
void M::seekFeature(unsigned long i, const String& s)
{ _pInput->seekFeature(i, s); }
//-------------------------------------------------------------------------
real_t M::getSampleRate() { return _pInput->getSampleRate(); }
//-------------------------------------------------------------------------
void M::reset() { _pInput->reset(); }
//-------------------------------------------------------------------------
void M::close() { _pInput->close(); }
//-------------------------------------------------------------------------
unsigned long M::getSourceCount() {return _pInput->getSourceCount();}
//-------------------------------------------------------------------------
unsigned long M::getFeatureCountOfASource(unsigned long srcIdx)
{ return _pInput->getFeatureCountOfASource(srcIdx); }
//-------------------------------------------------------------------------
unsigned long M::getFeatureCountOfASource(const String& f)
{ return _pInput->getFeatureCountOfASource(f); }
//-------------------------------------------------------------------------
unsigned long M::getFirstFeatureIndexOfASource(unsigned long srcIdx)
{ return _pInput->getFirstFeatureIndexOfASource(srcIdx); }
//-------------------------------------------------------------------------
unsigned long M::getFirstFeatureIndexOfASource(const String& srcName)
{ return _pInput->getFirstFeatureIndexOfASource(srcName); }
//-------------------------------------------------------------------------
const String& M::getNameOfASource(unsigned long srcIdx)
{ return _pInput->getNameOfASource(srcIdx); }
//-------------------------------------------------------------------------
String M::getClassName() const { return "FeatureInputStreamModifier"; }
//-------------------------------------------------------------------------
String M::toString() const
{
  String s =  FeatureInputStream::toString()
    + "\n  input stream = " + _pInput->getClassName()
    + "[" + getAddress() + "]";
  if (_useMask)
    s += "\n  mask = '" + _mask + "'";
  else
    s += "\n  mask = no mask";
  return s;
}
//-------------------------------------------------------------------------
M::~FeatureInputStreamModifier()
{
  if (_ownStream)
    delete _pInput;
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FeatureInputStreamModifier_cpp)

