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

#if !defined(ALIZE_Object_cpp)
#define ALIZE_Object_cpp

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdlib> // for exit()
#include <cstdio>
#include "Object.h"
#include "alizeString.h"
#include "Exception.h"

using namespace alize;

#if !defined(NDEBUG)
unsigned long Object::_creationCounter = 0;
unsigned long Object::_destructionCounter = 0;
unsigned long Object::_max = 0;
#endif

bool Object::_initialized = false;
const real_t Object::MIN_COV = 1e-200;
const real_t Object::EPS_LK = 1e-200;
const real_t Object::PI = 3.14159265358979323846;
const real_t Object::PI2 = 3.14159265358979323846*2;

const K K::k;
namespace alize
{
  ALIZE_API const TopDistribsAction DETERMINE_TOP_DISTRIBS;
  ALIZE_API const TopDistribsAction USE_TOP_DISTRIBS;
  ALIZE_API const TopDistribsAction TOP_DISTRIBS_NO_ACTION;
}
//-------------------------------------------------------------------------
Object::Object()
{
  if (!_initialized)
  {
    if (sizeof(char)      != 1 ||
      sizeof(short)     != 2 ||
      sizeof(int)       != 4 ||
      sizeof(unsigned int)  != 4 ||
      //sizeof(long)      != 4 || // == 8 sur IA64
      //sizeof(unsigned long) != 4 ||
      sizeof(float)     != 4 ||
      sizeof(double)    != 8 )
      exit(-1); // TODO : yes, but what to do ?

    const_cast<TopDistribsAction&>(DETERMINE_TOP_DISTRIBS)._i = 0;
    const_cast<TopDistribsAction&>(USE_TOP_DISTRIBS)._i = 1;
    const_cast<TopDistribsAction&>(TOP_DISTRIBS_NO_ACTION)._i = 2;
    _initialized = true;
  }

#if !defined NDEBUG
  _creationCounter++;
  unsigned long diff = _creationCounter-_destructionCounter;
  if (diff > _max)
    _max = diff;
#endif
}
//-------------------------------------------------------------------------
String Object::toString() const
{ return "[ " + getClassName() + " " + getAddress() + " ]"; }
//-------------------------------------------------------------------------
String Object::getAddress() const
{
  char str[50];
  ::sprintf(str, "%p", (void*)this);
  return str;
}
//-------------------------------------------------------------------------
bool Object::isSameObject(const Object& o) const
{ return this == &o; }
//-------------------------------------------------------------------------
#if !defined NDEBUG
unsigned long Object::getCreationCounter() // static method
{ return _creationCounter; }
//-------------------------------------------------------------------------
unsigned long Object::getDestructionCounter() // static method
{ return _destructionCounter; throw Exception("", "", 1);}
//-------------------------------------------------------------------------
unsigned long Object::getMax() // static method
{ return _max; }
#endif
//-------------------------------------------------------------------------
void Object::assertIsInBounds(const char* fileName, int line, 
                              unsigned long i, unsigned long size)
{
  if (i >= size)
    throw IndexOutOfBoundsException("", __FILE__, __LINE__, i, size);
}
//-------------------------------------------------------------------------
void Object::assertMemoryIsAllocated(const void* p, const char* f, int l)
{
  if (p == NULL)
    throw OutOfMemoryException("", f, l);
}
//-------------------------------------------------------------------------
unsigned long Object::max(unsigned long a, unsigned long b)
{ return (a>=b?a:b); }
//-------------------------------------------------------------------------
String Object::getParamTypeName(ParamType t)
{
  if (t == PARAMTYPE_INTEGER)
    return "INTEGER";
  if (t == PARAMTYPE_FLOAT)
    return "FLOAT";
  if (t == PARAMTYPE_BOOLEAN)
    return "BOOLEAN";
  if (t == PARAMTYPE_STRING)
    return "STRING";
  if (t == PARAMTYPE_UNDEFINED)
    return "";
  throw Exception("", __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
ParamType Object::getParamType(const String& s)
{
  if (s == "INTEGER")
   return PARAMTYPE_INTEGER;
  if (s == "FLOAT")
    return PARAMTYPE_FLOAT;
  if (s == "BOOLEAN")
    return PARAMTYPE_BOOLEAN;
  if (s == "STRING")
    return PARAMTYPE_STRING;
  if (s == "")
    return PARAMTYPE_UNDEFINED;
  throw Exception("", __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
MixtureServerFileWriterFormat Object::getMixtureServerFileWriterFormat(const String& name)
{
  if (name == "XML")
    return MixtureServerFileWriterFormat_XML;
  if (name == "RAW")
    return MixtureServerFileWriterFormat_RAW;
  throw Exception("Unavailable mixture file format name '" + name + "'",
                            __FILE__, __LINE__);
  return MixtureServerFileWriterFormat_RAW; // never called
}
//-------------------------------------------------------------------------
String Object::getDistribTypeName(DistribType t)
{
  if (t == DistribType_GD)
    return "GD";
  return "GF";
}
//-------------------------------------------------------------------------
DistribType Object::getDistribType(const String& name)
{
  if (name == "GD")
    return DistribType_GD;
  if (name == "GF")
    return DistribType_GF;
  throw Exception("Unavailable distrib type name '" + name + "'",
                            __FILE__, __LINE__);
  return DistribType_GD; // never called
}
//-------------------------------------------------------------------------
FeatureFileReaderFormat Object::getFeatureFileReaderFormat(const String& name)
{
  if (name == "SPRO3")
    return FeatureFileReaderFormat_SPRO3;
  if (name == "SPRO4")
    return FeatureFileReaderFormat_SPRO4;
  if (name == "RAW")
    return FeatureFileReaderFormat_RAW;
  if (name == "HTK")
    return FeatureFileReaderFormat_HTK;
  throw Exception("Unavailable feature file format name '" + name + "'",
                            __FILE__, __LINE__);
  return FeatureFileReaderFormat_RAW; // never called
}
//-------------------------------------------------------------------------
SPRO3DataKind Object::getSPro3DataKind(const String& name)
{
  if (name == "OTHER") // Anything else I didn't think about
    return SPRO3DataKind_OTHER;
  if (name == "FBANK") // Filter bank output
    return SPRO3DataKind_FBANK;
  if (name == "FBCEPSTRA") // Cepstra from filter bank analysis
    return SPRO3DataKind_FBCEPSTRA;
  if (name == "LPCEPSTRA") // Cepstra from LPC analysis
    return SPRO3DataKind_LPCEPSTRA;
  if (name == "LPCOEFF") // Linear prediction coefficients
    return SPRO3DataKind_LPCOEFF;
  if (name == "PARCOR") // Partial correlation coefficients
    return SPRO3DataKind_PARCOR;
  if (name == "LAR")// Log area ratios
    return SPRO3DataKind_LAR;
  throw Exception("Unavailable data kind name '" + name + "'",
                            __FILE__, __LINE__);
  return SPRO3DataKind_OTHER; // never called
}
//-------------------------------------------------------------------------
SegServerFileReaderFormat Object::getSegServerFileReaderFormat(const String& name)
{
  if (name == "XML")
    return SegServerFileReaderFormat_XML;
  if (name == ".seg")
    return SegServerFileReaderFormat_LIUM;
  if (name == "RAW")
    return SegServerFileReaderFormat_RAW;
  throw Exception("Unavailable segServer file format name '" + name + "'",
                            __FILE__, __LINE__);
  return SegServerFileReaderFormat_LIUM; // never called
}
//-------------------------------------------------------------------------
SegServerFileWriterFormat Object::getSegServerFileWriterFormat(const String& n)
{
  if (n == "XML")
    return SegServerFileWriterFormat_XML;
  if (n == ".seg")
    return SegServerFileWriterFormat_LIUM;
  if (n == "RAW")
    return SegServerFileWriterFormat_RAW;
  if (n == "TRS")
    return SegServerFileWriterFormat_TRS;
  throw Exception("Unavailable segServer file format name '" + n + "'",
                        __FILE__, __LINE__);
  return SegServerFileWriterFormat_XML; // never called
}
//-------------------------------------------------------------------------
MixtureFileWriterFormat Object::getMixtureFileWriterFormat(const String& name)
{
  if (name == "XML")
    return MixtureFileWriterFormat_XML;
  if (name == "RAW")
    return MixtureFileWriterFormat_RAW;
  if (name == "ETAT")
    return MixtureFileWriterFormat_ETAT;
  throw Exception("Unavailable mixture file format name '" + name + "'",
                            __FILE__, __LINE__);
  return MixtureFileWriterFormat_RAW; // never called
}
//-------------------------------------------------------------------------
FeatureFileWriterFormat Object::getFeatureFileWriterFormat(const String& name)
{
  if (name == "SPRO3")
    return FeatureFileWriterFormat_SPRO3;
  if (name == "SPRO4")
    return FeatureFileWriterFormat_SPRO4;
  if (name == "RAW")
    return FeatureFileWriterFormat_RAW;
  throw Exception("Unavailable feature file format name '" + name + "'",
                            __FILE__, __LINE__);
  return FeatureFileWriterFormat_RAW; // never called
}
//-------------------------------------------------------------------------
MixtureFileReaderFormat Object::getMixtureFileReaderFormat(const String& name)
{
  if (name == "AMIRAL")
    return MixtureFileReaderFormat_AMIRAL;
  if (name == "XML")
    return MixtureFileReaderFormat_XML;
  if (name == "RAW")
    return MixtureFileReaderFormat_RAW;
  throw Exception("Unavailable mixture file format name '" + name + "'",
                            __FILE__, __LINE__);
  return MixtureFileReaderFormat_RAW; // never called
}
//-------------------------------------------------------------------------
Object::~Object()
{
#if !defined NDEBUG
  _destructionCounter++;
  unsigned long diff = _creationCounter-_destructionCounter;
  if (diff > _max)
    _max = diff;
#endif
}
//-------------------------------------------------------------------------


#endif // !defined(ALIZE_Object_cpp)

