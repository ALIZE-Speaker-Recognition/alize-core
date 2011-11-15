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

#if !defined(ALIZE_Object_h)
#define ALIZE_Object_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include <cassert>

#ifndef NULL
  #define NULL 0
#endif

namespace alize
{
  class String;

  typedef double real_t;
  typedef double lk_t;
  typedef double occ_t;
  typedef double weight_t;
  typedef String FileName;

  enum ParamType
  {
    PARAMTYPE_INTEGER,
    PARAMTYPE_FLOAT,
    PARAMTYPE_BOOLEAN,
    PARAMTYPE_STRING,
    PARAMTYPE_UNDEFINED
  };
  enum BigEndian
  {
    BIGENDIAN_AUTO,
    BIGENDIAN_TRUE,
    BIGENDIAN_FALSE
  };
  enum BufferUsage
  {
    BUFFER_AUTO, // auto-sizes buffer
    BUFFER_USERDEFINE  // user defines buffer size
  };
  enum HistoricUsage
  {
    LIMITED,
    ALL_FEATURES
  };
  enum DeleteFlag
  {
    DELETE,
    DO_NOT_DELETE
  };
  enum DuplDistrib
  {
    DUPL_DISTRIB,
    SHARE_DISTRIB
  };
  enum DistribType
  {
    DistribType_GD,
    DistribType_GF
  };
  enum SPRO3DataKind    /* Definition of sample kinds     */
  {
    SPRO3DataKind_OTHER,     /* Anything else I didn't think about */
    SPRO3DataKind_FBANK,     /* Filter bank output         */
    SPRO3DataKind_FBCEPSTRA,   /* Cepstra from filter bank analysis  */
    SPRO3DataKind_LPCEPSTRA,   /* Cepstra from LPC analysis      */
    SPRO3DataKind_LPCOEFF,   /* Linear prediction coefficients   */
    SPRO3DataKind_PARCOR,    /* Partial correlation coefficients   */
    SPRO3DataKind_LAR      /* Log area ratios          */
  };
  enum MixtureFileWriterFormat
  {
    MixtureFileWriterFormat_XML,
    MixtureFileWriterFormat_RAW,
    MixtureFileWriterFormat_ETAT
  };

  enum FeatureFileReaderFormat
  {
    FeatureFileReaderFormat_RAW,
    FeatureFileReaderFormat_SPRO3,
    FeatureFileReaderFormat_SPRO4,
    FeatureFileReaderFormat_HTK,
  };

  enum MixtureFileReaderFormat
  {
    MixtureFileReaderFormat_AMIRAL,
    MixtureFileReaderFormat_RAW,
    MixtureFileReaderFormat_XML
  };

  enum FeatureFileWriterFormat
  {
    FeatureFileWriterFormat_SPRO3,
    FeatureFileWriterFormat_SPRO4,
    FeatureFileWriterFormat_RAW
  };

  enum SegServerFileReaderFormat
  {
    SegServerFileReaderFormat_LIUM,
    SegServerFileReaderFormat_XML,
    SegServerFileReaderFormat_RAW
  };

  enum SegServerFileWriterFormat
  {
    SegServerFileWriterFormat_XML,
    SegServerFileWriterFormat_LIUM,
    SegServerFileWriterFormat_RAW,
    SegServerFileWriterFormat_TRS
  };

  enum MixtureServerFileWriterFormat
  {
    MixtureServerFileWriterFormat_XML,
    MixtureServerFileWriterFormat_RAW
  };

  class ALIZE_API TopDistribsAction
  {
    friend class Object;
  public:
    TopDistribsAction(): _i(0) {};
    TopDistribsAction(const TopDistribsAction& o):_i(o._i) {};
    bool operator==(const TopDistribsAction& o) const { return _i == o._i; };
    bool operator!=(const TopDistribsAction& o) const { return _i != o._i; };
  private :
    long _i;
  };

  /*!
  Abstract base class for all classes in the toolkit. Both creation and
  destruction counters can be used for debugging. It's a good habit to
  verify that you delete as many objects as you create. Each time an object
  is created, the creation counter is incremented. Each time an object is
  destroyed, the destruction counter is incremented. When you leave your
  application, these counters must be equals.
  
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2003
  */

  class ALIZE_API Object
  {
  public :

    Object();
    virtual ~Object();

    /// Returns the name of the class
    /// @returns the name of the class of the object as a String
    ///
    virtual String getClassName() const = 0;

    /// This method is frequently overridden in the derived classes.
    /// If it is not, it returns the name of the class of the object and the
    /// address of the object
    ///
    /// @return a description of the object
    ///
    virtual String toString() const;

    /// Returns the address
    ///
    virtual String getAddress() const;

    bool isSameObject(const Object&) const;

    static String getParamTypeName(ParamType t);
    static ParamType getParamType(const String& s);
    static String getDistribTypeName(DistribType);
    static DistribType getDistribType(const String& name);
    static FeatureFileReaderFormat getFeatureFileReaderFormat(
             const String& name);
    static SPRO3DataKind getSPro3DataKind(const String& name);
    static SegServerFileReaderFormat getSegServerFileReaderFormat(
             const String& name);
    static SegServerFileWriterFormat getSegServerFileWriterFormat(
             const String& name);
    static MixtureFileWriterFormat getMixtureFileWriterFormat(
             const String& name);
    static FeatureFileWriterFormat getFeatureFileWriterFormat(
             const String& name);
    static MixtureFileReaderFormat getMixtureFileReaderFormat(
             const String& name);
    static MixtureServerFileWriterFormat getMixtureServerFileWriterFormat(
             const String& name);

    /// Tests whether i <= size. Throws an exception if not. For debbuging.
    /// @exception IndexOutOfBoundsException
    ///
    static void assertIsInBounds(const char* fileName, int line,
                                 unsigned long i, unsigned long size);

    /// Tests whether p != NULL. Throws an exception if not. For debbuging.
    /// @exception OutOfMemoryException
    ///    
    static void assertMemoryIsAllocated(const void* p, const char* fileName,
                                        int line);

    static unsigned long max(unsigned long, unsigned long);

#if !defined NDEBUG
  public:
    /// @return the value of the created objects counter
    ///
    static unsigned long getCreationCounter();

    /// @return the value of the destroyed objects counter
    ///
    static unsigned long getDestructionCounter();

    static unsigned long getMax();

  private:
    static unsigned long _max;
    static unsigned long _creationCounter;
    static unsigned long _destructionCounter;
#endif
    
  protected:
    /// Minimum covariance value
    //
    static const real_t MIN_COV;

    /// Minimum likelihood value
    //
    static const real_t EPS_LK;
    static const real_t PI;
    static const real_t PI2;

  private :
    static bool _initialized;
  };

  //-----------------------------------------------------------------------

  /*!
  Class used to authorize some classes to access some methods of others
  classes.
  The static object K::k is used as a key.\n
  INTERNAL USAGE
  <FRANCAIS>Attention : un membre de classe statique n'est detruit que
  lorsque l'application se termine. Cette classe ne dérive pas de Object
  pour ne pas fausser les compteurs internes de Object. En plus ça allège
  le code et il n'existe qu'une seule instance de cette classe dans
  tout alize...
  */

  /// Internal class. Do not use
  
  class K
  {
    friend class Seg;
    friend class XList;
    friend class Config;
    friend class Distrib;
    friend class TestSeg;
    friend class Mixture;
    friend class SegServer;
    friend class DistribGD;
    friend class DistribGF;
    friend class MixtureGD;
    friend class MixtureGF;
    friend class SegCluster;
    friend class StatServer;
    friend class SegAbstract;
    friend class MixtureStat;
    friend class MixtureDict;
    friend class TestFeature;
    friend class ViterbiAccum;
    friend class ConfigChecker;
    friend class TestSegServer;
    friend class TestDistribGD;
    friend class TestDistribGF;
    friend class TestMixtureGD;
    friend class TestMixtureGF;
    friend class MixtureGDStat;
    friend class MixtureGFStat;
    friend class MixtureServer;
    friend class TestSegCluster;
    friend class TestMixtureDict;
    friend class FeatureComputer;
    friend class DistribRefVector;
    friend class TestViterbiAccum;
    friend class TestMixtureGDStat;
    friend class TestMixtureGFStat;
    friend class TestMixtureServer;
    friend class MixtureFileWriter;
    friend class MixtureFileReaderXml;
    friend class TestDistribRefVector;
    friend class MixtureFileReaderRaw;
    friend class TestMixtureFileWriter;
    friend class SegServerFileReaderRaw;
    friend class MixtureFileReaderAmiral;
    friend class MixtureServerFileWriter;
    friend class TestMixtureFileReaderRaw;
    friend class MixtureFileReaderAbstract;
    friend class FeatureMultipleFileReader;
    friend class MixtureServerFileReaderXml;
    friend class MixtureServerFileReaderRaw;
    friend class TestMixtureServerFileWriter;
    friend class FeatureFileReader;
    friend class FeatureFileReaderSingle;
    friend class FeatureInputStreamModifier;
    friend class FeatureServer;

  private :
    K(){}; /*! private constructor */
    static const K k; /*! private object, only for friend classes */
  };
  extern ALIZE_API const TopDistribsAction DETERMINE_TOP_DISTRIBS;
  extern ALIZE_API const TopDistribsAction USE_TOP_DISTRIBS;
  extern ALIZE_API const TopDistribsAction TOP_DISTRIBS_NO_ACTION;

} // end namespace alize

#endif // !defined(ALIZE_Object_h)

