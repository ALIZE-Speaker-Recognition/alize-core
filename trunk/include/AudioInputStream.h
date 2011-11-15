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

#if !defined(ALIZE_AudioInputStream_h)
#define ALIZE_AudioInputStream_h

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

namespace alize
{
  class AudioFrame;

  /*!
  This class is the abstract class for audio data stream.\n
  
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2004
  */
  class ALIZE_API AudioInputStream : public Object
  {
    friend class TestAudioFileReader;

  public :

    explicit AudioInputStream(unsigned long selectedChannel);

    /// Resets the stream
    ///    
    virtual void reset() = 0;

    virtual ~AudioInputStream();

    /// Reads the next available audio frame<br>
    /// Reads a unique channel. See method setSelectedChannel(...)
    /// to choose a channel
    /// @param f the audio frame object to store the data
    /// @return false if there is no more data because the end of the
    ///     file has been reached
    /// @exception IOException if an I/O error occurs
    ///
    virtual bool readFrame(AudioFrame& f) = 0;

    /// Sets the position indicator associated with the stream to
    /// a new position. Does not throw any exception or error if the position
    /// exceeds file length
    /// @param pos position of the next frame to read
    ///
    virtual void seekFrame(unsigned long n) = 0;

    /// Returns the frame count of the file
    /// @return the frame count of the file
    ///
    virtual unsigned long getFrameCount() = 0;

    /// Returns the channel count of the file
    /// @return the channel count of the file
    ///
    virtual unsigned long getChannelCount() = 0;

    /// Returns the size of a sample (for one channel) : 8, 16...
    /// @return the size of a sample (for one channel) : 8, 16...
    ///
    virtual unsigned long getSampleBytes() = 0;

    /// Returns the frame rate in Hz
    /// @return the frame rate in Hz
    ///
    virtual double getFrameRate() = 0;

    /// Returns the selected channel number
    /// @return the selected channel number
    ///
    virtual unsigned long getSelectedChannel() const;

    /// Sets the channel to read
    /// @param c the channel number
    ///
    virtual void setSelectedChannel(unsigned long c);

    /// Closes the stream
    ///    
    virtual void close() = 0;

    virtual unsigned long getSourceCount() = 0;

  protected :

    unsigned long _selectedChannel;
  };

} // end namespace alize

#endif // !defined(ALIZE_AudioInputStream_h)

