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

#if !defined(ALIZE_LabelFileReader_cpp)
#define ALIZE_LabelFileReader_cpp

#include <new>
#include "LabelFileReader.h"
#include "Exception.h"
#include "Config.h"
#include "LabelSet.h"

using namespace alize;
typedef LabelFileReader R;

//-------------------------------------------------------------------------
R::LabelFileReader(const FileName& f, const Config& c)
:Object(), _pReader(&FileReader::create(f, "", "", false)) {}
//-------------------------------------------------------------------------
R& R::create(const FileName& f, const Config& c)
{
  R* p = new (std::nothrow) R(f, c);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void R::readLabels(LabelSet& set)
{
  set.clear();
  Config c;
  assert(_pReader != NULL);
  XList list(_pReader->getFullFileName(), c);
  for (unsigned long i=0; i<list.getLineCount(); i++)
  {
    const XLine& line = list.getLine(i);
    const String& s0 = line.getElement(0);
    const String& s1 = line.getElement(1);
    double d0, d1;
    try { d0 = s0.toDouble(); d1 = s1.toDouble(); }
    catch (Exception&)
    { throw IOException("Wrong data in label file. Line "
                        + String::valueOf(i+1) + "", __FILE__,
                        __LINE__, _pReader->getFullFileName()); }
    const String& s2 = line.getElement(2);
    if (s2.isEmpty())
      throw IOException("Wrong data in label file. Line "
                        + String::valueOf(i+1) + "", __FILE__,
                        __LINE__, _pReader->getFullFileName());
    set.addLabel(d0, d1, line.getElement(2));
  }

  /* // périmé le 14 jan 2005
  try
  {
    while (true)
    {
      String s = readLine(); // can throw IOException
      set.addLabel(s.getToken(0).toDouble(),
                   s.getToken(1).toDouble(),
                   s.getToken(2));
    }
  }
  catch (EOFException&) {}
  close();*/
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "LabelFileReader"; }
//-------------------------------------------------------------------------
R::~LabelFileReader()
{
  if (_pReader != NULL)
    delete _pReader;
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_LabelFileReader_cpp)

