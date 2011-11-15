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

#if !defined(ALIZE_AudioFileReader_cpp)
#define ALIZE_AudioFileReader_cpp
#include <new>
#include "AudioFileReader.h"
#include "AudioFrame.h"
#include "alizeString.h"
#include "Config.h"
#include "FileReader.h"

using namespace alize;
typedef AudioFileReader R;

//-------------------------------------------------------------------------
R::AudioFileReader(const FileName& f, const Config& c, BigEndian be)
:AudioInputStream(getChannel(c)), _pReader(&FileReader::create(f, getPath(f, c),
 getExt(f, c), getBigEndian(c, be))), _frameIndex(0), _paramDefined(false),
 _seekWanted(true), _seekWantedIdx(0)
{
  // only SPHERE files (other formats later...)
  // on part du principe que les .sph on un codage ulaw
  // si stereo, on ne garde que le 1er canal
}
//-------------------------------------------------------------------------
R& R::create(const FileName& f, const Config& c, BigEndian be)
{
  R* p = new (std::nothrow) R(f, c, be);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
String R::getPath(const FileName& f, const Config& c) const
{
  if (f.beginsWith("/") || f.beginsWith("./"))
    return "";
  return c.getParam_audioFilesPath();
}
//-------------------------------------------------------------------------
String R::getExt(const FileName& f, const Config& c) const
{
  if (f.beginsWith("/") || f.beginsWith("./"))
    return "";
  return c.getParam_loadAudioFileExtension();
}
//-------------------------------------------------------------------------
void R::close()
{
  if (_pReader != NULL)
    _pReader->close();
}
//-------------------------------------------------------------------------
unsigned long R::getChannel(const Config& c) const
{
  if (!c.existsParam_loadAudioFileChannel)
    return 0;
  return c.getParam_loadAudioFileChannel();
}
//-------------------------------------------------------------------------
bool R::getBigEndian(const Config& c, BigEndian b) const // protected
{
  if (b == BIGENDIAN_TRUE)
    return true;
  if (b == BIGENDIAN_FALSE)
    return false;
  // BIGENDIAN_AUTO
  if (c.existsParam_loadAudioFileBigEndian)
    return c.getParam_loadAudioFileBigEndian();
  if (c.existsParam_bigEndian)
    return c.getParam_bigEndian();
  return false;
}
//-------------------------------------------------------------------------
bool R::readFrame(AudioFrame& f)
{
  if (_seekWanted)
  {
    _seekWanted = false;
    _frameIndex = _seekWantedIdx;
    if (_frameIndex >= getFrameCount())
      return false;
    unsigned long n = getSampleBytes()*getChannelCount();
    _pReader->seek(getHeaderLength()+n*_frameIndex);
  }
  // si on depasse la fin du fichier
  unsigned long i, frameCount = getFrameCount();
  // the call to getFrameCount() defines _sampleBytes & other stuff
  if (_frameIndex >= frameCount)
    return false;
  if (_selectedChannel > _channelCount)
    throw Exception("Unavailable selected channel #"
          + String::valueOf(_selectedChannel), __FILE__, __LINE__);
  if (_sampleBytes == 2) // 16 bits
  {
    if (_channelCount == 1)
      f.setData(_pReader->readInt2());
    else if (_channelCount == 2)
    {
      if (_selectedChannel == 0)
      {
        f.setData(_pReader->readInt2());
        _pReader->readInt2();
      }
      else
      {
        _pReader->readInt2();
        f.setData(_pReader->readInt2());
      }
    }
    else // multi-channels
    {
      for (i=0; i<_selectedChannel; i++)
        _pReader->readInt2();
      f.setData(_pReader->readInt2());
      for (i++; i<_channelCount; i++)
        _pReader->readInt2();
    }
  }
  else if (_sampleBytes == 1) // 8 bits
  {
    if (_channelCount == 1)
      f.setData(_pReader->readChar());
    else if (_channelCount == 2)
    {
      if (_selectedChannel == 0)
      {
        f.setData(_pReader->readChar());
        _pReader->readChar();
      }
      else
      {
        _pReader->readChar();
        f.setData(_pReader->readChar());
      }
    }
    else // multi-channels
    {
      for (i=0; i<_selectedChannel; i++)
        _pReader->readChar();
      f.setData(_pReader->readChar());
      for (i++; i<_channelCount; i++)
        _pReader->readChar();
    }
  }
  else
    throw Exception("Unimplemented code (TODO)", __FILE__, __LINE__);
  f.setValidity(true);
  _frameIndex++;
  return true; // invalid frame
}
//-------------------------------------------------------------------------
void R::seekFrame(unsigned long n) // n = next frame to read
{
  _seekWanted = true;
  _seekWantedIdx = n;
}
//-------------------------------------------------------------------------
unsigned long R::getFrameCount()
{
  if (!_paramDefined)
    readParams();
  return _frameCount;
}
//-------------------------------------------------------------------------
void R::readParams() // private 
{
  assert(_pReader != NULL);
  _pReader->open(); // can throw FileNotFoundException
  unsigned long lineCount = 0;
  long headerLength = -1;
  long frameCount = -1;
  long channelCount = -1;
  long sampleBytes = -1;
  long sampleRate = -1;
  while (true)
  {
    const String& s = _pReader->readLine();
    if (s.beginsWith("end_head"))
      break;
    lineCount++;
    // on saute la 1ere ligne
    if (lineCount == 1)
      continue;
    if (lineCount == 2)
    {
      headerLength = s.toLong();
      continue;
    }
    // on saute les comentaires
    if (s.beginsWith(";"))
      continue;
    if (s.beginsWith("channel_count -i "))
      channelCount = s.getToken(2).toLong();
    if (s.beginsWith("sample_rate -i "))
      sampleRate = s.getToken(2).toLong();
    else if (s.beginsWith("sample_n_bytes -i "))
      sampleBytes = s.getToken(2).toLong();
    else if (s.beginsWith("sample_count -i "))
      frameCount = s.getToken(2).toLong();
    else if (s.beginsWith("sample_byte_format -s2 01"))
      _pReader->swap() = false;
  }
  // tests whether everything is ok
  if (headerLength == -1 ||
      frameCount == -1 ||
      channelCount == -1 ||
      sampleRate == -1 ||
      sampleBytes == -1)
  {
    _pReader->close();
    throw InvalidDataException("Wrong header", __FILE__, __LINE__,
                               _pReader->getFullFileName());
  }
  _headerLength = headerLength;
  _frameCount = frameCount;
  _sampleBytes = sampleBytes;
  _frameRate = (real_t)sampleRate;
  _channelCount = channelCount;
  _paramDefined = true;
}
//-------------------------------------------------------------------------
void R::reset() { seekFrame(0); }
//-------------------------------------------------------------------------
unsigned long R::getSourceCount() { return 1; }
//-------------------------------------------------------------------------
unsigned long R::getHeaderLength() // private
{
  if (!_paramDefined)
    readParams();
  return _headerLength;
}
//-------------------------------------------------------------------------
unsigned long R::getChannelCount()
{
  if (!_paramDefined)
    readParams();
  return _channelCount;
}
//-------------------------------------------------------------------------
unsigned long R::getSampleBytes()
{
  if (!_paramDefined)
    readParams();
  return _sampleBytes;
}
//-------------------------------------------------------------------------
real_t R::getFrameRate()
{
  if (!_paramDefined)
    readParams();
  return _frameRate;
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "AudioFileReader"; }
//-------------------------------------------------------------------------
String R::toString() const
{
  AudioFileReader& r = const_cast<AudioFileReader&>(*this);
  return Object::toString()
    +"\n frame count      = " + String::valueOf(r.getFrameCount())
    +"\n channel count    = " + String::valueOf(r.getChannelCount())
    +"\n selected channel = " + String::valueOf(r.getSelectedChannel())
    +"\n sample bytes     = " + String::valueOf(r.getSampleBytes());
}
//-------------------------------------------------------------------------
R::~AudioFileReader()
{
  assert(_pReader != NULL);
  delete _pReader;
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_AudioFileReader_cpp)

