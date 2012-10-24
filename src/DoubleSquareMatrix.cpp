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

#if !defined(ALIZE_DoubleSquareMatrix_cpp)
#define ALIZE_DoubleSquareMatrix_cpp

#include <new>
#include <math.h>
#include <memory.h>
#include <cstdlib>
#include "DoubleSquareMatrix.h"
#include "alizeString.h"
#include "Exception.h"

using namespace alize;
typedef DoubleSquareMatrix M;

//-------------------------------------------------------------------------
M::DoubleSquareMatrix(unsigned long size)
:Object(), _size(size), _array(size*size, size*size) {}
//-------------------------------------------------------------------------
M::DoubleSquareMatrix(const DoubleSquareMatrix& v)
:Object(), _size(v._size), _array(v._array) {}
//-------------------------------------------------------------------------
const DoubleSquareMatrix& M::operator=(const DoubleSquareMatrix& v)
{
  _array = v._array;
  _size = v._size;
  return *this;
}
//-------------------------------------------------------------------------
bool M::operator==(const DoubleSquareMatrix& v) const
{ return (_array == v._array); }
//-------------------------------------------------------------------------
bool M::operator!=(const DoubleSquareMatrix& v) const
{ return !(*this == v); }
//-------------------------------------------------------------------------
M::_type& M::operator()(unsigned long i, unsigned long j)
{
  assertIsInBounds(__FILE__, __LINE__, i, _size);
  assertIsInBounds(__FILE__, __LINE__, j, _size);
  return _array[i+j*_size];
}
//-------------------------------------------------------------------------
M::_type M::operator()(unsigned long i, unsigned long j) const
{
  assertIsInBounds(__FILE__, __LINE__, i, _size);
  assertIsInBounds(__FILE__, __LINE__, j, _size);
  return _array[i+j*_size];
}
//-------------------------------------------------------------------------
void M::setSize(const unsigned long size, bool saveMemory)
{
  _size = size;
  _array.setSize(size*size, saveMemory);
}
//-------------------------------------------------------------------------
real_t M::invert(DoubleSquareMatrix& m)
{
  long size = m._size; // unsigned long -> long
  if (size == 0)
    throw Exception("Cannot invert matrix : dimension = 0",__FILE__, __LINE__);
  if (size != (long)_size)
	  throw Exception("Cannot return the invert matrix : dimension not compatible",__FILE__, __LINE__);


  long j, k;
  real_t det;

  DoubleVector diag(_size, _size);
  DoubleVector b(_size, _size);
  DoubleSquareMatrix tmp(*this);
  real_t* pMatrix = m.getArray();
  real_t* pTmp = tmp.getArray();
  real_t* pDiag = diag.getArray();
  real_t* pB = b.getArray();

  // Cholesky decomposition: A = L*trans(L)
  choleskyDecomp(pTmp, pDiag, _size);
  
  det = pDiag[0]*pDiag[0];
  for (k=1; k<size; k++)
    det *= pDiag[k]*pDiag[k];

  // Solve inverse of the matrix by forward and backward substitution.
  // The right hand side is a unit matrix, the solution is thus the
  // inverse of the matrix.
  for (j=0; j<size; j++)
  {
    // one column at a time
    for (k=0; k<size; k++)
      pB[k] = 0.0;
    pB[j] = 1.0;
    choleskySolve(pTmp, pDiag, pB, size);
    unsigned long jsize = j*size;
    for (k=0; k<size; k++)
       pMatrix[k + jsize] = pB[k];
  }
  return det;
}
//-------------------------------------------------------------------------
void M::choleskyDecomp(real_t* pMatrix, real_t* pDiag, long n) // static private
{
  // This function performs Cholesky decomposition.
  // A is a positive-definite symmetric matrix. Only the upper triangle of
  // A is needed on input. On output, the lower triangle of A contains the
  // Cholesky factor L.  The diagonal elements of L are returned in vector
  // diag.

  long i, j, k;
  real_t sum;

  // Cholesky decompose A = L*trans(L)
  for (i=0; i<n; i++)
  {
    unsigned long in = i*n;
    for(j=i; j<n; j++)
    {
      sum = pMatrix[i + j*n];
      for (k=(i-1)*n; k>=0; k-=n)
      	sum -= pMatrix[i + k] * pMatrix[j + k];
      if (i == j)
      {
        if (sum < 0.0)
	        throw Exception("Matrix is not positive definite",
                  __FILE__, __LINE__);
      	pDiag[i] = sqrt(sum);
      }
      else
	      pMatrix[j + in] = sum/pDiag[i];
    }
  }
}
//-------------------------------------------------------------------------
void M::choleskySolve(real_t* pMatrix, real_t* pDiag, real_t* pB, long n) // static private
{
  // Solve linear equation A*x = b, where A positive-definite symmetric.
  // On input, A contains Cholesky factor L in its low triangle except the
  // diagonal elements which are in vector diag.  On return x contains the
  // solution.  b and x can be the same vector to save memory space.

  int i, k;
  real_t sum;

  // solve by forward and backward substitution.  
  // L*y = b
  for (i=0; i<n; i++)
  {
    sum = pB[i];
    for (k=i-1; k>=0; k--)
      sum -= pMatrix[i + k*n] * pB[k];
    pB[i] = sum / pDiag[i];
  }
  // trans(L)*x = y
  for (i=n-1; i>=0; i--)
  {
    sum = pB[i];
    for (k=i+1; k<n; k++)
      sum -= pMatrix[k + i*n] * pB[k];
    pB[i] = sum / pDiag[i];
  }
}
//-------------------------------------------------------------------------
void M::setAllValues(_type v) { _array.setAllValues(v); }
//-------------------------------------------------------------------------
M::_type* M::getArray() const { return _array.getArray(); }
//-------------------------------------------------------------------------
unsigned long M::size() const { return _size; }
//-------------------------------------------------------------------------
String M::getClassName() const { return "DoubleSquareMatrix"; }
//-------------------------------------------------------------------------
String M::toString() const
{
  String s = Object::toString()
    + "\n  size  = " + String::valueOf(_size)+"x"+String::valueOf(_size);
  for (unsigned long i=0; i<_size; i++)
  {
    for (unsigned long j=0; j<_size; j++)
      s += "\n  [" + String::valueOf(j)
        + "," + String::valueOf(i)
        + "] = " + String::valueOf((*this)(j, i));
    s += "\n";
  }
  return s;
}
//-------------------------------------------------------------------------
M::~DoubleSquareMatrix() {}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
real_t M::upperCholesky(DoubleSquareMatrix& m)
{
  long size = m._size; // unsigned long -> long
  if (size == 0)
    throw Exception("Cannot invert matrix : dimension = 0",__FILE__, __LINE__);
  if (size != (long)_size)
	  throw Exception("Cannot return the invert matrix : dimension not compatible",__FILE__, __LINE__);


  DoubleVector diag(_size, _size);
  DoubleVector b(_size, _size);
  DoubleSquareMatrix tmp(*this);
  real_t* pMatrix = m.getArray();
  real_t* pTmp = tmp.getArray();
  real_t* pDiag = diag.getArray();
 
  // Cholesky decomposition: A = L*trans(L)
  choleskyDecomp(pTmp, pDiag, _size);

  //At this point, the lower triangle of pTmp contains the cholesky decomposition
  // Diagonal elements are in pDiag
  
  real_t det = pDiag[0]*pDiag[0];
  for(long k=1;k<size;k++){
	  det *= pDiag[k]*pDiag[k];
  }

  //Fill the upper triangular matrix m
  m.setAllValues(0.0);
  for(long i=0;i<size;i++){
	  for(long j=i+1;j<size;j++){
			m(i,j) = pTmp[i*size+j];
	  }
	  //Fill the diagonal of m with pDiag
	  m(i,i) = pDiag[i];
  }
  return det;
}


#endif  // ALIZE_DoubleSquareMatrix_cpp