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

#if !defined(ALIZE_TestAudioFileReader_cpp)
#define ALIZE_TestAudioFileReader_cpp

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "TestAudioFileReader.h"
#include "AudioFileReader.h"
#include "AudioInputStream.h"
#include "alizeString.h"
#include "Config.h"
#include "AutoDestructor.h"
#include "AudioFrame.h"
#include "FileReader.h"
#include <cstdio>
#include <iostream>

using namespace alize;
using namespace std;
typedef AudioFileReader R;

//-------------------------------------------------------------------------
TestAudioFileReader::TestAudioFileReader()
:Test() {}
//-------------------------------------------------------------------------
bool TestAudioFileReader::exec()
{
  Test::exec();
  {
#ifdef WIN32
    String dataDir = "./../../data/";
#else
    String dataDir = "./../data/";
#endif

    // test method : String getClassName() const
    {
      Config c;
      c.setParam("loadAudioFrameFileFormat", "HTK");
      test(R("./11", c).getClassName() == "AudioFileReader", __LINE__);
    }

    // test method : explicit AudioFileReader(const FileName& f, const Config& c);
    {
      Config c;
      R r("./xxx", c);
      test(r._selectedChannel == 0, __LINE__);

      c.setParam("loadAudioFileChannel", "1");
      c.setParam("audioFilesPath", "path/");
      c.setParam("loadAudioFileExtension", ".test");
      R r1("yyy", c);
      test(r1._selectedChannel == 1, __LINE__);
      test(r1._pReader->getFullFileName() == "path/yyy.test", __LINE__);
    }

    // test method : setSelectedChannel(unsigned long s)
    {
      Config c;
      const String file(dataDir + "audio/TestAudioFileReader_pcm16_1channel.sph");
      AudioFileReader r(file, c);
      r.setSelectedChannel(1);
      test(r._selectedChannel == 1, __LINE__);
    }

    // test method : unsigned long getSelectedChannel() const
    {
      Config c;
      const String file(dataDir + "audio/TestAudioFileReader_pcm16_1channel.sph");
      AudioFileReader r(file, c);
      r.setSelectedChannel(1);
      test(r.getSelectedChannel() == 1, __LINE__);

      const String file2(dataDir + "audio/TestAudioFileReader_pcm16_2channel.sph");
      AudioFileReader r2(file2, c);
      r2.setSelectedChannel(2);
      test(r2.getSelectedChannel() == 2, __LINE__);
    }
  
    // test method : virtual bool readFrame(AudioFrame&)
    // mono 16 bits
    {
      Config c;
      c.setParam("loadAudioFileBigEndian", "true");
      const String file(dataDir + "audio/TestAudioFileReader_pcm16_1channel.sph");
      AudioFileReader r(file, c);
      unsigned long cpt=0;
      AudioFrame f;
      //cout << r.toString() << endl;
      try
      {
        while (r.readFrame(f))
        {
          test(f.isValid(), __LINE__);
          cpt++;
        }
      }
      catch (EOFException&) { showError(__LINE__); }
      test(cpt == r.getFrameCount(), __LINE__);
    }
    // stereo 16 bits: read 2 channels
    {
      Config c;
      c.setParam("loadAudioFileBigEndian", "true");
      const String file(dataDir + "audio/TestAudioFileReader_pcm16_1channel.sph");
      const String file2(dataDir + "audio/TestAudioFileReader_pcm16_2channel.sph");
      AudioFileReader mono(file, c);
      AudioFileReader stereo(file2, c);
      AudioFileReader stereo2(file2, c);
      stereo.setSelectedChannel(0);
      stereo2.setSelectedChannel(1);
      unsigned long cpt=0;
      AudioFrame f, f1, f2, f3;
      //cout << r2.toString() << endl;
      try
      {
        while (stereo.readFrame(f))
        {
          stereo2.readFrame(f3);
          mono.readFrame(f1); // read mono file
          mono.readFrame(f2);
          cpt++;
          test(f1.getData() == f.getData(), __LINE__);
          test(f2.getData() == f3.getData(), __LINE__);
        }
      }
      catch (EOFException&) { showError(__LINE__); }
    }
    // mono 8 bits
    {
      Config c;
      c.setParam("loadAudioFileBigEndian", "true");
      const String file(dataDir + "audio/TestAudioFileReader_pcm16_1channel.sph");
      const String file2(dataDir + "audio/TestAudioFileReader_pcm8_1channel.sph");
      AudioFileReader r16(file, c);
      AudioFileReader r8(file2, c);
      unsigned long cpt=0;
      AudioFrame f, f1, f2;
      //cout << r2.toString() << endl;
      try
      {
        while (r16.readFrame(f))
        {
          r8.readFrame(f1);
          r8.readFrame(f2);
          cpt+=2;
          if (sizeof(short) == 2 && sizeof(char) == 1)
          {
            unsigned short uii;
            short ii = f.getData();
            memcpy(&uii, &ii, sizeof(uii));
            char* p = (char*)&uii;
            test(p[0] == f2.getData(), __LINE__);
            test(p[1] == f1.getData(), __LINE__);
          }
        }
      }
      catch (EOFException&) { showError(__LINE__); }
    }
    // stereo 8 bits
    {
      Config c;
      c.setParam("loadAudioFileBigEndian", "true");
      const String file(dataDir + "audio/TestAudioFileReader_pcm8_1channel.sph");
      const String file2(dataDir + "audio/TestAudioFileReader_pcm8_2channel.sph");
      const String file3(dataDir + "audio/TestAudioFileReader_pcm8_2channel.sph");
      AudioFileReader r(file, c);
      AudioFileReader r2(file2, c);
      AudioFileReader r3(file2, c);
      unsigned long cpt=0;
      AudioFrame f, f1, f2, f3;
      r2.setSelectedChannel(0);
      r3.setSelectedChannel(1);
      //cout << r2.toString() << endl;
      try
      {
        while (r2.readFrame(f))
        {
          r3.readFrame(f3);
          r.readFrame(f1);
          r.readFrame(f2);
          cpt++;
          test(f1.getData() == f.getData(), __LINE__);
          test(f2.getData() == f3.getData(), __LINE__);
        }
      }
      catch (EOFException&) { showError(__LINE__); }
    }

    // test method : virtual void seekFrame(unsigned long n);
    {
      Config c;
      const String file(dataDir + "audio/TestAudioFileReader_pcm16_1channel.sph");
      AudioFileReader r(file, c);
      AudioFrame f0, f1, f2, f3, f;
      r.readFrame(f0);
      r.readFrame(f1);
      r.readFrame(f2);
      r.readFrame(f3);

      r.seekFrame(0);
      r.readFrame(f);
      test(f == f0, __LINE__);
      r.readFrame(f);
      test(f == f1, __LINE__);

      r.seekFrame(3);
      r.readFrame(f);
      test(f == f3, __LINE__);
    }

    // test method : virtual void reset();
    {
      Config c;
      const String file(dataDir + "audio/TestAudioFileReader_pcm16_1channel.sph");
      AudioFileReader r(file, c);
      AudioFrame f0, f1, f2, f3, f;
      r.readFrame(f0);
      r.readFrame(f1);
      r.readFrame(f2);
      r.readFrame(f3);

      r.seekFrame(0);
      r.readFrame(f);
      test(f == f0 && f != f1, __LINE__);
    }
    // test method : unsigned long getFrameCount();
    {
      Config c;
      const String file(dataDir + "audio/TestAudioFileReader_pcm16_1channel.sph");
      AudioFileReader r(file, c);
      test(r.getFrameCount() == 8236, __LINE__);
    }

    // test method : unsigned long getChannelCount();
    {
      Config c;
      const String file(dataDir + "audio/TestAudioFileReader_pcm16_1channel.sph");
      AudioFileReader r(file, c);
      test(r.getChannelCount() == 1, __LINE__);
      const String file2(dataDir + "audio/TestAudioFileReader_pcm16_2channel.sph");
      AudioFileReader r2(file2, c);
      test(r2.getChannelCount() == 2, __LINE__);
    }

    // test method : unsigned long getSampleBytes();
    {
      Config c;
      const String file(dataDir + "audio/TestAudioFileReader_pcm16_1channel.sph");
      AudioFileReader r(file, c);
      test(r.getSampleBytes() == 2, __LINE__);
    }

  } // local objects are deleted here 

  return endTest();
}
//-------------------------------------------------------------------------
String TestAudioFileReader::getClassName() const
{ return "TestAudioFileReader"; }
//-------------------------------------------------------------------------
TestAudioFileReader::~TestAudioFileReader() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_TestAudioFileReader_cpp)
