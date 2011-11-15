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

#if !defined(ALIZE_Mixture_cpp)
#define ALIZE_Mixture_cpp

#include "Mixture.h"
#include "MixtureGD.h"
#include "MixtureGF.h"
#include "Distrib.h"
#include "Exception.h"
#include "Config.h"
#include "MixtureFileWriter.h"

using namespace alize;
typedef Mixture M;
//-------------------------------------------------------------------------
M::Mixture(const String& id, unsigned long distribCount, unsigned long v)
:Object(), _vectSize(v), _weightVect(distribCount),
 _distribVect(distribCount), _id(id) {}
//-------------------------------------------------------------------------
bool M::operator!=(const Mixture& m) const { return !(*this == m); }
//-------------------------------------------------------------------------
void M::assign(const Mixture& m) // private
{
  if (this == &m)
    return;
  const unsigned long n = getDistribCount();
  if (n != m.getDistribCount())
    throw Exception("target distribCount ("
              + String::valueOf(getDistribCount())
              + ") != source distribCount ("
              + String::valueOf(m.getDistribCount())
              + ")", __FILE__, __LINE__);
  for (unsigned long i=0; i<n; i++)
  {
    getDistrib(i) = m.getDistrib(i); // operator= overloaded and
                     // can throw Exception
    weight(i) = m.weight(i);
  }
  // DOES NOT copy the identifier : _id = m._id;
}
//-------------------------------------------------------------------------
void M::reset()
{
  for (unsigned long c=0; c<getDistribCount(); c++)
  { getDistrib(c).reset(); }
  equalizeWeights();
}
//-------------------------------------------------------------------------
void M::equalizeWeights()
{
  unsigned long n = getDistribCount();
  if (n > 0)
  {
    weight_t w = (weight_t)1.0/n;
    for (unsigned long c=0; c<n; c++)
      weight(c) = w;
  }
}
//-------------------------------------------------------------------------
void M::removeAllDistrib(const K&)
{
  _distribVect.clear();
  _weightVect.clear();
}
//-------------------------------------------------------------------------
Mixture& M::duplicate(const K&, DuplDistrib d) const
{ return clone(d); }
//-------------------------------------------------------------------------
void M::setDistrib(const K&, Distrib& d, unsigned long i)
{
  _distribVect.setDistrib(d, i); // can throw IndexOutOfBoundsException
}
//-------------------------------------------------------------------------
void M::addDistrib(const K&, Distrib& d, weight_t w)
{
  _distribVect.addDistrib(d);
  _weightVect.addValue(w);
}
//-------------------------------------------------------------------------
Distrib& M::getDistrib(unsigned long i) const
{
  return _distribVect.getDistrib(i); // can throw IndexOutOfBoundsException
}
//-------------------------------------------------------------------------
weight_t& M::weight(unsigned long index)
{ return _weightVect[index]; /* can throw IndexOutOfBoundsException */ }
//-------------------------------------------------------------------------
weight_t M::weight(unsigned long index) const
{ return _weightVect[index]; /* can throw IndexOutOfBoundsException */}
//-------------------------------------------------------------------------
void M::save(const FileName& f, const Config& c) const
{ MixtureFileWriter(f, c).writeMixture(*this); }
//-------------------------------------------------------------------------
DoubleVector& M::getTabWeight() { return _weightVect; }
//-------------------------------------------------------------------------
const DoubleVector& M::getTabWeight() const { return _weightVect; }
//-------------------------------------------------------------------------
Distrib** M::getTabDistrib() const
{ return _distribVect.getArray(); }
//-------------------------------------------------------------------------
String M::getId() const { return _id; }
//-------------------------------------------------------------------------
void M::setId(const K&, const String& id) { _id = id; }
//-------------------------------------------------------------------------
void M::setId(const String& id) { _id = id; }
//-------------------------------------------------------------------------
unsigned long M::getDistribCount() const
{ return _distribVect.size(); }
//-------------------------------------------------------------------------
void M::computeAll()
{
  for (unsigned long i=0; i<getDistribCount(); i++)
    getDistrib(i).computeAll();
}
//-------------------------------------------------------------------------
unsigned long M::getVectSize() const { return _vectSize; }
//-------------------------------------------------------------------------
// static method
//-------------------------------------------------------------------------
Mixture& M::create(const K&, const unsigned long dc,
                         const DistribType type, const String& id,
                         const unsigned long vectSize)
{
  if (type == DistribType_GD)
    return MixtureGD::create(K::k, id, vectSize, dc);
  if (type == DistribType_GF)
    return MixtureGF::create(K::k, id, vectSize, dc);
  throw Exception("Don't know how to create a mixture '"
          + getDistribTypeName(type) + "'", __FILE__, __LINE__);
  return MixtureGD::create(K::k, "", 1); // never called
}
//-------------------------------------------------------------------------
//unsigned long M::getVectSize() const { return _vectSize; }
//-------------------------------------------------------------------------
String M::toString() const
{
  String s = Object::toString()
    + "\n  id      = '" + _id + "'"
    + "\n  distribCount  = " + String::valueOf(getDistribCount());
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
M::~Mixture() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_Mixture_cpp)

