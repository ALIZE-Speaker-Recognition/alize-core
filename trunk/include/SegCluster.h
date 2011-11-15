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

#if !defined(ALIZE_SegCluster_h)
#define ALIZE_SegCluster_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include "SegAbstract.h"
#include "RefVector.h"

namespace alize
{
  /*!
  Class for a hierarchical cluster of segments.
    
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2004
  */

  class ALIZE_API SegCluster : public SegAbstract
  {
    friend class TestSegCluster;
    friend class TestSegServer;

  public :

    virtual ~SegCluster();

    //<FRANCAIS>
    /// Recherche dans le cluster le segment dont le numéro de 1ere trame
    /// (begin) est le plus petit parmis tous les segments du cluster et
    /// retourne ce numéro. Cette fonction nécessite de parcourir tous les 
    /// segments donc est couteuse en temps.
    /// @return see description
    ///
    virtual unsigned long begin() const;

    //<FRANCAIS>
    /// Retourne le nombre de features du cluster par différence entre
    /// la 1ere feature (méthode begin()) et la dernière (même principe 
    /// que la méthode begin()).
    /// @warning La méthode ne vérifie pas si les segments sont contiguës.
    /// @return see description
    ///
    virtual unsigned long length() const;

    /// Adds a segment or sub-cluster to a cluster
    /// @param s the segment or sub-cluster to add
    ///
    void add(SegAbstract& s);

    /// Adds a segment or sub-cluster to this cluster at a particular position
    /// @param s the segment or sub-cluster to add
    /// @param pos the position of the added object
    ///
    void add(SegAbstract& s, unsigned long pos);

    /// Creates a new segment copying an existing segment and adds it to this
    /// cluster
    /// @param s the segment to copy
    /// @return the new segment
    ///
    Seg& addCopy(const Seg& s);

    /// Creates a new segment copying an existing segment and adds it to this
    /// cluster
    /// @param s the segment to copy
    /// @param pos the position of the new segment
    /// @return the new segment
    ///
    Seg& addCopy(const Seg& s, unsigned long pos);

    /// Creates a new segment and adds it to this cluster
    /// @param b = begin : number of the first feature
    /// @param l = length : count of features
    /// @param lc = label code
    /// @param s = a free string
    /// @param sn = source name (free). Ex: file name
    /// @return a reference to the new segment
    ///
    Seg& addNewSeg(unsigned long b = 0, unsigned long l = 0,
             unsigned long lc = 0, const String& s = "",
             const String& sn = "");

    /// Removes a segment or sub-cluster from this cluster
    /// @param s the segment or sub-cluster to add
    ///
    void remove(SegAbstract& s);

    /// Removes a segment or sub-cluster from this cluster by it's index
    /// @param idx the index
    ///
    SegAbstract& remove(unsigned long idx);

    /// Removes all components of this cluster
    ///           
    void removeAll();

    /// Returns the segment or sub-clusters at the position i
    /// @param i the position of the object to get
    /// @return the object found as a SegAbstract object. To know if it is
    ///         a Seg or a SegCluster, use a cast operator (quick method)
    ///         or call getClassName() (slow method)
    ///
    SegAbstract& get(unsigned long i) const;

    // <FRANCAIS>
    /// Recherche dans le cluster le 1er segment qui contient une feature
    /// (identifiée par son n°) et renvoi true s'il existe. La
    /// méthode parcours tous les segments du cluster et regarde si
    /// begin >= n°feature < begin + length\n
    /// Après appel de la méthode, la variable lc contient le code label
    /// du segment trouvé.\n
    /// De plus, les booléens first et last permettent de savoir si
    /// la feature est la dernière du segment ou la première
    /// METHODE PROVISOIRE. Sera remplacé par une méthode
    /// qui accepte directement pour paramètre une feature (Feature&)
    /// et non plus le n° de la feature
    /// @param n feature number
    /// @param lc a variable to store the label code
    /// @param isFirst a boolean to indicate that the feature is or is not
    /// the first inside the segment
    /// @param isLast a boolean to indicate that the feature is or is not
    /// the last inside the segment
    /// @return true if a segment was find; false otherwise
    ///
    bool getFeatureLabelCode(unsigned long n, unsigned long& lc,
                 bool& isFirst, bool& isLast) const;

    /// Like other same-name method but without needing parameters
    /// isFirst and isLast
    /// METHODE PROVISOIRE. Sera remplacé par une méthode
    /// qui accepte directement pour paramètre une feature (Feature&)
    /// et non plus le n° de la feature
    /// @param n feature number
    /// @param lc a variable to store the label code
    /// @return true if a segment was find; false otherwise
    ///
    bool getFeatureLabelCode(unsigned long n, unsigned long& lc) const;

    /// Returns the number of segments or clusters contained in this
    /// cluster.
    /// @return the number of segments or clusters contained in this
    ///         cluster.
    ///
    unsigned long getCount() const;
 
    /// Gets the next segment (sequential read). Reads all the segment of
    /// a cluster and sub-clusters.\n
    /// See method getSeg()
    /// @return a pointer to the segment if it exists; NULL otherwise
    ///
    virtual Seg* getSeg() const;

    /// Returns the id (unique) of the cluster
    /// @return the id (unique) of the cluster
    ///
    unsigned long getId() const;

    /// Sets the first segment to become the current segment
    /// (sequential read)
    ///
    virtual void rewind() const;

    virtual String getClassName() const;
    virtual String toString() const;

    // internal usage

    SegCluster& duplicate(const K&, SegServer&) const;
    void setId(const K&, unsigned long id);
    static SegCluster& create(const K&, SegServer& ss,
                  unsigned long lc = 0, const String& s = "",
                  const String& sn = "");
    virtual void getExtremeBoundaries(const K&, unsigned long& b,
                 unsigned long& e, bool& isDefined) const;

  private :

    RefVector<SegAbstract> _vect;
    unsigned long          _id;

    // for function getSeg()
    mutable SegAbstract*   _pCurrentSubSeg;

    // for functions getFeatureLabelCode(...)
    mutable Seg*           _pCurrentSeg;
	  mutable Seg*           _pStartSeg;
    mutable unsigned long  _b;
    mutable unsigned long  _e;
    mutable unsigned long  _lc;

    explicit SegCluster(SegServer& ss, unsigned long lc = 0,
                const String& s= "", const String& sn = "");
    SegCluster(const SegCluster&); /* not implemented */
    const SegCluster& operator=(const SegCluster& c); /* not implemented */
    bool operator==(const SegCluster& c) const; /* not implemented */
    bool operator!=(const SegCluster& c) const; /* not implemented */
  };   

} // end namespace alize

#endif // !defined(ALIZE_SegCluster_h)
