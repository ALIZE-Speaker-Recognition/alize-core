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

#if !defined(ALIZE_FeatureComputer_h)
#define ALIZE_FeatureComputer_h

#ifdef WIN32
#pragma warning( disable : 4250 )
#endif

#include "FeatureInputStream.h"
#include "AudioInputStream.h"
#include "AudioFrame.h"
#define _sig_c_
extern "C"
{
#include "spro.h"
}

namespace alize
{
  class FileReader;
  class Config;

  class FeatureComputer : public FeatureInputStream
  {
    friend class TestFeatureComputer;

  public :


    /// Build a FeatureComputer object
    /// @param s audio stream to read
    /// @param c config to use
    
    FeatureComputer(AudioInputStream& s, const Config& c);

    /// Reset the object. See constructor with same parameters
    ///
    void init(AudioInputStream& s, const Config& c);
    
    /// See constructor with same parameters
    ///
    static FeatureComputer& create(AudioInputStream& s, const Config& c);

    virtual ~FeatureComputer();

    /// @param f the feature to store the data into
    /// @param step
    ///
    virtual bool readFeature(Feature& f, unsigned long step = 1);
    virtual void reset();

    /// @param featureNbr
    /// @param srcName
    ///
    virtual void seekFeature(unsigned long featureNbr,
                             const String& srcName = "");
    virtual unsigned long getFeatureCount();
    virtual unsigned long getVectSize();
    virtual const FeatureFlags& getFeatureFlags();
    virtual real_t getSampleRate();
    virtual unsigned long getSourceCount();

    /// @param srcIdx
    ///
    virtual unsigned long getFeatureCountOfASource(unsigned long srcIdx);

    /// @param srcName
    ///
    virtual unsigned long getFeatureCountOfASource(const String& srcName);

    /// @param srcIdx
    ///
    virtual unsigned long getFirstFeatureIndexOfASource(unsigned long srcIdx);

    /// @param srcName
    ///
    virtual unsigned long getFirstFeatureIndexOfASource(const String& srcName);

    /// @param srcIdx
    ///
    virtual const String& getNameOfASource(unsigned long srcIdx);
    virtual void close();

    virtual String getClassName() const;

    bool operator==(const FeatureComputer&)
                         const; /*!Not implemented*/
    bool operator!=(const FeatureComputer&)
                         const; /*!Not implemented*/
    const FeatureComputer& operator=(
             const FeatureComputer&); /*!Not implemented*/
    FeatureComputer(
             const FeatureComputer&); /*!Not implemented*/
  
  private :

    unsigned long     _featureIndex;
    AudioFrame        _audioFrame;
    AudioInputStream* _pAudioInputStream;
    sigbuf_t*         _pBuf;
    sample_t*         _buf;
    float*            _w;
    spsig_t*          _s;
    spf_t*            _e;
    spf_t*            _c;
    float*            _r;
    unsigned short*   _idx;
    unsigned long     _l;
    unsigned long     _d;

    /* ---------------------------------------- */
    /* ----- FFT and DCT global variables ----- */
    /* ---------------------------------------- */

    float*            w1c;
    float*            w3c;
    float*            _fftbuf;
    float             _dctz;
    float**           _dctk;
    long*             jx0;
    unsigned long     _fftn;
    int               _fftm;
    int               _dctnin;
    int               _dctnout;
    FeatureFlags      _flags; // flags S SE D DE DD DDE
    unsigned long     _bp;  // current position in the stream buffer
    double            _prev; // pre-emphasis filter memory
    bool              _fft_initialized;
    bool              _dct_initialized;

    unsigned long     _nread;
    real_t            _sampleRate;
    unsigned long     _vectSize;

    unsigned int      _ibs;
    unsigned long     _numceps; // number of cepstral coefficients
    float             _emphco; // pre-emphasis coefficient
    float             _fm_d; // frame shift in ms
    float             _fm_l; // frame length in ms
    int               _win;  // weighting window
    unsigned short    _nfilters; // number of filters in the filter-bank
    float             _alpha; // spectral resolution coefficient
    int               _usemel; // use MEL frequency scale ?
    float             _f_min; // lower frequency bound
    float             _f_max; // higher frequency bound
    int               _fftnpts; // FFT length
    int               _lifter; // liftering value

    void freeMem();

    /* functions copied from file fft.c - SPRO toolkit -
       Copyright (C) 2002 Guillaume Gravier (ggravier@irisa.fr)
       cvs log: $Date$
                $Revision$
    */    
    unsigned long   sig_stream_read();
    int             get_next_sig_frame(int ch, int l, int d, float a, sample_t *s);
    int             fft_init(unsigned long npts);
    int             fft(spsig_t *s, float *m, float *ph);
    void            _fft(float *x, int m);
    void            _brx(float *x, int m);
    int             dct_init(unsigned short nin, unsigned short nout);
    int             dct(spf_t *ip, spf_t *op);
    unsigned short* set_alpha_idx(unsigned short n, float a, float fmin, float fmax);
    unsigned short* set_mel_idx(unsigned short n, float fmin, float fmax, float srate);
    float           theta(float o, float a);
    float           theta_inv(float oop, float a);
    float           mel(float f);
    float           mel_inv(float f);
    int             log_filter_bank(spsig_t *x, unsigned short nfilt, unsigned short *idx, spf_t *e);
  };

} // end namespace alize

#endif // !defined(ALIZE_FeatureComputer_h)


