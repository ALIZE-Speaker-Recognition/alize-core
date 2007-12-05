/*
Alize is a free, open tool for speaker recognition

Alize is a development project initiated by the ELISA consortium
  [www.lia.univ-avignon.fr/heberges/ALIZE/ELISA] and funded by the
  French Research Ministry in the framework of the
  TECHNOLANGUE program [www.technolangue.net]
  [www.technolangue.net]

The Alize project team wants to highlight the limits of voice 
  authentication in a forensic context.
  The following paper proposes a good overview of this point:
  [Bonastre J.F., Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., 
  Magrin-chagnolleau I., Person  Authentification by Voice: A Need of 
  Caution, Eurospeech 2003, Genova]
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
  the use of Alize

Copyright (C) 2003-2005
  Laboratoire d'informatique d'Avignon [www.lia.univ-avignon.fr]
  Frederic Wils [frederic.wils@lia.univ-avignon.fr]
  Jean-Francois Bonastre [jean-francois.bonastre@lia.univ-avignon.fr]
          
This file is part of Alize.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if !defined(ALIZE_TestXListFileReader_cpp)
#define ALIZE_TestXListFileReader_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestXListFileReader.h"
#include "XListFileReader.h"
#include "alizeString.h"
#include "Config.h"
#include "AutoDestructor.h"
#include <cstdio>

using namespace alize;

//-------------------------------------------------------------------------
TestXListFileReader::TestXListFileReader()
:Test() {}
//-------------------------------------------------------------------------
bool TestXListFileReader::exec()
{
   Test::exec();

    {
        const String file("./TestXListFileReader.tmp");
        // test getClassName()
        Config c;
        test(XListFileReader(file, c).getClassName()
                                    == "XListFileReader", __LINE__);

        // test method : XListFileReader& create(const FileName& f,
        //                                               const Config& c);
        {
            XListFileReader& r = XListFileReader::create(file, c);
            AutoDestructor ad(r);
            test(r.getClassName() == "XListFileReader", __LINE__);
        }
        // test method : explicit XListFileReader(const FileName& f,
        //                                               const Config& c);
        {
            XListFileReader r1(file, c);
            // that's all !
        }

        // test method : virtual void readList(XList& l);
        {
            XList list;
            // empty file
            FILE* f = fopen(file.c_str(), "w");
            fclose(f);
            try
            {
                XListFileReader r(file, c);
                r.readList(list);
                test(list.getLineCount() == 0, __LINE__);
            }
            catch (Exception&) { showError(__LINE__); }
            // not empty file
            f = fopen(file.c_str(), "w");
            char s[] = "aa\n bb\t cc dd\n \n a b c d";
            fwrite(&s, sizeof(s)-1, 1, f);
            fclose(f);
            XListFileReader r(file, c);
            r.readList(list);
            test(list._current == 0, __LINE__);
            XList l;
            l.addLine().addElement("aa");
            XLine& line = l.addLine();
            line.addElement("bb");
            line.addElement("cc");
            line.addElement("dd");
            XLine& line2 = l.addLine();
            line2.addElement("a");
            line2.addElement("b");
            line2.addElement("c");
            line2.addElement("d");
            test(list == l, __LINE__);
            remove(file.c_str());
        }


    } // local objects are deleted here 

    return endTest();
}
//-------------------------------------------------------------------------
String TestXListFileReader::getClassName() const
{
    return "TestXListFileReader";
}
//-------------------------------------------------------------------------
TestXListFileReader::~TestXListFileReader() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestXListFileReader_cpp)
