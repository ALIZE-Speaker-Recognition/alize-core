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

#if !defined(ALIZE_MixtureServerFileReaderRaw_cpp)
#define ALIZE_MixtureServerFileReaderRaw_cpp

#include <new>
#include "MixtureServerFileReaderRaw.h"
#include "MixtureServer.h"
#include "MixtureGD.h"
#include "DistribGD.h"
#include "Exception.h"
#include "Config.h"
#include "FileReader.h"

using namespace alize;
typedef MixtureServerFileReaderRaw R;

//-------------------------------------------------------------------------
R::MixtureServerFileReaderRaw(const FileName& f, const Config& c)
:MixtureServerFileReaderAbstract(&FileReader::create(f, getPath(f, c),
 getExt(f, c), false)) {}
//-------------------------------------------------------------------------
R& R::create(const FileName& f, const Config& c)
{
  R* p = new (std::nothrow) R(f, c);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void R::readMixtureServer(MixtureServer& ms)
{
  unsigned long i, j, k, dictIdx;
  ms.reset();
  assert(_pReader != NULL);
  if (_pReader->readString(13) != "MixtureServer")
    error("does not contain mixture server data");
  ms.setServerName(_pReader->readString(_pReader->readUInt4()));
  unsigned long vectSize = _pReader->readUInt4();
  unsigned long mixtureCount = _pReader->readUInt4();
  unsigned long distribCount = _pReader->readUInt4();
  for (i = 0; i<distribCount; i++) // loads distributions dict
  {
    const String type = _pReader->readString(2);
    if (type == "GD")
    {
      DistribGD& d = static_cast<DistribGD&>
                (ms.createDistrib(DistribType_GD, vectSize));
      if (_pReader->readString(1) == "t") // uses cov vect
        for (j=0; j<vectSize; j++)
          d.setCov(_pReader->readDouble(), j);
      for (j=0; j<vectSize; j++)
        d.setCovInv(K::k, _pReader->readDouble(), j);
      for (j=0; j<vectSize; j++)
        d.setMean(_pReader->readDouble(), j);
    }
    else if (type == "GF")
    {
      DistribGF& d = static_cast<DistribGF&>
                (ms.createDistrib(DistribType_GF, vectSize));
      if (_pReader->readString(1) == "t") // uses cov vect
        for (j=0; j<vectSize; j++)
          for (k=0; k<vectSize; k++)
            d.setCov(_pReader->readDouble(), j, k);
      for (j=0; j<vectSize; j++)
        for (k=0; k<vectSize; k++)
          d.setCovInv(K::k, _pReader->readDouble(), j, k);
      for (j=0; j<vectSize; j++)
        d.setMean(_pReader->readDouble(), j);
    }
    else
      error("Don't know how to read a distrib");
  }
  for (i = 0; i<mixtureCount; i++) // loads mixtures
  {
    const String type = _pReader->readString(2);
    if (type == "GD")
    {
      MixtureGD& m = ms.createMixtureGD(0);
      ms.setMixtureId(m, _pReader->readString(_pReader->readUInt4()));
      unsigned long n = _pReader->readUInt4(); // distrib count
      for (j=0; j<n; j++)
      {
        dictIdx = _pReader->readUInt4();
        m.addDistrib(K::k, ms.getDistrib(dictIdx), _pReader->readDouble());
      }
    }
    else if (type == "GF")
    {
      MixtureGF& m = ms.createMixtureGF(0);
      ms.setMixtureId(m, _pReader->readString(_pReader->readUInt4()));
      unsigned long n = _pReader->readUInt4(); // distrib count
      for (j=0; j<n; j++)
      {
        dictIdx = _pReader->readUInt4();
        m.addDistrib(K::k, ms.getDistrib(dictIdx), _pReader->readDouble());
      }
    }
    else
      error("Don't know how to read a mixture");
  }
  _pReader->close();
}
//-------------------------------------------------------------------------
void R::error(const String& msg)
{
  assert(_pReader != NULL);
  _pReader->close();
  throw InvalidDataException(msg, __FILE__, __LINE__,
                             _pReader->getFullFileName());
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "MixtureServerFileReaderRaw"; }
//-------------------------------------------------------------------------
R::~MixtureServerFileReaderRaw() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureServerFileReaderRaw_cpp)

