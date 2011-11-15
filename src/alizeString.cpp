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

#if !defined(ALIZE_String_cpp)
#define ALIZE_String_cpp

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <new>
#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "alizeString.h"
#include "Exception.h"

using namespace alize;
typedef String S;

//-------------------------------------------------------------------------
S::String(const char* c)
:Object()
{
  unsigned long length = (unsigned long)strlen(c);
  create(length, length+length+1, c);
}
//-------------------------------------------------------------------------
S::String(const String& s)
:Object()
{
  create(s._length, s._length+s._length+1, s._string);
}
//-------------------------------------------------------------------------
S& S::duplicate() const
{
  S* p = new (std::nothrow) S(*this);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
S S::valueOf(unsigned long v)
{
  std::ostringstream o;
  o << v << std::ends;
  return o.str().c_str();
}
//-------------------------------------------------------------------------
S S::valueOf(long v)
{
  std::ostringstream o;
  o << v << std::ends;
  return o.str().c_str();
}
//-------------------------------------------------------------------------
S S::valueOf(double v)
{
  std::ostringstream o;
  o << std::setprecision(19) << v << std::ends;
  return o.str().c_str();
}
//-------------------------------------------------------------------------
S S::valueOf(unsigned int v)
{
  std::ostringstream o;
  o << v << std::ends;
  return o.str().c_str();
}
//-------------------------------------------------------------------------
S S::valueOf(int v)
{
  std::ostringstream o;
  o << v << std::ends;
  return o.str().c_str();
}
//-------------------------------------------------------------------------
S S::valueOf(bool value) { return value?"true":"false"; }
//-------------------------------------------------------------------------
double S::toDouble() const
{
  // return atof(_string);
  double v;
  std::istringstream stream(_string);
  stream >> v;
  if (stream.fail())
    throw Exception("cannot convert '" + *this
                    + "' to double float", __FILE__, __LINE__);
  return v;
}
//-------------------------------------------------------------------------
bool S::toBool() const
{
  if (strcmp(_string, "true") == 0)
    return true;
  if (strcmp(_string, "false") == 0)
    return false;
  throw Exception("cannot convert '" + *this
                    + "' to boolean", __FILE__, __LINE__);
  return true;
}
//-------------------------------------------------------------------------
long S::toLong() const
{
  long v;
  std::istringstream stream(_string);
  stream >> v;
  if (stream.fail())
    throw Exception("cannot convert '" + *this
                    + "' to long integer", __FILE__, __LINE__);
  return v;
}
//-------------------------------------------------------------------------
unsigned long S::toULong() const
{
  unsigned long v;
  std::istringstream stream(_string);
  stream >> v;
  if (stream.fail())
    throw Exception("cannot convert '" + *this
                    + "' to unsigned long integer", __FILE__, __LINE__);
  return v;
}
//-------------------------------------------------------------------------
const S& S::operator=(const String& s)
{
  if (_string != s._string)
  {
    if (_capacity < s._length+1)
    {
      delete[] _string;
      create(s._length, s._length+s._length+1, s._string);
    }
    else
    {
      _length = s._length;
      strcpy(_string, s._string);
    }
  }
  assert(_length == s._length);
  return *this;
}
//-------------------------------------------------------------------------
const S& S::operator=(const char* s)
{
  unsigned long len = (unsigned long)strlen(s);
  if (_string != s)
  {
    if (_capacity < len+1)
    {
      delete[] _string;
      create(len, len+len+1, s);
    }
    else
    {
      _length = len;
      strcpy(_string, s);
    }
  }
  assert(_length == len);
  return *this;
}
//-------------------------------------------------------------------------
S& S::operator+=(const String& s)
{
  char* oldString = _string;
  unsigned long newLength = _length+s._length;
  if (_capacity < newLength+1)
  {
    create(newLength, newLength+newLength+1, oldString);
    delete [] oldString;
  }
  strcat(_string, s._string);
  _length = newLength;
  return *this;
}
//-------------------------------------------------------------------------
S& S::operator+=(const char* s)
{
  char* oldString = _string;
  unsigned long newLength = _length+strlen(s);
  if (_capacity < newLength+1)
  {
    create(newLength, newLength+newLength+1, oldString);
    delete [] oldString;
  }
  strcat(_string, s);
  _length = newLength;
  return *this;
}
//-------------------------------------------------------------------------
S S::operator+(const String& s) const
{
  String x(*this);
  x += s;
  return x;
}
//-------------------------------------------------------------------------
S S::operator+(const char* s) const
{
  String x(*this);
  x += s;
  return x;
}
//-------------------------------------------------------------------------
bool S::operator==(const String& s) const
{
  return strcmp(_string, s._string) == 0;
}
//-------------------------------------------------------------------------
bool S::operator!=(const String& s) const
{
  return strcmp(_string, s._string) != 0;
}
//-------------------------------------------------------------------------
bool S::operator==(const char* s) const
{
  return strcmp(_string, s) == 0;
}
//-------------------------------------------------------------------------
bool S::operator!=(const char* s) const
{
  return strcmp(_string, s) != 0;
}
//-------------------------------------------------------------------------
bool S::operator<(const String& s) const
{
  return strcmp(_string, s._string) < 0;
}
//-------------------------------------------------------------------------
bool S::operator<=(const String& s) const
{
  return strcmp(_string, s._string) <= 0;
}
//-------------------------------------------------------------------------
bool S::operator>(const String& s) const
{
  return strcmp(_string, s._string) > 0;
}
//-------------------------------------------------------------------------
bool S::operator>=(const String& s) const
{
  return strcmp(_string, s._string) >= 0;
}
//-------------------------------------------------------------------------
unsigned long String::length() const { return _length; }
//-------------------------------------------------------------------------
S S::operator[](unsigned long index) const
{
  if (index >= _length)
    return "";
  char oneChar[] = {0, 0};
  oneChar[0] = _string[index];
  return oneChar;
}
//-------------------------------------------------------------------------
void S::reset() { _length = 0; _string[0] = 0; }
//-------------------------------------------------------------------------
bool S::isEmpty() const { return _length == 0; }
//-------------------------------------------------------------------------
bool S::endsWith(const String& s) const
                     
{
  if (s._length == 0)
    return true;
  if (_length < s._length)
    return false;
  return strncmp(_string+(_length - s._length), s._string, s._length) == 0;
  
}
//-------------------------------------------------------------------------
bool S::beginsWith(const String& s) const
                     
{
  if (s._length == 0)
    return true;
  if (_length < s._length)
    return false;
  return strncmp(_string, s._string, s._length) == 0;
  
}
//-------------------------------------------------------------------------
long S::find(const String& s, unsigned long start) const
                     
{
  if (s._length == 0)
    return -1;
  unsigned long i;
  for (; start+s._length<=_length; start++)
  {
    for (i=0; i<s._length; i++)
    {
      if (_string[start+i] != s._string[i])
        break;
    }
    if (i == s._length)
      return start;
  }
  return -1;
}
//-------------------------------------------------------------------------
S S::getToken(unsigned long index, const String& separator) const
{
  if (separator.length() != 1)
    throw Exception("separator length must be 1", __FILE__, __LINE__);
  char sep = *separator.c_str();

  S s;
  long tokenCount = 0;
  long start = -1;
  long end = -1;
  long i;
  char previous = sep;
  long l = length();
  for (i=0; i<l; i++)
  {
    if ( (sep == ' ' && _string[i] != ' ' && _string[i] != '\t') ||
         (sep != ' ' && _string[i] != sep) )
    {
      if ( (sep == ' ' && (previous == ' ' || previous == '\t')) ||
           (sep != ' ' && previous == sep) )
      {
        tokenCount++;
        start = i;
        previous = '\1';
      }
    }
    else
    {
      if ( (sep == ' ' && previous != ' ' && previous != '\t') ||
           (sep != ' ' && previous != sep) )
      {
        end = i-1;
        previous = sep;
        if (tokenCount == (long)index+1)
          break;
        start = -1;
        end = -1;
      }
    }
  }
  if ( (sep == ' ' && previous != ' ' && previous != '\t') ||
    (sep != ' ' && previous != sep) )
    end = i-1;
  if (tokenCount != (long)index+1)
  {
    start = -1;
    end = -1;
  }
  
  if (start != -1)
  {
    char* string = new (std::nothrow) char[end-start+2];
    assertMemoryIsAllocated(string, __FILE__, __LINE__);
    for (i=start; i<=end; i++)
    { string[i-start]=_string[i]; }
    string[end-start+1] = 0;
    s = string;
    delete [] string;
  }
  return s;
}
//-------------------------------------------------------------------------
const char* String::c_str() const { return _string; }
//-------------------------------------------------------------------------
S S::getClassName() const { return "String"; }
//-------------------------------------------------------------------------
S S::toString() const
{
  return Object::toString() + "  '" + _string;
}
//-------------------------------------------------------------------------
void S::create(unsigned long length, unsigned long capacity,
                              const char* c)
{
  _length = length;
  _capacity = capacity<length+1?length+1:capacity;
  _string = new (std::nothrow) char[_capacity];
  assertMemoryIsAllocated(_string, __FILE__, __LINE__);
  strcpy(_string, c);
}
//-------------------------------------------------------------------------
S::~String()
{
  assert(_string != NULL);
  delete[] _string;
}
//-------------------------------------------------------------------------

// global functions

//-------------------------------------------------------------------------
ALIZE_API S operator+(const char* c, const String& s)
{
  String x(c);
  x += s;
  return x;
}
//-------------------------------------------------------------------------
ALIZE_API std::ostream& operator<<(std::ostream& o, const S& s)
{
  return o << s.c_str();
}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_String_cpp)
