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

#if !defined(ALIZE_MixtureGDStat_cpp)
#define ALIZE_MixtureGDStat_cpp

#include <new>
#include "MixtureGDStat.h"
#include "alizeString.h"
#include "Feature.h"
#include "DistribGD.h"
#include "Mixture.h"
#include "MixtureGD.h"
#include "DistribRefVector.h"
#include "Config.h"
#include "Exception.h"

using namespace alize;
typedef MixtureGDStat M;

//-------------------------------------------------------------------------
M::MixtureGDStat(const K&, StatServer& ss, const MixtureGD& m, const Config& c)
:MixtureStat(ss, m, c), _pMixForAccumulation(NULL), _pMixtureForEM(NULL) {}
//-------------------------------------------------------------------------
MixtureGDStat& M::create(const K&, StatServer& ss,
                                     const MixtureGD& m, const Config& c)
{
  MixtureGDStat* p = new (std::nothrow) MixtureGDStat(K::k, ss, m, c);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void M::resetEM()
{
  assert(_pMixture->getDistribCount() == _distribCount);
  resetOcc();

  // copy the original mixture and its ditributions
  if (_pMixtureForEM != NULL)
    delete _pMixtureForEM;
  _pMixtureForEM = &static_cast<MixtureGD&>(_pMixture->duplicate(K::k,
                                            DUPL_DISTRIB));
  // create and initialize a temporary mixtureGD to accumulate cov and mean
  if (_pMixForAccumulation != NULL)
    delete _pMixForAccumulation;
  _pMixForAccumulation = &MixtureGD::create(K::k, "",
                      _pMixtureForEM->getVectSize(),
                      _pMixtureForEM->getDistribCount());
  for (unsigned long c=0; c<_distribCount; c++)
  {
    DistribGD& d = _pMixForAccumulation->getDistrib(c);
    d.getMeanVect().setAllValues(0.0);
    d.getCovVect().setAllValues(0.0);
  }
  _featureCounterForEM = 0.0;
  _resetedEM = true;
}
//-------------------------------------------------------------------------
occ_t M::computeAndAccumulateEM(const Feature& f, double w)
{
  assertResetEMDone();
  real_t sum = computeAndAccumulateOcc(f, w);

  Feature::data_t* dataVect = f.getDataVector();
  real_t t, *meanVect, *covVect;
  unsigned long vectSize = _pMixture->getVectSize();

  for (unsigned long c=0; c<_distribCount; c++)
  {
    DistribGD& d = _pMixForAccumulation->getDistrib(c);
    meanVect = d.getMeanVect().getArray();
    covVect  = d.getCovVect().getArray();
    
    for (unsigned long i=0; i<vectSize; i++)
    {
      t = _occVect[c] * dataVect[i];
      covVect[i]  += t * dataVect[i];
      meanVect[i] += t;
    }
  }
  _featureCounterForEM += w;
  return sum;
}
//-------------------------------------------------------------------------
void M::addAccEM(const MixtureStat& mx)
{
  const MixtureGDStat* p = dynamic_cast<const MixtureGDStat*>(&mx);
  if (p == NULL)
    throw Exception("MixtureStat incompatibility", __FILE__, __LINE__);
  if (p->_distribCount != _distribCount)
    throw Exception("MixtureStat incompatibility", __FILE__, __LINE__);
  const MixtureGDStat& m = static_cast<const MixtureGDStat&>(mx);

  _accumulatedOccVect += m._accumulatedOccVect;
  _featureCounterForAccumulatedOcc += m._featureCounterForAccumulatedOcc;

  for (unsigned long c=0; c<_distribCount; c++)
  {
    DistribGD& d = _pMixForAccumulation->getDistrib(c);
    const DistribGD& d2 = m._pMixForAccumulation->getDistrib(c);
    d.getCovVect() += d2.getCovVect();
    d.getMeanVect() += d2.getMeanVect();
  }
  _featureCounterForEM += m._featureCounterForEM;
}
//-------------------------------------------------------------------------
const Mixture& M::getEM()
{
  assertResetEMDone();
  unsigned long c;

  occ_t totOcc = _accumulatedOccVect.computeSum();
  unsigned long vectSize = _pMixture->getVectSize();

  for (c=0; c<_distribCount; c++)
  {
    const occ_t occ = _accumulatedOccVect[c];
    if (occ > 0.0)
    {
      DistribGD& dTmp = _pMixForAccumulation->getDistrib(c);
      real_t* dTmpCovVect  = dTmp.getCovVect().getArray();
      real_t* dTmpMeanVect = dTmp.getMeanVect().getArray();

      DistribGD& d  = _pMixtureForEM->getDistrib(c);
      real_t* dCovVect   = d.getCovVect().getArray();
      real_t* dMeanVect  = d.getMeanVect().getArray();

      real_t mean, cov;

      for (unsigned long i=0; i<vectSize; i++)
      {
        mean = dTmpMeanVect[i] / occ;
        cov  = dTmpCovVect [i] / occ - mean * mean;
        if (cov >MIN_COV)
          dCovVect [i] = cov;
        else
          dCovVect [i] = MIN_COV;
        dMeanVect[i] = mean;
      }
      _pMixtureForEM->weight(c) = occ/totOcc;
      d.computeAll();
    }
  }
  return *_pMixtureForEM;
}
//-------------------------------------------------------------------------
MixtureGD& M::getInternalAccumEM()
{
  assertResetEMDone();
  assert(_pMixForAccumulation != NULL);
  return *_pMixForAccumulation;
}
//-------------------------------------------------------------------------
String M::getClassName() const { return "MixtureGDStat"; }
//-------------------------------------------------------------------------
M::~MixtureGDStat()
{
  if (_pMixForAccumulation != NULL)
    delete _pMixForAccumulation;
  if (_pMixtureForEM != NULL)
    delete _pMixtureForEM;
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureGDStat_cpp)

