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

#if !defined(ALIZE_SegServerFileWriter_cpp)
#define ALIZE_SegServerFileWriter_cpp

#include "SegServerFileWriter.h"
#include "Exception.h"
#include "SegServer.h"
#include "Config.h"

#include <cstdlib>

using namespace alize;
using namespace std;
typedef SegServerFileWriter W;

//-------------------------------------------------------------------------
W::SegServerFileWriter(const FileName& f,const Config& c)
:FileWriter(getFullFileName(c, f)), _config(c)
{
  if (_fileName.endsWith(".xml"))
    _format = SegServerFileWriterFormat_XML;
  else 
    _format = c.getParam_saveSegServerFileFormat();
}
//-------------------------------------------------------------------------
String W::getFullFileName(const Config& c, const String& n) const // protected
{
  if (n.beginsWith("/") || n.beginsWith("./"))
    return n;
  return c.getParam_segServerFilesPath() + n
      + c.getParam_saveSegServerFileExtension();
}
//-------------------------------------------------------------------------
void W::writeSegServer(const SegServer& ss)
{
  if (_format == SegServerFileWriterFormat_XML)
    writeSegServerXml(ss);
  else if (_format == SegServerFileWriterFormat_RAW)
    writeSegServerRaw(ss);
  else if (_format == SegServerFileWriterFormat_TRS)
    writeSegServerTrs(ss);
  //else if (_format == SegServerFileWriterFormat_LIUM)
  //  writeSegServerLium(ss);
  else
   throw Exception("I don't know how to save the segment server",
           __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
void W::writeSegServerXml(const SegServer& ss)
{
  open(); //can throw IOException
  unsigned long i;
  writeString("<SegServer");
  writeAttribute("name", ss.getServerName());
  writeString(">");
  for (i=0; i<ss.getSegCount(); i++)
  {
    const Seg& s = ss.getSeg(i);
    writeString("\n\t<Seg");
    writeAttribute("id", i);
    writeAttribute("begin", s.begin());
    writeAttribute("length", s.length());
    writeAttribute("codeLabel", s.labelCode());
    writeAttribute("string", s.string());
    writeAttribute("sourceName", s.sourceName());
    if (s.list().getLineCount() != 0)
    {
      writeString(">");
      writeListXml(s.list());
      writeString("\n\t</Seg>");
    }
    else
      writeString("/>");
  }
  for (i=0; i<ss.getClusterCount(); i++)
  {
    SegCluster& cl = ss.getCluster(i);
    writeString("\n\t<SegCluster");
    writeAttribute("id", cl.getId());
    writeAttribute("codeLabel", cl.labelCode());
    writeAttribute("string", cl.string());
    writeAttribute("sourceName", cl.sourceName());
    writeString(">");
    if (cl.list().getLineCount() != 0)
      writeListXml(cl.list());
    writeSubSegXml(ss.getCluster(i), ss);
    writeString("\n\t</SegCluster>");
  }
  writeString("\n</SegServer>");
  close();
}
//-------------------------------------------------------------------------
void W::writeSubSegXml(const SegCluster& cl, const SegServer& ss) // private
{
  for (unsigned long i=0; i<cl.getCount(); i++)
  {
    SegAbstract& s = cl.get(i);
    if (dynamic_cast<Seg*>(&s) != NULL)
      writeString("\n\t\t<Seg");
    else if (dynamic_cast<SegCluster*>(&s) != NULL)
      writeString("\n\t\t<SegCluster");
    else
      throw Exception("unexpected object", __FILE__, __LINE__);
    writeAttribute("idx", ss.getIndex(s));
    writeString("/>");
  }
}
//-------------------------------------------------------------------------
void W::writeListXml(const XList& l)
{
  for (unsigned long i=0; i<l.getLineCount(); i++)
  {
    writeString("\n\t\t<Line");
    writeAttribute("idx", i);
    writeString(">");
    const XLine& line = l.getLine(i);
    for (unsigned long j=0; j<line.getElementCount(); j++)
    {
      writeString("\n\t\t\t<element");
      writeAttribute("idx", j);
      writeString(">" + line.getElement(j) + "</element>");
    }
    writeString("\n\t\t</Line>");
  }
}  
//-------------------------------------------------------------------------
void W::writeSegServerRaw(const SegServer& ss)
{
  open(); //can throw IOException
  unsigned long i;

  writeUInt4(ss.getServerName().length());
  writeString(ss.getServerName());
  writeUInt4(ss.getSegCount());
  for (i=0; i<ss.getSegCount(); i++)
  {
    const Seg& s = ss.getSeg(i);
    writeUInt4(s.begin());
    writeUInt4(s.length());
    writeUInt4(s.labelCode());
    writeUInt4(s.string().length());
    writeString(s.string());
    writeUInt4(s.sourceName().length());
    writeString(s.sourceName());
    //writeUInt4(s.list().getLineCount());
    writeListRaw(s.list());
  }
  writeUInt4(ss.getClusterCount());
  for (i=0; i<ss.getClusterCount(); i++)
  {
    SegCluster& cl = ss.getCluster(i);
    writeUInt4(cl.getId());
    writeUInt4(cl.labelCode());
    writeUInt4(cl.string().length());
    writeString(cl.string());
    writeUInt4(cl.sourceName().length());
    writeString(cl.sourceName());
    //writeUInt4(cl.list().getLineCount());
    writeListRaw(cl.list());
    writeSubSegRaw(ss.getCluster(i), ss);
  }
  close();
}
//-------------------------------------------------------------------------
void W::writeSubSegRaw(const SegCluster& cl, const SegServer& ss) // private
{
  writeUInt4(cl.getCount());
  for (unsigned long i=0; i<cl.getCount(); i++)
  {
    SegAbstract& s = cl.get(i);
    if (dynamic_cast<Seg*>(&s) != NULL)
      writeString("s");
    else if (dynamic_cast<SegCluster*>(&s) != NULL)
      writeString("c");
    else
      throw Exception("unexpected object", __FILE__, __LINE__);
    writeUInt4(ss.getIndex(s));
  }
}
//-------------------------------------------------------------------------
void W::writeListRaw(const XList& l)
{
  writeUInt4(l.getLineCount());
  for (unsigned long i=0; i<l.getLineCount(); i++)
  {
    const XLine& line = l.getLine(i);
    unsigned long c = line.getElementCount();
    writeUInt4(c);
    for (unsigned long j=0; j<c; j++)
    {
      const String& e = line.getElement(j);
      writeUInt4(e.length());
      writeString(e);
    }
  }
}  
//-------------------------------------------------------------------------
void W::writeSegServerTrs(const SegServer& ss)
{
  open(); //can throw IOException
  unsigned long i, n, vSize, id = 0, begin = 0, end = 0, endTurn = 0;
  double sampleRate = _config.getParam_sampleRate();
  writeString("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>");
  writeString("\n<!DOCTYPE Trans SYSTEM \"trans-13.dtd\">");
  unsigned long nbCl = ss.getClusterCount();
  writeString("\n<Trans>");
  // create segVect[]
  n = 0;
  for (i=0; i<nbCl; i++)
  {
      const SegCluster& cl = ss.getCluster(i);
      cl.rewind();
      while (cl.getSeg() != NULL)
        n++;
  }
  _type* segVect = new (std::nothrow) _type[n!=0?n:1];
  assertMemoryIsAllocated(segVect, __FILE__, __LINE__);
  //
  try
  {
    writeString("\n\t<Speakers>");
    vSize = 0;
    for (i=0; i<nbCl; i++)
    {
      const SegCluster& cl = ss.getCluster(i);
      writeString("\n\t\t<Speaker");
      writeAttribute("id", "spk" + String::valueOf(cl.getId()));
      writeAttribute("name", cl.string());
      writeString("/>");
      Seg* p;
      cl.rewind();
      while ( (p = cl.getSeg()) != NULL)
      {
        segVect[vSize].begin = (unsigned long)((p->begin()*1000.0)/sampleRate);
        segVect[vSize].end = (unsigned long)(((p->begin()+p->length()-1+1)*1000.0)/sampleRate);
        segVect[vSize].id = cl.getId();
        if (segVect[vSize].begin != segVect[vSize].end)
          vSize++;
      }
    }
    writeString("\n\t</Speakers>");
    writeString("\n\t<Episode>");

    if (vSize != 0)
    {
      qsort(segVect, vSize, sizeof(_type), compare);
      writeString("\n\t\t<Section");
      writeAttribute("type","report");
      writeTime("startTime", segVect[0].begin);

      for (long ii=vSize-1; ii>=0; ii--)
      {
        _type& seg = segVect[ii];
        if ((unsigned long)ii == vSize-1 || seg.id != id || seg.end < begin)
          endTurn = seg.end;
        seg.endTurn = endTurn;
        id = seg.id;
        begin = seg.begin;
      }
      writeTime("endTime", segVect[vSize-1].end);
      writeString(">");
      bool turnStarted = false;
      for (i=0; i<vSize; i++)
      {
        _type& seg = segVect[i];
        if (i == 0 || seg.id != id || seg.begin > end)
        {
          if (i != 0)
            writeString("\n\t\t\t</Turn>");
          turnStarted = true;
          writeString("\n\t\t\t<Turn");
          writeAttribute("speaker", "spk" + String::valueOf(seg.id));
          writeTime("startTime", seg.begin);
          writeTime("endTime", seg.endTurn);
          writeString(">");
        }
        writeString("\n\t\t\t\t<Sync");
        writeTime("time", seg.begin);
        writeString("/>");
        id = seg.id;
        end = seg.end;
      }
      if (turnStarted)
        writeString("\n\t\t\t</Turn>");
      writeString("\n\t\t</Section>");
    }
    delete [] segVect;
    writeString("\n\t</Episode>");
    writeString("\n</Trans>");
    close();
  }
  catch (Exception&)
  {
    delete [] segVect;
    throw; // do not use 'throw e'
  }
}
//-------------------------------------------------------------------------
void W::writeTime(const String& n, unsigned long t) // private
{ writeAttribute(n, String::valueOf(t/1000.0)); }
//-------------------------------------------------------------------------
String W::getClassName() const { return "SegServerFileWriter"; }
//-------------------------------------------------------------------------
W::~SegServerFileWriter() {}
//-------------------------------------------------------------------------
// static method
//-------------------------------------------------------------------------
int W::compare(const void *s1, const void *s2)

{
    if (((_type*)s1)->begin > ((_type*)s2)->begin)
        return 1;
    if (((_type*)s1)->begin < ((_type*)s2)->begin)
        return -1;
    return 0;
}

#endif // !defined(ALIZE_SegServerFileWriter_cpp)

