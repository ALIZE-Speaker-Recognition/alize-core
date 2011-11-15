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

#if !defined(ALIZE_SegServer_h)
#define ALIZE_SegServer_h

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
using alize::String; // before #include <map>
#include <map>
#include "RefVector.h"
#include "Seg.h"
#include "SegCluster.h"

namespace alize
{
  class Config;

  /*!
  Class used to store segments and clusters of segments.
    
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2004
  */

  class ALIZE_API SegServer : public Object
  {
    friend class TestSegServer;

  public :

    ///
    ///
    explicit SegServer();

    /// Copy constructor
    ///    
    SegServer(const SegServer&);

    /// Assignment operator
    ///
    const SegServer& operator=(const SegServer& ss);

    virtual ~SegServer();

    /// Creates a new segment inside the server
    /// @param b = begin : number of the first feature
    /// @param l = length : count of features
    /// @param lc = label code
    /// @param s = a free string
    /// @param sn = source name (free). Ex: file name
    /// @return a reference to the new segment
    ///
    Seg& createSeg(unsigned long b = 0, unsigned long l = 0,
             unsigned long lc = 0, const String& s = "",
             const String& sn = "");

    /// Duplicates a segment.
    /// @param s the seg to duplicate
    /// @return the new segment
    ///
    Seg& duplicateSeg(const Seg& s);

    /// Creates a new cluster inside the server
    /// @param lc = label code
    /// @param s = a free string
    /// @param sn = source name (free). Ex: file name
    /// @return a reference to the new cluster
    ///
    SegCluster& createCluster(unsigned long lc = 0, const String& s = "",
               const String& sn = "");

    /// Removes (deletes) a segment or a cluster from the server and
    /// from all the clusters (hierarchical clusters)
    /// @param s the segment or cluster to remove
    ///           
    void remove(SegAbstract& s);

    /// Removes (deletes) all segments from the server and from
    /// all the clusters
    ///
    void removeAllSegs();

    /// Removes (deletes) all clusters from the server
    ///
    void removeAllClusters();

    /// Returns the segment by an index
    /// @param idx the index
    /// @return the segment found
    /// @exception IndexOutOfBoundsException
    ///
    Seg& getSeg(unsigned long idx) const;

    /// Returns the cluster by an index
    /// @param idx the index
    /// @return the cluster found
    /// @exception IndexOutOfBoundsException
    ///
    SegCluster& getCluster(unsigned long idx) const;
    
    /// Tests whether a cluster with a particular id exists inside
    /// the server
    /// @param id identifier to find
    /// @return the index of the cluster if it exists; -1 otherwise
    ///
    long getClusterIndexById(unsigned long id) const;
    
    /// Returns a cluster with a particular id 
    /// @param id identifier to find
    /// @return the cluster
    /// @exception Exception if the cluster does not exist
    ///
    SegCluster& getClusterById(unsigned long id) const;
    
    /// Returns the index of a segment or a cluster 
    /// @param s segment or cluster
    /// @return the index
    /// @exception Exception if the segment/cluster does not belong
    ///      to the server
    ///
    unsigned long getIndex(const SegAbstract& s) const;
    
    /// Sets a new id to a cluster. The id must be unique in the server
    /// @param cl the cluster
    /// @param id the id
    /// @exception Exception thrown if the id is not unique or if the
    ///      cluster does not belong to the server
    ///
    void setClusterId(SegCluster& cl, unsigned long id);

    /// Returns the count of segments in the server
    /// @return the count
    ///
    unsigned long getSegCount() const;

    /// Returns the count of clusters in the server
    /// @return the count
    ///
    unsigned long getClusterCount() const;

    /// Returns the name of the server
    /// @return  the name of the server
    ///
    const String& getServerName() const;

    /// Sets the name of the server
    /// @param s the name of the server
    ///
    void setServerName(const String& s);

    /// Saves the server in a file
    /// @param f the name of the file
    /// @param c the configuration to use
    ///
    void save(const FileName& f, const Config& c) const;

    /// Loads the server from a file
    /// @param f the name of the file
    /// @param c the configuration to use
    ///
    void load(const FileName& f, const Config& c);

    /// Creates a new server (static method)
    /// @return a reference to the new server
    /// @warning the server must be destroy at the end using operator "delete"
    ///
    static SegServer& create();

    virtual String getClassName() const;
    virtual String toString() const;

    // internal usage
    void deleteDeletableSeg(const K&);

  private :

    String          _serverName;
    RefVector<Seg> _segVect;
    RefVector<SegCluster> _clusterVect;
    unsigned long   _nextClusterId;
    typedef std::map<unsigned long, unsigned long>::const_iterator it_t;
    std::map<unsigned long, unsigned long> _map;
    SegAbstract& matchingSegAbstract(const SegAbstract&) const;
    void assertServerOwns(const SegAbstract& s) const;
    void assign(const SegServer& ss);

    bool operator==(const SegServer& c) const;/*! not implemented */
    bool operator!=(const SegServer& c) const;/*! not implemented */
  };   

} // end namespace alize

#endif // !defined(ALIZE_SegServer_h)

