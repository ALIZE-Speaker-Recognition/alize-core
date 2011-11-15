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

#if !defined(ALIZE_ViterbiAccum_cpp)
#define ALIZE_ViterbiAccum_cpp

#include <iostream>
#include <new>
#include "Object.h"
#include "ViterbiAccum.h"
#include "Exception.h"
#include "Mixture.h"
#include "Feature.h"
#include "Config.h"
#include "MixtureStat.h"
#include "StatServer.h"

using namespace alize;

//-------------------------------------------------------------------------
ViterbiAccum::ViterbiAccum(StatServer& ss, const Config& c)
:Object(), _pConfig(&c), _pStatServer(&ss) { reset(); } 
//-------------------------------------------------------------------------
ViterbiAccum& ViterbiAccum::create(StatServer& ss, const Config& c,
                                   const K&)
{
    ViterbiAccum* p = new (std::nothrow) ViterbiAccum(ss, c);
    assertMemoryIsAllocated(p, __FILE__, __LINE__);
    return *p;
} 
//-------------------------------------------------------------------------
void ViterbiAccum::addState(Mixture& m)
{
    _stateVect.addObject(const_cast<Mixture&>(m));
    unsigned long size = _stateVect.size();
    _transMatrix.setSize(size*size);
}
//-------------------------------------------------------------------------
real_t& ViterbiAccum::logTransition(unsigned long i, unsigned long j)
{
    unsigned long size = _stateVect.size();
    if (i >= size)
        throw IndexOutOfBoundsException("", __FILE__, __LINE__, i, size);
    if (j >= size)
        throw IndexOutOfBoundsException("", __FILE__, __LINE__, i, size);
    return _transMatrix[j*size + i];
}
//-------------------------------------------------------------------------
real_t ViterbiAccum::logTransition(unsigned long i, unsigned long j) const
{ return const_cast<ViterbiAccum*>(this)->logTransition(i, j); }
//-------------------------------------------------------------------------
Mixture& ViterbiAccum::getState(unsigned long i) const
{ return _stateVect.getObject(i); }
//-------------------------------------------------------------------------
unsigned long ViterbiAccum::getStateCount() const
{ return _stateVect.size(); }
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
void ViterbiAccum::reset()
{
    _tmpTab.clear();
    _llpVect.clear();
    _llpDefined = false;
    _pathDefined = false;
    _featureCount = 0;
}
//-------------------------------------------------------------------------
void ViterbiAccum::computeAndAccumulate(const Feature& f, double llkW)
{
    unsigned long i, j, nbStates = _stateVect.size();
    _llpDefined = _pathDefined = false;

    // compute llk between the feature and each state
    _tmpLLKVect.clear();
    for (i=0; i<nbStates; i++)
        //_tmpLLKVect.addValue(_mixtureStatVect.getObject(i).computeLLK(f)-llkW);
        _tmpLLKVect.addValue(computeStateLLK(i, f)-llkW);
    //
    if (_featureCount == 0) // if first feature
        _llpVect = _tmpLLKVect;
    else
    {
        _tmpllpVect.clear();
        unsigned long maxInd = 0;
        real_t maxllp = 0;
        for (i=0; i<nbStates; i++)
        {

            for (j=0; j<nbStates; j++)
            {
                real_t llp = _llpVect[j] + _tmpLLKVect[i]
                                         + logTransition(j, i);
                if (j == 0 || llp > maxllp)
                {
                    maxllp = llp;
                    maxInd = j;
                }
            }
            _tmpllpVect.addValue(maxllp);
            _tmpTab.addValue(maxInd);
        }
        _llpVect = _tmpllpVect;
    }
    _featureCount++;
}

//-------------------------------------------------------------------------
void ViterbiAccum::computeAndAccumulate(FeatureServer& fs,
               DoubleVector& llkW, unsigned long start, unsigned long count)
{
  unsigned long i, j, nbStates = _stateVect.size();
  _llpDefined = _pathDefined = false;
  double l;
  Feature f;
    
  // compute llk between the feature and each state
  _tmpLLKVect.clear();
  for (i=0; i<nbStates; i++)
  {
     l = 0.0;
    fs.seekFeature(start);
    for (unsigned long ifeature=start; ifeature < (start+count); ifeature++)
    {
       fs.readFeature(f);
       l += computeStateLLK(i, f) -llkW[ifeature]+logTransition(i,i);
    }     
    _tmpLLKVect.addValue(l/count);
    //cout << "start: " << start << " & count: " << count << " Etat " << i << " => " << l/count << endl;
  }
  if (_featureCount == 0)  // if first feature in the viterbi path
  {
    _llpVect = _tmpLLKVect;
    for(unsigned long c=0; c<count; c++)
      for (i=0; i<nbStates; i++)    
        _tmpTab.addValue(i);
  }       
  else
  {
    _tmpllpVect.clear();
    unsigned long maxInd = 0;
    real_t maxllp = 0;
    real_t llp;
  
    // For the first frame of the n block (n>0)- find the path
    for (i=0; i<nbStates; i++)
    {
      for (j=0; j<nbStates; j++)
      {
        llp = _llpVect[j] + _tmpLLKVect[i] + logTransition(j, i);
        if (j == 0 || llp > maxllp)
        {
          maxllp = llp;
          maxInd = j;
        }
      }
      _tmpllpVect.addValue(maxllp);
      _tmpTab.addValue(maxInd);
    }
    for(unsigned long c=1; c<count; c++)
      for (i=0; i<nbStates; i++)
        _tmpTab.addValue(i);
    _llpVect = _tmpllpVect;
  }
  _featureCount+=count;
  //cout << " FeatureCount: " << _featureCount << endl;
}
//-------------------------------------------------------------------------
void ViterbiAccum::computeAndAccumulate(FeatureServer& fs,
                    unsigned long start, unsigned long count, double fudge)
{
  unsigned long i, j, nbStates = _stateVect.size();
  _llpDefined = _pathDefined = false;
  double l;
  Feature f;
    
  // compute llk between the feature and each state
  _tmpLLKVect.clear();
  for (i=0; i<nbStates; i++)
  {
    l = 0.0;
    fs.seekFeature(start);
    for (unsigned long ifeature=start; ifeature < (start+count); ifeature++)
    {
      fs.readFeature(f);
      l += computeStateLLK(i, f);
    }     
    _tmpLLKVect.addValue(l/count);
    //  cout << "start: " << start << " & count: " << count << " Etat " << i << " => " << l/count << endl;
  }
  if (_featureCount == 0) // if first feature in the viterbi path
  {
    _llpVect = _tmpLLKVect;
    for(unsigned long c=0; c<count; c++)
      for (i=0; i<nbStates; i++)    
        _tmpTab.addValue(i);
  }       
  else
  {
    _tmpllpVect.clear();
    unsigned long maxInd = 0;
    real_t maxllp = 0;
    real_t llp;
  
    // For the first frame of the n block (n>0)- find the path
    for (i=0; i<nbStates; i++)
    {
      for (j=0; j<nbStates; j++)
      {
        llp = _llpVect[j] + _tmpLLKVect[i] + logTransition(j, i)*fudge;
        if (j == 0 || llp > maxllp)
        {
          maxllp = llp;
          maxInd = j;
        }
      }
      _tmpllpVect.addValue(maxllp);
      _tmpTab.addValue(maxInd);
    }
    for(unsigned long c=1; c<count; c++)
      for (i=0; i<nbStates; i++)
        _tmpTab.addValue(i);
    _llpVect = _tmpllpVect;
  }
  _featureCount+=count;
  //cout << " FeatureCount: " << _featureCount << endl;
}

/*//-------------------------------------------------------------------------
void ViterbiAccum::computeAndAccumulate(FeatureServer *fs, DoubleVector& llkW, int start, int count)
//-------------------------------------------------------------------------
 {
    unsigned long i, j, nbStates = _stateVect.size();
    _llpDefined = _pathDefined = false;
  double l;
    
    // compute llk between the feature and each state
    _tmpLLKVect.clear();
    for (i=0; i<nbStates; i++){
       l = 0.0;
  for(int ifeature=start; ifeature < (start+count); ifeature++){
     Feature& f=fs->getFeature(ifeature);
    l += computeStateLLK(i, f)-llkW[ifeature];
   }     
       _tmpLLKVect.addValue(l/count);
  //cout << "start: " << start << " & count: " << count << " Etat " << i << " => " << l/count << endl;
       
    }
    if (_featureCount == 0){ // if first feature
        _llpVect = _tmpLLKVect;
      for(int c=0; c<count; c++){
    for (i=0; i<nbStates; i++){    
            _tmpTab.addValue(i);
    }  
   }
  
    }       
    else
    {
        _tmpllpVect.clear();
        unsigned long maxInd = 0;
        real_t maxllp = 0;
  real_t llp;
        for(int c=0; c<count; c++){
     if(c==0){
      for (i=0; i<nbStates; i++){
        for (j=0; j<nbStates; j++){
                llp = _llpVect[j] + _tmpLLKVect[i] + logTransition(j, i);
          if (j == 0 || llp > maxllp){
                      maxllp = llp;
                        maxInd = j;
                }
                     }
           _tmpllpVect.addValue(maxllp);
                 _tmpTab.addValue(maxInd);
      }
    }
         else{
      for (i=0; i<nbStates; i++){
        _tmpTab.addValue(i);
      }
    }
        }
        _llpVect = _tmpllpVect;
    }
    _featureCount+=count;
    //cout << " FeatureCount: " << _featureCount << endl;
}
*/
 
 
 //-------------------------------------------------------------------------
/*void ViterbiAccum::computeAndAccumulate(FeatureServer *fs, DoubleVector& llkW, int start, int count)
//-------------------------------------------------------------------------
 {
    unsigned long i, j, nbStates = _stateVect.size();
    _llpDefined = _pathDefined = false;
  double l;
    
    // compute llk between the feature and each state
    _tmpLLKVect.clear();
    cout << count << " " << start << endl;
    for(int c=0, ifeature=start; c<count; c++, ifeature++){
       
  Feature& f=fs->getFeature(ifeature);
      for (i=0; i<nbStates; i++){
    l = computeStateLLK(i, f)-llkW[ifeature];
    
    cout << c << " => " << l << endl;
    _tmpLLKVect.addValue(l);   
  }
       
      if (_featureCount == 0){ // if first feature
           _llpVect = _tmpLLKVect;
    for (i=0; i<nbStates; i++){    
             _tmpTab.addValue(i);
    }      
      }       
     else{
          _tmpllpVect.clear();
          unsigned long maxInd = 0;
          real_t maxllp = 0;
    real_t llp;
          
    if(c==0){
      for (i=0; i<nbStates; i++){
         for (j=0; j<nbStates; j++){
              llp = _llpVect[j] + _tmpLLKVect[i] + logTransition(j, i);
        if (j == 0 || llp > maxllp){
                      maxllp = llp;
                  maxInd = j;
              }
                }
         _tmpllpVect.addValue(maxllp);
               _tmpTab.addValue(maxInd);
      }
    }
         else{
      for (i=0; i<nbStates; i++){
              llp = _llpVect[i] + _tmpLLKVect[i] + logTransition(i, i);
          _tmpllpVect.addValue(llp);
                _tmpTab.addValue(i);
            }
    
         }
         _llpVect = _tmpllpVect;
        }
    }//fin for(int c=0... 
    _featureCount+=count;
    //cout << " FeatureCount: " << _featureCount << endl;
}
*/

//-------------------------------------------------------------------------
void ViterbiAccum::computeAndAccumulate(const Feature& f, double fudge, double penality)
//-------------------------------------------------------------------------
 {
    unsigned long i, j, nbStates = _stateVect.size();
    _llpDefined = _pathDefined = false;

    // compute llk between the feature and each state
    _tmpLLKVect.clear();
    for (i=0; i<nbStates; i++)
      _tmpLLKVect.addValue(computeStateLLK(i, f));
    //
    if (_featureCount == 0) // if first feature
        _llpVect = _tmpLLKVect;
    else
    {
        _tmpllpVect.clear();
        unsigned long maxInd = 0;
        real_t maxllp = 0;
        for (i=0; i<nbStates; i++)
        {

            for (j=0; j<nbStates; j++)
            {
                real_t llp = _llpVect[j] + _tmpLLKVect[i] + fudge * logTransition(j, i);
                if(i != j) 
      llp += penality;
                if (j == 0 || llp > maxllp)
                {
                    maxllp = llp;
                    maxInd = j;
                }
            }
            _tmpllpVect.addValue(maxllp);
            _tmpTab.addValue(maxInd);
        }
        _llpVect = _tmpllpVect;
    }
    _featureCount++;
}

//-------------------------------------------------------------------------
unsigned long ViterbiAccum::getFeatureCount() const {return _featureCount;}
//-------------------------------------------------------------------------
const ULongVector& ViterbiAccum::getPath()
{
    
  if (!_pathDefined)
    {
        unsigned long i, max = 0, nbStates = _stateVect.size();

        // looks for the largest llp
        for (i=0; i<nbStates; i++){
   if (_llpVect[i] > _llpVect[max])
                max = i;
   }
        _llp = _llpVect[max];
        _llpDefined = true;

        _path.setSize(_featureCount);
        if (_featureCount != 0)
        {
            for (i=_featureCount-1; i>0; i--)
            {
                _path[i] = max;
                max = _tmpTab[(i-1)*nbStates+max];
            }
            _path[0] = max;
        }
        _pathDefined = true;
    }
    return _path;
}
//-------------------------------------------------------------------------
real_t ViterbiAccum::getLlp() const
{
    if (!_llpDefined)
        throw Exception("llp undefined", __FILE__, __LINE__);
    return _llp;
}
//-------------------------------------------------------------------------
lk_t ViterbiAccum::computeStateLLK(unsigned long i, const Feature& f) const
{
  return _pStatServer->computeLLK(_stateVect.getObject(i), f);
}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
String ViterbiAccum::getClassName() const { return "ViterbiAccum"; }
//-------------------------------------------------------------------------
String ViterbiAccum::toString() const
{
    unsigned long i, nbStates = getStateCount();
    String s = Object::toString()
               + " Nb States = " + String::valueOf(nbStates)
               + " Nb features = " + String::valueOf(_featureCount);
    for (i=0; i<nbStates; i++)
    {
        for (unsigned long j=0; j<nbStates; j++)
        s += "\n  Log transition " + String::valueOf(i) + "->"
           + String::valueOf(j) + " = "
           + String::valueOf(logTransition(i, j));
    }
    if (_llpDefined)
        s += "\n  llp = " + String::valueOf(_llp);
    else
        s += "\n  llp = undefined";

    if (_pathDefined)
        for (i=0; i<_featureCount; i++)
            s += "\n  path[" + String::valueOf(i) + "]"
            + " = " + String::valueOf(_path[i]);
    else
        s += "\n  path = undefined";
    return s;
}
//-------------------------------------------------------------------------
ViterbiAccum::~ViterbiAccum() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_ViterbiAccum_cpp)
