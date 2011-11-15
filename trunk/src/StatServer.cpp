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

#if !defined(ALIZE_StatServer_cpp)
#define ALIZE_StatServer_cpp

#include <new>
#include <cmath> // for log
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

#include "MixtureServer.h"
#include "StatServer.h"
#include "MixtureStat.h"
#include "MixtureGDStat.h"
#include "MixtureGFStat.h"
#include "Mixture.h"
#include "Exception.h"
#include "Config.h"
#include "RealVector.h"
#include "ULongVector.h"
#include "ViterbiAccum.h"
#include "FrameAccGD.h"
#include "FrameAccGF.h"

using namespace alize;
using namespace std;

#include <cstdio>
#include <iostream>
using namespace std;


typedef StatServer S;
//-------------------------------------------------------------------------
S::StatServer(const Config& c)
:Object(), _config(c), _pMixtureServer(NULL), 
_topDistribsVect(0, 0), _minLLK(c.getParam_minLLK()), 
_maxLLK(c.getParam_maxLLK()){ 
	reset(); 
	}
//-------------------------------------------------------------------------
S::StatServer(const Config& c, MixtureServer& ms)
:Object(), _config(c), _pMixtureServer(&ms),
 _topDistribsVect(0, 0), _minLLK(c.getParam_minLLK()),
_maxLLK(c.getParam_maxLLK())

{ reset(); }
//-------------------------------------------------------------------------
void S::reset()
{
  _mixtureStatVect.deleteAllObjects();
  _viterbiAccumVect.deleteAllObjects();
  _pLastMixture = NULL;
  _pLastMixtureStat = NULL;
  _topDistribsVect.clear();
}
//-------------------------------------------------------------------------
real_t S::getAccumulatedOccFeatureCount(const Mixture& m)
{ return getMixtureStat(m).getAccumulatedOccFeatureCount(); }
//-------------------------------------------------------------------------
void S::resetLLK(const Mixture& m) { getMixtureStat(m).resetLLK(); }
//-------------------------------------------------------------------------
lk_t S::computeAndAccumulateLLK(const Mixture& m, const Feature& f,
                                const TopDistribsAction& a)
{ return computeAndAccumulateLLK(m, f, (double)1.0f, a); }
//-------------------------------------------------------------------------
lk_t S::computeAndAccumulateLLK(const Mixture& m, const Feature& f,
                                double w, const TopDistribsAction& a)
{ return getMixtureStat(m).computeAndAccumulateLLK(f, w, a); }
//-------------------------------------------------------------------------
void S::accumulateLLK(const Mixture& m, lk_t v, double w)
{ getMixtureStat(m).accumulateLLK(v, w); }
//-------------------------------------------------------------------------
const LKVector& S::getTopDistribIndexVector() const
{ return _topDistribsVect; }
//-------------------------------------------------------------------------
const DoubleVector& S::getDistribLKVector(const K&) const
{ return _distribLKVect; }
//-------------------------------------------------------------------------
LKVector& S::getTopDistribIndexVector(const K&) { return _topDistribsVect; }
//-------------------------------------------------------------------------
lk_t S::computeAndAccumulateLLK(const Mixture& m)
{ return getMixtureStat(m).computeAndAccumulateLLK(); }
//-------------------------------------------------------------------------
lk_t S::getLLK(const Mixture& m) { return getMixtureStat(m).getLLK(); }
//-------------------------------------------------------------------------
lk_t S::getMeanLLK(const Mixture& m) { return getMixtureStat(m).getMeanLLK(); }
//-------------------------------------------------------------------------
lk_t S::computeLLK(const Mixture& m, const Feature& f) const
{
  lk_t lk = 0.0;
  weight_t*  w = m.getTabWeight().getArray();
  Distrib**  d = m.getTabDistrib();
  unsigned long distribCount = m.getDistribCount();
  for (unsigned long c=0; c<distribCount; c++) {
    lk += w[c] * d[c]->computeLK(f);
  }
  return computeLLK(lk);
}
//-------------------------------------------------------------------------
lk_t S::computeLLK(const Mixture& m, const Feature& f, unsigned long idx) const
{
  lk_t lk = 0.0;
  weight_t*  w = m.getTabWeight().getArray();
  Distrib**  d = m.getTabDistrib();
  unsigned long distribCount = m.getDistribCount();
  for (unsigned long c=0; c<distribCount; c++)
    lk += w[c] * d[c]->computeLK(f, idx);
  return computeLLK(lk);
}
//-------------------------------------------------------------------------
lk_t S::computeLLK(const K&, const Mixture& m) const
{
  const weight_t* weightVect  = m.getTabWeight().getArray();
  Distrib** distribVect = m.getTabDistrib();
  const lk_t* lkVect = _distribLKVect.getArray();
  unsigned long distribCount = m.getDistribCount();

  lk_t lk = 0.0;
  for (unsigned long c=0; c<distribCount; c++)
    lk += lkVect[distribVect[c]->dictIndex(K::k)] * weightVect[c];
  return computeLLK(lk);
}
//-------------------------------------------------------------------------
lk_t S::computeLLK(lk_t lk) const // private
{
  if ( ISNAN(lk) || lk == 0 || lk<_minLLK )
    lk = _minLLK;
  else
  {
   lk = log(lk);
    if (lk > _maxLLK)
      lk = _maxLLK;
  }
  return lk;
}
//-------------------------------------------------------------------------
lk_t S::computeLLK(const K&, const Mixture& m, const Feature& f,
                   const TopDistribsAction& a)
{
  if (a == TOP_DISTRIBS_NO_ACTION)
    return computeLLK(m, f);

  LKVector& lkVect = _topDistribsVect;
  lk_t lk = 0.0;
  weight_t* w = m.getTabWeight().getArray();
  Distrib** d = m.getTabDistrib();
  unsigned long distribCount = m.getDistribCount();
  unsigned long c, i, nTop = _config.getParam_topDistribsCount();

  if (a == USE_TOP_DISTRIBS)
  {
    if (nTop >= distribCount)
      nTop = distribCount;
    if (distribCount != lkVect.size()) // TODO gerer ce cas
      throw Exception("", __FILE__, __LINE__);
    LKVector::type* v = lkVect.getArray();
    real_t sumTopDistribWeights = 0.0;

    for (i=0; i<nTop; i++)
    {
      c = v[i].idx;
      sumTopDistribWeights += w[c];
      //lk += w[c] * d[c]->computeLK(f);
      lk +=(v[c].lk =(w[c] * d[c]->computeLK(f)));
    }
    if (_config.getParam_computeLLKWithTopDistribs()) // COMPLETE
      lk += lkVect.sumNonTopDistribLK *
          (1.0 - sumTopDistribWeights) / lkVect.sumNonTopDistribWeights;
    else // PARTIAL
      if (nTop != 0)
        lk /= sumTopDistribWeights;
    return computeLLK(lk);
  }
  // a == DETERMINE_TOP_DISTRIBS
  lkVect.setSize(distribCount);
  LKVector::type* v = lkVect.getArray();
  lkVect.topDistribsCount = nTop;

  for (c=0; c<distribCount; c++)
  {
    v[c].idx = c;
    lk += (v[c].lk = w[c] * d[c]->computeLK(f));
  }
  lkVect.descendingSort();
  //
  if (_config.getParam_computeLLKWithTopDistribs() == true) // COMPLETE
  {
    real_t sumTopDistribWeights = 0.0;
    real_t sumTopDistribLK = 0.0;
    for (i=0; i<nTop; i++)
    {
      sumTopDistribWeights += w[v[i].idx];
      sumTopDistribLK += v[i].lk;
    }
    lkVect.sumNonTopDistribWeights = 1.0 - sumTopDistribWeights;
    lkVect.sumNonTopDistribLK = lk - sumTopDistribLK;
    if (lkVect.sumNonTopDistribLK < EPS_LK)
      lkVect.sumNonTopDistribLK = EPS_LK;
  }
  return computeLLK(lk);
}
//-------------------------------------------------------------------------
lk_t S::computeLLK(const K&, const Mixture& m, const Feature& f,
                   const LKVector& lkVect)
{
  lk_t lk = 0.0;
  weight_t* w = m.getTabWeight().getArray();
  Distrib** d = m.getTabDistrib();
  unsigned long distribCount = m.getDistribCount();
  unsigned long c, i, nTop = _config.getParam_topDistribsCount();


  if (nTop >= distribCount)
    nTop = distribCount;
  LKVector::type* v = lkVect.getArray();
  real_t sumTopDistribWeights = 0.0;

  for (i=0; i<nTop; i++)
  {
    c = v[i].idx;
    sumTopDistribWeights += w[c];
    lk += w[c] * d[c]->computeLK(f);
  }
  if (_config.getParam_computeLLKWithTopDistribs()) {// COMPLETE
    lk += lkVect.sumNonTopDistribLK *
        (1.0 - sumTopDistribWeights) / lkVect.sumNonTopDistribWeights;
  }
/*  else 
	  {// PARTIAL
    if (nTop != 0)
      lk /= sumTopDistribWeights;
//	cerr << "PARTIAL" << endl;
  }*/
  return computeLLK(lk);
}
//-------------------------------------------------------------------------
void S::computeAllDistribLK(const Feature& f)
{
  if (_pMixtureServer == NULL)
    throw Exception("No mixture server connected to this stat server"
        , __FILE__, __LINE__);
  _distribLKVect.clear();
  unsigned long n = _pMixtureServer->getDistribCount();
  for (unsigned long i=0; i<n; i++)
    _distribLKVect.addValue(_pMixtureServer->getDistrib(i).computeLK(f));
}
//-------------------------------------------------------------------------
void S::resetOcc(const Mixture& m) { getMixtureStat(m).resetOcc(); }
//-------------------------------------------------------------------------
real_t S::computeAndAccumulateOcc(const Mixture& m, const Feature& f)
{ return getMixtureStat(m).computeAndAccumulateOcc(f); }
//-------------------------------------------------------------------------
occ_t* S::getMeanOccVect(const Mixture& m)
{ return getMixtureStat(m).getMeanOccVect().getArray(); }
//-------------------------------------------------------------------------
occ_t* S::getAccumulatedOccVect(const Mixture& m)
{ return getMixtureStat(m).getAccumulatedOccVect().getArray(); }
//-------------------------------------------------------------------------
occ_t S::getAccumulatedOcc(const Mixture& m)
{ return getMixtureStat(m).getAccumulatedOcc(); }
//-------------------------------------------------------------------------
void S::setTopDistribIndexVector(const ULongVector& indexVect,
                                  real_t w, real_t l)
{
  unsigned long topDistribCount = indexVect.size();
  if (topDistribCount>_topDistribsVect.size()) // TODO gerer ce cas
    throw Exception("", __FILE__, __LINE__);
  LKVector::type* v = _topDistribsVect.getArray();
  for (unsigned long i=0; i<topDistribCount; i++)
    v[i].idx = indexVect[i];
  _topDistribsVect.sumNonTopDistribWeights = w;
  _topDistribsVect.sumNonTopDistribLK = l;
}
//-------------------------------------------------------------------------
MixtureStat& S::getMixtureStat(const Mixture& m) // private
{
  // TODO : not optimised...
  if (&m == _pLastMixture)
  {
    assert(_pLastMixtureStat != NULL);
    return *_pLastMixtureStat;
  }
  const unsigned long n = _mixtureStatVect.size();
  for (unsigned long i=0; i<n; i++)
  {
    MixtureStat& ms = _mixtureStatVect.getObject(i);
    if (ms.getMixture().isSameObject(m))
    {
      _pLastMixture = &m;
      _pLastMixtureStat = &ms;
      return ms;
    }
  }
  return createAndStoreMixtureStat(m);
}
//-------------------------------------------------------------------------
MixtureStat& S::createAndStoreMixtureStat(const Mixture& m)
{
  MixtureStat& ms = m.createNewMixtureStatObject(K::k,*this,  _config);
  _mixtureStatVect.addObject(ms);
  _pLastMixture = &m;
  _pLastMixtureStat = &ms;
  return ms;
}
//-------------------------------------------------------------------------
MixtureGDStat& S::createAndStoreMixtureStat(MixtureGD& m)
{
  return static_cast<MixtureGDStat&>(
    createAndStoreMixtureStat(static_cast<Mixture&>(m)));
}
//-------------------------------------------------------------------------
MixtureGFStat& S::createAndStoreMixtureStat(MixtureGF& m)
{
  return static_cast<MixtureGFStat&>(
    createAndStoreMixtureStat(static_cast<Mixture&>(m)));
}
//-------------------------------------------------------------------------
MixtureGDStat& S::createAndStoreMixtureGDStat(Mixture& m)
{
  MixtureGD* p = dynamic_cast<MixtureGD*>(&m);
  if (p == NULL)
    throw Exception("Wrong mixture type", __FILE__, __LINE__);
  return createAndStoreMixtureStat(*p);
}
//-------------------------------------------------------------------------
MixtureGFStat& S::createAndStoreMixtureGFStat(Mixture& m)
{
  MixtureGF* p = dynamic_cast<MixtureGF*>(&m);
  if (p == NULL)
    throw Exception("Wrong mixture type", __FILE__, __LINE__);
  return createAndStoreMixtureStat(*p);
}
//-------------------------------------------------------------------------
unsigned long S::getMixtureStatCount() const
{ return _mixtureStatVect.size(); }
//-------------------------------------------------------------------------
MixtureStat& S::getMixtureStat(unsigned long idx)
{ return _mixtureStatVect.getObject(idx); }
//-------------------------------------------------------------------------
MixtureGDStat& S::getMixtureGDStat(unsigned long idx)
{
  MixtureGDStat* p = dynamic_cast<MixtureGDStat*>(
                                    &_mixtureStatVect.getObject(idx));
  if (p == NULL)
    throw Exception("No mixtureGDStat object for index "+String::valueOf(idx),
                    __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
MixtureGFStat& S::getMixtureGFStat(unsigned long idx)
{
  MixtureGFStat* p = dynamic_cast<MixtureGFStat*>(
                                    &_mixtureStatVect.getObject(idx));
  if (p == NULL)
    throw Exception("No mixtureGFStat object for index "+String::valueOf(idx),
                    __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void S::deleteMixtureStat(MixtureStat& m)
{
  delete &_mixtureStatVect.removeObject(m);
  _pLastMixtureStat = NULL;
}
//-------------------------------------------------------------------------
void S::deleteMixtureStat(unsigned long b, unsigned long e)
{
  _mixtureStatVect.removeObjects(b, e, DELETE);
  _pLastMixtureStat = NULL;
}
//-------------------------------------------------------------------------
void S::deleteAllMixtureStat()
{
  _mixtureStatVect.deleteAllObjects();
  _pLastMixtureStat = NULL;
}
//-------------------------------------------------------------------------
unsigned long S::getMixtureStatIndex(MixtureStat& m) const
{ return _mixtureStatVect.getObjectIndex(m); }
//-------------------------------------------------------------------------


// EM


//-------------------------------------------------------------------------
void S::resetEM(const Mixture& m)
{ getMixtureStat(m).resetEM(); }
//-------------------------------------------------------------------------
occ_t S::computeAndAccumulateEM(const Mixture& m, const Feature& f)
{ return getMixtureStat(m).computeAndAccumulateEM(f); }
//-------------------------------------------------------------------------
const Mixture& S::getEM(const Mixture& m)
{ return getMixtureStat(m).getEM(); }
//-------------------------------------------------------------------------


// VITERBI

//-------------------------------------------------------------------------
ViterbiAccum& S::createViterbiAccum()
{
  ViterbiAccum& va = ViterbiAccum::create(*this, _config, K::k);
  _viterbiAccumVect.addObject(va);
  return va;
}

// FrameAcc

//-------------------------------------------------------------------------
FrameAccGD S::createFrameAccGD()
{ return FrameAccGD(); }
//-------------------------------------------------------------------------
FrameAccGF S::createFrameAccGF()
{ return FrameAccGF(); }
//-------------------------------------------------------------------------


// OTHER STUFF


//-------------------------------------------------------------------------
const String& S::getServerName() const { return _serverName; }
//-------------------------------------------------------------------------
void S::setServerName(const String& s) { _serverName = s; }
//-------------------------------------------------------------------------
String S::getClassName() const { return "StatServer"; }
//-------------------------------------------------------------------------
String S::toString() const
{
  String s(Object::toString()
    + "\n  serverName = '" + _serverName + "'");
  if (_pMixtureServer != NULL)
    return s
    + "\n  mixture server = [ " + _pMixtureServer->getClassName()
    + " " + _pMixtureServer->getAddress() + " ] name = '"
    + _pMixtureServer->getServerName() + "'";
  else
    return s + "\n  mixture server = [NULL]";
}
//-------------------------------------------------------------------------
S::~StatServer()
{
  //_mixtureStatVect.deleteAllObjects();
  _viterbiAccumVect.deleteAllObjects();
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_StatServer_cpp)
