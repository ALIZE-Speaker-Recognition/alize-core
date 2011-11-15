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

#if !defined(ALIZE_FileWriter_cpp)
#define ALIZE_FileWriter_cpp

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "FileWriter.h"
#include "Exception.h"

using namespace alize;

//-------------------------------------------------------------------------
FileWriter::FileWriter(const FileName& f)
:Object(), _pFileStruct(NULL) , _fileName(f), _swap(false){}
//-------------------------------------------------------------------------
bool FileWriter::isClosed() const { return _pFileStruct == NULL; }
//-------------------------------------------------------------------------
bool FileWriter::isOpen() const { return _pFileStruct != NULL; }
//-------------------------------------------------------------------------
void FileWriter::open()
{
  if (isOpen())
    close();
  if (_fileName.isEmpty())
    throw Exception("empty file name", __FILE__, __LINE__);
  _pFileStruct = ::fopen(_fileName.c_str(),"wb");
  if (_pFileStruct == NULL)
    throw IOException("Cannot create new file", __FILE__, __LINE__,
               _fileName);
}
//-------------------------------------------------------------------------
void FileWriter::close()
{
  if (isOpen())
    if (::fclose(_pFileStruct) == EOF)
      throw IOException("Cannot close file", __FILE__, __LINE__,
                 _fileName);
  _pFileStruct = NULL;
}
//-------------------------------------------------------------------------
void FileWriter::writeUInt4(unsigned long v)
{
  assert(_pFileStruct != NULL);
  if (sizeof(unsigned int) == 4)
  {
    if (::fwrite(&v, sizeof(unsigned int), 1, _pFileStruct) != 1)
      throw IOException("Cannot write in file", __FILE__, __LINE__,
                 _fileName);
  }
  else if (sizeof(unsigned long) == 4)
  {
    if (::fwrite(&v, sizeof(v), 1, _pFileStruct) != 1)
      throw IOException("Cannot write in file", __FILE__, __LINE__,
                _fileName);
  }
  else
    return; // TODO : what to do ?
}
//-------------------------------------------------------------------------
void FileWriter::writeDouble(double v)
{
  assert(_pFileStruct != NULL);
  if (::fwrite(&v, sizeof(v), 1, _pFileStruct) != 1)
    throw IOException("Cannot write in file", __FILE__, __LINE__,
               _fileName);
}
//-------------------------------------------------------------------------
void FileWriter::writeFloat(float v)
{
  assert(_pFileStruct != NULL);
  if (::fwrite(&v, sizeof(v), 1, _pFileStruct) != 1)
    throw IOException("Cannot write in file", __FILE__, __LINE__,
               _fileName);
}
//-------------------------------------------------------------------------
void FileWriter::writeShort(short v)
{
  assert(_pFileStruct != NULL);
  if (::fwrite(&v, sizeof(v), 1, _pFileStruct) != 1)
    throw IOException("Cannot write in file", __FILE__, __LINE__,
               _fileName);
}
//-------------------------------------------------------------------------
void FileWriter::writeChar(char v)
{
  assert(_pFileStruct != NULL);
  if (::fwrite(&v, sizeof(v), 1, _pFileStruct) != 1)
    throw IOException("Cannot write in file", __FILE__, __LINE__,
               _fileName);
}
//-------------------------------------------------------------------------
void FileWriter::writeString(const String& string)
{
  if (string.isEmpty())
    return;
  assert(_pFileStruct != NULL);
  if (::fwrite(string.c_str(), string.length(), 1, _pFileStruct) != 1)
    throw IOException("Cannot write in file", __FILE__, __LINE__,
               _fileName);
}
//-------------------------------------------------------------------------
void FileWriter::writeAttribute(const String& name, const String& value)
{
  //assert(false); // transformer les < > &... idem pour FileReader
  writeString(" " + name + "=\"" + value + "\"");
}
//-------------------------------------------------------------------------
void FileWriter::writeAttribute(const String& name, unsigned long value)
{ writeString(" " + name + "=\"" + String::valueOf(value) + "\""); }
//-------------------------------------------------------------------------
void FileWriter::writeAttribute(const String& name, double value)
{ writeString(" " + name + "=\"" + String::valueOf(value) + "\""); }
//-------------------------------------------------------------------------
String FileWriter::toString() const
{
  return Object::toString()
    + "\n  file name = '" + _fileName;
}
//-------------------------------------------------------------------------
void FileWriter::swap2Bytes(void *src, void *dest)
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
void FileWriter::swap4Bytes(void *src, void *dest)
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
void FileWriter::swap8Bytes(void *src, void *dest)
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
String FileWriter::getClassName() const { return "FileWriter"; }
//-------------------------------------------------------------------------
FileWriter::~FileWriter() { close(); }
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FileWriter_cpp)

