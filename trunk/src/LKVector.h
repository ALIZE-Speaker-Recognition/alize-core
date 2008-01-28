/*
Alize is a free, open tool for speaker recognition

Alize is a development project initiated by the ELISA consortium
  [www.lia.univ-avignon.fr/heberges/ALIZE/ELISA] and funded by the
  French Research Ministry in the framework of the
  TECHNOLANGUE program [www.technolangue.net]
  [www.technolangue.net]

The Alize project team wants to highlight the limits of voice 
  authentication in a forensic context.
  The following paper proposes a good overview of this point:
  [Bonastre J.F., Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., 
  Magrin-chagnolleau I., Person  Authentification by Voice: A Need of 
  Caution, Eurospeech 2003, Genova]
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
  the use of Alize

Copyright (C) 2003-2005
  Laboratoire d'informatique d'Avignon [www.lia.univ-avignon.fr]
  Frederic Wils [frederic.wils@lia.univ-avignon.fr]
  Jean-Francois Bonastre [jean-francois.bonastre@lia.univ-avignon.fr]
      
This file is part of Alize.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if !defined(ALIZE_LKVector_h)
#define ALIZE_LKVector_h

#include "Object.h"

namespace alize
{
  /// This class implements a growable array of a special type values
  /// Used for determine top distributions
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2004

  class LKVector : public Object
  {

  public:

    typedef struct
    {
      unsigned long idx;
      lk_t          lk;
    } type;

  
    /// Create a vector of type type values.
    /// @param size initial size of the array\n
    /// @param capacity maximum number of elements in the array\n
    ///    The capacity will grow automatically when the size will
    ///    overtake the capacity
    ///
    explicit LKVector(unsigned long capacity = 0, unsigned long size = 0);
    virtual ~LKVector();

    unsigned long size() const;

    void clear();
  
    /// Set a new size
    /// @param size new size
    ///
    void setSize(unsigned long size);
  
    /// Appends a real_t data to the end of the vector
    /// @param v the real_t value to add
    ///
    void addValue(type v);
  
    /// Overloaded operator[] to access an element in the vector.
    /// @param index index of the element to access
    /// @return a reference to the element
    /// @exception IndexOutOfBoundsException
    ///
    type& operator[](unsigned long index);
    const type& operator[](unsigned long index) const;

    /// Sorts the internal vector
    ///
    void descendingSort() const;

    /// Use this method to access directly to the internal vector
    /// @return a pointer on the first element
    /// @warning Fast but dangerous ! Use preferably operator [].
    ///
    type* getArray() const;

    virtual String getClassName() const;
    virtual String toString() const;

    // for top distribs in MixtureStat
    real_t sumNonTopDistribWeights;
    real_t sumNonTopDistribLK;
    unsigned long topDistribsCount;

  private:

    unsigned long _capacity;
    unsigned long _size;
    type*         _array;

    type* createArray() const;
    static int compare(const void*, const void*);


    const LKVector& operator=(const LKVector&);
    bool operator==(const LKVector&) const;
    bool operator!=(const LKVector&) const;
    LKVector(const LKVector&);

  };

} // end namespace alize

#endif  // ALIZE_LKVector_h
