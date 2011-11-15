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

#if !defined(ALIZE_Feature_h)
#define ALIZE_Feature_h

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
  class Config;
  class ULongVector;

  /// This class represents a feature. A feature contains a vector of
  /// acoustic parameters and other data like :\n
  /// > a label code (speaker identifier, phoneme identifier, etc.)\n
  /// > a flag for the validity of the feature\n
  /// and in the next version of the class :\n
  /// > some end-point-detection information\n
  /// > a generic object (for example, in order to store computation
  ///   results)\n
  /// 
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API Feature : public Object
  {
    friend class TestFeature;

  public :

    typedef double data_t;

    /// Create a Feature object
    /// @param vectSize size of the acoustic parameters vector
    ///
    explicit Feature(unsigned long vectSize = 0);

    /// Create a Feature object
    /// @param c a Config object that contains the vectSize parameter
    ///
    explicit Feature(const Config& c);

    /// Create a Feature object
    /// @param vectSize size of the acoustic parameters vector
    ///
    static Feature& create(unsigned long vectSize);

    Feature(const Feature&);

    /// Copy the content of the feature f in this feature. Data
    /// copied are : the vector of parameters, the validity flag and 
    /// the label code.
    /// @exception Exception if the target vectSize dos not match the
    ///      source vectSize
    ///
    const Feature& operator=(const Feature&);

    /// Two Feature objects are equal if their dimensions are equal and
    /// their validities are equal and all their parameters (vector) are
    /// the same.
    ///
    bool operator==(const Feature&) const;

    bool operator!=(const Feature&) const;

    virtual ~Feature();

    /// Returns the size of the acoustic parameters vector
    /// @return the size of the acoustic parameters vector
    ///
    unsigned long getVectSize() const;

    /// Overloaded operator[] to access an element in the acoustic
    /// parameters vector
    /// @param index index of the element to access
    /// @return a reference to the element
    /// @exception IndexOutOfBoundsException
    ///
    data_t& operator[](unsigned long index);

    /// like the other operator[] but for constant Feature object
    ///
    data_t  operator[](unsigned long index) const;

    /// Set all the acoustic parameters to 0.0, the label to an empty
    /// string and the validity to false;
    ///
    virtual void reset();

    /// Tells whether or not this feature is valid.
    /// @return true if this feature is valid; false otherwise.
    ///
    bool isValid() const;

    /// Sets the validity of this feature.
    /// @param validity true or false.
    ///
    void setValidity(bool validity);

    /// Returns the label code
    /// @return the label code
    ///
    unsigned long getLabelCode() const;

    /// Method to set the label code
    /// @param code the value of the code.
    ///
    void setLabelCode(unsigned long code);

    /// Updates all the acoustic parameters
    /// @param v vector of values
    /// @param start index of the first value to use in the vector
    /// @exception thrown if the vector size is not equals to
    /// the feature size
    ///    
    void setData(const FloatVector& v, unsigned long start=0);

    /// Updates all the acoustic parameters
    /// @param v vector of values
    /// @param start index of the first value to use in the vector
    /// @exception thrown if the vector size is not equals to
    /// the feature size
    ///    
    void setData(const DoubleVector& v, unsigned long start=0);

    /// Copy some parameters from a feature (copy too validity and label code)
    /// <br>Gets parameters from f according to the selection s and put them
    /// into this feature
    /// @param f the feature with the parameters to select
    /// @param s an array of parameter indexes
    ///
    void copySelectedData(const Feature& f, const ULongVector& s);

    /// Use this method to access directly to the internal vector
    /// @return a pointer on the first acoustic parameter
    /// @warning Fast but dangerous ! Use preferably operator [].
    ///
    data_t* getDataVector() const;

    void setVectSize(const K&, unsigned long);

    virtual String getClassName() const;
    virtual String toString() const;


  private :

    unsigned long _vectSize;
    bool          _isValid;
    unsigned long _labelCode;
    data_t*       _dataVector;

    data_t* createVector() const;
  };

} // end namespace alize

#endif // !defined(ALIZE_Feature_h)
