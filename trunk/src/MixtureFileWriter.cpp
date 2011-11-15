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

#if !defined(ALIZE_MixtureFileWriter_cpp)
#define ALIZE_MixtureFileWriter_cpp

#include "MixtureFileWriter.h"
#include "MixtureGD.h"
#include "MixtureGF.h"
#include "DistribGD.h"
#include "DistribGF.h"
#include "Exception.h"
#include "Config.h"
#include <cmath>

using namespace alize;
typedef MixtureFileWriter W;

//-------------------------------------------------------------------------
W::MixtureFileWriter(const FileName& f, const Config& c)
:FileWriter(getFullFileName(c, f)), _config(c) {}
//-------------------------------------------------------------------------
String W::getFullFileName(const Config& c, const FileName& f) const
{   // protected
  if (f.beginsWith("/") || f.beginsWith("./"))
    return f;
  return c.getParam_mixtureFilesPath() + f
       + c.getParam_saveMixtureFileExtension();
}
//-------------------------------------------------------------------------
void W::writeMixture(const Mixture& m)
{
  MixtureFileWriterFormat format;
  if (!_config.existsParam_saveMixtureFileFormat
       && _fileName.endsWith(".xml"))
    format = MixtureFileWriterFormat_XML;
  else 
    format = _config.getParam_saveMixtureFileFormat();

  if (m.getClassName() == "MixtureGD")
  {
    switch (format)
    {
      case MixtureFileWriterFormat_XML:
        open(); //can throw IOException
        writeMixtureGD_XML((const MixtureGD&)m);
        close();
        return;
      case MixtureFileWriterFormat_RAW:
        open(); //can throw IOException
        writeMixtureGD_RAW((const MixtureGD&)m);
        close();
        return;
      case MixtureFileWriterFormat_ETAT:
        open(); //can throw IOException
        writeMixtureGD_ETAT((const MixtureGD&)m);
        close();
        return;
    }
  }
  else if (m.getClassName() == "MixtureGF")
  {
    switch (format)
    {
      case MixtureFileWriterFormat_XML:
        open(); //can throw IOException
        writeMixtureGF_XML((const MixtureGF&)m);
        close();
        return;
      case MixtureFileWriterFormat_RAW:
        open(); //can throw IOException
        writeMixtureGF_RAW((const MixtureGF&)m);
        close();
        return;
      case MixtureFileWriterFormat_ETAT:
        throw Exception("function not implemented", __FILE__, __LINE__);
    }
  }
  
  throw Exception("I don't know how to save a " + m.getClassName()
                    + " object", __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
void W::writeMixtureGD_XML(const MixtureGD& m)
{
  unsigned long i, c, vectSize = m.getVectSize();
  writeString("<MixtureGD");
  writeAttribute("version", "1");
  writeAttribute("id", m.getId());
  writeAttribute("distribCount", m.getDistribCount());
  writeAttribute("vectSize", vectSize);
  writeString(">");

  for (i=0; i< m.getDistribCount(); i++)
  {
    DistribGD& d = m.getDistrib(i);

    writeString("\n\t<DistribGD");
    writeAttribute("i", i);
    writeAttribute("weight", m.weight(i));
    writeAttribute("cst", d.getCst());
    writeAttribute("det", d.getDet());
    writeString(">");

    for (c=0; c<vectSize; c++)
    {
      writeString("\n\t\t<covInv");
      writeAttribute("i", c);
      writeString(">" + String::valueOf(d.getCovInv(c)) + "</covInv>");
    }

    for (c=0; c<vectSize; c++)
    {
      writeString("\n\t\t<mean");
      writeAttribute("i", c);
      writeString(">" + String::valueOf(d.getMean(c)) + "</mean>");
    }

    writeString("\n\t</DistribGD>");
  }
  writeString("\n</MixtureGD>");
}
//-------------------------------------------------------------------------
void W::writeMixtureGF_XML(const MixtureGF& m)
{
  unsigned long i, c, vectSize = m.getVectSize();
  writeString("<MixtureGF");
  writeAttribute("version", "1");
  writeAttribute("id", m.getId());
  writeAttribute("distribCount", m.getDistribCount());
  writeAttribute("vectSize", vectSize);
  writeString(">");

  for (i=0; i< m.getDistribCount(); i++)
  {
    DistribGF& d = m.getDistrib(i);

    writeString("\n\t<DistribGF");
    writeAttribute("i", i);
    writeAttribute("weight", m.weight(i));
    writeAttribute("cst", d.getCst());
    writeAttribute("det", d.getDet());
    writeString(">");

    for (c=0; c<vectSize; c++)
      for (unsigned long cc=0; cc<vectSize; cc++)
      {
        writeString("\n\t\t<covInv");
        writeAttribute("i", c);
        writeAttribute("j", cc);
        writeString(">" + String::valueOf(d.getCovInv(c, cc)) + "</covInv>");
      }

    for (c=0; c<vectSize; c++)
    {
      writeString("\n\t\t<mean");
      writeAttribute("i", c);
      writeString(">" + String::valueOf(d.getMean(c)) + "</mean>");
    }

    writeString("\n\t</DistribGF>");
  }
  writeString("\n</MixtureGF>");
}
//-------------------------------------------------------------------------
void W::writeMixtureGD_RAW(const MixtureGD& m)
{
  unsigned long v, c;
  writeUInt4(m.getDistribCount());
  writeUInt4(m.getVectSize());
  for (c=0; c<m.getDistribCount(); c++)
    writeDouble(m.weight(c));
  for (c=0; c<m.getDistribCount(); c++)
  {
    DistribGD& d = m.getDistrib(c);
    writeDouble(d.getCst());
    writeDouble(d.getDet());
    writeChar((char)0); // not used
    for (v=0; v<m.getVectSize(); v++)
      writeDouble(d.getCovInv(v));

    for (v=0; v<m.getVectSize(); v++)
      writeDouble(d.getMean(v));
  }
}
//-------------------------------------------------------------------------
void W::writeMixtureGD_ETAT(const MixtureGD& m)
{
  writeUInt4(3); // aux = LHM_STATE_FILE = 3
  unsigned long i,d, c;
  // tag = file name ?
  for (i=0; i<_fileName.length() && i<63; i++)
    writeString(_fileName[i]);
  for (; i<64; i++)
    writeChar('\0');
  writeUInt4(0); // num = unknown
  writeUInt4(0); // nbt = unknown
  writeDouble(0.0); // logVrais = unknown
  writeUInt4(0); // id = unknown
  writeUInt4(m.getDistribCount()); // nb_pdf
  for (d=0; d<m.getDistribCount(); d++)
    writeDouble(log(m.weight(d)));
  for (d=0; d<m.getDistribCount(); d++)
  {
    writeUInt4(d);
    writeShort(0); // type = LHM_GAUSS = 0
    writeShort((short)m.getVectSize()); // tpar
    DistribGD& dis = m.getDistrib(d);
    for (c=0; c<dis.getVectSize(); c++)
      writeDouble(dis.getMean(c));
    for (c=0; c<dis.getVectSize(); c++)
      writeDouble(dis.getCov(c));
    writeDouble(-2.0*log(dis.getCst()));
  }
}
//-------------------------------------------------------------------------
void W::writeMixtureGF_RAW(const MixtureGF& m)
{
  unsigned long v, vv, c, vectSize = m.getVectSize();
  writeUInt4(m.getDistribCount());
  writeUInt4(m.getVectSize());
  for (c=0; c<m.getDistribCount(); c++)
  { writeDouble(m.weight(c)); }
  for (c=0; c<m.getDistribCount(); c++)
  {
    DistribGF& d = m.getDistrib(c);
    writeDouble(d.getCst());
    writeDouble(d.getDet());
    writeChar(d.getCovMatrix().size()!=0?(char)1:(char)0);
    if (d.getCovMatrix().size()!=0)
      for (v=0; v<vectSize; v++)
        for (vv=0; vv<vectSize; vv++)
          writeDouble(d.getCov(v, vv));
    for (v=0; v<vectSize; v++)
      for (vv=0; vv<vectSize; vv++)
        writeDouble(d.getCovInv(v, vv));

    for (v=0; v<m.getVectSize(); v++)
      writeDouble(d.getMean(v));
  }
}
//-------------------------------------------------------------------------
String W::getClassName() const { return "MixtureFileWriter"; }
//-------------------------------------------------------------------------
W::~MixtureFileWriter() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureFileWriter_cpp)

