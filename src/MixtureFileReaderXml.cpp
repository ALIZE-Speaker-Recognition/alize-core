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

#if !defined(ALIZE_MixtureFileReaderXml_cpp)
#define ALIZE_MixtureFileReaderXml_cpp

#include <new>
#include "MixtureFileReaderXml.h"
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
typedef MixtureFileReaderXml R;

//-------------------------------------------------------------------------
R::MixtureFileReaderXml(const FileName& f, const Config& c)
:MixtureFileReaderAbstract(&FileReader::create(f, getPath(f, c),
 getExt(f, c), false), c), _pMixture(NULL) {}
//-------------------------------------------------------------------------
R& R::create(const FileName& f, const Config& c)
{
  R* p = new (std::nothrow) R(f, c);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
const Mixture& R::readMixture()
{
  _line = 1;
  _idFound = false;
  _distribCountFound = false;
  _vectSizeFound = false;
  _typeFound = false;

  parse();
  _pReader->close();
  return *_pMixture;
}
//-------------------------------------------------------------------------
const MixtureGD& R::readMixtureGD()
{
  const MixtureGD* p = dynamic_cast<const MixtureGD*>(&readMixture());
  if (p == NULL)
    throw Exception("The file does not contain a Mixture with type GD",
      __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
const MixtureGF& R::readMixtureGF()
{
  const MixtureGF* p = dynamic_cast<const MixtureGF*>(&readMixture());
  if (p == NULL)
    throw Exception("The file does not contain a mixture with type GF",
      __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void R::eventOpeningElement(const String& path)
{
  if (path.endsWith("<mean><i>"))
  {
    if (_meanIndexFound)
      eventError("More than one tag " + path + " !");
    _meanIndexFound = true;
  }
  else if (path.endsWith("<mean>"))
  {
    _meanIndexFound = false;
  }
  else if (path.endsWith("<covInv><i>"))
  {
    if (_covInvIndexFound)
      eventError("More than one tag " + path + " !");
    _covInvIndexFound = true;
  }
  else if (path.endsWith("<covInv><j>"))
  {
    _covInvIndexJFound = true;
  }
  else if (path.endsWith("<covInv>"))
  {
    _covInvIndexFound = false;
  }
  else if (path.endsWith("<cov><i>"))
  {
    if (_covIndexFound)
      eventError("More than one tag " + path + " !");
  }
  else if (path.endsWith("<cov><j>"))
  {
    if (_covIndexFound)
      eventError("More than one tag " + path + " !");
  }
  else if (path.endsWith("<cov>"))
  {
    _covIndexFound = false;
  }
  else if (path.endsWith("<DistribGD><i>") || path.endsWith("<DistribGF><i>"))
  {
    if (_distribIndexFound)
      eventError("More than one tag " + path + " !");
    _distribIndexFound = true;
  }
  else if (path.endsWith("<weight>"))
  {
    if (_weightFound)
      eventError("More than one tag " + path + " !");
    _weightFound = true;
  }
  else if (path.endsWith("<cst>")) {}
  else if (path.endsWith("<det>")) {}
  else if (path.endsWith("<DistribGD>") || path.endsWith("<DistribGF>"))
  {
    if (!_distribCountFound)
      eventError("Dont't know distribCount to create the mixture");
    if (!_vectSizeFound)
      eventError("Dont't know vectSize to create the mixture");
    _distribIndexFound = false;
    _weightFound = false;
  }
  else if (path.endsWith("<distribCount>"))
  {
    if (_distribCountFound)
      eventError("More than one tag " + path + " !");
    _distribCountFound = true;
  }
  else if (path.endsWith("<vectSize>"))
  {
    if (_vectSizeFound)
      eventError("More than one tag " + path + " !");
    _vectSizeFound = true;
  }
  else if (path.endsWith("<MixtureGD><id>") || path.endsWith("<MixtureGF><id>"))
  {
    if (_idFound)
      eventError("More than one tag " + path + " !");
    _idFound = true;
  }
  else if (path.endsWith("<version>")) {}
  else if (path.endsWith("<MixtureGD>"))
  {
    if (_pMixture != NULL)
      eventError("More than one tag " + path + " !");
    _type = DistribType_GD;
    _typeFound = true;
  }
  else if (path.endsWith("<MixtureGF>"))
  {
    if (_pMixture != NULL)
      eventError("More than one tag " + path + " !");
    _type = DistribType_GF;
    _typeFound = true;
  }
  else
    eventError("Unknown tag in the path " + path);
}
//-------------------------------------------------------------------------
void R::eventClosingElement(const String& path, const String& value)
{
  if (path.endsWith("<mean><i>"))
  {
    _meanIndex = value.toULong();
  }
  else if (path.endsWith("<mean>"))
  {
    if (_meanIndexFound == false)
      eventError("Index missing for mean");
    switch (type())
    {
    case DistribType_GD:
      distribGD().setMean(value.toDouble(), _meanIndex);
      break;
    case DistribType_GF:
      distribGF().setMean(value.toDouble(), _meanIndex);
    }
  }
  else if (path.endsWith("<covInv><i>"))
  {
    _covInvIndex = value.toULong();
  }
  else if (path.endsWith("<covInv><j>"))
  {
    _covInvIndexJ = value.toULong();
  }
  else if (path.endsWith("<covInv>"))
  {
    if (_covInvIndexFound == false)
      eventError("Index missing for covInv");
    switch (type())
    {
    case DistribType_GD:
      distribGD().setCovInv(K::k, value.toDouble(),_covInvIndex);
      break;
    case DistribType_GF:
      distribGF().setCovInv(K::k, value.toDouble(), _covInvIndex,
                                                       _covInvIndexJ);
    }
  }
  else if (path.endsWith("<cov><i>"))
  {
    _covIndex = value.toULong();
    _covIndexFound = true;
  }
  else if (path.endsWith("<cov>"))
  {
    if (!_covIndexFound)
      eventError("Index missing for cov");
    if (type() == DistribType_GD)
      distribGD().setCov(value.toDouble(), _covIndex);
    else
      ; // no cov matrix for GF
  }
  else if (path.endsWith("<DistribGD><i>") || path.endsWith("<DistribGF><i>"))
  {
    _distribIndex = value.toULong();
    _distribIndexFound = true;
  }
  else if (path.endsWith("<weight>"))
  {
    if (!_distribIndexFound)
      eventError("Don't know distrib index");
    mixture().weight(_distribIndex) = value.toDouble();
  }
  else if (path.endsWith("<cst>"))
  {
    if (!_distribIndexFound)
      eventError("Don't know distrib index");
    switch (type())
    {
    case DistribType_GD:
      mixtureGD().getDistrib(_distribIndex).
                     setCst(K::k, value.toDouble());
      break;
    case DistribType_GF:
      mixtureGF().getDistrib(_distribIndex).
                     setCst(K::k, value.toDouble());
    }
  }
  else if (path.endsWith("<det>"))
  {
    if (!_distribIndexFound)
      eventError("Don't know distrib index");
    switch (type())
    {
    case DistribType_GD:
      mixtureGD().getDistrib(_distribIndex).
                     setDet(K::k, value.toDouble());
      break;
    case DistribType_GF:
      mixtureGF().getDistrib(_distribIndex).
                     setDet(K::k, value.toDouble());
    }
  }
  else if (path.endsWith("<DistribGD>"))
  {
    if (!_weightFound)
      eventError("Unknow weight");
  }
  else if (path.endsWith("<distribCount>"))
  {
    _distribCount = value.toULong();
  }
  else if (path.endsWith("<vectSize>"))
  {
    _vectSize = value.toULong();
  }
  else if (path.endsWith("<MixtureGD><id>"))
  {
    _id = value;
  }
  else if (path.endsWith("<MixtureGD><version>"))
  {
    if (value != "1")
      eventError("invalid version");
  }
  else if (path.endsWith("<MixtureGD>"))
  {
    if (_idFound)
      mixture().setId(K::k, _id);
  }
}
//-------------------------------------------------------------------------
void R::eventError(const String& msg)
{
  assert(_pReader != NULL);
  _pReader->close();
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
Mixture& R::mixture() // private
{
  if (_pMixture == NULL)
  {
    if (!_vectSizeFound)
      eventError("Don't know the vectSize to create the mixture");
    if (!_distribCountFound)
      eventError("Don't know the distrib count to create the mixture");
    switch (type())
    {
    case DistribType_GD:
      _pMixture = &MixtureGD::create(K::k, "", _vectSize, _distribCount);
      break;
    case DistribType_GF:
      _pMixture = &MixtureGF::create(K::k, "", _vectSize, _distribCount);
    }
  }
  return *_pMixture;
}
//-------------------------------------------------------------------------
MixtureGD& R::mixtureGD() // private
{ return *dynamic_cast<MixtureGD*>(&mixture()); }
//-------------------------------------------------------------------------
MixtureGF& R::mixtureGF() // private
{ return *dynamic_cast<MixtureGF*>(&mixture()); }
//-------------------------------------------------------------------------
DistribGD& R::distribGD() // private
{
  if (!_distribIndexFound)
     eventError("Don't know distrib index");
  return *dynamic_cast<DistribGD*>(&mixture().getDistrib(_distribIndex));
}
//-------------------------------------------------------------------------
DistribGF& R::distribGF() // private
{
  if (!_distribIndexFound)
     eventError("Don't know distrib index");
  return *dynamic_cast<DistribGF*>(&mixture().getDistrib(_distribIndex));
}
//-------------------------------------------------------------------------
const DistribType& R::type() // private
{
  if (!_typeFound)
    eventError("Unknown mixture type (GD ?, GF ?)");
  return _type;
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "MixtureFileReaderXml"; }
//-------------------------------------------------------------------------
R::~MixtureFileReaderXml()
{
  if (_pMixture != NULL)
    delete _pMixture;
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureFileReaderXml_cpp)

