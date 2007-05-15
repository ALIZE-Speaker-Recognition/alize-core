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

#if !defined(ALIZE_TestString_cpp)
#define ALIZE_TestString_cpp

#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <climits>
#include "TestString.h"
#include "alizeString.h"
#include "Exception.h"

using namespace alize;

/*  dans limits.h
 CHAR_BIT     Nombre de bits du type char
 CHAR_MIN     Valeur minimale du type char
 CHAR_MAX     Valeur maximale du type char
 SHRT_MIN     Valeur minimale du type short
 SHRT_MAX     Valeur maximale du type short
 INT_MIN      Valeur minimale du type int
 INT_MAX      Valeur maximale du type int
 LONG_MIN     Valeur minimale du type long
 LONG_MAX     Valeur maximale du type long
 UCHAR_MAX      Valeur maximale du type unsigned char
 USHRT_MAX     Valeur maximale du type unsigned short 
 UINT_MAX      Valeur maximale du type unsigned int
 ULONG_MAX     Valeur maximale du type unsigned long
 */

//-------------------------------------------------------------------------
TestString::TestString()
:Test() {}
//-------------------------------------------------------------------------
bool TestString::exec()
{
    Test::exec();
    {

        // TEST getClassName()
        test(String().getClassName() == "String", __LINE__);

        // test methode length()
        test(String().length() == 0,  __LINE__);
        test(String("abc\n").length() == 4, __LINE__);
        
        // test methode String(const char* = "");
        test(String().length() == 0, __LINE__);
        test(strcmp(String().c_str(), "") == 0, __LINE__);
        test(String("").length() == 0, __LINE__);
        test(strcmp(String("").c_str(), "") == 0, __LINE__);
        test(String("abc\n").length() == 4, __LINE__);
        test(strcmp(String("abc\n").c_str(), "abc\n") == 0, __LINE__);

        // test methode String(const String&);
        {
            String a, x(a);
            test(x.length() == 0, __LINE__);
            test(strcmp(x.c_str(), "") == 0, __LINE__);
            String b("abc\n"), z(b);
            test(z.length() == 4, __LINE__);
            test(strcmp(z.c_str(), "abc\n") == 0, __LINE__);
        }

        // test methode operator=(const String&);
        {
            String b("123");
            test(strcmp((b = "").c_str(), "") == 0, __LINE__);
            test(b.length() == 0, __LINE__);
            test(strcmp((b = "45678").c_str(), "45678") == 0, __LINE__);
            test(b.length() == 5, __LINE__);
        }

        // test methode operator+=(const String&);
        {
            String a("");
            a += "123";
            a += "456";
            a += "";
            test(strcmp(a.c_str(), "123456") == 0, __LINE__);
            test(a.length() == 6, __LINE__);
        }

        // test methode operator==(const String&);
        test(String("") == "", __LINE__);
        test(String("123") == "123", __LINE__);
        test(!(String("123") == "456"), __LINE__);
        test(String("1") == "1", __LINE__);
        test(!(String("1") == "2"), __LINE__);

        // test methode operator!=(const String&);
        test(!(String("") != ""), __LINE__);
        test(!(String("123") != "123"), __LINE__);
        test(String("123") != "456", __LINE__);
        test(!(String("1") != "1"), __LINE__);
        test(String("1") != "2", __LINE__);

        // test methode operator<(const String&);
        test(!(String("") < ""), __LINE__);
        test(!(String("1") < "1"), __LINE__);
        test(String("1") < "1 ", __LINE__);
        test(!(String("1 ") < "1 "), __LINE__);
        test(String("12") < "13", __LINE__);
        test(String("123") < "13", __LINE__);
        test(!(String("2") < "1"), __LINE__);
        test(!(String("2") < "12"), __LINE__);

        // test methode operator>(const String&);
        test(!(String("") > ""), __LINE__);
        test(!(String("1") > "1"), __LINE__);
        test(String("1 ") > "1", __LINE__);
        test(!(String("1 ") > "1 "), __LINE__);
        test(String("13") > "12", __LINE__);
        test(String("13") > "123", __LINE__);
        test(!(String("1") > "2"), __LINE__);
        test(!(String("12") > "2"), __LINE__);

        // test methode operator+(const String&);
        {
            String x("123");
            String y = x + "456" + "789";
            test(strcmp(y.c_str(), "123456789") == 0, __LINE__);
            test(y.length() == 9, __LINE__);
        }
        // test methode operator[](const String&);
        {
            String x("123");
            test(x[0] == "1", __LINE__);
            test(x[1] == "2", __LINE__);
            test(x[2] == "3", __LINE__);
            test(String()[0] == "", __LINE__);
            test(x[3] == "", __LINE__);
            test(x[100] == "", __LINE__);
         }

        // test methode valueOf(unsigned long)
        {
            unsigned long x = 0;
            test(strtoul(String::valueOf(x).c_str(), NULL, 0) == x, __LINE__);
            x = 1;
            test(strtoul(String::valueOf(x).c_str(), NULL, 0) == x, __LINE__);
            x = ULONG_MAX;
            test(strtoul(String::valueOf(x).c_str(), NULL, 0) == x, __LINE__);
        }

        // test methode valueOf(long)
        {
            long x = 0;
            test(atol(String::valueOf(x).c_str()) == x, __LINE__);
            x = 1;
            test(atol(String::valueOf(x).c_str()) == x, __LINE__);
            x = -1;
            test(atol(String::valueOf(x).c_str()) == x, __LINE__);
            x = LONG_MAX;
            test(atol(String::valueOf(x).c_str()) == x, __LINE__);
            x = LONG_MIN;
            test(atol(String::valueOf(x).c_str()) == x, __LINE__);
        }

        // test methode valueOf(double)
        {
            double x = 0;
            test(atof(String::valueOf(x).c_str()) == x, __LINE__);
            x = 1;
            test(atof(String::valueOf(x).c_str()) == x, __LINE__);
            x = -1;
            test(atof(String::valueOf(x).c_str()) == x, __LINE__);
            x = 123456e15;
            test(atof(String::valueOf(x).c_str()) == x, __LINE__);
            x = -12345678e-56;
            test(atof(String::valueOf(x).c_str()) == x, __LINE__);
        }

        // test methode valueOf(unsigned int)
        {
            unsigned int x = 0;
            test(strtoul(String::valueOf(x).c_str(), NULL, 0) == x, __LINE__);
            x = 1;
            test(strtoul(String::valueOf(x).c_str(), NULL, 0) == x, __LINE__);
            x = UINT_MAX;
            test(strtoul(String::valueOf(x).c_str(), NULL, 0) == x, __LINE__);
        }

        // test method long String::toLong() const
        {
          try
          {
            test(String("1").toLong() == 1, __LINE__);
            test(String("-1").toLong() == -1, __LINE__);
            test(String(" 1").toLong() == 1, __LINE__);
            test(String("1 ").toLong() == 1, __LINE__);
            test(String("123").toLong() == 123, __LINE__);
          }
          catch (Exception&) { showError(__LINE__); }
          try { String("").toLong(); showError(__LINE__); } catch (Exception&) {}
          try { String("x").toLong(); showError(__LINE__); } catch (Exception&) {}
          try { String(" ").toLong(); showError(__LINE__); } catch (Exception&) {}
          try { String(".").toLong(); showError(__LINE__); } catch (Exception&) {}
        }

        // test method long toDouble() const
        {
          try
          {
            test(String("1").toDouble() == 1, __LINE__);
            test(String("-1").toDouble() == -1, __LINE__);
            test(String(" 1").toDouble() == 1, __LINE__);
            test(String("1 ").toDouble() == 1, __LINE__);
            test(String("123").toDouble() == 123, __LINE__);
          }
          catch (Exception&) { showError(__LINE__); }
          try { String("").toDouble(); showError(__LINE__); } catch (Exception&) {}
          try { String("x").toDouble(); showError(__LINE__); } catch (Exception&) {}
          try { String(" ").toDouble(); showError(__LINE__); } catch (Exception&) {}
          try { String(".").toDouble(); showError(__LINE__); } catch (Exception&) {}
        }
        // test method : bool toBool(const String& s) const
        {
          String a = "true";
          try { test(a.toBool() == true, __LINE__); }
          catch (Exception&) { showError(__LINE__); }
          String b = "false";
          try { test(b.toBool() == false, __LINE__); }
          catch (Exception&) { showError(__LINE__); }
          String c = "xzsds";
          try { c.toBool(); showError(__LINE__); }
          catch (Exception&) {}
        }

        // test methode valueOf(int)
        test(atol(String::valueOf((int)0).c_str()) == (int)0, __LINE__);
        test(atol(String::valueOf((int)1).c_str()) == (int)1, __LINE__);
        test(atol(String::valueOf((int)-1).c_str()) == (int)-1, __LINE__);
        test(atol(String::valueOf(INT_MAX).c_str()) == INT_MAX, __LINE__);
        test(atol(String::valueOf(INT_MIN).c_str()) == INT_MIN, __LINE__);

        // test methode valueOf(bool)
        test(String::valueOf(true) == "true", __LINE__);
        test(String::valueOf(false) == "false", __LINE__);

        // test methode endWith(const String&)
        test(String().endsWith(""), __LINE__);
        test(!String().endsWith("a"), __LINE__);
        test(!String().endsWith("abc"), __LINE__);
        test(String("a").endsWith("a"), __LINE__);
        test(String("a").endsWith(""), __LINE__);
        test(!String("a").endsWith("b"), __LINE__);
        test(String("ab").endsWith("b"), __LINE__);
        test(!String("ab").endsWith("a"), __LINE__);
        test(String("aaabbb").endsWith("b"), __LINE__);
        test(String("aaabbb").endsWith("bb"), __LINE__);
        test(String("aaabbb").endsWith("bbb"), __LINE__);

        // test methode beginsWith(const String&)
        test(String().beginsWith(""), __LINE__);
        test(!String().beginsWith("a"), __LINE__);
        test(String("a").beginsWith("a"), __LINE__);
        test(String("ab").beginsWith("a"), __LINE__);
        test(!String("ab").beginsWith("b"), __LINE__);
        test(String("aabb").beginsWith("a"), __LINE__);
        test(String("aabb").beginsWith("a"), __LINE__);
        test(String("abc").beginsWith(""), __LINE__);
        test(!String("abc").beginsWith("abcd"), __LINE__);

        // test methode getToken(unsigned long index)
        test(String().getToken(0) == "", __LINE__);
        test(String().getToken(111) == "", __LINE__);
        test(String("a").getToken(0) == "a", __LINE__);
        test(String("a").getToken(1) == "", __LINE__);
        test(String(" a").getToken(0) == "a", __LINE__);
        test(String(" a").getToken(1) == "", __LINE__);
        test(String("  a").getToken(0) == "a", __LINE__);
        test(String("  a").getToken(1) == "", __LINE__);
        test(String("  a ").getToken(0) == "a", __LINE__);
        test(String("  a ").getToken(1) == "", __LINE__);
        test(String("  a  ").getToken(0) == "a", __LINE__);
        test(String("  a  ").getToken(1) == "", __LINE__);
        test(String("  ab ").getToken(0) == "ab", __LINE__);
        test(String("  ab ").getToken(1) == "", __LINE__);
        test(String("  ab\t cde ").getToken(0) == "ab", __LINE__);
        test(String("  ab cde ").getToken(1) == "cde", __LINE__);
        test(String("  ab\t cde ").getToken(2) == "", __LINE__);
        test(String("  ab cde  fghi\t").getToken(0) == "ab", __LINE__);
        test(String("\t  ab  cde   fghi").getToken(1) == "cde", __LINE__);
        test(String("  \tab cde    fghi").getToken(2) == "fghi", __LINE__);
        test(String("  ab  \tcde     fghi").getToken(3) == "", __LINE__);

        test(String().getToken(0, " ") == "", __LINE__);
        test(String().getToken(111, " ") == "", __LINE__);
        test(String("a").getToken(0, " ") == "a", __LINE__);
        test(String("a").getToken(1, " ") == "", __LINE__);
        test(String(" a").getToken(0, " ") == "a", __LINE__);
        test(String(" a").getToken(1, " ") == "", __LINE__);
        test(String("  a").getToken(0, " ") == "a", __LINE__);
        test(String("  a").getToken(1, " ") == "", __LINE__);
        test(String("  a ").getToken(0, " ") == "a", __LINE__);
        test(String("  a ").getToken(1, " ") == "", __LINE__);
        test(String("  a  ").getToken(0, " ") == "a", __LINE__);
        test(String("  a  ").getToken(1, " ") == "", __LINE__);
        test(String("  ab ").getToken(0, " ") == "ab", __LINE__);
        test(String("  ab ").getToken(1, " ") == "", __LINE__);
        test(String("  ab\t cde ").getToken(0, " ") == "ab", __LINE__);
        test(String("  ab cde ").getToken(1, " ") == "cde", __LINE__);
        test(String("  ab\t cde ").getToken(2, " ") == "", __LINE__);
        test(String("  ab cde  fghi\t").getToken(0, " ") == "ab", __LINE__);
        test(String("\t  ab  cde   fghi").getToken(1, " ") == "cde", __LINE__);
        test(String("  \tab cde    fghi").getToken(2, " ") == "fghi", __LINE__);
        test(String("  ab  \tcde     fghi").getToken(3, " ") == "", __LINE__);

        test(String().getToken(0, "-") == "", __LINE__);
        test(String().getToken(111, "-") == "", __LINE__);
        test(String("a").getToken(0, "-") == "a", __LINE__);
        test(String("a").getToken(1, "-") == "", __LINE__);
        test(String("-a").getToken(0, "-") == "a", __LINE__);
        test(String("-a").getToken(1, "-") == "", __LINE__);
        test(String("--a").getToken(0, "-") == "a", __LINE__);
        test(String("--a").getToken(1, "-") == "", __LINE__);
        test(String("--a-").getToken(0, "-") == "a", __LINE__);
        test(String("--a-").getToken(1, "-") == "", __LINE__);
        test(String("--a--").getToken(0, "-") == "a", __LINE__);
        test(String("--a--").getToken(1, "-") == "", __LINE__);
        test(String("--ab-").getToken(0, "-") == "ab", __LINE__);
        test(String("--ab-").getToken(1, "-") == "", __LINE__);
        test(String("--ab--cde ").getToken(0, "-") == "ab", __LINE__);
        test(String("--ab-cde-").getToken(1, "-") == "cde", __LINE__);
        test(String("--ab--cde-").getToken(2, "-") == "", __LINE__);
        test(String("--ab-cde--fghi-").getToken(0, "-") == "ab", __LINE__);
        test(String("---ab--cde---fghi").getToken(1, "-") == "cde", __LINE__);
        test(String("---ab-cde----fghi").getToken(2, "-") == "fghi", __LINE__);
        test(String("---ab---cde-----fghi").getToken(3, "-") == "", __LINE__);


        // test methode find(const String&, unsigned long start=0)
        test(String("").find("") == -1, __LINE__);
        test(String("").find("", 0) == -1, __LINE__);
        test(String("").find("", 1) == -1, __LINE__);
        test(String("a").find("") == -1, __LINE__);
        test(String("a").find("", 1) == -1, __LINE__);
        test(String("a").find("a") == 0, __LINE__);
        test(String(" abab").find("ab") == 1, __LINE__);
        test(String(" ab abc").find("abc") == 4, __LINE__);
        test(String(" ab abc").find("abc", 4) == 4, __LINE__);
        test(String(" ab abc").find("abc", 5) == -1, __LINE__);

    } // local objects are deleted here

    return endTest();
}
//-------------------------------------------------------------------------
String TestString::getClassName() const
{
    return "TestString";
}
//-------------------------------------------------------------------------
TestString::~TestString() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestString_cpp)
