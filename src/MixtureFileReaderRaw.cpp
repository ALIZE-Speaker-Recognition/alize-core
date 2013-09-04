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

#if !defined(ALIZE_MixtureFileReaderRaw_cpp)
#define ALIZE_MixtureFileReaderRaw_cpp

#include <new>
#include "MixtureFileReaderRaw.h"
#include "MixtureGD.h"
#include "MixtureGF.h"
#include "DistribGD.h"
#include "DistribGF.h"
#include "Exception.h"
#include "Config.h"
#include "FileReader.h"

using namespace alize;
typedef MixtureFileReaderRaw R;

//-------------------------------------------------------------------------
R::MixtureFileReaderRaw(const FileName& f, const Config& c, BigEndian be)
:MixtureFileReaderAbstract(&FileReader::create(f, getPath(f, c),
 getExt(f, c), getBigEndian(c, be)), c) {}
//-------------------------------------------------------------------------
R& R::create(const FileName& f, const Config& c, BigEndian be)
{
  R* p = new (std::nothrow) R(f, c, be);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
const Mixture& R::readMixture()
{
  if (_config.existsParam_distribType)
  {
    switch (_config.getParam_distribType())
    {
      case DistribType_GD:
        return readMixtureGD();
      case DistribType_GF:
        return readMixtureGF();
    }
  }
  throw Exception("Don't know how to read this king of mixture",
                   __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
const MixtureGD& R::readMixtureGD()
{
  unsigned long c, v;
  assert(_pReader != NULL);

  // number of distributions
  unsigned long distribCount = _pReader->readInt4();

  // size of the vector
  unsigned long vectSize = _pReader->readInt4();

  _pMixture = &MixtureGD::create(K::k, _pReader->getFileName(),
                                 vectSize, distribCount);

  // distribution weights
  for (c=0; c<distribCount; c++)
  { _pMixture->weight(c) = _pReader->readDouble(); }

  for (c=0; c<distribCount; c++)
  {
    DistribGD& d = static_cast<DistribGD&>(_pMixture->getDistrib(c));

    // cst
    d.setCst(K::k, _pReader->readDouble());

    // determinant
    d.setDet(K::k, _pReader->readDouble());

    // covariance
    if (_pReader->readChar() == (char)1)
      for (v = 0; v < vectSize; v++)
      { d.setCov(_pReader->readDouble(), v); }

    // inverse covariance
    for (v = 0; v < vectSize; v++)
    { d.getCovInvVect()[v] = _pReader->readDouble(); }

    // mean
    for (v = 0; v < vectSize; v++)
    { d.setMean(_pReader->readDouble(), v); }
  }
  _pReader->close();
  return *static_cast<MixtureGD*>(_pMixture);
}
//-------------------------------------------------------------------------
const MixtureGF& R::readMixtureGF()
{
  unsigned long c, v, vv;
  assert(_pReader != NULL);

  // nombre de distributions
  unsigned long distribCount = _pReader->readInt4();

  // taille du vecteur
  unsigned long vectSize = _pReader->readInt4();

  _pMixture = &MixtureGF::create(K::k, _pReader->getFileName(),
                                 vectSize, distribCount);

  // distribution weights
  for (c=0; c<distribCount; c++)
  { _pMixture->weight(c) = _pReader->readDouble(); }

  for (c=0; c<distribCount; c++)
  {
    DistribGF& d = static_cast<DistribGF&>(_pMixture->getDistrib(c));

    // cst
    d.setCst(K::k, _pReader->readDouble());

    // determinant
    d.setDet(K::k, _pReader->readDouble());

    // covariance
    if (_pReader->readChar() == (char)1)
      for (v = 0; v < vectSize; v++)
        for (vv = 0; vv < vectSize; vv++)
          d.setCov(_pReader->readDouble(), v, vv);

    // inverse covariance
    for (v = 0; v < vectSize; v++)
      for (vv = 0; vv < vectSize; vv++)
        d.setCovInv(K::k, _pReader->readDouble(), v, vv);

    // mean
    for (v = 0; v < vectSize; v++)
      d.setMean(_pReader->readDouble(), v);
  }
  _pReader->close();
  return *static_cast<MixtureGF*>(_pMixture);
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "MixtureFileReaderRaw"; }
//-------------------------------------------------------------------------
R::~MixtureFileReaderRaw() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureFileReaderRaw_cpp)

