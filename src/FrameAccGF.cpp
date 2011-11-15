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

#if !defined(ALIZE_FrameAccGF_cpp)
#define ALIZE_FrameAccGF_cpp

#include "FrameAccGF.h"
#include "Exception.h"
#include "Feature.h"
#include <new>
#include <cmath>

using namespace alize;
typedef FrameAccGF A;

//-------------------------------------------------------------------------
A::FrameAccGF()
:FrameAcc() {}
//-------------------------------------------------------------------------
A::FrameAccGF(const FrameAccGF& a)
:FrameAcc() { copy(a); }
//-------------------------------------------------------------------------
const FrameAccGF& A::operator=(const FrameAccGF& a)
{ copy(a); return *this; }
//-------------------------------------------------------------------------
void A::copy(const FrameAccGF& a) // private
{
  _vectSize = a._vectSize;
  _vectSizeDefined = a._vectSizeDefined;
  _count = a._count;
  _computed = a._computed;
  _stdComputed = a._stdComputed;
  _accVect = a._accVect;
  _meanVect = a._meanVect;
  _covMatrix = a._covMatrix;
  _stdMatrix = a._stdMatrix;
  _xaccMatrix = a._xaccMatrix;
}
//-------------------------------------------------------------------------
A& A::create()
{
  A* p = new (std::nothrow) A();
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
const DoubleSquareMatrix& A::getCovMatrix()
{
  if (!_computed)
    computeAll();
  return _covMatrix;
}
//-------------------------------------------------------------------------
const DoubleSquareMatrix& A::getStdMatrix()
{
  if (!_stdComputed)
  {
    if (!_computed)
      computeAll();
    _stdMatrix.setSize(_vectSize);
    double* stdMatrix = _stdMatrix.getArray();
    const double* covMatrix = _covMatrix.getArray();
    unsigned long i, j, ii;
    for (i=0; i<_vectSize; i++)
      for (j=i, ii = i*(_vectSize+1); j<_vectSize; j++, ii += _vectSize)
        stdMatrix[ii] = sqrt(covMatrix[ii]);
    _stdComputed = true;
  }
  return _stdMatrix;
}
//-------------------------------------------------------------------------
const DoubleSquareMatrix& A::getxAccMatrix() const { return _xaccMatrix; }
//-------------------------------------------------------------------------
void A::accumulate(const Feature& f)
{
  const unsigned long vectSize = f.getVectSize();
  if (!_vectSizeDefined)
  {
    _vectSize = vectSize;
    _accVect.setSize(_vectSize);
    _accVect.setAllValues(0.0);
    _xaccMatrix.setSize(_vectSize);
    _xaccMatrix.setAllValues(0.0);
    _vectSizeDefined = true;
  }
  else if (vectSize != _vectSize)
    throw Exception("Incompatible vectSize ("
          + String::valueOf(vectSize) + "/"
          + String::valueOf(_vectSize) + ")", __FILE__, __LINE__);
  const double* dataVect = f.getDataVector();
  double* xaccMatrix = _xaccMatrix.getArray();
  unsigned long i,j, ii;
  for (i=0; i<_vectSize; i++)
  {
    const double v = dataVect[i];
    _accVect[i] += v;
    for (j=i, ii=i*(_vectSize+1); j<_vectSize; j++, ii+=_vectSize)
      xaccMatrix[ii] += v*dataVect[j];
  }
  _count++;
  _computed = false;
  _stdComputed = false;
}
//-------------------------------------------------------------------------
void A::add(const FrameAccGF& f)
{
  const unsigned long vectSize = f.getVectSize();
  if (!_vectSizeDefined)
  {
    _vectSize = vectSize;
    _accVect.setSize(_vectSize);
    _accVect.setAllValues(0.0);
    _xaccMatrix.setSize(_vectSize);
    _xaccMatrix.setAllValues(0.0);
    _vectSizeDefined = true;
  }
  else if (vectSize != _vectSize)
    throw Exception("Incompatible vectSize ("
          + String::valueOf(vectSize) + "/"
          + String::valueOf(_vectSize) + ")", __FILE__, __LINE__);

  const DoubleVector& accVect =  f.getAccVect();
  const DoubleSquareMatrix& xaccMatrix = f.getxAccMatrix();
  //xaccMatrix.setSize(vectSize);
  double * matrixValues = xaccMatrix.getArray();
  double * _matrixValues = _xaccMatrix.getArray();

  unsigned long i,j, ii;
  for (i=0; i<_vectSize; i++)
  {
    _accVect[i] += accVect[i];
    for (j=i, ii=i*(_vectSize+1); j<_vectSize; j++, ii+=_vectSize)
      _matrixValues[ii] += matrixValues[ii];
  }
  _count += f.getCount();
  _computed = false;
  _stdComputed = false;
}
//-------------------------------------------------------------------------
void A::deaccumulate(const Feature& f)
{
	const unsigned long vectSize = f.getVectSize();

	if (!_vectSizeDefined) {
		return;
	} else if (vectSize != _vectSize) {
	    throw Exception("Incompatible vectSize ("
		  + String::valueOf(vectSize) + "/"
		  + String::valueOf(_vectSize) + ")", __FILE__, __LINE__);
	}

	const double* dataVect = f.getDataVector();
	double* xaccMatrix = _xaccMatrix.getArray();
	unsigned long i,j, ii;
	for (i=0; i<_vectSize; i++)
	{
		const double v = dataVect[i];
		_accVect[i] -= v;
		for (j=i, ii=i*(_vectSize+1); j<_vectSize; j++, ii+=_vectSize)
			xaccMatrix[ii] -= v*dataVect[j];
	}
	_count--;
	_computed = false;
	_stdComputed = false;
}
//-------------------------------------------------------------------------
void A::computeAll() // private
{
  if (_count == 0)
    throw Exception("No frame accumulated", __FILE__, __LINE__);
  const double invCount = 1.0/(double)_count;
  const unsigned long vectSize = getVectSize();
  unsigned long i, j, ii; 
  _meanVect.setSize(_vectSize);
  _covMatrix.setSize(vectSize);
  _stdMatrix.setSize(vectSize);
  const double* accVect = _accVect.getArray();
  const double* xaccMatrix = _xaccMatrix.getArray();
  double* meanVect = _meanVect.getArray();
  double* covMatrix = _covMatrix.getArray();
  for (i=0; i<vectSize; i++)
    meanVect[i] = accVect[i]*invCount;
  for (i=0; i<vectSize; i++)
  {
    const double mean = meanVect[i];
    for (j=i, ii = i*(vectSize+1); j<vectSize; j++, ii += vectSize)
      covMatrix[ii] = xaccMatrix[ii]*invCount - mean*meanVect[j];
  }

  // compute det and cov inv --------------------------------

   DoubleSquareMatrix covInvMatrix(vectSize);

  _det = _covMatrix.invert(covInvMatrix);

  // compute cst -------------------------------

  if (_det > EPS_LK)
    _cst = 1.0 / ( pow(_det, 0.5) * pow( PI2 , _vectSize/2.0 ) );
  else
    _cst = 1.0 / ( pow(EPS_LK, 0.5) * pow( PI2 , _vectSize/2.0 ) );
  _computed = true;
}
//-------------------------------------------------------------------------
String A::getClassName() const { return "FrameAccGF"; }
//-------------------------------------------------------------------------
A::~FrameAccGF() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FrameAccGF_cpp)

