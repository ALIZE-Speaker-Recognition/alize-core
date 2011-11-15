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

#if !defined(ALIZE_FrameAccGD_cpp)
#define ALIZE_FrameAccGD_cpp

#include "FrameAccGD.h"
#include "Exception.h"
#include "Feature.h"
#include <new>
#include <cmath>

using namespace alize;
typedef FrameAccGD A;

//-------------------------------------------------------------------------
A::FrameAccGD()
:FrameAcc() {}
//-------------------------------------------------------------------------
FrameAccGD& A::create()
{
  A* p = new (std::nothrow) A();
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
A::FrameAccGD(const FrameAccGD& a)
:FrameAcc() { copy(a); }
//-------------------------------------------------------------------------
const FrameAccGD& A::operator=(const FrameAccGD& a)
{ copy(a); return *this; }
//-------------------------------------------------------------------------
void A::copy(const FrameAccGD& a) // private
{
  _vectSize = a._vectSize;
  _vectSizeDefined = a._vectSizeDefined;
  _count = a._count;
  _computed = a._computed;
  _stdComputed = a._stdComputed;
  _accVect = a._accVect;
  _meanVect = a._meanVect;
  _covVect = a._covVect;
  _stdVect = a._stdVect;
  _xaccVect = a._xaccVect;
}
//-------------------------------------------------------------------------
const DoubleVector& A::getCovVect()
{
  if (!_computed)
    computeAll();
  return _covVect;
}
//-------------------------------------------------------------------------
const DoubleVector& A::getStdVect()
{
  if (!_stdComputed)
  {
    if (!_computed)
      computeAll();
    _stdVect.setSize(_vectSize);
    double* stdVect = _stdVect.getArray();
    const double* covVect = _covVect.getArray();
    for (unsigned long i=0; i<_vectSize; i++)
      stdVect[i] = sqrt(covVect[i]);
    _stdComputed = true;
  }
  return _stdVect;
}
//-------------------------------------------------------------------------
const DoubleVector& A::getxAccVect() const { return _xaccVect; }
//-------------------------------------------------------------------------
void A::accumulate(const Feature& f)
{
  const unsigned long vectSize = f.getVectSize();
  if (!_vectSizeDefined)
  {
    _vectSize = vectSize;
    _accVect.setSize(_vectSize);
    _accVect.setAllValues(0.0);
    _xaccVect.setSize(_vectSize);
    _xaccVect.setAllValues(0.0);
    _vectSizeDefined = true;
  }
  else if (vectSize != _vectSize)
    throw Exception("Incompatible vectSize ("
          + String::valueOf(vectSize) + "/"
          + String::valueOf(_vectSize) + ")", __FILE__, __LINE__);
  const double* dataVect = f.getDataVector();
  for (unsigned long i=0; i<_vectSize; i++)
  {
    const double v = dataVect[i];
    _accVect[i] += v;
    _xaccVect[i] += v*v;
  }
  _count++;
  _computed = false;
  _stdComputed = false;
}
//-------------------------------------------------------------------------
void A::add(const FrameAccGD& f)
{
  const unsigned long vectSize = f.getVectSize();
  if (!_vectSizeDefined)
  {
    _vectSize = vectSize;
    _accVect.setSize(_vectSize);
    _accVect.setAllValues(0.0);
    _xaccVect.setSize(_vectSize);
    _xaccVect.setAllValues(0.0);
    _vectSizeDefined = true;
  }
  else if (vectSize != _vectSize)
    throw Exception("Incompatible vectSize ("
          + String::valueOf(vectSize) + "/"
          + String::valueOf(_vectSize) + ")", __FILE__, __LINE__);

  const DoubleVector& accVect =  f.getAccVect();
  const DoubleVector& xAccVect = f.getxAccVect();

  for(unsigned long i = 0; i<_vectSize; i++)
  {
    _accVect[i] += accVect[i];
    _xaccVect[i] += xAccVect[i];
  }
  _count += f.getCount();
  _computed = false;
  _stdComputed = false;
}
//-------------------------------------------------------------------------
void A::deaccumulate(const Feature& f)
{
  const unsigned long vectSize = f.getVectSize();
  if (!_vectSizeDefined)
  {
	return;
  }
  else if (vectSize != _vectSize)
    throw Exception("Incompatible vectSize ("
          + String::valueOf(vectSize) + "/"
          + String::valueOf(_vectSize) + ")", __FILE__, __LINE__);

  const double* dataVect = f.getDataVector();

  for (unsigned long i=0; i<_vectSize; i++)
  {
    const double v = dataVect[i];
    _accVect[i] -= v;
    _xaccVect[i] -= v*v;
  }
  _count--;
  _computed = false;
  _stdComputed = false;

}
//-------------------------------------------------------------------------
void A::computeAll()
{
  if (_count == 0)
    throw Exception("No frame accumulated", __FILE__, __LINE__);
  const double invCount = 1.0/(double)_count;
  const unsigned long vectSize = getVectSize();
  unsigned long i;  
  _meanVect.setSize(_vectSize);
  _covVect.setSize(vectSize);
  _stdVect.setSize(vectSize);
  const double* accVect = _accVect.getArray();
  const double* xaccVect = _xaccVect.getArray();
  double* meanVect = _meanVect.getArray();
  double* covVect = _covVect.getArray();
  for (i=0; i<vectSize; i++)
  {
    const double mean = accVect[i]*invCount;
    meanVect[i] = mean;
    covVect[i] = xaccVect[i]*invCount - mean*mean;
  }

   // compute det --------------------------------

  _det = 1.0;
  for (i=0; i< _vectSize; i++)
  {
     _det *= covVect[i];
  }

  // compute cst -------------------------------

  if (_det > EPS_LK)
    _cst = 1.0 / ( pow(_det, 0.5) * pow( PI2 , _vectSize/2.0 ) );
  else
    _cst = 1.0 / ( pow(EPS_LK, 0.5) * pow( PI2 , _vectSize/2.0 ) );

  _computed = true;
}
//-------------------------------------------------------------------------
String A::getClassName() const { return "FrameAccGD"; }
//-------------------------------------------------------------------------
A::~FrameAccGD() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FrameAccGD_cpp)

