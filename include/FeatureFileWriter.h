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

#if !defined(ALIZE_FeatureFileWriter_h)
#define ALIZE_FeatureFileWriter_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include "FileWriter.h"

namespace alize
{
  class Feature;
  class Config;

  /*!
  Convenient class used to save a set of features in a file.
  In the RAW format, the dimension of the features is not saved. Each data
  of each feature is saved as a double float value (8 bytes).
  In the SPRO formats, the flags comes from the configuration.
  A raw file can be read using a FeatureFileReaderRaw object.\n
  
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2003
  */

  class ALIZE_API FeatureFileWriter : public FileWriter
  {
    friend class TestFeatureFileWriter;

  public :

    /// Create a new FeatureFileWriter object to save a set of features
    /// in a file
    /// @param f the file to write. No path
    ///    is required. It uses the parameter "featureFilesPath" of
    ///    the configuration.
    /// @param c the configuration to use
    ///
    explicit FeatureFileWriter(const FileName& f, const Config& c);

    /// See constructor with same parameters
    ///
    static FeatureFileWriter& create(const FileName& f, const Config& c);


    /// Closes the writer. A automatic call to close() is done
    /// when the writer is destroyed. But, if the file must be
    /// re-opened without destroying the writer, you MUST close
    /// the writer previously.
    /// @exception IOException if an I/O error occurs
    ///
    virtual void close();

    virtual ~FeatureFileWriter();

    /// Write a feature to the file
    /// @param feature the feature to save
    /// @exception IOException if an I/O error occurs
    /// @exception Exception if feature vectSize does not match
    ///      previous features
    ///
    virtual void writeFeature(const Feature& feature);

    virtual String getClassName() const;

  private :

    FeatureFileWriterFormat _format;
    bool                    _vectSizeDefined;
    unsigned long           _vectSize;
    unsigned long           _featureCount;
    bool                    _headerWritten; // for SPRO format
    const Config&           _config;

    String getFullFileName(const Config& c, const String& n) const;
    FeatureFileWriter(const FeatureFileWriter&);   /*!Not implemented*/
    const FeatureFileWriter& operator=(
                const FeatureFileWriter&); /*!Not implemented*/
    bool operator==(const FeatureFileWriter&) const; /*!Not implemented*/
    bool operator!=(const FeatureFileWriter&) const; /*!Not implemented*/
  };

} // end namespace alize

#endif // !defined(ALIZE_FeatureFileWriter_h)

