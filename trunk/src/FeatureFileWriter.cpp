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

#if !defined(ALIZE_FeatureFileWriterFormat_cpp)
#define ALIZE_FeatureFileWriterFormat_cpp

#include <new>
#include "FeatureFileWriter.h"
#include "Feature.h"
#include "Exception.h"
#include "Config.h"

using namespace alize;
typedef FeatureFileWriter W;

//-------------------------------------------------------------------------
W::FeatureFileWriter(const FileName& f, const Config& c)
:FileWriter(getFullFileName(c, f)),
 _format(c.getParam_saveFeatureFileFormat()), _vectSizeDefined(false),
 _headerWritten(false), _config(c) {}
//-------------------------------------------------------------------------
W& W::create(const FileName& f, const Config& c)
{
  W* p = new (std::nothrow) W(f, c);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
String W::getFullFileName(const Config& c, const FileName& f) const
{ // private
  if (f.beginsWith("/") || f.beginsWith("./"))
    return f;
  return c.getParam_featureFilesPath() + f
       + c.getParam_saveFeatureFileExtension();
}
//-------------------------------------------------------------------------
void W::writeFeature(const Feature& f)
{
  if (!_vectSizeDefined)
  {
    _vectSize = f.getVectSize();
    _vectSizeDefined = true;
  }
  else
    if (f.getVectSize() != _vectSize)
      throw Exception("Incompatible vectSize", __FILE__, __LINE__);

  if (isClosed())
    open();

  if (_format == FeatureFileWriterFormat_RAW) // **************************************************
  {
    for (unsigned long i=0; i<_vectSize; i++)
    {
      writeFloat((float)f[i]);
    }
  }
  else if (_format == FeatureFileWriterFormat_SPRO3) // *******************************************
  {
    if (!_headerWritten)
    {
      const FeatureFlags flags = _config.getParam_featureFlags();
      unsigned long dim = 0;
      const String& s = flags.getString();
      if (s == "100000")
        dim = _vectSize;
      else if (s == "110000")
        dim = _vectSize-1;
      else if (s == "101000")
        dim = _vectSize/2;
      else if (s == "111000" || s == "101100")
        dim = (_vectSize-1)/2;
      else if (s == "111100")
        dim = (_vectSize-2)/2;
      else if (s == "100010")
        dim = _vectSize/2;
      else if (s == "110010")
        dim = (_vectSize-1)/2;
      else if (s == "101010")
        dim = _vectSize/3;
      else if (s == "111010" || s == "101110")
        dim = (_vectSize-1)/3;
      else if (s == "111110")
        dim = (_vectSize-2)/3;
      else if (s == "100011")
        dim = (_vectSize-1)/2;
      else if (s == "110011")
        dim = (_vectSize-2)/2;
      else if (s == "101011")
        dim = (_vectSize-1)/3;
      else if (s == "111011" || s == "101111")
        dim = (_vectSize-2)/3;
      else if (s == "111111")
        dim = (_vectSize-3)/3;
      else
        throw Exception("Wrong featureFlag : " + s,
                 __FILE__, __LINE__);
      writeUInt4(_config.getParam_saveFeatureFileSPro3DataKind());
      writeUInt4(dim);
      writeUInt4(0);
      writeUInt4(flags.toSPro3());
      _headerWritten = true;
      _featureCount = 0;
    }
    for (unsigned long i=0; i<_vectSize; i++)
    { writeFloat((float)f[i]); }
    _featureCount++;
  }
  else if (_format == FeatureFileWriterFormat_SPRO4) // *******************************************
  {
    if (!_headerWritten)
    {
      writeString("<header>\n");
      writeString("</header>\n");
      writeShort((short)_vectSize);
      writeUInt4(_config.getParam_featureFlags().toSPro4());
      writeFloat((float)_config.getParam_sampleRate());
      _headerWritten = true;
    }
    for (unsigned long i=0; i<_vectSize; i++)
    { writeFloat((float)f[i]); }
  }
  else
     ;
}
//-------------------------------------------------------------------------
void W::close()
{
  if (_format == FeatureFileWriterFormat_SPRO3 && isOpen() && _headerWritten)
  {
     // update feature count just before closing the file
    if (::fseek(_pFileStruct, 4+4, SEEK_SET) != 0) // if error
      throw IOException("", __FILE__, __LINE__, _fileName);
    writeUInt4(_featureCount);
  }
  FileWriter::close();
}
//-------------------------------------------------------------------------
String W::getClassName() const {return "FeatureFileWriter";}
//-------------------------------------------------------------------------
W::~FeatureFileWriter() { close(); }
//-------------------------------------------------------------------------
#endif // !defined(ALIZE_FeatureFileWriterFormat_cpp)

