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

#if !defined(ALIZE_FeatureFileReaderSingle_cpp)
#define ALIZE_FeatureFileReaderSingle_cpp

#include <new>
#include "FeatureFileReaderSingle.h"
#include "FileReader.h"
#include "Exception.h"
#include "LabelServer.h"
#include "Label.h"
#include "FeatureFlags.h"
#include "Config.h"
#include "RealVector.h"
#include "FileReader.h"

#include <iostream>

using namespace alize;
typedef FeatureFileReaderSingle R;

//-------------------------------------------------------------------------
R::FeatureFileReaderSingle(FileReader* r, FeatureInputStream* st, 
                           const Config& c, LabelServer* p,
                           BufferUsage b, unsigned long bufferSize,
                           HistoricUsage h, unsigned long historicSize)
:FeatureFileReaderAbstract(NULL, c, p, b, bufferSize, h, historicSize),
 _pReader(r), _pFeatureInputStream(st), _pFeature(NULL), _featureIndex(0),
 _lastFeatureIndex(0),
 _featureIndexOfBuffer(0), _nbStored(0), _pBuffer(&FloatVector::create())
{}
//-------------------------------------------------------------------------
String R::getPath(const FileName& f, const Config& c) const
{  // protected method
   if (f.beginsWith("/") || f.beginsWith("./"))
    return "";
  return c.getParam_featureFilesPath();
}
//-------------------------------------------------------------------------
String R::getExt(const FileName& f, const Config& c) const
{  // protected method
   if (f.beginsWith("/") || f.beginsWith("./"))
    return "";
  return c.getParam_loadFeatureFileExtension();
}
//-------------------------------------------------------------------------
bool R::getBigEndian(const Config& c, BigEndian b) const // protected
{
  if (b == BIGENDIAN_TRUE)
    return true;
  if (b == BIGENDIAN_FALSE)
    return false;
  // BIGENDIAN_AUTO
  if (c.existsParam_loadFeatureFileBigEndian)
    return c.getParam_loadFeatureFileBigEndian();
  if (c.existsParam_bigEndian)
    return c.getParam_bigEndian();
  return false;
}
//-------------------------------------------------------------------------
void R::close()
{
  if (_pReader != NULL)
    _pReader->close();
  if (_pFeatureInputStream != NULL)
    _pFeatureInputStream->close();
}
//-------------------------------------------------------------------------
bool R::readFeature(Feature& f, unsigned long step)
{
  assert(_pReader != NULL || _pFeatureInputStream != NULL);
  if (_seekWanted)
  {
    _seekWanted = false;
    if (_historicUsage == LIMITED && !featureWantedIsInHistoric())
    {
      f.setVectSize(K::k, getVectSize());
      f.setValidity(false);
      _error = FEATURE_OUT_OF_HISTORY;
      return true;
    }
    _featureIndex = _seekWantedIdx;
  }
  // si on depasse la fin du fichier
  unsigned long featureCount = getFeatureCount();
  if (_featureIndex >= featureCount)
    return false;
  // si on demande une feature hors du buffer
  if (_featureIndex < _featureIndexOfBuffer ||
      _featureIndex >= _featureIndexOfBuffer + _nbStored)
  {
    if (!_bufferSizeDefined)
    {
      unsigned long m = _pBuffer->size();
      if (_bufferIsInternal)
      {
        if (_bufferUsage == BUFFER_USERDEFINE)
          m = _userDefineBufferSize/sizeof(float);
        else if (_bufferUsage == BUFFER_AUTO)
        {
          if (getConfig().existsParam_loadFeatureFileMemAlloc)
          {
            m = getConfig().getParam_loadFeatureFileMemAlloc()/sizeof(float);
            unsigned long n = featureCount*getVectSize();
            if (n < m)
              m = n;
          }
        }
      }
      if (m < getVectSize()) // minimum size
        m = getVectSize();
      _pBuffer->setSize(m);
      _bufferSizeDefined = true;
    }
    unsigned long start = _featureIndex;
    if (featureCount-_featureIndex < _pBuffer->size()/getVectSize())
    {
      unsigned long x = _pBuffer->size()/getVectSize() -
                        (featureCount-_featureIndex);
      if (x < _featureIndex)
        start -= x;
      else
        start = 0;
    }
    // si le bloc de donnees a charger ne suit pas le bloc deja en memoire
    // on se repositionne dans le fichier
    if (start != _featureIndexOfBuffer + _nbStored /*+ 1*/) {
      if (_pReader != NULL) {
        _pReader->seek(getHeaderLength() + start*getVectSize()*sizeof(float));
      }
      else {
        _pFeatureInputStream->seekFeature(start);
      }
    }
    // chargement des donnees dans le buffer
    if (_pReader != NULL)
      _nbStored = _pReader->readSomeFloats(*_pBuffer)/getVectSize();
    else
    {
      // Pas performant. A améliorer
      _nbStored = 0;
      unsigned long vectSize = _pFeatureInputStream->getVectSize();
      while ((_nbStored+1)*vectSize <= _pBuffer->size()
              && _pFeatureInputStream->readFeature(_f))
      {
        unsigned long ii = _nbStored*vectSize;
        for (unsigned long j=0; j<vectSize; j++)
          (*_pBuffer)[ii+j] = (float) _f[j];
        _nbStored++;
      }
    }

    _featureIndexOfBuffer = start;
    // if all the features are loaded in the buffer, we close the file
    if (_nbStored == featureCount)
      close();
    else
      // données pas toutes en mémoire -> interdit le writeFeature()
      _featuresAreWritable = false;
  }
  f.setVectSize(K::k, getVectSize());
  f.setData(*_pBuffer, (_featureIndex-_featureIndexOfBuffer)*getVectSize());
  f.setValidity(true);

  _featureIndex += step;
  if (_featureIndex > _lastFeatureIndex)
    _lastFeatureIndex = _featureIndex;
  if (_pLabelServer != NULL)
  {
    Label l;
    if (_pReader != NULL)
      l.setSourceName(_pReader->getFileName());
    else
      l.setSourceName(_pFeatureInputStream->getNameOfASource(0)); // TODO : not always 0 ?
    f.setLabelCode(_pLabelServer->addLabel(l));
  }
  _error = NO_ERROR;
  return true;
}
//-------------------------------------------------------------------------
bool R::addFeature(const Feature& f) {
	/* if not yet read --> not charged in memory */
	if (_nbStored == 0) {
		Feature tmp;
		this->readFeature(tmp);
		_featureIndex=0;
	}
	unsigned long featureServerMemAlloc = getConfig().getParam_featureServerMemAlloc();
	if (((_nbStored+1)*_vectSize*sizeof(float))>getConfig().getParam_featureServerMemAlloc()) {
		throw Exception("Feature adding not possible (not enougth space	regarding featureServerMemAlloc parameter)", __FILE__, __LINE__);
	}
	// copy of the data at the end
	_pBuffer->setSize(_nbStored*_vectSize);
	fprintf(stderr, "%d ---", (int)_pBuffer->size());
	unsigned long cptri;
	for (cptri=0 ; cptri<f.getVectSize() ; cptri++) {
		_pBuffer->addValue((float)f[cptri]);
	}
	_nbStored++;
	fprintf(stderr, " %d\n", (int)_pBuffer->size());
	fprintf(NULL, "toto");
  return true;
}
//-------------------------------------------------------------------------
bool R::writeFeature(const Feature& f, unsigned long step)
{
  if (!_featuresAreWritable)
    throw Exception("Feature writing forbidden", __FILE__, __LINE__);
  assert(_pReader != NULL || _pFeatureInputStream != NULL);
  if (_seekWanted)
  {
    _seekWanted = false;
    if (_historicUsage == LIMITED && !featureWantedIsInHistoric())
      throw Exception("Feature out of Historic", __FILE__, __LINE__);
    _featureIndex = _seekWantedIdx;
  }
  // si on depasse la fin du fichier
  unsigned long featureCount = getFeatureCount();
  if (_featureIndex >= featureCount)
    return false;

  // si on demande une feature hors du buffer
  if (_featureIndex < _featureIndexOfBuffer ||
      _featureIndex >= _featureIndexOfBuffer + _nbStored)
  {
    if (!_bufferSizeDefined)
    {
      unsigned long m = _pBuffer->size();
      if (_bufferIsInternal)
      {
        if (_bufferUsage == BUFFER_USERDEFINE)
          m = _userDefineBufferSize/sizeof(float);
        else if (_bufferUsage == BUFFER_AUTO)
        {
          if (getConfig().existsParam_loadFeatureFileMemAlloc)
          {
            m = getConfig().getParam_loadFeatureFileMemAlloc()/sizeof(float);
            unsigned long n = featureCount*getVectSize();
            if (n < m)
              m = n;
          }
        }
      }
      if (m < getVectSize()) // minimum size
        m = getVectSize();
      _pBuffer->setSize(m);
      _bufferSizeDefined = true;
    }
    unsigned long start = _featureIndex;
    if (featureCount-_featureIndex < _pBuffer->size()/getVectSize())
    {
      unsigned long x = _pBuffer->size()/getVectSize() -
                        (featureCount-_featureIndex);
      if (x < _featureIndex)
        start -= x;
      else
        start = 0;
    }
    // si le bloc de donnees a charger ne suit pas le bloc deja en memoire
    // on se repositionne dans le fichier
    if (start != _featureIndexOfBuffer + _nbStored + 1) {
      if (_pReader != NULL) {
        _pReader->seek(getHeaderLength() + start*getVectSize()*sizeof(float));
      }
      else {
        _pFeatureInputStream->seekFeature(start);
      }
    }
    // chargement des donnees dans le buffer
    if (_pReader != NULL)
      _nbStored = _pReader->readSomeFloats(*_pBuffer)/getVectSize();
    else
    {
      // Pas performant. A améliorer
      _nbStored = 0;
      unsigned long vectSize = _pFeatureInputStream->getVectSize();
      while ((_nbStored+1)*vectSize <= _pBuffer->size()
              && _pFeatureInputStream->readFeature(_f))
      {
        unsigned long ii = _nbStored*vectSize;
        for (unsigned long j=0; j<vectSize; j++)
          (*_pBuffer)[ii+j] = (float) _f[j];
        _nbStored++;
      }
    }

    _featureIndexOfBuffer = start;
    // if all the features are loaded in the buffer, we close the file
    if (_nbStored == featureCount)
      close();
    else
      // données pas toutes en mémoire -> interdit le writeFeature()
      throw Exception("Feature writing forbidden (data are not all in memory)"
                      , __FILE__, __LINE__);
  }
  unsigned long vectSize = getVectSize();
  if (vectSize != f.getVectSize())
    throw Exception("incompatibles vectSize (" + String::valueOf(vectSize)
        + "/" + String::valueOf(f.getVectSize()) + ")", __FILE__, __LINE__);
  unsigned long offset = (_featureIndex-_featureIndexOfBuffer)*vectSize;
  for (unsigned long i=0; i<vectSize; i++)
    (*_pBuffer)[i+offset] = (float)f[i]; // TODO : conversion a revoir ?
  _featureIndex += step;
  if (_featureIndex > _lastFeatureIndex)
    _lastFeatureIndex = _featureIndex;
  return true;
}
//-------------------------------------------------------------------------
bool R::featureWantedIsInHistoric() const
{
  if (_seekWantedIdx > _lastFeatureIndex)
    return false;
  if (_historicSize > _lastFeatureIndex)
    return _seekWantedIdx >= 0;
  return _seekWantedIdx >= _lastFeatureIndex-_historicSize;
}
//-------------------------------------------------------------------------
void R::setExternalBufferToUse(FloatVector& v)
{
  if (_bufferIsInternal && _pBuffer != NULL )
    delete _pBuffer;
  _pBuffer = &v;
  _bufferSizeDefined = false;
  _bufferIsInternal = false;
  _featureIndexOfBuffer = 0;
  _nbStored = 0;
}
//-------------------------------------------------------------------------
// Comportement par defaut. Methode surchargee dans les sous-classes
unsigned long R::getHeaderLength() { return 0; }
//-------------------------------------------------------------------------
unsigned long R::getSourceCount() {return 1;}
//-------------------------------------------------------------------------
unsigned long R::getFeatureCountOfASource(unsigned long srcIdx)
{
  if (srcIdx != 0)
    throw Exception("Only 1 file available", __FILE__, __LINE__);
  return getFeatureCount();
}
//-------------------------------------------------------------------------
unsigned long R::getFeatureCountOfASource(const FileName& f)
{
  assert(_pReader != NULL || _pFeatureInputStream != NULL);
  if (_pReader != NULL)
  {
    if (f != _pReader->getFileName())
      throw Exception("Wrong source name : " + f, __FILE__, __LINE__);
  }
  else
    if (f != _pFeatureInputStream->getNameOfASource(0)) // TODO : always 0 ?
      throw Exception("Wrong source name : " + f, __FILE__, __LINE__);
  return getFeatureCount();
}
//-------------------------------------------------------------------------
unsigned long R::getFirstFeatureIndexOfASource(unsigned long srcIdx)
{
  if (srcIdx != 0)
    throw Exception("Only 1 file available", __FILE__, __LINE__);
  return 0;
}
//-------------------------------------------------------------------------
unsigned long R::getFirstFeatureIndexOfASource(const FileName& f)
{
  assert(_pReader != NULL || _pFeatureInputStream != NULL);
  if (_pReader != NULL)
  {
    if (f != _pReader->getFileName())
      throw Exception("Wrong source name : " + f, __FILE__, __LINE__);
  }
  else
    if (f != _pFeatureInputStream->getNameOfASource(0)) // TODO : always 0 ?
      throw Exception("Wrong source name : " + f, __FILE__, __LINE__);
  return 0;
}
//-------------------------------------------------------------------------
const String& R::getNameOfASource(unsigned long srcIdx)
{
  if (srcIdx != 0)
    throw Exception("Only 1 file available", __FILE__, __LINE__);
  assert(_pReader != NULL || _pFeatureInputStream != NULL);
  if (_pReader != NULL)
    return _pReader->getFileName();
  else
    return _pFeatureInputStream->getNameOfASource(0); // TODO : always 0 ?
}
//-------------------------------------------------------------------------
String R::toString() const
{
  assert(_pReader != NULL || _pFeatureInputStream != NULL);
  FeatureFileReaderSingle& r
                           = const_cast<FeatureFileReaderSingle&>(*this);
  const FeatureFlags flags(r.getFeatureFlags());
  String n;
  if (_pReader != NULL)
    n = _pReader->getFullFileName();
  else
    n = _pFeatureInputStream->getNameOfASource(0); // TODO : always 0 ?
  return Object::toString()
    + "\n  file name   = '" + n + "'"
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
R::~FeatureFileReaderSingle()
{
  if (_pReader != NULL)
    delete _pReader;
  // do not delete _pFeatureInputStream
  if (_pFeature != NULL)
    delete _pFeature;
  if (_bufferIsInternal && _pBuffer != NULL )
    delete _pBuffer;
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FeatureFileReaderSingle_cpp)

