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

#if !defined(ALIZE_TestMixtureServerFileReaderRaw_cpp)
#define ALIZE_TestMixtureServerFileReaderRaw_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestMixtureServerFileReaderRaw.h"
#include "MixtureServerFileReaderRaw.h"
#include "MixtureServer.h"
#include <cstdio>
#include "Config.h"

using namespace alize;

//-------------------------------------------------------------------------
TestMixtureServerFileReaderRaw::TestMixtureServerFileReaderRaw()
:Test() {}
//-------------------------------------------------------------------------
bool TestMixtureServerFileReaderRaw::exec()
{
    Test::exec();

    {
        Config c;
        const String file("./TestMixtureServerFileReaderRaw");

        // test method : String getClassName()
        {
            test(MixtureServerFileReaderRaw(file, c).getClassName()
                                    == "MixtureServerFileReaderRaw", __LINE__);
        }

        // test method : explicit MixtureServerFileReaderRaw(const String& f)
        {
            MixtureServerFileReaderRaw r(file, c);
        }

        // test method : virtual void readMixtureServer(MixtureServer&, Config&);
        // GD
        {
            FILE* f = fopen(file.c_str(), "wb");
            unsigned long i;
            double v;
            char type[] ="GD";

            char s0[] ="MixtureServer";
            fwrite(s0, sizeof(s0)-1, 1, f);
            char s1[] ="serverName";
            i = sizeof(s1)-1;
            fwrite(&i, sizeof(i), 1, f);
            fwrite(s1, i, 1, f);
            i = 2; // vectSize
            fwrite(&i, sizeof(i), 1, f);
            i = 2; // mixtureCount
            fwrite(&i, sizeof(i), 1, f);
            i = 3; // distribCount
            fwrite(&i, sizeof(i), 1, f);

            fwrite(type, sizeof(type)-1, 1, f);
            char s2[] ="t";
            fwrite(s2, sizeof(s2)-1, 1, f);
            v = 1.0; // cov
            fwrite(&v, sizeof(v), 1, f);
            v = 2.0;
            fwrite(&v, sizeof(v), 1, f);
            v = 1.1; // covInv
            fwrite(&v, sizeof(v), 1, f);
            v = 2.2;
            fwrite(&v, sizeof(v), 1, f);
            v = 3.3; // mean
            fwrite(&v, sizeof(v), 1, f);
            v = 4.4;
            fwrite(&v, sizeof(v), 1, f);

            fwrite(type, sizeof(type)-1, 1, f);
            char s3[] ="f";
            fwrite(s3, sizeof(s3)-1, 1, f);
            v = 5.5; // covInv
            fwrite(&v, sizeof(v), 1, f);
            v = 6.6;
            fwrite(&v, sizeof(v), 1, f);
            v = 7.7; // mean
            fwrite(&v, sizeof(v), 1, f);
            v = 8.8;
            fwrite(&v, sizeof(v), 1, f);

            fwrite(type, sizeof(type)-1, 1, f);
            char s4[] ="f";
            fwrite(s4, sizeof(s4)-1, 1, f);
            v = 9.9; // covInv
            fwrite(&v, sizeof(v), 1, f);
            v = 10.1;
            fwrite(&v, sizeof(v), 1, f);
            v = 11.11; // mean
            fwrite(&v, sizeof(v), 1, f);
            v = 12.12;
            fwrite(&v, sizeof(v), 1, f);
            
            fwrite(type, sizeof(type)-1, 1, f);
            char s5[] ="mixtureId";
            i = sizeof(s5)-1;
            fwrite(&i, sizeof(i), 1, f);
            fwrite(s5, i, 1, f);
            i = 3; // distrib count
            fwrite(&i, sizeof(i), 1, f);
            i = 0; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.1; // weight
            fwrite(&v, sizeof(v), 1, f);
            i = 1; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.2; // weight
            fwrite(&v, sizeof(v), 1, f);
            i = 2; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.3; // weight
            fwrite(&v, sizeof(v), 1, f);
            
            fwrite(type, sizeof(type)-1, 1, f);
            char s6[] ="mixtureId2";
            i = sizeof(s6)-1;
            fwrite(&i, sizeof(i), 1, f);
            fwrite(s6, i, 1, f);
            i = 3; // distrib count
            fwrite(&i, sizeof(i), 1, f);
            i = 2; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.4; // weight
            fwrite(&v, sizeof(v), 1, f);
            i = 2; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.5; // weight
            fwrite(&v, sizeof(v), 1, f);
            i = 0; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.6; // weight
            fwrite(&v, sizeof(v), 1, f);

            fclose(f);
            //
            MixtureServerFileReaderRaw r(file, c);
            MixtureServer ms(c);
            r.readMixtureServer(ms);
            test(ms.getServerName() == "serverName", __LINE__);
            test(ms.getDistribCount() == 3, __LINE__);
            test(ms.getMixtureCount() == 2, __LINE__);
            test(ms.getVectSize() == 2, __LINE__);

            DistribGD& d0 = ms.getDistribGD(0);
            DistribGD& d1 = ms.getDistribGD(1);
            DistribGD& d2 = ms.getDistribGD(2);
            test(d0.getCov(0) == 1.0, __LINE__);
            test(d0.getCov(1) == 2.0, __LINE__);
            test(d0.getCovInv(0) == 1.1, __LINE__);
            test(d0.getCovInv(1) == 2.2, __LINE__);
            test(d0.getMean(0) == 3.3, __LINE__);
            test(d0.getMean(1) == 4.4, __LINE__);
            test(d1.getCovInv(0) == 5.5, __LINE__);
            test(d1.getCovInv(1) == 6.6, __LINE__);
            test(d1.getMean(0) == 7.7, __LINE__);
            test(d1.getMean(1) == 8.8, __LINE__);
            test(d2.getCovInv(0) == 9.9, __LINE__);
            test(d2.getCovInv(1) == 10.1, __LINE__);
            test(d2.getMean(0) == 11.11, __LINE__);
            test(d2.getMean(1) == 12.12, __LINE__);

            const MixtureGD& mix0 = ms.getMixtureGD(0);
            const MixtureGD& mix1 = ms.getMixtureGD(1);

            test(&mix0.getDistrib(0) == &d0, __LINE__);
            test(&mix0.getDistrib(1) == &d1, __LINE__);
            test(&mix0.getDistrib(2) == &d2, __LINE__);
            test(&mix1.getDistrib(0) == &d2, __LINE__);
            test(&mix1.getDistrib(1) == &d2, __LINE__);
            test(&mix1.getDistrib(2) == &d0, __LINE__);

            test(mix0.weight(0) == 0.1, __LINE__);
            test(mix0.weight(1) == 0.2, __LINE__);
            test(mix0.weight(2) == 0.3, __LINE__);
            test(mix1.weight(0) == 0.4, __LINE__);
            test(mix1.weight(1) == 0.5, __LINE__);
            test(mix1.weight(2) == 0.6, __LINE__);

            remove(file.c_str());

            // TODO : more serious tests...
        }
        // GF
        {
            FILE* f = fopen(file.c_str(), "wb");
            unsigned long i;
            double v;
            char type[] ="GF";

            char s0[] ="MixtureServer";
            fwrite(s0, sizeof(s0)-1, 1, f);
            char s1[] ="serverName";
            i = sizeof(s1)-1;
            fwrite(&i, sizeof(i), 1, f);
            fwrite(s1, i, 1, f);
            i = 2; // vectSize
            fwrite(&i, sizeof(i), 1, f);
            i = 2; // mixtureCount
            fwrite(&i, sizeof(i), 1, f);
            i = 3; // distribCount
            fwrite(&i, sizeof(i), 1, f);

            fwrite(type, sizeof(type)-1, 1, f);
            char s2[] ="t";
            fwrite(s2, sizeof(s2)-1, 1, f);
            v = 1.0; // cov
            fwrite(&v, sizeof(v), 1, f);
            v = 2.0;
            fwrite(&v, sizeof(v), 1, f);
            v = 1.01;
            fwrite(&v, sizeof(v), 1, f);
            v = 2.02;
            fwrite(&v, sizeof(v), 1, f);
            v = 1.1; // covInv
            fwrite(&v, sizeof(v), 1, f);
            v = 2.2;
            fwrite(&v, sizeof(v), 1, f);
            v = 2.01;
            fwrite(&v, sizeof(v), 1, f);
            v = 2.02;
            fwrite(&v, sizeof(v), 1, f);
            v = 3.3; // mean
            fwrite(&v, sizeof(v), 1, f);
            v = 4.4;
            fwrite(&v, sizeof(v), 1, f);

            fwrite(type, sizeof(type)-1, 1, f);
            char s3[] ="f";
            fwrite(s3, sizeof(s3)-1, 1, f);
            v = 5.5; // covInv
            fwrite(&v, sizeof(v), 1, f);
            v = 6.6;
            fwrite(&v, sizeof(v), 1, f);
            v = 6.01;
            fwrite(&v, sizeof(v), 1, f);
            v = 6.02;
            fwrite(&v, sizeof(v), 1, f);
            v = 7.7; // mean
            fwrite(&v, sizeof(v), 1, f);
            v = 8.8;
            fwrite(&v, sizeof(v), 1, f);

            fwrite(type, sizeof(type)-1, 1, f);
            char s4[] ="f";
            fwrite(s4, sizeof(s4)-1, 1, f);
            v = 9.9; // covInv
            fwrite(&v, sizeof(v), 1, f);
            v = 10.1;
            fwrite(&v, sizeof(v), 1, f);
            v = 10.01;
            fwrite(&v, sizeof(v), 1, f);
            v = 10.02;
            fwrite(&v, sizeof(v), 1, f);
            v = 11.11; // mean
            fwrite(&v, sizeof(v), 1, f);
            v = 12.12;
            fwrite(&v, sizeof(v), 1, f);
            
            fwrite(type, sizeof(type)-1, 1, f);
            char s5[] ="mixtureId";
            i = sizeof(s5)-1;
            fwrite(&i, sizeof(i), 1, f);
            fwrite(s5, i, 1, f);
            i = 3; // distrib count
            fwrite(&i, sizeof(i), 1, f);
            i = 0; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.1; // weight
            fwrite(&v, sizeof(v), 1, f);
            i = 1; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.2; // weight
            fwrite(&v, sizeof(v), 1, f);
            i = 2; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.3; // weight
            fwrite(&v, sizeof(v), 1, f);
            
            fwrite(type, sizeof(type)-1, 1, f);
            char s6[] ="mixtureId2";
            i = sizeof(s6)-1;
            fwrite(&i, sizeof(i), 1, f);
            fwrite(s6, i, 1, f);
            i = 3; // distrib count
            fwrite(&i, sizeof(i), 1, f);
            i = 2; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.4; // weight
            fwrite(&v, sizeof(v), 1, f);
            i = 2; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.5; // weight
            fwrite(&v, sizeof(v), 1, f);
            i = 0; // dict idx
            fwrite(&i, sizeof(i), 1, f);
            v = 0.6; // weight
            fwrite(&v, sizeof(v), 1, f);

            fclose(f);
            //
            MixtureServerFileReaderRaw r(file, c);
            MixtureServer ms(c);
            r.readMixtureServer(ms);
            test(ms.getServerName() == "serverName", __LINE__);
            test(ms.getDistribCount() == 3, __LINE__);
            test(ms.getMixtureCount() == 2, __LINE__);
            test(ms.getVectSize() == 2, __LINE__);

            const DistribGF& d0 = ms.getDistribGF(0);
            const DistribGF& d1 = ms.getDistribGF(1);
            const DistribGF& d2 = ms.getDistribGF(2);
            test(d0.getCov(0,0) == 1.0, __LINE__);
            test(d0.getCov(0,1) == 2.0, __LINE__);
            test(d0.getCov(1,0) == 1.01, __LINE__);
            test(d0.getCov(1,1) == 2.02, __LINE__);
            test(d0.getCovInv(0,0) == 1.1, __LINE__);
            test(d0.getCovInv(0,1) == 2.2, __LINE__);
            test(d0.getCovInv(1,0) == 2.01, __LINE__);
            test(d0.getCovInv(1,1) == 2.02, __LINE__);
            test(d0.getMean(0) == 3.3, __LINE__);
            test(d0.getMean(1) == 4.4, __LINE__);
            test(d1.getCovInv(0,0) == 5.5, __LINE__);
            test(d1.getCovInv(0,1) == 6.6, __LINE__);
            test(d1.getCovInv(1,0) == 6.01, __LINE__);
            test(d1.getCovInv(1,1) == 6.02, __LINE__);
            test(d1.getMean(0) == 7.7, __LINE__);
            test(d1.getMean(1) == 8.8, __LINE__);
            test(d2.getCovInv(0,0) == 9.9, __LINE__);
            test(d2.getCovInv(0,1) == 10.1, __LINE__);
            test(d2.getCovInv(1,0) == 10.01, __LINE__);
            test(d2.getCovInv(1,1) == 10.02, __LINE__);
            test(d2.getMean(0) == 11.11, __LINE__);
            test(d2.getMean(1) == 12.12, __LINE__);

            const MixtureGF& mix0 = ms.getMixtureGF(0);
            const MixtureGF& mix1 = ms.getMixtureGF(1);

            test(&mix0.getDistrib(0) == &d0, __LINE__);
            test(&mix0.getDistrib(1) == &d1, __LINE__);
            test(&mix0.getDistrib(2) == &d2, __LINE__);
            test(&mix1.getDistrib(0) == &d2, __LINE__);
            test(&mix1.getDistrib(1) == &d2, __LINE__);
            test(&mix1.getDistrib(2) == &d0, __LINE__);

            test(mix0.weight(0) == 0.1, __LINE__);
            test(mix0.weight(1) == 0.2, __LINE__);
            test(mix0.weight(2) == 0.3, __LINE__);
            test(mix1.weight(0) == 0.4, __LINE__);
            test(mix1.weight(1) == 0.5, __LINE__);
            test(mix1.weight(2) == 0.6, __LINE__);

            remove(file.c_str());

            // TODO : more serious tests...
        }


    } // local objects are deleted here 

    return endTest();
}
//-------------------------------------------------------------------------
String TestMixtureServerFileReaderRaw::getClassName() const
{ return "TestMixtureServerFileReaderRaw"; }
//-------------------------------------------------------------------------
TestMixtureServerFileReaderRaw::~TestMixtureServerFileReaderRaw() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestMixtureServerFileReaderRaw_cpp)
