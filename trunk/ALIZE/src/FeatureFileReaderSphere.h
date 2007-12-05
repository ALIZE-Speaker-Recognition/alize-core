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

#if !defined(ALIZE_FeatureFileReaderSphere_h)
#define ALIZE_FeatureFileReaderSphere_h

#ifdef WIN32
#pragma warning( disable : 4250 )
#endif

#include "FeatureFileReaderSingle.h"
#include "Feature.h"
#include "RealVector.h"

namespace alize
{
  class Config;
  class FileReader;
  class FeatureComputer;
  class AudioFileReader;
  
  /// Convenient class for reading features from a raw file
  /// The file is opened only after calling one of the methods readFeature,
  /// get FeatureCount, getVectSize, getFeatureFlags
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class FeatureFileReaderSphere : public FeatureFileReaderSingle
  {
    friend class TestFeatureFileReaderSphere;

  public :

    /// Creates a reader for a feature file.
    /// @param f a file to read. No path is
    ///    required. It uses the parameter "featureFilesPath" of the
    ///    configuration.
    /// @param c the configuration to use
    /// @param ls address of a label server. can be NULL.
    /// @param be big endian ? Can be BIGENDIAN_AUTO, BIGENDIAN_TRUE or
    ///        BIGENDIAN_FALSE
    ///
    FeatureFileReaderSphere(const FileName& f,
       const Config& c, LabelServer* ls = NULL,
       BigEndian be = BIGENDIAN_AUTO, BufferUsage b = BUFFER_AUTO,
       unsigned long bufferSize = 0,
       HistoricUsage = ALL_FEATURES, unsigned long historicSize = 0);
    
    /// See constructor with same parameters
    ///
    static FeatureFileReaderSphere& create(const FileName&, const Config&,
        LabelServer* = NULL, BigEndian = BIGENDIAN_AUTO,
        BufferUsage = BUFFER_AUTO, unsigned long bufferSize = 0,
        HistoricUsage = ALL_FEATURES, unsigned long historicSize = 0);

    virtual ~FeatureFileReaderSphere();

    /// Returns the number of features in the file.\n
    /// Slow : requires to open and close all files !
    /// @return the number of features in the file
    /// @exception Exception if vectSize is 0
    /// @exception IOException if an I/O error occurs
    /// @exception InvalidDataException thrown if file length % vectSize
    ///      is not equal to 0.
    /// 
    virtual unsigned long getFeatureCount();

    /// Returns the size of the vector inside each feature of the stream.
    /// @return the size of the vector inside each feature of the stream
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    ///
    virtual unsigned long getVectSize();

    /// Returns the feature flags of this stream.
    /// @return the feature flags of this stream
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    ///
    virtual const FeatureFlags& getFeatureFlags();

    /// Returns the sample rate of this stream.
    /// @return the sample rate of this stream
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    ///
    virtual real_t getSampleRate();

    virtual String getClassName() const;

  private :

    bool _featureCountDefined;
    FeatureComputer* _pFeatureComputer;
    AudioFileReader* _pAudioFileReader;

    FeatureInputStream* createFeatureStream(const FileName& f, const Config& c,
                             BigEndian be);

    bool operator==(const FeatureFileReaderSphere&)
                         const; /*!Not implemented*/
    bool operator!=(const FeatureFileReaderSphere&)
                         const; /*!Not implemented*/
    const FeatureFileReaderSphere& operator=(
             const FeatureFileReaderSphere&); /*!Not implemented*/
    FeatureFileReaderSphere(
             const FeatureFileReaderSphere&); /*!Not implemented*/
  };

} // end namespace alize

#endif // !defined(ALIZE_FeatureFileReaderSphere_h)

