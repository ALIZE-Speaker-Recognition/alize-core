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

#if !defined(ALIZE_MixtureFileReaderAmiral_h)
#define ALIZE_MixtureFileReaderAmiral_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include "MixtureFileReaderAbstract.h"

namespace alize
{
  class DistribGD;
  class Config;

  /// Convenient class used to read 1 mixture from a .gmm file 
  /// (AMIRAL file format)
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API MixtureFileReaderAmiral : public MixtureFileReaderAbstract
  {

  public :

    /// Creates a new MixtureFileReaderAmiral object to read a mixture
    /// from a .gmm file.
    /// @param f the file to use
    /// @param c the configuration to use
    /// @param be big endian ? Can be BIGENDIAN_AUTO, BIGENDIAN_TRUE or
    ///        BIGENDIAN_FALSE
    ///
    explicit MixtureFileReaderAmiral(const FileName& f, const Config& c,
                                             BigEndian be = BIGENDIAN_AUTO);

    /// Creates a new MixtureFileReaderAmiral object to read a mixture
    /// from a .gmm file.
    /// @param f the file to use
    /// @param c the configuration to use
    /// @param be big endian ? Can be BIGENDIAN_AUTO, BIGENDIAN_TRUE or
    ///        BIGENDIAN_FALSE
    ///
    static MixtureFileReaderAmiral& create(const FileName& f,
                            const Config& c, BigEndian be = BIGENDIAN_AUTO);

    virtual ~MixtureFileReaderAmiral();

    /// Reads the mixture
    /// @return a reference to a constant mixture
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    /// @exception IOException if an I/O error occurs
    ///
    virtual const Mixture& readMixture();

    /// Reads the mixtureGD
    /// @return a reference to a constant mixture
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    /// @exception IOException if an I/O error occurs
    ///
    virtual const MixtureGD& readMixtureGD();

    virtual String getClassName() const;

  private :

    unsigned long _vectSize;

    /// @exception IOException if an I/O error occurs

    void readVersionGD2();

    /// @exception IOException if an I/O error occurs

    void readVersion2();

    /// @exception IOException if an I/O error occurs

    void readDistribGD(DistribGD&);

    bool operator==(const MixtureFileReaderAmiral&)
                          const; /*!Not implemented*/
    bool operator!=(const MixtureFileReaderAmiral&)
                          const; /*!Not implemented*/
    const MixtureFileReaderAmiral& operator=(
            const MixtureFileReaderAmiral&); /*!Not implemented*/
    MixtureFileReaderAmiral(
            const MixtureFileReaderAmiral&); /*!Not implemented*/
  };

} // end namespace alize

#endif // !defined(ALIZE_MixtureFileReaderAmiral_h)

