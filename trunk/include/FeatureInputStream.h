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

#if !defined(ALIZE_FeatureInputStream_h)
#define ALIZE_FeatureInputStream_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include "Object.h"
#include "FeatureFlags.h"
#include "alizeString.h"

namespace alize
{
  class Feature;
  class LabelServer;
  class Config;
  
  /*!
  Abstract class for a feature input stream. <FRANCAIS> \n
  Classe abstraite de base pour les classes dont le rôle est de fournir
  des features a la demande (methode readFeature).
  Toutes ces classes peuvent etre connectees a un serveur de label.
  Ainsi il leur est possible d'affecter un label à chaque feature.
  L'adresse du serveur de label peut etre passee lors de la contruction
  La variable _error permet aux sous-classes de passer un
  message a l'utilisateur. Par exemple : signaler la cause d'un arret
  d'envoi de donnees (fin de fichier, fin de stream...)\n
  Ce message peut etre lu grace a la methode heritee publique getError()
  
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2003
  */

  class ALIZE_API FeatureInputStream : public Object
  {

  public :

    enum Error
    {
      NO_ERROR,
      FEATURE_OUT_OF_HISTORY,
    };
    
    explicit FeatureInputStream();
    
    /// Store the address of the label server and set the internal message
    /// to "".
    /// @param p Address of the label server. Can be NULL.
    ///
    explicit FeatureInputStream(const Config& c, LabelServer* ls = NULL);

    const Config& getConfig() const;
    bool existsConfig() const;

    virtual ~FeatureInputStream();

    /// Reads a feature in the stream and move the pointer s step
    /// forward
    /// @param f the feature to store the data read
    /// @param s step (default value = 1)
    /// @return false if there is no more data because the end of the
    ///     file has been reached
    /// @exception IOException if an I/O error occurs
    ///
    virtual bool readFeature(Feature& f, unsigned long s = 1) = 0;

    /// adds a feature in the buffer is enougth memory have been allocated by 
    /// featureServerMemAlloc option
    /// @param f the feature to add in the buffer
    /// @return right if ok
    /// @exception if not enougth space
    ///
    virtual bool addFeature(const Feature& f) = 0;

    /// Writes a feature in the stream and move the pointer s step
    /// forward
    /// @param f the feature to store the data read
    /// @param s step (default value = 1)
    /// @return false if there is no more data because the end of the
    ///     file has been reached
    /// @exception IOException if an I/O error occurs
    ///
    virtual bool writeFeature(const Feature& f, unsigned long s = 1);

    virtual void reset() = 0;
    virtual void close() = 0;


    /// Returns the size of the vector inside each feature of the stream.
    /// Must be overloaded in derived classes.
    /// @return the size of the vector inside each feature of the stream
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    ///
    virtual unsigned long getVectSize() = 0;

    /// Returns the feature flags of this stream. Must be overloaded
    /// in derived classes.
    /// @return the feature flags of this stream
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    ///
    virtual const FeatureFlags& getFeatureFlags() = 0;

    /// Returns the sample rate of this stream. Must be overloaded
    /// in derived classes.
    /// @return the sample rate of this stream
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    ///
    virtual real_t getSampleRate() = 0;

    /// Returns the number of features of the stream. Must be overloaded
    /// in derived classes. Always throw Exception in this class.
    /// @return the number of features of the stream
    /// @exception IOException if an I/O error occurs
    ///
    virtual unsigned long getFeatureCount() = 0;

    /// Sets the position indicator associated with the stream to
    /// a new position. Does not throw any exception or error.
    /// @param pos new position of the indicator
    /// @param srcName source name to seek in a particular source
    ///
    virtual void seekFeature(unsigned long pos, const String& srcName = "") = 0;

    /// Returns the number of sources (files) read by the reader
    /// @return the number of sources
    ///
    virtual unsigned long getSourceCount() = 0;

    /// Returns the feature count of a feature source (a file)
    /// @param srcIdx index of the source
    /// @return the feature count of the source
    ///
    virtual unsigned long getFeatureCountOfASource(unsigned long srcIdx) = 0;

    /// Returns the feature count of a feature source (a file)
    /// @param srcName name of the source
    /// @return the feature count of the source
    ///
    virtual unsigned long getFeatureCountOfASource(const String& srcName) = 0;

    /// Returns the number of the first feature of a feature source. Useful
    /// for a multiple source stream.
    /// @param srcIdx index of the source
    /// @return the number of the first feature
    ///
    virtual unsigned long getFirstFeatureIndexOfASource(unsigned long srcIdx) = 0;

    /// Returns the number of the first feature of a feature source. Useful
    /// for a multiple source stream.
    /// @param srcName name of the source
    /// @return the number of the first feature
    ///
    virtual unsigned long getFirstFeatureIndexOfASource(const String& srcName) = 0;

    /// Returns the a particular feature source. Useful
    /// for a multiple source stream.
    /// @param srcIdx index of the source
    /// @return the name of the source
    ///
    virtual const String& getNameOfASource(unsigned long srcIdx) = 0;

    /// Gets the last feature error code from this stream
    /// @return the error.
    ///
    Error getError();

    virtual String getClassName() const = 0;

  protected :
    LabelServer*  _pLabelServer;
    Error         _error;
    bool          _seekWanted;
    unsigned long _seekWantedIdx;
    String        _seekWantedSrcName;
    bool          _featuresAreWritable;
    void init(const Config& c, LabelServer* ls = NULL);

  private :
    const Config* _pConfig;
  };

} // end namespace alize

#endif // !defined(ALIZE_FeatureInputStream_h)

