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

#if !defined(ALIZE_FeatureFileReader_cpp)
#define ALIZE_FeatureFileReader_cpp

#include <new>
#include "FeatureFileReader.h"
#include "FeatureMultipleFileReader.h"
#include "FeatureFileReaderRaw.h"
#include "FeatureFileReaderSPro3.h"
#include "FeatureFileReaderSPro4.h"
#include "FeatureFileReaderHTK.h"
#include "Feature.h"
#include "Exception.h"
#include "LabelServer.h"
#include "Label.h"
#include "FeatureFlags.h"
#include "XLine.h"
#include "Config.h"

using namespace alize;
typedef FeatureFileReader R;

//-------------------------------------------------------------------------
R::FeatureFileReader(const Config& c)
:FeatureFileReaderAbstract(NULL, c, NULL, BUFFER_USERDEFINE, 0,
                           ALL_FEATURES, 0),
 _pFeatureReader(NULL) {}
//-------------------------------------------------------------------------
R::FeatureFileReader(const FileName& f, const Config& c, LabelServer* p,
                     BigEndian be, BufferUsage b, unsigned long bufferSize,
                     HistoricUsage h, unsigned long historicSize)
:FeatureFileReaderAbstract(NULL, c, NULL, b, bufferSize, h, historicSize),
 _pFeatureReader(&createStream(f, c, p, be, b, bufferSize, h, historicSize))
 {}
//-------------------------------------------------------------------------
// static method
FeatureFileReaderAbstract& R::createStream(const Config& c)
{ return FeatureFileReader::create(c); }
//-------------------------------------------------------------------------
// static method
FeatureFileReaderAbstract& R::createStream(const XLine& l, const Config& c,
     LabelServer* p, BigEndian be, BufferUsage b, unsigned long bufferSize,
     HistoricUsage h, unsigned long historicSize)
{
  return FeatureMultipleFileReader::create(l, c, p, be, b, bufferSize,
                                           h, historicSize);
}
//-------------------------------------------------------------------------
// static method
FeatureFileReaderAbstract& R::createStream(const FileName& f,
             const Config& c, LabelServer* p, BigEndian be, BufferUsage b,
             unsigned long bufferSize, HistoricUsage h,
             unsigned long historicSize)
{
  if (f.endsWith(".lst"))
    return FeatureMultipleFileReader::create(
                 XList(f, c).getAllElements(), c, p, be, b, bufferSize, h, historicSize);
  if (c.existsParam_loadFeatureFileFormat)
    switch (c.getParam_loadFeatureFileFormat())
    {
    case FeatureFileReaderFormat_SPRO3:
        return FeatureFileReaderSPro3::create(f, c, p, b, bufferSize, h, historicSize);
    case FeatureFileReaderFormat_SPRO4:
        return FeatureFileReaderSPro4::create(f, c, p, be, b, bufferSize, h, historicSize);
    case FeatureFileReaderFormat_HTK:
        return FeatureFileReaderHTK::create(f, c, p, be, b, bufferSize, h, historicSize);
    case FeatureFileReaderFormat_RAW:
        return FeatureFileReaderRaw::create(f, c, p, be, b, bufferSize, h, historicSize);
    }
  throw Exception("Param 'loadFeatureFileFormat' expected in the config",
                  __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
R::FeatureFileReader(const XLine& l, const Config& c, LabelServer* p,
                     BigEndian be, BufferUsage b, unsigned long bufferSize,
                     HistoricUsage h, unsigned long historicSize)
:FeatureFileReaderAbstract(NULL, c, NULL, b, bufferSize, h, historicSize),
 _pFeatureReader(&FeatureMultipleFileReader::create(l, c, p, be, b,
 bufferSize, h, historicSize)) {}
//-------------------------------------------------------------------------
R& R::create(const Config& c)
{
  FeatureFileReader* p = new (std::nothrow) FeatureFileReader(c);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
R& R::create(const FileName& f, const Config& c, LabelServer* l,
             BigEndian be, BufferUsage b, unsigned long bufferSize,
             HistoricUsage h, unsigned long historicSize)
{
  R* p = new (std::nothrow) R(f, c, l, be, b, bufferSize, h, historicSize);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
R& R::create(const XLine& l, const Config& c, LabelServer* ls,
             BigEndian be, BufferUsage b, unsigned long bufferSize,
             HistoricUsage h, unsigned long historicSize)
{
  R* p = new (std::nothrow) R(l, c, ls, be, b, bufferSize, h, historicSize);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
bool R::readFeature(Feature& f, unsigned long step)
{
  if (_pFeatureReader == NULL)
    return false;
  if (_seekWanted)
  {
    _seekWanted = false;
    _pFeatureReader->seekFeature(_seekWantedIdx, _seekWantedSrcName);
  }
  bool ok = _pFeatureReader->readFeature(f, step);
  _error = _pFeatureReader->getError();
  return ok;
}
//-------------------------------------------------------------------------
bool R::addFeature(const Feature& f)
{
  if (_pFeatureReader == NULL)
    return false;
  bool ok = _pFeatureReader->addFeature(f);
//  _error = _pFeatureReader->getError();
  return ok;
}
//-------------------------------------------------------------------------
void R::seekFeature(unsigned long featureNbr, const String& sourceName)
{
  if (_pFeatureReader != NULL)
      _pFeatureReader->seekFeature(featureNbr, sourceName);
}
//-------------------------------------------------------------------------
bool R::writeFeature(const Feature& f, unsigned long step)
{
  if (_pFeatureReader == NULL)
    throw Exception("feature writing forbidden", __FILE__, __LINE__);
  if (_seekWanted)
  {
    _seekWanted = false;
    _pFeatureReader->seekFeature(_seekWantedIdx, _seekWantedSrcName);
  }
  bool ok = _pFeatureReader->writeFeature(f, step);
  _error = _pFeatureReader->getError();
  return ok;
}
//-------------------------------------------------------------------------
unsigned long R::getFeatureCount()
{
  if (_pFeatureReader != NULL)
    return _pFeatureReader->getFeatureCount();
  return 0;
}
//-------------------------------------------------------------------------
unsigned long R::getVectSize()
{
  if (_pFeatureReader == NULL)
    throw Exception("no file to read", __FILE__, __LINE__);
  return _pFeatureReader->getVectSize();
}
//-------------------------------------------------------------------------
void R::setExternalBufferToUse(FloatVector& v)
{ _pFeatureReader->setExternalBufferToUse(v); }
//-------------------------------------------------------------------------
const FeatureFlags& R::getFeatureFlags()
{
  if (_pFeatureReader == NULL)
    throw Exception("no file to read", __FILE__, __LINE__);
  return _pFeatureReader->getFeatureFlags();
}
//-------------------------------------------------------------------------
real_t R::getSampleRate() 
{
  if (_pFeatureReader == NULL)
    throw Exception("no file to read", __FILE__, __LINE__);
  return _pFeatureReader->getSampleRate();
}
//-------------------------------------------------------------------------
void R::close()
{
  if (_pFeatureReader != NULL)
    _pFeatureReader->close();
}
//-------------------------------------------------------------------------
unsigned long R::getSourceCount()
{
  if (_pFeatureReader != NULL)
    return _pFeatureReader->getSourceCount();
  return 0;
}
//-------------------------------------------------------------------------
unsigned long R::getFeatureCountOfASource(unsigned long srcIdx)
{
  if (_pFeatureReader == NULL)
    throw Exception("No source of features", __FILE__, __LINE__);
  return _pFeatureReader->getFeatureCountOfASource(srcIdx);
}
//-------------------------------------------------------------------------
unsigned long R::getFeatureCountOfASource(const FileName& f)
{
  if (_pFeatureReader == NULL)
    throw Exception("No source of features", __FILE__, __LINE__);
  return _pFeatureReader->getFeatureCountOfASource(f);
}
//-------------------------------------------------------------------------
unsigned long R::getFirstFeatureIndexOfASource(unsigned long srcIdx)
{
  if (_pFeatureReader == NULL)
    throw Exception("No source of features", __FILE__, __LINE__);
  return _pFeatureReader->getFirstFeatureIndexOfASource(srcIdx);
}
//-------------------------------------------------------------------------
unsigned long R::getFirstFeatureIndexOfASource(const FileName& f)
{
  if (_pFeatureReader == NULL)
    throw Exception("No source of features", __FILE__, __LINE__);
  return _pFeatureReader->getFirstFeatureIndexOfASource(f);
}
//-------------------------------------------------------------------------
const String& R::getNameOfASource(unsigned long srcIdx)
{
  if (_pFeatureReader == NULL)
    throw Exception("No source of features", __FILE__, __LINE__);
  return _pFeatureReader->getNameOfASource(srcIdx); }
//-------------------------------------------------------------------------
String R::getClassName() const{return "FeatureFileReader";}
//-------------------------------------------------------------------------
R::~FeatureFileReader()
{
  if (_pFeatureReader != NULL)
    delete _pFeatureReader;
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FeatureFileReader_cpp)

