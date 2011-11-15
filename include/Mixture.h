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

#if !defined(ALIZE_Mixture_h)
#define ALIZE_Mixture_h

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
#include "DistribRefVector.h"
#include "RealVector.h"
#include "alizeString.h"

namespace alize
{
  class MixtureStat; // TODO : garder ici ?
  class Config;
  class StatServer;
  
  /// Abstract class for a mixture of distributions.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003
  /// @warning The mixtures server is responsible for creating and deleting
  ///      mixtures. Do not create mixtures by yourself.

  class ALIZE_API Mixture : public Object
  {

  public :
   
    explicit Mixture(const String& id, unsigned long distribCount,
                     unsigned long vectSize);

    virtual bool operator==(const Mixture&) const = 0;
    bool operator!=(const Mixture&) const;

    virtual ~Mixture();

    Mixture& duplicate(const K&, DuplDistrib) const; // NOT VIRTUAL

    /// Reset the mixture : set all distribution weights to 0.0 and
    /// call the method reset() of each distribution.\n
    /// See Distrib::reset()
    ///
    virtual void reset();

    /// Copy data members of a mixture in this mixture.
    /// Does not copy the identifier.
    /// @exception Exception if the dimensions or the numbers of
    ///      distributions does not match or if the types are not
    ///      the same or other reasons
    ///
    virtual Mixture& operator=(const Mixture& m) = 0;

    /// Adds a distribution to this mixture
    /// @param d Distrib object to add
    /// @param w weight of the distribution
    ///
    virtual void addDistrib(const K&, Distrib& d, weight_t w = 0.0);

    /// Replace a distribution by an other one
    /// @param d Distrib object to add
    /// @param index position of the distribution to be replaced
    /// @exception IndexOutOfBoundsException
    ///
    virtual void setDistrib(const K&, Distrib& d, unsigned long index);

    /// Returns a distribution
    // Is NOT a virtual method
    /// @param index position of the distribution
    /// @return a reference to the distribution
    /// @exception IndexOutOfBoundsException
    ///
    Distrib& getDistrib(unsigned long index) const;

    /// Returns a reference to the weight of a distribution
    /// @param index position of the distribution
    /// @return a reference to the weight
    /// @exception IndexOutOfBoundsException
    ///
    weight_t& weight(unsigned long index);

    /// Returns the weight of a distribution
    /// @param index position of the distribution
    /// @return the weight
    /// @exception IndexOutOfBoundsException
    ///
    weight_t weight(unsigned long index) const;

    /// Returns the identifier of the mixture
    /// @return a string that identify the mixture
    ///
    String getId() const;

    /// Returns the dimension of the distributions
    /// @return the dimension of the distributions
    ///
    unsigned long getVectSize() const;

    /// Saves a mixture to a file
    /// @param n the name of the file
    /// @param c the configuration to use
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    ///
    void save(const String& n, const Config& c) const;

    /// Returns the number of distribution
    /// @return the number of distribution
    ///
    unsigned long getDistribCount() const;

    /// Set the weights of all distributions to the same value.
    /// Sum of the weights equals 1.
    ///
    void equalizeWeights();

    /// Computes distributions internal data (determinant of the matrix,
    /// inverse covariance and a constante used for likelihood computation)
    ///
    void computeAll();

    /// Returns a reference to the weight vector
    /// @return a reference to the weight vector
    ///
    DoubleVector& getTabWeight();
    const DoubleVector& getTabWeight() const;

    /// Returns a pointer to the vector of distribution pointers
    /// @return a pointer to the first value of the internal vector
    /// @warning Fast but dangerous ! Use preferably getDistrib() and
    ///      setDistrib()
    ///
    Distrib** getTabDistrib() const;

    /// Sets the identifier of the mixture. Internal usage
    /// @param id the identifier
    ///
    void setId(const K&, const String& id);
    void setId(const String& id);

    virtual DistribType getType() const = 0;

    /// Internal usage
    ///
    virtual MixtureStat& createNewMixtureStatObject(const K&,
                           StatServer& ss, const Config&) const = 0;
    void removeAllDistrib(const K&);

    virtual String toString() const;

    virtual String getClassName() const = 0;
    static Mixture& create(const K&, const unsigned long dc,
                          const DistribType, const String& id,
                          const unsigned long vectSize);

  protected :

    // Does not copy the identifier.
    // Exception is thrown if the dimensions or the numbers of
    // distributions does not match or if the types are not
    // the same or other reasons
    //
    virtual void assign(const Mixture&);

    unsigned long _vectSize; /*! dimension of the distributions */
    DoubleVector   _weightVect;  // a vector for weights
    DistribRefVector _distribVect; // a vector for distributions
    String       _id;      // identifier of the mixture
    
    virtual Mixture& clone(DuplDistrib) const = 0;
  };

} // end namespace alize

#endif // !defined(ALIZE_Mixture_h)
