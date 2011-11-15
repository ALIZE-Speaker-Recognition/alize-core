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

#if !defined(ALIZE_ULongVector_h)
#define ALIZE_ULongVector_h

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

namespace alize
{
  /// This class implements a growable array of unsigned long values.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API ULongVector : public Object
  {
    friend class TestULongVector;

  private:

    typedef unsigned long _type;

  public:
  
    /// Create a vector of type unsigned long values.
    /// @param size initial size of the array\n
    /// @param capacity maximum number of elements in the array\n
    ///    The capacity will grow automatically when the size will
    ///    overtake the capacity
    ///
    explicit ULongVector(unsigned long capacity = 0, unsigned long size = 0);
    const ULongVector& operator=(const ULongVector&);
    bool operator==(const ULongVector&) const;
    bool operator!=(const ULongVector&) const;
    ULongVector(const ULongVector&);
    virtual ~ULongVector();

    unsigned long size() const;

    void clear();
  
    /// Set a new size
    /// @param size new size
    ///
    void setSize(unsigned long size);
  
    /// Appends a real_t data to the end of the vector
    /// @param v the real_t value to add
    /// @return this vector
    ///
    ULongVector& addValue(_type v);
  
    /// Appends a real_t data vector  to the end of the vector
    /// @param vect the real_t vector to add
    ///
    void addValue(const ULongVector& vect);

    /// Overloaded operator[] to access an element in the vector.
    /// @param index index of the element to access
    /// @return a reference to the element
    /// @exception IndexOutOfBoundsException
    ///
    _type& operator[](unsigned long index);

    /// like the other operator[] but for constant ULongVector object.
    ///
    _type  operator[](unsigned long index) const;


    /// Sorts the internal vector (ascending)
    ///
    void ascendingSort() const;

    /// Removes a set of values from the array.
    /// @param f index of the first value to remove
    /// @param l index of the last value to remove
    ///
    void removeValues(unsigned long f, unsigned long l);

    /// Use this method to access directly to the internal vector
    /// @return a pointer on the first element
    /// @warning Fast but dangerous ! Use preferably operator [].
    ///
    _type* getArray() const;

    /// Sets all the values to a a particular value
    /// @param U the unsigned long value to set
    ///
    void setAllValues(unsigned long u);

    virtual String getClassName() const;
    virtual String toString() const;

	void save(const FileName& f);
	void load(const FileName& f);
  private:

    unsigned long _size;
    unsigned long _capacity;
    _type*        _array;

    _type* createArray() const;
    static int compare(const void*, const void*);
  };

} // end namespace alize

#endif  // ALIZE_ULongVector_h
