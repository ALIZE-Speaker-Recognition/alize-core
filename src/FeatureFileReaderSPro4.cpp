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

#if !defined(ALIZE_FeatureFileReaderSPro4_cpp)
#define ALIZE_FeatureFileReaderSPro4_cpp

#include <new>
#include "FeatureFileReaderSPro4.h"
#include "FileReader.h"
#include "Feature.h"
#include "Exception.h"
#include "LabelServer.h"
#include "Label.h"
#include "Config.h"

using namespace alize;
typedef FeatureFileReaderSPro4 R;

//-------------------------------------------------------------------------
R::FeatureFileReaderSPro4(const FileName& f, const Config& c,
      LabelServer* l, BigEndian be, BufferUsage b, unsigned long bufferSize,
      HistoricUsage h, unsigned long historicSize)
:FeatureFileReaderSingle(&FileReader::create(f, getPath(f, c),
 getExt(f, c), getBigEndian(c, be)), NULL, c, l, b, bufferSize, h, historicSize),
 _paramDefined(false) {}
//-------------------------------------------------------------------------
R& R::create(const FileName& f, const Config& c, LabelServer* l,
             BigEndian be, BufferUsage b, unsigned long bufferSize,
             HistoricUsage h, unsigned long historicSize)
{
  R* p = new (std::nothrow)
         FeatureFileReaderSPro4(f, c, l, be, b, bufferSize, h, historicSize);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void R::readParams() // private virtual
{
  assert(_pReader != NULL);
  _pReader->open(); // can throw FileNotFoundException

  if (!readHeader())
  {
    _pReader->close();
    throw InvalidDataException("Wrong header", __FILE__, __LINE__,
                  _pReader->getFullFileName());
  }
}
//-------------------------------------------------------------------------
unsigned long R::getFeatureCount()
{
  if (!_paramDefined)
    readParams();  // can throw FileNotFoundException
  return _featureCount;
}
//-------------------------------------------------------------------------
unsigned long R::getVectSize()
{
  if (!_paramDefined)
    readParams(); // can throw FileNotFoundException
  return _vectSize;
}
//-------------------------------------------------------------------------
const FeatureFlags& R::getFeatureFlags()
{
  if (!_paramDefined)
    readParams(); // can throw FileNotFoundException
  return _flags;
}
//-------------------------------------------------------------------------
real_t R::getSampleRate()
{
  if (!_paramDefined)
    readParams(); // can throw FileNotFoundException
  return _sampleRate;
}
//-------------------------------------------------------------------------
unsigned long R::getHeaderLength()
{
  if (!_paramDefined)
    readParams(); // can throw FileNotFoundException
  return _headerLength;
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "FeatureFileReaderSPro4";}
//-------------------------------------------------------------------------
bool R::readHeader()
{
  assert(_pReader != NULL);
  const int WITHE = 0x01; /* Data is with log energy         */
  //const int WITHZ = 0x02; /* Data as mean suppressed
  //                           (const only)*/
  //const int WITHN = 0x04; /* Absolute log energy suppressed    */
  const int WITHD = 0x08; /* Data as delta coefficients      */
  const int WITHA = 0xa0; /* Data contains delta-delta coeffs    */
  const int WITHNODE = 0x10; /* */

  if (_pReader->readString(9) == "<header>\n")
    while(true)
    {
      String line = _pReader->readLine();
      unsigned long l = line.length();
      if (l == 0)
        continue;
      if (line.getToken(0) == "</header>")
        break;
      const char* s = line.c_str();
      String name, value;
      unsigned long i;
      for (i=0; i<l&&s[i]!=';'&&s[i]!='='&&(s[i]==' '||s[i]=='\t'); i++)
        ;
      for (; i<l&&s[i]!=';'&&s[i]!='='&&s[i]!=' '&&s[i]!='\t'; i++)
        name += line[i];
      for (; i<l&&s[i]!=';'&&s[i]!='='; i++)
        ;
      for (; i<l&&s[i]!=';'&&(s[i]==' '||s[i]=='\t'); i++)
        ;
      for (; i<l&&s[i]!=';'&&s[i]!=' '&&s[i]!='\t'; i++)
      {
        value += line[i];
      }
      if (name.isEmpty() && value.isEmpty())
        return false;
      // ... use name & value
    }
  else
    _pReader->rewind();

  _vectSize = _pReader->readInt2();
  unsigned long flag = _pReader->readUInt4();
  _sampleRate = (real_t)_pReader->readFloat();
  _headerLength = _pReader->tell();
  //_vectSizeStatic = _vectSize;
  _flags.useS = true;
  
  /* version Sylvain */
  if ((flag & WITHE) != 0)
  {
    _flags.useE = true;
    //_vectSize++;
    if ((flag & WITHD) != 0)
    {
      _flags.useD = true;
      //_vectSize += _vectSizeStatic;
      if ((flag & WITHNODE) == 0)
      {
        _flags.useDE = true;
        //_vectSize++;
      }
      if ((flag & WITHA) != 0)
      {
        _flags.useDD = true;
        _flags.useDDE = true;
        //_vectSize += _vectSizeStatic;
        //_vectSize++;
      }
    }
    else
    {
      if ((flag & WITHA) != 0)
      {
        _flags.useD = true;
        _flags.useDE = true;
        //_vectSize += _vectSizeStatic;
        //_vectSize++;
      }
    }
  }
  else
  {
    if ((flag & WITHD) != 0)
    {
      _flags.useD = true;
      //_vectSize += _vectSizeStatic;
      if ((flag & WITHA) != 0)
      {
        _flags.useDD = true;
        //_vectSize += _vectSizeStatic;
      }
    }
    else
      if ((flag & WITHA) != 0)
      {
        _flags.useDD = true;
        //_vectSize += _vectSizeStatic;
      }
  }

  if (_vectSize == 0)
    return false;
  _featureCount = (_pReader->getFileLength()-_headerLength)/(_vectSize*sizeof(float));
  _paramDefined = true;

  return true;
}
//-------------------------------------------------------------------------
R::~FeatureFileReaderSPro4() {}
//-------------------------------------------------------------------------
/*
Format en-tete fichier SPRO 4.0
L'en-tete peut ne pas exister (pas de 1er caractere "<")
* = 0, 1 or n blanks or tab

<header>\n
*name*=*value;optional comment\n
*name*=*value;optional comment\n
*name*=*value;optional comment\n
...
</header>\n

dimension  (short)
flag     (long)
frame rate (float)

*/

#endif // !defined(ALIZE_FeatureFileReaderSPro4_cpp)

