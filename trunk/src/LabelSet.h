/*
Alize is a free, open tool for speaker recognition

Alize is a development project initiated by the ELISA consortium
  [www.lia.univ-avignon.fr/heberges/ALIZE/ELISA] and funded by the
  French Research Ministry in the framework of the
  TECHNOLANGUE program [www.technolangue.net]
  [www.technolangue.net]

The Alize project team wants to highlight the limits of voice 
  authentication in a forensic context.
  The following paper proposes a good overview of this point:
  [Bonastre J.F., Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., 
  Magrin-chagnolleau I., Person  Authentification by Voice: A Need of 
  Caution, Eurospeech 2003, Genova]
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
  the use of Alize

Copyright (C) 2003-2005
  Laboratoire d'informatique d'Avignon [www.lia.univ-avignon.fr]
  Frederic Wils [frederic.wils@lia.univ-avignon.fr]
  Jean-Francois Bonastre [jean-francois.bonastre@lia.univ-avignon.fr]
      
This file is part of Alize.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if !defined(ALIZE_LabelSet_h)
#define ALIZE_LabelSet_h

#include "Object.h"
#include "RefVector.h"
#include "RealVector.h"

namespace alize
{
  class Config;

  /// This class represents a set of labels.
  /// Each label got a name, a begin value and a end value
  /// @deprecated since jan 2005
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003
  /// @warning Not stable. Beta version

  class LabelSet : public Object
  {
    friend class TestLabelSet;

  public :

    /// Create a LabelSet object
    ///
    LabelSet();
    explicit LabelSet(const FileName&, const Config&);
    LabelSet(const LabelSet&);
    const LabelSet& operator=(const LabelSet&);
    bool operator==(const LabelSet&);
    bool operator!=(const LabelSet&);
    virtual ~LabelSet();

    /// Load a set of labels from a file
    /// @param f the file to read
    /// @param c the configuration to use
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    ///
    void load(const FileName& f, const Config& c);

    /// Adds a new line to the list
    /// @param b the begin value in seconds
    /// @param e the end value in seconds
    /// @param n the name of the label
    ///
    void addLabel(real_t b, real_t e, const String& n);

    /// Gets the label of the line i
    /// @param i the index of the line
    /// @return a constant reference to the label
    /// @exception IndexOutOfBoundsException
    ///
    const String& getName(unsigned long i) const;

    /// Gets the begin value of the line i
    /// @param i the index of the line
    /// @return the value
    /// @exception IndexOutOfBoundsException
    ///
    real_t getBegin(unsigned long index) const;

    /// Gets the end value of the line i
    /// @param i the index of the line
    /// @return the value
    /// @exception IndexOutOfBoundsException
    ///
    real_t getEnd(unsigned long index) const;

    /// Gets the number of labels in the set
    /// @return the number of labels in the set
    ///
    unsigned long size() const;

    /// Clears the set.
    ///
    void clear();

    virtual String toString() const;
    virtual String getClassName() const;


  private :

    RefVector<String> _nameVect;
    DoubleVector  _beginVect;
    DoubleVector  _endVect;
  };

} // end namespace alize

#endif // !defined(ALIZE_LabelSet_h)

