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

#if !defined(ALIZE_TestException_cpp)
#define ALIZE_TestException_cpp

#include "TestException.h"
#include "Exception.h"
//#include <iostream>

using namespace alize;

//-------------------------------------------------------------------------
TestException::TestException()
:Test() {}
//-------------------------------------------------------------------------
bool TestException::exec()
{
    Test::exec();

    {
        { // test Exception

            Exception e("azerty", "sourceFileName", 123);
            test(e.getClassName() == "Exception", __LINE__);

            // test constructor
            
            test(e.msg == "azerty", __LINE__);
            test(e.sourceFile == "sourceFileName", __LINE__);
            test(e.line == 123, __LINE__);
            
            // test copy constructor

            Exception e1(e);
            test(e1.msg == e.msg, __LINE__);
            test(e1.sourceFile == e.sourceFile, __LINE__);
            test(e1.line == e.line, __LINE__);
        }

        { // test IndexOutOfBoundsException

            IndexOutOfBoundsException e("azerty", "sourceFileName", 123, 1, 2);
            test(e.getClassName() == "IndexOutOfBoundsException", __LINE__);
            test(e.index == 1, __LINE__);
            test(e.limit == 2, __LINE__);
            //std::cout << e.toString() << std::endl;
            //test(e.toString() == "

            // test constructor

            test(e.msg == "azerty", __LINE__);
            test(e.sourceFile == "sourceFileName", __LINE__);
            test(e.line == 123, __LINE__);
            
            // test copy constructor

            IndexOutOfBoundsException e1(e);
            test(e1.msg == e.msg, __LINE__);
            test(e1.sourceFile == e.sourceFile, __LINE__);
            test(e1.line == e.line, __LINE__);
        }
        
        { // test IOException

            IOException e("azerty", "sourceFileName", 123, "fileName");
            test(e.getClassName() == "IOException", __LINE__);
            //std::cout << e.toString() << std::endl;

            // test constructor

            test(e.msg == "azerty", __LINE__);
            test(e.sourceFile == "sourceFileName", __LINE__);
            test(e.line == 123, __LINE__);
            test(e.fileName == "fileName", __LINE__);
            
            // test copy constructor

            IOException e1(e);
            test(e1.msg == e.msg, __LINE__);
            test(e1.sourceFile == e.sourceFile, __LINE__);
            test(e1.line == e.line, __LINE__);
        }

        { // test IdAlreadyExistsException

            IdAlreadyExistsException e("azerty", "sourceFileName", 123);
            test(e.getClassName() == "IdAlreadyExistsException", __LINE__);

            // test constructor

            test(e.msg == "azerty", __LINE__);
            test(e.sourceFile == "sourceFileName", __LINE__);
            test(e.line == 123, __LINE__);
            
            // test copy constructor

            IdAlreadyExistsException e1(e);
            test(e1.msg == e.msg, __LINE__);
            test(e1.sourceFile == e.sourceFile, __LINE__);
            test(e1.line == e.line, __LINE__);
        }

        { // test InvalidDataException

            InvalidDataException e("azerty", "sourceFileName", 123, "fileName");
            test(e.getClassName() == "InvalidDataException", __LINE__);

            // test constructor

            test(e.msg == "azerty", __LINE__);
            test(e.sourceFile == "sourceFileName", __LINE__);
            test(e.line == 123, __LINE__);
            test(e.fileName == "fileName", __LINE__);
            
            // test copy constructor

            InvalidDataException e1(e);
            test(e1.msg == e.msg, __LINE__);
            test(e1.sourceFile == e.sourceFile, __LINE__);
            test(e1.line == e.line, __LINE__);
        }

        { // test FileNotFoundException

            FileNotFoundException e("azerty", "sourceFileName", 123, "fileName");
            test(e.getClassName() == "FileNotFoundException", __LINE__);

            // test constructor

            test(e.msg == "azerty", __LINE__);
            test(e.sourceFile == "sourceFileName", __LINE__);
            test(e.line == 123, __LINE__);
            test(e.fileName == "fileName", __LINE__);
            
            // test copy constructor

            FileNotFoundException e1(e);
            test(e1.msg == e.msg, __LINE__);
            test(e1.sourceFile == e.sourceFile, __LINE__);
            test(e1.line == e.line, __LINE__);
        }

        { // test EOFException

            EOFException e("azerty", "sourceFileName", 123, "fileName");
            test(e.getClassName() == "EOFException", __LINE__);

            // test constructor

            test(e.msg == "azerty", __LINE__);
            test(e.sourceFile == "sourceFileName", __LINE__);
            test(e.line == 123, __LINE__);
            test(e.fileName == "fileName", __LINE__);
            
            // test copy constructor

            EOFException e1(e);
            test(e1.msg == e.msg, __LINE__);
            test(e1.sourceFile == e.sourceFile, __LINE__);
            test(e1.line == e.line, __LINE__);
        }

        { // test OutOfMemoryException

            OutOfMemoryException e("azerty", "sourceFileName", 123);
            test(e.getClassName() == "OutOfMemoryException", __LINE__);

            // test constructor

            test(e.msg == "azerty", __LINE__);
            test(e.sourceFile == "sourceFileName", __LINE__);
            test(e.line == 123, __LINE__);
            
            // test copy constructor

            OutOfMemoryException e1(e);
            test(e1.msg == e.msg, __LINE__);
            test(e1.sourceFile == e.sourceFile, __LINE__);
            test(e1.line == e.line, __LINE__);
        }

        { // test ParamNotFoundInConfigException

            ParamNotFoundInConfigException e("azerty", "sourceFileName", 123);
            test(e.getClassName() == "ParamNotFoundInConfigException", __LINE__);

            // test constructor

            test(e.msg == "azerty", __LINE__);
            test(e.sourceFile == "sourceFileName", __LINE__);
            test(e.line == 123, __LINE__);
            
            // test copy constructor

            ParamNotFoundInConfigException e1(e);
            test(e1.msg == e.msg, __LINE__);
            test(e1.sourceFile == e.sourceFile, __LINE__);
            test(e1.line == e.line, __LINE__);
        }

        { // test ConfigCheckException

            ConfigCheckException e("azerty", "sourceFileName", 123);
            test(e.getClassName() == "ConfigCheckException", __LINE__);

            // test constructor

            test(e.msg == "azerty", __LINE__);
            test(e.sourceFile == "sourceFileName", __LINE__);
            test(e.line == 123, __LINE__);
            
            // test copy constructor

            ConfigCheckException e1(e);
            test(e1.msg == e.msg, __LINE__);
            test(e1.sourceFile == e.sourceFile, __LINE__);
            test(e1.line == e.line, __LINE__);
        }

    } // local objects are deleted here

    return endTest();
}
//-------------------------------------------------------------------------
String TestException::getClassName() const { return "TestException"; }
//-------------------------------------------------------------------------
TestException::~TestException() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestException_cpp)
