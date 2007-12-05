/*
Alize is a free, open tool for speaker recognition

Alize is a development project initiated by the ELISA consortium
  [www.lia.univ-avignon.fr/heberges/ALIZE/ELISA] and funded by the
  French Research Ministry in the framework of the
  TECHNOLANGUE program [www.technolangue.net]
  [www.technolangue.net]

The Alize project team wants to highlight the limits of voice 
  authentication in a forensic context.
  The following paper proposes a good overview of this point:
  [Bonastre J.F., Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., 
  Magrin-chagnolleau I., Person  Authentification by Voice: A Need of 
  Caution, Eurospeech 2003, Genova]
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
  the use of Alize

Copyright (C) 2003-2005
  Laboratoire d'informatique d'Avignon [www.lia.univ-avignon.fr]
  Frederic Wils [frederic.wils@lia.univ-avignon.fr]
  Jean-Francois Bonastre [jean-francois.bonastre@lia.univ-avignon.fr]
      
This file is part of Alize.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if !defined(ALIZE_ULongVector_cpp)
#define ALIZE_ULongVector_cpp

#include <new>
#include <math.h>
#include <memory.h>
#include <cstdlib>
#include "ULongVector.h"
#include "alizeString.h"
#include "Exception.h"

using namespace alize;

//-------------------------------------------------------------------------
ULongVector::ULongVector(unsigned long capacity, unsigned long size)
:Object(), _size(0), _capacity(capacity!=0?capacity:1), 
_array(createArray()) { setSize(size); }
//-------------------------------------------------------------------------
ULongVector::ULongVector(const ULongVector& v)
:Object(), _size(v._size), _capacity(v._size!=0?v._size:1),
 _array(createArray())
{ memcpy(_array, v._array, _size*sizeof(_array[0])); }
//-------------------------------------------------------------------------
const ULongVector& ULongVector::operator=(const ULongVector& v)
{
  if (this->isSameObject(v))
    return v;
  assert(_array != NULL);
  _size = v._size;
  if (_capacity < _size)
  {
    delete[] _array;
    _capacity = _size!=0?_size:1;
    _array = createArray();
  }
  memcpy(_array, v._array, _size*sizeof(_array[0]));
  return *this;
}
//-------------------------------------------------------------------------
bool ULongVector::operator==(const ULongVector& v) const
{
  if (_size != v._size)
    return false;
  for (unsigned long i=0; i<_size; i++)
  {
    if (_array[i] != v._array[i])
      return false;
  }
  return true;
}
//-------------------------------------------------------------------------
bool ULongVector::operator!=(const ULongVector& v) const
{ return !(*this == v); }
//-------------------------------------------------------------------------
ULongVector::_type* ULongVector::createArray() const // private
{
  assert(_capacity != 0);
  _type* p = new (std::nothrow) _type[_capacity];
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return p;
}
//-------------------------------------------------------------------------
ULongVector::_type& ULongVector::operator[](unsigned long i)
{
  assertIsInBounds(__FILE__, __LINE__, i, _size);
  assert(_array != NULL);
  return _array[i];
}
//-------------------------------------------------------------------------
ULongVector::_type ULongVector::operator[](unsigned long i) const
{
  assertIsInBounds(__FILE__, __LINE__, i, _size);
  assert(_array != NULL);
  return _array[i];
}
//-------------------------------------------------------------------------
ULongVector& ULongVector::addValue(_type element)
{
  assert(_array != NULL);
  if (_size == _capacity)
  {
    _capacity += _capacity;
    _type* oldArray = _array;
    _array = createArray(); // can throw OutOfMemoryException
    memcpy(_array, oldArray, _size*sizeof(_array[0]));
    delete[] oldArray;
  }
  _array[_size] = element;
  _size++;
  return *this;
}
//-------------------------------------------------------------------------
void ULongVector::addValue(const ULongVector& v)
{
  for (unsigned long i=0; i<v._size; i++)
  { addValue(v._array[i]); }
  // TODO : methode a optimiser
}
//-------------------------------------------------------------------------
void ULongVector::setSize(unsigned long size)
{
  assert(_array != NULL);
  if (size > _capacity)
  {
    unsigned long oldSize = _size;
    _size = size;
    _capacity = _size;
    _type* oldArray = _array;
    _array = createArray(); // can throw OutOfMemoryException
    memcpy(_array, oldArray, oldSize*sizeof(_array[0]));
    delete[] oldArray;
  }
  else
    _size = size;
}
//-------------------------------------------------------------------------
int ULongVector::compare(const void *s1, const void *s2) // static method
{
  if (*((_type*)s1) > *((_type*)s2))
    return 1;
  if (*((_type*)s1) < *((_type*)s2))
    return -1;
  return 0;
}
//-------------------------------------------------------------------------
void ULongVector::ascendingSort() const
{
  assert(_array != NULL);
  qsort(_array, _size, sizeof(_type), compare);
}
//-------------------------------------------------------------------------
void ULongVector::removeValues(unsigned long first, unsigned long last) 
{
  if (last>_size-1)
    last = _size-1;
  if (first > last)
    return;
  last++;
  assert(_array != NULL);
  for (; last<_size; first++, last++)
  { _array[first] = _array[last]; }
  _size -= last-first;
}
//-------------------------------------------------------------------------
ULongVector::_type* ULongVector::getArray() const { return _array; }
//-------------------------------------------------------------------------
void ULongVector::clear() { _size = 0; }
//-------------------------------------------------------------------------
unsigned long ULongVector::size() const { return _size; }
//-------------------------------------------------------------------------
String ULongVector::getClassName() const { return "ULongVector"; }
//-------------------------------------------------------------------------
String ULongVector::toString() const
{
  String s = Object::toString() + " size = " + String::valueOf(_size);
  for (unsigned long i=0; i<_size; i++)
  {
    s += "  \n  [" + String::valueOf(i) + "] = "
    + String::valueOf(_array[i]);
  }
  return s;
}
//-------------------------------------------------------------------------
ULongVector::~ULongVector() { delete[] _array; }
//-------------------------------------------------------------------------

#endif  // ALIZE_ULongVector_cpp
