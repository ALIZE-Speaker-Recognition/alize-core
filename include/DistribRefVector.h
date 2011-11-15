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

#if !defined(ALIZE_DistribRefVector_h)
#define ALIZE_DistribRefVector_h

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
#include "Distrib.h"

namespace alize
{
  /// This class implements a growable array of Distrib objects.
  /// Note that the array just store pointers on Distrib objects.
  /// If you delete the array, the Distrib objects are not deleted.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API DistribRefVector : public Object
  {
    friend class TestDistribRefVector;
  
  public:
  
    /// Create a DistribRefVector object.
    /// @param capacity maximum number of elements in the array\n
    ///    The capacity will grow automatically when the size will
    ///    overtake the capacity
    ///
    explicit DistribRefVector(unsigned long capacity = 1);

    DistribRefVector(const DistribRefVector&);

    virtual ~DistribRefVector();
  
    /// Appends a Distrib object to the end of the vector
    /// @param distrib the object to add
    /// @return the index of the added object
    ///
    unsigned long addDistrib(Distrib& distrib);
  
    /// Set a Distrib object at a specified location in the vector. Note
    /// that the previous object inside this location is not destroyed.
    /// @param d the object to store
    /// @param index an index into this vector
    /// @exception IndexOutOfBoundsException
    ///
    void setDistrib(Distrib& d, unsigned long index) const;
  
    /// Returns the Distrib object at the specified index
    /// @param index an index into this vector
    /// @return a reference to the Distrib object at the specified index
    /// @exception IndexOutOfBoundsException
    ///
    Distrib& getDistrib(unsigned long index) const;

    /// Use this method to access directly to the internal vector
    /// @return a pointer on the first element
    /// @warning Fast but dangerous ! Use preferably set and get.
    ///
    Distrib** getArray() const;

    /// Deletes all distributions which are not referenced by other
    /// objects
    ///     
    void deleteUnreferencedDistribs();

    /// Deletes a distribution if it is not referenced by other
    /// objects
    ///     
    void deleteUnreferencedDistrib(const Distrib& d);

    unsigned long size() const;

    void clear();

    virtual String getClassName() const;


  private:

    unsigned long _size;
    unsigned long _capacity;
    Distrib**     _array;

    Distrib** createArray() const;
    const DistribRefVector& operator=(const
                  DistribRefVector&); /*!Not implemented*/
    bool operator==(const DistribRefVector&) const; /*!Not implemented*/
    bool operator!=(const DistribRefVector&) const; /*!Not implemented*/
  };

} // end namespace alize

#endif // ALIZE_DistribRefVector_h
