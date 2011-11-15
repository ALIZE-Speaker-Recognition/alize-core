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

#if !defined(ALIZE_MixtureServer_cpp)
#define ALIZE_MixtureServer_cpp

//#include <cstdlib>

#include <ctime>
#include "MixtureServer.h"
#include "MixtureFileReader.h"
#include "MixtureServerFileReader.h"
#include "MixtureServerFileWriter.h"
#include "MixtureGD.h"
#include "MixtureGF.h"
#include "DistribGD.h"
#include "DistribGF.h"
#include "Exception.h"
#include "XLine.h"
#include "ULongVector.h"

using namespace alize;
typedef MixtureServer S;

//-------------------------------------------------------------------------
S::MixtureServer(const Config& c)
:Object(), _config(c) { reset(); }
//-------------------------------------------------------------------------
S::MixtureServer(const FileName& f, const Config& c)
:Object(), _config(c)
{
  reset();
  load(f);
}
//-------------------------------------------------------------------------
void S::reset()
{
  _mixtureDict.clear(); // delete all mixtures
  _distribDict.clear(); // delete all distributions
  _lastMixtureId = 0;
  _vectSizeDefined = false;
}
//-------------------------------------------------------------------------
Distrib& S::createDistrib()
{
  unsigned long vectSize;
  if (_vectSizeDefined)
    vectSize = _vectSize;
  else
    vectSize = _config.getParam_vectSize();
  return createDistrib(_config.getParam_distribType(), vectSize);
}
//-------------------------------------------------------------------------
DistribGD& S::createDistribGD()
{
  unsigned long vectSize;
  if (_vectSizeDefined)
    vectSize = _vectSize;
  else
    vectSize = _config.getParam_vectSize();
  return static_cast<DistribGD&>(createDistrib(DistribType_GD, vectSize));
}
//-------------------------------------------------------------------------
DistribGF& S::createDistribGF()
{
  unsigned long vectSize;
  if (_vectSizeDefined)
    vectSize = _vectSize;
  else
    vectSize = _config.getParam_vectSize();
  return static_cast<DistribGF&>(createDistrib(DistribType_GF, vectSize));
}
//-------------------------------------------------------------------------
Distrib& S::createDistrib(const DistribType type, unsigned long vectSize)
{
  if (_vectSizeDefined && vectSize != _vectSize)
    throw Exception("Incompatible vectSize", __FILE__, __LINE__);
  Distrib& d = Distrib::create(K::k, type, vectSize);
  addDistribToDict(d);
  _vectSize = vectSize;
  _vectSizeDefined = true;
  return d;
}
//-------------------------------------------------------------------------
void S::addDistribToDict(Distrib& d) // private
{ d.dictIndex(K::k) = _distribDict.addDistrib(d); }
//-------------------------------------------------------------------------
void S::addMixtureToDict(Mixture& m) // private
{ _mixtureDict.addMixture(m); }
//-------------------------------------------------------------------------
Mixture& S::createMixture()
{
  return createMixture(_config.getParam_mixtureDistribCount(),
                     _config.getParam_distribType());
}
//-------------------------------------------------------------------------
Mixture& S::createMixture(unsigned long dc)
{ return createMixture(dc, _config.getParam_distribType()); }
//-------------------------------------------------------------------------
MixtureGD& S::createMixtureGD()
{
  Mixture& m = createMixture(_config.getParam_mixtureDistribCount(),
                            DistribType_GD);
  return static_cast<MixtureGD&>(m);
}
//-------------------------------------------------------------------------
MixtureGF& S::createMixtureGF()
{
  Mixture& m = createMixture(_config.getParam_mixtureDistribCount(),
                            DistribType_GF);
  return static_cast<MixtureGF&>(m);
}
//-------------------------------------------------------------------------
MixtureGD& S::createMixtureGD(unsigned long dc)
{ return static_cast<MixtureGD&>(createMixture(dc, DistribType_GD)); }
//-------------------------------------------------------------------------
MixtureGF& S::createMixtureGF(unsigned long dc)
{ return static_cast<MixtureGF&>(createMixture(dc, DistribType_GF)); }
//-------------------------------------------------------------------------
Mixture& S::createMixture(unsigned long dc, DistribType type)
{
  unsigned long vectSize;
  if (_vectSizeDefined)
    vectSize = _vectSize;
  else
    vectSize = _config.getParam_vectSize();
  Mixture& m = Mixture::create(K::k, dc, type, newId(), vectSize);
  addMixtureToDict(m);
  const unsigned long n = m.getDistribCount();
  for (unsigned long c=0; c<n; c++)
  { addDistribToDict(m.getDistrib(c)); }
  _vectSize = vectSize;
  _vectSizeDefined = true;
  return m;
}
//-------------------------------------------------------------------------
String S::newId() // private
{
  String id;
  do { id = "#" + String::valueOf(++_lastMixtureId); }
  while (_mixtureDict.getIndexOfId(id) != -1);
  return id;
}
//-------------------------------------------------------------------------
void S::setMixtureId(Mixture& mixture, const String& id)
{
  /*long i = _mixtureDict.getIndexOfId(id);
  if (i != -1 && &getMixture(i) != &mixture)
    throw IdAlreadyExistsException(id, __FILE__, __LINE__);*/
  _mixtureDict.setMixtureId(mixture, id);
}
//-------------------------------------------------------------------------
Mixture& S::duplicateMixture(const Mixture& mixture, DuplDistrib d)
{
  Mixture& m = mixture.duplicate(K::k, d);
  String id(mixture.getId());
  unsigned long cpt = 1;
  while (_mixtureDict.getIndexOfId(id) != -1) // if the id already exists
    id = mixture.getId() + " #" + String::valueOf(++cpt);
  m.setId(K::k, id);
  addMixtureToDict(m);

  if (d == DUPL_DISTRIB)
  {
    const unsigned long n= m.getDistribCount();
    for (unsigned long c=0; c<n; c++)
    { addDistribToDict(m.getDistrib(c)); }
  }
  return m;
}
//-------------------------------------------------------------------------
MixtureGD& S::duplicateMixture(const MixtureGD& m, DuplDistrib d)
{
  return static_cast<MixtureGD&>(duplicateMixture(
            static_cast<const Mixture&>(m), d));
}
//-------------------------------------------------------------------------
MixtureGF& S::duplicateMixture(const MixtureGF& m, DuplDistrib d)
{
  return static_cast<MixtureGF&>(duplicateMixture(
            static_cast<const Mixture&>(m), d));
}
//-------------------------------------------------------------------------
MixtureGD& S::duplicateMixtureGD(const Mixture& m, DuplDistrib d)
{ return static_cast<MixtureGD&>(duplicateMixture(m, d)); }
//-------------------------------------------------------------------------
MixtureGF& S::duplicateMixtureGF(const Mixture& m, DuplDistrib d)
{ return static_cast<MixtureGF&>(duplicateMixture(m, d)); }
//-------------------------------------------------------------------------
DistribGD& S::duplicateDistrib(const DistribGD& d)
{
  return static_cast<DistribGD&>(duplicateDistrib(
                  static_cast<const Distrib&>(d)));
}
//-------------------------------------------------------------------------
DistribGF& S::duplicateDistrib(const DistribGF& d)
{
  return static_cast<DistribGF&>(duplicateDistrib(
                  static_cast<const Distrib&>(d)));
}
//-------------------------------------------------------------------------
/*DistribGD& S::duplicateDistribGD(const Distrib& d)
{
  const DistribGD* p = dynamic_cast<const DistribGD*>(&d);
  if (p == NULL)
    throw Exception("The distrib is not a GD", __FILE__, __LINE__);
  return static_cast<DistribGD&>(duplicateDistrib(*p));
}*/
//-------------------------------------------------------------------------
Distrib& S::duplicateDistrib(const Distrib& distrib)
{
  Distrib& d = distrib.duplicate(K::k);
  addDistribToDict(d);
  return d;
}
//-------------------------------------------------------------------------
long S::getMixtureIndex(const String& id) const
{ return _mixtureDict.getIndexOfId(id); }
//-------------------------------------------------------------------------
void S::setDistribToMixture(Mixture& m, Distrib& d, weight_t w,
                            unsigned long i)
{
  m.setDistrib(K::k, d, i); // can throw IndexOutOfBoundsException
  m.weight(i) = w; // can throw IndexOutOfBoundsException
}
//-------------------------------------------------------------------------
void S::addDistribToMixture(Mixture& m, Distrib& d, weight_t w)
{ m.addDistrib(K::k, d, w); }
//-------------------------------------------------------------------------
MixtureGD& S::loadMixtureGD(const FileName& f)
{ return static_cast<MixtureGD&>(loadMixture(f, DistribType_GD)); }
//-------------------------------------------------------------------------
MixtureGF& S::loadMixtureGF(const FileName& f)
{ return static_cast<MixtureGF&>(loadMixture(f, DistribType_GF)); }
//-------------------------------------------------------------------------
Mixture& S::loadMixture(const FileName& f)
{
  MixtureFileReader r(f, _config);
  const Mixture& m0 = r.readMixture();
  if (!_config.existsParam_vectSize)
    const_cast<Config&>(_config)
                   .setParam("vectSize", String::valueOf(m0.getVectSize()));
  Mixture& m = createMixture(m0.getDistribCount(), m0.getType());
  m = m0;
  autoSetMixtureId(m, f);
  return m;
}
//-------------------------------------------------------------------------
void S::loadMixture(Mixture& m, const FileName& f)
{
  MixtureFileReader r(f, _config);
  m = r.readMixture();
  autoSetMixtureId(m, f);
}
//-------------------------------------------------------------------------
Mixture& S::loadMixture(const FileName& f, DistribType type) // private
{
  MixtureFileReader r(f, _config);
  const Mixture& m0 = r.readMixture(type);    
  if (!_config.existsParam_vectSize)
    const_cast<Config&>(_config)
                   .setParam("vectSize", String::valueOf(m0.getVectSize()));
  Mixture& m = createMixture(m0.getDistribCount(), type);
  m = m0; // operator= overloaded. // Does not copy Id.
  autoSetMixtureId(m, f);
  return m;
}
//-------------------------------------------------------------------------
void S::autoSetMixtureId(Mixture& m, String id) // private
{
  const String f = id;
  unsigned long cpt = 1;
  while (_mixtureDict.getIndexOfId(id) != -1) // if the id already exists
    id = f + " #" + String::valueOf(++cpt);
  _mixtureDict.setMixtureId(m, id);
}
//-------------------------------------------------------------------------
unsigned long S::loadMixture(const XLine& l)
{
  unsigned long first = getMixtureCount();
  for (unsigned long i=0; i<l.getElementCount(); i++)
    loadMixture(l.getElement(i));
  return first;
}
//-------------------------------------------------------------------------
unsigned long S::getDistribCount() const { return _distribDict.size(); }
//-------------------------------------------------------------------------
unsigned long S::getVectSize() const
{
  if (_vectSizeDefined)
    return _vectSize;
  throw Exception("Unknown vectSize", __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
Distrib& S::getDistrib(unsigned long i) const
{ return _distribDict.getDistrib(i); }
//-------------------------------------------------------------------------
DistribGD& S::getDistribGD(unsigned long i) const
{
  DistribGD* p = dynamic_cast<DistribGD*>(&_distribDict.getDistrib(i));
  if (p == NULL)
    throw Exception("The distrib found is not a GD", __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
DistribGF& S::getDistribGF(unsigned long i) const
{
  DistribGF* p = dynamic_cast<DistribGF*>(&_distribDict.getDistrib(i));
  if (p == NULL)
    throw Exception("The distrib found is not a GF", __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
unsigned long S::getMixtureCount() const { return _mixtureDict.size(); }
//-------------------------------------------------------------------------
Mixture& S::getMixture(unsigned long i) const
{ return _mixtureDict.getMixture(i); }
//-------------------------------------------------------------------------
MixtureGD& S::getMixtureGD(unsigned long i) const
{
  MixtureGD* p = dynamic_cast<MixtureGD*>(&_mixtureDict.getMixture(i));
  if (p == NULL)
    throw Exception("The mixture found is not a GD", __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
MixtureGF& S::getMixtureGF(unsigned long i) const
{
  MixtureGF* p = dynamic_cast<MixtureGF*>(&_mixtureDict.getMixture(i));
  if (p == NULL)
    throw Exception("The mixture found is not a GF", __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
const String& S::getServerName() const { return _serverName; }
//-------------------------------------------------------------------------
void S::setServerName(const String& s) { _serverName = s; }
//-------------------------------------------------------------------------
void S::load(const FileName& f)
{ MixtureServerFileReader(f, _config).readMixtureServer(*this); }
//-------------------------------------------------------------------------
void S::deleteMixtures(unsigned long first, unsigned long last)
{
  _mixtureDict.deleteMixtures(first, last);
  if (getDistribCount() == 0 && getMixtureCount() == 0)
    _vectSizeDefined = false;
}
//-------------------------------------------------------------------------
void S::deleteMixture(const Mixture& m)
{
  _mixtureDict.deleteMixture(m);
  if (getDistribCount() == 0 && getMixtureCount() == 0)
    _vectSizeDefined = false;
}
//-------------------------------------------------------------------------
void S::deleteUnusedDistribs()
{
  _distribDict.deleteUnreferencedDistribs();
  if (getDistribCount() == 0 && getMixtureCount() == 0)
    _vectSizeDefined = false;
}
//-------------------------------------------------------------------------
void S::save(const FileName& f) const
{ MixtureServerFileWriter(f, _config).writeMixtureServer(*this); }
//-------------------------------------------------------------------------
String S::toString() const
{
  String s = Object::toString()
    + "\n  serverName   = '" + _serverName + "'"
    + "\n  distribCount = " + String::valueOf(getDistribCount())
    + "\n  mixtureCount = " + String::valueOf(getMixtureCount());
  for (unsigned long i=0; i<getMixtureCount(); i++)
    s += "\n  mixture #" + String::valueOf(i)
      + " Id = " + getMixture(i).getId();
  return s;
}
//-------------------------------------------------------------------------
String S::getClassName() const { return "MixtureServer"; }
//-------------------------------------------------------------------------
S::~MixtureServer() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureServer_cpp)

