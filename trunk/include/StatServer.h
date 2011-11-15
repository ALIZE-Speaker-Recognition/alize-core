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

#if !defined(ALIZE_StatServer_h)
#define ALIZE_StatServer_h

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
#include "alizeString.h"
#include "LKVector.h"
#include "ViterbiAccum.h"
#include "MixtureStat.h"

#if defined(__GNUC__)
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

namespace alize
{
  class Config;
  class FrameAcc;
  class FrameAccGD;
  class FrameAccGF;
  class MixtureGDStat;
  class MixtureGFStat;
  class MixtureServer;
  class Mixture;
  class MixtureGF;
  class MixtureGD;
  class MixtureStat;

  /// This class is used to compute all the statistics needed for models
  /// training and adapting algorithms as well as for decoding algorithms.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API StatServer : public Object
  {
    friend class TestStatServer;
    friend class TestMixtureGDStat;

  public :

    /// Creates a StatServer object with a specific configuration
    /// @param c configuration of the server
    ///    The server stores a pointer to the configuration.
    ///
    explicit StatServer(const Config& c);

    /// Creates a StatServer object with a specific configuration
    /// @param c configuration of the server
    ///    The server stores a pointer to the configuration.
    /// @param ms a mixture server.
    ///
    explicit StatServer(const Config& c, MixtureServer& ms);

    virtual ~StatServer();

    /// Resets the server. Delete all temporary objects.
    ///
    void reset();
    
    /// Computes log-likelihood between a mixture and a feature
    /// @param m the mixture
    /// @param f the feature
    /// @return the log-likelihood
    ///
    lk_t computeLLK(const Mixture& m, const Feature& f) const;
    
    /// Computes log-likelihood between a mixture and a single parameter
    /// of a feature
    /// @param m the mixture
    /// @param f the feature
    /// @param idx index of the parameter of the feature
    /// @return the log-likelihood
    ///
    lk_t computeLLK(const Mixture& m, const Feature& f, unsigned long idx) const;

    /// Computes the log-likelihood between ALL the distributions of the
    /// server and the feature. The results are store in an array.\n
    /// That is useful when many distributions are shared by mixtures.
    /// The log-likelihood is just computed once for each distribution.
    /// @param f the feature
    ///
    void computeAllDistribLK(const Feature& f);

    /// Returns the best distributions index vector defined after calling
    /// computeAndAccumulateLLK(...)
    /// @return the best distributions index vector
    /// 
    const LKVector& getTopDistribIndexVector() const;

	/// Sets indexes of internal top distrib vector
    /// @param indexVect vector of indexes
    /// @param sumNonTopDistribWeights
    /// @param sumNonTopDistribLK
    ///    
    void setTopDistribIndexVector(const ULongVector& indexVect,
                                  real_t sumNonTopDistribWeights,
                                  real_t sumNonTopDistribLK);

    /// ***** DEPRECATED *****<br>
    /// Returns the count of accumulated features for occupation
    /// @param m the mixture
    /// @return the count of accumulated features
    /// @deprecated since 1.06
    ///
    DEPRECATED(real_t getAccumulatedOccFeatureCount(const Mixture& m));

    /// ***** DEPRECATED *****<br>
    /// Resets internal variables used to store likelihood computation
    /// results between a mixture and a feature or a set of features :\n
    /// > log-likelihood between the mixture and a feature\n
    /// > accumulation of log-likelihoods\n
    /// > feature counter
    /// @param m the mixture
    /// @deprecated since 1.06
    ///
    void resetLLK(const Mixture& m);
    
    /// ***** DEPRECATED *****<br>
    /// Returns the last log-likelihood computation stored for a mixture
    /// @param m the mixture
    /// @return the log-likelihood computed between the mixture and the
    ///     last feature
    /// @deprecated since 1.06
    ///
    DEPRECATED(lk_t getLLK(const Mixture& m));
    
    /// ***** DEPRECATED *****<br>
    /// Like computeLLK() and, in addition, accumulate the log-likelihood.
    /// The internal feature counter increases by 1.
    /// @param m the mixture
    /// @param f the feature
    /// @param a flag used to deal with top distributions
    /// @return the log-likelihood for this feature (not the accumulated
    ///     value)
    /// @exception Exception if the dimension of the mixture is not
    ///      equals to the dimension of the feature
    /// @deprecated since 1.06
    ///
    lk_t computeAndAccumulateLLK(const Mixture& m, const Feature& f,
                     const TopDistribsAction& a = TOP_DISTRIBS_NO_ACTION);

    /// ***** DEPRECATED *****<br>
    /// Like computeLLK() and, in addition, accumulate the log-likelihood.
    /// The internal feature counter increases by w.
    /// @param m the mixture
    /// @param f the feature
    /// @param w the weight of the feature
    /// @param a flag used to deal with top distributions
    /// @return the log-likelihood for this feature (not multiplied by w)
    /// @exception Exception if the dimension of the mixture is not
    ///      equals to the dimension of the feature
    /// @deprecated since 1.06
    ///
    lk_t computeAndAccumulateLLK(const Mixture& m, const Feature& f,
             double w, const TopDistribsAction& a = TOP_DISTRIBS_NO_ACTION);

    /// ***** DEPRECATED *****<br>
    /// Accumulates the log-likelihood.
    /// <br>Increase internal feature counter by w
    /// @param m the mixture
    /// @param l the value to accumulate
    /// @param w the weight (0...1)
    /// @deprecated since 1.06
    ///
    DEPRECATED(void accumulateLLK(const Mixture& m, double l, double w = 1.0));
    
    /// ***** DEPRECATED *****<br>
    /// Computes and returns the mean of the accumulated log-likelihood.
    /// The accumulated value is simply divided by the value of the
    /// feature counter.
    /// @param m the mixture
    /// @return the mean of the accumulated log-likelihood
    /// @exception Exception if the mean cannot be computed
    ///      (no likelihood accumulation)
    /// @deprecated since 1.06
    ///
    lk_t getMeanLLK(const Mixture& m);
    
    /// ***** DEPRECATED *****<br>
    /// Like computeAndAccumulateLLK(Mixture& mix, const Feature& f) but
    /// using the internal precalculated log-likelihood array.
    /// @param m the mixture
    /// @return the log-likelihood value
    /// @exception Exception if no mixture server is connected to this
    ///      stat server (the log-likelihood array is not computed)
    /// @deprecated since 1.06
    ///
    DEPRECATED(lk_t computeAndAccumulateLLK(const Mixture& m));

    /// ***** DEPRECATED *****<br>
    /// Resets internal variables used to store occupation of a feature
    /// or a set of features :\n
    /// > log-likelihood between the mixture and a feature\n
    /// > accumulation of log-likelihoods\n
    /// > feature counter
    /// @param m the mixture
    /// @deprecated since 1.06
    ///
    DEPRECATED(void resetOcc(const Mixture& m));

    /// ***** DEPRECATED *****<br>
    /// Computes and accumulates occupation of a feature 
    /// @param f the feature
    /// @param m the mixture
    /// @deprecated since 1.06
    /// @return sum of occupations BEFORE normalization
    ///
    DEPRECATED(real_t computeAndAccumulateOcc(const Mixture& m, const Feature& f));

    /// ***** DEPRECATED *****<br>
    /// Gets a pointer to the vector of mean occupations.
    /// @param m the mixture
    /// @return a pointer
    /// @exception Exception if no occ accumulated
    /// @deprecated since 1.06
    ///
    DEPRECATED(occ_t* getMeanOccVect(const Mixture& m));

    /// ***** DEPRECATED *****<br>
    /// Gets a pointer to the vector of accumulated occupations.
    /// @param m the mixture
    /// @return a pointer
    /// @exception Exception if no occ accumulated
    /// @deprecated since 1.06
    ///
    DEPRECATED(occ_t* getAccumulatedOccVect(const Mixture& m));

    /// ***** DEPRECATED *****<br>
    /// Computes and return the sum of accumulated occupations
    /// @param m the mixture
    /// @deprecated since 1.06
    ///
    DEPRECATED(occ_t getAccumulatedOcc(const Mixture& m));

    /// ***** DEPRECATED *****<br>
    /// Reset all internal variables used for EM computation
    /// @param m the mixture
    /// @deprecated since 1.06
    ///
    DEPRECATED(void resetEM(const Mixture& m));
  
    /// ***** DEPRECATED *****<br>
    /// Compute an iteration of EM and accumulate results.
    /// @param m the mixture
    /// @exception Exception if resetEm() have not been called beforehand
    /// @exception Exception if something else goes wrong
    /// @return sum of occupations BEFORE normalization
    ///
    DEPRECATED(occ_t computeAndAccumulateEM(const Mixture& m, const Feature&));

    /// ***** DEPRECATED *****<br>
    /// Gets the result of EM accumulation.
    /// @param m the mixture
    /// @return a constant mixture.
    /// @exception Exception if resetEm() have not been called beforehand
    /// @deprecated since 1.06
    ///
    DEPRECATED(const Mixture& getEM(const Mixture& m));

    //--------------------------------------------------------------------

    /// Creates, stores in the server and returns an accumulator of
    /// statistics for a generic mixture
    /// @param m the mixture
    /// @return the generic accumulator
    ///    
    MixtureStat& createAndStoreMixtureStat(const Mixture& m);

    /// Creates, stores in the server and returns an accumulator of
    /// statistics for a GD mixture
    /// @param m the GD mixture
    /// @return the GD accumulator
    ///    
    MixtureGDStat& createAndStoreMixtureStat(MixtureGD& m);

    /// Creates, stores in the server and returns an accumulator of
    /// statistics for a GF mixture
    /// @param m the GF mixture
    /// @return the GF accumulator
    ///    
    MixtureGFStat& createAndStoreMixtureStat(MixtureGF& m);

    /// Creates, stores in the server and returns an accumulator of
    /// statistics for a GD mixture
    /// @param m the GD mixture
    /// @return the GD accumulator
    /// @exception Exception if m type is not GD
    ///    
    MixtureGDStat& createAndStoreMixtureGDStat(Mixture& m);

    /// Creates, stores in the server and returns an accumulator of
    /// statistics for a GF mixture
    /// @param m the GF mixture
    /// @return the GF accumulator
    /// @exception Exception if m type is not GF
    ///    
    MixtureGFStat& createAndStoreMixtureGFStat(Mixture& m);

    /// Returns the count of mixture accumulators stored in the server
    /// @return the count of mixture accumulators stored in the server
    ///
    unsigned long getMixtureStatCount() const;

    /// Returns the generic mixture accumulator with index i
    /// @param idx the index
    /// @return the generic mixture accumulator with index i
    ///
    MixtureStat& getMixtureStat(unsigned long idx);

    /// Returns the mixture GD accumulator with index i
    /// @param i the index
    /// @return the mixture GD accumulator with index i
    ///
    MixtureGDStat& getMixtureGDStat(unsigned long idx);

    /// Returns the mixture GF accumulator with index i
    /// @param idx the index
    /// @return the mixture GF accumulator with index i
    ///
    MixtureGFStat& getMixtureGFStat(unsigned long idx);

    /// Deletes a mixture acumulator
    /// @param m the accumulator
    ///    
    void deleteMixtureStat(MixtureStat& m);

    /// Deletes a mixture acumulator
    /// @param begin index of the first accumulator
    /// @param end index of the last accumulator
    ///    
    void deleteMixtureStat(unsigned long begin, unsigned long end);

    /// Deletes all mixture acumulators
    ///    
    void deleteAllMixtureStat();

    /// Returns the index of a MixtureStat object
    /// @param m the MixtureStat object
    /// @return the index of a MixtureStat object
    /// @exception Exception if m cannot be found in the server
    ///
    unsigned long getMixtureStatIndex(MixtureStat& m) const;
    
    //--------------------------------------------------------------------

    /// Creates and stores a ViterbiAccum object in a pool of objects inside
    /// the server.
    ///
    ViterbiAccum& createViterbiAccum();

    //--------------------------------------------------------------------

    /// Returns a FrameAccGD object. The objet is not stored inside the server
    ///
    static FrameAccGD createFrameAccGD();

    /// Returns a FrameAccGF object. The objet is not stored inside the server
    ///
    static FrameAccGF createFrameAccGF();

    //--------------------------------------------------------------------
    
    const String& getServerName() const;

    void setServerName(const String& s);

    virtual String getClassName() const;

    virtual String toString() const;

    // INTERNAL USAGE
    
    const DoubleVector& getDistribLKVector(const K&) const;
    LKVector& getTopDistribIndexVector(const K&);
    lk_t computeLLK(const K&, const Mixture&) const;
    lk_t computeLLK(const K&, const Mixture& m, const Feature& f,
                              const TopDistribsAction& a);
	// LIUM, sylvain
    lk_t computeLLK(const K&, const Mixture& m, const Feature& f,
                              const LKVector & lkVect);							  
  private :

    String                  _serverName;
    const Config&           _config;
    DoubleVector            _distribLKVect;
    MixtureServer*          _pMixtureServer;
    RefVector<MixtureStat>  _mixtureStatVect;
    RefVector<ViterbiAccum> _viterbiAccumVect;
    const Mixture*          _pLastMixture;
    MixtureStat*            _pLastMixtureStat;
    LKVector                _topDistribsVect; // For top distributions management
    const lk_t              _minLLK;
    const lk_t              _maxLLK;

    lk_t computeLLK(lk_t lk) const;

    /// @param m
    ///
    MixtureStat& getMixtureStat(const Mixture& m); /*! internal use */
    StatServer(const StatServer&); /*!Not implemented*/
    const StatServer& operator=(const StatServer&); /*!Not implemented*/
    bool operator==(const StatServer&) const; /*!Not implemented*/
    bool operator!=(const StatServer&) const; /*!Not implemented*/
  };

} // end namespace alize

#endif // !defined(ALIZE_StatServer_h)
