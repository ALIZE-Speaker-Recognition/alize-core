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

#if !defined(ALIZE_MixtureFileReader_cpp)
#define ALIZE_MixtureFileReader_cpp

#include "MixtureFileReader.h"
#include "MixtureFileReaderAmiral.h"
#include "MixtureFileReaderRaw.h"
#include "MixtureFileReaderXml.h"
#include "Exception.h"
#include "Mixture.h"
#include "MixtureGD.h"
#include "MixtureGF.h"
#include "Config.h"

using namespace alize;
typedef MixtureFileReader R;

//-------------------------------------------------------------------------
R::MixtureFileReader(const FileName& f, const Config& c, BigEndian be)
:MixtureFileReaderAbstract(NULL, c),
 _pMixFileReader(&createReader(f, c, be)) {}
//-------------------------------------------------------------------------
MixtureFileReaderAbstract& R::createReader(const FileName& f,
                                           const Config& c, BigEndian be)
{ // private
  if (c.existsParam_loadMixtureFileFormat)
    switch (c.getParam_loadMixtureFileFormat())
    {
      case MixtureFileReaderFormat_AMIRAL:
        return MixtureFileReaderAmiral::create(f, c, be);
      case MixtureFileReaderFormat_XML:
        return MixtureFileReaderXml::create(f, c);
      case MixtureFileReaderFormat_RAW:
        return MixtureFileReaderRaw::create(f, c, be);
    }
  const FileName& ff = f + getExt(f, c);
  if (ff.endsWith(".xml"))
    return MixtureFileReaderXml::create(f, c);
  //if (ff.endsWith(".gmm"))
  //  return MixtureFileReaderAmiral::create(f, c, be);
  throw Exception("Param 'loadMixtureFileFormat' expected in the config",
                  __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
const Mixture& R::readMixture() { return _pMixFileReader->readMixture(); }
//-------------------------------------------------------------------------
const Mixture& R::readMixture(DistribType t)
{
  if (t == DistribType_GD)
    return static_cast<const Mixture&>(_pMixFileReader->readMixtureGD());
  if (t == DistribType_GF)
    return static_cast<const Mixture&>(_pMixFileReader->readMixtureGF());
  return readMixtureGF(); // never called
 }
//-------------------------------------------------------------------------
const MixtureGD& R::readMixtureGD() 
{ return _pMixFileReader->readMixtureGD(); }
//-------------------------------------------------------------------------
const MixtureGF& R::readMixtureGF()
{ return _pMixFileReader->readMixtureGF(); }
//-------------------------------------------------------------------------
String R::getClassName() const { return "MixtureFileReader"; }
//-------------------------------------------------------------------------
R::~MixtureFileReader()
{
  if (_pMixFileReader != NULL)
    delete _pMixFileReader;
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureFileReader_cpp)

