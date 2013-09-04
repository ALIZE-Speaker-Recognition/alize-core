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

#if !defined(ALIZE_FeatureServer_cpp)
#define ALIZE_FeatureServer_cpp

#include <new>
#include "FeatureServer.h"
#include "Feature.h"
#include "alizeString.h"
#include "FeatureFileReader.h"
#include "FeatureInputStreamModifier.h"
#include "Config.h"
#include "XLine.h"

using namespace alize;
typedef FeatureServer S;

//-------------------------------------------------------------------------
S::FeatureServer()
:_pInputStream(NULL) {}
//-------------------------------------------------------------------------
S::FeatureServer(const Config& c)
:_pInputStream(NULL) { init(c); }
//-------------------------------------------------------------------------
void S::init(const Config& c)
{
  releaseAll();
  FeatureInputStream::init(c);
  _pInputStream = &FeatureFileReader::create(c);
  init();
}
//-------------------------------------------------------------------------
S::FeatureServer(const Config& c, FeatureInputStream& s)
:_pInputStream(NULL) { init(c, s); }
//-------------------------------------------------------------------------
void S::init(const Config& c, FeatureInputStream& s)
{
  releaseAll();
  FeatureInputStream::init(c);
  _ownInputStream = false;
  _pInputStream = &s;
  init();
}
//-------------------------------------------------------------------------
S::FeatureServer(const Config& c, const FileName& f, LabelServer& ls)
:_pInputStream(NULL) { init(c, f, ls); }
//-------------------------------------------------------------------------
void S::init(const Config& c, const FileName& f, LabelServer& ls)
{
  releaseAll();
  FeatureInputStream::init(c);
  _pLabelServer = &ls;
  _pInputStream = &FeatureFileReader::createStream(f, c, &ls,
    BIGENDIAN_AUTO, defineBufferUsage(), defineBufferSize(),
    defineHistoricUsage(), defineHistoricSize());
  init();
}
//-------------------------------------------------------------------------
S::FeatureServer(const Config& c, const FileName& f)
:_pInputStream(NULL) { init(c, f); }
//-------------------------------------------------------------------------
void S::init(const Config& c, const FileName& f)
{
  releaseAll();
  FeatureInputStream::init(c);
  _pInputStream = &FeatureFileReader::createStream(f, c, NULL,
    BIGENDIAN_AUTO, defineBufferUsage(), defineBufferSize(),
    defineHistoricUsage(), defineHistoricSize());
  init();
}
//-------------------------------------------------------------------------
S::FeatureServer(const Config& c, const XLine& l, LabelServer& ls)
:_pInputStream(NULL) { init(c, l, ls); }
//-------------------------------------------------------------------------
void S::init(const Config& c, const XLine& l, LabelServer& ls)
{
  releaseAll();
  FeatureInputStream::init(c);
  _pLabelServer = &ls;
  _pInputStream = &FeatureFileReader::createStream(l, c, &ls,
    BIGENDIAN_AUTO, defineBufferUsage(), defineBufferSize(),
    defineHistoricUsage(), defineHistoricSize());
  init();
}
//-------------------------------------------------------------------------
S::FeatureServer(const Config& c, const XLine& l)
:_pInputStream(NULL) { init(c, l); }
//-------------------------------------------------------------------------
void S::init(const Config& c, const XLine& l)
{
  releaseAll();
  FeatureInputStream::init(c);
  _pInputStream = &FeatureFileReader::createStream(l, c, NULL,
    BIGENDIAN_AUTO, defineBufferUsage(), defineBufferSize(),
    defineHistoricUsage(), defineHistoricSize());
  init();
}
//-------------------------------------------------------------------------
void S::init() // private
{
  const Config& config = this->getConfig();
  if (config.existsParam_featureServerMask)
    if (_pInputStream != NULL)
    {
      _pInputStream = &FeatureInputStreamModifier::create(inputStream(),
                  config.getParam_featureServerMask(), _ownInputStream);
      _ownInputStream = true;
    }
  bool vectSizeDefined = false;
  unsigned long vectSize = 0;
  try
  {
    vectSize = getVectSize();
    vectSizeDefined = true;
  }
  catch (Exception&) {}
  if (vectSizeDefined)
  {
    if (!config.existsParam_vectSize)
      const_cast<Config&>(config).setParam("vectSize", String::valueOf(vectSize));
    else if (config.getParam_vectSize() != vectSize){

      throw Exception("vectSize from config ("
            + String::valueOf(config.getParam_vectSize())
            + ") is not equal to vectSize from file ("
            + String::valueOf(getVectSize()) + ")", __FILE__, __LINE__);
	}
  }
  /*try
  {
    const_cast<Config&>(config).setParam("vectSize", getVectSize());
  }
  catch (Exception&) {} // thrown if getVectSize() fail*/
  reset();
}
//-------------------------------------------------------------------------
void S::reset()
{
  if (_pInputStream != NULL)
    seekFeature(0);
}
//-------------------------------------------------------------------------
HistoricUsage S::defineHistoricUsage() const
{
  if (!getConfig().existsParam_featureServerBufferSize ||
      getConfig().getParam_featureServerBufferSize() == "ALL_FEATURES")
    return ALL_FEATURES;
  return LIMITED;
}
//-------------------------------------------------------------------------
unsigned long S::defineHistoricSize() const
{
  if (!getConfig().existsParam_featureServerBufferSize)
    return 0; // size not used
  const String& s = getConfig().getParam_featureServerBufferSize();
  if (s == "ALL_FEATURES")
    return 0; // size not used
  return s.toULong();
}
//-------------------------------------------------------------------------
BufferUsage S::defineBufferUsage() const // private
{
  if (getConfig().existsParam_featureServerMemAlloc)
    return BUFFER_USERDEFINE;
  return BUFFER_AUTO;
}
//-------------------------------------------------------------------------
unsigned long S::defineBufferSize() const // private
{
  if (getConfig().existsParam_featureServerMemAlloc)
    return getConfig().getParam_featureServerMemAlloc();
  return 0;
}
//-------------------------------------------------------------------------
void S::close()
{
  if (_pInputStream != NULL)
    inputStream().close();
}
//-------------------------------------------------------------------------
void S::seekFeature(unsigned long featureNbr, const String& srcName)
{
  if (_pInputStream != NULL)
    inputStream().seekFeature(featureNbr, srcName);
}
//-------------------------------------------------------------------------
bool S::writeFeature(const Feature& f, unsigned long step)
{
  bool ok = inputStream().writeFeature(f, step);
  _error = inputStream().getError();
  return ok;
}
//-------------------------------------------------------------------------
bool S::readFeature(Feature& f, unsigned long step)
{
  if (_pInputStream == NULL)
    return false;
  bool ok = inputStream().readFeature(f, step);
  _error = inputStream().getError();
  return ok;
}
//-------------------------------------------------------------------------
bool S::addFeature(const Feature& f)
{
  if (_pInputStream == NULL)
    return false;
  if (!getConfig().existsParam_featureServerMemAlloc)
  	throw Exception("featureServerMemAlloc needed to be able to add Feature to a featureServer", __FILE__, __LINE__);
  bool ok = inputStream().addFeature(f);
  _error = inputStream().getError();
  return ok;
}
//-------------------------------------------------------------------------
const String& S::getServerName() const { return _serverName; }
//-------------------------------------------------------------------------
void S::setServerName(const String& s) { _serverName = s; }
//-------------------------------------------------------------------------
unsigned long S::getFeatureCount() 
{
  if (_pInputStream != NULL)
    return inputStream().getFeatureCount();
  return 0;
}
//-------------------------------------------------------------------------
unsigned long S::getVectSize() { return inputStream().getVectSize(); }
//-------------------------------------------------------------------------
const FeatureFlags& S::getFeatureFlags() 
{ return inputStream().getFeatureFlags(); }
//-------------------------------------------------------------------------
real_t S::getSampleRate() { return inputStream().getSampleRate(); }
//-------------------------------------------------------------------------
unsigned long S::getSourceCount()
{
  if (_pInputStream != NULL)
    return inputStream().getSourceCount();
  return 0;
}
//-------------------------------------------------------------------------
unsigned long S::getFeatureCountOfASource(unsigned long srcIdx)
{ return inputStream().getFeatureCountOfASource(srcIdx); }
//-------------------------------------------------------------------------
unsigned long S::getFeatureCountOfASource(const String& f)
{ return inputStream().getFeatureCountOfASource(f); }
//-------------------------------------------------------------------------
unsigned long S::getFirstFeatureIndexOfASource(unsigned long srcIdx)
{ return inputStream().getFirstFeatureIndexOfASource(srcIdx); }
//-------------------------------------------------------------------------
unsigned long S::getFirstFeatureIndexOfASource(const String& srcName)
{ return inputStream().getFirstFeatureIndexOfASource(srcName); }
//-------------------------------------------------------------------------
const String& S::getNameOfASource(unsigned long srcIdx)
{ return inputStream().getNameOfASource(srcIdx); }
//-------------------------------------------------------------------------
FeatureInputStream& S::inputStream()
{
  if (_pInputStream == NULL)
    throw Exception("No feature stream in this server", __FILE__, __LINE__);
  return *_pInputStream;
}
//-------------------------------------------------------------------------
String S::toString() const
{
  String s = Object::toString()
    + "\n  serverName = '" + _serverName + "'";
  if (_pInputStream != NULL)
  {
    s += "\n  vectSize   = " + String::valueOf(_pInputStream->getVectSize())
    + "\n  flags    = " + _pInputStream->getFeatureFlags().getString();
  }
  else
    s += "\n  vectSize   = UNKNOWN\n  flags    = UNKNOWN";
  return s;
}
//-------------------------------------------------------------------------
String S::getClassName() const { return "FeatureServer"; }
//-------------------------------------------------------------------------
void S::releaseAll()
{
  if (_pInputStream != NULL && _ownInputStream)
    delete _pInputStream;
  _pInputStream = NULL;
  _pLabelServer = NULL;
  _ownInputStream = true;
}
//-------------------------------------------------------------------------
S::~FeatureServer() { releaseAll(); }
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FeatureServer_cpp)

