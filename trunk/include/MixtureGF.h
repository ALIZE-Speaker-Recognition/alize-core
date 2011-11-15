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

#if !defined(ALIZE_MixtureGF_h)
#define ALIZE_MixtureGF_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include "Mixture.h"
#include "DistribGF.h"

namespace alize
{
  class MixtureStat; // TODO : garder ici ?
  class Config;

  /// Class for a mixture of gaussian distributions with full matrix
  /// of covariance (DistribGF objects).
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API MixtureGF : public Mixture
  {

  public :


    //explicit MixtureGF(const Config& config);
    MixtureGF(const MixtureGF&);
    explicit MixtureGF(const String& id, unsigned long vectSize,
                      unsigned long distribCount = 0);
    virtual bool operator==(const Mixture&) const;

    static MixtureGF& create(const K&, const String& id,
          unsigned long vectSize, unsigned long distribCount = 0);
    //static MixtureGF& create(const Config& config);

    MixtureGF& duplicate(const K&, DuplDistrib) const; // not virtual

    /// Adds a distribution GF to this mixture
    /// @param d DistribGF object to add
    /// @param w weight of the distribution
    /// @exception Exception if the dimension of the mixture is not equal
    ///      to the dimension of the distribution
    ///
    void addDistrib(const K&, DistribGF& d, weight_t w = 0.0);

    /// Adds a distribution to this mixture
    /// @param d Distrib object to add
    /// @param w weight of the distribution
    /// @exception Exception if d is not a DistribGF object
    /// @exception Exception if the dimension of d is not equals to the
    ///      dimension of the mixture
    ///
    virtual void addDistrib(const K&, Distrib& d, weight_t w = 0.0);

    /// Replace a distribution GF by an other one
    /// @param d DistribGF object to set
    /// @param index position of the distribution to be replaced
    /// @exception IndexOutOfBoundsException
    /// @exception Exception if the dimension of d is not equals to the
    ///      dimension of the mixture
    ///
    void setDistrib(const K&, DistribGF& d, unsigned long index);

    /// Replace a distribution by an other one
    /// @param d DistribGF object to set
    /// @param index position of the distribution to be replaced
    /// @exception IndexOutOfBoundsException
    /// @exception Exception if d is not a DistribGF object
    /// @exception Exception if the dimension of d is not equals to the
    ///      dimension of the mixture
    ///
    virtual void setDistrib(const K&, Distrib& d, unsigned long index);

    /// @exception Exception if the dimensions or the numbers of
    ///      distributions does not match or if the types are not
    ///      the same
    ///
    MixtureGF& operator=(const MixtureGF&);

    /// @exception Exception if the dimensions or the numbers of
    ///      distributions does not match or if the types are not
    ///      the same
    ///
    Mixture& operator=(const Mixture&);

    virtual ~MixtureGF();

    /// Returns a distribution
    /// @param index position of the distribution
    /// @return a reference to the distribution
    /// @exception IndexOutOfBoundsException
    ///    
    DistribGF& getDistrib(unsigned long index) const; // NOT virtual method

    virtual DistribType getType() const;

    virtual String getClassName() const;
    virtual String toString() const;
        

  private :

    virtual Mixture& clone(DuplDistrib) const;
    virtual MixtureStat& createNewMixtureStatObject(const K&,
                            StatServer& ss, const Config&) const;
  };

} // end namespace alize

#endif // !defined(ALIZE_MixtureGF_h)

