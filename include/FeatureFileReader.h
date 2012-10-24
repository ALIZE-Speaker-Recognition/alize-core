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

#if !defined(ALIZE_FeatureFileReader_h)
#define ALIZE_FeatureFileReader_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include "FeatureFileReaderAbstract.h"
#include "RealVector.h"

namespace alize
{
  class LabelServer;
  class XLine;
  class Config;

  /// Convenient class for reading features from files with various
  /// formats (HTK, SPRO3/4, RAW)
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API FeatureFileReader : public FeatureFileReaderAbstract
  {
  public :

    friend class TestFeatureFileReader;

    /// Creates a reader for an empty file.
    /// @param c the configuration to use
    ///
    FeatureFileReader(const Config& c);

    /// Creates a reader for a feature file.
    /// @param f a file to read. No path is
    ///    required. It uses the parameter "featureFilesPath" of the
    ///    configuration.
    /// @param c the configuration to use
    /// @param ls address of a label server. can be NULL.
    /// @param be big endian ? Can be BIGENDIAN_AUTO, BIGENDIAN_TRUE or
    ///        BIGENDIAN_FALSE
    ///
    FeatureFileReader(const FileName& f, const Config& c,
              LabelServer* ls = NULL, BigEndian be = BIGENDIAN_AUTO,
              BufferUsage = BUFFER_AUTO, unsigned long bufferSize = 0,
              HistoricUsage = ALL_FEATURES, unsigned long historicSize = 0);
    
    /// Create a new file reader to read features from a list of file\n
    /// The files will be opened, read and closed one after the other.
    /// The order will be the same as the list order.
    /// For the user, it will be as if it was a single file.
    /// 
    /// @param l list of file to read
    /// @param ls address of a label server. can be NULL.
    /// @param c configuration to use
    /// @param be big endian ? Can be BIGENDIAN_AUTO, BIGENDIAN_TRUE or
    ///        BIGENDIAN_FALSE
    ///
    explicit FeatureFileReader(const XLine& l, const Config&, 
                    LabelServer* ls = NULL, BigEndian be = BIGENDIAN_AUTO,
                    BufferUsage = BUFFER_AUTO, unsigned long bufferSize = 0,
                    HistoricUsage = ALL_FEATURES, unsigned long historicSize = 0);
    /// See constructor with same parameters
    ///
    static FeatureFileReader& create(const Config& c);

    /// See constructor with same parameters
    ///
    static FeatureFileReader& create(const FileName& f, const Config& c,
                    LabelServer* ls = NULL, BigEndian be = BIGENDIAN_AUTO,
                    BufferUsage = BUFFER_AUTO, unsigned long bufferSize = 0,
                    HistoricUsage = ALL_FEATURES, unsigned long historicSize = 0);
    
    /// See constructor with same parameters
    ///
    static FeatureFileReader& create(const XLine& l, const Config& c,
                    LabelServer* ls = NULL, BigEndian be = BIGENDIAN_AUTO,
                    BufferUsage = BUFFER_AUTO, unsigned long bufferSize = 0,
                    HistoricUsage = ALL_FEATURES, unsigned long historicSize = 0);

    virtual ~FeatureFileReader();

    virtual void seekFeature(unsigned long featureNbr,
                             const String& srcName = "");
    virtual bool addFeature(const Feature& f);
    virtual bool readFeature(Feature& f, unsigned long s = 1);

    virtual bool writeFeature(const Feature& f, unsigned long step = 1);

    /// Returns the number of features in the file
    /// @return the number of features in the file
    /// @exception Exception if vectSize is 0
    /// @exception IOException if an I/O error occurs
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

    /// Returns the number of files read by the reader
    /// @return the number of files
    ///
    virtual unsigned long getSourceCount();

    /// Returns the feature count of a feature file
    /// @param srcIdx index of the file
    /// @return the feature count of the file
    ///
    virtual unsigned long getFeatureCountOfASource(unsigned long srcIdx);

    /// Returns the feature count of a feature file
    /// @param src name of the file
    /// @return the feature count of the file
    ///
    virtual unsigned long getFeatureCountOfASource(const String& src);

    /// Returns the number of the first feature of a feature file. Useful
    /// for a multiple file stream.
    /// @param srcIdx index of the file
    /// @return the number of the first feature
    ///
    virtual unsigned long getFirstFeatureIndexOfASource(unsigned long srcIdx);

    /// Returns the number of the first feature of a feature file. Useful
    /// for a multiple file stream.
    /// @param src name of the file
    /// @return the number of the first feature
    ///
    virtual unsigned long getFirstFeatureIndexOfASource(const String& srcName);

    /// Returns the a particular feature file. Useful
    /// for a multiple file stream.
    /// @param srcIdx index of the file
    /// @return the name of the file
    ///
    virtual const String& getNameOfASource(unsigned long srcIdx);

    /// Closes all opened files
    ///
    virtual void close();

    virtual void setExternalBufferToUse(FloatVector& v);
    static FeatureFileReaderAbstract& createStream(const Config& c);
    static FeatureFileReaderAbstract& createStream(const FileName& f,
                                            const Config& c,
                                            LabelServer* ls,
                                            BigEndian be,
                                            BufferUsage b,
                                            unsigned long bufferSize,
                                            HistoricUsage,
                                            unsigned long historicSize);
    static FeatureFileReaderAbstract& createStream(const XLine& l,
                                            const Config& c,
                                            LabelServer* p,
                                            BigEndian be,
                                            BufferUsage b,
                                            unsigned long bufferSize,
                                            HistoricUsage,
                                            unsigned long historicSize);

    virtual String getClassName() const;

  private :

    FeatureFileReaderAbstract* _pFeatureReader;

    bool operator==(const FeatureFileReader&)
                         const; /*!Not implemented*/
    bool operator!=(const FeatureFileReader&)
                         const; /*!Not implemented*/
    const FeatureFileReader& operator=(
             const FeatureFileReader&); /*!Not implemented*/
    FeatureFileReader(
             const FeatureFileReader&); /*!Not implemented*/
  };

} // end namespace alize

#endif // !defined(ALIZE_FeatureFileReader_h)

