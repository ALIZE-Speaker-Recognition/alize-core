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

#if !defined(ALIZE_TestSuite_cpp)
#define ALIZE_TestSuite_cpp

#include "TestSuite.h"
#include "TestString.h"
#include "TestFeatureFlags.h"
#include "TestException.h"
#include "TestRefVector.h"
#include "TestXLine.h"
#include "TestXList.h"
#include "TestRealVector.h"
#include "TestMatrix.h"
#include "TestDoubleSquareMatrix.h"
#include "TestULongVector.h"
#include "TestDistribGD.h"
#include "TestDistribGF.h"
#include "TestDistribRefVector.h"
#include "TestLabel.h"
#include "TestFeature.h"
#include "TestAudioFrame.h"
#include "TestFileReader.h"
#include "TestConfig.h" 
#include "TestSeg.h"
#include "TestSegCluster.h"
#include "TestSegServer.h"
#include "TestSegServerFileWriter.h"
#include "TestMixtureGD.h"
#include "TestMixtureGF.h"
#include "TestMixtureDict.h"
#include "TestMixtureServer.h"

#include "TestLabelSet.h"
#include "TestMixtureGDStat.h"
#include "TestMixtureGFStat.h"
#include "TestFrameAccGD.h"
#include "TestFrameAccGF.h"
#include "TestStatServer.h"

#include "TestLabelServer.h"

#include "TestFeatureFileList.h"
#include "TestFeatureFileReaderRaw.h"
#include "TestFeatureFileReaderSPro3.h"
#include "TestFeatureFileReaderSPro4.h"
#include "TestFeatureFileReaderHTK.h"
#include "TestFeatureFileReader.h"
#include "TestFeatureMultipleFileReader.h"
#include "TestFeatureInputStreamModifier.h"
#include "TestFeatureServer.h"
#include "TestFeatureComputer.h"

#include "TestAudioFileReader.h"

#include "TestFeatureFileWriter.h"
#include "TestXListFileReader.h"
#include "TestLabelFileReader.h"
#include "TestConfigFileReader.h"
#include "TestMixtureFileReaderXml.h"
#include "TestMixtureFileReaderRaw.h"
#include "TestMixtureFileReaderAmiral.h"
#include "TestMixtureFileWriter.h"
#include "TestMixtureServerFileReaderXml.h"
#include "TestMixtureServerFileReaderRaw.h"
#include "TestSegServerFileReaderRaw.h"
#include "TestMixtureServerFileWriter.h"
#include "TestViterbiAccum.h"

#include "TestHisto.h"
#include "TestConfigChecker.h"

#include "alizeString.h"

using namespace alize;

//-------------------------------------------------------------------------
TestSuite::TestSuite()
:Test() {}
//-------------------------------------------------------------------------
bool TestSuite::exec()
{
    Test::exec();

    if (!TestString().exec()) // test 31/12/3 I work this day !
        _errorFound = true;
    if (!TestRealVector().exec()) // test 1 juin 2004
        _errorFound = true;
    if (!TestDoubleSquareMatrix().exec()) //
        _errorFound = true;
    if (!TestMatrix().exec()) //
        _errorFound = true;
    if (!TestFeatureFlags().exec()) // test 26/8/3
        _errorFound = true;
    if (!TestException().exec()) // test 26/8/3
        _errorFound = true;
    if (!TestFeature().exec()) // test 31/12/3 02/02/2004
        _errorFound = true;
    if (!TestAudioFrame().exec()) // test 8 juillet 2005
        _errorFound = true;
    if (!TestDistribGD().exec()) // test 29/8/3 3/1/4
        _errorFound = true;
    if (!TestDistribGF().exec())
        _errorFound = true;
    if (!TestLabel().exec()) // test 5/1/4
        _errorFound = true;
    if (!TestFileReader().exec()) // test 3 fev 2005
        _errorFound = true;
    if (!TestConfig().exec()) // test 2/1/4 23/02/2004
        _errorFound = true;
    if (!TestMixtureGDStat().exec()) // test 12/9/3
        _errorFound = true;
    if (!TestMixtureGFStat().exec())
        _errorFound = true;
    if (!TestFrameAccGD().exec()) // test 8 avril 2005
        _errorFound = true;
    if (!TestFrameAccGF().exec()) // test 8 avril 2005
        _errorFound = true;

    if (!TestRefVector().exec()) // test 12/1/4
        _errorFound = true;
    if (!TestXLine().exec()) // test 14/02/2004
        _errorFound = true;
    if (!TestXList().exec()) // test 14/02/2004
        _errorFound = true;
    if (!TestDistribRefVector().exec()) // test 7 mai 2004
        _errorFound = true;
    if (!TestMixtureDict().exec()) // test 7 mai 2004
        _errorFound = true;
    if (!TestSeg().exec()) // test 12 oct 2004
        _errorFound = true;
    if (!TestSegCluster().exec()) // test 12 oct 2004
        _errorFound = true;
    if (!TestSegServer().exec()) // test 15 oct 2004
        _errorFound = true;

    if (!TestULongVector().exec()) // test 1 juin 2004
        _errorFound = true;
    if (!TestLabelServer().exec()) // test 12/1/4
        _errorFound = true;
    if (!TestLabelSet().exec()) // test 02 mars 2004
        _errorFound = true;

    if (!TestMixtureGD().exec()) // test 3/1/4
        _errorFound = true;
    if (!TestMixtureGF().exec())
        _errorFound = true;
    if (!TestStatServer().exec()) // test 16/9/3 not complete
        _errorFound = true;

    if (!TestFeatureFileList().exec()) // test 20 janv 2005
        _errorFound = true;
    if (!TestFeatureFileReaderRaw().exec()) // test 31/01/2004
        _errorFound = true;
    if (!TestFeatureFileReaderSPro3().exec()) // TODO : to complete
         _errorFound = true;
    if (!TestFeatureFileReaderSPro4().exec()) // 
          _errorFound = true;
    if (!TestFeatureFileReaderHTK().exec()) // test 17 sept 2004
        _errorFound = true;
    if (!TestFeatureFileReader().exec()) // test 27 janv 2005
        _errorFound = true;
    if (!TestFeatureMultipleFileReader().exec()) // test 11 jan 2005
        _errorFound = true;
    if (!TestFeatureInputStreamModifier().exec()) // test 30 sept 2004
        _errorFound = true;
    if (!TestFeatureServer().exec()) // 
        _errorFound = true;

    if (!TestAudioFileReader().exec())
        _errorFound = true;

    if (!TestFeatureFileWriter().exec()) // test 02/02/2004
        _errorFound = true;
    if (!TestXListFileReader().exec()) // test 16/02/2004
        _errorFound = true;
    if (!TestLabelFileReader().exec()) // test 19/02/2004
        _errorFound = true;
    if (!TestConfigFileReader().exec()) // test 23/02/2004
        _errorFound = true;
    if (!TestMixtureFileReaderXml().exec())
        _errorFound = true;
    if (!TestMixtureFileReaderRaw().exec())
        _errorFound = true;
    if (!TestMixtureFileReaderAmiral().exec())
        _errorFound = true;
    if (!TestMixtureFileWriter().exec())
        _errorFound = true;
    if (!TestMixtureServerFileReaderXml().exec())
        _errorFound = true;
    if (!TestMixtureServerFileReaderRaw().exec())
        _errorFound = true;
    if (!TestSegServerFileReaderRaw().exec()) // test 16 nov 2004
        _errorFound = true;
    if (!TestSegServerFileWriter().exec()) // test 17 nov 2004
        _errorFound = true;
    if (!TestMixtureServerFileWriter().exec())
        _errorFound = true;
    if (!TestHisto().exec()) // a finir - reste des bugs
        _errorFound = true;
    if (!TestViterbiAccum().exec()) // test mai 2004
        _errorFound = true;
    if (!TestMixtureServer().exec()) // test 7 mai 2004
        _errorFound = true;
    if (!TestFeatureComputer().exec()) // 
        _errorFound = true;

    if (!TestConfigChecker().exec()) // 
        _errorFound = true;

        return endTest();
}
//-------------------------------------------------------------------------
String TestSuite::getClassName() const { return "TestSuite"; }
//-------------------------------------------------------------------------
TestSuite::~TestSuite() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestSuite_cpp)
