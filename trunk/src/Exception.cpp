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

#if !defined(ALIZEExceptioncpp)
#define ALIZEExceptioncpp

#include "Exception.h"

/* g++-4.3 needs following inlcusions (seems to work without for g++-4.1 and 4.2) */
#include <cstdio>    /* for: stderr fprintf sprintf popen fgets pclose */

#if defined(__GNUC__)
#include <unistd.h>  /* for: readlink getpid */
#endif

/* TODO: If you know a way to do it the clean C++ way, feel free ! */

using namespace alize;

//-------------------------------------------------------------------------
Exception::Exception(const String& msg, const String& sourceFile, int line)
:Object(), msg(msg), sourceFile(sourceFile), line(line), trace(stackTrace(getClassName())) {}
//-------------------------------------------------------------------------
/* NOTE: we need the name of the derived class in the superclass. But execution sequence is:
 * superclass-initializationlist, superclass-constructor, class-initializationlist, class-constructor.
 * The only way is to pass the className of the current class in the call to the superclass-constructor.
 */
Exception::Exception(const String& msg, const String& sourceFile, int line, String callerName)
:Object(), msg(msg), sourceFile(sourceFile), line(line), trace(stackTrace(callerName)) {}
//-------------------------------------------------------------------------
Exception::Exception(const Exception& e)
:Object(),msg(e.msg),sourceFile(e.sourceFile),line(e.line),trace(e.trace) {}
//-------------------------------------------------------------------------
String Exception::toString() const
{
  return trace  //stack trace is output in front, since self-made message will be extended by other FooException classes
    + Object::toString()
    + "\n  message   = \"" + msg + "\""
    + "\n  source file = " + sourceFile
    + "\n  line number = " + String::valueOf(line);
}
//-------------------------------------------------------------------------
String Exception::getClassName() const { return "Exception"; }
//-------------------------------------------------------------------------
/*!
 * /brief  stackTrace - tries to get the stack trace of current point.
 *  If (libALIZE and) MISTRAL bits were compiled with -g or -ggdb option,
 *  prints srcFile+line and method parameters. Otherwise, just method names.
 *
 * ATTENTION:
 *  works only on GNU/Linux systems, because:
 *  - it calls gdb
 *  - it accesses the symlink /proc/self/exe to fetch the name of current executable
 * EXAMPLES:
 *  standalone gdb call for testing:
 *    gdb -ex run --batch --args ../EigenChannel/EigenChannel --config /users/verdet/lid/fa1-30sSeg-CMSSDCCMS/cfg/EigenChannel.cfg --ndxFilename foo --inputWorldFilename foo --channelMatrix foo
 *  gdb call attaching to running process:
 *    gdb -ex run --batch ../EigenChannel/EigenChannel 1234
 *
 * CREDITS:
 * Idea to call gdb from within exception/crash handler to get a correct stack trace
 *  comes from:
 *   Mark Kretschmann markey, prominent Amarok hacker / C++ guru
 * 20080904120004
 *  Florian Verdet _goto. <florian.verdet@univ-avignon.fr>,<hacking@verdet.ch>
 *
 * TODO: make gdb call etc. parametrable through a config option (i.e. debugLevel) for:
 *  - do a 'bt full' to show also local variables
 *  - launch interactive gdb to be able to analyze manually (without '--batch')
 */
String Exception::stackTrace( const String callerName) const
{
	//fprintf(stdout, "DBG: Exception::stackTrace() called with caller[%s]!\n", callerName.c_str()) ;

	/*! EOFException is usually catched non-failing and occurs for every file read.
	 * Thus we don't build a stack trace (otherwise, we loose far too much time. */
	if( callerName == "EOFException") {
		return "" ;
	}
	if( callerName == "OutOfMemoryException") {
		// don't allocate a String, since we don't have any memory any more... (implies putting msg twice hardcoded)
		fprintf(stderr, "OutOfMemoryException: Exception::stackTrace() won't succeed in tracing the stack...");
		return "OutOfMemoryException: Exception::stackTrace() won't succeed in tracing the stack..." ;
	}

	/*! FileNotFoundException may be catched non-failing in EnergyDetectorMain.cpp(1*),
	 * SegTools.cpp(each lblFile, currently unused), LabelFusion.cpp(1*).
	 * Since there's no so many (useless) cases, we build a stack trace anyway (no special handling) */
	/*if( callerName == "FileNotFoundException") {
		//return "" ;
	}*/

	String data ;

#if defined(linux) || defined(__linux)  	///< fetch required bits (workaround the fact we don't have access to any info (argv[0] i.e.) )
	char* mistralProg = new char[2048];
	size_t length = readlink("/proc/self/exe", mistralProg, 2048);
	mistralProg[length] = '\0';
	char* myPid = new char[9]; //usually 32k -> 5chars+nul, max possible is 8chars
	sprintf( myPid, "%d", getpid() ) ;

	// TODO: escape mistralProg to avoid nasty things
	// with 'bt full', prints also local variables (not very useful if having only complex objects...)
	//String cmd = "gdb --batch -ex='bt full' " + String(mistralProg) +" "+ String(myPid) ;
#if defined(THREAD)  //note: no threads in ALIZE!
	String cmd = "gdb --batch -ex='info threads' -ex=bt " + String(mistralProg) +" "+ String(myPid) ; //doesn't hurt if there's no threads split off
#else
	String cmd = "gdb --batch -ex=bt " + String(mistralProg) +" "+ String(myPid) ;
#endif
	data += callerName + "; stack trace by GDB [" + cmd +"]\n" ;

	// run cmd and read its output
	size_t MAX_BUFFER = 255 ;
	FILE *stream;
	char buffer[MAX_BUFFER];
	stream = popen( cmd.c_str(), "r");
	while( fgets(buffer, MAX_BUFFER, stream) != NULL ) {
	   data += buffer ;
	}
	pclose(stream);
	
	/* //another way to do it (the above is +- a wrapper to this)
	pid_t pid;
	pid = fork();
	if ( pid < 0 ) {
		printf( "fork failed" );
		//exit(1);
	} else if ( pid ==0 ) {
		printf ( "I'm the child (%d), my parent is %d\n", getpid(), getppid());
		printf( "executing [gdb --batch -ex=bt %s %s]\n", mistralProg, myPid) ;
		int ret = execlp( "gdb", "gdb", "--batch", "-ex=bt", mistralProg, myPid, (char *) NULL) ;
		printf( "ERROR: exec returned with [%d]\n", ret) ;
	} else {
		printf ( "I'm the parent (%d), I too have a parent (%d)\n", getpid(), getppid() );
	}
	*/
#else
	data = " *** Exception::stackTrace() uses gdb and GNU/Linux' /proc fs which are not available on others systems - won't trace the stack.\n" ;
#endif


	//fprintf( stdout, "DBG: trace[%s]", data.c_str());
	return data;
}
//-------------------------------------------------------------------------
Exception::~Exception() {}
//-------------------------------------------------------------------------



//-------------------------------------------------------------------------
IndexOutOfBoundsException::IndexOutOfBoundsException(const String& msg,
        const String& sourceFile, int line, long index, long limit)
:Exception(msg, sourceFile, line, getClassName()), index(index), limit(limit) {}
//-------------------------------------------------------------------------
IndexOutOfBoundsException::IndexOutOfBoundsException(
                const IndexOutOfBoundsException& e)
:Exception(e.msg, e.sourceFile, e.line, getClassName()),
 index(e.index), limit(e.limit) {}
//-------------------------------------------------------------------------
String IndexOutOfBoundsException::toString() const
{
  return Exception::toString()
    + "\n  index " + String::valueOf(index)
    + " >= limit " + String::valueOf(limit);
}
//-------------------------------------------------------------------------
String IndexOutOfBoundsException::getClassName() const
{ return "IndexOutOfBoundsException"; }
//-------------------------------------------------------------------------
IndexOutOfBoundsException::~IndexOutOfBoundsException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
IOException::IOException(const String& msg, const String& sourceFile,
                     int line, const FileName& f)
:Exception(msg, sourceFile, line, getClassName()), fileName(f) {}
//-------------------------------------------------------------------------
IOException::IOException(const String& msg, const String& sourceFile,
                     int line, const FileName& f, const String callerName)
:Exception(msg, sourceFile, line, callerName), fileName(f) {}
//-------------------------------------------------------------------------
IOException::IOException(const IOException& e)
:Exception(e.msg, e.sourceFile, e.line, getClassName()) {}
//-------------------------------------------------------------------------
String IOException::toString() const
{ return Exception::toString() + "\n  fileName =  " + fileName; }
//-------------------------------------------------------------------------
String IOException::getClassName() const { return "IOException"; }
//-------------------------------------------------------------------------
IOException::~IOException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
IdAlreadyExistsException::IdAlreadyExistsException(const String& msg,
                    const String& sourceFile, int line)
:Exception(msg, sourceFile, line, getClassName()) {}
//-------------------------------------------------------------------------
IdAlreadyExistsException::IdAlreadyExistsException(
                  const IdAlreadyExistsException& e)
:Exception(e.msg, e.sourceFile, e.line, getClassName()) {}
//-------------------------------------------------------------------------
String IdAlreadyExistsException::getClassName() const
{ return "IdAlreadyExistsException"; }
//-------------------------------------------------------------------------
IdAlreadyExistsException::~IdAlreadyExistsException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
InvalidDataException::InvalidDataException(const String& msg,
        const String& sourceFile, int line, const FileName& f)
:IOException(msg, sourceFile, line, f, getClassName()) {}
//-------------------------------------------------------------------------
InvalidDataException::InvalidDataException(const InvalidDataException& e)
:IOException(e.msg, e.sourceFile, e.line, e.fileName, getClassName()) {}
//-------------------------------------------------------------------------
String InvalidDataException::getClassName() const
{ return "InvalidDataException"; }
//-------------------------------------------------------------------------
InvalidDataException::~InvalidDataException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
OutOfMemoryException::OutOfMemoryException(const String& msg,
                    const String& sourceFile, int line)
:Exception(msg, sourceFile, line, getClassName()) {}
//-------------------------------------------------------------------------
OutOfMemoryException::OutOfMemoryException(const OutOfMemoryException& e)
:Exception(e.msg, e.sourceFile, e.line, getClassName()) {}
//-------------------------------------------------------------------------
String OutOfMemoryException::getClassName() const
{ return "OutOfMemoryException"; }
//-------------------------------------------------------------------------
OutOfMemoryException::~OutOfMemoryException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
FileNotFoundException::FileNotFoundException(const String& msg,
        const String& sourceFile, int line, const FileName& f)
:IOException(msg, sourceFile, line, f, getClassName()) {}
//-------------------------------------------------------------------------
FileNotFoundException::FileNotFoundException(
                  const FileNotFoundException& e)
:IOException(e.msg, e.sourceFile, e.line, e.fileName, getClassName()) {}
//-------------------------------------------------------------------------
String FileNotFoundException::getClassName() const
{ return "FileNotFoundException"; }
//-------------------------------------------------------------------------
FileNotFoundException::~FileNotFoundException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
ParamNotFoundInConfigException::ParamNotFoundInConfigException(const String& msg,
        const String& sourceFile, int line)
:Exception(msg, sourceFile, line, getClassName()) {}
//-------------------------------------------------------------------------
ParamNotFoundInConfigException::ParamNotFoundInConfigException(
                  const ParamNotFoundInConfigException& e)
:Exception(e.msg, e.sourceFile, e.line, getClassName()) {}
//-------------------------------------------------------------------------
String ParamNotFoundInConfigException::getClassName() const
{ return "ParamNotFoundInConfigException"; }
//-------------------------------------------------------------------------
ParamNotFoundInConfigException::~ParamNotFoundInConfigException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
ConfigCheckException::ConfigCheckException(const String& msg,
        const String& sourceFile, int line)
:Exception(msg, sourceFile, line, getClassName()) {}
//-------------------------------------------------------------------------
ConfigCheckException::ConfigCheckException(
                  const ConfigCheckException& e)
:Exception(e.msg, e.sourceFile, e.line, getClassName()) {}
//-------------------------------------------------------------------------
String ConfigCheckException::getClassName() const
{ return "ConfigCheckException"; }
//-------------------------------------------------------------------------
ConfigCheckException::~ConfigCheckException() {}
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
EOFException::EOFException(const String& msg, const String& sourceFile,
                                               int line, const FileName& f)
:IOException(msg, sourceFile, line, f, getClassName()) {}
//-------------------------------------------------------------------------
EOFException::EOFException(const EOFException& e)
:IOException(e.msg, e.sourceFile, e.line, e.fileName, getClassName()) {}
//-------------------------------------------------------------------------
String EOFException::getClassName() const { return "EOFException"; }
//-------------------------------------------------------------------------
EOFException::~EOFException() {}
//-------------------------------------------------------------------------

/*void* operator new(sizet n)
{
  void* p = new (std::nothrow) (n);
  return p;
}
*/
#endif // !defined(ALIZEExceptioncpp)
