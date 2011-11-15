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

#if !defined(ALIZE_SegServer_cpp)
#define ALIZE_SegServer_cpp

#include <new>
#include "SegServer.h"
#include "Exception.h"
#include "SegServerFileWriter.h"
#include "SegServerFileReaderRaw.h"
#include "Config.h"

using namespace alize;

//-------------------------------------------------------------------------
SegServer::SegServer()
:Object(), _nextClusterId(0) {}
//-------------------------------------------------------------------------
SegServer::SegServer(const SegServer& ss)
:Object() { assign(ss); }
//-------------------------------------------------------------------------
const SegServer& SegServer::operator=(const SegServer& ss)
{
  assign(ss);
  return *this;
}
//-------------------------------------------------------------------------
void SegServer::assign(const SegServer& ss)
{
  removeAllSegs();
  removeAllClusters();
  _map = ss._map;
  _nextClusterId = ss._nextClusterId;
  _serverName = ss._serverName;
  unsigned long i;
  for (i=0; i<ss._segVect.size(); i++)
  { _segVect.addObject(ss.getSeg(i).duplicate(K::k, *this)); }
  for (i=0; i<ss._clusterVect.size(); i++)
  {
    const SegCluster& cl = ss.getCluster(i);
    SegCluster& newCluster = cl.duplicate(K::k, *this);
    newCluster.setId(K::k, cl.getId());
    _clusterVect.addObject(newCluster);
    for (unsigned long j=0; j<cl.getCount(); j++)
    { newCluster.add(matchingSegAbstract(cl.get(j))); }
  }
}
//-------------------------------------------------------------------------
SegAbstract& SegServer::matchingSegAbstract(const SegAbstract& s) const
{  // private
  unsigned long i;
  const SegServer& ss = s.getServer();
  for (i=0; i<ss.getSegCount(); i++)
  {
    if (ss.getSeg(i).isSameObject(s))
      return getSeg(i);
  }
  for (i=0; i<ss.getClusterCount(); i++)
  {
    if (ss.getCluster(i).isSameObject(s))
      return getCluster(i);
  }
  assert(false);
  return getSeg(0); // never called
}
//-------------------------------------------------------------------------
void SegServer::assertServerOwns(const SegAbstract& s) const // private
{
  if (!s.getServer().isSameObject(*this))
    throw Exception("The segment does not belong to the server",
          __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
SegServer& SegServer::create() // static
{
  SegServer* p = new (std::nothrow) SegServer();
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
Seg& SegServer::createSeg(unsigned long b, unsigned long l,
             unsigned long lc, const String& s, const String& sn)
{
  Seg& seg = Seg::create(K::k, *this, b, l, lc, s, sn);
  _segVect.addObject(seg);
  return seg;
}
//-------------------------------------------------------------------------
Seg& SegServer::duplicateSeg(const Seg& s)
{
  Seg& seg = s.duplicate(K::k, *this);
  _segVect.addObject(seg);
  return seg;
}
//-------------------------------------------------------------------------
SegCluster& SegServer::createCluster(unsigned long lc, const String& s,
                                                          const String& sn)
{
  SegCluster& cluster = SegCluster::create(K::k, *this, lc, s, sn);
  while ( getClusterIndexById(_nextClusterId) > 0)
    _nextClusterId++;
  cluster.setId(K::k, _nextClusterId);
  _map[_nextClusterId] = _clusterVect.addObject(cluster);
  _nextClusterId++;
  return cluster;
}
//-------------------------------------------------------------------------
void SegServer::remove(SegAbstract& s)
{
  assertServerOwns(s);
  s.removeAllOwners(K::k);
  if (dynamic_cast<const Seg*>(&s) != NULL)
    _segVect.removeObject(static_cast<Seg&>(s));
  else
  {
    SegCluster& cl = static_cast<SegCluster&>(s);
    cl.removeAll();
    _clusterVect.removeObject(cl);
    _map.erase(cl.getId());
  }
  delete &s;
}
//-------------------------------------------------------------------------
void SegServer::removeAllSegs()
{
  while (getSegCount() != 0)
    remove(getSeg(getSegCount()-1));
}
//-------------------------------------------------------------------------
void SegServer::removeAllClusters()
{
  while (getClusterCount() != 0)
    remove(getCluster(getClusterCount()-1));
  _nextClusterId = 0;
}
//-------------------------------------------------------------------------
Seg& SegServer::getSeg(unsigned long idx) const
{ return _segVect.getObject(idx); }
//-------------------------------------------------------------------------
unsigned long SegServer::getSegCount() const { return _segVect.size(); }
//-------------------------------------------------------------------------
SegCluster& SegServer::getCluster(unsigned long idx) const
{ return _clusterVect.getObject(idx); }
//-------------------------------------------------------------------------
long SegServer::getClusterIndexById(unsigned long id) const
{
  it_t i = _map.find(id);
  if (i == _map.end())
    return -1;
  return i->second;
}
//-------------------------------------------------------------------------
void SegServer::setClusterId(SegCluster& cl, unsigned long id)
{
  it_t i = _map.find(id);
  if (i != _map.end() && !getCluster(i->second).isSameObject(cl))
    throw Exception("Cluster with id='" + String::valueOf(id) +
          "' already exists in the server", __FILE__, __LINE__);
  unsigned long oldId = cl.getId();
  unsigned long idx = _map.find(oldId)->second;
  _map.erase(oldId);
  _map[id] = idx;
  cl.setId(K::k, id);
}
//-------------------------------------------------------------------------
SegCluster& SegServer::getClusterById(unsigned long id) const
{
  it_t i = _map.find(id);
  if (i == _map.end())
    throw Exception("Cluster with id='" + String::valueOf(id) +
          "' does not exist in the server", __FILE__, __LINE__);
  return getCluster(i->second);
}
//-------------------------------------------------------------------------
unsigned long SegServer::getIndex(const SegAbstract& s) const
{
  assertServerOwns(s);
  if (dynamic_cast<const Seg*>(&s) != NULL)
  {
    for (unsigned long i=0; i<_segVect.size(); i++)
    {
      if (_segVect.getObject(i).isSameObject(s))
        return i;
    }
  }
  else if (dynamic_cast<const SegCluster*>(&s) != NULL)
  {
    for (unsigned long i=0; i<_segVect.size(); i++)
    {
      if (_clusterVect.getObject(i).isSameObject(s))
        return i;
    }
  }
  else
    throw Exception("Unexpected object", __FILE__, __LINE__);
  return 0; // never called
}
//-------------------------------------------------------------------------
unsigned long SegServer::getClusterCount() const
{ return _clusterVect.size(); }
//-------------------------------------------------------------------------
void SegServer::setServerName(const String& s) { _serverName = s; }
//-------------------------------------------------------------------------
const String& SegServer::getServerName() const { return _serverName; }
//-------------------------------------------------------------------------
void SegServer::save(const FileName& f, const Config& c) const
{ SegServerFileWriter(f, c).writeSegServer(*this); }
//-------------------------------------------------------------------------
void SegServer::load(const FileName& f, const Config& c)
{
  if (f.endsWith(".xml"))
  {
    //SegServerFileReaderXml r(f, c);
    //r.readSegServer(*this);
    //return;
    throw Exception("cannot read XML format for seg server",
                          __FILE__, __LINE__);
  }
  switch (c.getParam_loadSegServerFileFormat())
  {
    case SegServerFileReaderFormat_XML:
    {
      //SegServerFileReaderXml r(f, c);
      //r.readSegServer(*this);
      //break;
    }
    case SegServerFileReaderFormat_RAW:
    {
      SegServerFileReaderRaw r(f, c);
      r.readSegServer(*this);
      break;
    }
    case SegServerFileReaderFormat_LIUM:
    {
      //SegServerFileReaderXml r(f, c);
      //r.readSegServer(*this);
      //break;
    }
    default:
      throw Exception("Unable to read this seg server file format",
        __FILE__, __LINE__);
  }
}
//-------------------------------------------------------------------------
String SegServer::getClassName() const { return "SegServer"; }
//-------------------------------------------------------------------------
String SegServer::toString() const
{
  return Object::toString() + "\n  name = '" + _serverName + "'"
    + "\n  seg count = " + String::valueOf(getSegCount())
    + "\n  cluster count = " + String::valueOf(getClusterCount());
}
//-------------------------------------------------------------------------
SegServer::~SegServer()
{
  removeAllClusters();
  removeAllSegs();
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_SegServer_cpp)
