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

#if !defined(ALIZEExceptioncpp)
#define ALIZEExceptioncpp

#include "Exception.h"

using namespace alize;

//-------------------------------------------------------------------------
Exception::Exception(const String& msg, const String& sourceFile, int line)
:Object(), msg(msg), sourceFile(sourceFile), line(line) {}
//-------------------------------------------------------------------------
Exception::Exception(const Exception& e)
:Object(),msg(e.msg),sourceFile(e.sourceFile),line(e.line) {}
//-------------------------------------------------------------------------
String Exception::toString() const
{
  return Object::toString()
    + "\n  message   = \"" + msg + "\""
    + "\n  source file = " + sourceFile
    + "\n  line number = " + String::valueOf(line);
}
//-------------------------------------------------------------------------
String Exception::getClassName() const { return "Exception"; }
//-------------------------------------------------------------------------
Exception::~Exception() {}
//-------------------------------------------------------------------------



//-------------------------------------------------------------------------
IndexOutOfBoundsException::IndexOutOfBoundsException(const String& msg,
        const String& sourceFile, int line, long index, long limit)
:Exception(msg, sourceFile, line), index(index), limit(limit) {}
//-------------------------------------------------------------------------
IndexOutOfBoundsException::IndexOutOfBoundsException(
                const IndexOutOfBoundsException& e)
:Exception(e.msg, e.sourceFile, e.line),
 index(e.index), limit(e.limit) {}
//-------------------------------------------------------------------------
String IndexOutOfBoundsException::toString() const
{
  return Exception::toString()
    + "\n  index " + String::valueOf(index)
    + " >= limit " + String::valueOf(limit);
}
//-------------------------------------------------------------------------
String IndexOutOfBoundsException::getClassName() const
{ return "IndexOutOfBoundsException"; }
//-------------------------------------------------------------------------
IndexOutOfBoundsException::~IndexOutOfBoundsException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
IOException::IOException(const String& msg, const String& sourceFile,
                     int line, const FileName& f)
:Exception(msg, sourceFile, line), fileName(f) {}
//-------------------------------------------------------------------------
IOException::IOException(const IOException& e)
:Exception(e.msg, e.sourceFile, e.line) {}
//-------------------------------------------------------------------------
String IOException::toString() const
{ return Exception::toString() + "\n  fileName =  " + fileName; }
//-------------------------------------------------------------------------
String IOException::getClassName() const { return "IOException"; }
//-------------------------------------------------------------------------
IOException::~IOException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
IdAlreadyExistsException::IdAlreadyExistsException(const String& msg,
                    const String& sourceFile, int line)
:Exception(msg, sourceFile, line) {}
//-------------------------------------------------------------------------
IdAlreadyExistsException::IdAlreadyExistsException(
                  const IdAlreadyExistsException& e)
:Exception(e.msg, e.sourceFile, e.line) {}
//-------------------------------------------------------------------------
String IdAlreadyExistsException::getClassName() const
{ return "IdAlreadyExistsException"; }
//-------------------------------------------------------------------------
IdAlreadyExistsException::~IdAlreadyExistsException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
InvalidDataException::InvalidDataException(const String& msg,
        const String& sourceFile, int line, const FileName& f)
:IOException(msg, sourceFile, line, f) {}
//-------------------------------------------------------------------------
InvalidDataException::InvalidDataException(const InvalidDataException& e)
:IOException(e.msg, e.sourceFile, e.line, e.fileName) {}
//-------------------------------------------------------------------------
String InvalidDataException::getClassName() const
{ return "InvalidDataException"; }
//-------------------------------------------------------------------------
InvalidDataException::~InvalidDataException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
OutOfMemoryException::OutOfMemoryException(const String& msg,
                    const String& sourceFile, int line)
:Exception(msg, sourceFile, line) {}
//-------------------------------------------------------------------------
OutOfMemoryException::OutOfMemoryException(const OutOfMemoryException& e)
:Exception(e.msg, e.sourceFile, e.line) {}
//-------------------------------------------------------------------------
String OutOfMemoryException::getClassName() const
{ return "OutOfMemoryException"; }
//-------------------------------------------------------------------------
OutOfMemoryException::~OutOfMemoryException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
FileNotFoundException::FileNotFoundException(const String& msg,
        const String& sourceFile, int line, const FileName& f)
:IOException(msg, sourceFile, line, f) {}
//-------------------------------------------------------------------------
FileNotFoundException::FileNotFoundException(
                  const FileNotFoundException& e)
:IOException(e.msg, e.sourceFile, e.line, e.fileName) {}
//-------------------------------------------------------------------------
String FileNotFoundException::getClassName() const
{ return "FileNotFoundException"; }
//-------------------------------------------------------------------------
FileNotFoundException::~FileNotFoundException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
ParamNotFoundInConfigException::ParamNotFoundInConfigException(const String& msg,
        const String& sourceFile, int line)
:Exception(msg, sourceFile, line) {}
//-------------------------------------------------------------------------
ParamNotFoundInConfigException::ParamNotFoundInConfigException(
                  const ParamNotFoundInConfigException& e)
:Exception(e.msg, e.sourceFile, e.line) {}
//-------------------------------------------------------------------------
String ParamNotFoundInConfigException::getClassName() const
{ return "ParamNotFoundInConfigException"; }
//-------------------------------------------------------------------------
ParamNotFoundInConfigException::~ParamNotFoundInConfigException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
ConfigCheckException::ConfigCheckException(const String& msg,
        const String& sourceFile, int line)
:Exception(msg, sourceFile, line) {}
//-------------------------------------------------------------------------
ConfigCheckException::ConfigCheckException(
                  const ConfigCheckException& e)
:Exception(e.msg, e.sourceFile, e.line) {}
//-------------------------------------------------------------------------
String ConfigCheckException::getClassName() const
{ return "ConfigCheckException"; }
//-------------------------------------------------------------------------
ConfigCheckException::~ConfigCheckException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
EOFException::EOFException(const String& msg, const String& sourceFile,
                                               int line, const FileName& f)
:IOException(msg, sourceFile, line, f) {}
//-------------------------------------------------------------------------
EOFException::EOFException(const EOFException& e)
:IOException(e.msg, e.sourceFile, e.line, e.fileName) {}
//-------------------------------------------------------------------------
String EOFException::getClassName() const { return "EOFException"; }
//-------------------------------------------------------------------------
EOFException::~EOFException() {}
//-------------------------------------------------------------------------

/*void* operator new(sizet n)
{
  void* p = new (std::nothrow) (n);
  return p;
}
*/
#endif // !defined(ALIZEExceptioncpp)
