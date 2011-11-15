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

#if !defined(ALIZE_FeatureFileReaderSPro3_cpp)
#define ALIZE_FeatureFileReaderSPro3_cpp

#include <new>
#include "FeatureFileReaderSPro3.h"
#include "FileReader.h"
#include "Feature.h"
#include "Exception.h"
#include "LabelServer.h"
#include "Label.h"
#include "Config.h"

static const int WITHE = 0x01; /* Data is with log energy         */
static const int WITHZ = 0x02; /* Data as mean suppressed
                           (static const only)*/
static const int WITHN = 0x04; /* Absolute log energy suppressed    */
static const int WITHD = 0x08; /* Data as delta coefficients      */
static const int WITHA = 0xa0; /* Data contains delta-delta coeffs    */
static const int WITHNODE = 0x10; /* */

using namespace alize;
typedef FeatureFileReaderSPro3 R;

//-------------------------------------------------------------------------
R::FeatureFileReaderSPro3(const FileName& f, const Config& c,
                   LabelServer* p, BufferUsage b, unsigned long bufferSize,
                   HistoricUsage h, unsigned long historicSize)
:FeatureFileReaderSingle(&FileReader::create(f, getPath(f, c), getExt(f, c),
 false /* endian auto detected */), NULL, c, p, b, bufferSize, h, historicSize),
 _paramDefined(false) {}
//-------------------------------------------------------------------------
R& FeatureFileReaderSPro3::create(const FileName& f, const Config& c,
              LabelServer* l, BufferUsage b, unsigned long bufferSize,
              HistoricUsage h, unsigned long historicSize)
{
  FeatureFileReaderSPro3* p
        = new (std::nothrow) FeatureFileReaderSPro3(f, c, l, b, bufferSize,
        h, historicSize);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void R::readParams() // private
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
{ return getConfig().getParam_sampleRate(); } // TODO : or 100 ?
//-------------------------------------------------------------------------
String R::getClassName() const
{ return "FeatureFileReaderSPro3"; }
//-------------------------------------------------------------------------
unsigned long R::getHeaderLength()
{
  if (!_paramDefined)
    readParams(); // can throw FileNotFoundException
  return _headerLength;
}
//-------------------------------------------------------------------------
/*!
 * Lecture d'en-tete SPro 3.x
 * Lecture de l'en-tete des fichiers SPro, en determinant au
 *        passage la necessite d'une conversion big/little endian
 *  _pFileStruct   Le fichier PRM.
 *  _vectSizeStatic Dimension de la partie statique des vecteurs
 *  _nbFeature   Nombre de vecteurs
 *  _vectSize    Taille reelle des vecteurs, tenant compte des
 *              eventuels deltas et energie.
 *  _swap      Booleen pour savoir si un byte-swap est necessaire
 *  _flag      Flag SPro (voir spro.h)
 *  _kind      Type de parametrisation (voir spro.h)
 *
 * see http://www.irisa.fr/metiss/guig/softwares/spro-3.3.1/node8.html

 * @return false en cas d'erreur, true si tout s'est bien passe.
 */

bool R::readHeader()
{
  // Modifie le 4 juin 2004 pour gerer les long et unsigned long sur 8
  // octets sur IA 64
  assert(_pReader != NULL);
  if (sizeof(unsigned long) == 4)
  {
    typedef unsigned int flag_t; /* Flag for qualifiers       */
    SPRO3DataKind kind, kindInv;
    unsigned long dim, dimInv, n, nInv;
    flag_t flag, flagInv;
    short OK_kind, OK_reste;
    unsigned long nbElem, tailleReelle, tailleCalculee, tailleCalcInv;
    _headerLength = sizeof(SPRO3DataKind) + sizeof(unsigned long) + 
      sizeof(unsigned long) + sizeof(flag_t);
  
    /* Lecture de l'en-tete */
    kind = (SPRO3DataKind)_pReader->readInt4();
    dim = _pReader->readUInt4();
    n = _pReader->readUInt4();
    flag = _pReader->readUInt4();
  
    /* Verification du format */
    /* Test sur kind */
    OK_kind = 0;
    _pReader->swap4Bytes(&kind, &kindInv); // king -> kindInv
    if ((kind <= SPRO3DataKind_LAR) && (kind >= SPRO3DataKind_OTHER)) // ???
      OK_kind++;
    if ((kindInv <= SPRO3DataKind_LAR) && (kindInv >= SPRO3DataKind_OTHER))
      OK_kind--;
    else
      if (OK_kind == 0) // si dans aucun des cas on n'a une valeur
        return false; // correcte c'est que le fichier n'est pas bon.
  
    /* Test sur dim, n, flag */

    OK_reste = 0;

    // determine taille reelle du fichier

    tailleReelle = _pReader->getFileLength();

    // calcule taille theorique du fichier
  
    nbElem = dim;
    if (flag & WITHE)
      nbElem++;
    if (flag & WITHD) {
      if (flag & WITHA) {
        nbElem *= 3;
      }
      else {
        nbElem += nbElem;
      }
    }
    if (flag & WITHN)
      nbElem--;
    nbElem *= n;
    tailleCalculee = nbElem*sizeof(param_t) + sizeof(SPRO3DataKind)
               + 2*sizeof(unsigned long) + sizeof(flag_t);

    // compare les tailles theorique et reelle du fichier
  
    if (tailleCalculee == tailleReelle)
      OK_reste++;

    _pReader->swap4Bytes(&dim,&dimInv);
    _pReader->swap4Bytes(&n,&nInv);
    _pReader->swap4Bytes(&flag,&flagInv);

    // calcule taille theorique du fichier (apres swap des donnees)

    nbElem = dimInv;
    if (flagInv & WITHE)
      nbElem++;
    if (flagInv & WITHD) {
      if (flagInv & WITHA) {
        nbElem *= 3;
      }
      else {
        nbElem += nbElem;
      }
    }
    if (flagInv & WITHN)
      nbElem--;
    nbElem *= nInv;
    tailleCalcInv = nbElem*sizeof(param_t) + sizeof(SPRO3DataKind)
            + 2*sizeof(unsigned long) + sizeof(flag_t);

    // compare les tailles theorique (apres swap) et reelle du fichier

    if (tailleCalcInv == tailleReelle)
      OK_reste--;
    else
      if (OK_reste == 0) // si dans aucun cas on ne retombe sur la bonne
        return false;  // bonne c'est que le fichier n'est pas bon.

    /* Decision */
    /* OK_kind + OK_reste > 0  => pas de byte swap */
    /* OK_kind + OK_reste < 0  => byte swap */
    /* OK_kind + OK_reste = 0  => indetermine, ou fichier foireux */

    if ((OK_kind + OK_reste == 0) && (OK_kind != OK_reste))
      return false;

    if (OK_kind + OK_reste < 0)
    {
      kind = kindInv;
      dim = dimInv;
      n = nInv;
      flag = flagInv;
      _pReader->swap() = true;
    }
  
    /* Valeurs de retour */
    _kind = kind;
    _featureCount = n;
    _vectSize = dim;

    _flags.useS = true;
  
    /* version Teva
    if (flag & WITHE)
      _vectSize++;
    if (flag & WITHD)
      if (flag & WITHA)
        _vectSize *= 3;
      else
        _vectSize *= 2;
    if (flag & WITHN)
      _vectSize--;*/

    /* version Sylvain */
    if ((flag & WITHE) != 0)
    {
      _flags.useE = true;
      _vectSize++;
      if ((flag & WITHD) != 0)
      {
        _flags.useD = true;
        _vectSize += dim;
        if ((flag & WITHNODE) == 0)
        {
          _flags.useDE = true;
          _vectSize++;
        }
        if ((flag & WITHA) != 0)
        {
          _flags.useDD = true;
          _flags.useDDE = true;
          _vectSize += dim;
          _vectSize++;
        }
      }
      else
      {
        if ((flag & WITHA) != 0)
        {
          _flags.useD = true;
          _flags.useDE = true;
          _vectSize += dim;
          _vectSize++;
        }
      }
    }
    else
    {
      if ((flag & WITHD) != 0)
      {
        _flags.useD = true;
        _vectSize += dim;
        if ((flag & WITHA) != 0)
        {
          _flags.useDD = true;
          _vectSize += dim;
        }
      }
      else
        if ((flag & WITHA) != 0)
        {
          _flags.useDD = true;
          _vectSize += dim;
        }
    }
    _paramDefined = true;
    return true;
  }
  else if (sizeof(unsigned int) == 4)
  {
    typedef unsigned int flag_t; /* Flag for qualifiers       */
    SPRO3DataKind kind, kindInv;
    unsigned int dim, dimInv, n, nInv;
    flag_t flag, flagInv;
    short OK_kind, OK_reste;
    unsigned int nbElem, tailleReelle, tailleCalculee, tailleCalcInv;
    _headerLength = sizeof(SPRO3DataKind) + sizeof(unsigned int) + 
      sizeof(unsigned int) + sizeof(flag_t);
  
    /* Lecture de l'en-tete */
    kind = (SPRO3DataKind)_pReader->readInt4();
    dim = _pReader->readUInt4();
    n = _pReader->readUInt4();
    flag = _pReader->readUInt4();
  
    /* Verification du format */
    /* Test sur kind */
    OK_kind = 0;
    _pReader->swap4Bytes(&kind, &kindInv); // king -> kindInv
    if ((kind <= SPRO3DataKind_LAR) && (kind >= SPRO3DataKind_OTHER)) // ???
      OK_kind++;
    if ((kindInv <= SPRO3DataKind_LAR) && (kindInv >= SPRO3DataKind_OTHER))
      OK_kind--;
    else
      if (OK_kind == 0) // si dans aucun des cas on n'a une valeur
        return false; // correcte c'est que le fichier n'est pas bon.
  
    /* Test sur dim, n, flag */

    OK_reste = 0;

    // determine taille reelle du fichier

    tailleReelle = _pReader->getFileLength();

    // calcule taille theorique du fichier
  
    nbElem = dim;
    if (flag & WITHE)
      nbElem++;
    if (flag & WITHD) {
      if (flag & WITHA) {
        nbElem *= 3;
      }
      else {
        nbElem += nbElem;
      }
    }
    if (flag & WITHN)
      nbElem--;
    nbElem *= n;
    tailleCalculee = nbElem*sizeof(param_t) + sizeof(SPRO3DataKind)
               + 2*sizeof(unsigned int) + sizeof(flag_t);

    // compare les tailles theorique et reelle du fichier
  
    if (tailleCalculee == tailleReelle)
      OK_reste++;

    _pReader->swap4Bytes(&dim,&dimInv);
    _pReader->swap4Bytes(&n,&nInv);
    _pReader->swap4Bytes(&flag,&flagInv);

    // calcule taille theorique du fichier (apres swap des donnees)

    nbElem = dimInv;
    if (flagInv & WITHE)
      nbElem++;
    if (flagInv & WITHD) {
      if (flagInv & WITHA) {
        nbElem *= 3;
      }
      else {
        nbElem += nbElem;
      }
    }
    if (flagInv & WITHN)
      nbElem--;
    nbElem *= nInv;
    tailleCalcInv = nbElem*sizeof(param_t) + sizeof(SPRO3DataKind)
            + 2*sizeof(unsigned int) + sizeof(flag_t);

    // compare les tailles theorique (apres swap) et reelle du fichier

    if (tailleCalcInv == tailleReelle)
      OK_reste--;
    else
      if (OK_reste == 0) // si dans aucun cas on ne retombe sur la bonne
        return false;  // bonne c'est que le fichier n'est pas bon.

    /* Decision */
    /* OK_kind + OK_reste > 0  => pas de byte swap */
    /* OK_kind + OK_reste < 0  => byte swap */
    /* OK_kind + OK_reste = 0  => indetermine, ou fichier foireux */

    if ((OK_kind + OK_reste == 0) && (OK_kind != OK_reste))
      return false;

    if (OK_kind + OK_reste < 0)
    {
      kind = kindInv;
      dim = dimInv;
      n = nInv;
      flag = flagInv;
      _pReader->swap() = true;
    }
  
    /* Valeurs de retour */
    _kind = kind;
    _featureCount = n;
    _vectSize = dim;

    _flags.useS = true;
  
    /* version Teva
    if (flag & WITHE)
      _vectSize++;
    if (flag & WITHD)
      if (flag & WITHA)
        _vectSize *= 3;
      else
        _vectSize *= 2;
    if (flag & WITHN)
      _vectSize--;*/

    /* version Sylvain */
    if ((flag & WITHE) != 0)
    {
      _flags.useE = true;
      _vectSize++;
      if ((flag & WITHD) != 0)
      {
        _flags.useD = true;
        _vectSize += dim;
        if ((flag & WITHNODE) == 0)
        {
          _flags.useDE = true;
          _vectSize++;
        }
        if ((flag & WITHA) != 0)
        {
          _flags.useDD = true;
          _flags.useDDE = true;
          _vectSize += dim;
          _vectSize++;
        }
      }
      else
      {
        if ((flag & WITHA) != 0)
        {
          _flags.useD = true;
          _flags.useDE = true;
          _vectSize += dim;
          _vectSize++;
        }
      }
    }
    else
    {
      if ((flag & WITHD) != 0)
      {
        _flags.useD = true;
        _vectSize += dim;
        if ((flag & WITHA) != 0)
        {
          _flags.useDD = true;
          _vectSize += dim;
        }
      }
      else
        if ((flag & WITHA) != 0)
        {
          _flags.useDD = true;
          _vectSize += dim;
        }
    }
    _paramDefined = true;
    return true;
  }
  else
    return false; // la y'a un probleme !
}
//-------------------------------------------------------------------------
R::~FeatureFileReaderSPro3() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FeatureFileReaderSPro3_cpp)

