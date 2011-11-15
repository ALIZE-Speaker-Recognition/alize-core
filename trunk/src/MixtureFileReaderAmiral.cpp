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

#if !defined(ALIZE_MixtureFileReaderAmiral_cpp)
#define ALIZE_MixtureFileReaderAmiral_cpp

#include <new>
#include "MixtureFileReaderAmiral.h"
#include "Mixture.h"
#include "MixtureGD.h"
#include "DistribGD.h"
#include "Exception.h"
#include "Config.h"
#include "FileReader.h"

using namespace alize;
typedef MixtureFileReaderAmiral R;

//-------------------------------------------------------------------------
R::MixtureFileReaderAmiral(const FileName& f, const Config& c, BigEndian be)
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
  assert(_pReader != NULL);
  if (_pReader->readString(24) != "LIA Modele Multidistrib.")
    throw InvalidDataException("Invalid file", __FILE__, __LINE__,
                  _pReader->getFullFileName());
  _pReader->readChar(); // '0'
  _pReader->swap() = (_pReader->readInt2() != 1);
  if (_pReader->readChar() == 2) // version
    readVersion2();
  else
    throw InvalidDataException("Invalid file", __FILE__, __LINE__,
                  _pReader->getFullFileName());
  return *_pMixture;
}
//-------------------------------------------------------------------------
const MixtureGD& R::readMixtureGD()
{
  assert(_pReader != NULL);
  if (_pReader->readString(24) != "LIA Modele Multidistrib.")
    throw InvalidDataException("Invalid file", __FILE__, __LINE__,
                  _pReader->getFullFileName());
  _pReader->readChar(); // '0'
  _pReader->swap() = (_pReader->readInt2() != 1);
  if (_pReader->readChar() == 2) // version
    readVersionGD2();
  else
    throw InvalidDataException("Invalid file", __FILE__, __LINE__,
                  _pReader->getFullFileName());
  return *static_cast<MixtureGD*>(_pMixture);
}
//-------------------------------------------------------------------------
void R::readVersionGD2() // private
{
  unsigned long c;
  MixtureGD* pMix = NULL;
  assert(_pReader != NULL);

  try
  {
    // Taille du modele
    _pReader->readInt4();

    // taille zone commentaire
    long t = _pReader->readInt4();

    // commentaires + identite (voir MM_io.c)
    if (t > 0)
    {
      _pReader->readString(t);
      //readChar(); // '0'
    }

    // nombre de distributions
    unsigned long distribCount = _pReader->readInt4();
  
    // taille du vecteur
    _vectSize = _pReader->readInt2();

    // Nbre de trames d'apprentissage
    _pReader->readUInt4();

    // distribution weights
    DoubleVector weightVector;
    for (c=0; c<distribCount; c++)
    { weightVector.addValue(_pReader->readDouble()); }

    for (c=0; c<distribCount; c++)
    {
      if (_pReader->readChar() == 1) // type de distribution 1 = GAUSSIAN
      {
        _pReader->readDouble(); // cst

        char typeMatrice = _pReader->readChar();

        if (typeMatrice == 1) // 1 = matrice diag
        {
          if (pMix == NULL)
            pMix = &MixtureGD::create(K::k, _pReader->getFileName(),
                        _vectSize, distribCount);
          pMix->weight(c) = weightVector[c];
          readDistribGD(pMix->getDistrib(c));
        }
        else // autres types de matrices
          throw InvalidDataException("Matrix is not diagonal",
                     __FILE__, __LINE__, _pReader->getFullFileName());
      }
      else // autres types de distributions
        throw InvalidDataException("Distribution is not Gaussian",
                    __FILE__, __LINE__, _pReader->getFullFileName());
    }
    _pMixture = pMix;
  }
  catch (Exception&)
  {
    if (pMix != NULL)
       delete pMix;
    throw; // do not use 'throw e'
  }
}
//-------------------------------------------------------------------------
void R::readVersion2() // private
{
  unsigned long c;
  _pMixture = NULL;
  assert(_pReader != NULL);
  try
  {
    // Taille du modele
    _pReader->readInt4();

    // taille zone commentaire
    long t = _pReader->readInt4();

    // commentaires + identite (voir MM_io.c)
    if (t > 0)
      _pReader->readString(t);

    // nombre de distributions
    unsigned long distribCount = _pReader->readInt4();
  
    // taille du vecteur
    _vectSize = _pReader->readInt2();

    // Nbre de trames d'apprentissage
    _pReader->readUInt4();

    // distribution weights
    DoubleVector weightVector;
    for (c=0; c<distribCount; c++)
    { weightVector.addValue(_pReader->readDouble()); }

    for (c=0; c<distribCount; c++)
    {
      if (_pReader->readChar() == 1) // type de distribution 1 = GAUSSIAN
      {
        _pReader->readDouble(); // cst

        char typeMatrice = _pReader->readChar();

        if (typeMatrice == 1) // 1 = matrice diag
        {
          if (_pMixture == NULL)
            _pMixture = &MixtureGD::create(K::k, _pReader->getFileName()
                         ,_vectSize, distribCount);
          _pMixture->weight(c) = weightVector[c];
          readDistribGD(static_cast<DistribGD&>
                      (_pMixture->getDistrib(c)));
        }
        else if (typeMatrice == 0) // 0 = matrice full
        {
          throw InvalidDataException("Unknown matrix type",
                      __FILE__, __LINE__, _pReader->getFullFileName());
        }
        else // autres types de matrices
          throw InvalidDataException("Unknown matrix type",
                      __FILE__, __LINE__, _pReader->getFullFileName());
      }
      else // autres types de distributions
        throw InvalidDataException("Unknown distribution type",
                      __FILE__, __LINE__, _pReader->getFullFileName());
    }
  }
  catch (Exception&)
  {
    if (_pMixture != NULL)
       delete _pMixture;
    throw; // do not use 'throw e'
  }
}
//-------------------------------------------------------------------------
void R::readDistribGD(DistribGD& distrib)
{
  assert(distrib.getVectSize() == _vectSize);
  assert(_pReader != NULL);
  unsigned long i;
  // covariance
  for (i = 0; i < _vectSize; i++)
  { distrib.setCov(_pReader->readDouble(), i); }

  // covariance inverse
  for (i = 0; i < _vectSize; i++)
  { _pReader->readDouble(); }

  // moyenne
  for (i = 0; i < _vectSize; i++)
  { distrib.setMean(_pReader->readDouble(), i); }

  // determinant
  _pReader->readDouble();

  // somme des coeff d'appartenance des
  // trames d'apprentissage a la distribution
  _pReader->readDouble();

  // calcul det, cst, cov inv
  distrib.computeAll();
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "MixtureFileReaderAmiral"; }
//-------------------------------------------------------------------------
R::~MixtureFileReaderAmiral() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureFileReaderAmiral_cpp)

