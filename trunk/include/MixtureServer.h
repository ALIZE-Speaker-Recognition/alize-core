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

#if !defined(ALIZE_MixtureServer_h)
#define ALIZE_MixtureServer_h

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
#include "MixtureDict.h"
#include "MixtureGD.h"
#include "MixtureGF.h"
#include "DistribGD.h"
#include "DistribGF.h"
#include "DistribRefVector.h"
#include "Config.h"

namespace alize
{
  class XLine;

  /// Class used to store and manage Mixture and Distrib objects.
  /// This class is responsible for creating and deleting these objects.
  /// Both mixtures and distributions can be accessed by an index.
  /// Mixtures can share the same distributions.
  /// In addition, mixtures can be found using the identifier.
  /// Each mixture identifier is unique.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API MixtureServer : public Object
  {
    friend class TestMixtureServer;

  public :

    /// Creates a MixtureServer object
    /// @param config parameters to build the server.
    ///
    explicit MixtureServer(const Config& config);

    /// Creates a MixtureServer object and load data from a file
    /// @param f name of the file. 
    /// @param c configuration
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    ///
    explicit MixtureServer(const FileName& f, const Config& c);

    virtual ~MixtureServer();

    /// Deletes all mixtures and distributions inside the server.
    ///
    void reset();

    /// Creates a new distribution. The type and other parameters are
    /// specified in the configuration of the server
    /// @return a reference to the distribution
    ///
    Distrib& createDistrib();

    /// Creates a new distribution
    /// @param type distribution type
    /// @param vectSize
    /// @return a reference to the distribution
    ///
    Distrib& createDistrib(const DistribType type, unsigned long vectSize);

    /// Duplicates an existing distribution. The new one is added
    /// to the internal dictionnary
    /// @param d the distribution to copy
    /// @return a reference to the new distribution
    ///
    Distrib& duplicateDistrib(const Distrib& d);

    /// Creates a new mixture inside the server. The type  and the number
    /// of distributions must be defined in the configuration of the
    /// server. A default identifier "#n" is set where n is a number.
    /// @return a reference to the mixture
    ///
    Mixture& createMixture();

    /// Creates a new mixture inside the server. The type must be defined
    /// in the configuration of the server. A default identifier "#n" is
    /// set where n is a number.
    /// @param dc the number of distribution to create for this mixture.
    /// @return a reference to the mixture
    ///
    Mixture& createMixture(unsigned long dc);

    /// Creates a new mixture inside the server. A default identifier
    /// "#n" is set where n is a number.
    /// @param dc the number of distribution to create for this mixture.
    /// @param type distribution type
    /// @return a reference to the mixture
    ///
    Mixture& createMixture(unsigned long dc, DistribType);

    /// Duplicates inside the server an existing mixture. Distributions can
    /// be duplicated too. If they are not, the new mixture and the
    /// original one share the same distributions.
    /// The new mixture identifier is equal to the original mixture
    /// identifier followed by " #n" where n is a number
    /// @param mix the mixture to duplicate.
    /// @param duplicateDistrib DUPL_DISTRIB to duplicate the
    ///    distributions; SHARE_DISTRIB to share the
    ///    existing distributions. Default = DUPL_DISTRIB.
    /// @return a reference to the new mixture
    ///
    Mixture& duplicateMixture(const Mixture& mix,
                              DuplDistrib = DUPL_DISTRIB);

    /// Gets a distribution using its index 
    /// @param index the index
    /// @return a reference to the distribution
    /// @exception IndexOutOfBoundsException
    ///
    Distrib& getDistrib(unsigned long index) const;

    /// Gets a mixture using its index 
    /// @param index the index
    /// @return a reference to the mixture
    /// @exception IndexOutOfBoundsException
    ///
    Mixture& getMixture(unsigned long index) const;

    /// Assigns a distribution to a mixture instead of an other
    /// distribution. The number of distributions is not changed in the
    /// mixture.
    /// @param mix the mixture
    /// @param distr the distribution to set in the mixture
    /// @param w weight of the distribution
    /// @param index index of the distribution inside the mixture
    /// @exception IndexOutOfBoundsException
    ///
    void setDistribToMixture(Mixture& mix, Distrib& distr, weight_t w,
                            unsigned long index);

    /// Adds a distribution to a mixture. The number of distributions
    /// increases by 1 for the mixture.
    /// @param mix the mixture
    /// @param distr the distribution to add
    /// @param w weight of the distribution
    ///
    void addDistribToMixture(Mixture& mix, Distrib& distr, weight_t w = 0.0);

    /// Deletes a set of mixtures.
    /// @warning Does not delete associated distributions. Use method
    ///      deleteUnusedDistribs()
    /// @param f index of the first mixture
    /// @param l index of the last mixture
    ///
    void deleteMixtures(unsigned long f, unsigned long l);

    /// Deletes a mixture
    /// @warning Does not delete associated distributions. Use method
    ///      deleteUnusedDistribs()
    /// @param m the mixture
    ///
    void deleteMixture(const Mixture& m);

    /// Removes all unused distributions (unreferenced by a mixture)
    ///
    void deleteUnusedDistribs();

    //-------------------------------------------------------------------
    
    /// Creates a new distribution GD and adds it to the internal 
    /// dictionnary of the server
    /// @return a reference to the distribution
    ///
    DistribGD& createDistribGD();
    
    /// Creates a new distribution GF and adds it to the internal 
    /// dictionnary of the server
    /// @return a reference to the distribution
    ///
    DistribGF& createDistribGF();

    /// Duplicates an existing distribution GD. The new one is added
    /// to the internal dictionnary
    /// @param d the distribution to copy
    /// @return a reference to the new distribution GD
    ///
    DistribGD& duplicateDistrib(const DistribGD& d);

    /// Duplicates an existing distribution GF. The new one is added
    /// to the internal dictionnary
    /// @param d the distribution to copy
    /// @return a reference to the new distribution GF
    ///
    DistribGF& duplicateDistrib(const DistribGF& d);

    /// Creates a new mixture GD inside the server. The number of
    /// distributions must be specified in the configuration of the server.
    /// A default identifier "#n" is set where n is a number.
    /// @return a reference to the mixture GD
    /// @exception IdAlreadyExistsException
    ///
    MixtureGD& createMixtureGD();

    /// Creates a new mixture GF inside the server. The number of
    /// distributions must be specified in the configuration of the server.
    /// A default identifier "#n" is set where n is a number.
    /// @return a reference to the mixture GF
    /// @exception IdAlreadyExistsException
    ///
    MixtureGF& createMixtureGF();

    /// Creates a new mixture GD inside the server. The type must be defined
    /// in the configuration of the server. A default identifier "#n" is
    /// set where n is a number.
    /// @param dc the number of distribution to create for this mixture.
    /// @return a reference to the mixture GD
    ///
    MixtureGD& createMixtureGD(unsigned long dc);

    /// Creates a new mixture GF inside the server. The type must be defined
    /// in the configuration of the server. A default identifier "#n" is
    /// set where n is a number.
    /// @param dc the number of distribution to create for this mixture.
    /// @return a reference to the mixture GF
    ///
    MixtureGF& createMixtureGF(unsigned long dc);

    /// Duplicates inside the server an existing mixture GD. Distributions
    /// can be duplicated too. If they are not, the new mixture and the
    /// original one share the same distributions.
    /// The new mixture identifier is equal to the original mixture
    /// identifier followed by "#n" where n is a number
    /// @param mix the mixture GD to duplicate.
    /// @param duplicateDistrib DUPL_DISTRIB to duplicate the
    ///    distributions; SHARE_DISTRIB to share the existing
    ///    distributions. Default = DUPL_DISTRIB.
    /// @return a reference to the new mixture GD
    ///
    MixtureGD& duplicateMixture(const MixtureGD& mix,
                                DuplDistrib = DUPL_DISTRIB);

    /// Duplicates inside the server an existing mixture GF. Distributions
    /// can be duplicated too. If they are not, the new mixture and the
    /// original one share the same distributions.
    /// The new mixture identifier is equal to the original mixture
    /// identifier followed by "#n" where n is a number
    /// @param mix the mixture GF to duplicate.
    /// @param duplicateDistrib DUPL_DISTRIB to duplicate the
    ///    distributions; SHARE_DISTRIB to share the existing
    ///    distributions. Default = DUPL_DISTRIB.
    /// @return a reference to the new mixture GF
    ///
    MixtureGF& duplicateMixture(const MixtureGF& mix,
                  DuplDistrib = DUPL_DISTRIB);

    /// Duplicates inside the server an existing mixture GD. Distributions
    /// can be duplicated too. If they are not, the new mixture and the
    /// original one share the same distributions.
    /// The new mixture identifier is equal to the original mixture
    /// identifier followed by "#n" where n is a number
    /// @param mix the mixture GD to duplicate.
    /// @param duplicateDistrib DUPL_DISTRIB to duplicate the
    ///    distributions; SHARE_DISTRIB to share the existing
    ///    distributions. Default = DUPL_DISTRIB.
    /// @return a reference to the new mixture GD
    ///
    MixtureGD& duplicateMixtureGD(const Mixture& mix,
                   DuplDistrib = DUPL_DISTRIB);

    /// Duplicates inside the server an existing mixture GF. Distributions
    /// can be duplicated too. If they are not, the new mixture and the
    /// original one share the same distributions.
    /// The new mixture identifier is equal to the original mixture
    /// identifier followed by "#n" where n is a number
    /// @param mix the mixture GF to duplicate.
    /// @param duplicateDistrib DUPL_DISTRIB to duplicate the
    ///    distributions; SHARE_DISTRIB to share the existing
    ///    distributions. Default = DUPL_DISTRIB.
    /// @return a reference to the new mixture GF
    ///
    MixtureGF& duplicateMixtureGF(const Mixture& mix,
                   DuplDistrib = DUPL_DISTRIB);

    /// Gets a distribution GD using its index 
    /// @param index the index
    /// @return a reference to the distribution
    /// @exception IndexOutOfBoundsException
    /// @exception Exception if a distrib was found but is not a distrib GD
    ///
    DistribGD& getDistribGD(unsigned long index) const;

    /// Gets a distribution GF using its index 
    /// @param index the index
    /// @return a reference to the distribution
    /// @exception IndexOutOfBoundsException
    /// @exception Exception if a distrib was found but is not a distrib GF
    ///
    DistribGF& getDistribGF(unsigned long index) const;

    /// Gets a mixture GD using its index 
    /// @param index the index
    /// @return a reference to the mixture
    /// @exception IndexOutOfBoundsException
    /// @exception Exception if a mixture was found but is not a mixture GD
    ///
    MixtureGD& getMixtureGD(unsigned long index) const;

    /// Gets a mixture GF using its index 
    /// @param index the index
    /// @return a reference to the mixture
    /// @exception IndexOutOfBoundsException
    /// @exception Exception if a mixture was found but is not a mixture GF
    ///
    MixtureGF& getMixtureGF(unsigned long index) const;

    //-------------------------------------------------------------------

    /// Creates a new mixture in the server and loads data from a file
    /// @param f the mixture file to read
    /// @return a reference to the mixture
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    ///
    Mixture& loadMixture(const FileName& f);

    /// Creates a new mixtureGD in the server and loads data from a file
    /// @param f the mixture file to read
    /// @return a reference to the mixture
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    ///
    MixtureGD& loadMixtureGD(const FileName& f);

    /// Creates a new mixtureGF in the server and loads data from a file
    /// @param f the mixture file to read
    /// @return a reference to the mixture
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    ///
    MixtureGF& loadMixtureGF(const FileName& f);

    /// Loads data from a mixture file into an existing mixture
    /// @param f the file to read
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    ///
    void loadMixture(Mixture& m, const FileName& f);

    /// Loads mixtures from a list of single mixture file
    /// @param l the list of mixture file to read
    /// @return the index of the first mixture loaded in the server
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    ///
    unsigned long loadMixture(const XLine& l);

    //-------------------------------------------------------------------
    
    /// Returns the number of distributions stored inside the server
    /// @return the number of distributions stored inside the server
    ///
    unsigned long getDistribCount() const;
    
    /// Returns the vectSize value
    /// @return the vectSize value
    /// @exception Exception if neither mixture nor distribution is stored
    ///      (no vectSize defined)
    ///
    unsigned long getVectSize() const;

    /// Returns the number of mixtures stored inside the server
    /// @return the number of mixtures stored inside the server
    ///
    unsigned long getMixtureCount() const;

    /// Tests whether a mixture with a particular identifier exists inside
    /// the server
    /// @param id identifier to find
    /// @return the index of the mixture if it exists; -1 otherwise
    ///
    long getMixtureIndex(const String& id) const;

    /// Sets the identifier of a mixture
    /// @param mix the mixture
    /// @param id the id
    /// @exception IdAlreadyExistsException
    ///
    void setMixtureId(Mixture& mix, const String& id);

    //-------------------------------------------------------------------

    const String& getServerName() const;
    void setServerName(const String&);

    /// Loads a mixture server from a file
    /// File naming rules are the same as mixture file.
    /// @param f name of the file. 
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    ///
    void load(const FileName& f);

    /// Saves a mixture server into a file
    /// File naming rules are the same as mixture file.
    /// @param f name of the file. 
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    /// @exception InvalidDataException
    ///
    void save(const FileName& f) const;

    virtual String getClassName() const;
    virtual String toString() const;


  private :

    const Config&     _config;
    String        _serverName;
    DistribRefVector    _distribDict;
    MixtureDict       _mixtureDict;
    unsigned long     _lastMixtureId;
    mutable unsigned long _vectSize;
    mutable bool      _vectSizeDefined;

    void addDistribToDict(Distrib&);
    void addMixtureToDict(Mixture&);
    String newId();
    Mixture& loadMixture(const FileName& f, DistribType);
    void autoSetMixtureId(Mixture& m, String id);


    ///
    MixtureServer(const MixtureServer& m); /*!Not implemented*/
    const MixtureServer& operator=(
            const MixtureServer&); /*!Not implemented*/
    bool operator==(const MixtureServer&); /*!Not implemented*/
    bool operator!=(const MixtureServer&); /*!Not implemented*/
  };

} // end namespace alize

#endif // !defined(ALIZE_MixtureServer_h)

