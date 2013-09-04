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

#if !defined(ALIZE_XList_h)
#define ALIZE_XList_h

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
#include "XLine.h"
#include "RefVector.h"
#include "Exception.h"

namespace alize
{
  class Config;

  /*!
  Class for 
    
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2004
  */

  class ALIZE_API XList : public Object
  {
    friend class TestXList;
    friend class TestXListFileReader;

  public :

    /// Create an empty list
    ///    
    XList();
    explicit XList(const FileName&);
    explicit XList(const FileName&, const Config&);
    static XList& create();
    XList(const XList&);
    const XList& operator=(const XList& c);
    bool operator==(const XList& c) const;
    bool operator!=(const XList& c) const;
    virtual ~XList();

    /// Loads a XList from a file
    /// @param f the file to read
    /// @param c the configuration to use
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    ///
    void load(const FileName& f, const Config& c);

    /// Saves this XList in a ascii file
    /// @param f the file to create
    /// @param c the configuration to use
    ///
    void save(const FileName& f, const Config& c) const;

    /// Saves this XList in a ascii file
    /// @param f the file to create
    ///
    void save(const FileName& f) const;

    /// Appends a new line to the list. The line becomes the current line
    /// @param key the key of the line
    /// @param value the value of the line
    /// @return a reference to the XLine object
    /// @author richard.dufour@lium.univ-lemans.fr
    ///
    XLine& addLine(String& key, String& value);

    /// Search the value of an index
    /// @param index the index of the line
    /// @return the value
    /// @author richard.dufour@lium.univ-lemans.fr
    ///
    String searchValue(String& index);

    /// Appends a new line to the list. The line becomes the current line
    /// @return a reference to the XLine object
    ///
    XLine& addLine();

    /// Sets the first line to become the current line
    ///
    void rewind() const;

    /// Gets a line by an index. The line becomes the current line.\n
    /// The current element of the line is the first one.
    /// @param lineIndex the index of the line to get
    /// @return a reference to the XLine object
    /// @exception IndexOutOfBoundsException    
    ///
    XLine& getLine(unsigned long lineIndex) const;

    /// Gets the current line. The next line becomes the current line.\n
    /// The current element of the line is the first one.
    /// @return a pointer to the line if it exists; NULL otherwise
    ///
    XLine* getLine() const;

    /// Gets all elements of the list.
    /// The current element of the line is the first one.
    /// @return a reference to a XLine object that contains all elements
    ///     of all lines
    ///
    XLine& getAllElements() const;

	/// Gets all unique elements of the list.
    /// The current element of the line is the first one.
    /// @return a reference to a XLine object that contains all elements
    ///     of all lines
    ///
    XLine& getAllUniqueElements() const;

    /// Gets the number of lines in the list
    /// @return  the number of lines in the list
    ///
    unsigned long getLineCount() const;

    /// Finds a line. This line becomes the current line
    /// @param key the key
    /// @param idx the index of the key
    /// @return a pointer to the line if it exists; NULL otherwise
    /// 
    XLine* findLine(const String& key, unsigned long idx = 0) const;

	/// Sort the Xlines by 
	/// @param order could be 'ascend' or 'descend'. Default value is 'descend'
	/// 
	void sortByElementNumber(String order = "descend");

    /// Remove all lines
    ///
    void reset();

    virtual String toString() const;
    virtual String getClassName() const;

  private :

    RefVector<XLine>      _vector;
    mutable unsigned long _current;
    mutable XLine         _line;
  };

} // end namespace alize

#endif // !defined(ALIZE_XList_h)
