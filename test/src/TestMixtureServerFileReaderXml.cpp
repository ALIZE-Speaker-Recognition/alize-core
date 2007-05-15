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

#if !defined(ALIZE_TestMixtureServerFileReaderXml_cpp)
#define ALIZE_TestMixtureServerFileReaderXml_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestMixtureServerFileReaderXml.h"
#include "MixtureServerFileReaderXml.h"
#include "MixtureServer.h"
#include <cstdio>

using namespace alize;

//-------------------------------------------------------------------------
TestMixtureServerFileReaderXml::TestMixtureServerFileReaderXml()
:Test() {}
//-------------------------------------------------------------------------
bool TestMixtureServerFileReaderXml::exec()
{
    Test::exec();

    {
        Config c;
        const String file("./TestMixtureServerFileReaderXml.xml");

        // test method : String getClassName()
        {
            test(MixtureServerFileReaderXml(file, c).getClassName()
                                    == "MixtureServerFileReaderXml", __LINE__);
        }

        // test method : explicit MixtureServerFileReaderXml(const String& f)
        {
            MixtureServerFileReaderXml r(file, c);
        }

        // test method : virtual void readMixtureServer(MixtureServer&, Config&);
        // GD type
        {
            FILE* f = fopen(file.c_str(), "wt");
            char buffer[] =
"<MixtureServer version=\"1\" name=\"serverName\" vectSize=\"2\"\
 mixtureCount=\"2\" distribCount=\"3\">\n\
  <DistribGD i=\"0\">\n\
    <cov i=\"0\">1.0</cov>\n\
    <cov i=\"1\">2.0</cov>\n\
    <covInv i=\"0\">1.1</covInv>\n\
    <covInv i=\"1\">2.2</covInv>\n\
    <mean i=\"0\">3.3</mean>\n\
    <mean i=\"1\">4.4</mean>\n\
  </DistribGD>\n\
  <DistribGD i=\"1\">\n\
    <covInv i=\"0\">5.5</covInv>\n\
    <covInv i=\"1\">6.6</covInv>\n\
    <mean i=\"0\">7.7</mean>\n\
    <mean i=\"1\">8.8</mean>\n\
  </DistribGD>\n\
  <DistribGD i=\"2\">\n\
    <covInv i=\"0\">9.9</covInv>\n\
    <covInv i=\"1\">10.1</covInv>\n\
    <mean i=\"0\">11.11</mean>\n\
    <mean i=\"1\">12.12</mean>\n\
  </DistribGD>\n\
  <MixtureGD id=\"mixtureId\" distribCount=\"3\">\n\
    <DistribGD i=\"0\" dictIdx=\"0\" weight=\"0.1\"/>\n\
    <DistribGD i=\"1\" dictIdx=\"1\" weight=\"0.2\"/>\n\
    <DistribGD i=\"2\" dictIdx=\"2\" weight=\"0.3\"/>\n\
  </MixtureGD>\n\
  <MixtureGD id=\"mixtureId2\" distribCount=\"3\">\n\
    <DistribGD i=\"0\" dictIdx=\"2\" weight=\"0.4\"/>\n\
    <DistribGD i=\"1\" dictIdx=\"2\" weight=\"0.5\"/>\n\
    <DistribGD i=\"2\" dictIdx=\"0\" weight=\"0.6\"/>\n\
  </MixtureGD>\n\
</MixtureServer>";
            fwrite(&buffer, sizeof(buffer)-1, 1, f);
            fclose(f);
            //
            MixtureServerFileReaderXml r(file, c);
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
        // GF type
        {
            FILE* f = fopen(file.c_str(), "wt");
            char buffer[] =
"<MixtureServer version=\"1\" name=\"serverName\" vectSize=\"2\"\
 mixtureCount=\"2\" distribCount=\"3\">\n\
  <DistribGF i=\"0\">\n\
    <covInv i=\"0\" j=\"0\">1.1</covInv>\n\
    <covInv i=\"0\" j=\"1\">2.2</covInv>\n\
    <covInv i=\"1\" j=\"0\">1.1</covInv>\n\
    <covInv i=\"1\" j=\"1\">2.2</covInv>\n\
    <mean i=\"0\">3.3</mean>\n\
    <mean i=\"1\">4.4</mean>\n\
  </DistribGF>\n\
  <DistribGF i=\"1\">\n\
    <covInv i=\"0\" j=\"0\">5.5</covInv>\n\
    <covInv i=\"0\" j=\"1\">6.6</covInv>\n\
    <covInv i=\"1\" j=\"0\">5.5</covInv>\n\
    <covInv i=\"1\" j=\"1\">6.6</covInv>\n\
    <mean i=\"0\">7.7</mean>\n\
    <mean i=\"1\">8.8</mean>\n\
  </DistribGF>\n\
  <DistribGF i=\"2\">\n\
    <covInv i=\"0\" j=\"0\">9.9</covInv>\n\
    <covInv i=\"0\" j=\"1\">10.1</covInv>\n\
    <covInv i=\"1\" j=\"0\">9.9</covInv>\n\
    <covInv i=\"1\" j=\"1\">10.1</covInv>\n\
    <mean i=\"0\">11.11</mean>\n\
    <mean i=\"1\">12.12</mean>\n\
  </DistribGF>\n\
  <MixtureGF id=\"mixtureId\" distribCount=\"3\">\n\
    <DistribGF i=\"0\" dictIdx=\"0\" weight=\"0.1\"/>\n\
    <DistribGF i=\"1\" dictIdx=\"1\" weight=\"0.2\"/>\n\
    <DistribGF i=\"2\" dictIdx=\"2\" weight=\"0.3\"/>\n\
  </MixtureGF>\n\
  <MixtureGF id=\"mixtureId2\" distribCount=\"3\">\n\
    <DistribGF i=\"0\" dictIdx=\"2\" weight=\"0.4\"/>\n\
    <DistribGF i=\"1\" dictIdx=\"2\" weight=\"0.5\"/>\n\
    <DistribGF i=\"2\" dictIdx=\"0\" weight=\"0.6\"/>\n\
  </MixtureGF>\n\
</MixtureServer>";
            fwrite(&buffer, sizeof(buffer)-1, 1, f);
            fclose(f);
            //
            MixtureServerFileReaderXml r(file, c);
            MixtureServer ms(c);
            r.readMixtureServer(ms);
            test(ms.getServerName() == "serverName", __LINE__);
            test(ms.getDistribCount() == 3, __LINE__);
            test(ms.getMixtureCount() == 2, __LINE__);
            test(ms.getVectSize() == 2, __LINE__);

            const DistribGF& d0 = ms.getDistribGF(0);
            const DistribGF& d1 = ms.getDistribGF(1);
            const DistribGF& d2 = ms.getDistribGF(2);
            test(d0.getCovInv(0, 0) == 1.1, __LINE__);
            test(d0.getCovInv(0, 1) == 2.2, __LINE__);
            test(d0.getCovInv(1, 0) == 1.1, __LINE__);
            test(d0.getCovInv(1, 1) == 2.2, __LINE__);
            test(d0.getMean(0) == 3.3, __LINE__);
            test(d0.getMean(1) == 4.4, __LINE__);
            test(d1.getCovInv(0, 0) == 5.5, __LINE__);
            test(d1.getCovInv(0, 1) == 6.6, __LINE__);
            test(d0.getCovInv(1, 0) == 1.1, __LINE__);
            test(d0.getCovInv(1, 1) == 2.2, __LINE__);
            test(d1.getMean(0) == 7.7, __LINE__);
            test(d1.getMean(1) == 8.8, __LINE__);
            test(d2.getCovInv(0, 0) == 9.9, __LINE__);
            test(d2.getCovInv(0, 1) == 10.1, __LINE__);
            test(d0.getCovInv(1, 0) == 1.1, __LINE__);
            test(d0.getCovInv(1, 1) == 2.2, __LINE__);
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
String TestMixtureServerFileReaderXml::getClassName() const
{ return "TestMixtureServerFileReaderXml"; }
//-------------------------------------------------------------------------
TestMixtureServerFileReaderXml::~TestMixtureServerFileReaderXml() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestMixtureServerFileReaderXml_cpp)
