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

#if !defined(ALIZE_FeatureServer_h)
#define ALIZE_FeatureServer_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include "FeatureInputStream.h"
#include "RefVector.h"

namespace alize
{
  class Config;
  class XLine;

  /*!
  This class represents a features server.\n
  A featureServer is a circular buffer that can store features. Each feature
  can be accessed using an index or in a sequential way like a file.
  A feature server can read feature from a FeatureInputStream derived
  object. A feature server is itself derived from the FeatureInputStream
  class. It is possible to connect a feature server to an other one.
  When you create a feature server with a FeatureInputStream object, the
  server will read features from this object and store them until the object
  returns a NULL pointer. If the buffer size exceed the number of features
  loaded, the superfluous features are set to invalid.
  If you create a feature server without FeatureInputStream object connected,
  all the features are invalid.
  
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2003
  */
  class ALIZE_API FeatureServer : public FeatureInputStream
  {
    friend class TestFeatureServer;
  public :

    /// Default constructor
    ///
    explicit FeatureServer();

    /// Creates a FeatureServer object
    /// @param c configuration to build the server
    ///
    explicit FeatureServer(const Config& c);

    /// Creates a FeatureServer object.
    /// @param c configuration to build the server
    /// @param s a reference to a FeatureStream object that can
    ///    provide features for the server.
    ///
    explicit FeatureServer(const Config& c, FeatureInputStream& s);

    /// Creates a FeatureServer object.
    /// @param c configuration to build the server
    /// @param f a feature file or a file (extension ".lst")
    ///    that contains a list of feature files
    ///
    explicit FeatureServer(const Config& c, const FileName& f);

    /// Creates a FeatureServer object.
    /// @param c configuration to build the server
    /// @param f a feature file or a file (extension ".lst")
    ///    that contains a list of feature files
    /// @param ls a label server.
    ///
    explicit FeatureServer(const Config& c, const FileName& f,
                                                        LabelServer& ls);
    /// Creates a FeatureServer object.
    /// @param c configuration to build the server
    /// @param l a list of feature file names WITHOUT PATH AND EXTENSION
    ///    The file type, extension and path come from the
    ///    configuration (parameters "loadFeatureFileFormat" and
    ///    "featureFilesPath")
    ///
    explicit FeatureServer(const Config& c, const XLine& l);

    /// Creates a FeatureServer object.
    /// @param c configuration to build the server
    /// @param l a list of feature file names WITHOUT PATH AND EXTENSION
    ///    The file type, extension and path come from the
    ///    configuration (parameters "loadFeatureFileFormat" and
    ///    "featureFilesPath")
    /// @param ls a label server.
    ///
    explicit FeatureServer(const Config& c, const XLine& l,
                                                        LabelServer& ls);
    /// Re-initialize this server.
    /// @param c configuration to build the server
    ///
    void init(const Config& c);

    /// Re-initialize this server.
    /// @param c configuration to build the server
    /// @param s a reference to a FeatureStream object that can
    ///    provide features for the server.
    ///
    void init(const Config& c, FeatureInputStream& s);

    /// Re-initialize this server.
    /// Creates a FeatureServer object.
    /// @param c configuration to build the server
    /// @param f a feature file or a file (extension ".lst")
    ///    that contains a list of feature files
    ///
    void init(const Config& c, const FileName& f);

    /// Re-initialize this server.
    /// @param c configuration to build the server
    /// @param f a feature file or a file (extension ".lst")
    ///    that contains a list of feature files
    /// @param ls a label server.
    ///
    void init(const Config& c, const FileName& f, LabelServer& ls);

    /// Re-initialize this server.
    /// Creates a FeatureServer object.
    /// @param c configuration to build the server
    /// @param l a list of feature file names WITHOUT PATH AND EXTENSION
    ///    The file type, extension and path come from the
    ///    configuration (parameters "loadFeatureFileFormat" and
    ///    "featureFilesPath")
    ///
    void init(const Config& c, const XLine& l);

    /// Re-initialize this server.
    /// @param c configuration to build the server
    /// @param l a list of feature file names WITHOUT PATH AND EXTENSION
    ///    The file type, extension and path come from the
    ///    configuration (parameters "loadFeatureFileFormat" and
    ///    "featureFilesPath")
    /// @param ls a label server.
    ///
    void init(const Config& c, const XLine& l, LabelServer& ls);

    virtual ~FeatureServer();

    /// Returns the number of features in the stream connected to the
    /// server.\n
    /// @return the number of features in the stream if possible
    /// @exception Exception if the stream cannot give it's features count
    // TODO : gérer une exception particulière - la même que pour les param
    /// 
    virtual unsigned long getFeatureCount();

    /// Returns the vectSize of the features
    /// @return the vectSize of the features
    ///
    virtual unsigned long getVectSize();

    /// Returns the feature flags of the features
    /// @return the feature flags of the features
    ///
    virtual const FeatureFlags& getFeatureFlags();

    /// Returns the sample rate of the stream.
    /// @return the sample rate of the stream
    ///
    virtual real_t getSampleRate();

    const String& getServerName() const;
    void setServerName(const String& s);

    /// Reads a feature
    /// @param f the feature to store the data read
    /// @param s after reading, the feature pointer is moved s steps forward.
    ///          Can be 0
    /// @return false if end of stream met; true otherwise. In that case
    /// think to test feature validity before using
    ///    
    virtual bool readFeature(Feature& f, unsigned long s = 1);

    /// adds a feature
    /// @param f the feature to store the data read
    /// @return false not possible to add feature
    /// @exception featureServerMemAlloc is required to add feature to a featureServer
    ///    
    virtual bool addFeature(const Feature& f);

    /// Writes a feature
    /// @param f the feature
    /// @param s after writing, the feature pointer is moved s steps forward.
    ///          Can be 0
    ///    
    virtual bool writeFeature(const Feature& f, unsigned long s = 1);

    /// Moves the feature pointer for the next reading/writing
    /// @param n index of the next feature to read or write in the source
    /// @param s name of the source. Default  is ""
    ///    
    virtual void seekFeature(unsigned long n, const String& s = "");

    /// Reset the server
    /// @exception Exception if the server cannot be reseted
    /// @exception IOException if an I/O error occurs
    ///
    virtual void reset(); // TODO : tester

    /// Closes all the opened sources
    ///
    virtual void close();

    /// Returns the number of source read by the reader
    /// @return the number of sources
    ///
    virtual unsigned long getSourceCount();

    /// Returns the feature count of a feature source
    /// @param srcIdx index of the source
    /// @return the feature count of the source
    ///
    virtual unsigned long getFeatureCountOfASource(unsigned long srcIdx);

    /// Returns the feature count of a feature source
    /// @param src name of the source
    /// @return the feature count of the source
    ///
    virtual unsigned long getFeatureCountOfASource(const String& src);

    /// Returns the number of the first feature of a feature source. Useful
    /// for a multiple source stream.
    /// @param srcIdx index of the source
    /// @return the number of the first feature
    ///
    virtual unsigned long getFirstFeatureIndexOfASource(unsigned long srcIdx);

    /// Returns the number of the first feature of a feature source. Useful
    /// for a multiple source stream.
    /// @param src name of the source
    /// @return the number of the first feature
    ///
    virtual unsigned long getFirstFeatureIndexOfASource(const String& srcName);

    /// Returns the a particular feature source. Useful
    /// for a multiple file stream.
    /// @param srcIdx index of the source
    /// @return the name of the source
    ///
    virtual const String& getNameOfASource(unsigned long srcIdx);

    virtual String getClassName() const;
    virtual String toString() const;

  private :

    bool                _ownInputStream;
    FeatureInputStream* _pInputStream;
    String              _serverName;

    FeatureInputStream& inputStream();
    void init();
    HistoricUsage defineHistoricUsage() const;
    unsigned long defineHistoricSize() const;
    BufferUsage   defineBufferUsage() const;
    unsigned long defineBufferSize() const;
    void releaseAll();
    FeatureServer(const FeatureServer& s); /*!Not implemented*/
    bool operator==(const FeatureServer&); /*!Not implemented*/
    bool operator!=(const FeatureServer&); /*!Not implemented*/
    const FeatureServer& operator=(
            const FeatureServer&); /*!Not implemented*/
  };

} // end namespace alize

#endif // !defined(ALIZE_FeatureServer_h)

