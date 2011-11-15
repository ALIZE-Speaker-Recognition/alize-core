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

#if !defined(ALIZE_FeatureFlags_cpp)
#define ALIZE_FeatureFlags_cpp

#include "FeatureFlags.h"
#include "Exception.h"
#include "alizeString.h"

using namespace alize;

//-------------------------------------------------------------------------
FeatureFlags::FeatureFlags(const String& flags)
:Object() { set(flags); }
//-------------------------------------------------------------------------
FeatureFlags::FeatureFlags()
:Object() { set("000000"); }
//-------------------------------------------------------------------------
FeatureFlags::FeatureFlags(const FeatureFlags& flag)
:Object(), useS(flag.useS), useE(flag.useE), useD(flag.useD),
useDE(flag.useDE), useDD(flag.useDD), useDDE(flag.useDDE) {}
//-------------------------------------------------------------------------
bool FeatureFlags::operator==(const FeatureFlags& flags) const
{
  return ( useS   == flags.useS   &&
       useE   == flags.useE   &&
       useD   == flags.useD   &&
       useDE  == flags.useDE  &&
       useDD  == flags.useDD  &&
       useDDE == flags.useDDE );
}  
//-------------------------------------------------------------------------
bool FeatureFlags::operator!=(const FeatureFlags& flags) const
{ return !(*this==flags); }
//-------------------------------------------------------------------------
void FeatureFlags::set(const String& s)
{
  char f = s[0].c_str()[0];
  if (f == '0')
    useS = false;
  else if (f == '1')
    useS = true;
  else if (f != '-')
    throw Exception("Wrong parameter : '" + s + "'",
               __FILE__, __LINE__);
  f = s[1].c_str()[0];
  if (f == '0')
    useE = false;
  else if (f == '1')
    useE = true;
  else if (f != '-')
    throw Exception("Wrong parameter : '" + s + "'",
               __FILE__, __LINE__);
  f = s[2].c_str()[0];
  if (f == '0')
    useD = false;
  else if (f == '1')
    useD = true;
  else if (f != '-')
    throw Exception("Wrong parameter : '" + s + "'",
               __FILE__, __LINE__);
  f = s[3].c_str()[0];
  if (f == '0')
    useDE = false;
  else if (f == '1')
    useDE = true;
  else if (f != '-')
    throw Exception("Wrong parameter : '" + s + "'",
               __FILE__, __LINE__);
  f = s[4].c_str()[0];
  if (f == '0')
    useDD = false;
  else if (f == '1')
    useDD = true;
  else if (f != '-')
    throw Exception("Wrong parameter : '" + s + "'",
               __FILE__, __LINE__);
  f = s[5].c_str()[0];
  if (f == '0')
    useDDE = false;
  else if (f == '1')
    useDDE = true;
  else if (f != '-')
    throw Exception("Wrong parameter : '" + s + "'",
               __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
unsigned int FeatureFlags::toSPro3() const
{
  const unsigned int WITHE = 0x01; /* Data is with log energy        */
  //const unsigned int WITHZ = 0x02; /* Data as mean suppressed (const only )*/
  //const unsigned int WITHN = 0x04; /* Absolute log energy suppressed     */
  const unsigned int WITHD = 0x08; /* Data as delta coefficients       */
  const unsigned int WITHA = 0xa0; /* Data contains delta-delta coeffs   */
  const unsigned int WITHNODE = 0x10; /* */

  unsigned int flags = 0;
  if (useD)
    flags |= WITHD;
  if (useDD)
    flags |= WITHA;
  if (useE)
    flags |= WITHE;
  if (!useDE)
    flags |= WITHNODE;
  return flags;
  // TODO : lancer une exception pour les cas interdits
}
//-------------------------------------------------------------------------
unsigned long FeatureFlags::toSPro4() const
{
  const unsigned long WITHE = 0x01; /* Data is with log energy        */
  //const unsigned long WITHZ = 0x02; /* Data as mean suppressed (const only )*/
  //const unsigned long WITHN = 0x04; /* Absolute log energy suppressed     */
  const unsigned long WITHD = 0x08; /* Data as delta coefficients       */
  const unsigned long WITHA = 0xa0; /* Data contains delta-delta coeffs   */
  const unsigned long WITHNODE = 0x10; /* */

  unsigned long flags = 0;
  if (useD)
    flags |= WITHD;
  if (useDD)
    flags |= WITHA;
  if (useE)
    flags |= WITHE;
  if (!useDE)
    flags |= WITHNODE;
  return flags;
  // TODO : lancer une exception pour les cas interdits
}
//-------------------------------------------------------------------------
String FeatureFlags::getString() const
{
  char flags[] = "000000";
  if (useS)   flags[0] = '1';
  if (useE)   flags[1] = '1';
  if (useD)   flags[2] = '1';
  if (useDE)  flags[3] = '1';
  if (useDD)  flags[4] = '1';
  if (useDDE) flags[5] = '1';

  return flags;
}
//-------------------------------------------------------------------------
String FeatureFlags::getClassName() const { return "FeatureFlags"; }
//-------------------------------------------------------------------------
String FeatureFlags::toString() const
{
  return Object::toString()
    + "\n  S   = " + String::valueOf(useS)
    + "\n  E   = " + String::valueOf(useE)
    + "\n  D   = " + String::valueOf(useD)
    + "\n  DE  = " + String::valueOf(useDE)
    + "\n  DD  = " + String::valueOf(useDD)
    + "\n  DDE = " + String::valueOf(useDDE);
}
//-------------------------------------------------------------------------
FeatureFlags::~FeatureFlags() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FeatureFlags_cpp)

