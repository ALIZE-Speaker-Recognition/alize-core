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

#if !defined(ALIZE_Distrib_h)
#define ALIZE_Distrib_h

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
  class Feature;

  /// Abstract base class for all distribution classes.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API Distrib : public Object
  {
    friend class TestDistribGD;
    friend class TestDistribGF;
    friend class TestMixtureGD;
    friend class TestMixtureGF;
    friend class TestMixtureServer;
    friend class TestDistribRefVector;
    
  public :

    Distrib(unsigned long vectSize);

    /// Copy data members of a distribution in this distribution.
    /// @param d the distribution source
    /// @return this distribution as a generic distribution
    /// @exception Exception see sub-classes
    ///
    virtual const Distrib& operator=(const Distrib& d) = 0;

    /// Tests whether this distribution content is equal to
    /// the d distribution content. To know the meaning of
    /// "equals", see the same method in the derived classes.
    /// @param d the distribution to compare with this one
    ///    
    virtual bool operator==(const Distrib& d) const = 0;

    /// Tests whether this distribution content is different from
    /// the d distribution content. This method uses the method
    /// operator=().
    /// @param d the distribution to compare with this one
    ///    
    bool operator!=(const Distrib& d) const;

    virtual ~Distrib();

    Distrib& duplicate(const K&) const;

    /// Reset the distribution. This is a pure virtual method.
    /// To know the exact meaning of 'reset', look at the overriden method
    /// in the derived classes
    ///
    virtual void reset() = 0;

    /// Returns the size of the vectors/matrices
    /// @return the size of the vectors/matrices
    ///
    unsigned long getVectSize() const;

    /// get a value in the mean vector
    /// @param index position in the array
    /// @return the value of the mean
    /// @exception IndexOutOfBoundsException
    ///
    real_t getMean(unsigned long index) const;
    
    /// Returns a reference to the mean vector
    /// @return a reference to the mean vector
    /// 
    DoubleVector& getMeanVect();
    
    /// Returns a constant reference to the mean vector
    /// @return a constant reference to the mean vector
    /// 
    const DoubleVector& getMeanVect() const;

    /// Sets a value in the mean vector.
    /// @param value value to set
    /// @param index position in the array
    /// @exception IndexOutOfBoundsException
    ///
    void setMean(real_t value, unsigned long index);

    /// Sets all the values in the mean vector.
    /// @param v the vector
    /// @exception Exception if size of v is not equal to size of mean vector
    ///
    void setMeanVect(const DoubleVector& v);

    /// Compute the likelihood between this distribution and a Feature
    /// object. The algorithm is implemented in the derived classes.
    /// @return the likelihood
    ///
    virtual lk_t computeLK(const Feature&) const = 0;
    virtual lk_t computeLK(const Feature&, unsigned long idx) const = 0;

    /// Returns the constante used to compute likelihood.
    /// @return the value of the constant
    ///
    real_t getCst() const;

    /// Returns the determinant of the matrix.
    /// @return the value of the determinant
    ///
    real_t getDet() const;

    /// Computes internal data (determinant of the matrix, inverse
    /// covariance and a constante used for likelihood computation)
    ///
    virtual void computeAll() = 0;

    virtual String getClassName() const = 0;
    virtual String toString() const = 0;
 
    // internal usage

    /// Sets the determinant (internal usage)
    /// @param v value of the determinant
    ///
    void setDet(const K&, const real_t v);

    /// Sets the constante (internal usage)
    /// @param v value of the constante
    ///
    void setCst(const K&, const real_t v);

    unsigned long& dictIndex(const K&);
    unsigned long& refCounter(const K&);

    static Distrib& create(const K&, const DistribType,
                           unsigned long vectSize);
  protected:

    const unsigned long _vectSize;   /*!< dimension of the distribution */
    real_t              _det;        /*!< determinant */
    real_t              _cst;        /*!< constante */
    DoubleVector        _meanVect;   /*!< mean vector */
  private :
    unsigned long _refCounter;
    unsigned long _dictIndex;

    virtual Distrib& clone() const = 0;
  };

} // end namespace alize

#endif // !defined(ALIZE_Distrib_h)

