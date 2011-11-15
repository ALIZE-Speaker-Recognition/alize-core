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

#if !defined(ALIZE_MixtureServerFileReaderXml_cpp)
#define ALIZE_MixtureServerFileReaderXml_cpp

#include <new>
#include "MixtureServerFileReaderXml.h"
#include "MixtureServer.h"
#include "MixtureGD.h"
#include "MixtureGF.h"
#include "DistribGD.h"
#include "DistribGF.h"
#include "Exception.h"
#include "XmlParser.h"
#include "Config.h"
#include "FileReader.h"

// see http://babel.alis.com/web_ml/xml/REC-xml.fr.html#NT-XMLDecl

using namespace alize;
typedef MixtureServerFileReaderXml R;

//-------------------------------------------------------------------------
R::MixtureServerFileReaderXml(const FileName& f, const Config& c)
:MixtureServerFileReaderAbstract(&FileReader::create(f, getPath(f, c),
 getExt(f, c), false /* no swap */)), XmlParser() {}
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
  assert(_pReader != NULL);
  _line = 1;
  _pMixtureServer = &ms;
  parse();
  _pReader->close();
}
//-------------------------------------------------------------------------
void R::eventOpeningElement(const String& path)
{
  if (false) {}
  else if (path.endsWith("<mean>"))
  {
    _meanIndexFound = false;
  }
  else if (path.endsWith("<covInv>"))
  {
    _covInvIndexFound = false;
    _covInvIndexJFound = false;
  }
  else if (path.endsWith("<cov>"))
  {
    _covIndexFound = false;
  }
  else if (path.endsWith("<mean><i>"))   {}
  else if (path.endsWith("<covInv><i>") || path.endsWith("<covInv><j>")) {}
  else if (path.endsWith("<cov><i>") || path.endsWith("<cov><j>"))  {}
  else if (path.endsWith("<MixtureServer><DistribGD>"))
  {
    _distribTypeDefined = true;
    _distribType = DistribType_GD;
    _pDistrib = NULL;
  }
  else if (path.endsWith("<MixtureServer><DistribGF>"))
  {
    _distribTypeDefined = true;
    _distribType = DistribType_GF;
    _pDistrib = NULL;
  }
  else if (path.endsWith("<MixtureServer><DistribGD><i>")) {}
  else if (path.endsWith("<MixtureServer><DistribGF><i>")) {}
  else if (path.endsWith("<MixtureServer><MixtureGD><DistribGD>"))
  {
    _distribIndexFound = false;
    _weightFound = false;
    _distribTypeDefined = true;
    _distribType = DistribType_GD;
  }
  else if (path.endsWith("<MixtureServer><MixtureGF><DistribGF>"))
  {
    _distribIndexFound = false;
    _weightFound = false;
    _distribTypeDefined = true;
    _distribType = DistribType_GF;
  }
  else if (path.endsWith("<MixtureServer><MixtureGD><DistribGD><i>"))     {}
  else if (path.endsWith("<MixtureServer><MixtureGF><DistribGF><i>"))     {}
  else if (path.endsWith("<MixtureServer><MixtureGD><DistribGD><dictIdx>")) {}
  else if (path.endsWith("<MixtureServer><MixtureGF><DistribGF><dictIdx>")) {}
  else if (path.endsWith("<MixtureServer><MixtureGD><DistribGD><weight>"))  {}
  else if (path.endsWith("<MixtureServer><MixtureGF><DistribGF><weight>"))  {}
  else if (path.endsWith("<MixtureServer><MixtureGD>"))
  {
    _pMixture = NULL;
    _mixtType = DistribType_GD;
    _mixtTypeDefined = true;
    _distribTypeDefined = false;
  }
  else if (path.endsWith("<MixtureServer><MixtureGF>"))
  {
    _pMixture = NULL;
    _mixtType = DistribType_GF;
    _mixtTypeDefined = true;
    _distribTypeDefined = false;
  }
  else if (path.endsWith("<MixtureServer><MixtureGD><id>"))       {}
  else if (path.endsWith("<MixtureServer><MixtureGF><id>"))       {}
  else if (path.endsWith("<MixtureServer><MixtureGD><distribCount>")) {}
  else if (path.endsWith("<MixtureServer><MixtureGF><distribCount>")) {}
  else if (path.endsWith("<MixtureServer><vectSize>"))
  {
    if (_vectSizeFound)
      eventError("More than one tag " + path + " !");
    _vectSizeFound = true;
  }
  else if (path.endsWith("<MixtureServer><version>"))    {}
  else if (path.endsWith("<MixtureServer><mixtureCount>")) {}
  else if (path.endsWith("<MixtureServer><distribCount>")) {}
  else if (path.endsWith("<MixtureServer><name>"))     {}
  else if (path.endsWith("<MixtureServer>"))
  {
    _pMixtureServer->reset();
    _pMixtureServer->setServerName("");
    _vectSizeFound = false;
    _mixtTypeDefined = false;
  }
  else
    eventError("Unknown tag in the path " + path);
}
//-------------------------------------------------------------------------
void R::eventClosingElement(const String& path,
                             const String& value)
{
  if (false) {}
  else if (path.endsWith("<mean>"))
  {
    if (!_meanIndexFound)
      eventError("Index missing for mean");
    if (!_distribTypeDefined)
      eventError("unknown mixture type");
    if (_distribType == DistribType_GD)
      getDistribGD().setMean(value.toDouble(), _meanIndex);
    else if (_distribType == DistribType_GF)
      getDistribGF().setMean(value.toDouble(), _meanIndex);
    _meanIndexFound = false;
  }
  else if (path.endsWith("<covInv>"))
  {
    if (!_covInvIndexFound)
      eventError("Index i missing for covInv");
    if (!_distribTypeDefined)
      eventError("unknown mixture type");
    if (_distribType == DistribType_GD)
      getDistribGD().setCovInv(K::k, value.toDouble(), _covInvIndex);
    else if (_distribType == DistribType_GF)
    {
      if (!_covInvIndexJFound)
        eventError("Index j missing for covInv");
      getDistribGF().setCovInv(K::k, value.toDouble(), _covInvIndex, _covInvIndexJ);
    }
  }
  else if (path.endsWith("<cov>"))
  {
    if (!_covIndexFound)
      eventError("Index missing for cov");
    if (!_distribTypeDefined)
      eventError("unknown mixture type");
    if (_distribType == DistribType_GD)
    {
      getDistribGD().setCov(value.toDouble(), _covIndex);
    }
    else if (_distribType == DistribType_GF)
    {
      if (!_covIndexFound)
        eventError("Index j missing for cov");
      getDistribGF().setCov(value.toDouble(), _covIndex, _covIndexJ);
    }
  }
  else if (path.endsWith("<mean><i>"))
  {
    _meanIndex = value.toLong();
    _meanIndexFound = true;
  }
  else if (path.endsWith("<covInv><i>"))
  {
    _covInvIndex = value.toLong();
    _covInvIndexFound = true;
  }
  else if (path.endsWith("<covInv><j>"))
  {
    _covInvIndexJ = value.toLong();
    _covInvIndexJFound = true;
  }
  else if (path.endsWith("<cov><i>"))
  {
    _covIndex = value.toLong();
    _covIndexFound = true;
  }
  else if (path.endsWith("<cov><j>"))
  {
    _covIndexJ = value.toLong();
    _covIndexJFound = true;
  }

  // -----------------------------------------------

  else if (path.endsWith("<MixtureServer><DistribGD><i>")) {}
  else if (path.endsWith("<MixtureServer><DistribGF><i>")) {}
  else if (path.endsWith("<MixtureServer><DistribGD>") ||
           path.endsWith("<MixtureServer><DistribGF>"))
    _distribTypeDefined = false;

  // -----------------------------------------------

  else if (path.endsWith("<MixtureServer><MixtureGD><DistribGD><i>"))     {}
  else if (path.endsWith("<MixtureServer><MixtureGF><DistribGF><i>"))     {}
  else if (path.endsWith("<MixtureServer><MixtureGD><DistribGD><dictIdx>") ||
           path.endsWith("<MixtureServer><MixtureGF><DistribGF><dictIdx>"))
  {
    _distribIndex = value.toLong();
    _distribIndexFound = true;
  }
  else if (path.endsWith("<MixtureServer><MixtureGD><DistribGD><weight>") ||
           path.endsWith("<MixtureServer><MixtureGF><DistribGF><weight>"))
  {
    _weight = value.toDouble();
    _weightFound = true;
  }
  else if (path.endsWith("<MixtureServer><MixtureGD><DistribGD>") ||
           path.endsWith("<MixtureServer><MixtureGF><DistribGF>"))
  {
    if (!_distribIndexFound)
      eventError("dict index missing to assign a distrib to a mixture");
    if (!_weightFound)
      eventError("unknown weigth to assign a distrib to a mixture");
    Distrib& d = _pMixtureServer->getDistrib(_distribIndex);
    _pMixtureServer->addDistribToMixture(getMixture(), d, _weight);
    _distribTypeDefined = false;
  }

  // -----------------------------------------------

  else if (path.endsWith("<MixtureServer><MixtureGD>") ||
           path.endsWith("<MixtureServer><MixtureGF>"))
    _mixtTypeDefined = false;
  else if (path.endsWith("<MixtureServer><MixtureGD><id>"))
  {
    _pMixtureServer->setMixtureId(getMixtureGD(), value);
  }
  else if (path.endsWith("<MixtureServer><MixtureGF><id>"))
  {
    _pMixtureServer->setMixtureId(getMixtureGF(), value);
  }
  else if (path.endsWith("<MixtureServer><MixtureGD><distribCount>")) {}
  else if (path.endsWith("<MixtureServer><MixtureGF><distribCount>")) {}

  // -----------------------------------------------

  else if (path.endsWith("<MixtureServer><version>"))
  {
    if (value != "1")
      eventError("invalid version");
  }
  else if (path.endsWith("<MixtureServer><name>"))
  {
    _pMixtureServer->setServerName(value);
  }
  else if (path.endsWith("<MixtureServer><vectSize>"))
  {
    _vectSize = value.toLong();
    _vectSizeFound = true;
  }
  else if (path.endsWith("<MixtureServer><mixtureCount>"))  {}
  else if (path.endsWith("<MixtureServer><distribCount>"))  {}
}
//-------------------------------------------------------------------------
void R::eventError(const String& msg)
{
  assert(_pReader != NULL);
  _pReader->close();
  _pMixtureServer->reset();
  _pMixtureServer->setServerName("");
  throw InvalidDataException("Error line " + String::valueOf(_line)
           + " : " + msg, __FILE__, __LINE__, _pReader->getFullFileName());
}
//-------------------------------------------------------------------------
const String& R::readOneChar()
{
  assert(_pReader != NULL);
  const String& s = _pReader->readString(1);
  if (s == "\n")
    _line++;
  return s;
}

//-------------------------------------------------------------------------
Mixture& R::getMixture()
{
  if (_pMixture == NULL)
  {
    if (!_mixtTypeDefined)
      eventError("unknown mixture type");
    _pMixture = &_pMixtureServer->createMixture(0, _mixtType);
  }
  return *_pMixture;
}
//-------------------------------------------------------------------------
MixtureGD& R::getMixtureGD()
{ return static_cast<MixtureGD&>(getMixture()); }
//-------------------------------------------------------------------------
MixtureGF& R::getMixtureGF()
{ return static_cast<MixtureGF&>(getMixture()); }
//-------------------------------------------------------------------------
Distrib& R::getDistrib()
{
  if (_pDistrib == NULL)
  {
    if (!_vectSizeFound)
      eventError("Unknown vectSize !");
    if (!_distribTypeDefined)
      eventError("unknown mixture type");
    _pDistrib = &_pMixtureServer->createDistrib(_distribType, _vectSize);
  }
  return *_pDistrib;
}
//-------------------------------------------------------------------------
DistribGD& R::getDistribGD()
{ return static_cast<DistribGD&>(getDistrib()); }
//-------------------------------------------------------------------------
DistribGF& R::getDistribGF()
{ return static_cast<DistribGF&>(getDistrib()); }
//-------------------------------------------------------------------------
String R::getClassName() const { return "MixtureServerFileReaderXml"; }
//-------------------------------------------------------------------------
R::~MixtureServerFileReaderXml() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureServerFileReaderXml_cpp)

