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

#if !defined(ALIZE_SegServerFileReaderRaw_cpp)
#define ALIZE_SegServerFileReaderRaw_cpp

#include <new>
#include "SegServerFileReaderRaw.h"
#include "SegServer.h"
#include "Exception.h"
#include "Config.h"
#include "ULongVector.h"
//#include <iostream>

using namespace alize;
//using namespace std;
typedef SegServerFileReaderRaw R;

//-------------------------------------------------------------------------
R::SegServerFileReaderRaw(const FileName& f,const Config& c)
:SegServerFileReaderAbstract(f, c) {}
//-------------------------------------------------------------------------
R& R::create(const FileName& n,const Config& c)
{
  R* p = new (std::nothrow) R(n, c);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void R::readSegServer(SegServer& ss)
{
  assert(_pReader != NULL);
  ss.removeAllClusters();
  ss.removeAllSegs();
  unsigned long i, j, k, n, nbLines, nbElements, idxCluster;

  ss.setServerName(_pReader->readString(_pReader->readUInt4()));
  n = _pReader->readUInt4(); // nb segments
  for (i=0; i<n; i++)
  {
    unsigned long b = _pReader->readUInt4(); // begin
    unsigned long l = _pReader->readUInt4(); // length
    unsigned long lc = _pReader->readUInt4(); // label code
    const String st = _pReader->readString(_pReader->readUInt4()); // string
    const String sn = _pReader->readString(_pReader->readUInt4()); // source name
    Seg& seg = ss.createSeg(b, l, lc, st, sn);
    nbLines = _pReader->readUInt4();
    XList& list = seg.list();
    list.reset();
    for (j=0; j<nbLines; j++)
    {
      XLine& line = list.addLine();
      nbElements = _pReader->readUInt4();
      for (k=0; k<nbElements; k++)
      { line.addElement(_pReader->readString(_pReader->readUInt4())); }
    }
  }
  unsigned long nbClusters = _pReader->readUInt4(); // nb clusters
  ULongVector idxClusterVect, typeVect, idxSubSegVect, idClusterVect;
  for (idxCluster=0; idxCluster<nbClusters; idxCluster++)
  {
    unsigned long id = _pReader->readUInt4(); // id
    unsigned long lc = _pReader->readUInt4(); // label code
    const String st = _pReader->readString(_pReader->readUInt4()); // string
    const String sn = _pReader->readString(_pReader->readUInt4()); // source name
    SegCluster& cl = ss.createCluster(lc, st, sn);
    ss.setClusterId(cl, id);

    // cluster list
    nbLines = _pReader->readUInt4();
    XList& list = cl.list();
    list.reset();
    for (j=0; j<nbLines; j++)
    {
      XLine& line = list.addLine();
      nbElements = _pReader->readUInt4();
      for (k=0; k<nbElements; k++)
      { line.addElement(_pReader->readString(_pReader->readUInt4())); }
    }
    // cluster sub segments
    unsigned long nbSubSeg = _pReader->readUInt4();
    for (j=0; j<nbSubSeg; j++)
    {
      idxClusterVect.addValue(idxCluster);
      typeVect.addValue(_pReader->readString(1).c_str()[0]);
      idxSubSegVect.addValue(_pReader->readUInt4());
      //cout << id << " " << idSubSegVect[idSubSegVect.size()-1] << endl;
    }
  }
  for (idxCluster=0; idxCluster<idxClusterVect.size(); idxCluster++)
  {
    if (typeVect[idxCluster] == 's')
      ss.getCluster(idxClusterVect[idxCluster])
        .add(ss.getSeg(idxSubSegVect[idxCluster]));
    else
      ss.getCluster(idxClusterVect[idxCluster])
        .add(ss.getCluster(idxSubSegVect[idxCluster]));
  }
  _pReader->close();
}
//-------------------------------------------------------------------------
void R::error(const String& msg)
{
  assert(_pReader != NULL);
  _pReader->close();
  throw InvalidDataException(msg, __FILE__, __LINE__,
                             _pReader->getFullFileName());
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "SegServerFileReaderRaw"; }
//-------------------------------------------------------------------------
R::~SegServerFileReaderRaw() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_SegServerFileReaderRaw_cpp)
