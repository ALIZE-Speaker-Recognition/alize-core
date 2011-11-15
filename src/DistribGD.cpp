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

#if !defined(ALIZE_DistribGD_cpp)
#define ALIZE_DistribGD_cpp

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
#include "DistribGD.h"
#include "alizeString.h"
#include "Feature.h"
#include "Exception.h"
#include "Config.h"

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
DistribGD::DistribGD(unsigned long vectSize)
 :Distrib(vectSize), _covInvVect(_vectSize, _vectSize)
{ reset(); }
//-------------------------------------------------------------------------
DistribGD::DistribGD(const Config& c)
 :Distrib(c.getParam_vectSize()>0?c.getParam_vectSize():1),
 _covInvVect(_vectSize, _vectSize) { reset(); }
//-------------------------------------------------------------------------
void DistribGD::reset() // random init
{
  //srand(time(NULL));
  _covVect.setSize(_vectSize);

  for (unsigned long i=0; i< _vectSize; i++)
  {
    _covVect[i] = (rand()+1.0)/(RAND_MAX+1.0); // always > 0.0
    _meanVect[i] = (double)rand()*2/RAND_MAX - 1.0;
  }
  computeAll();
}
//-------------------------------------------------------------------------
DistribGD& DistribGD::create(const K&, unsigned long vectSize)
{
  DistribGD* p = new (std::nothrow) DistribGD(vectSize);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
DistribGD& DistribGD::create(const K&, const Config& c)
{ return create(K::k, c.getParam_vectSize()); }
//-------------------------------------------------------------------------
DistribGD::DistribGD(const DistribGD& d)
:Distrib(d._vectSize), _covVect(d._covVect), _covInvVect(d._covInvVect)
{
  _meanVect = d._meanVect;
  _det = d._det;
  _cst = d._cst;
}
//-------------------------------------------------------------------------
const Distrib& DistribGD::operator=(const Distrib& d) // virtual
{
  const DistribGD* p = dynamic_cast<const DistribGD*>(&d);
  if (p == NULL)
    throw Exception("incompatible distrib", __FILE__, __LINE__);
  return operator=(*p);
}
//-------------------------------------------------------------------------
const DistribGD& DistribGD::operator=(const DistribGD& d)
{
  if (_vectSize != d.getVectSize())
    throw Exception("target distrib vectSize ("
        + String::valueOf(_vectSize) + ") != source distrib vectSize ("
        + String::valueOf(d._vectSize) + ")", __FILE__, __LINE__);
  _meanVect = d._meanVect;
  _covInvVect = d._covInvVect;
  _covVect = d._covVect;
  _det = d._det;
  _cst = d._cst;
  return *this;
}
//-------------------------------------------------------------------------
bool DistribGD::operator==(const Distrib& d) const
{
  const DistribGD* p = dynamic_cast<const DistribGD*>(&d);
  return (p != NULL && _meanVect == p->_meanVect &&
      _covInvVect == p->_covInvVect);
}  
//-------------------------------------------------------------------------
DistribGD& DistribGD::duplicate(const K&) const
{ return static_cast<DistribGD&>(clone()); }
//-------------------------------------------------------------------------
Distrib& DistribGD::clone() const // private
{
  DistribGD* p = new (std::nothrow) DistribGD(*this);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
// TODO : A OPTIMISER !!
//-------------------------------------------------------------------------
lk_t DistribGD::computeLK(const Feature& frame) const
{
  if (frame.getVectSize() != _vectSize)
    throw Exception("distrib vectSize ("
        + String::valueOf(_vectSize) + ") != feature vectSize ("
      + String::valueOf(frame.getVectSize()) + ")", __FILE__, __LINE__);
  real_t tmp = 0.0;
  real_t*      m = _meanVect.getArray();
  real_t*      c = _covInvVect.getArray();
  Feature::data_t* f = frame.getDataVector();

  for (unsigned long i=0; i<_vectSize; i++)
    tmp += (f[i] - m[i]) * (f[i] - m[i]) * c[i];

  tmp = _cst * exp(-0.5*tmp);
  if (ISNAN(tmp))
    return EPS_LK;
  return tmp;
}
//-------------------------------------------------------------------------
lk_t DistribGD::computeLK(const Feature& frame, unsigned long i) const
{
  real_t fm = frame[i] - _meanVect[i];
  real_t tmp = _cst * exp(-0.5 * fm * fm * _covInvVect[i]);
  if (ISNAN(tmp))
    return EPS_LK;
  return tmp;
}
//-------------------------------------------------------------------------
void DistribGD::computeAll()
{
  real_t* vect = getCovVect().getArray();
  assert(vect != NULL);
  unsigned long i;

   // compute det --------------------------------

  _det = 1.0;
  for (i=0; i< _vectSize; i++)
  {
     _det *= vect[i];
  }

  // compute covInv ----------------------------

  for (i=0; i< _vectSize; i++)
  {
  	if( vect[i] == 0.0) { throw Exception( "Assertion 'vect[i] != 0.0' - Can't invert covariance vector - This error is mainly due to bad parametric data !", __FILE__, __LINE__) ; }
    _covInvVect[i] = 1.0/vect[i];
  }

  // compute cst -------------------------------

  if (_det > EPS_LK)
    _cst = 1.0 / ( pow(_det, 0.5) * pow( PI2 , _vectSize/2.0 ) );
  else
    _cst = 1.0 / ( pow(EPS_LK, 0.5) * pow( PI2 , _vectSize/2.0 ) );

  //
  _covVect.setSize(0, true); // set capacity to 0 too
}
//-------------------------------------------------------------------------
void DistribGD::setCov(real_t v, unsigned long i)
{
  if (v < MIN_COV)
    getCovVect()[i] = MIN_COV;
  else
    getCovVect()[i] = v;
}
//-------------------------------------------------------------------------
void DistribGD::setCovInv(const K&, real_t v, unsigned long i)
{ _covInvVect[i] = v; }
//-------------------------------------------------------------------------
real_t DistribGD::getCov(unsigned long i)
{ return getCovVect()[i];}
//-------------------------------------------------------------------------
real_t DistribGD::getCov(unsigned long i) const 
{ return getCovVect()[i];}
//-------------------------------------------------------------------------
real_t DistribGD::getCovInv(unsigned long i) const {return _covInvVect[i];}
//-------------------------------------------------------------------------
DoubleVector& DistribGD::getCovInvVect() { return _covInvVect; }
//-------------------------------------------------------------------------
const DoubleVector& DistribGD::getCovInvVect() const { return _covInvVect; }
//-------------------------------------------------------------------------
DoubleVector& DistribGD::getCovVect()
{
  return const_cast<DoubleVector&>(
          const_cast<const DistribGD*>(this)->getCovVect());
}
//-------------------------------------------------------------------------
const DoubleVector& DistribGD::getCovVect() const
{
  if (_covVect.size() != _vectSize)
  {
    _covVect.setSize(_vectSize);
    for (unsigned long i=0; i< _vectSize; i++)
    {
      if (_covInvVect[i] < 1.0/MIN_COV)
        _covVect[i] = 1.0/_covInvVect[i];
      else
        _covVect[i] = MIN_COV;
    }
  }
  return _covVect;
}

//-------------------------------------------------------------------------
String DistribGD::getClassName() const { return "DistribGD"; }
//-------------------------------------------------------------------------
String DistribGD::toString() const
{
  String s = Object::toString()
  + "\n  vectSize  = " + String::valueOf(_vectSize)
  + "\n  det     = " + String::valueOf(_det)
  + "\n  cst     = " + String::valueOf(_cst);
  
  
  if (_covVect.size() != 0)
    for (unsigned long i=0; i<_vectSize; i++)
    {
      s += "\n  cov[" + String::valueOf(i) + "] = "
        + String::valueOf(_covVect[i])
        + "  covInv[" + String::valueOf(i) + "] = "
        + String::valueOf(_covInvVect[i])
        + "  mean[" + String::valueOf(i) + "] = "
        + String::valueOf(_meanVect[i]);
    }
  else
    for (unsigned long i=0; i<_vectSize; i++)
    {
      s += "\n  covInv[" + String::valueOf(i) + "] = "
        + String::valueOf(_covInvVect[i])
        + "  mean[" + String::valueOf(i) + "] = "
        + String::valueOf(_meanVect[i]);
    }
  return s;
}
//-------------------------------------------------------------------------
DistribGD::~DistribGD() {}
//-------------------------------------------------------------------------
#endif // !defined(ALIZE_DistribGD_cpp)

