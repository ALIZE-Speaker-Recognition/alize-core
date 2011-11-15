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

#if !defined(ALIZE_String_h)
#define ALIZE_String_h

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
#include <iosfwd> // do not use <ostream> (too slow for compiling)

namespace alize
{
  /// The String class represents character strings.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API String : public Object
  {
  friend class TestString;

  public:

    String(const char* = "");
    String(const String&);
    String& duplicate() const;
    const String& operator=(const String&);
    const String& operator=(const char*);
    String& operator+=(const String&);
    String& operator+=(const char*);
    bool operator==(const String&) const;
    bool operator!=(const String&) const;
    bool operator==(const char*) const;
    bool operator!=(const char*) const;
    bool operator<(const String&) const;
    bool operator<=(const String&) const;
    bool operator>(const String&) const;
    bool operator>=(const String&) const;
    String operator+(const String&) const;
    String operator+(const char*) const;
    String operator[](unsigned long index) const;
    virtual ~String();

    static String valueOf(unsigned long v);
    static String valueOf(long v);
    static String valueOf(unsigned int v);
    static String valueOf(int v);
    static String valueOf(double v);
    static String valueOf(bool v);

    /// Converts this string into a double value
    /// @return the value or 0.0 if it cannot convert
    // TODO : si conversion impossible, generer une exception
    ///
    double toDouble() const;

    /// Converts this string into a boolean value
    /// @return the value 
    // TODO : si conversion impossible, generer une exception
    ///
    bool toBool() const;

    /// Converts this string into a long value
    /// @return the value or 0 if it cannot convert
    // TODO : si conversion impossible, generer une exception
    ///
    long toLong() const;

    /// Converts this string into an unsigned long value
    /// @return the value or 0 if it cannot convert
    // TODO : si conversion impossible ou valeur <0, generer une exception
    ///
    unsigned long toULong() const;

    /// Returns the length of this string
    /// @return the number of characters of the string
    ///
    unsigned long length() const;

    /// The string becomes empty
    ///
    void reset();

    /// Tests whether this string is equal to ""
    /// @return true or false
    ///
    bool isEmpty() const;

    /// Tests whether this string ends with the specified suffix
    /// @return true if the character sequence represented by the
    ///     argument is a suffix of the character sequence
    ///     represented by this object; return false otherwise.
    ///     Note that the result will be true if the argument is
    ///     the empty string.
    ///
    bool endsWith(const String&) const;

    /// Tests whether this string begins with the specified prefix
    /// @return true if the character sequence represented by the
    ///     argument is a prefix of the character sequence
    ///     represented by this object; return false otherwise.
    ///     Note that the result will be true if the argument is
    ///     the empty string.
    ///
    bool beginsWith(const String&) const;

    /// Finds the token number 'index' in this string\n
    /// Example : string = " aaa bbb ccc "\n
    ///       getToken(0) will return "aaa"\n
    ///       getToken(1) will return "bbb"\n
    ///       getToken(2) will return "ccc"\n
    ///       getToken(3 and more) will return ""
    /// @param index index of the token to return
    /// @param sep Separator. A single character. Default is " ".
    /// @return a string with the token if it exists; return an empty
    ///     string otherwise
    ///
    String getToken(unsigned long index, const String& sep = " ") const;

    /// Returns the index within this string of the first occurrence of
    /// the specified substring, starting at the specified index.
    /// @param s the substring to find
    /// @param start the position from which to start the search.
    /// @return the position of the first occurence of the substring.
    ///     If not found, return -1;
    ///
    long find(const String& s, unsigned long start = 0) const;

    /// Returns the string representation of the argument.
    /// @return a string representation of the argument 
    ///
    const char* c_str() const;

    virtual String getClassName() const;

    virtual String toString() const;

  private:

    /// Internal c-style string (an array of char ended by a 0).

    char*         _string;
    unsigned long _capacity;
    unsigned long _length;

    void create(unsigned long length, unsigned long capacity,
                            const char*);
  };

} // end namespace alize

ALIZE_API alize::String operator+(const char*, const alize::String&);
ALIZE_API std::ostream& operator<<(std::ostream&, const alize::String&);

#endif // !defined(ALIZE_String_h)

