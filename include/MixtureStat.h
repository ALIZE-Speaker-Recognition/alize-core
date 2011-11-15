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

#if !defined(ALIZE_MixtureStat_h)
#define ALIZE_MixtureStat_h

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
#include "StatServer.h"
#include "RealVector.h"

namespace alize
{
  class Config;
  class Feature;
  class LKVector;

  /// Abstract class used to make calculation in a Mixture object
  /// and to store and accumulate results
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003
  ///
  class ALIZE_API MixtureStat : public Object
  {
    friend class TestMixtureGFStat;
    friend class TestMixtureGDStat;
    friend class TestMixtureStat;
  
  public :

    explicit MixtureStat(StatServer&, const Mixture&, const Config&);
    virtual ~MixtureStat();
  
    /// Returns the mixture of this accumulator
    /// @return the mixture of this accumulator
    /// 
    Mixture& getMixture() const;

    /// Resets internal variables used to store likelihood computation
    /// results between the mixture and a feature or a set of features :\n
    /// > log-likelihood between the mixture and a feature\n
    /// > accumulation of log-likelihoods\n
    /// > feature counter
    ///
    void resetLLK();
    
    /// Computes log-likelihood between a mixture and a feature
    /// @param f the feature
    /// @return the log-likelihood
    ///
    lk_t computeLLK(const Feature& f);
    
    /// Computes log-likelihood between a mixture and a parameter of a feature
    /// @param f the feature
    /// @param idx the index of the parameter of the feature
    /// @return the log-likelihood
    ///
    lk_t computeLLK(const Feature& f, unsigned long idx);

    /// Returns the last log-likelihood computation stored for the mixture
    /// @return the log-likelihood computed between the mixture and the
    ///     last feature
    ///
    lk_t getLLK() const;

        
    /// Returns the accumulated log-likelihood computed between the mixture and the
    /// last feature
    /// @return the accumulated log-likelihood computed between the mixture and the
    ///     last feature
    ///
    lk_t getAccumulatedLLK() const;
    
    /// Like computeLLK() and, in addition, accumulate the log-likelihood.
    /// The internal feature counter increases by w.
    /// @param f the feature
    /// @param w the weight of the feature
    /// @param a flag used to deal with top distributions
    /// @return the log-likelihood for this feature (not multiplied by w)
    /// @exception Exception if the dimension of the mixture is not
    ///      equals to the dimension of the feature
    ///
    lk_t computeAndAccumulateLLK(const Feature& f, double w = 1.0f,
                const TopDistribsAction& a = TOP_DISTRIBS_NO_ACTION);

    // LIUM, Sylvain
	/// Like computeLLK() and, in addition, accumulate the log-likelihood.
    /// The internal feature counter increases by w.
    /// @param f the feature
    /// @param w the weight of the feature
    /// @param topDistribsVector a top distributions vector
    /// @return the log-likelihood for this feature (not multiplied by w)
    /// @exception Exception if the dimension of the mixture is not
    ///      equals to the dimension of the feature
    ///
    lk_t computeAndAccumulateLLK(const Feature& f, const LKVector& topDistribsVector,
		double w = 1.0f);
				
    /// Like computeAndAccumulateLLK(const Feature& f...) but
    /// using the internal precalculated log-likelihood array.
    /// @return the log-likelihood value
    /// @exception Exception if no mixture server is connected to the
    ///      stat server (the log-likelihood array is not computed)
    ///
    lk_t computeAndAccumulateLLK();
    
    /// Accumulates the log-likelihood.
    /// @param llk the value to accumulate
    /// @param w the weigth (0...1)
    /// @return the same log-likelihood value (not multiplied by w)
    ///
    lk_t accumulateLLK(lk_t llk, double w = 1.0);
    
    /// Computes and returns the mean of the accumulated log-likelihood.
    /// The accumulated value is simply divided by the value of the
    /// feature counter.
    /// @return the mean of the accumulated log-likelihood
    /// @exception Exception if the mean cannot be computed
    ///      (no likelihood accumulation)
    ///
    lk_t getMeanLLK() const;

    /// Returns the count of accumulated features for llk
    /// @return the count of accumulated features for llk
    ///
    double getAccumulatedLLKFeatureCount() const;

    //
    //const DoubleVector& getDistribLKVect();


    // --------------------- OCC ----------------------

    /// Resets internal variables used to store occupation of a feature
    /// or a set of features
    ///
    void resetOcc();

    /// Returns the count of accumulated features for occupation
    /// @return the count of accumulated features
    ///
    real_t getAccumulatedOccFeatureCount() const;

    /// Gets a reference to the vector of occupations.
    /// @return a reference to the vector of occupations
    ///
    DoubleVector& getOccVect();
    const DoubleVector& getOccVect() const;

    /// Gets a reference to the vector of accumulated occupations.
    /// @return a reference to the vector of accumulated occupations
    ///
    DoubleVector& getAccumulatedOccVect();
    const DoubleVector& getAccumulatedOccVect() const;

    /// Computes and return the sum of accumulated occupations
    /// @return the sum of accumulated occupations
    ///
    occ_t getAccumulatedOcc();

    /// Computes and accumulates occupation of a feature
    /// @param f the feature
    /// @param w the weight of the feature
    /// @return ???
    ///
    real_t computeAndAccumulateOcc(const Feature& f, weight_t w = 1.0);

    /// Gets a reference to the vector of mean occupations.
    /// @return a reference to the vector of mean occupations.
    /// @exception Exception if no occ accumulated
    ///
    DoubleVector& getMeanOccVect();

    // -------------------------- EM ----------------------------

    /// Reset all internal variables used for EM computation
    ///
    virtual void resetEM() = 0;

    /// Acumulate data for EM algorithm
    /// @param f the feature (data)
    /// @exception Exception if resetEM() have not been called beforehand
    /// or something else goes wrong
    /// @return sum of occupations BEFORE normalization
    ///
    virtual occ_t computeAndAccumulateEM(const Feature& f, real_t weight = 1.0) = 0;

    virtual void addAccEM(const MixtureStat&) = 0;

    /// Gets the result of EM accumulation.
    /// @return a constant mixture.
    /// @exception Exception if resetEm() have not been called beforehand
    ///
    virtual const Mixture& getEM() = 0;

    /// Returns the count of accumulated features for EM
    /// @return the count of accumulated features for EM
    ///
    real_t getEMFeatureCount() const;

    // -----------------------------------------------------

    virtual String getClassName() const = 0;
    virtual String toString() const;

  protected :
  
    const unsigned long _distribCount;
    const Mixture*      _pMixture;
    const Config&       _config;
    const lk_t          _minLLK;
    const lk_t          _maxLLK;

    lk_t                _llk;
    lk_t                _accumulatedLLK;
    double              _featureCounterForAccumulatedLK;

    DoubleVector        _occVect;
    DoubleVector        _accumulatedOccVect;
    DoubleVector        _meanOccVect;
    real_t              _featureCounterForAccumulatedOcc;

    bool                _resetedEM;
    StatServer*         _pStatServer;
    real_t              _featureCounterForEM;

    real_t computeOccVect(const Feature&);
    void assertResetEMDone() const;

  private:
    bool operator==(const MixtureStat&) const;/*!Not implemented*/
    bool operator!=(const MixtureStat&) const;/*!Not implemented*/
    const MixtureStat& operator=(
               const MixtureStat&); /*!Not implemented*/
    MixtureStat(const MixtureStat&);   /*!Not implemented*/

  };

} // end namespace alize

#endif // !defined(ALIZE_MixtureStat_h)
