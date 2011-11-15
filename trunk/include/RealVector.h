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

#if !defined(ALIZE_RealVector_h)
#define ALIZE_RealVector_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include <new>
#include <math.h>
#include <memory.h>
#include <cstdlib>
#include "alizeString.h"
#include "Exception.h"

namespace alize
{
  /// This class implements a growable array of real (float/double) values.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  template <class T> class ALIZE_API RealVector : public Object
  {
	  ;

    friend class TestDoubleVector;
    friend class TestFloatVector;
    friend class TestRealVector;

  public:
  
    /// Create a vector of type double values.
    /// @param size initial size of the array\n
    /// @param capacity maximum number of elements in the array\n
    ///    The capacity will grow automatically when the size will
    ///    overtake the capacity
    ///
    explicit RealVector(unsigned long capacity = 0, unsigned long size = 0)
    :Object(), _size(0), _capacity(capacity!=0?capacity:1), 
    _array(createArray())
    {
      setSize(size);
    }

    RealVector(const RealVector<T>& v)
    :Object(), _size(v._size), _capacity(v._size!=0?v._size:1),
    _array(createArray())
    {
      memcpy(_array, v._array, _size*sizeof(_array[0]));
    }

    static RealVector<T>& create(unsigned long capacity = 0,
      unsigned long size = 0)
    {
      RealVector<T>* p = new (std::nothrow) RealVector<T>(capacity, size);
      assertMemoryIsAllocated(p, __FILE__, __LINE__);
      return *p;
    }

    const RealVector<T>& operator=(const RealVector<T>& v)
    {
      if (this->isSameObject(v))
        return v;
      assert(_array != NULL);
      _size = v._size;
      if (_capacity < _size)
      {
        delete[] _array;
        _capacity = _size!=0?_size:1;
        _array = createArray();
      }
      memcpy(_array, v._array, _size*sizeof(_array[0]));
      return *this;
    }

    const RealVector<T>& operator+=(const RealVector<T>& v)
    {
      if (_size != v._size)
        throw Exception("Mismatch vector sizes", __FILE__, __LINE__);
      for (unsigned long i=0; i<_size; i++)
        _array[i] += v._array[i];
      return *this;
    }
    const RealVector<T>& operator-=(const RealVector<T>& v)
    {
      if (_size != v._size)
        throw Exception("Mismatch vector sizes", __FILE__, __LINE__);
      for (unsigned long i=0; i<_size; i++)
        _array[i] -= v._array[i];
      return *this;
    }

    bool operator==(const RealVector<T>& v) const
    {
      if (_size != v._size)
        return false;
      for (unsigned long i=0; i<_size; i++)
      {
        if (_array[i] != v._array[i])
          return false;
      }
      return true;
    }

    bool operator!=(const RealVector<T>& v) const
    {
      return !(*this == v);
    }

    virtual ~RealVector()
    {
      delete[] _array;
    }

    unsigned long size() const
    {
      return _size;
    }

    void clear()
    {
      _size = 0;
    }
  
    /// Set a new size. If updateCapacity is set to true, update the
    /// capacity of the vector (useful to save memory)
    /// @param size new size
    /// @param updateMemory 
    ///
    void setSize(const unsigned long size,
                 const bool updateCapacity = false)
    {
      assert(_array != NULL);
      if ((size > _capacity) || (size < _capacity && updateCapacity))
      {
        unsigned long oldSize = _size;
        _size = size;
        _capacity = _size;
        if (_capacity == 0)
          _capacity = 1;
        T* oldArray = _array;
        _array = createArray(); // can throw OutOfMemoryException
        memcpy(_array, oldArray, (size>oldSize?oldSize:size)*sizeof(_array[0]));
        delete[] oldArray;
        //for (unsigned long i=oldSize; i<_size; i++)
        //  _array[i] = 0.0;
      }
      else
        _size = size;
    }

    /// Appends a real data to the end of the vector
    /// @param v the real value to add
    ///
    void addValue(T v)
    {
      assert(_array != NULL);
      if (_size == _capacity)
      {
        _capacity += _capacity;
        T* oldArray = _array;
        _array = createArray(); // can throw OutOfMemoryException
        memcpy(_array, oldArray, _size*sizeof(_array[0]));
        delete[] oldArray;
      }
      _array[_size] = v;
      _size++;
    }
  
    /// Appends a real data vector  to the end of the vector
    /// @param v the real vector to add
    ///
    void addValue(const RealVector<T>& v)
    {
      for (unsigned long i=0; i<v._size; i++)
      {
        addValue(v._array[i]);
      }
      // TODO : methode a optimiser
    }

    /// Sets all the values
    /// @param v the vector that contains the values
    /// @exception Exception if vector sizes are different
    ///
    void setValues(const RealVector<T>& v)
    {
      if (_size != v._size)
      throw Exception("Cannot set values : vector size mismatch ",
                       __FILE__, __LINE__);
      memcpy(_array, v._array, _size*sizeof(_array[0]));
    }
  
    /// Sets all the values to a a particular value
    /// @param v the real value to set
    ///
    void setAllValues(T v)
    {
      for (unsigned long i=0; i< _size; i++)
        _array[i] = v;
    }

    /// Computes and returns the sum of all values
    /// @return the sum of all values
    ///
    T computeSum() const
    {
       T s = 0.0;
      for (unsigned long i=0; i<_size; i++)
        s += _array[i];
      return s;
    }

    /// Multiplies each value by a scalar
    /// @param s a scalar
    /// @return this vector
    ///
    const RealVector<T>& operator*=(double s)
    {
      for (unsigned long i=0; i<_size; i++)
        _array[i] *= (T)s;
       return *this;
    }
    
    /// Overloaded operator[] to access an element in the vector.
    /// @param i index of the element to access
    /// @return a reference to the element
    /// @exception IndexOutOfBoundsException
    ///
    T& operator[](unsigned long i)
    {
      assertIsInBounds(__FILE__, __LINE__, i, _size);
      assert(_array != NULL);
      return _array[i];
    }

    /// like the other operator[] but for constant RealVector object.
    ///
    T  operator[](unsigned long i) const
    {
      assertIsInBounds(__FILE__, __LINE__, i, _size);
      assert(_array != NULL);
      return _array[i];
    }

    /// Sorts the internal vector (ascending)
    ///
    void ascendingSort() const
    {
      assert(_array != NULL);
      qsort(_array, _size, sizeof(T), compare);
    }

    /// Returns the index of largest value in the vector
    /// @return the index of the largest value
    /// @exception Exception if the vector is empty
    ///
    unsigned long getIndexOfLargestValue() //const
    {
      if (_size == 0)
        throw Exception("Empty vector : cannot find the largest value", __FILE__, __LINE__);
      unsigned long i, maxI = 0;
      T v = _array[maxI];

      for (i=1; i<_size; i++)
        if (_array[i] > v)
        {
          maxI = i;
          v = _array[maxI];
        }
      return maxI;
    }

    /// Use this method to access directly to the internal vector
    /// @return a pointer on the first element
    /// @warning Fast but dangerous ! Use preferably operator [].
    ///
    T* getArray() const
    {
      return _array;
    }

    virtual String getClassName() const
    {
      return "RealVector";
    }

    virtual String toString() const
    {
      String s = Object::toString()
        + "\n  size = " + String::valueOf(size());
      for (unsigned long i=0; i<size(); i++)
        s += "\n  value[" + String::valueOf(i) + "] = "
          + String::valueOf(operator[](i)); 
      return s;
    }

  private:

    unsigned long _size;
    unsigned long _capacity;
    T*            _array;

    T* createArray() const
    {
      assert(_capacity != 0);
      T* p = new (std::nothrow) T[_capacity];
      assertMemoryIsAllocated(p, __FILE__, __LINE__);
      return p;
    }
    static int compare(const void* s1, const void* s2)
    {
      if (*((T*)s1) > *((T*)s2))
        return 1;
      if (*((T*)s1) < *((T*)s2))
        return -1;
      return 0;
    }

	
  };

  typedef RealVector<double> DoubleVector;
  typedef RealVector<float> FloatVector;

#if defined(_WIN32)
  template class RealVector<double>;
  template class RealVector<float>;
#endif

} // end namespace alize

#endif  // ALIZE_RealVector_h
