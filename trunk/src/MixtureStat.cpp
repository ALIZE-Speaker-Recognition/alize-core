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

#if !defined(ALIZE_MixtureStat_cpp)
#define ALIZE_MixtureStat_cpp

//#include <cmath>
#include "MixtureStat.h"
#include "alizeString.h"
#include "Mixture.h"
#include "Distrib.h"
#include "Exception.h"
#include "Feature.h"
#include "Config.h"
#include "RealVector.h"
#include "StatServer.h"

using namespace alize;
typedef MixtureStat S;
//-------------------------------------------------------------------------
S::MixtureStat(StatServer& ss, const Mixture& m, const Config& c)
:Object(), _distribCount(m.getDistribCount()), _pMixture(&m), _config(c), 
 _minLLK(c.getParam_minLLK()), _maxLLK(c.getParam_maxLLK()), _llk(0),
 _accumulatedLLK(0), _occVect(_distribCount, _distribCount),
 _accumulatedOccVect(_distribCount, _distribCount),
 _meanOccVect(_distribCount, _distribCount), _resetedEM(false),
 _pStatServer(&ss), _featureCounterForEM(0.0)
{
  resetLLK();
  resetOcc();
}
//-------------------------------------------------------------------------
Mixture& S::getMixture() const { return const_cast<Mixture&>(*_pMixture); }
//-------------------------------------------------------------------------
lk_t S::computeLLK(const Feature& f)
{ return _llk =  _pStatServer->computeLLK(*_pMixture, f); }
//-------------------------------------------------------------------------
lk_t S::computeLLK(const Feature& f, unsigned long idx)
{ return _llk =  _pStatServer->computeLLK(*_pMixture, f, idx); }
//-------------------------------------------------------------------------
lk_t S::getLLK() const { return _llk; }
//-------------------------------------------------------------------------
void S::resetLLK()
{
  _accumulatedLLK = 0.0;
  _featureCounterForAccumulatedLK = 0.0;
}
//-------------------------------------------------------------------------
lk_t S::accumulateLLK(lk_t llk, double w)
{
  _accumulatedLLK += (_llk = llk*w);
  _featureCounterForAccumulatedLK += w;
  return llk;
}
//-------------------------------------------------------------------------
lk_t S::computeAndAccumulateLLK()
{
  lk_t llk = _pStatServer->computeLLK(K::k, *_pMixture);
  return accumulateLLK(llk, 1.0);
}
//-------------------------------------------------------------------------
lk_t S::computeAndAccumulateLLK(const Feature& f, double w,
                                const TopDistribsAction& a)
{
  lk_t llk = _pStatServer->computeLLK(K::k, *_pMixture, f, a);
  return accumulateLLK(llk, w);
}
//-------------------------------------------------------------------------
lk_t S::computeAndAccumulateLLK(const Feature& f, 
		const LKVector& topDistribsVector, double w)
{
  lk_t llk = _pStatServer->computeLLK(K::k, *_pMixture, f, topDistribsVector);
  return accumulateLLK(llk, w);
}
//-------------------------------------------------------------------------
lk_t S::getAccumulatedLLK() const { return _accumulatedLLK; }
//-------------------------------------------------------------------------
lk_t S::getMeanLLK() const
{
  if (_featureCounterForAccumulatedLK == 0.0)
    throw Exception("No features -> no mean", __FILE__, __LINE__);
  return _accumulatedLLK/_featureCounterForAccumulatedLK;
}
//-------------------------------------------------------------------------
//const DoubleVector& S::getDistribLKVect() { return _distribLKVect; }
//-------------------------------------------------------------------------
void S::resetOcc()
{
  _accumulatedOccVect.setAllValues(0.0);
  _featureCounterForAccumulatedOcc = 0.0;
}
//-------------------------------------------------------------------------
real_t S::computeAndAccumulateOcc(const Feature& f, weight_t w)
{
  real_t sum = computeOccVect(f);
  _accumulatedOccVect += (_occVect *= w);
  _featureCounterForAccumulatedOcc += w;
  return sum;
}
//-------------------------------------------------------------------------
// calcule la contribution de la trame à chaque distribution de la mixture
// -> _occVect[nb distrib]
// 0 < occ(distrib) <= 1
//-------------------------------------------------------------------------
real_t S::computeOccVect(const Feature& f) // private
{
  // source : Amiral AppMM_IterationApp.c ContributionTrame(...)

  // EPS_APP : Utilise pour tester si une trame a un poids total
  // non negligeable

  const real_t EPS_APP = 1e-200;

  occ_t sum = 0.0;
  unsigned long c;
  weight_t* weightVect  = _pMixture->getTabWeight().getArray();
  Distrib** distribVect = _pMixture->getTabDistrib();
  occ_t*  occVect   = _occVect.getArray(); 

  for (c=0; c<_distribCount; c++)
  {
    Distrib* d = distribVect[c];
    occVect[c] = weightVect[c] * d->computeLK(f);
    sum += _occVect[c];
  }
  if (sum > EPS_APP) /* si la trame a un poids non negligeable */
  {
    for (c=0; c<_distribCount; c++)
    { _occVect[c] /= sum; } /* normalisation   Somme des occ = 1 */
  }
  else /* si la trame a un poids negligeable */
  {
    for (c=0; c<_distribCount; c++)
    {
      //if (_occVect[c] != 0.0)
        _occVect[c] = EPS_APP;
    }
    sum  = EPS_APP;
  }
  return sum;
}
//-------------------------------------------------------------------------
DoubleVector& S::getOccVect() { return _occVect; }
//-------------------------------------------------------------------------
const DoubleVector& S::getOccVect() const { return _occVect; }
//-------------------------------------------------------------------------
const DoubleVector& S::getAccumulatedOccVect() const
{ return _accumulatedOccVect; }
//-------------------------------------------------------------------------
DoubleVector& S::getAccumulatedOccVect() { return _accumulatedOccVect; }
//-------------------------------------------------------------------------
occ_t S::getAccumulatedOcc()
{ return _accumulatedOccVect.computeSum(); }
//-------------------------------------------------------------------------
real_t S::getAccumulatedOccFeatureCount() const
{ return _featureCounterForAccumulatedOcc; }
//-------------------------------------------------------------------------
double S::getAccumulatedLLKFeatureCount() const
{ return _featureCounterForAccumulatedLK; }
//-------------------------------------------------------------------------
real_t S::getEMFeatureCount() const
{ return _featureCounterForEM; }
//-------------------------------------------------------------------------
DoubleVector& S::getMeanOccVect()
{
  if (_featureCounterForAccumulatedOcc == 0.0)
    throw Exception("no occ accumulated", __FILE__, __LINE__);

  occ_t* meanOccVect = _meanOccVect.getArray();
  for (unsigned long c=0; c<_distribCount; c++)
    meanOccVect[c] = _accumulatedOccVect[c]
             / _featureCounterForAccumulatedOcc;
  return _meanOccVect;
}
//-------------------------------------------------------------------------
void S::assertResetEMDone() const
{
  if (!_resetedEM)
    throw Exception("EM not reseted", __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
String S::toString() const
// TODO : a completer
{
  return Object::toString()
    + "\n  id        = '" + _pMixture->getId() + "'"
    + "\n  llk       = " + String::valueOf(_llk)
    + "\n  accum llk     = " + String::valueOf(_accumulatedLLK)
    + "\n  feature counter = "
            + String::valueOf(_featureCounterForAccumulatedLK);
}
//-------------------------------------------------------------------------
S::~MixtureStat() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureStat_cpp)
