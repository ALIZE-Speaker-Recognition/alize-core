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

#if !defined(ALIZE_DistribRefVector_cpp)
#define ALIZE_DistribRefVector_cpp

#include <new>
#include <memory.h>
#include "Object.h"
#include "alizeString.h"
#include "Exception.h"
#include "Distrib.h"
#include "DistribRefVector.h"

using namespace alize;

//-------------------------------------------------------------------------
DistribRefVector::DistribRefVector(unsigned long capacity)
:Object(), _size(0), _capacity(capacity!=0?capacity:1),
_array(createArray()) {}
//-------------------------------------------------------------------------
DistribRefVector::DistribRefVector(const DistribRefVector& v)
:Object(), _size(v._size), _capacity(v._size!=0?v._size:1),
_array(createArray())
{
  for (unsigned long i=0; i<_size; i++)
    (_array[i] = v._array[i])->refCounter(K::k)++;
}
//-------------------------------------------------------------------------
Distrib** DistribRefVector::createArray() const
{
  assert(_capacity != 0);
  Distrib** p = new (std::nothrow) Distrib*[_capacity];
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return p;
}
//-------------------------------------------------------------------------
Distrib& DistribRefVector::getDistrib(unsigned long i) const
{
  assertIsInBounds(__FILE__, __LINE__, i, _size);
  assert(_array != NULL);
  return *_array[i];
}
//-------------------------------------------------------------------------
void DistribRefVector::setDistrib(Distrib& d, unsigned long i) const
{
  assert(_array != NULL);
  assertIsInBounds(__FILE__, __LINE__, i, _size);
  Distrib* pOld = _array[i];
  _array[i] = &d;
  d.refCounter(K::k)++;
  if (--(pOld->refCounter(K::k)) == 0)
    delete pOld;
}
//-------------------------------------------------------------------------
unsigned long DistribRefVector::addDistrib(Distrib& d)
{
  assert(_array != NULL);
  if (_size == _capacity)
  {
    _capacity += _capacity;
    Distrib** oldArray = _array;
    _array = createArray();
    memcpy(_array, oldArray, _size*sizeof(_array[0]));
    delete[] oldArray;
  }
  _array[_size] = &d;
  d.refCounter(K::k)++;
  return ++_size-1;
}
//-------------------------------------------------------------------------
void DistribRefVector::deleteUnreferencedDistribs()
{
  assert(_array != NULL);
  unsigned long i, j=0;
  for (i=0; i<_size; i++)
  {
    Distrib* p = _array[i];
    assert(p != NULL);
    if (p->refCounter(K::k) == 1)
      delete p;
    else
    {
      _array[j] = _array[i];
      j++;
    }
  }
  _size = j;
}
//-------------------------------------------------------------------------
void DistribRefVector::deleteUnreferencedDistrib(const Distrib& d)
{
  assert(_array != NULL);
  unsigned long i;
  for (i=0; i<_size; i++)
    if (_array[i] == &d)
    {
      if (_array[i]->refCounter(K::k) == 1)
      {
        delete _array[i];
        for (i++; i<_size; i++)
          _array[i-1] = _array[i];
        _size--;
      }
      return;   
    }
  throw Exception("Distrib not found in the vector", __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
Distrib** DistribRefVector::getArray() const { return _array; }
//-------------------------------------------------------------------------
unsigned long DistribRefVector::size() const { return _size; }
//-------------------------------------------------------------------------
void DistribRefVector::clear()
{
  for (unsigned long i=0; i<_size; i++)
  {
    Distrib* p = _array[i];
    assert(p != NULL);
    if ( (--(p->refCounter(K::k))) == 0)
      delete p;
  }
  _size = 0;
}
//-------------------------------------------------------------------------
String DistribRefVector::getClassName() const { return "DistribRefVector";}
//-------------------------------------------------------------------------
DistribRefVector::~DistribRefVector()
{
  clear();
  delete [] _array;
}
//-------------------------------------------------------------------------

#endif  // ALIZE_DistribRefVector_cpp
