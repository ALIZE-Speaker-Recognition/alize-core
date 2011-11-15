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

#if !defined(ALIZE_Exception_h)
#define ALIZE_Exception_h

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

namespace alize
{

  /// Base class for exceptions
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003


  class ALIZE_API Exception : public Object
  {
    public :
      /// Create an Exception object
      ///   Also tries to fetch a stack trace and stores it in the trace field.
      /// @param msg detailed message
      /// @param sourceFile name of the source file that thrown the
      ///    exception
      /// @param line line number in the souce file
      ///
      explicit Exception(const String& msg,
                         const String& sourceFile, int line);
      explicit Exception(const String& msg,
                         const String& sourceFile, int line, const String callerName);
      Exception(const Exception&);
      virtual ~Exception();
      virtual String toString() const;
      virtual String getClassName() const;

      //void OBS__storeStackTrace() ;
      /*! \brief  Tries to get the stack trace of current point
       *
       * \param callerName name of the class derived FooException class
       *   (used to avoid gathering stack trace for EOFException which are non-failing)
       *
       * \note  ATTENTION:
       *    works only on GNU/Linux systems, because:
       *    - it calls gdb
       *    - it accesses the symlink /proc/self/exe to fetch the name of current executable
       *
       * \date  20080904120004
       * \author  Florian Verdet _goto. <florian.verdet@univ-avignon.fr>,<hacking@verdet.ch>
       *   CREDITS:
       *    Idea to call gdb from within exception/crash handler to get a correct stack trace comes from:
       *     Mark Kretschmann markey, prominent Amarok hacker / C++ guru
       */
      String stackTrace( const String callerName) const ;

      const String msg; // message of the exception
      const String sourceFile; // name of the source file
      const int  line; // line number of the source file
      String trace ;  ///< gets filled by sTrace upon throwing time to fetch current stack trace
    private:
      bool operator==(const Exception&) const;   /*!Not implemented*/
      bool operator!=(const Exception& e) const;   /*!Not implemented*/
      const Exception& operator=(const Exception&);/*!Not implemented*/
  };

  //---------------------------------------------------------------------
  /// This exception may be thrown to indicate that an index of some sort
  /// (such as to an array, to a string, or to a vector) is out of range. 
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class IndexOutOfBoundsException : public Exception
  {
    public :
      explicit IndexOutOfBoundsException(const String& msg,
                const String& sourceFile, int line,
                long index, long limit);
      IndexOutOfBoundsException(
        const IndexOutOfBoundsException&);
      virtual ~IndexOutOfBoundsException();
      virtual String getClassName() const;
      virtual String toString() const;
      const long index;
      const long limit;
    private:
      bool operator==(const IndexOutOfBoundsException&)
                        const; /*!Not implemented*/
      bool operator!=(const IndexOutOfBoundsException& e)
                        const; /*!Not implemented*/
      const IndexOutOfBoundsException& operator=(
          const IndexOutOfBoundsException&); /*!Not implemented*/
  };

  //---------------------------------------------------------------------
  /// Signals that an I/O exception of some sort has occurred. This class
  /// is the general class of exceptions produced by failed or interrupted
  /// I/O operations.
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API IOException : public Exception
  {
    public :
      explicit IOException(const String& msg,
            const String& sourceFile, int line, const FileName& f);
      explicit IOException(const String& msg,
            const String& sourceFile, int line, const FileName& f, const String callerName);
      IOException(const IOException&);
      virtual ~IOException();
      virtual String toString() const;
      virtual String getClassName() const;
      const FileName fileName;
    private:
      bool operator==(const IOException&) const;   /*!Not implemented*/
      bool operator!=(const IOException& e) const; /*!Not implemented*/
      const IOException& operator=(
                  const IOException&); /*!Not implemented*/
  };
  //---------------------------------------------------------------------
  /// Exception thrown if you try to create or duplicate a mixture inside
  /// the mixture server with an already used identifier
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class IdAlreadyExistsException : public Exception
  {
    public :
      explicit IdAlreadyExistsException(const String& msg,
          const String& sourceFile, int line);
      IdAlreadyExistsException(
        const IdAlreadyExistsException&);
      virtual ~IdAlreadyExistsException();
      virtual String getClassName() const;
    private:
      bool operator==(const IdAlreadyExistsException&)
                          const; /*!Not implemented*/
      bool operator!=(const IdAlreadyExistsException& e)
                          const; /*!Not implemented*/
      const IdAlreadyExistsException& operator=(const
               IdAlreadyExistsException&); /*!Not implemented*/
  };
  //---------------------------------------------------------------------
  /// Exception thrown when an unexpected invalid data is encountered
  /// in a file
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class InvalidDataException : public IOException
  {
    public :
      explicit InvalidDataException(const String& msg,
        const String& sourceFile, int line, const FileName& f);
      InvalidDataException(const InvalidDataException&);
      virtual ~InvalidDataException();
      virtual String getClassName() const;
    private:
      bool operator==(const InvalidDataException&)
                        const; /*!Not implemented*/
      bool operator!=(const InvalidDataException& e)
                        const; /*!Not implemented*/
      const InvalidDataException& operator=(const
                 InvalidDataException&); /*!Not implemented*/
  };
  //---------------------------------------------------------------------
  /// Signals that an attempt to open the file has failed.
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class FileNotFoundException : public IOException
  {
    public :
      explicit FileNotFoundException(const String& msg,
         const String& sourceFile, int line, const FileName& f);
      FileNotFoundException(const FileNotFoundException&);
      virtual ~FileNotFoundException();
      virtual String getClassName() const;
    private:
      bool operator==(const FileNotFoundException&)
                        const; /*!Not implemented*/
      bool operator!=(const FileNotFoundException& e)
                        const; /*!Not implemented*/
      const FileNotFoundException& operator=(const
                FileNotFoundException&); /*!Not implemented*/
  };
  //---------------------------------------------------------------------
  /// Signals that an end of file has been reached unexpectedly during
  /// input.
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class EOFException : public IOException
  {
    public :
      explicit EOFException(const String& msg,
        const String& sourceFile, int line, const FileName& f);
      EOFException(const EOFException&);
      virtual ~EOFException();
      virtual String getClassName() const;
    private:
      bool operator==(const EOFException&) const; /*!Not implemented*/
      bool operator!=(const EOFException& e)
                         const; /*!Not implemented*/
      const EOFException& operator=(
                  const EOFException&); /*!Not implemented*/
  };
  //---------------------------------------------------------------------
  /// Thrown when the free memory is not enough to allocate an object
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class OutOfMemoryException : public Exception
  {
    public :
      explicit OutOfMemoryException(const String& msg,
          const String& sourceFile, int line);
      OutOfMemoryException(const OutOfMemoryException&);
      virtual ~OutOfMemoryException();
      virtual String getClassName() const;
    private:
      bool operator==(const OutOfMemoryException&)
                        const; /*!Not implemented*/
      bool operator!=(const OutOfMemoryException& e)
                        const; /*!Not implemented*/
      const OutOfMemoryException& operator=(const
                 OutOfMemoryException&); /*!Not implemented*/
  };
  //---------------------------------------------------------------------
  /// Thrown when a trial to get a parameter from a configuration abort
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2005

  class ParamNotFoundInConfigException : public Exception
  {
    public :
      explicit ParamNotFoundInConfigException(const String& msg,
          const String& sourceFile, int line);
      ParamNotFoundInConfigException(const ParamNotFoundInConfigException&);
      virtual ~ParamNotFoundInConfigException();
      virtual String getClassName() const;
    private:
      bool operator==(const ParamNotFoundInConfigException&)
                        const; /*!Not implemented*/
      bool operator!=(const ParamNotFoundInConfigException& e)
                        const; /*!Not implemented*/
      const ParamNotFoundInConfigException& operator=(const
                 ParamNotFoundInConfigException&); /*!Not implemented*/
  };
  //---------------------------------------------------------------------
  /// Thrown when a configuration check fail
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2005

  class ConfigCheckException : public Exception
  {
    public :
      explicit ConfigCheckException(const String& msg,
          const String& sourceFile, int line);
      ConfigCheckException(const ConfigCheckException&);
      virtual ~ConfigCheckException();
      virtual String getClassName() const;
    private:
      bool operator==(const ConfigCheckException&)
                        const; /*!Not implemented*/
      bool operator!=(const ConfigCheckException& e)
                        const; /*!Not implemented*/
      const ConfigCheckException& operator=(const
                 ConfigCheckException&); /*!Not implemented*/
  };
  //---------------------------------------------------------------------
} // end namespace alize

//extern void* operator new(size_t);

#endif // !defined(ALIZE_Exception_h)

