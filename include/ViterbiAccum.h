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

#if !defined(ALIZE_ViterbiAccum_h)
#define ALIZE_ViterbiAccum_h

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
#include "RefVector.h"
#include "ULongVector.h"
#include "RealVector.h"
#include "FeatureServer.h"
#include <iostream>


namespace alize
{
    class Mixture;
    class Config;
    class Feature;
    class StatServer;

    /*!
    Class used to determine the Viterbi path.
    Only the Stat server can create this king of object

    <FRANCAIS>Le modele ergodique est sans constrainte : toutes les transitions
    d'un etat vers l'autre sont possibles
        
    @author Frederic Wils    frederic.wils@lia.univ-avignon.fr
    @version 1.0
    @date 2004
    */

    class ALIZE_API ViterbiAccum : public Object
    {
        friend class TestViterbiAccum;

    public :


        /// Adds a state (a mixture)
        /// @param m the mixture to add
        ///
        void addState(Mixture& m);

        /// Sets or gets a log-probability transition between two states
        /// @param i1 index of the first state
        /// @param i2 index of the second state
        /// @return a reference to the value of the log-probability
        /// @exception IndexOutOfBoundException
        ///
        real_t& logTransition(unsigned long i1, unsigned long i2);
        real_t logTransition(unsigned long i1, unsigned long i2) const;

        /// Returns the state (Mixture object) of index i
        /// @param i index
        /// @return a reference to the Mixture object
        /// @exception IndexOutOfBoundException
        ///
        Mixture& getState(unsigned long i) const;

        /// Returns the number of states
        /// @return the number of states
        ///
        unsigned long getStateCount() const;

        // -----------
        // Computation
        // -----------

        /// Resets the accumulators
        ///
        void reset();

        /// Updates the internal accumulators with the feature
        /// @param f the feature
        /// @param llkW new parameter : contact
        ///             corinne.fredouille@lia.univ-avignon.fr
        ///
	      void computeAndAccumulate(const Feature& f, double llkW = 0);
        
        /// New method from Corinne Fredouille
        /// (corinne.fredouille@lia.univ-avignon.fr)
        ///
	      void computeAndAccumulate(FeatureServer& fs,
                     unsigned long start, unsigned long count, double fudge);
        
        /// New method from Corinne Fredouille
        /// (corinne.fredouille@lia.univ-avignon.fr)
        ///
        void computeAndAccumulate(FeatureServer& fs, DoubleVector& llk,
                                   unsigned long start, unsigned long count);
        
        /// New method from Corinne Fredouille
        /// (corinne.fredouille@lia.univ-avignon.fr)
        ///
        void computeAndAccumulate(const Feature& f, double fudge,
                                                            double penality);

        /// Returns the number of features accumulated
        ///
        unsigned long getFeatureCount() const;

        // -------
        // Results
        // -------
        
        /// Computes and returns the Viterbi path
        /// @return the path (a constant reference to an ULongVector object)
        ///
        const ULongVector& getPath();

        /// Returns the maximum log-probability value. You must call
        /// getPath() beforehand to compute the value
        /// @exception Exception if the value is not computed 
        ///
        real_t getLlp() const;

        virtual String getClassName() const;
        virtual String toString() const;

        // inaccessible for user
        static ViterbiAccum& create(StatServer&, const Config&, const K&);
        virtual ~ViterbiAccum();

    private :

        const Config*      _pConfig;

        RefVector<Mixture> _stateVect;
        DoubleVector       _transMatrix;

        DoubleVector       _llpVect;
        DoubleVector       _tmpLLKVect;
        DoubleVector       _tmpllpVect;
        ULongVector        _tmpTab;
        unsigned long      _featureCount;

        ULongVector        _path;
        bool               _pathDefined;
        real_t             _llp;
        bool               _llpDefined;
        StatServer*        _pStatServer;

        lk_t computeStateLLK(unsigned long stateIndex, const Feature&) const;
        ViterbiAccum(StatServer&, const Config&);
        ViterbiAccum(const ViterbiAccum&);            /*! not implemented */
        const ViterbiAccum& operator=(const ViterbiAccum& c);
                                                      /*! not implemented */
        bool operator==(const ViterbiAccum& c) const; /*! not implemented */
        bool operator!=(const ViterbiAccum& c) const; /*! not implemented */
    };   

} // end namespace alize

#endif // !defined(ALIZE_ViterbiAccum_h)
