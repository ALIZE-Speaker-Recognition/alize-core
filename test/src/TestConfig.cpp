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

#if !defined(ALIZE_TestConfig_cpp)
#define ALIZE_TestConfig_cpp

#include "TestConfig.h"
#include "Config.h"
#include <cstdio>
#include <iostream>

using namespace alize;
using namespace std;

//-------------------------------------------------------------------------
TestConfig::TestConfig()
:Test() {}
//-------------------------------------------------------------------------
bool TestConfig::exec()
{
   Test::exec(); // TODO : test a refaire

    {
        // test getClassName()
        test(Config().getClassName() == "Config", __LINE__);

        // test method : Config()
        {
            Config c;
            test(c._set.getLineCount() == 1, __LINE__);
            test(!c.existsParam_vectSize, __LINE__);
            test(!c.existsParam_loadFeatureFileMemAlloc, __LINE__);
            test(!c.existsParam_featureFlags, __LINE__);
            test(!c.existsParam_mixtureDistribCount, __LINE__);
            test(!c.existsParam_minLLK, __LINE__);
            test(!c.existsParam_maxLLK, __LINE__);
            test(!c.existsParam_distribType, __LINE__);
            test(!c.existsParam_saveMixtureFileFormat, __LINE__);
            test(!c.existsParam_saveMixtureServerFileFormat, __LINE__);
            test(!c.existsParam_saveFeatureFileSPro3DataKind, __LINE__);
            test(!c.existsParam_saveSegServerFileFormat, __LINE__);
            test(!c.existsParam_loadFeatureFileFormat, __LINE__);
            test(!c.existsParam_loadAudioFileChannel, __LINE__);
            test(!c.existsParam_loadMixtureFileFormat, __LINE__);
            test(!c.existsParam_loadSegServerFileFormat, __LINE__);
            test(!c.existsParam_bigEndian, __LINE__);
            test(c.existsParam_debug, __LINE__);
            test(!c.existsParam_loadFeatureFileBigEndian, __LINE__);
            test(!c.existsParam_loadAudioFileBigEndian, __LINE__);
            test(!c.existsParam_loadMixtureFileBigEndian, __LINE__);
            test(!c.existsParam_mixtureFilesPath, __LINE__);
            test(!c.existsParam_featureFilesPath, __LINE__);
            test(!c.existsParam_audioFilesPath, __LINE__);
            test(!c.existsParam_segServerFilesPath, __LINE__);
            test(!c.existsParam_loadSegServerFileExtension, __LINE__);
            test(!c.existsParam_loadFeatureFileExtension, __LINE__);
            test(!c.existsParam_loadAudioFileExtension, __LINE__);
            test(!c.existsParam_saveSegServerFileExtension, __LINE__);
            test(!c.existsParam_sampleRate, __LINE__);
            test(!c.existsParam_featureServerMask, __LINE__);
        }

        // test method : const String& getParam(const String& name) const
        {
            Config c;
            c._set.addLine().addElement("p1").addElement("c1");
            try { test(c.getParam("p1") == "c1", __LINE__); }
            catch (Exception& ) { showError(__LINE__); }
            c._set.addLine().addElement("p2").addElement("c2");
            test(c.getParam("p2") == "c2", __LINE__);
            try { c.getParam("dezzea"); showError(__LINE__); }
            catch (Exception&) {}
        }

        // test method : void setParam(const String& name, const String& content)
        {
            Config c;
            c.setParam("yuyuu", "true");
            c.setParam("vectSize", "123456");
            test(c._param_vectSize == 123456, __LINE__);

            c.setParam("loadFeatureFileMemAlloc", "123456");
            test(c._param_loadFeatureFileMemAlloc == 123456, __LINE__);

            FeatureFlags f("111111");
            c.setParam("featureFlags", "111111");
            test(c._param_featureFlags == f, __LINE__);
            f.set("000000");
            c.setParam("featureFlags", "000000");
            test(c._param_featureFlags == f, __LINE__);

            c.setParam("mixtureDistribCount", "123456");
            test(c._param_mixtureDistribCount == 123456, __LINE__);

            c.setParam("minLLK", "123456");
            test(c._param_minLLK == 123456, __LINE__);

            c.setParam("maxLLK", "123456");
            test(c._param_maxLLK == 123456, __LINE__);

            c.setParam("distribType", "GD");
            test(c._param_distribType == DistribType_GD, __LINE__);
            c.setParam("distribType", "GF");
            test(c._param_distribType == DistribType_GF, __LINE__);

            c.setParam("saveMixtureFileFormat", "XML");
            test(c._param_saveMixtureFileFormat == MixtureFileWriterFormat_XML, __LINE__);
            c.setParam("saveMixtureFileFormat", "RAW");
            test(c._param_saveMixtureFileFormat == MixtureFileWriterFormat_RAW, __LINE__);

            c.setParam("saveMixtureServerFileFormat", "XML");
            test(c._param_saveMixtureServerFileFormat == MixtureServerFileWriterFormat_XML, __LINE__);
            c.setParam("saveMixtureServerFileFormat", "RAW");
            test(c._param_saveMixtureServerFileFormat == MixtureServerFileWriterFormat_RAW, __LINE__);

            c.setParam("saveFeatureFileSPro3DataKind", "LPCEPSTRA");
            test(c._param_saveFeatureFileSPro3DataKind == SPRO3DataKind_LPCEPSTRA, __LINE__);
            c.setParam("saveFeatureFileSPro3DataKind", "OTHER");
            test(c._param_saveFeatureFileSPro3DataKind == SPRO3DataKind_OTHER, __LINE__);

            c.setParam("saveSegServerFileFormat", "XML");
            test(c._param_saveSegServerFileFormat == SegServerFileWriterFormat_XML, __LINE__);
            c.setParam("saveSegServerFileFormat", "LIUM");
            test(c._param_saveSegServerFileFormat == SegServerFileWriterFormat_LIUM, __LINE__);

            c.setParam("loadFeatureFileFormat", "SPRO3");
            test(c._param_loadFeatureFileFormat == FeatureFileReaderFormat_SPRO3, __LINE__);
            c.setParam("loadFeatureFileFormat", "RAW");
            test(c._param_loadFeatureFileFormat == FeatureFileReaderFormat_RAW, __LINE__);

            c.setParam("loadAudioFileChannel", "1");
            test(c._param_loadAudioFileChannel == 1, __LINE__);
            c.setParam("loadAudioFileChannel", "2");
            test(c._param_loadAudioFileChannel == 2, __LINE__);

            c.setParam("loadMixtureFileFormat", "AMIRAL");
            test(c._param_loadMixtureFileFormat == MixtureFileReaderFormat_AMIRAL, __LINE__);
            c.setParam("loadMixtureFileFormat", "RAW");
            test(c._param_loadMixtureFileFormat == MixtureFileReaderFormat_RAW, __LINE__);

            c.setParam("loadSegServerFileFormat", "XML");
            test(c._param_loadSegServerFileFormat == SegServerFileReaderFormat_XML, __LINE__);
            c.setParam("loadSegServerFileFormat", "LIUM");
            test(c._param_loadSegServerFileFormat == SegServerFileReaderFormat_LIUM, __LINE__);
            try { c.setParam("loadSegServerFileFormat", "sdfgfrsdfsd"); showError(__LINE__); }
            catch (Exception&) {}

            c.setParam("bigEndian", "true");
            test(c._param_bigEndian == true, __LINE__);
            c.setParam("bigEndian", "false");
            test(c._param_bigEndian == false, __LINE__);
            try { c.setParam("bigEndian", "sdf"); showError(__LINE__); }
            catch (Exception&) {}

            c.setParam("debug", "true");
            test(c._param_debug == true, __LINE__);
            c.setParam("debug", "false");
            test(c._param_debug == false, __LINE__);
            c.setParam("debug", ""); // empty = true
            test(c._param_debug == true, __LINE__);
            try { c.setParam("debug", "sdf"); showError(__LINE__); }
            catch (Exception&) {}

            c.setParam("loadFeatureFileBigEndian", "true");
            test(c._param_loadFeatureFileBigEndian == true, __LINE__);
            c.setParam("loadFeatureFileBigEndian", "false");
            test(c._param_loadFeatureFileBigEndian == false, __LINE__);
            try { c.setParam("loadFeatureFileBigEndian", "sdf"); showError(__LINE__); }
            catch (Exception&) {}

            c.setParam("loadAudioFileBigEndian", "true");
            test(c._param_loadAudioFileBigEndian == true, __LINE__);
            c.setParam("loadAudioFileBigEndian", "false");
            test(c._param_loadAudioFileBigEndian == false, __LINE__);
            try { c.setParam("loadAudioFileBigEndian", "sdf"); showError(__LINE__); }
            catch (Exception&) {}

            c.setParam("loadMixtureFileBigEndian", "true");
            test(c._param_loadMixtureFileBigEndian == true, __LINE__);
            c.setParam("loadMixtureFileBigEndian", "false");
            test(c._param_loadMixtureFileBigEndian == false, __LINE__);
            try { c.setParam("loadMixtureFileBigEndian", "sdf"); showError(__LINE__); }
            catch (Exception&) {}

            c.setParam("mixtureFilesPath", "123456789");
            test(c._param_mixtureFilesPath == "123456789", __LINE__);

            c.setParam("featureFilesPath", "123456789");
            test(c._param_featureFilesPath == "123456789", __LINE__);

            c.setParam("audioFilesPath", "123456789");
            test(c._param_audioFilesPath == "123456789", __LINE__);

            c.setParam("segServerFilesPath", "123456789");
            test(c._param_segServerFilesPath == "123456789", __LINE__);

            c.setParam("loadSegServerFileExtension", "123456789");
            test(c._param_loadSegServerFileExtension == "123456789", __LINE__);

            c.setParam("loadFeatureFileExtension", "123456789");
            test(c._param_loadFeatureFileExtension == "123456789", __LINE__);

            c.setParam("loadAudioFileExtension", "123456789");
            test(c._param_loadAudioFileExtension == "123456789", __LINE__);

            c.setParam("saveSegServerFileExtension", "123456789");
            test(c._param_saveSegServerFileExtension == "123456789", __LINE__);

            c.setParam("sampleRate", "12345.6");
            test(c._param_sampleRate == 12345.6, __LINE__);

            c.setParam("featureServerMask", "abcdefg");
            test(c._param_featureServerMask == "abcdefg", __LINE__);

            test(c.existsParam_vectSize, __LINE__);
            test(c.existsParam_loadFeatureFileMemAlloc, __LINE__);
            test(c.existsParam_featureFlags, __LINE__);
            test(c.existsParam_mixtureDistribCount, __LINE__);
            test(c.existsParam_minLLK, __LINE__);
            test(c.existsParam_maxLLK, __LINE__);
            test(c.existsParam_distribType, __LINE__);
            test(c.existsParam_saveMixtureFileFormat, __LINE__);
            test(c.existsParam_saveMixtureServerFileFormat, __LINE__);
            test(c.existsParam_saveFeatureFileSPro3DataKind, __LINE__);
            test(c.existsParam_saveSegServerFileFormat, __LINE__);
            test(c.existsParam_loadFeatureFileFormat, __LINE__);
            test(c.existsParam_loadAudioFileChannel, __LINE__);
            test(c.existsParam_loadMixtureFileFormat, __LINE__);
            test(c.existsParam_loadSegServerFileFormat, __LINE__);
            test(c.existsParam_bigEndian, __LINE__);
            test(c.existsParam_debug, __LINE__);
            test(c.existsParam_loadFeatureFileBigEndian, __LINE__);
            test(c.existsParam_loadAudioFileBigEndian, __LINE__);
            test(c.existsParam_loadMixtureFileBigEndian, __LINE__);
            test(c.existsParam_mixtureFilesPath, __LINE__);
            test(c.existsParam_featureFilesPath, __LINE__);
            test(c.existsParam_audioFilesPath, __LINE__);
            test(c.existsParam_segServerFilesPath, __LINE__);
            test(c.existsParam_loadSegServerFileExtension, __LINE__);
            test(c.existsParam_loadFeatureFileExtension, __LINE__);
            test(c.existsParam_loadAudioFileExtension, __LINE__);
            test(c.existsParam_saveSegServerFileExtension, __LINE__);
            test(c.existsParam_sampleRate, __LINE__);
            test(c.existsParam_featureServerMask, __LINE__);
        }

        // test method : unsigned long getParam_vectSize() const
        {
            Config c;
            c.setParam("vectSize", "123456");
            test(c.getParam_vectSize() == 123456, __LINE__);
        }

        // test method : unsigned long getParam_loadFeatureFileMemAlloc() const
        {
            Config c;
            c.setParam("loadFeatureFileMemAlloc", "123456");
            test(c.getParam_loadFeatureFileMemAlloc() == 123456, __LINE__);
        }

        // test method : FeatureFlags  getParam_featureFlags() const
        {
            Config c;
            FeatureFlags f("111111");
            c.setParam("featureFlags", "111111");
            test(c.getParam_featureFlags() == f, __LINE__);
            f.set("000000");
            c.setParam("featureFlags", "000000");
            test(c.getParam_featureFlags() == f, __LINE__);
        }

        // test method : unsigned long getParam_mixtureDistribCount() const
        {
            Config c;
            c.setParam("mixtureDistribCount", "123456");
            test(c.getParam_mixtureDistribCount() == 123456, __LINE__);
        }

        // test method : lk_t getParam_minLLK() const
        {
            Config c;
            c.setParam("minLLK", "123456");
            test(c.getParam_minLLK() == 123456, __LINE__);
        }

        // test method : lk_t getParam_maxLLK() const
        {
            Config c;
            c.setParam("maxLLK", "123456");
            test(c.getParam_maxLLK() == 123456, __LINE__);
        }

        // test method : DistribType getParam_distribType() const
        {
            Config c;
            c.setParam("distribType", "GD");
            test(c.getParam_distribType() == DistribType_GD, __LINE__);
            c.setParam("distribType", "GF");
            test(c.getParam_distribType() == DistribType_GF, __LINE__);
        }

        // test method : const FileFormat& getParam_saveMixtureFileFormat() const
        {
            Config c;
            c.setParam("saveMixtureFileFormat", "XML");
            test(c.getParam_saveMixtureFileFormat() == MixtureFileWriterFormat_XML, __LINE__);
            c.setParam("saveMixtureFileFormat", "RAW");
            test(c.getParam_saveMixtureFileFormat() == MixtureFileWriterFormat_RAW, __LINE__);
        }

        // test method : const FileFormat& getParam_saveMixtureServerFileFormat() const
        {
            Config c;
            c.setParam("saveMixtureServerFileFormat", "XML");
            test(c.getParam_saveMixtureServerFileFormat() == MixtureServerFileWriterFormat_XML, __LINE__);
            c.setParam("saveMixtureServerFileFormat", "RAW");
            test(c.getParam_saveMixtureServerFileFormat() == MixtureServerFileWriterFormat_RAW, __LINE__);
        }

        // test method : const FileFormat& getParam_saveFeatureFileSPro3DataKind() const
        {
            Config c;
            c.setParam("saveFeatureFileSPro3DataKind", "LPCEPSTRA");
            test(c.getParam_saveFeatureFileSPro3DataKind() == SPRO3DataKind_LPCEPSTRA, __LINE__);
            c.setParam("saveFeatureFileSPro3DataKind", "OTHER");
            test(c.getParam_saveFeatureFileSPro3DataKind() == SPRO3DataKind_OTHER, __LINE__);
        }

        // test method : const FileFormat& getParam_saveSegServerFileFormat() const
        {
            Config c;
            c.setParam("saveSegServerFileFormat", "XML");
            test(c.getParam_saveSegServerFileFormat() == SegServerFileWriterFormat_XML, __LINE__);
            c.setParam("saveSegServerFileFormat", "LIUM");
            test(c.getParam_saveSegServerFileFormat() == SegServerFileWriterFormat_LIUM, __LINE__);
        }

        // test method : const FileFormat& getParam_loadFeatureFileFormat() const
        {
            Config c;
            c.setParam("loadFeatureFileFormat", "SPRO4");
            test(c.getParam_loadFeatureFileFormat() == FeatureFileReaderFormat_SPRO4, __LINE__);
            c.setParam("loadFeatureFileFormat", "RAW");
            test(c.getParam_loadFeatureFileFormat() == FeatureFileReaderFormat_RAW, __LINE__);
        }

        // test method : const unsigned long getParam_loadAudioFileChannel() const
        {
            Config c;
            c.setParam("loadAudioFileChannel", "1");
            test(c.getParam_loadAudioFileChannel() == 1, __LINE__);
            c.setParam("loadAudioFileChannel", "2");
            test(c.getParam_loadAudioFileChannel() == 2, __LINE__);
        }

        // test method : const FileFormat& getParam_loadMixtureFileFormat() const
        {
            Config c;
            c.setParam("loadMixtureFileFormat", "XML");
            test(c.getParam_loadMixtureFileFormat() == MixtureFileReaderFormat_XML, __LINE__);
            c.setParam("loadMixtureFileFormat", "RAW");
            test(c.getParam_loadMixtureFileFormat() == MixtureFileReaderFormat_RAW, __LINE__);
        }

        // test method : const FileFormat& getParam_loadSegServerFileFormat() const
        {
            Config c;
            c.setParam("loadSegServerFileFormat", "XML");
            test(c.getParam_loadSegServerFileFormat() == SegServerFileReaderFormat_XML, __LINE__);
            c.setParam("loadSegServerFileFormat", "LIUM");
            test(c.getParam_loadSegServerFileFormat() == SegServerFileReaderFormat_LIUM, __LINE__);
        }

        // test method : bool getParam_bigEndian() const
        {
            Config c;
            c.setParam("bigEndian", "true");
            test(c.getParam_bigEndian() == true, __LINE__);
            c.setParam("bigEndian", "false");
            test(c.getParam_bigEndian() == false, __LINE__);
        }

        // test method : bool getParam_debug() const
        {
            Config c;
            c.setParam("debug", "true");
            test(c.getParam_debug() == true, __LINE__);
            c.setParam("debug", "false");
            test(c.getParam_debug() == false, __LINE__);
        }

        // test method : bool getParam_loadFeatureFileBigEndian() const
        {
            Config c;
            c.setParam("loadFeatureFileBigEndian", "true");
            test(c.getParam_loadFeatureFileBigEndian() == true, __LINE__);
            c.setParam("loadFeatureFileBigEndian", "false");
            test(c.getParam_loadFeatureFileBigEndian() == false, __LINE__);
        }

        // test method : bool getParam_loadAudioFileBigEndian() const
        {
            Config c;
            c.setParam("loadAudioFileBigEndian", "true");
            test(c.getParam_loadAudioFileBigEndian() == true, __LINE__);
            c.setParam("loadAudioFileBigEndian", "false");
            test(c.getParam_loadAudioFileBigEndian() == false, __LINE__);
        }

        // test method : bool getParam_loadMixtureFileBigEndian() const
        {
            Config c;
            c.setParam("loadMixtureFileBigEndian", "true");
            test(c.getParam_loadMixtureFileBigEndian() == true, __LINE__);
            c.setParam("loadMixtureFileBigEndian", "false");
            test(c.getParam_loadMixtureFileBigEndian() == false, __LINE__);
        }

        // test method : bool getParam_mixtureFilesPath() const
        {
            Config c;
            c.setParam("mixtureFilesPath", "123");
            test(c.getParam_mixtureFilesPath() == "123", __LINE__);
        }

        // test method : bool getParam_featureFilesPath() const
        {
            Config c;
            c.setParam("featureFilesPath", "123");
            test(c.getParam_featureFilesPath() == "123", __LINE__);
        }

        // test method : bool getParam_audioFilesPath() const
        {
            Config c;
            c.setParam("audioFilesPath", "123");
            test(c.getParam_audioFilesPath() == "123", __LINE__);
        }

        // test method : bool getParam_segServerFilesPath() const
        {
            Config c;
            c.setParam("segServerFilesPath", "123");
            test(c.getParam_segServerFilesPath() == "123", __LINE__);
        }

        // test method : bool getParam_loadSegServerFileExtension() const
        {
            Config c;
            c.setParam("loadSegServerFileExtension", "123");
            test(c.getParam_loadSegServerFileExtension() == "123", __LINE__);
        }

        // test method : bool getParam_loadFeatureFileExtension() const
        {
            Config c;
            c.setParam("loadFeatureFileExtension", "123");
            test(c.getParam_loadFeatureFileExtension() == "123", __LINE__);
        }

        // test method : bool getParam_loadAudioFileExtension() const
        {
            Config c;
            c.setParam("loadAudioFileExtension", "123");
            test(c.getParam_loadAudioFileExtension() == "123", __LINE__);
        }

        // test method : bool getParam_saveSegServerFileExtension() const
        {
            Config c;
            c.setParam("saveSegServerFileExtension", "123");
            test(c.getParam_saveSegServerFileExtension() == "123", __LINE__);
        }

        // test method : real_t getParam_sampleRate() const
        {
            Config c;
            c.setParam("sampleRate", "12345.6");
            test(c.getParam_sampleRate() == 12345.6, __LINE__);
        }

        // test method : real_t getParam_featureServerMask() const
        {
            Config c;
            c.setParam("featureServerMask", "12345.6");
            test(c.getParam_featureServerMask() == "12345.6", __LINE__);
        }

        // test method : reset()
        {
            Config c;
            c.setParam("vectSize", "444");
            c.setParam("loadFeatureFileMemAlloc", "12300");
            c.setParam("featureFlags", "010101");
            c.setParam("mixtureDistribCount", "555");
            c.setParam("minLLK", "666");
            c.setParam("maxLLK", "777");
            c.setParam("distribType", "GF");
            c.setParam("saveMixtureFileFormat", "RAW");
            c.setParam("saveMixtureServerFileFormat", "RAW");
            c.setParam("saveFeatureFileSPro3DataKind", "OTHER");
            c.setParam("saveSegServerFileFormat", "LIUM");
            c.setParam("loadFeatureFileFormat", "RAW");
            c.setParam("loadAudioFileChannel", "1");
            c.setParam("loadMixtureFileFormat", "RAW");
            c.setParam("loadSegServerFileFormat", "LIUM");
            c.setParam("bigEndian", "true");
            c.setParam("debug", "true");
            c.setParam("loadFeatureFileBigEndian", "true");
            c.setParam("loadAudioFileBigEndian", "true");
            c.setParam("loadMixtureFileBigEndian", "true");
            c.setParam("mixtureFilesPath", "123456");
            c.setParam("featureFilesPath", "789");
            c.setParam("audioFilesPath", "789");
            c.setParam("segServerFilesPath", "789");
            c.setParam("loadSegServerFileExtension", "789");
            c.setParam("loadFeatureFileExtension", "789");
            c.setParam("loadAudioFileExtension", "789");
            c.setParam("saveSegServerFileExtension", "789");
            c.setParam("sampleRate", "12345.6");
            c.setParam("featureServerMask", "12345.6");
            c.setParam("param1", "");
            c.setParam("param2", "content2");
            c.reset();
            test(c._set.getLineCount() == 1, __LINE__);
            test(!c.existsParam_vectSize, __LINE__);
            test(!c.existsParam_loadFeatureFileMemAlloc, __LINE__);
            test(!c.existsParam_featureFlags, __LINE__);
            test(!c.existsParam_mixtureDistribCount, __LINE__);
            test(!c.existsParam_minLLK, __LINE__);
            test(!c.existsParam_maxLLK, __LINE__);
            test(!c.existsParam_distribType, __LINE__);
            test(!c.existsParam_saveMixtureFileFormat, __LINE__);
            test(!c.existsParam_saveMixtureServerFileFormat, __LINE__);
            test(!c.existsParam_saveFeatureFileSPro3DataKind, __LINE__);
            test(!c.existsParam_saveSegServerFileFormat, __LINE__);
            test(!c.existsParam_loadFeatureFileFormat, __LINE__);
            test(!c.existsParam_loadAudioFileChannel, __LINE__);
            test(!c.existsParam_loadMixtureFileFormat, __LINE__);
            test(!c.existsParam_loadSegServerFileFormat, __LINE__);
            test(!c.existsParam_bigEndian, __LINE__);
            test(c.existsParam_debug, __LINE__);
            test(!c.existsParam_loadFeatureFileBigEndian, __LINE__);
            test(!c.existsParam_loadAudioFileBigEndian, __LINE__);
            test(!c.existsParam_loadMixtureFileBigEndian, __LINE__);
            test(!c.existsParam_mixtureFilesPath, __LINE__);
            test(!c.existsParam_featureFilesPath, __LINE__);
            test(!c.existsParam_audioFilesPath, __LINE__);
            test(!c.existsParam_segServerFilesPath, __LINE__);
            test(!c.existsParam_loadSegServerFileExtension, __LINE__);
            test(!c.existsParam_loadFeatureFileExtension, __LINE__);
            test(!c.existsParam_loadAudioFileExtension, __LINE__);
            test(!c.existsParam_saveSegServerFileExtension, __LINE__);
            test(!c.existsParam_sampleRate, __LINE__);
            test(!c.existsParam_featureServerMask, __LINE__);
        }

        // test method : bool operator==(const Config& c) const
        //           and bool operator!=(const Config& c) const
        {
            Config c;
            c.setParam("vectSize", "444");
            c.setParam("loadFeatureFileMemAlloc", "44400");
            c.setParam("featureFlags", "010101");
            c.setParam("mixtureDistribCount", "555");
            c.setParam("minLLK", "666");
            c.setParam("maxLLK", "777");
            c.setParam("distribType", "GF");
            c.setParam("saveMixtureFileFormat", "XML");
            c.setParam("saveMixtureServerFileFormat", "XML");
            c.setParam("saveFeatureFileSPro3DataKind", "LPCEPSTRA");
            c.setParam("saveSegServerFileFormat", "XML");
            c.setParam("loadFeatureFileFormat", "SPRO4");
            c.setParam("loadAudioFileChannel", "2");
            c.setParam("loadMixtureFileFormat", "XML");
            c.setParam("loadSegServerFileFormat", "XML");
            c.setParam("bigEndian", "true");
            c.setParam("debug", "true");
            c.setParam("loadFeatureFileBigEndian", "true");
            c.setParam("loadAudioFileBigEndian", "true");
            c.setParam("loadMixtureFileBigEndian", "true");
            c.setParam("mixtureFilesPath", "12345");
            c.setParam("featureFilesPath", "6789");
            c.setParam("audioFilesPath", "6789");
            c.setParam("segServerFilesPath", "6789");
            c.setParam("loadSegServerFileExtension", "6789");
            c.setParam("loadFeatureFileExtension", "6789");
            c.setParam("loadAudioFileExtension", "6789");
            c.setParam("saveSegServerFileExtension", "6789");
            c.setParam("sampleRate", "12345.6");
            c.setParam("featureServerMask", "12345.6");
            c.setParam("param1", "");
            c.setParam("param2", "content2");
            Config c1;
            c1.setParam("vectSize", "444");
            c1.setParam("loadFeatureFileMemAlloc", "44400");
            c1.setParam("featureFlags", "010101");
            c1.setParam("mixtureDistribCount", "555");
            c1.setParam("minLLK", "666");
            c1.setParam("maxLLK", "777");
            c1.setParam("distribType", "GF");
            c1.setParam("saveMixtureFileFormat", "XML");
            c1.setParam("saveMixtureServerFileFormat", "XML");
            c1.setParam("saveFeatureFileSPro3DataKind", "LPCEPSTRA");
            c1.setParam("saveSegServerFileFormat", "XML");
            c1.setParam("loadFeatureFileFormat", "SPRO4");
            c1.setParam("loadAudioFileChannel", "2");
            c1.setParam("loadMixtureFileFormat", "XML");
            c1.setParam("loadSegServerFileFormat", "XML");
            c1.setParam("bigEndian", "true");
            c1.setParam("debug", "true");
            c1.setParam("loadFeatureFileBigEndian", "true");
            c1.setParam("loadAudioFileBigEndian", "true");
            c1.setParam("loadMixtureFileBigEndian", "true");
            c1.setParam("mixtureFilesPath", "12345");
            c1.setParam("featureFilesPath", "6789");
            c1.setParam("audioFilesPath", "6789");
            c1.setParam("segServerFilesPath", "6789");
            c1.setParam("loadSegServerFileExtension", "6789");
            c1.setParam("loadFeatureFileExtension", "6789");
            c1.setParam("loadAudioFileExtension", "6789");
            c1.setParam("saveSegServerFileExtension", "6789");
            c1.setParam("sampleRate", "12345.6");
            c1.setParam("featureServerMask", "12345.6");
            c1.setParam("param1", "");
            c1.setParam("param2", "content2");
            test(c == c1, __LINE__);
            test(!(c != c1), __LINE__);

            c.setParam("vectSize", "999999");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("vectSize", String::valueOf(c1.getParam_vectSize()));

            c.setParam("loadFeatureFileMemAlloc", "999999");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadFeatureFileMemAlloc",
                 String::valueOf(c1.getParam_loadFeatureFileMemAlloc()));

            c.setParam("featureFlags", "111111");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("featureFlags", c1.getParam_featureFlags().getString());

            c.setParam("mixtureDistribCount", "999999999");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("mixtureDistribCount", String::valueOf(c1.getParam_mixtureDistribCount()));

            c.setParam("minLLK", "999999");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("minLLK", String::valueOf(c1.getParam_minLLK()));

            c.setParam("maxLLK", "99999999");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("maxLLK", String::valueOf(c1.getParam_maxLLK()));

            c.setParam("distribType", "GD");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("distribType", "GF");

            c.setParam("saveMixtureFileFormat", "RAW");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("saveMixtureFileFormat", "XML");

            c.setParam("saveMixtureServerFileFormat", "RAW");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("saveMixtureServerFileFormat", "XML");

            c.setParam("saveFeatureFileSPro3DataKind", "OTHER");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("saveFeatureFileSPro3DataKind", "LPCEPSTRA");

            c.setParam("saveSegServerFileFormat", "LIUM");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("saveSegServerFileFormat", "LIUM");

            c.setParam("loadFeatureFileFormat", "RAW");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadFeatureFileFormat", "SPRO4");

            c.setParam("loadAudioFileChannel", "1");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadAudioFileChannel", "2");

            c.setParam("loadMixtureFileFormat", "RAW");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadMixtureFileFormat", "XML");

            c.setParam("loadSegServerFileFormat", "LIUM");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadSegServerFileFormat", "LIUM");

            c.setParam("bigEndian", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("bigEndian", String::valueOf(c1.getParam_bigEndian()));

            c.setParam("debug", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("debug", String::valueOf(c1.getParam_debug()));

            c.setParam("loadFeatureFileBigEndian", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadFeatureFileBigEndian", String::valueOf(c1.getParam_loadFeatureFileBigEndian()));

            c.setParam("loadAudioFileBigEndian", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadAudioFileBigEndian", String::valueOf(c1.getParam_loadAudioFileBigEndian()));

            c.setParam("loadMixtureFileBigEndian", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadMixtureFileBigEndian", String::valueOf(c1.getParam_loadMixtureFileBigEndian()));

            c.setParam("mixtureFilesPath", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("mixtureFilesPath", c1.getParam_mixtureFilesPath());

            c.setParam("featureFilesPath", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("featureFilesPath", c1.getParam_featureFilesPath());

            c.setParam("audioFilesPath", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("audioFilesPath", c1.getParam_audioFilesPath());

            c.setParam("segServerFilesPath", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("segServerFilesPath", c1.getParam_segServerFilesPath());

            c.setParam("loadSegServerFileExtension", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadSegServerFileExtension", c1.getParam_loadSegServerFileExtension());

            c.setParam("loadFeatureFileExtension", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadFeatureFileExtension", c1.getParam_loadFeatureFileExtension());

            c.setParam("loadAudioFileExtension", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("loadAudioFileExtension", c1.getParam_loadAudioFileExtension());

            c.setParam("saveSegServerFileExtension", "false");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("saveSegServerFileExtension", c1.getParam_saveSegServerFileExtension());

            c.setParam("sampleRate", "99999");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("sampleRate", String::valueOf(c1.getParam_sampleRate()));

            c.setParam("featureServerMask", "99999");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("featureServerMask", c1.getParam_featureServerMask());

            c.setParam("param1", "");
            c.setParam("param2", "content2");
            test(c != c1, __LINE__);
            test(!(c == c1), __LINE__);
            c.setParam("param1", c1.getParam("param1"));
            c.setParam("param2", c1.getParam("param2"));
        }

        // test method : const Config& Config::operator=(const Config& c)
        {
            Config c;
            c.setParam("vectSize", "444");
            c.setParam("loadFeatureFileMemAlloc", "44400");
            c.setParam("featureFlags", "010101");
            c.setParam("mixtureDistribCount", "555");
            c.setParam("minLLK", "666");
            c.setParam("maxLLK", "777");
            c.setParam("distribType", "GF");
            c.setParam("saveMixtureFileFormat", "RAW");
            c.setParam("saveMixtureServerFileFormat", "RAW");
            c.setParam("saveFeatureFileSPro3DataKind", "OTHER");
            c.setParam("saveSegServerFileFormat", "LIUM");
            c.setParam("loadFeatureFileFormat", "RAW");
            c.setParam("loadAudioFileChannel", "1");
            c.setParam("loadMixtureFileFormat", "RAW");
            c.setParam("loadSegServerFileFormat", "LIUM");
            c.setParam("bigEndian", "true");
            c.setParam("debug", ""); // = true
            c.setParam("loadFeatureFileBigEndian", "true");
            c.setParam("loadAudioFileBigEndian", "true");
            c.setParam("loadMixtureFileBigEndian", "true");
            c.setParam("mixtureFilesPath", "true");
            c.setParam("featureFilesPath", "true");
            c.setParam("audioFilesPath", "true");
            c.setParam("segServerFilesPath", "true");
            c.setParam("loadSegServerFileExtension", "true");
            c.setParam("loadFeatureFileExtension", "true");
            c.setParam("loadAudioFileExtension", "true");
            c.setParam("saveSegServerFileExtension", "true");
            c.setParam("sampleRate", "12345.6");
            c.setParam("featureServerMask", "12345.6");
            c.setParam("param1", "");
            c.setParam("param2", "content2");
            Config c1;
            c1 = c;
            test(c == c1, __LINE__);
        }

        // test method : Config(const Config&)
        {
            Config c;
            c.setParam("vectSize", "444");
            c.setParam("loadFeatureFileMemAlloc", "44400");
            c.setParam("featureFlags", "010101");
            c.setParam("mixtureDistribCount", "555");
            c.setParam("minLLK", "666");
            c.setParam("maxLLK", "777");
            c.setParam("distribType", "GF");
            c.setParam("saveMixtureFileFormat", "RAW");
            c.setParam("saveMixtureServerFileFormat", "RAW");
            c.setParam("saveFeatureFileSPro3DataKind", "OTHER");
            c.setParam("saveSegServerFileFormat", "LIUM");
            c.setParam("loadFeatureFileFormat", "RAW");
            c.setParam("loadAudioFileChannel", "1");
            c.setParam("loadMixtureFileFormat", "RAW");
            c.setParam("loadSegServerFileFormat", "LIUM");
            c.setParam("bigEndian", "true");
            c.setParam("debug", "true");
            c.setParam("loadFeatureFileBigEndian", "true");
            c.setParam("loadAudioFileBigEndian", "true");
            c.setParam("loadMixtureFileBigEndian", "true");
            c.setParam("mixtureFilesPath", "true");
            c.setParam("featureFilesPath", "true");
            c.setParam("audioFilesPath", "true");
            c.setParam("segServerFilesPath", "true");
            c.setParam("loadSegServerFileExtension", "true");
            c.setParam("loadFeatureFileExtension", "true");
            c.setParam("loadAudioFileExtension", "true");
            c.setParam("saveSegServerFileExtension", "true");
            c.setParam("sampleRate", "12345.6");
            c.setParam("featureServerMask", "12345.6");
            c.setParam("param1", "");
            c.setParam("param2", "content2");
            Config c1(c);
            test(c == c1, __LINE__);
        }

        // test method : void load(const String&);
        {
            char file[] = "./TestConfigFileReaderXml.xml";
            FILE* f = fopen(file, "w");
            char buffer[] =
"<config version=\"1\">\n\
  <param name=\"vectSize\">32</param>\n\
  <param name=\"mixtureDistribCount\">128</param>\n\
  <param name=\"featureFlags\">110000</param>\n\
  <param name=\"maxLLK\">1e10</param>\n\
  <param name=\"minLLK\">-1e10</param>\n\
  <param name=\"bigEndian\">false</param>\n\
  <param name=\"debug\">false</param>\n\
  <param name=\"sampleRate\">100.0</param>\n\
  <param name=\"featureServerMask\">1,2,3-5</param>\n\
  <param name=\"saveMixtureFileFormat\">XML</param>\n\
  <param name=\"saveMixtureServerFileFormat\">XML</param>\n\
  <param name=\"loadFeatureFileFormat\">SPRO3</param>\n\
  <param name=\"loadMixtureFileFormat\">AMIRAL</param>\n\
  <param name=\"mixtureFilesPath\">123456</param>\n\
  <param name=\"featureFilesPath\">789</param>\n\
</config>";
            fwrite(&buffer, sizeof(buffer)-1, 1, f);
            fclose(f);
            //
            Config c, c1;
            c.load(file);
            c1.setParam("vectSize", "32");
            c1.setParam("mixtureDistribCount", "128");
            c1.setParam("featureFlags", "110000");
            c1.setParam("maxLLK", "1e10");
            c1.setParam("minLLK", "-1e10");
            c1.setParam("bigEndian", "false");
            c1.setParam("debug", "false");
            c1.setParam("sampleRate", "100.0");
            c1.setParam("featureServerMask", "1,2,3-5");
            c1.setParam("saveMixtureFileFormat", "XML");
            c1.setParam("saveMixtureServerFileFormat", "XML");
            c1.setParam("loadFeatureFileFormat", "SPRO3");
            c1.setParam("loadMixtureFileFormat", "AMIRAL");
            c1.setParam("mixtureFilesPath", "123456");
            c1.setParam("featureFilesPath", "789");
            test(c1 == c, __LINE__);
            //cout << c.toString() <<endl;
            //cout << c1.toString() <<endl;
            remove(file);

            // TODO : more serious tests...

        }

        // test method : Config(const String&);
        {
            char file[] = "./TestConfigFileReaderXml.xml";
            FILE* f = fopen(file, "w");
            char buffer[] =
"<config version=\"1\">\n\
  <param name=\"vectSize\">32</param>\n\
  <param name=\"mixtureDistribCount\">128</param>\n\
  <param name=\"featureFlags\">110000</param>\n\
  <param name=\"maxLLK\">1e10</param>\n\
  <param name=\"minLLK\">-1e10</param>\n\
  <param name=\"bigEndian\">false</param>\n\
  <param name=\"debug\">false</param>\n\
  <param name=\"sampleRate\">100.0</param>\n\
  <param name=\"featureServerMask\">1,2,3-5</param>\n\
  <param name=\"saveMixtureFileFormat\">XML</param>\n\
  <param name=\"saveMixtureServerFileFormat\">XML</param>\n\
  <param name=\"loadFeatureFileFormat\">SPRO3</param>\n\
  <param name=\"loadMixtureFileFormat\">AMIRAL</param>\n\
  <param name=\"mixtureFilesPath\">123456</param>\n\
  <param name=\"featureFilesPath\">789</param>\n\
</config>";
            fwrite(&buffer, sizeof(buffer)-1, 1, f);
            fclose(f);
            //
            Config c(file), c1;
            c1.setParam("vectSize", "32");
            c1.setParam("mixtureDistribCount", "128");
            c1.setParam("featureFlags", "110000");
            c1.setParam("maxLLK", "1e10");
            c1.setParam("minLLK", "-1e10");
            c1.setParam("bigEndian", "false");
            c1.setParam("debug", "false");
            c1.setParam("sampleRate", "100.0");
            c1.setParam("featureServerMask", "1,2,3-5");
            c1.setParam("saveMixtureFileFormat", "XML");
            c1.setParam("saveMixtureServerFileFormat", "XML");
            c1.setParam("loadFeatureFileFormat", "SPRO3");
            c1.setParam("loadMixtureFileFormat", "AMIRAL");
            c1.setParam("mixtureFilesPath", "123456");
            c1.setParam("featureFilesPath", "789");
            test(c1 == c, __LINE__);
            remove(file);

            // TODO : more serious tests...

        }

    } // local objects are deleted here

    return endTest();
}
//-------------------------------------------------------------------------
String TestConfig::getClassName() const { return "TestConfig"; }
//-------------------------------------------------------------------------
TestConfig::~TestConfig() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestConfig_cpp)
