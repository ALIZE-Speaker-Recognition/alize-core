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

#if !defined(ALIZE_LabelServer_h)
#define ALIZE_LabelServer_h

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
#include "RefVector.h"
#include "Label.h"

namespace alize
{
  /*!
  This class is a container for Label objects. Each Label got an index.
  This index is used to set the label code in a feature. 
  @deprecated since jan 2005
  
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2004
  */

  class ALIZE_API LabelServer : public Object
  {
    friend class TestLabelServer;
    
  public :

    /// Initialize internal data of the server
    /// @param usePredefinedLabels true to use the pre-defined labels;
    ///    false otherwise (no labels in the server)
    ///    
    LabelServer(bool usePredefinedLabels = false);

    virtual ~LabelServer();

    /// Adds a new Label object to the server.
    /// <FRANCAIS>
    /// Le serveur commence par regarder si le label ajoute est identique
    /// a celui ajoute precedemment. Si oui, il retourne son index sans
    /// ajouter de label. Avantage : c'est tres rapide. Si non, il
    /// parcours tous les labels du serveur a la recherche d'un label
    /// identique. S'il le trouve, il retourne son index sans ajouter de 
    /// nouveau label. Dans le cas contraire, il ajoute une copie de ce
    /// label à la fin de sa liste de label et retourne l'index.
    /// On peut forcer l'ajout d'un label dans le cas ou un exemplaire
    /// existe déjà
    /// @param l The object to add. The Label object stored is a copy
    ///    of this parameter.\n
    /// @param forceAdd force the add of the label
    /// @return the index of the label
    ///
    unsigned long addLabel(const Label& l, bool forceAdd = false);

    /// Set a new Label object at a specific position inside the server.
    /// @param l The object to store. The Label object stored is a
    ///    copy of this parameter
    /// @param index the position of the label inside the server
    /// @exception IndexOutOfBoundsException
    ///
    void setLabel(const Label& l, unsigned long index) const;

    /// Returns a reference to the label stored inside the server at a
    /// specific position
    /// @param index the position of the label inside the server
    /// @exception IndexOutOfBoundsException
    /// @return a reference to the label
    ///
    Label& getLabel(unsigned long index) const;

    /// Find and returns the index of a label
    /// @param s the string used as a key to search the label
    /// @return the index of the label if it exists; -1 otherwise
    ///
    long getLabelIndexByString(const String& s) const;

    /// Delete all Label objects stored in the server except for the
    /// pre-defined labels.
    /// @param deletePreDefined true to delete the pre-defined labels
    ///    too; otherwise false
    ///
    void clear(bool deletePreDefined = false);

    /// Returns the count of label stored in the server
    /// @return the count of label stored in the server
    ///
    unsigned long size() const;

    virtual String getClassName() const;


  private :

    unsigned long   _first; /*! index of the first non-predefined label */
    unsigned long   _lastAdded;/*! index of the last label added*/
    RefVector<Label> _vect;

    LabelServer(const LabelServer&); /*! Not implemented */
    const LabelServer& operator=(const LabelServer&); /*! Not implemented*/
    bool operator==(const LabelServer&) const; /*! Not implemented */
    bool operator!=(const LabelServer&) const; /*! Not implemented */
  };

} // end namespace alize

#endif // !defined(ALIZE_LabelServer_h)
