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

#if !defined(ALIZE_FeatureInputStreamModifier_h)
#define ALIZE_FeatureInputStreamModifier_h

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
#include "alizeString.h"
#include "Feature.h"
#include "ULongVector.h"

namespace alize
{
  /*!
  <FRANCAIS>Cette classe représente un flux de features sur lequel il est
  possible d'agir de différentes façons :<br>
  - sélection des parametres acoustiques à l'aide d'un masque<br>
  - autres... TODO<br>

  TODO : to complete...

  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2004
  */

  class ALIZE_API FeatureInputStreamModifier : public FeatureInputStream
  {
  
    friend class TestFeatureInputStreamModifier;

  public :

    /// Build the object
    /// @param m the mask
    /// @param is the input feature stream
    ///
    FeatureInputStreamModifier(FeatureInputStream& is,
                     const String& m = "NO_MASK", bool ownStream = false);
    static FeatureInputStreamModifier& create(FeatureInputStream& is,
                     const String& m = "NO_MASK", bool ownStream = false);

    /// Defines the mask to select acoustic parameters<br>
    /// Examples :<br>
    /// "0" : select parameter #0<br>
    /// "1-35" : select parameters #1 to #35<br>
    /// "0,1-3,6,7" : select parameters #0 and #1 to #3 and #6 and #7<br>
    /// Characters allowed : "0" to "9", "," and "-".<br>
    /// To remove the mask, set m to "NO_MASK"
    /// @exception Exception if the mask is invalid
    ///
    void setMask(const String& m);
    
    virtual bool addFeature(const Feature& f);

    virtual bool readFeature(Feature& f, unsigned long step = 1);

    virtual bool writeFeature(const Feature& f, unsigned long step = 1);

    /// Returns the number of features in the file.
    /// @return the number of features in the file
    /// @exception IOException if an I/O error occurs
    /// 
    virtual unsigned long getFeatureCount();

    /// Returns the size of the vector inside each feature of the stream.
    /// @return the size of the vector inside each feature of the stream
    /// @exception IOException if an I/O error occurs
    ///
    virtual unsigned long getVectSize();

    /// Returns the feature flags of this stream.
    /// @return the feature flags of this stream
    /// @exception IOException if an I/O error occurs
    ///
    virtual const FeatureFlags& getFeatureFlags();

    /// Returns the sample rate of this stream.
    /// @return the sample rate of this stream
    /// @exception IOException if an I/O error occurs
    ///
    virtual real_t getSampleRate();

    /// Resets the input stream
    ///
    virtual void reset();

    /// Closes the input stream
    ///
    virtual void close();

    /// Returns the number of sources (files) read by the reader
    /// @return the number of sources
    ///
    virtual unsigned long getSourceCount();

    /// Returns the feature count of a feature source (a file)
    /// @param srcIdx index of the source
    /// @return the feature count of the source
    ///
    virtual unsigned long getFeatureCountOfASource(unsigned long srcIdx);

    /// Returns the feature count of a feature source (a file)
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
    /// for a multiple source stream.
    /// @param srcIdx index of the source
    /// @return the name of the source
    ///
    virtual const String& getNameOfASource(unsigned long srcIdx);

    virtual void seekFeature(unsigned long featureNbr,
                             const String& srcName);

    virtual ~FeatureInputStreamModifier();

    virtual String getClassName() const;
    virtual String toString() const;

  private:

    FeatureInputStream* _pInput;
    Feature             _feature;
    String              _mask;
    String              _tmpMask;
    ULongVector         _selection;
    unsigned long       _selectionSize;
    bool                _useMask;
    bool                _ownStream;

    void updateMask(const String& begin, const String& end);
  };

} // end namespace alize

#endif // !defined(ALIZE_FeatureInputStreamModifier_h)

