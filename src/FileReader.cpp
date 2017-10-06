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

#if !defined(ALIZE_FileReader_cpp)
#define ALIZE_FileReader_cpp

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <new>
#include "FileReader.h"
#include "Exception.h"
#include "RealVector.h"
//#include <iostream>
//using namespace std;
using namespace alize;
typedef FileReader R;

//-------------------------------------------------------------------------
R::FileReader(const FileName& f, const String& path,
              const String& extension, bool swap)
:Object(), _fullFileName(path + f + extension), _pFileStruct(NULL),
 _fileName(f), _path(path), _extension(extension), 
 _fileLengthDefined(false), _swap(swap) {}
//-------------------------------------------------------------------------
R& R::create(const FileName& f, const String& path, const String& ext,
             bool swap)
{
  R* p = new (std::nothrow) R(f, path, ext, swap);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
bool R::isClosed() const { return _pFileStruct == NULL; }
//-------------------------------------------------------------------------
bool R::isOpen() const { return _pFileStruct != NULL; }
//-------------------------------------------------------------------------
void R::reset()
{
  if (isOpen())
    seek(0);
}
//-------------------------------------------------------------------------
void R::open()
{
  close();
  _pFileStruct = ::fopen(_fullFileName.c_str(),"rb");
  if (_pFileStruct == NULL)
    throw FileNotFoundException("", __FILE__, __LINE__, _fullFileName);
}
//-------------------------------------------------------------------------
void R::close()
{
  if (isOpen())
    if (::fclose(_pFileStruct) == EOF)
      throw IOException("Cannot close file", __FILE__, __LINE__,
                 _fullFileName);
  _pFileStruct = NULL;
}
//-------------------------------------------------------------------------
const FileName& R::getFullFileName() const { return _fullFileName; }
//-------------------------------------------------------------------------
const FileName& R::getFileName() const { return _fileName; }
//-------------------------------------------------------------------------
unsigned long R::getFileLength()
{
  if (!_fileLengthDefined)
  {
    bool wasOpened = isOpen();
    long pos;
    long l;

    if (wasOpened)
    {
      if ( (pos = ::ftell(_pFileStruct)) < 0)
        throw IOException("ftell", __FILE__, __LINE__, _fullFileName);
    }
    else
    {
      open();
      pos = 0;
    }
    if (::fseek(_pFileStruct, 0, SEEK_END) != 0)
      throw IOException("fseek", __FILE__, __LINE__, _fullFileName);
    if ( (l = ::ftell(_pFileStruct)) < 0)
      throw IOException("ftell", __FILE__, __LINE__, _fullFileName);
    _fileLengthDefined = true;
    _fileLength = l;
    if (::fseek(_pFileStruct, pos, SEEK_SET) != 0)
      throw IOException("fseek", __FILE__, __LINE__, _fullFileName);
  }
  return _fileLength;
}
//-------------------------------------------------------------------------
void R::seek(unsigned long pos) // protected
{
  if (!isOpen())
    open();
  if (::fseek(_pFileStruct, pos, SEEK_SET) != 0 )
    throw IOException("seek out of bounds",
          __FILE__, __LINE__, _fullFileName);
}
//-------------------------------------------------------------------------
void R::read(void* buffer, unsigned long length) // private
{
  assert(buffer != NULL); // TODO : if public method, throw an Exception ?
  if (isClosed())
    open(); // can throw Exception if file name = ""
  if (::fread(buffer, 1, length, _pFileStruct) == length)
    return;

  // if end of file
  if (feof(_pFileStruct))
    throw EOFException("", __FILE__, __LINE__, _fullFileName);

  // other cases
  throw IOException("Cannot read file", __FILE__, __LINE__, _fullFileName);
}
//-------------------------------------------------------------------------
char R::readChar()
{
  char byte;
  read(&byte, 1); // can throw IOException, EOFException
  return byte;
}
//-------------------------------------------------------------------------
int R::readInt2()
{
  if (sizeof(short) == 2)
  {
    short s;
    read(&s, 2); // can throw IOException, EOFException
    if (_swap)
      swap2Bytes(&s, &s);
    return s;
  }
  else
    return 0; // pas terrible comme sortie...
}
//-------------------------------------------------------------------------
long R::readInt4()
{
  // Modified june 4th 2004 for IA 64 sizeof(long) = 8

  if (sizeof(long) == 4)
  {
    long s;
    read(&s, 4); // can throw IOException, EOFException
    if (_swap)
      swap4Bytes(&s);
    return s;
  }
  else if (sizeof(int) == 4)
  {
    int s;
    read(&s, 4); // can throw IOException, EOFException
    if (_swap)
      swap4Bytes(&s);
    return (long)s;
  }
  else
    return 0; // TODO : sortie a ameliorer
}
//-------------------------------------------------------------------------
unsigned long R::readUInt4()
{
  // Modified june 4th 2004 for IA 64 sizeof(unsigned long) = 8

  if (sizeof(unsigned long) == 4)
  {
    unsigned long s;
    read(&s, 4); // can throw IOException, EOFException
    if (_swap)
      swap4Bytes(&s);
    return s;
  }
  if (sizeof(unsigned int) == 4)
  {
    unsigned int s;
    read(&s, 4); // can throw IOException, EOFException
    if (_swap)
      swap4Bytes(&s);
    return (unsigned long)s;
  }
  return 0;
}
//-------------------------------------------------------------------------
double R::readDouble()
{
  double s;
  read(&s, 8); // can throw IOException, EOFException
  if (_swap)
    swap8Bytes(&s, &s);
  return s;
}
//-------------------------------------------------------------------------
FloatVector& R::readFloats(FloatVector& v)
{
  if (_swap)
  {
    float* array = v.getArray();
    unsigned long n = v.size();
    read(array, n*4);
    for (unsigned long i=0; i<n; i++)
      swap4Bytes(&array[i]);
  }
  else
    read(v.getArray(), v.size()*4);
  return v;
}
//-------------------------------------------------------------------------
unsigned long R::readSomeFloats(FloatVector& v)
{
  //static unsigned long f = 0;
  //f++;
  //if (f%10 == 0)
  //  cout << f << endl;
  if (isClosed())
    open(); // can throw Exception if file name = ""
  float* array = v.getArray();
  unsigned long n = (unsigned long)(::fread(array, 4, v.size(), _pFileStruct));
  if (_swap)
  {
    char* p = (char*)array;
    char t;
    for (unsigned long i=0; i<n; i++)
    {
      t = p[3]; p[3] = p[0]; p[0] = t;
      t = p[2]; p[2] = p[1]; p[1] = t;
      p += 4;
    }
  }
  return n;
}
//-------------------------------------------------------------------------
float R::readFloat()
{
  float s;
  read(&s, 4); // can throw IOException, EOFException
  if (_swap)
    swap4Bytes(&s);
  return s;
}
//-------------------------------------------------------------------------
const String& R::readString(unsigned long length)
{
  _string.reset();
  if (length != 0)
  {
    char* str = new (std::nothrow) char[length+1];
    assertMemoryIsAllocated(str, __FILE__, __LINE__);
    try { read(str, length); } // can throw IOException, EOFException
    catch (Exception&)
    {
      delete [] str;
      throw; // do not use 'throw e'
    }
    str[length] = 0;
    _string = str;
    delete [] str;
  }
  return _string;
}
//-------------------------------------------------------------------------
const String& R::readLine()
{

  char t[2];
  t[1] = 0;
  _string.reset();

  while (true)
  {
    try { read(&t, 1); } // can throw IOException, EOFException
    catch (EOFException&)
    {
      if (_string.length() != 0)
        return _string;
      throw; // do not use 'throw e'
    }

    if (t[0] == '\n')
      return _string;
    if (t[0] == '\r')
    {
      try { read(&t, 1); } // can throw IOException, EOFException
      catch (EOFException&)
      {
        if (_string.length() != 0)
          return _string;
        throw; // do not use 'throw e'
      }
      if (t[0] == '\n')
        return _string;
    }
    _string += t;
  }
}
//-------------------------------------------------------------------------
void R::rewind() {seek(0);}
//-------------------------------------------------------------------------
long R::tell()
{
  if (isClosed())
    open();
  return ::ftell(_pFileStruct);
}
//-------------------------------------------------------------------------
bool& R::swap() { return _swap; }
//-------------------------------------------------------------------------
void R::swap2Bytes(void *src, void *dest)
{
  char *p1,*p2;
  char tmp;
 
  p1 = (char*)src;
  p2 = (char*)dest;
  tmp = p1[1]; /* pour pouvoir utiliser src == dest */
  p2[1] = p1[0];
  p2[0] = tmp;
}
//-------------------------------------------------------------------------
void R::swap4Bytes(void *src, void *dest)
{
  char *p1,*p2;
  char tmp;
 
  p1 = (char*)src;
  p2 = (char*)dest;
  tmp = p1[3]; /* pour pouvoir utiliser src == dest */
  p2[3] = p1[0];
  p2[0] = tmp;
  tmp = p1[2]; /* pour pouvoir utiliser src == dest */
  p2[2] = p1[1];
  p2[1] = tmp;
}
//-------------------------------------------------------------------------
void R::swap4Bytes(void *src)
{
  char* p = (char*)src;
  char t;
  t = p[3]; p[3] = p[0]; p[0] = t;
  t = p[2]; p[2] = p[1]; p[1] = t;
}
//-------------------------------------------------------------------------
void R::swap8Bytes(void *src, void *dest)
{
  char *p1,*p2;
  char tmp;
  int i;
 
  p1 = (char*)src;
  p2 = (char*)dest;
  for (i=0; i<4; i++)
  {
    tmp = p1[i]; /* pour pouvoir utiliser src == dest */
    p1[i] = p2[7-i];
    p2[7-i] = tmp;
  }
}
//-------------------------------------------------------------------------
String R::toString() const
{
  return Object::toString()
    + "\n  full file name = '" + getFullFileName();
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "FileReader"; }
//-------------------------------------------------------------------------
R::~FileReader() { close(); }
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FileReader_cpp)

