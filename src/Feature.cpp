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

#if !defined(ALIZE_Feature_cpp)
#define ALIZE_Feature_cpp

#include <new>
#include <memory.h>
#include "Feature.h"
#include "alizeString.h"
#include "Exception.h"
#include "Config.h"
#include "ULongVector.h"

using namespace alize;

//-------------------------------------------------------------------------
Feature::Feature(unsigned long vectSize)
:Object(), _vectSize(vectSize), _isValid(false), _labelCode(0),
 _dataVector(createVector())
{
  //for (unsigned long i=0; i<_vectSize; i++)
  //  _dataVector[i] = 0;
}
//-------------------------------------------------------------------------
Feature::Feature(const Config& config)
:Object(), _vectSize(config.getParam_vectSize()), _isValid(false),
 _labelCode(0), _dataVector(createVector())
{
  //for (unsigned long i=0; i<_vectSize; i++)
  //  _dataVector[i] = 0;
}
//-------------------------------------------------------------------------
Feature& Feature::create(unsigned long vectSize)
{
  Feature* p = new (std::nothrow) Feature(vectSize);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
Feature::Feature(const Feature& f)
:Object(), _vectSize(f._vectSize), _isValid(f._isValid),
 _labelCode(f._labelCode), _dataVector(createVector())
{
  memcpy(_dataVector, f._dataVector, _vectSize*sizeof(_dataVector[0]));
}
//-------------------------------------------------------------------------
const Feature& Feature::operator=(const Feature& f)
{
  if (_vectSize != f._vectSize)
    throw Exception("Feature copy : source vectSize ("
        + String::valueOf(f._vectSize)
        + ") does not match target vectSize ("
        + String::valueOf(_vectSize) + ")", __FILE__, __LINE__);
  memcpy(_dataVector, f._dataVector, _vectSize*sizeof(_dataVector[0]));
  _isValid   = f._isValid;
  _labelCode = f._labelCode;
  return *this;
}
//-------------------------------------------------------------------------
bool Feature::operator==(const Feature& f) const
{
  if (_vectSize != f._vectSize || _isValid != f._isValid)
    return false;
  for (unsigned long i=0; i<_vectSize; i++)
    if (_dataVector[i] != f._dataVector[i])
      return false;
  return true;
}  
//-------------------------------------------------------------------------
bool Feature::operator!=(const Feature& c) const { return !(*this == c); }
//-------------------------------------------------------------------------
Feature::data_t* Feature::createVector() const // private
{
  data_t* p = new (std::nothrow) data_t[_vectSize==0?1:_vectSize];
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return p;
}
//-------------------------------------------------------------------------
void Feature::reset()
{
  for (unsigned long i=0; i<_vectSize; i++)
  {
    _dataVector[i] = 0.0;
  }
  _labelCode = 0;
  _isValid   = false;
}
//-------------------------------------------------------------------------
bool Feature::isValid() const { return _isValid; }
//-------------------------------------------------------------------------
void Feature::setValidity(bool v) { _isValid = v; }
//-------------------------------------------------------------------------
unsigned long Feature::getLabelCode() const { return _labelCode; }
//-------------------------------------------------------------------------
void Feature::setLabelCode(unsigned long v) { _labelCode = v; }
//-------------------------------------------------------------------------
Feature::data_t& Feature::operator[](unsigned long i)
{
  assertIsInBounds(__FILE__, __LINE__, i, _vectSize);
  assert(_dataVector != NULL);
  return _dataVector[i];
}
//-------------------------------------------------------------------------
Feature::data_t Feature::operator[](unsigned long i) const
{
  assertIsInBounds(__FILE__, __LINE__, i, _vectSize);
  assert(_dataVector != NULL);
  return _dataVector[i];
}
//-------------------------------------------------------------------------
unsigned long Feature::getVectSize() const { return _vectSize; }
//-------------------------------------------------------------------------
void Feature::setVectSize(const K&, unsigned long s)
{
  if (s != _vectSize)
  {
    // all data are lost
    assert(_dataVector != NULL);
    delete [] _dataVector;
    _vectSize = s;
    _dataVector = createVector();
  }
}
//-------------------------------------------------------------------------
void Feature::setData(const FloatVector& v, unsigned long start)
{
  if (_vectSize + start> v.size())
    throw Exception("Lack of data to fill the feature", __FILE__, __LINE__);
  const float* source = v.getArray();
  for (unsigned long i=0; i<_vectSize; i++)
    _dataVector[i] = (data_t)source[i+start];
}
//-------------------------------------------------------------------------
void Feature::setData(const DoubleVector& v, unsigned long start)
{
  if (_vectSize + start> v.size())
    throw Exception("Lack of data to fill the feature", __FILE__, __LINE__);
  const double* source = v.getArray();
  for (unsigned long i=0; i<_vectSize; i++)
    _dataVector[i] = (data_t)source[i+start];
}
//-------------------------------------------------------------------------
void Feature::copySelectedData(const Feature& f, const ULongVector& selection)
{
  const unsigned long selectionSize = selection.size();
  if (selectionSize > _vectSize)
      throw Exception("", __FILE__, __LINE__);
  const unsigned long sourceSize = f._vectSize;
  unsigned long* selectionVect = selection.getArray();
  double* sourceVect = f._dataVector;
  unsigned long sourceIdx;
  for (unsigned long i=0; i<selectionSize; i++)
  {
    if ( (sourceIdx = selectionVect[i]) >= sourceSize)
      throw Exception("", __FILE__, __LINE__);
    _dataVector[i] = sourceVect[sourceIdx];
  }
  // copy validity and labelCode too
  _labelCode = f._labelCode;
  _isValid = f._isValid;
}
//-------------------------------------------------------------------------
Feature::data_t* Feature::getDataVector() const { return _dataVector; }
//-------------------------------------------------------------------------
String Feature::getClassName() const { return "Feature"; }
//-------------------------------------------------------------------------
String Feature::toString() const
{
  String s = Object::toString()
    + "\n  vectSize   = " + String::valueOf(_vectSize)
    + "\n  label code = " + String::valueOf(_labelCode)
    + "\n  is valid   = " + String::valueOf(_isValid);
  for (unsigned long i=0; i<_vectSize; i++)
    s += "\n  data[" + String::valueOf(i) + "] = "
      + String::valueOf(operator[](i)); 
  return s;
}
//-------------------------------------------------------------------------
Feature::~Feature()
{
  assert(_dataVector != NULL);
  delete[] _dataVector;
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_Feature_cpp)

