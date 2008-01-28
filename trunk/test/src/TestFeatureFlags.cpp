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

#if !defined(ALIZE_TestFeatureFlags_cpp)
#define ALIZE_TestFeatureFlags_cpp

#include "TestFeatureFlags.h"
#include "FeatureFlags.h"
#include "alizeString.h"

using namespace alize;

//-------------------------------------------------------------------------
TestFeatureFlags::TestFeatureFlags()
:Test() {}
//-------------------------------------------------------------------------
bool TestFeatureFlags::exec()
{
    Test::exec();

    {
        // TEST getClassName()
        FeatureFlags f1("110000");
        test(f1.getClassName() == "FeatureFlags", __LINE__);
    
        // TEST constructors

        test(f1.useS == true, __LINE__);
        test(f1.useE == true, __LINE__);
        test(f1.useD == false, __LINE__);
        test(f1.useDE == false, __LINE__);
        test(f1.useDD == false, __LINE__);
        test(f1.useDDE == false, __LINE__);

        FeatureFlags f2(f1); // Copy constructor
        test(f2.useS == f1.useS, __LINE__);
        test(f2.useE == f1.useE, __LINE__);
        test(f2.useD == f1.useD, __LINE__);
        test(f2.useDE == f1.useDE, __LINE__);
        test(f2.useDD == f1.useDD, __LINE__);
        test(f2.useDDE == f1.useDDE , __LINE__);

        FeatureFlags f11;
        f11.useS   = true;
        f11.useE   = true;
        f11.useD   = true;
        f11.useDE  = true;
        f11.useDD  = true;
        f11.useDDE = true;

        FeatureFlags f22(f11); // Copy constructor
        test(f22.useS == true, __LINE__);
        test(f22.useE == true, __LINE__);
        test(f22.useD == true, __LINE__);
        test(f22.useDE == true, __LINE__);
        test(f22.useDD == true, __LINE__);
        test(f22.useDDE == true, __LINE__);
    
        // TEST operator== et operator!=
    
        test(f1 == f2, __LINE__);
    
        test(f1 == f1, __LINE__);
    
        f2.useS = false;
        test(!(f1 == f2), __LINE__);
        f2.useS = true;
    
        f2.useE = false;
        test(!(f1 == f2), __LINE__);
        f2.useE = true;
    
        f2.useD = true;
        test(!(f1 == f2), __LINE__);
        f2.useD = false;
    
        f2.useDE = true;
        test(!(f1 == f2), __LINE__);
        f2.useDE = false;
    
        f2.useDD = true;
        test(!(f1 == f2), __LINE__);
        f2.useDD = false;
    
        f2.useDDE = true;
        test(!(f1 == f2), __LINE__);
        f2.useDDE = false;
    
        test(!(f1 != f1), __LINE__);
        test(!(f1 != f2), __LINE__);
    
    } // local objects are deleted here

    return endTest();
}
//-------------------------------------------------------------------------
String TestFeatureFlags::getClassName() const
{
    return "TestFeatureFlags";
}
//-------------------------------------------------------------------------
TestFeatureFlags::~TestFeatureFlags() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestFeatureFlags_cpp)
