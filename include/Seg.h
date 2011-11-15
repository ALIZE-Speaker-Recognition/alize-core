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

#if !defined(ALIZE_Seg_h)
#define ALIZE_Seg_h

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
#include "SegServer.h"

namespace alize
{
  /*!
  Class for a segment.
    
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2004
  */

  class ALIZE_API Seg : public SegAbstract
  {
    friend class TestSeg;
    friend class TestSegCluster;
    friend class TestSegServer;

  public :

    virtual ~Seg();

    /// Sets the number of the first feature of the segment
    /// @param b beginning value
    ///
    void setBegin(unsigned long b);

    /// Sets the number of features (length) of the segment
    /// @param l the length
    ///
    void setLength(unsigned long l);

    /// Returns the number of the beginning feature of the segment
    /// @return the number of the beginning feature of the segment
    ///
    virtual unsigned long begin() const;

    /// Returns the number of features (length) of the segment
    /// @return the number of features (length) of the segment
    ///
    virtual unsigned long length() const;

    /// Merges two segments. The second segment is destroyed after merging.\n
    /// If the data "string" are not the same, the second string is added
    /// to the first string.\n
    /// If the data "srcName" are not the same, the source names are*
    /// concatenated.\n
    /// The internal lists are concatenated\n
    /// The label code of the second segment is ignored.
    /// @param s the segment to merge with this segment
    /// @return this segment
    ///
    Seg& merge(Seg& s);

    /// Splits this segment starting to a given feature and create a
    /// second segment in the server. If the original segment belongs
    /// to clusters, the second segment is added behind the original
    /// segment in the clusters. All the data (except begin and length)
    /// of the segment are duplicated in the second segment.\n
    /// @param i number of the feature used as the first feature
    ///    of the second generated segment
    /// @return a reference to the new segment
    ///
    Seg& split(unsigned long i);

    /// Duplicate a segment. The new segment is stored inside the
    /// segment server.\n
    /// @return the new segment
    ///
    Seg& duplicate() const;

    virtual String getClassName() const;
    virtual String toString() const;

    // internal usage 
    

    /// Do not use. Internal usage.
    ///
    virtual Seg* getSeg() const;
    Seg& duplicate(const K&, SegServer& ss) const;
    static Seg& create(const K&, SegServer& ss, unsigned long b = 0,
               unsigned long e = 0, unsigned long lc = 0,
               const String& s = "", const String& sn = "");
    virtual void getExtremeBoundaries(const K&, unsigned long& b,
                 unsigned long& e, bool& isDefined) const;

  private :

    unsigned long _begin;
    unsigned long _length;

    explicit Seg(SegServer& ss, unsigned long b = 0,
           unsigned long e = 0, unsigned long lc = 0,
           const String& s = "", const String& sn = "");
    Seg(const Seg&);/*! not implemented */
    const Seg& operator=(const Seg& c);/*! not implemented */
    bool operator==(const Seg& c) const;/*! not implemented */
    bool operator!=(const Seg& c) const;/*! not implemented */
  };   

} // end namespace alize

#endif // !defined(ALIZE_Seg_h)
