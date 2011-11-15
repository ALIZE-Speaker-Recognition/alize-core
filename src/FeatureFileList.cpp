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

#if !defined(ALIZE_FeatureFileList_cpp)
#define ALIZE_FeatureFileList_cpp

#include "FeatureFileList.h"
#include "FeatureFileReader.h"

using namespace alize;

//-------------------------------------------------------------------------
FeatureFileList::FeatureFileList(const XLine& l, const Config& c)
:Object(), _fileNameVect(l), _config(c), _vectSizeDefined(false),
 _sampleRateDefined(false), _featureFlagsDefined(false),
 _featureCountDefined(false) {}
//-------------------------------------------------------------------------
unsigned long FeatureFileList::size() const
{ return _fileNameVect.getElementCount(); }
//-------------------------------------------------------------------------
unsigned long FeatureFileList::getVectSize() const
{
  if (!_vectSizeDefined)
  {
    FeatureFileReader r(_fileNameVect.getElement(0), _config, NULL,
                        BIGENDIAN_AUTO, BUFFER_USERDEFINE, 0);
    _vectSize = r.getVectSize();
    _vectSizeDefined = true;
    if (!_sampleRateDefined)
      try { _sampleRate = r.getSampleRate(); _sampleRateDefined = true; }
      catch (Exception&) {}
    if (!_featureFlagsDefined)
      try { _featureFlags = r.getFeatureFlags(); _featureFlagsDefined = true; }
      catch (Exception&) {}
  }
  return _vectSize;
}
//-------------------------------------------------------------------------
real_t FeatureFileList::getSampleRate() const
{
  if (!_sampleRateDefined)
  {
    FeatureFileReader r(_fileNameVect.getElement(0), _config);
    _sampleRate = r.getSampleRate();
    _sampleRateDefined = true;
    if (!_vectSizeDefined)
      try { _vectSize = r.getVectSize(); _vectSizeDefined = true; }
      catch (Exception&) {}
    if (!_featureFlagsDefined)
      try { _featureFlags = r.getFeatureFlags(); _featureFlagsDefined = true; }
      catch (Exception&) {}
  }
  return _sampleRate;
}
//-------------------------------------------------------------------------
const FeatureFlags& FeatureFileList::getFeatureFlags() const
{
  if (!_featureFlagsDefined)
  {
    FeatureFileReader r(_fileNameVect.getElement(0), _config);
    _featureFlags = r.getFeatureFlags();
    _featureFlagsDefined = true;
    if (!_vectSizeDefined)
      try { _vectSize = r.getVectSize(); _vectSizeDefined = true; }
      catch (Exception&) {}
    if (!_sampleRateDefined)
      try { _sampleRate = r.getSampleRate(); _sampleRateDefined = true; }
      catch (Exception&) {}
  }
  return _featureFlags;
}
//-------------------------------------------------------------------------
unsigned long FeatureFileList::getFeatureCount() const
{
  if (!_featureCountDefined)
  {
    unsigned long size = _fileNameVect.getElementCount();
    _featureCountTot = 0;
    for (unsigned long i=0; i<size; i++)
    {
      FeatureFileReader r(_fileNameVect.getElement(i), _config);
      unsigned long c = r.getFeatureCount();
      _featureFirst.addValue(_featureCountTot);
      _featureCountTot += c;
      _featureCount.addValue(c);
      if (i == 0)
      {
        if (!_vectSizeDefined)
          try { _vectSize = r.getVectSize(); _vectSizeDefined = true; }
          catch (Exception&) {}
        if (!_sampleRateDefined)
          try { _sampleRate = r.getSampleRate(); _sampleRateDefined = true; }
          catch (Exception&) {}
        if (!_featureFlagsDefined)
          try { _featureFlags = r.getFeatureFlags(); _featureFlagsDefined = true; }
          catch (Exception&) {}
       }
    }
    _featureCountDefined = true;
  }
  return _featureCountTot;
}
//-------------------------------------------------------------------------
unsigned long FeatureFileList::getIndexOfFirstFeature(
                                               unsigned long fileIdx) const
{
  if (!_featureCountDefined)
    getFeatureCount();
  return _featureFirst[fileIdx];
}
//-------------------------------------------------------------------------
unsigned long FeatureFileList::getIndexOfFirstFeature(
                                                   const FileName& f) const
{ return getIndexOfFirstFeature(getFileIndex(f)); }
//-------------------------------------------------------------------------
unsigned long FeatureFileList::getFeatureCount(unsigned long fileIdx) const
{
  if (!_featureCountDefined)
    getFeatureCount();
  return _featureCount[fileIdx];
}
//-------------------------------------------------------------------------
unsigned long FeatureFileList::getFeatureCount(const FileName& f) const
{
  if (!_featureCountDefined)
    getFeatureCount();
  unsigned long i, size = _fileNameVect.getElementCount();
  for (i=0; i<size; i++)
  {
    if (f == _fileNameVect.getElement(i))
      return _featureCount[i];
  }
  throw Exception(f + " : Unknown feature file name",
                  __FILE__, __LINE__);
  return 0; // never called
}
//-------------------------------------------------------------------------
const String& FeatureFileList::getFileName(unsigned long fileIdx) const
{ return _fileNameVect.getElement(fileIdx); }
//-------------------------------------------------------------------------
unsigned long FeatureFileList::getFileIndex(const FileName& f) const
{
  unsigned long i, size = _fileNameVect.getElementCount();
  for (i=0; i<size; i++)
  {
    if (f == _fileNameVect.getElement(i))
      return i;
  }
  throw Exception(f + " : Unknown feature file name",
                  __FILE__, __LINE__);
  return 0; // never called
}
//-------------------------------------------------------------------------
unsigned long FeatureFileList::getFileIndex(unsigned long featureNbr) const
{
  unsigned long i, size = _fileNameVect.getElementCount();
  if (size == 0)
    throw Exception("The file list is empty", __FILE__, __LINE__);
  if (size == 1)
    return 0;
  for (i=0; i<size-1; i++)
  {
    if (featureNbr < getIndexOfFirstFeature(i+1))
      return i;
  }
  return i;
}
//-------------------------------------------------------------------------
String FeatureFileList::getClassName() const { return "FeatureFileList"; }
//-------------------------------------------------------------------------
FeatureFileList::~FeatureFileList() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FeatureFileList_cpp)

