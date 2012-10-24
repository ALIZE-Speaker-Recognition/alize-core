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

#if !defined(ALIZE_FeatureMultipleFileReader_cpp)
#define ALIZE_FeatureMultipleFileReader_cpp

#include <new>
#include "FeatureMultipleFileReader.h"
#include "Feature.h"
#include "Exception.h"
#include "FeatureFlags.h"
#include "LabelServer.h"
#include "Config.h"
#include <iostream>
using namespace std;

using namespace alize;
typedef FeatureMultipleFileReader R;

//-------------------------------------------------------------------------
R::FeatureMultipleFileReader(const XLine& l, const Config& c,
     LabelServer* p, BigEndian be, BufferUsage b, unsigned long bufferSize,
     HistoricUsage h, unsigned long historicSize)
:FeatureFileReaderAbstract(NULL, c, p, b, bufferSize, h, historicSize),
 _fileCounter(0), _fileList(l, c), _bigEndian(be),
 _fileCount(_fileList.size()), _readerPtrVect(createReaderPtrVect()),
 _bufferPtrVect(createBufferPtrVect()), _memUsed(0),
 _featuresAreWritableDefined(false), _lastFeatureIndex(0) {}
//-------------------------------------------------------------------------
FeatureFileReader** R::createReaderPtrVect()
{
  FeatureFileReader** p = new (std::nothrow)
                          FeatureFileReader*[_fileCount==0?1:_fileCount];
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  for (unsigned long i=0; i<_fileCount; i++)
    p[i] = NULL;
  return p;
}
//-------------------------------------------------------------------------
FloatVector** R::createBufferPtrVect()
{
  FloatVector** p = new (std::nothrow)
                          FloatVector*[_fileCount==0?1:_fileCount];
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  for (unsigned long i=0; i<_fileCount; i++)
    p[i] = NULL;
  return p;
}
//-------------------------------------------------------------------------
R& R::create(const XLine& list, const Config& c,
    LabelServer* ls, BigEndian be, BufferUsage b, unsigned long bufferSize,
    HistoricUsage h, unsigned long historicSize)
{
  R* p = new (std::nothrow) R(list, c, ls, be, b, bufferSize, h, historicSize);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
bool R::readFeature(Feature& f, unsigned long s) { return rw(true, f, s); }
//-------------------------------------------------------------------------
bool R::addFeature(const Feature& f) { throw Exception ("featureMultipleFileReader::addFeature not yet implemented", __FILE__, __LINE__); }
//-------------------------------------------------------------------------
bool R::writeFeature(const Feature& f, unsigned long step)
{
  // temporary test
  if (!_featuresAreWritableDefined)
  {
    if (_featuresAreWritable)
    {
      unsigned long m = 0;
      if (_bufferUsage == BUFFER_USERDEFINE)
        m = _userDefineBufferSize/sizeof(float);
      else if (_bufferUsage == BUFFER_AUTO && 
               getConfig().existsParam_loadFeatureFileMemAlloc)
        m = getConfig().getParam_loadFeatureFileMemAlloc()/sizeof(float);
      if (m < getFeatureCount()*getVectSize())
        _featuresAreWritable = false;
    }
    _featuresAreWritableDefined = true;
  }
  if (!_featuresAreWritable)
    throw Exception("Feature writing forbidden", __FILE__, __LINE__);

  return rw(false, const_cast<Feature&>(f), step);
}
//-------------------------------------------------------------------------
bool R::rw(bool read, Feature& f, unsigned long s) // private
{
  // _fileCounter = n° du PROCHAIN fichier à lire
  unsigned long featureNbr = 0;
  unsigned long futureLastFeatureIndex = _lastFeatureIndex;
  bool seekWantedInCurrentFile = _seekWanted;

  // _fileCounter = number of file to read
  // featureNbr = number of feature to read in the file
  // featureNbrGlobal

  if (_seekWanted)
  {
    _seekWanted = false;
    if (_fileCount != 0)
    {
      unsigned long featureNbrGlobal;
      if (_seekWantedSrcName.isEmpty())
      {
        _fileCounter     = _fileList.getFileIndex(_seekWantedIdx);
        featureNbr       = _seekWantedIdx
                           - _fileList.getIndexOfFirstFeature(_fileCounter);
        featureNbrGlobal = _seekWantedIdx;
      }
      else
      {
        _fileCounter     = _fileList.getFileIndex(_seekWantedSrcName);
        featureNbr       = _seekWantedIdx;
        featureNbrGlobal = _fileList.getIndexOfFirstFeature(_fileCounter)
                           + _seekWantedIdx;
      }
      if (_historicUsage == LIMITED &&
          !featureWantedIsInHistoric(featureNbrGlobal))
      {
        if (!read) // if write
          throw Exception("Feature out of historic", __FILE__, __LINE__);
        f.setVectSize(K::k, getVectSize());
        f.setValidity(false);
        _error = FEATURE_OUT_OF_HISTORY;
        return true;
      }
      if (featureNbrGlobal > futureLastFeatureIndex)
        futureLastFeatureIndex = featureNbrGlobal;
    }
    else
      _fileCounter = 0;
  }
  else
    futureLastFeatureIndex++;

  while (true)
  {
    if (_fileCounter >= _fileCount) // end of file list
      return false;
    FeatureFileReader& r = getReader(_fileCounter);
    if (seekWantedInCurrentFile)
    {
      r.seekFeature(featureNbr);
      seekWantedInCurrentFile = false;
    }
    bool ok;
    if (read)
      ok = r.readFeature(f, s);
    else
      ok = r.writeFeature(f, s);
    _error = r.getError();
    if (ok)
    {
      _lastFeatureIndex = futureLastFeatureIndex;
      return true;
    }
    _fileCounter++;
    featureNbr = 0;
    seekWantedInCurrentFile = true;
  }
}
//-------------------------------------------------------------------------
bool R::featureWantedIsInHistoric(unsigned long featureNbr) const
{
  if (featureNbr > _lastFeatureIndex)
    return false;
  if (_historicSize > _lastFeatureIndex)
    return featureNbr >= 0;
  return featureNbr >= _lastFeatureIndex-_historicSize;
}
//-------------------------------------------------------------------------
FeatureFileReader& R::getReader(unsigned long idx) // private
{
  assert(idx < _fileCount);
  assert(_readerPtrVect != NULL);
  //
  FeatureFileReader*& pReader = _readerPtrVect[idx];
  if (pReader != NULL)
    return *pReader;
  //
  pReader = &FeatureFileReader::create(_fileList.getFileName(idx),
                 getConfig(), _pLabelServer, _bigEndian, BUFFER_USERDEFINE, 0);
  // <FRANCAIS>
  // Creer un buffer
  // S'il ne reste pas assez de memoire disponible, détruit les
  // readers les plus anciens pour récuperer la memoire des buffers
  //
  unsigned long memMax = 0;
  if (_bufferUsage == BUFFER_USERDEFINE)
    memMax = _userDefineBufferSize / sizeof(float);
  else if (_bufferUsage == BUFFER_AUTO && 
      getConfig().existsParam_loadFeatureFileMemAlloc)
    memMax = getConfig().getParam_loadFeatureFileMemAlloc() / sizeof(float);
  //
  FloatVector*& pBuffer = _bufferPtrVect[idx];
  unsigned long memNeeded = pReader->getFeatureCount()*pReader->getVectSize();
  //
  unsigned long i, m = _memUsed + memNeeded;
  for (i=0; i<_memStack.size(); i++)
  {
    if (m <= memMax)
      break;
    m -= _memStack[i];
    unsigned long j = _readerStack[i];
    FeatureFileReader*& p = _readerPtrVect[j];
    assert(p != NULL);
    delete p;
    p = NULL;
    FloatVector*& p1 = _bufferPtrVect[j];
    assert(p1 != NULL);
    delete p1;
    p1 = NULL;
  }
  if (i>0)
  {
    _memStack.removeValues(0, i-1);
    _readerStack.removeValues(0, i-1);
  }
  if (m >= memMax) // not enough memory
  {
    memNeeded = memMax;
    m = memMax;
  }
  pBuffer = &FloatVector::create(memNeeded, memNeeded);
  //cout << "memNeeded = " << memNeeded << endl;
  _readerStack.addValue(idx);
  _memStack.addValue(memNeeded);
  pReader->setExternalBufferToUse(*pBuffer);
  //std::cout << memNeeded << std::endl;
  _memUsed = m;
  return *pReader;

  //_readerStack.removeValues(0, 0);
  unsigned long mem = memNeeded;
  //pBuffer->setSize(mem);
  _memUsed += mem;
  _readerStack.addValue(idx);
  return *pReader;
}
//-------------------------------------------------------------------------
unsigned long R::getFeatureCount() { return _fileList.getFeatureCount(); }
//-------------------------------------------------------------------------
unsigned long R::getVectSize() { return _fileList.getVectSize(); }
//-------------------------------------------------------------------------
const FeatureFlags& R::getFeatureFlags()  
{ return _fileList.getFeatureFlags(); }
//-------------------------------------------------------------------------
real_t R::getSampleRate() { return _fileList.getSampleRate(); }
//-------------------------------------------------------------------------
unsigned long R::getSourceCount() { return _fileList.size(); }
//-------------------------------------------------------------------------
unsigned long R::getFeatureCountOfASource(unsigned long srcIdx)
{ return _fileList.getFeatureCount(srcIdx); }
//-------------------------------------------------------------------------
unsigned long R::getFeatureCountOfASource(const FileName& f)
{ return _fileList.getFeatureCount(f); }
//-------------------------------------------------------------------------
unsigned long R::getFirstFeatureIndexOfASource(unsigned long srcIdx)
{ return _fileList.getIndexOfFirstFeature(srcIdx); }
//-------------------------------------------------------------------------
unsigned long R::getFirstFeatureIndexOfASource(const FileName& f)
{ return _fileList.getIndexOfFirstFeature(f); }
//-------------------------------------------------------------------------
const String& R::getNameOfASource(unsigned long srcIdx)
{ return _fileList.getFileName(srcIdx); }
//-------------------------------------------------------------------------
void R::close()
{
  for (unsigned long i=0; i<_fileCount; i++)
    if (_readerPtrVect[i] != NULL)
      _readerPtrVect[i]->close();
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "FeatureMultipleFileReader"; }
//-------------------------------------------------------------------------
String R::toString() const
{
  FeatureMultipleFileReader& r = const_cast<FeatureMultipleFileReader&>(*this);
  const FeatureFlags flags(r.getFeatureFlags());
  return Object::toString()
    + "\n  file count   = '" + String::valueOf(_fileList.size())
    + "\n  vectSize    = " + String::valueOf(r.getVectSize())
    + "\n  feature count = " + String::valueOf(r.getFeatureCount())
    + "\n  sample rate   = " + String::valueOf(r.getSampleRate())
    + "\n  flag S    = " + String::valueOf(flags.useS)
    + "\n  flag E    = " + String::valueOf(flags.useE)
    + "\n  flag D    = " + String::valueOf(flags.useD)
    + "\n  flag DE     = " + String::valueOf(flags.useDE)
    + "\n  flag DD     = " + String::valueOf(flags.useDD)
    + "\n  flag DDE    = " + String::valueOf(flags.useDDE);
}
//-------------------------------------------------------------------------
R::~FeatureMultipleFileReader()
{
  if (_readerPtrVect != NULL)
  {
    for (unsigned long i=0; i<_fileCount; i++)
      if (_readerPtrVect[i] != NULL)
        delete _readerPtrVect[i];
    delete [] _readerPtrVect;
  }
  if (_bufferPtrVect != NULL)
  {
    for (unsigned long i=0; i<_fileCount; i++)
      if (_bufferPtrVect[i] != NULL)
        delete _bufferPtrVect[i];
    delete [] _bufferPtrVect;
  }
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FeatureMultipleFileReader_cpp)

