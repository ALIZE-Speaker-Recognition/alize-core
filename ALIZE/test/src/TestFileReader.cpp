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

#if !defined(ALIZE_TestFileReader_cpp)
#define ALIZE_TestFileReader_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestFileReader.h"
#include "FileReader.h"
#include "alizeString.h"
#include "RealVector.h"
#include "Exception.h"
#include <cstdio>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestFileReader::TestFileReader()
:Test() {}
//-------------------------------------------------------------------------
bool TestFileReader::exec()
{
  Test::exec();
  {
    // test method : String getClassName() const
    {
      test(FileReader("hjhjj", "", "", false).getClassName()
            == "FileReader", __LINE__);
    }

    // Test method : unsigned long readSomeFloats(FloatVector& v)
    {
      const String file("./TestFileReader");
      FILE* f = fopen(file.c_str(), "w");
      float v1 = 1.2f;
      float v2 = 3.4f;
      float v3 = 5.6f;
      float v4 = 7.8f;
      float v5 = 9.4f;
      float v6 = 3.1f;
      fwrite(&v1, sizeof(v1), 1, f);
      fwrite(&v2, sizeof(v2), 1, f);
      fwrite(&v3, sizeof(v2), 1, f);
      fwrite(&v4, sizeof(v2), 1, f);
      fwrite(&v5, sizeof(v2), 1, f);
      fwrite(&v6, sizeof(v2), 1, f);
      fclose(f);

      FileReader r(file, "", "", false);
      try {
        FloatVector v(2,2);
        test(r.readSomeFloats(v) == 2, __LINE__);
        test(v[0] == v1, __LINE__);
        test(v[1] == v2, __LINE__);
        test(r.readSomeFloats(v) == 2, __LINE__);
        test(v[0] == v3, __LINE__);
        test(v[1] == v4, __LINE__);
        r.reset();
        FloatVector vv1(10,10), vv2(6,6);
        test(r.readSomeFloats(vv1) == 6, __LINE__);
        r.reset();
        test(r.readSomeFloats(vv2) == 6, __LINE__);
        r.reset();
      }
      catch (Exception&) { showError(__LINE__); }
      remove(file.c_str());

      f = fopen(file.c_str(), "w");
      v1 = 1.2f;
      char  x = '0';
      // file content is not a multiple of sizeof(float)
      fwrite(&v1, sizeof(v1), 1, f);
      fwrite(&x, sizeof(x), 1, f);
      fclose(f);

      FileReader rr(file, "", "", false);
      FloatVector vv3(2,2);
      test(rr.readSomeFloats(vv3) == 1, __LINE__);
      remove(file.c_str());
    }

  } // local objects are deleted here 

  return endTest();
}
//-------------------------------------------------------------------------
String TestFileReader::getClassName() const
{ return "TestFileReader"; }
//-------------------------------------------------------------------------
TestFileReader::~TestFileReader() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFileReader_cpp)
