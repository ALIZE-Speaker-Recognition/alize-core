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

#if !defined(ALIZE_DistribGD_h)
#define ALIZE_DistribGD_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include "Distrib.h"
#include "RealVector.h"

namespace alize
{
  class Config;

  /// Class for a distribution GD (gaussian with diagonal matrix)\n
  ///\n
  /// Just after creation, all the values are randomly initialized.\n\n
  /// To build a valid distribution, you have to set all the covariance and
  /// mean values and call computeAll() to compute inverse covariance
  /// values, the constante and the determinant.\n
  /// A temporary array is used to store covariance values. This array
  /// is destroyed after calling computeAll().
  /// Before calling computeAll(), the distribution is not valid for some
  /// methods.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @date 2003


  class ALIZE_API DistribGD : public Distrib
  {
    friend class TestDistribGD;
    friend class TestMixtureGD;

  public :


    /// Copy constructor
    /// @param d the distribution GD to copy.
    /// @exception Exception can be thrown if the dimension of d
    ///      does not match the dimension of this distribution
    ///    
    DistribGD(const DistribGD& d);

    /// Creates a new DistribGD object. All the values are randomly
    /// initialized.
    /// @param vectSize dimension of the distribution
    ///
    explicit DistribGD(unsigned long vectSize);

    /// Creates a new DistribGD object. All the values are randomly
    /// initialized.
    /// @param config parameters used to build the distribution
    ///
    explicit DistribGD(const Config& config);

    /// Copy data members of a distribution in this distribution.
    /// @param d the distribution source
    /// @return this distribution as a generic distribution
    /// @exception Exception if the dimension of d does not match the
    ///      dimension of this distribution or if d is not a
    ///      DistribGD object.
    ///
    virtual const Distrib& operator=(const Distrib& d);
    
    /// Copy data members of a distribution GD in this distribution
    /// @param d the distribution GD source
    /// @return this distribution
    /// @exception Exception can be thrown if the dimension of d
    ///      does not match the dimension of this distribution
    ///
    const DistribGD& operator=(const DistribGD& d);

    /// Compares the distribution d to this one.
    /// @param d the generic distribution to compare to this one.
    /// @return false if :\n
    /// > d is not a DistribGD object or\n
    /// > dimension of d does not match dimension of this distribution or\n
    /// > one of the inverse covariance values in d does not match the
    ///   corresponding value in this distribution or\n
    /// > one of the mean values in d does not match the corresponding
    ///   value in this distribution\n
    /// Otherwise return true.
    ///
    virtual bool operator==(const Distrib& d) const;

    virtual ~DistribGD();

    /// Resets the distribution (as creation)
    ///
    virtual void reset();

    /// Computes the likelihood between this distribution and a Feature
    /// object.
    /// @return the likelihood
    /// @exception Exception if the feature vectSize does not match the
    ///      distribution vectSize
    ///
    virtual lk_t computeLK(const Feature&) const;
    virtual lk_t computeLK(const Feature&, unsigned long idx) const;
	
	virtual lk_t computeLLK(const Feature&) const;    
	virtual lk_t computeLLK(const Feature&, unsigned long idx) const;

    /// Sets a value in the covariance vector.
    /// A zero value is automatically replaced by a positive-and-non-zero
    /// value near to zero.
    /// @param value value to set
    /// @param index position in the array
    /// @exception IndexOutOfBoundsException
    ///
    void setCov(real_t value, unsigned long index);

    /// Computes internal data (determinant of the matrix, inverse
    /// covariance vector and a constante used for likelihood computation)
    ///
    virtual void computeAll();

    /// Gets a value in the covariance vector.
    /// @param index position in the array
    /// @return the value of the covariance
    /// @exception IndexOutOfBoundsException
    ///
    real_t getCov(unsigned long index);
    real_t getCov(unsigned long index) const;

    /// Gets a value in the inverse covariance vector
    /// @param index position in the array
    /// @return the value of the inverse covariance
    /// @exception IndexOutOfBoundsException
    ///
    real_t getCovInv(unsigned long index) const;

    /// Returns a reference to the covariance vector. If it does not exist
    /// it is created and randomly initialized.
    /// @return a reference to the covariance vector
    ///
    DoubleVector& getCovVect();
    const DoubleVector& getCovVect() const;

    /// Returns a reference to the inverse covariance vector
    /// @return a reference to the inverse covariance vector
    ///
    DoubleVector& getCovInvVect();
    const DoubleVector& getCovInvVect() const;

    virtual String getClassName() const;
    virtual String toString() const;

    /// Sets a value in the inverse covariance vector.
    /// *** internal usage ***
    /// @param value value to set
    /// @param index position in the array
    /// @exception IndexOutOfBoundsException
    ///
    void setCovInv(const K&, real_t value, unsigned long index);

    /// Creates a new DistribGD object. Call the constructor and returns
    /// the object.
    /// @param vectSize dimension of the distribution
    /// @return the new DistribGD object
    ///
    static DistribGD& create(const K&, unsigned long vectSize);

    /// Creates a new DistribGD object. Call the constructor and returns
    /// the object.
    /// @param config parameters used to build the distribution
    /// @return the new DistribGD object
    ///
    static DistribGD& create(const K&, const Config& config);

    /// Duplicates this DistribGD Object. See copy contructor
    /// @return a reference to the copy
    ///
    DistribGD& duplicate(const K&) const;
    
  private :
    virtual Distrib& clone() const;

    mutable DoubleVector _covVect;   /*!< temporary covariance
                                          vector. The vector is cleared
                                          after calling computeAll()*/
    DoubleVector         _covInvVect; /*!< inverse covariance vector */
  };

} // end namespace alize

#endif // !defined(ALIZE_DistribGD_h)

