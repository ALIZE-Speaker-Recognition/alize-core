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

#if !defined(ALIZE_FileReader_h)
#define ALIZE_FileReader_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include <cstdio>
#include "Object.h"
#include "alizeString.h"
#include "RealVector.h"


namespace alize
{

  /// Convenient class used to read a file.
  /// *** DO NOT USE ***
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API FileReader : virtual public Object
  {
  
    friend class TestFeatureFileReaderRaw;
    friend class TestFeatureFileReaderHTK;
    friend class TestFeatureFileReaderSPro4;
    friend class TestFeatureFileReader;
    friend class TestMixtureFileReaderRaw;
    friend class TestMixtureFileReaderAmiral;

  public :


    explicit FileReader(const FileName&, const String& path,
                                  const String& extension, bool swap);
    static FileReader& create(const FileName& f, const String& path,
                                  const String& extension, bool swap);

    /// Close the opened file (if it is).
    ///
    virtual ~FileReader();

    /// Tests whether the file is closed
    /// @return true if the file is closed; false otherwise
    ///
    bool isClosed() const;

    /// Tests whether the file is opened 
    /// @return true if the file is opened; false otherwise
    ///
    bool isOpen() const;

    /// Closes the file and releases any system resources
    /// associated with the stream 
    /// @exception IOException if an I/O error occurs
    ///
    virtual void close();

    /// Repositions the file position indicator to the beginning of the
    /// file (if it is opened). Does nothing if the file is closed. 
    /// @exception IOException if an I/O error occurs
    ///
    virtual void reset();

    /// Reads an input byte
    /// @return the char value
    /// @exception IOException if an I/O error occurs
    /// @exception EOFException if end of file has been reached
    ///
    char readChar();
    
    /// Reads two input bytes
    /// @return the value in a integer variable
    /// @exception IOException if an I/O error occurs
    /// @exception EOFException if end of file has been reached
    ///
    int readInt2();
    
    /// Reads FOUR input bytes
    /// @return the value in a long variable
    /// @exception IOException if an I/O error occurs
    /// @exception EOFException if end of file has been reached
    ///
    long readInt4();
    
    /// Reads four input bytes
    /// @return the value in an unsigned long variable
    /// @exception IOException if an I/O error occurs
    /// @exception EOFException if end of file has been reached
    ///
    unsigned long readUInt4();
    
    /// Reads eight input bytes
    /// @return the double value
    /// @exception IOException if an I/O error occurs
    /// @exception EOFException if end of file has been reached
    ///
    double readDouble();
    
    /// Reads four input bytes
    /// @return the float value
    /// @exception IOException if an I/O error occurs
    /// @exception EOFException if end of file has been reached
    ///
    float readFloat();

    /// Reads a set of float value (4 bytes)
    /// @param v the vector to fill
    /// @exception IOException if an I/O error occurs
    /// @exception EOFException if end of file has been reached
    ///
    FloatVector& readFloats(FloatVector& v);

    /// Tries to read a set of float value (4 bytes). The number is
    /// determined by the smaller between vector size and the number
    /// of available float values in the stream before the end of its
    /// buffer
    /// @param v the vector to fill
    /// @return the number of float values read
    /// @exception IOException if an I/O error occurs
    ///
    unsigned long readSomeFloats(FloatVector& v);
    
    /// Reads the next line of text from the input stream. It reads
    /// successive bytes until it encounters a line terminator or end of
    /// file. The characters read are then returned as a String.
    /// If end of file is encountered then a EOFException is thrown.
    /// If the character '\n' is encountered, it is discarded and reading
    /// ceases. If the character '\r' is encountered, it is discarded and,
    /// if the following byte converts to the character '\n', then that
    /// is discarded also; reading then ceases. If end of file is
    /// encountered before either of the characters '\n' and '\r' is
    /// encountered, reading ceases. Once reading has ceased, a String is
    /// returned that contains all the characters read and not discarded,
    /// taken in order.
    /// @return the next line of text from the input stream
    /// @exception IOException if an I/O error occurs
    /// @exception EOFException if the end of file is encountered
    ///
    const String& readLine();

    /// Reads a sequence of 'length' characters.
    /// @param length number of characters to read
    /// @return a constant reference to a string of the characters read
    /// @exception IOException if an I/O error occurs
    /// @exception EOFException if end of file has been reached
    ///
    const String& readString(unsigned long length);

    /// Returns the full name of the file.
    /// @return the full name of the file.
    ///
    const String& getFullFileName() const;

    /// Returns the name of the file.
    /// @return the name of the file.
    ///
    const String& getFileName() const;
    
    /// Opens the file. If it is already opened, closes it beforehand.
    /// @exception FileNotFoundException
    /// @exception IOException if an I/O error occurs
    ///
    void open();

    /// Reads and returns the file length
    /// @exception IOException if an I/O error occurs
    /// @return the length of the file in bytes
    ///
    unsigned long getFileLength();

    /// @exception IOException if an I/O error occurs
    ///
    void seek(unsigned long pos);

    void rewind();
    long tell();
    bool& swap();
    void swap2Bytes(void *src, void *dest);
    void swap4Bytes(void *src, void *dest);
    void swap4Bytes(void *src);
    void swap8Bytes(void *src, void *dest);

    virtual String toString() const;
    virtual String getClassName() const;


  protected :
    
    String _fullFileName;

  private :


    FILE*          _pFileStruct; /*! current file descriptor. Can be NULL */
    FileName       _fileName;
    String         _path;
    String         _extension;
    unsigned long  _fileLength;
    bool           _fileLengthDefined;
    mutable String _string; /*! to store temporary data */
    bool           _swap; /*! flag for numeric data */

    /// Low-level method to read bytes from a file.
    /// @param buffer A pointer to a memory area to store the data
    /// @length number of bytes to read
    /// @exception IOException if an I/O error occurs
    /// @exception EOFException if end of file has been reached
    ///
    void read(void* buffer, unsigned long length);

    FileReader(const FileReader&); /*!Not implemented*/
    const FileReader& operator=(const FileReader&); /*!Not implemented*/
    bool operator==(const FileReader&) const; /*!Not implemented*/
    bool operator!=(const FileReader&) const; /*!Not implemented*/
  };

} // end namespace alize

#endif // !defined(ALIZE_FileReader_h)

