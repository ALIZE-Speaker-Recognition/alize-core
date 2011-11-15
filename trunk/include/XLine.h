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

#if !defined(ALIZE_XLine_h)
#define ALIZE_XLine_h

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
#include "Exception.h"

namespace alize
{
  /*!
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2004
  */

  class ALIZE_API XLine : public Object
  {
    friend class TestXLine;

  public :

    /// Create en empty line
    ///
    XLine();

    /// Create en empty line
    ///
    static XLine& create();
    /// Create a line with key / value - richard.dufour@lium.univ-lemans.fr
    ///
    static XLine& create(String& key, String& value);
    XLine(const XLine&);
    XLine& duplicate() const;
    const XLine& operator=(const XLine& c);
    bool operator==(const XLine& c) const;
    bool operator!=(const XLine& c) const;
    virtual ~XLine();

    /// Adds a new element at the end of the line. The element become
    /// the current element
    /// @return this line
    ///
    XLine& addElement(const String& e);

    /// Removes and deletes an element
    /// @param e the element to delete
    ///   
    void deleteElement(const String& e);

    /// Sets the first element to become the current element
    ///
    void rewind() const;

    /// Gets an element
    /// @return a reference to the element
    /// @param idx index of the element
    /// @param becomeCurrent if is true the element becomes the current element
    /// @exception IndexOutOfBoundsException    
    ///
    String& getElement(const unsigned long idx,
                       const bool becomeCurrent = true) const;

    /// Gets the current element. The next element become the current
    /// element
    /// @return a pointer to the element if it exists; NULL otherwise
    ///
    String* getElement() const;

    /// Gets the elements from the current element to the last one.
    /// The current element does not change.
    /// @return a reference to a constant XLine object.
    ///
    const XLine& getElements() const;
    
    /// Returns the index of an element
    /// @param e the element to search
    /// @return the index of the element if it exists; -1 otherwise
    ///
    long getIndex(const String& e) const;

    /// Gets the number of elements in the line
    /// @return  the number of elements in the line
    ///
    unsigned long getElementCount() const;

    /// Removes all elements
    ///
    void reset();

    virtual String toString() const;
    virtual String getClassName() const;

  private :

    RefVector<String>     _vector;
    mutable unsigned long _current;
    mutable XLine*        _pLine;
  };

} // end namespace alize

#endif // !defined(ALIZE_XLine_h)
