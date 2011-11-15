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

#if !defined(ALIZE_SegAbstract_h)
#define ALIZE_SegAbstract_h

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
#include "RefVector.h"
#include "XList.h"

namespace alize
{
  class SegServer;
  class Seg;
  /*!
  Abstract base class for classes Seg and SegCluster
    
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2004
  */

  class ALIZE_API SegAbstract : public Object
  {
    friend class TestSeg;
    friend class TestSegCluster;
    friend class TestSegServer;

  public :

    explicit SegAbstract(SegServer& ss, unsigned long lc, 
               const String& s, const String& sn);
    virtual ~SegAbstract();

    virtual unsigned long begin() const = 0;
    virtual unsigned long length() const = 0;

    /// Returns the label code of this object (segment or cluster)
    /// @return the label code of this object (segment or cluster)
    ///
    unsigned long labelCode() const;

    /// Returns the string associated to this object (segment or cluster)
    /// @return the string associated to this object (segment or cluster)
    ///
    const String& string() const;

    /// Returns a reference to the list associated to this object
    /// (segment or cluster)
    /// @return a reference to the list associated to this object
    /// (segment or cluster)
    ///
    XList& list();

    /// Returns a constant reference to the list associated to this
    /// constant object (segment or cluster)
    /// @return a constant reference the list associated to this
    /// constant object (segment or cluster)
    ///
    const XList& list() const;

    /// Returns the srcName data of this object (segment or cluster)
    /// @return the srcName data of this object (segment or cluster)
    ///
    const String& sourceName() const;

    /// Sets the label code of this object (segment or cluster)
    /// @param lc the label code
    ///
    void setLabelCode(unsigned long lc);

    /// Sets the string data of this object (segment or cluster)
    /// @param s the string
    ///
    void setString(const String& s);

    /// Sets the source name of this object (segment or cluster)
    /// @param sn the source name (a string)
    ///
    void setSourceName(const String& sn);

    /// Returns a reference to the owner (a segment server) of this object
    /// @return a reference to the owner (a segment server) of this object
    ///
    SegServer& getServer() const;

    /// Sets the first segment to become the current segment
    /// (sequential read)
    ///
    virtual void rewind() const;

    /// Gets the next segment (sequential read)\n
    /// See method rewind()
    /// @return a pointer to the segment if it exists; NULL otherwise
    ///
    virtual Seg* getSeg() const = 0;

    virtual String getClassName() const = 0;
    virtual String toString() const = 0;

    // internal usage

    void addOwner(const K&, SegAbstract& o);
    void removeOwner(const K&, SegAbstract& o);
    void removeAllOwners(const K&);
    virtual void getExtremeBoundaries(const K&, unsigned long& b,
               unsigned long& e, bool& isDefined) const = 0;
  protected :

    unsigned long          _labelCode;
    String                 _string;
    String                 _srcName;
    XList                  _list;
    mutable unsigned long  _current;

  private :

    SegServer*             _pServer;
    RefVector<SegAbstract> _ownersVect;
    //virtual void assign(const SegAbstract& s);
  };

} // end namespace alize

#endif // !defined(ALIZE_SegAbstract_h)
