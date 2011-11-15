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

#if !defined(ALIZE_Config_cpp)
#define ALIZE_Config_cpp

#include "Config.h"
#include "FeatureFlags.h"
#include "ConfigFileWriter.h"
#include "ConfigFileReaderXml.h"
#include "ConfigFileReaderRaw.h"
#include "Exception.h"

using namespace alize;

//-------------------------------------------------------------------------
Config::Config()
:Object() { reset(); }
//-------------------------------------------------------------------------
Config::Config(const FileName& f)
:Object() { load(f); }
//-------------------------------------------------------------------------
Config::Config(const Config& c)
:Object() { assign(c); }
//-------------------------------------------------------------------------
const Config& Config::operator=(const Config& c)
{ // TODO : test auto-assignment
  assign(c);
  return *this;
}
//-------------------------------------------------------------------------
void Config::assign(const Config& c) // private
{
  #define ASSIGN(x) x = c.x
  ASSIGN(_param_minCov);
  ASSIGN(_param_vectSize);
  ASSIGN(_param_loadFeatureFileMemAlloc);
  ASSIGN(_param_featureServerMemAlloc);
  ASSIGN(_param_computeLLKWithTopDistribs);
  ASSIGN(_param_debug);
  ASSIGN(_param_topDistribsCount);
  ASSIGN(_param_featureServerBufferSize);
  ASSIGN(_param_featureServerMask);
  ASSIGN(_param_featureFlags);
  ASSIGN(_param_mixtureDistribCount);
  ASSIGN(_param_loadFeatureFileFormat);
  ASSIGN(_param_loadFeatureFileVectSize);
  ASSIGN(_param_loadAudioFileChannel);
  ASSIGN(_param_loadMixtureFileFormat);
  ASSIGN(_param_loadSegServerFileFormat);
  ASSIGN(_param_saveMixtureFileFormat);
  ASSIGN(_param_saveMixtureServerFileFormat);
  ASSIGN(_param_saveSegServerFileFormat);
  ASSIGN(_param_saveFeatureFileFormat);
  ASSIGN(_param_saveFeatureFileSPro3DataKind);
  ASSIGN(_param_loadFeatureFileExtension);
  ASSIGN(_param_loadAudioFileExtension);
  ASSIGN(_param_loadFeatureFileBigEndian);
  ASSIGN(_param_loadAudioFileBigEndian);
  ASSIGN(_param_featureServerMode);
  ASSIGN(_param_loadMixtureFileBigEndian);
  ASSIGN(_param_loadMixtureFileExtension);
  ASSIGN(_param_loadSegServerFileExtension);
  ASSIGN(_param_saveMixtureFileExtension);
  ASSIGN(_param_saveMixtureServerFileExtension);
  ASSIGN(_param_saveSegServerFileExtension);
  ASSIGN(_param_saveFeatureFileExtension);
  ASSIGN(_param_distribType);
  ASSIGN(_param_mixtureFilesPath);
  ASSIGN(_param_featureFilesPath);
  ASSIGN(_param_audioFilesPath);
  ASSIGN(_param_segServerFilesPath);
  ASSIGN(_param_minLLK);
  ASSIGN(_param_maxLLK);
  ASSIGN(_param_bigEndian);
  ASSIGN(_param_sampleRate);

  ASSIGN(existsParam_minCov);
  ASSIGN(existsParam_vectSize);
  ASSIGN(existsParam_loadFeatureFileMemAlloc);
  ASSIGN(existsParam_featureServerMemAlloc);
  ASSIGN(existsParam_computeLLKWithTopDistribs);
  ASSIGN(existsParam_debug);
  ASSIGN(existsParam_topDistribsCount);
  ASSIGN(existsParam_featureServerBufferSize);
  ASSIGN(existsParam_featureServerMask);
  ASSIGN(existsParam_loadFeatureFileFormat);
  ASSIGN(existsParam_loadFeatureFileVectSize);
  ASSIGN(existsParam_loadAudioFileChannel);
  ASSIGN(existsParam_loadMixtureFileFormat);
  ASSIGN(existsParam_loadSegServerFileFormat);
  ASSIGN(existsParam_saveMixtureFileFormat);
  ASSIGN(existsParam_saveMixtureServerFileFormat);
  ASSIGN(existsParam_saveSegServerFileFormat);
  ASSIGN(existsParam_saveFeatureFileFormat);
  ASSIGN(existsParam_saveFeatureFileSPro3DataKind);
  ASSIGN(existsParam_loadFeatureFileExtension);
  ASSIGN(existsParam_loadAudioFileExtension);
  ASSIGN(existsParam_loadFeatureFileBigEndian);
  ASSIGN(existsParam_loadAudioFileBigEndian);
  ASSIGN(existsParam_featureServerMode);
  ASSIGN(existsParam_loadMixtureFileBigEndian);
  ASSIGN(existsParam_loadMixtureFileExtension);
  ASSIGN(existsParam_loadSegServerFileExtension);
  ASSIGN(existsParam_saveMixtureFileExtension);
  ASSIGN(existsParam_saveMixtureServerFileExtension);
  ASSIGN(existsParam_saveSegServerFileExtension);
  ASSIGN(existsParam_saveFeatureFileExtension);
  ASSIGN(existsParam_featureFlags);
  ASSIGN(existsParam_mixtureDistribCount);
  ASSIGN(existsParam_minLLK);
  ASSIGN(existsParam_maxLLK);
  ASSIGN(existsParam_distribType);
  ASSIGN(existsParam_saveMixtureFileFormat);
  ASSIGN(existsParam_saveMixtureServerFileFormat);
  ASSIGN(existsParam_saveSegServerFileFormat);
  ASSIGN(existsParam_saveFeatureFileFormat);
  ASSIGN(existsParam_saveFeatureFileSPro3DataKind);
  ASSIGN(existsParam_bigEndian);
  ASSIGN(existsParam_sampleRate);
  ASSIGN(existsParam_featureFilesPath);
  ASSIGN(existsParam_audioFilesPath);
  ASSIGN(existsParam_segServerFilesPath);
  ASSIGN(existsParam_mixtureFilesPath);
  ASSIGN(_set);
}
//-------------------------------------------------------------------------
bool Config::operator==(const Config& c) const
{
  unsigned long i;
  for (i=0; i<getParamCount(); i++)
  {
    const String& name = getParamName(i);
    if (!c.existsParam(name))
      return false;
    if (getParamContent(i) != c.getParam(name))
      return false;
  }
  for (i=0; i<c.getParamCount(); i++)
  {
    const String& name = c.getParamName(i);
    if (!existsParam(name))
      return false;
    if (c.getParamContent(i) != getParam(name))
      return false;
  }
  return true;
}  
//-------------------------------------------------------------------------
bool Config::operator!=(const Config& c) const { return !(*this == c); }
//-------------------------------------------------------------------------
void Config::reset()
{
  existsParam_minCov = false;
  existsParam_vectSize = false;
  existsParam_loadFeatureFileMemAlloc = false;
  existsParam_featureServerMemAlloc = false;
  existsParam_topDistribsCount = false;
  existsParam_featureServerBufferSize = false;
  existsParam_featureServerMask = false;
  existsParam_featureFlags = false;
  existsParam_mixtureDistribCount = false;
  existsParam_minLLK = false;
  existsParam_maxLLK = false;
  existsParam_distribType = false;
  existsParam_saveMixtureFileFormat = false;
  existsParam_saveMixtureServerFileFormat = false;
  existsParam_saveSegServerFileFormat = false;
  existsParam_saveFeatureFileFormat = false;
  existsParam_saveFeatureFileSPro3DataKind = false;
  existsParam_loadFeatureFileFormat = false;
  existsParam_loadFeatureFileVectSize = false;
  existsParam_loadAudioFileChannel = false;
  existsParam_loadMixtureFileFormat = false;
  existsParam_loadSegServerFileFormat = false;
  existsParam_saveMixtureFileExtension = false;
  existsParam_saveMixtureServerFileExtension = false;
  existsParam_saveSegServerFileExtension = false;
  existsParam_saveFeatureFileExtension = false;
  existsParam_loadFeatureFileExtension = false;
  existsParam_loadAudioFileExtension = false;
  existsParam_loadFeatureFileBigEndian = false;
  existsParam_loadAudioFileBigEndian = false;
  existsParam_featureServerMode = false;
  existsParam_loadMixtureFileBigEndian = false;
  existsParam_loadMixtureFileExtension = false;
  existsParam_loadSegServerFileExtension = false;
  existsParam_bigEndian = false;
  existsParam_sampleRate = false;
  existsParam_mixtureFilesPath = false;
  existsParam_featureFilesPath = false;
  existsParam_audioFilesPath = false;
  existsParam_segServerFilesPath = false;
  _set.reset();
  setParam("debug", "false"); // always defined
}
//-------------------------------------------------------------------------
void Config::load(const FileName& f)
{
  if (f.endsWith(".xml"))
    ConfigFileReaderXml(f).readConfig(*this);
  else
    ConfigFileReaderRaw(f).readConfig(*this);
  // readConfig() can throw FileNotFoundException, IOException
}
//-------------------------------------------------------------------------
void Config::save(const FileName& f) const
{ ConfigFileWriter(f).writeConfig(*this); }
//-------------------------------------------------------------------------
unsigned long Config::getParamCount() const { return _set.getLineCount(); }
//-------------------------------------------------------------------------
const String& Config::getParamName(unsigned long i) const
{ return _set.getLine(i).getElement(0); }
//-------------------------------------------------------------------------
const String& Config::getParamContent(unsigned long i) const
{ return _set.getLine(i).getElement(1); }
//-------------------------------------------------------------------------
const String& Config::getParam(const String& name) const
{
  XLine* p =_set.findLine(name);
  if (p == NULL)
    throw ParamNotFoundInConfigException(name, __FILE__, __LINE__);
  return p->getElement(1);
}
//-------------------------------------------------------------------------
long Config::getIntegerParam(const String& name) const
{ return getParam(name).toLong(); }
//-------------------------------------------------------------------------
double Config::getFloatParam(const String& name) const
{ return getParam(name).toDouble(); }
//-------------------------------------------------------------------------
bool Config::getBooleanParam(const String& name) const
{
  const String& s = getParam(name);
  if (s == "")
    return true;
  return s.toBool();
}
//-------------------------------------------------------------------------
bool Config::existsParam(const String& name) const
{ return _set.findLine(name) != NULL; }
//-------------------------------------------------------------------------
real_t Config::getParam_minCov() const
{
  if (!existsParam_minCov)
    throw ParamNotFoundInConfigException("minCov", __FILE__,  __LINE__);
  return _param_minCov;
}
//-------------------------------------------------------------------------
unsigned long Config::getParam_vectSize() const
{
  if (!existsParam_vectSize)
    throw ParamNotFoundInConfigException("vectSize", __FILE__, __LINE__);
  return _param_vectSize;
}
//-------------------------------------------------------------------------
unsigned long Config::getParam_loadFeatureFileMemAlloc() const
{
  if (!existsParam_loadFeatureFileMemAlloc)
    throw ParamNotFoundInConfigException("loadFeatureFileMemAlloc' in the config",
                    __FILE__, __LINE__);
  return _param_loadFeatureFileMemAlloc;
}
//-------------------------------------------------------------------------
unsigned long Config::getParam_featureServerMemAlloc() const
{
  if (!existsParam_featureServerMemAlloc)
    throw ParamNotFoundInConfigException("featureServerMemAlloc' in the config",
                    __FILE__, __LINE__);
  return _param_featureServerMemAlloc;
}
//-------------------------------------------------------------------------
bool Config::getParam_computeLLKWithTopDistribs() const
{
  if (!existsParam_computeLLKWithTopDistribs)
    throw ParamNotFoundInConfigException("computeLLKWithTopDistribs' in the config",
                             __FILE__, __LINE__);
  return _param_computeLLKWithTopDistribs;
}
//-------------------------------------------------------------------------
bool Config::getParam_debug() const { return _param_debug; }
//-------------------------------------------------------------------------
unsigned long Config::getParam_topDistribsCount() const
{
  if (!existsParam_topDistribsCount)
    throw ParamNotFoundInConfigException("topDistribsCount' in the config", __FILE__,
                                __LINE__);
  return _param_topDistribsCount;
}
//-------------------------------------------------------------------------
const String& Config::getParam_featureServerBufferSize() const
{
  if (!existsParam_featureServerBufferSize)
    throw ParamNotFoundInConfigException("featureServerBufferSize' in the config",
                        __FILE__, __LINE__);
  return _param_featureServerBufferSize;
}
//-------------------------------------------------------------------------
const String& Config::getParam_featureServerMask() const
{
  if (!existsParam_featureServerMask)
    throw ParamNotFoundInConfigException("featureServermask' in the config",
                        __FILE__, __LINE__);
  return _param_featureServerMask;
}
//-------------------------------------------------------------------------
const FeatureFlags& Config::getParam_featureFlags() const
{
  if (!existsParam_featureFlags)
    throw ParamNotFoundInConfigException("featureFlags' in the config", __FILE__,
                                __LINE__);
  return _param_featureFlags;
}
//-------------------------------------------------------------------------
unsigned long Config::getParam_mixtureDistribCount() const
{
  if (!existsParam_mixtureDistribCount)
    throw ParamNotFoundInConfigException("mixtureDistribCount' in the config",
                            __FILE__, __LINE__);
  return _param_mixtureDistribCount;
}
//-------------------------------------------------------------------------
lk_t Config::getParam_minLLK() const
{
  if (!existsParam_minLLK)
    throw ParamNotFoundInConfigException("minLLK", __FILE__, __LINE__);
  return _param_minLLK;
}
//-------------------------------------------------------------------------
lk_t Config::getParam_maxLLK() const
{
  if (!existsParam_maxLLK)
    throw ParamNotFoundInConfigException("maxLLK' in the config", __FILE__,__LINE__);
  return _param_maxLLK;
}
//-------------------------------------------------------------------------
DistribType Config::getParam_distribType() const
{
  if (!existsParam_distribType)
    throw ParamNotFoundInConfigException("distribType' in the config",
                            __FILE__, __LINE__);
  return _param_distribType;
}
//-------------------------------------------------------------------------
MixtureFileWriterFormat Config::getParam_saveMixtureFileFormat() const
{
  if (!existsParam_saveMixtureFileFormat)
    throw ParamNotFoundInConfigException("saveMixtureFileFormat' in the config",
                            __FILE__, __LINE__);
  return _param_saveMixtureFileFormat;
}
//-------------------------------------------------------------------------
MixtureServerFileWriterFormat Config::getParam_saveMixtureServerFileFormat() const
{
  if (!existsParam_saveMixtureServerFileFormat)
    throw ParamNotFoundInConfigException("saveMixtureServerFileFormat' in the config",
                            __FILE__, __LINE__);
  return _param_saveMixtureServerFileFormat;
}
//-------------------------------------------------------------------------
SegServerFileWriterFormat Config::getParam_saveSegServerFileFormat() const
{
  if (!existsParam_saveSegServerFileFormat)
    throw ParamNotFoundInConfigException("saveSegServerFileFormat' in the config",
                            __FILE__, __LINE__);
  return _param_saveSegServerFileFormat;
}
//-------------------------------------------------------------------------
FeatureFileWriterFormat Config::getParam_saveFeatureFileFormat() const
{
  if (!existsParam_saveFeatureFileFormat)
    throw ParamNotFoundInConfigException("saveFeatureFileFormat' in the config",
                            __FILE__, __LINE__);
  return _param_saveFeatureFileFormat;
}
//-------------------------------------------------------------------------
SPRO3DataKind Config::getParam_saveFeatureFileSPro3DataKind() const
{
  if (!existsParam_saveFeatureFileSPro3DataKind)
    throw ParamNotFoundInConfigException("saveFeatureFileSPro3DataKind' in the config",
                            __FILE__, __LINE__);
  return _param_saveFeatureFileSPro3DataKind;
}
//-------------------------------------------------------------------------
FeatureFileReaderFormat Config::getParam_loadFeatureFileFormat() const
{
  if (!existsParam_loadFeatureFileFormat)
    throw ParamNotFoundInConfigException("loadFeatureFileFormat' in the config",
                            __FILE__, __LINE__);
  return _param_loadFeatureFileFormat;
}
//-------------------------------------------------------------------------
unsigned long Config::getParam_loadFeatureFileVectSize() const
{
  if (!existsParam_loadFeatureFileVectSize)
    throw ParamNotFoundInConfigException("loadFeatureFileVectSize' in the config",
                            __FILE__, __LINE__);
  return _param_loadFeatureFileVectSize;
}
//-------------------------------------------------------------------------
unsigned long Config::getParam_loadAudioFileChannel() const
{
  if (!existsParam_loadAudioFileChannel)
    throw ParamNotFoundInConfigException("loadAudioFileChannel' in the config",
                            __FILE__, __LINE__);
  return _param_loadAudioFileChannel;
}
//-------------------------------------------------------------------------
MixtureFileReaderFormat Config::getParam_loadMixtureFileFormat() const
{
  if (!existsParam_loadMixtureFileFormat)
    throw ParamNotFoundInConfigException("loadMixtureFileFormat' in the config",
                            __FILE__, __LINE__);
  return _param_loadMixtureFileFormat;
}
//-------------------------------------------------------------------------
SegServerFileReaderFormat Config::getParam_loadSegServerFileFormat() const
{
  if (!existsParam_loadSegServerFileFormat)
    throw ParamNotFoundInConfigException("loadSegServerFileFormat' in the config",
                            __FILE__, __LINE__);
  return _param_loadSegServerFileFormat;
}
//-------------------------------------------------------------------------
const String& Config::getParam_saveMixtureFileExtension() const
{
  if (!existsParam_saveMixtureFileExtension)
    throw ParamNotFoundInConfigException("saveMixtureFileExtension' in the config",
                            __FILE__, __LINE__);
  return _param_saveMixtureFileExtension;
}
//-------------------------------------------------------------------------
const String& Config::getParam_saveMixtureServerFileExtension() const
{
  if (!existsParam_saveMixtureServerFileExtension)
    throw ParamNotFoundInConfigException("saveMixtureServerFileExtension' in the config",
                            __FILE__, __LINE__);
  return _param_saveMixtureServerFileExtension;
}
//-------------------------------------------------------------------------
const String& Config::getParam_saveSegServerFileExtension() const
{
  if (!existsParam_saveSegServerFileExtension)
    throw ParamNotFoundInConfigException("saveSegServerFileExtension' in the config",
                            __FILE__, __LINE__);
  return _param_saveSegServerFileExtension;
}
//-------------------------------------------------------------------------
const String& Config::getParam_saveFeatureFileExtension() const
{
  if (!existsParam_saveFeatureFileExtension)
    throw ParamNotFoundInConfigException("saveFeatureFileExtension' in the config",
                            __FILE__, __LINE__);
  return _param_saveFeatureFileExtension;
}
//-------------------------------------------------------------------------
const String& Config::getParam_loadFeatureFileExtension() const
{
  if (!existsParam_loadFeatureFileExtension)
    throw ParamNotFoundInConfigException("loadFeatureFileExtension' in the config",
                            __FILE__, __LINE__);
  return _param_loadFeatureFileExtension;
}
//-------------------------------------------------------------------------
const String& Config::getParam_loadAudioFileExtension() const
{
  if (!existsParam_loadAudioFileExtension)
    throw ParamNotFoundInConfigException("loadAudioFileExtension' in the config",
                            __FILE__, __LINE__);
  return _param_loadAudioFileExtension;
}
//-------------------------------------------------------------------------
bool Config::getParam_loadFeatureFileBigEndian() const
{
  if (!existsParam_loadFeatureFileBigEndian)
    throw ParamNotFoundInConfigException("loadFeatureFileBigEndian' in the config",
                            __FILE__, __LINE__);
  return _param_loadFeatureFileBigEndian;
}
//-------------------------------------------------------------------------
bool Config::getParam_loadAudioFileBigEndian() const
{
  if (!existsParam_loadAudioFileBigEndian)
    throw ParamNotFoundInConfigException("loadAudioFileBigEndian' in the config",
                            __FILE__, __LINE__);
  return _param_loadAudioFileBigEndian;
}
//-------------------------------------------------------------------------
const String& Config::getParam_featureServerMode() const
{
  if (!existsParam_featureServerMode)
    throw ParamNotFoundInConfigException("featureServerMode' in the config",
                            __FILE__, __LINE__);
  return _param_featureServerMode;
}
//-------------------------------------------------------------------------
bool Config::getParam_loadMixtureFileBigEndian() const
{
  if (!existsParam_loadMixtureFileBigEndian)
    throw ParamNotFoundInConfigException("loadMixtureFileBigEndian' in the config",
                            __FILE__, __LINE__);
  return _param_loadMixtureFileBigEndian;
}
//-------------------------------------------------------------------------
const String& Config::getParam_loadMixtureFileExtension() const
{
  if (!existsParam_loadMixtureFileExtension)
    throw ParamNotFoundInConfigException("loadMixtureFileExtension' in the config",
                            __FILE__, __LINE__);
  return _param_loadMixtureFileExtension;
}
//-------------------------------------------------------------------------
const String& Config::getParam_loadSegServerFileExtension() const
{
  if (!existsParam_loadSegServerFileExtension)
    throw ParamNotFoundInConfigException("loadSegServerFileExtension' in the config",
                            __FILE__, __LINE__);
  return _param_loadSegServerFileExtension;
}
//-------------------------------------------------------------------------
const String& Config::getParam_featureFilesPath() const
{
  if (!existsParam_featureFilesPath)
    throw ParamNotFoundInConfigException("featureFilesPath' in the config",
                            __FILE__, __LINE__);
  return _param_featureFilesPath;
}
//-------------------------------------------------------------------------
const String& Config::getParam_audioFilesPath() const
{
  if (!existsParam_audioFilesPath)
    throw ParamNotFoundInConfigException("audioFilesPath' in the config",
                            __FILE__, __LINE__);
  return _param_audioFilesPath;
}
//-------------------------------------------------------------------------
const String& Config::getParam_segServerFilesPath() const
{
  if (!existsParam_segServerFilesPath)
    throw ParamNotFoundInConfigException("segServerFilesPath' in the config",
                            __FILE__, __LINE__);
  return _param_segServerFilesPath;
}
//-------------------------------------------------------------------------
const String& Config::getParam_mixtureFilesPath() const
{
  if (!existsParam_mixtureFilesPath)
    throw ParamNotFoundInConfigException("mixtureFilesPath' in the config",
                            __FILE__, __LINE__);
  return _param_mixtureFilesPath;
}
//-------------------------------------------------------------------------
bool Config::getParam_bigEndian() const
{
  if (!existsParam_bigEndian)
    throw ParamNotFoundInConfigException("bigEndian' in the config",
                            __FILE__, __LINE__);
  return _param_bigEndian;
}
//-------------------------------------------------------------------------
real_t Config::getParam_sampleRate() const
{
  if (!existsParam_sampleRate)
    throw ParamNotFoundInConfigException("sampleRate' in the config",
                              __FILE__, __LINE__);
  return _param_sampleRate;
}
//-------------------------------------------------------------------------
void Config::setParam(const String& name, const String& content)
{
  if (name == "minCov")
  {
    _param_minCov = content.toDouble();
    if (_param_minCov <= 0.0)
      throw Exception("parameter '"+name+"' cannot be <= 0.0",
               __FILE__, __LINE__);
    existsParam_minCov = true;
  }
  else if (name == "vectSize")
  {
    _param_vectSize = content.toULong();
    if (_param_vectSize == 0)
      throw Exception("parameter '"+name+"' cannot be 0",
              __FILE__, __LINE__);
    existsParam_vectSize = true;
  }
  else if (name == "loadFeatureFileMemAlloc")
  {
    _param_loadFeatureFileMemAlloc = content.toULong();
    existsParam_loadFeatureFileMemAlloc = true;
  }
  else if (name == "featureServerMemAlloc")
  {
    _param_featureServerMemAlloc = content.toULong();
    existsParam_featureServerMemAlloc = true;
  }
  else if (name == "computeLLKWithTopDistribs")
  {
    if (content == "COMPLETE")
      _param_computeLLKWithTopDistribs = true;
    else if (content == "PARTIAL")
      _param_computeLLKWithTopDistribs = false;
    else
      throw Exception(
           "parameter '"+name+"' must be 'COMPLETE' or 'PARTIAL'",
              __FILE__, __LINE__);
    existsParam_computeLLKWithTopDistribs = true;
  }
  else if (name == "topDistribsCount")
  {
    _param_topDistribsCount = content.toULong();
    if (_param_topDistribsCount == 0)
      throw Exception("parameter '"+name+"' cannot be 0",
              __FILE__, __LINE__);
    existsParam_topDistribsCount = true;
  }
  else if (name == "featureServerBufferSize")
  {
    if (content != "ALL_FEATURES")
    {
      if (content.isEmpty())
        throw Exception("parameter '"+name+"' cannot be empty",
                        __FILE__, __LINE__);
      for (unsigned long i=0; i<content.length(); i++)
      {
        if (content[i]<"0" || content[i]>"9")
          throw Exception("parameter '"+name+"' cannot be empty",
                        __FILE__, __LINE__);
      }
    }
      _param_featureServerBufferSize = content;
    existsParam_featureServerBufferSize = true;
  }
  else if (name == "featureServerMask")
  {
    _param_featureServerMask = content;
    existsParam_featureServerMask = true;
  }
  else if (name == "featureFlags")
  {
    _param_featureFlags.set(content);
    existsParam_featureFlags = true;
  }
  else if (name == "mixtureDistribCount")
  {
    _param_mixtureDistribCount = content.toULong();
    existsParam_mixtureDistribCount = true;
  }
  else if (name == "minLLK")
  {
    _param_minLLK = content.toDouble();
    existsParam_minLLK = true;
  }
  else if (name == "maxLLK")
  {
    _param_maxLLK = content.toDouble();
    existsParam_maxLLK = true;
  }
  else if (name == "distribType")
  {
     _param_distribType = getDistribType(content);
     existsParam_distribType = true;
  }
  else if (name == "bigEndian")
  {
    _param_bigEndian = content.toBool();
    existsParam_bigEndian = true;
  }
  else if (name == "sampleRate")
  {
    _param_sampleRate = content.toDouble();
    existsParam_sampleRate = true;
  }
  else if (name == "saveMixtureFileFormat")
  {
    _param_saveMixtureFileFormat = getMixtureFileWriterFormat(content);
    existsParam_saveMixtureFileFormat = true;
  }
  else if (name == "saveMixtureServerFileFormat")
  {
    _param_saveMixtureServerFileFormat = getMixtureServerFileWriterFormat(content);
    existsParam_saveMixtureServerFileFormat = true;
  }
  else if (name == "saveSegServerFileFormat")
  {
    _param_saveSegServerFileFormat = getSegServerFileWriterFormat(content);
    existsParam_saveSegServerFileFormat = true;
  }
  else if (name == "saveFeatureFileFormat")
  {
    _param_saveFeatureFileFormat = getFeatureFileWriterFormat(content);
    existsParam_saveFeatureFileFormat = true;
  }
  else if (name == "saveFeatureFileSPro3DataKind")
  {
    _param_saveFeatureFileSPro3DataKind = getSPro3DataKind(content);
    existsParam_saveFeatureFileSPro3DataKind = true;
  }
  else if (name == "loadFeatureFileFormat")
  {
    _param_loadFeatureFileFormat
      = getFeatureFileReaderFormat(content);
    existsParam_loadFeatureFileFormat = true;
  }
  else if (name == "loadFeatureFileVectSize")
  {
    _param_loadFeatureFileVectSize = content.toULong();
    existsParam_loadFeatureFileVectSize = true;
  }
  else if (name == "loadAudioFileChannel")
  {
    _param_loadAudioFileChannel
      = content.toULong();
    existsParam_loadAudioFileChannel = true;
  }
  else if (name == "loadMixtureFileFormat")
  {
    _param_loadMixtureFileFormat = getMixtureFileReaderFormat(content);
    existsParam_loadMixtureFileFormat = true;
  }
  else if (name == "loadSegServerFileFormat")
  {
    _param_loadSegServerFileFormat = getSegServerFileReaderFormat(content);
    existsParam_loadSegServerFileFormat = true;
  }
  else if (name == "saveMixtureFileExtension")
  {
    _param_saveMixtureFileExtension = content;
    existsParam_saveMixtureFileExtension = true;
  }
  else if (name == "saveMixtureServerFileExtension")
  {
    _param_saveMixtureServerFileExtension = content;
    existsParam_saveMixtureServerFileExtension = true;
  }
  else if (name == "saveSegServerFileExtension")
  {
    _param_saveSegServerFileExtension = content;
    existsParam_saveSegServerFileExtension = true;
  }
  else if (name == "saveFeatureFileExtension")
  {
    _param_saveFeatureFileExtension = content;
    existsParam_saveFeatureFileExtension = true;
  }
  else if (name == "loadFeatureFileExtension")
  {
    _param_loadFeatureFileExtension = content;
    existsParam_loadFeatureFileExtension = true;
  }
  else if (name == "loadAudioFileExtension")
  {
    _param_loadAudioFileExtension = content;
    existsParam_loadAudioFileExtension = true;
  }
  else if (name == "loadFeatureFileBigEndian")
  {
    _param_loadFeatureFileBigEndian = content.toBool();
    existsParam_loadFeatureFileBigEndian = true;
  }
  else if (name == "loadAudioFileBigEndian")
  {
    _param_loadAudioFileBigEndian = content.toBool();
    existsParam_loadAudioFileBigEndian = true;
  }
  else if (name == "featureServerMode")
  {
    if (content != "FEATURE_UNWRITABLE" &&
        content != "FEATURE_WRITABLE")
      throw Exception("parameter '"+name+
        "' must be 'FEATURE_UNWRITABLE' or 'FEATURE_WRITABLE'",
                      __FILE__, __LINE__);
    _param_featureServerMode = content;
    existsParam_featureServerMode = true;
  }
  else if (name == "loadMixtureFileBigEndian")
  {
    _param_loadMixtureFileBigEndian = content.toBool();
    existsParam_loadMixtureFileBigEndian = true;
  }
  else if (name == "loadMixtureFileExtension")
  {
    _param_loadMixtureFileExtension = content;
    existsParam_loadMixtureFileExtension = true;
  }
  else if (name == "loadSegServerFileExtension")
  {
    _param_loadSegServerFileExtension = content;
    existsParam_loadSegServerFileExtension = true;
  }
  else if (name == "mixtureFilesPath")
  {
    _param_mixtureFilesPath = content;
    existsParam_mixtureFilesPath = true;
  }
  else if (name == "featureFilesPath")
  {
    _param_featureFilesPath = content;
    existsParam_featureFilesPath = true;
  }
  else if (name == "audioFilesPath")
  {
    _param_audioFilesPath = content;
    existsParam_audioFilesPath = true;
  }
  else if (name == "segServerFilesPath")
  {
    _param_segServerFilesPath = content;
    existsParam_segServerFilesPath = true;
  }
  else if (name == "debug")
  {
    if (content.getToken(0).isEmpty())
      _param_debug = true;
    else
      _param_debug = content.toBool();
    existsParam_debug = true;
  }

  _set.rewind();
  XLine* p;
  while( (p=_set.getLine()) != NULL && p->getElement(0) != name)
    ;
  if (p == NULL)
    _set.addLine().addElement(name).addElement(content);
  else
    p->getElement(1) = content;
}
//-------------------------------------------------------------------------
void Config::setParam(const Config& c)
{
  for (int i = c._set.getLineCount()-1; i>=0; i--)
    setParam(c.getParamName(i), c.getParamContent(i));
}
//-------------------------------------------------------------------------
String Config::getClassName() const { return "Config"; }
//-------------------------------------------------------------------------
String Config::toString() const
{
  String s = Object::toString();
  _set.rewind();
  XLine* p;
  while( (p = _set.getLine()) != NULL)
    s += "\n  <" + p->getElement(0) + "> = " + p->getElement(1);
  return s;
}
//-------------------------------------------------------------------------
Config::~Config() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_Config_cpp)
