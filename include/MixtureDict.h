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

#if !defined(ALIZE_MixtureDict_h)
#define ALIZE_MixtureDict_h

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
#include "RefVector.h"
#include "alizeString.h"
using alize::String; // before #include <map>
#include <map>
#include "Mixture.h"

namespace alize
{
  /// This class implements a growable array of Mixture objects.
  /// It is used only in a mixture server.
  /// A method allow to find a particular mixture by the name.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003
  /// @warning Internal class. Do not use.

  class ALIZE_API MixtureDict : public Object
  {
    friend class TestMixtureDict;
  
  public:
  
    MixtureDict();
    virtual ~MixtureDict();
  
    /// @return the index of the added object
    ///
    unsigned long addMixture(Mixture&);

    /// @exception IndexOutOfBoundsException
    ///
    Mixture& getMixture(unsigned long index) const;

    /// @return the index of the mixture.  Returns -1 if not found
    ///
    long getIndexOfId(const String& id) const;

    void setMixtureId(Mixture& m, const String& id);
  
    /// Deletes a set of mixtures from the array.
    /// @param f index of the first mixture to delete
    /// @param l index of the last mixture to delete
    ///
    void deleteMixtures(unsigned long f, unsigned long l);

    /// Deletes a mixture
    /// @param m the mixture
    ///
    void deleteMixture(const Mixture& m);

    void clear();

    unsigned long size() const;

    virtual String getClassName() const;
    virtual String toString() const;

  
  private:

	std::map<String, unsigned long> _map;
    // map = conteneur associatif à clés uniques
    std::map<String, unsigned long>::iterator _it;
    RefVector<Mixture> _vect;
  
    MixtureDict(const MixtureDict&); /*!Not implemented*/
    const MixtureDict& operator=(const MixtureDict&);/*!Not implemented*/
    bool operator==(const MixtureDict&) const; /*!Not implemented*/
    bool operator!=(const MixtureDict&) const; /*!Not implemented*/
  };

} // end namespace alize

#endif // !defined(ALIZE_MixtureDict_h)

