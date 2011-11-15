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

#if !defined(ALIZE_LKVector_cpp)
#define ALIZE_LKVector_cpp

#include <new>
#include <math.h>
#include <memory.h>
#include <cstdlib>
#include "LKVector.h"
#include "alizeString.h"
#include "Exception.h"

using namespace alize;

//-------------------------------------------------------------------------
LKVector::LKVector(unsigned long capacity, unsigned long size)
:Object(), _capacity(capacity!=0?capacity:1), _size(0),
_array(createArray()) { setSize(size); }

//-------------------------------------------------------------------------
//LIUM, sylvain
LKVector::LKVector(const LKVector& obj){
  _capacity = obj._capacity;
  _size = 0;
  if (_capacity > 0) {
	  _array = createArray();
	  //memcpy(_array, obj._array, _capacity*sizeof(_array[0]));
	  for(unsigned long i = 0; i < obj._size; i++) {
		  addValue(obj[i]);
      }
  }
  sumNonTopDistribWeights = obj.sumNonTopDistribWeights;
  sumNonTopDistribLK = obj.sumNonTopDistribLK;
  topDistribsCount = obj.topDistribsCount;
}

//LIUM, sylvain
const LKVector& LKVector::operator=(const LKVector& obj){
  _capacity = obj._capacity;
  _size = 0;
  if (_capacity > 0) {
	  _array = createArray();
	  for(unsigned long i = 0; i < obj._size; i++) {
		  addValue(obj[i]);
      }
  }
  sumNonTopDistribWeights = obj.sumNonTopDistribWeights;
  sumNonTopDistribLK = obj.sumNonTopDistribLK;
  topDistribsCount = obj.topDistribsCount;
  return *this;
}
//-------------------------------------------------------------------------
LKVector::type* LKVector::createArray() const // private
{
  assert(_capacity != 0);
  type* p = new (std::nothrow) type[_capacity];
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return p;
}
//-------------------------------------------------------------------------
LKVector::type& LKVector::operator[](unsigned long i)
{
  assertIsInBounds(__FILE__, __LINE__, i, _size);
  assert(_array != NULL);
  return _array[i];
}
//-------------------------------------------------------------------------
const LKVector::type& LKVector::operator[](unsigned long i) const
{
  assertIsInBounds(__FILE__, __LINE__, i, _size);
  assert(_array != NULL);
  return _array[i];
}
//-------------------------------------------------------------------------
void LKVector::addValue(type element)
{
  assert(_array != NULL);
  if (_size == _capacity)
  {
    _capacity += _capacity;
    type* oldArray = _array;
    _array = createArray(); // can throw OutOfMemoryException
    memcpy(_array, oldArray, _size*sizeof(_array[0]));
    delete[] oldArray;
  }
  _array[_size] = element;
  _size++;
}
//-------------------------------------------------------------------------
void LKVector::pack(unsigned long size)
{
    unsigned long oldSize = _size;
    _size = size;
    _capacity = _size;
    type* oldArray = _array;
    _array = createArray(); // can throw OutOfMemoryException
	unsigned long s = oldSize;
	if (oldSize > size) {
		s = size;
	}
    memcpy(_array, oldArray, s*sizeof(_array[0]));
    delete[] oldArray;
}
//-------------------------------------------------------------------------
void LKVector::setSize(unsigned long size)
{
  assert(_array != NULL);
  if (size > _capacity)
  {
    unsigned long oldSize = _size;
    _size = size;
    _capacity = _size;
    type* oldArray = _array;
    _array = createArray(); // can throw OutOfMemoryException
    memcpy(_array, oldArray, oldSize*sizeof(_array[0]));
    delete[] oldArray;
  }
  else
    _size = size;
}
//-------------------------------------------------------------------------
int LKVector::compare(const void *s1, const void *s2) // static method
{
  if (((type*)s1)->lk > ((type*)s2)->lk)
    return -1;
  if (((type*)s1)->lk < ((type*)s2)->lk)
    return 1;
  return 0;
}
//-------------------------------------------------------------------------
void LKVector::descendingSort() const
{
  assert(_array != NULL);
  qsort(_array, _size, sizeof(type), compare);
}
//-------------------------------------------------------------------------
LKVector::type* LKVector::getArray() const { return _array; }
//-------------------------------------------------------------------------
void LKVector::clear() { _size = 0; }
//-------------------------------------------------------------------------
unsigned long LKVector::size() const { return _size; }
//-------------------------------------------------------------------------
String LKVector::getClassName() const { return "LKVector"; }
//-------------------------------------------------------------------------
String LKVector::toString() const
{
  String s = Object::toString() + "Size = " + String::valueOf(_size);
  for (unsigned long i=0; i<_size; i++)
  {
    s += "\n  idx = " + String::valueOf(_array[i].idx)
      +  " lk = " + String::valueOf(_array[i].lk);
  }

  return s;
}
//-------------------------------------------------------------------------
LKVector::~LKVector() { delete[] _array; }
//-------------------------------------------------------------------------

#endif  // ALIZE_LKVector_cpp
