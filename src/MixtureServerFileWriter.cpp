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

#if !defined(ALIZE_MixtureServerFileWriter_cpp)
#define ALIZE_MixtureServerFileWriter_cpp

#include "MixtureServerFileWriter.h"
#include "MixtureGD.h"
#include "DistribGD.h"
#include "Exception.h"
#include "MixtureServer.h"
#include "Config.h"

using namespace alize;
typedef MixtureServerFileWriter W;

//-------------------------------------------------------------------------
W::MixtureServerFileWriter(const FileName& f, const Config& c)
:FileWriter(getFullFileName(c, f))
{
  if (_fileName.endsWith(".xml"))
    _format = MixtureServerFileWriterFormat_XML;
  else
  {
    if (c.getParam_saveMixtureServerFileFormat() == MixtureServerFileWriterFormat_RAW)
      _format = MixtureServerFileWriterFormat_RAW;
    else if (c.getParam_saveMixtureServerFileFormat() == MixtureServerFileWriterFormat_XML)
      _format = MixtureServerFileWriterFormat_XML; // TODO : gerer des param dans la config
  }
}
//-------------------------------------------------------------------------
String W::getFullFileName(const Config& c, const FileName& f) const
{  // protected
  if (f.beginsWith("/") || f.beginsWith("./"))
    return f;
  return c.getParam_mixtureFilesPath() + f
       + c.getParam_saveMixtureFileExtension();
}
//-------------------------------------------------------------------------
void W::writeMixtureServer(const MixtureServer& ms)
{
  open(); //can throw IOException
  if (_format == MixtureServerFileWriterFormat_XML)
    writeMixtureServerXml(ms);
  else
    writeMixtureServerRaw(ms);
  close();
}
//-------------------------------------------------------------------------
void W::writeMixtureServerXml(const MixtureServer& ms)
{
  unsigned long i, c;
  writeString("<MixtureServer");
  writeAttribute("name", ms.getServerName());
  try { writeAttribute("vectSize", ms.getVectSize()); } catch (Exception&){}
  writeAttribute("mixtureCount", ms.getMixtureCount());
  writeAttribute("distribCount", ms.getDistribCount());
  writeString(">");
  //writeString("\n\t<DistribDict>");
  for (i=0; i<ms.getDistribCount(); i++)
  {
    const DistribGD* p = dynamic_cast<const DistribGD*>(&ms.getDistrib(i));
    if (p != NULL)
    {
      writeString("\n\t\t<DistribGD");
      writeAttribute("i", i);
      writeString(">");
      for (c=0; c<p->getVectSize(); c++)
      {
          writeString("\n\t\t\t<covInv");
          writeAttribute("i", c);
          writeString(">"
            + String::valueOf(p->getCovInv(c)) + "</covInv>");
      }
      for (c=0; c<p->getVectSize(); c++)
      {
          writeString("\n\t\t\t<mean");
          writeAttribute("i", c);
          writeString(">"
            + String::valueOf(p->getMean(c)) + "</mean>");
      }
      writeString("\n\t\t</DistribGD>");
    }
    else
      throw Exception("I don't know how to save a "
               + ms.getDistrib(i).getClassName()
               + " object", __FILE__, __LINE__);
  }
  //writeString("\n\t</DistribDict>");
  for (i=0; i<ms.getMixtureCount(); i++)
  {
    const MixtureGD* p = dynamic_cast<const MixtureGD*>(&ms.getMixture(i));
    if (p != NULL)
      writeMixtureGDXml(*p);
    else
      throw Exception("I don't know how to save a "
               + ms.getDistrib(i).getClassName()
               + " object", __FILE__, __LINE__);
  }

  writeString("\n</MixtureServer>");
}
//-------------------------------------------------------------------------
void W::writeMixtureGDXml(const MixtureGD& m)
{
  unsigned long i;
  writeString("\n\t<MixtureGD");
  writeAttribute("id", m.getId());
  writeAttribute("distribCount", m.getDistribCount());
  writeString(">");

  for (i=0; i< m.getDistribCount(); i++)
  {
    DistribGD& d = m.getDistrib(i);
    writeString("\n\t\t<DistribGD");
    writeAttribute("i", i);
    writeAttribute("dictIdx", d.dictIndex(K::k));
    writeAttribute("weight", m.weight(i));
    writeString("/>");
  }
  writeString("\n\t</MixtureGD>");
}
//-------------------------------------------------------------------------
void W::writeMixtureServerRaw(const MixtureServer& ms)
{
  unsigned long i, c;
  writeString("MixtureServer");
  writeUInt4(ms.getServerName().length());
  writeString(ms.getServerName());
  try
  {
    writeUInt4(ms.getVectSize());
  }
  catch (Exception&)
  {
    writeUInt4(0);
  }
  writeUInt4(ms.getMixtureCount());
  writeUInt4(ms.getDistribCount());
  for (i=0; i<ms.getDistribCount(); i++)
  {
    const DistribGD* p = dynamic_cast<const DistribGD*>(&ms.getDistrib(i));
    if (p != NULL)
    {
      writeString("GD");
      writeString("f"); // unused
      for (c=0; c<p->getVectSize(); c++)
       writeDouble(p->getCovInv(c));

      for (c=0; c<p->getVectSize(); c++)
        writeDouble(p->getMean(c));
    }
    else
      throw Exception("I don't know how to save a "
               + ms.getDistrib(i).getClassName()
               + " object", __FILE__, __LINE__);
  }
  for (i=0; i<ms.getMixtureCount(); i++)
  {
    const MixtureGD* p = dynamic_cast<const MixtureGD*>(&ms.getMixture(i));
    if (p != NULL)
      writeMixtureGDRaw(*p);
    else
      throw Exception("I don't know how to save a "
               + ms.getDistrib(i).getClassName()
               + " object", __FILE__, __LINE__);
  }
}
//-------------------------------------------------------------------------
void W::writeMixtureGDRaw(const MixtureGD& m)
{
  unsigned long i;
  writeString("GD");
  writeUInt4(m.getId().length());
  writeString(m.getId());
  writeUInt4(m.getDistribCount());

  for (i=0; i< m.getDistribCount(); i++)
  {
    DistribGD& d = m.getDistrib(i);

    writeUInt4(d.dictIndex(K::k));
    writeDouble(m.weight(i));
  }
}
//-------------------------------------------------------------------------
String W::getClassName() const { return "MixtureServerFileWriter"; }
//-------------------------------------------------------------------------
W::~MixtureServerFileWriter() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureServerFileWriter_cpp)

