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

#if !defined(ALIZE_MixtureGD_cpp)
#define ALIZE_MixtureGD_cpp

#include <new>
#include "MixtureGD.h"
#include "DistribGD.h"
#include "Exception.h"
#include "MixtureStat.h"
#include "MixtureGDStat.h"
//#include "Config.h"
#include "alizeString.h"
//#include <iostream>
#include "StatServer.h"

using namespace alize;

//-------------------------------------------------------------------------
MixtureGD::MixtureGD(const String& id, unsigned long vs, unsigned long dc)
:Mixture(id, dc, vs)
{
  for (unsigned long c=0; c<dc; c++)
  { Mixture::addDistrib(K::k, DistribGD::create(K::k, _vectSize)); }
  equalizeWeights();
}
//-------------------------------------------------------------------------
/*MixtureGD::MixtureGD(const Config& c)
:Mixture("", c.getParam_mixtureDistribCount()),
 _vectSize(c.getParam_vectSize())
{
  for (unsigned long i=0; i<c.getParam_mixtureDistribCount(); i++)
    Mixture::addDistrib(DistribGD::create(c));
  equalizeWeights();
}*/
//-------------------------------------------------------------------------
MixtureGD::MixtureGD(const MixtureGD& m)
:Mixture(m._id, m.getDistribCount(), m._vectSize)
{
  // Attention : les distributions ne sont pas copiees, la copie pointe sur
  // les mêmes distributions que l'original <FRANCAIS>
  unsigned long n = m.getDistribCount();
  for (unsigned long i=0; i< n; i++)
  { Mixture::addDistrib(K::k, m.getDistrib(i), m.weight(i)); }
}
//-------------------------------------------------------------------------
bool MixtureGD::operator==(const Mixture& m) const
{
  // ** ne compare pas les id qui sont propres a chaque mixture **
  if (dynamic_cast<const MixtureGD*>(&m) == NULL)
    return false;
  const unsigned long n = getDistribCount();
  if (n != m.getDistribCount())
    return false;
  /*for (unsigned j=0; j<n; j++)
  {
    std::cout << getDistrib(j).toString() << std::endl;
    std::cout << m.getDistrib(j).toString() << std::endl;
  }*/
      
  for (unsigned i=0; i<n; i++)
  {
    if (weight(i) != m.weight(i) || getDistrib(i) != m.getDistrib(i))
      return false;
  }
  return true;
}
//-------------------------------------------------------------------------
MixtureGD& MixtureGD::create(const K&, const String& id,
            unsigned long vectSize, unsigned long distribCount)
{
  MixtureGD* p = new (std::nothrow) MixtureGD(id, vectSize, distribCount);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
/*MixtureGD& MixtureGD::create(const Config& c)
{
  return create("", c.getParam_mixtureDistribCount(),
            c.getParam_vectSize());
}*/
//-------------------------------------------------------------------------
Mixture& MixtureGD::clone(DuplDistrib d) const // private
{
  MixtureGD* p = new (std::nothrow) MixtureGD(*this);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  if (d == DUPL_DISTRIB)
  {
    const unsigned long n = getDistribCount();
    for (unsigned long c=0; c<n; c++)
    { p->setDistrib(K::k, getDistrib(c).duplicate(K::k), c); }
  }
  return *p;
}
//-------------------------------------------------------------------------
MixtureGD& MixtureGD::duplicate(const K&, DuplDistrib d) const
{
  return static_cast<MixtureGD&>(clone(d));
}
//-------------------------------------------------------------------------
MixtureGD& MixtureGD::operator=(const MixtureGD& m)
{
  assign(m);
  // assign can throw Exception if the number of distributions is
  // different or if the vectSize is different
  return *this;
}
//-------------------------------------------------------------------------
Mixture& MixtureGD::operator=(const Mixture& m)
{
  const MixtureGD* p = dynamic_cast<const MixtureGD*>(&m);
  if (p == NULL)
    throw Exception("the mixture source is not a mixture GD",
            __FILE__, __LINE__);
  assign(m);
  // assign can throw Exception if the number of distributions is
  // different or if the vectSize is different
  return *this;
}
//-------------------------------------------------------------------------
DistribGD& MixtureGD::getDistrib(unsigned long i) const
{
  return static_cast<DistribGD&>(Mixture::getDistrib(i));
  // can throw IndexOutOfBoundsException
}
//-------------------------------------------------------------------------
void MixtureGD::setDistrib(const K&, DistribGD& d, unsigned long i)
{
  if (d.getVectSize() != _vectSize)
    throw Exception("mixture vectSize (" + String::valueOf(_vectSize)
            + ") <> distrib vectSize ("
            + String::valueOf(d.getVectSize()) + ")",
            __FILE__, __LINE__);
  Mixture::setDistrib(K::k, d, i);
}
//-------------------------------------------------------------------------
void MixtureGD::setDistrib(const K&, Distrib& d, unsigned long i)
{
  DistribGD* p = dynamic_cast<DistribGD*>(&d);
  if (p == NULL)
    throw Exception("incompatibility mixture type / distribution type",
            __FILE__, __LINE__);
  MixtureGD::setDistrib(K::k, *p, i);
}
//-------------------------------------------------------------------------
void MixtureGD::addDistrib(const K&, DistribGD& d, weight_t w)
{
  if (d.getVectSize() != _vectSize)
    throw Exception("mixture vectSize (" + String::valueOf(_vectSize)
            + ") <> distrib vectSize ("
            + String::valueOf(d.getVectSize()) + ")",
            __FILE__, __LINE__);
  Mixture::addDistrib(K::k, d, w);
}
//-------------------------------------------------------------------------
void MixtureGD::addDistrib(const K&, Distrib& d, weight_t w)
{
  DistribGD* p = dynamic_cast<DistribGD*>(&d);
  if (p == NULL)
    throw Exception("incompatibility mixture/distribution",
            __FILE__, __LINE__);
  MixtureGD::addDistrib(K::k, *p, w);
}
//-------------------------------------------------------------------------
MixtureStat& MixtureGD::createNewMixtureStatObject(const K&,
                                    StatServer& ss, const Config& c) const
{ return MixtureGDStat::create(K::k, ss, *this, c); }
//-------------------------------------------------------------------------
DistribType MixtureGD::getType() const { return DistribType_GD; }
//-------------------------------------------------------------------------
String MixtureGD::getClassName() const { return "MixtureGD"; }
//-------------------------------------------------------------------------
String MixtureGD::toString() const
{
  String s = Object::toString()
    + "\n  id      = '" + _id + "'"
    + "\n  distribCount  = " + String::valueOf(getDistribCount())
    + "\n  vectSize    = " + String::valueOf(_vectSize);
  for (unsigned long i=0; i<getDistribCount(); i++)
  {
    s += "\n  weight[" + String::valueOf(i) + "] = "
      + String::valueOf(weight(i))
      + "  distrib[" + String::valueOf(i) + "] = [ "
      + getDistrib(i).getClassName()
      + " " + getDistrib(i).getAddress() + " ]";
  }
  return s;
}
//-------------------------------------------------------------------------
MixtureGD::~MixtureGD() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureGD_cpp)

