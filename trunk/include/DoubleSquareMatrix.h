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

#if !defined(ALIZE_DoubleSquareMatrix_h)
#define ALIZE_DoubleSquareMatrix_h

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
#include "RealVector.h"

namespace alize
{
  /// This class implements a symmetric matrix of real_t values.<br>
  /// Inside the object, the matrix is stored as a single-dimension
  /// array. Remember that when you call the method getArray() to
  /// access the internal array.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API DoubleSquareMatrix : public Object
  {
    friend class TestDoubleSquareMatrix;

  private:

    typedef double _type;

  public:
  
    /// Create a symmetric matrix of type double values.
    /// @param dimension of the matrix\n
    ///
    explicit DoubleSquareMatrix(unsigned long size = 0);
    const DoubleSquareMatrix& operator=(const DoubleSquareMatrix&);
    bool operator==(const DoubleSquareMatrix&) const;
    bool operator!=(const DoubleSquareMatrix&) const;
    DoubleSquareMatrix(const DoubleSquareMatrix&);
    virtual ~DoubleSquareMatrix();

    unsigned long size() const;

    /// Sets the new size of the square matrix
    /// @param size the size
    /// @param updateCapacity true = frees unused memory
    ///                       false = does not free unused memory
    ///
    void setSize(const unsigned long size,
                 const bool updateCapacity = false);

    /// Inverts the matrix and compute the determinant\n
    /// ONLY for symmetric positive definite matrix
    /// @param m to store the inverted matrix
    /// @return the determinant
    /// @exception if the matrix is not a positive definite matrix
    ///
    real_t invert(DoubleSquareMatrix& m);

    /// Sets all the values to a a particular value
    /// @param v the real_t value to set
    ///
    void setAllValues(_type v);

    /// Overloaded operator() to access an element in the matrix.<br>
    /// WARNING : contrary to class Matrix, colum index is FIRST
    /// argument and row index is SECOND argument<br>
    /// This will probably change in next release of Alize
    /// @param col column index of the element to access
    /// @param row index of the element to access
    /// @return a reference to the element
    /// @exception IndexOutOfBoundsException
    ///
    _type& operator()(unsigned long col, unsigned long row);

    /// like the other operator[] but for constant DoubleSquareMatrix object.
    ///
    _type  operator()(unsigned long col, unsigned long row) const;

    /// Use this method to access directly to the internal vector
    /// @return a pointer on the first element
    /// @warning Fast but dangerous ! Use preferably operator (col, row).
    ///
    _type* getArray() const;

    virtual String toString() const;
    virtual String getClassName() const;

    real_t upperCholesky(DoubleSquareMatrix& m);

  private:

    unsigned long _size;
    DoubleVector  _array;

    static void choleskyDecomp(real_t*, real_t*, long);
    static void choleskySolve(real_t*, real_t*, real_t*, long);
  };

} // end namespace alize

#endif  // ALIZE_DoubleSquareMatrix_h