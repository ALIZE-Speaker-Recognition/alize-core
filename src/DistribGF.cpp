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

#if !defined(ALIZE_DistribGF_cpp)
#define ALIZE_DistribGF_cpp


#if defined(_WIN32)
  #include <cfloat> // for _isnan()
  #define ISNAN(x) _isnan(x)
//#elif defined(__APPLE__)
//  #define ISNAN(x) std::isnan(x)
#elif defined(linux) || defined(__linux) || defined(__CYGWIN__) || defined(__APPLE__)
  #define ISNAN(x) isnan(x)
#else
  #error "Unsupported OS\n"
#endif

#include <new>
#include <cmath>
#include <cstdlib>
#include <memory.h>
#include "DistribGF.h"
#include "alizeString.h"
#include "Feature.h"
#include "Exception.h"
#include "Config.h"


using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
DistribGF::DistribGF(const unsigned long vectSize)
 :Distrib(vectSize), _covInvMatr(_vectSize),
 _cst(0.0), _tmpVect(_vectSize) {}
//-------------------------------------------------------------------------
DistribGF::DistribGF(const Config& c)
 :Distrib(c.getParam_vectSize()>0?c.getParam_vectSize():1),
 _covInvMatr(_vectSize), _cst(0.0), _tmpVect(_vectSize) {}
//-------------------------------------------------------------------------
void DistribGF::reset() // random init
{
  throw Exception("Do not use this method temporary", __FILE__, __LINE__);
// TODO: HOW TO FILL A POSITIVE-DEFINITE SYMMETRIC MATRIX WITH RANDOM VALUES ?

  //srand(time(NULL));
  /*if (_pCovMatr == NULL)
    _pCovMatr = createMatrix();

  for (unsigned long i=0; i< _vectSize; i++)
  {
    for (unsigned long j=0; j<_vectSize; j++)
    {
      (*_pCovMatr)(i,j) = (rand()+1.0)/(RAND_MAX+1.0); // always > 0.0
    }
    _meanVect[i] = (double)rand()*2/RAND_MAX - 1.0;
  }
  computeAll();*/
}
//-------------------------------------------------------------------------
DistribGF& DistribGF::create(const K&, const unsigned long vectSize)
{
  DistribGF* p = new (std::nothrow) DistribGF(vectSize);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
DistribGF& DistribGF::create(const K&, const Config& c)
{ return create(K::k, c.getParam_vectSize()); }
//-------------------------------------------------------------------------
DistribGF::DistribGF(const DistribGF& d)
:Distrib(d._vectSize), _covMatr(d._covMatr), _covInvMatr(d._covInvMatr),
 _cst(d._cst), _tmpVect(d._vectSize)
{
  _meanVect = d._meanVect;
  _det = d._det;
}
//-------------------------------------------------------------------------
const Distrib& DistribGF::operator=(const Distrib& d) // virtual
{
  const DistribGF* p = dynamic_cast<const DistribGF*>(&d);
  if (p == NULL)
    throw Exception("incompatible distrib", __FILE__, __LINE__);
  return operator=(*p);
}
//-------------------------------------------------------------------------
const DistribGF& DistribGF::operator=(const DistribGF& d)
{
  if (_vectSize != d._vectSize)
    throw Exception("target distrib vectSize ("
        + String::valueOf(_vectSize) + ") != source distrib vectSize ("
        + String::valueOf(d._vectSize) + ")", __FILE__, __LINE__);
  _meanVect = d._meanVect;
  _covInvMatr = d._covInvMatr;
  _covMatr = d._covMatr;
  _det = d._det;
  _cst = d._cst;
  return *this;
}
//-------------------------------------------------------------------------
bool DistribGF::operator==(const Distrib& d) const
{
  const DistribGF* p = dynamic_cast<const DistribGF*>(&d);
  if (p == NULL || _meanVect != p->_meanVect ||
    (_covMatr.size() == 0 && p->_covMatr.size() != 0) ||
    (_covMatr.size() != 0 && p->_covMatr.size() == 0))
    return false;
  if (_covMatr.size() == 0)
    return (_covInvMatr == p->_covInvMatr);
  return _covMatr == p->_covMatr;
}  
//-------------------------------------------------------------------------
DistribGF& DistribGF::duplicate(const K&) const
{ return static_cast<DistribGF&>(clone()); }
//-------------------------------------------------------------------------
Distrib& DistribGF::clone() const // private
{
  DistribGF* p = new (std::nothrow) DistribGF(*this);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
// TODO : A OPTIMISER !!
//-------------------------------------------------------------------------
lk_t DistribGF::computeLK(const Feature& frame) const
{
  if (frame.getVectSize() != _vectSize)
    throw Exception("distrib vectSize ("
        + String::valueOf(_vectSize) + ") != feature vectSize ("
      + String::valueOf(frame.getVectSize()) + ")", __FILE__, __LINE__);

  real_t tmp = 0.0;
  real_t tmp2;
  unsigned long i, j, ii;
  real_t*      m = _meanVect.getArray();
  real_t*      x = _tmpVect.getArray();
  real_t*      c = _covInvMatr.getArray();
  Feature::data_t* f = frame.getDataVector();

  for (j=0; j<_vectSize; j++)
    x[j] = f[j] - m[j];
  for (i=0; i<_vectSize; i++)
  {
    tmp2 = 0.0;
    ii = i*_vectSize;
    for (j=0; j<_vectSize; j++)
      tmp2 += x[j] * c[j+ii];
    tmp += tmp2 * x[i];
  }

  tmp = _cst * exp(-0.5*tmp);
  if (ISNAN(tmp))
    return EPS_LK;
  return tmp;
}
//-------------------------------------------------------------------------
lk_t DistribGF::computeLK(const Feature& frame, unsigned long idx) const
{
  real_t x = frame[idx] - _meanVect[idx];
  real_t tmp = _cst * exp(-0.5 * x * x * _covInvMatr(idx, idx) );
  if (ISNAN(tmp))
    return EPS_LK;
  return tmp;
}
//-------------------------------------------------------------------------
void DistribGF::computeAll()
{
  // compute det and cov inv --------------------------------

  _det = _covMatr.invert(_covInvMatr);

  // compute cst -------------------------------

  if (_det > EPS_LK)
    _cst = 1.0 / ( pow(_det, 0.5) * pow( PI2 , _vectSize/2.0 ) );
  else
    _cst = 1.0 / ( pow(EPS_LK, 0.5) * pow( PI2 , _vectSize/2.0 ) );

  // remove cov matrix
  _covMatr.setSize(0, true);
}
//-------------------------------------------------------------------------
void DistribGF::setCov(real_t v, unsigned long col, unsigned long row)
{
  _covMatr.setSize(_vectSize);
  if (v < MIN_COV)
    v = MIN_COV;
  _covMatr(col, row) = v;
}
//-------------------------------------------------------------------------
void DistribGF::setCovInv(const K&, const real_t v, const unsigned long col,
                                                   const  unsigned long row)
{ _covInvMatr(col, row) = v; }
//-------------------------------------------------------------------------
real_t DistribGF::getCov(unsigned long col, unsigned long row) const
{
  _covMatr.setSize(_vectSize);
  return _covMatr(col, row);
}
//-------------------------------------------------------------------------
real_t DistribGF::getCovInv(const unsigned long col,
                            const unsigned long row) const
{ return _covInvMatr(col, row); }
//-------------------------------------------------------------------------
DoubleSquareMatrix& DistribGF::getCovInvMatrix() {return _covInvMatr;}
//-------------------------------------------------------------------------
const DoubleSquareMatrix& DistribGF::getCovInvMatrix() const {return _covInvMatr;}
//-------------------------------------------------------------------------
DoubleSquareMatrix& DistribGF::getCovMatrix() { return _covMatr; }
//-------------------------------------------------------------------------
const DoubleSquareMatrix& DistribGF::getCovMatrix() const { return _covMatr; }
//-------------------------------------------------------------------------
String DistribGF::getClassName() const { return "DistribGF"; }
//-------------------------------------------------------------------------
String DistribGF::toString() const
{
  String s = Object::toString()
  + "\n  vectSize  = " + String::valueOf(_vectSize)
  + "\n  det     = " + String::valueOf(_det)
  + "\n  cst     = " + String::valueOf(_cst);
  
  
  if (_covMatr.size() != 0)
    for (unsigned long i=0; i<_vectSize; i++)
      for (unsigned long j=0; j<_vectSize; j++)
        s += "\n  cov["+String::valueOf(i)+","+String::valueOf(j)+"] = "
          + String::valueOf(_covMatr(i,j))
          + "  covInv["+String::valueOf(i)+","+String::valueOf(j)+"] = "
          + String::valueOf(_covInvMatr(i,j));
  else
    for (unsigned long i=0; i<_vectSize; i++)
      for (unsigned long j=0; j<_vectSize; j++)
        s += "\n  covInv["+String::valueOf(i)+","+String::valueOf(j)+"] = "
          + String::valueOf(_covInvMatr(i,j));
    for (unsigned long i=0; i<_vectSize; i++)
        s += "\n  mean[" + String::valueOf(i) + "] = "
          + String::valueOf(_meanVect[i]);
  return s;
}
//-------------------------------------------------------------------------
DistribGF::~DistribGF() {}
//-------------------------------------------------------------------------
#endif // !defined(ALIZE_DistribGF_cpp)

