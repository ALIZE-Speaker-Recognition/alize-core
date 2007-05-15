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

#if !defined(ALIZE_FeatureComputer_cpp)
#define ALIZE_FeatureComputer_cpp

#ifdef WIN32
#pragma warning( disable : 4291 4127 4702)
#endif

#include "FeatureComputer.h"
#include "Exception.h"
#include "Config.h"
#include "Feature.h"
#include <new>
#include <memory>
#include <cmath>
#include <iostream>
#include "spro.h"

#define OM_EPSILON 0.00001
#define round(x) ((int)ceil(x) - x < x - (int)floor(x)) ? (int)ceil(x) : (int)floor(x)

using namespace std;
using namespace alize;
typedef FeatureComputer R;

/*
--featureComputerNumCeps     integer - number of cepstral coefficients (12)
--featureComputerPreEmphasis float - pre-emphasis coefficient (0.95)
--featureComputerShift       float - frame shift in ms (10.0)
--featureComputerLength      float - frame length in ms (20.0)
--featureComputerWindows     string - weighting window (HAMMING)
--featureComputerNumFilters  integer - number of filters in the filter-bank (24)
--featureComputerAlpha       float - frequency warping parameter (0.0)
--featureComputerMel         boolean - use MEL frequency scale (false)
--featureComputerFreqMin     float - lower frequency bound (0 Hz)
--featureComputerFreqMax     float - higher frequency bound (0 Hz)
--featureComputerFFTLength   integer - FFT length (512)
--featureComputerLifter      integer - liftering value (0)
--featureComputerEnergy      boolean - add log-energy (false)
*/
//-------------------------------------------------------------------------
R::FeatureComputer(AudioInputStream& a, const Config& c)
:_pBuf(NULL), _buf(NULL), _w(NULL), _s(NULL), _e(NULL), _c(NULL), _r(NULL),
 _idx(NULL), _fft_initialized(false), _dct_initialized(false) {init(a, c);}
//-------------------------------------------------------------------------
R& R::create(AudioInputStream& a, const Config& c)
{
  R* p = new (std::nothrow) R(a, c);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void R::init(AudioInputStream& a, const Config& c)
{
  freeMem();
  _pAudioInputStream = &a;
  _pAudioInputStream->reset();
  _featureIndex = 0;
  w1c = w3c = NULL;
  jx0 = NULL;
  _fftbuf = NULL;
  _dctk = NULL;
  _bp = _fftn = _fftm = _dctnin = _dctnout = 0;
  _fft_initialized = _dct_initialized = false;
  _prev = _dctz = 0.0;
  _flags.set("100000");

  // parameter default values

  _ibs = 10000*1000; // 10000 ko
  _numceps = 12;
  _emphco = 0.95f;
  _fm_d = 10.0; // ms
  _fm_l = 20.0; // ms
  _win = SPRO_HAMMING_WINDOW;
  _nfilters = 24;
  _alpha = 0.0;
  _usemel = 0;
  _f_min = 0.0;
  _f_max = 0.0;
  _fftnpts = 512;
  _lifter = 0;

  // number of cepstral coefficients

  String o = "featureComputerNumCeps";
  if (c.existsParam(o))
  {
    long x = c.getIntegerParam(o);
    if (x < 1)
      throw Exception("Null number of cepstral coefficients", __FILE__, __LINE__);
    _numceps = x;
  }

  // pre-emphasis coefficient

  o = "featureComputerPreEmphasis";
  if (c.existsParam(o))
  {
    float x = c.getFloatParam(o);
    if (x < 0.0 || x >= 1.0)
      throw Exception("Invalid pre-emphasis coefficient "
        + String::valueOf(x) + " (not in [0,1[)", __FILE__, __LINE__);
    _emphco = x;
  }

  // frame shift in ms

  o = "featureComputerShift";
  if (c.existsParam(o))
  {
    float x = c.getFloatParam(o);
    if (x <= 0.0)
      throw Exception("Invalid shift "
          + String::valueOf(x) + ": (must be > 0.0 ms)", __FILE__, __LINE__);
    _fm_d = x;
  }

  // frame length in ms

  o = "featureComputerLength";
  if (c.existsParam(o))
    _fm_l = c.getFloatParam(o);

  // weighting window

  o = "featureComputerWindow";
  if (c.existsParam(o))
  {
    const String& x = c.getParam(o);
    if (x == "none" || x == "NONE")
	    _win = SPRO_NULL_WINDOW;
    else if (x == "hamming" || x == "HAMMING")
	    _win = SPRO_HAMMING_WINDOW;
    else if (x == "hanning" || x == "HANNING")
	    _win = SPRO_HANNING_WINDOW;
    else if (x == "blackman" || x == "BLACKMAN")
	    _win = SPRO_BLACKMAN_WINDOW;
    else
      throw Exception("unknown window type " + x
        + ". Must be one of NONE, HAMMING, HANNING or BLACKMAN ",
        __FILE__, __LINE__);
  }

  // number of filters in the filter-bank

  o = "featureComputerNumFilters";
  if (c.existsParam(o))
  {
    long x = c.getIntegerParam(o);
    if (x < SPRO_MIN_FILTERS || x > SPRO_MAX_FILTERS)
      throw Exception("invalid number of filters " + String::valueOf(x)
        + " (not in ["+String::valueOf(SPRO_MIN_FILTERS)+","+String::valueOf(SPRO_MAX_FILTERS)+"])",
        __FILE__, __LINE__);
    _nfilters = x;
  }

  // spectral resolution coefficient

  o = "featureComputerAlpha";
  if (c.existsParam(o))
  {
    float x = c.getFloatParam(o);
    if (x <= -1 || x >= 1)
      throw Exception("Invalid spectral resolution coefficient "
        + String::valueOf(x) + " (not in ]-1,1[)", __FILE__, __LINE__);
    _alpha = x;
  }

  // use Usemel scale ?

  if (c.existsParam("featureComputerMel"))
    _usemel = c.getBooleanParam(o);

  // lower frequency bound

  o = "featureComputerFreqMin";
  if (c.existsParam(o))
    _f_min = c.getFloatParam(o);

  // higher frequency bound

  o = "featureComputerFreqMax";
  if (c.existsParam(o))
    _f_max = c.getFloatParam(o);

  // FFT length

  o = "featureComputerFFTLength";
  if (c.existsParam(o))
  {
    long x = c.getIntegerParam(o);
    if (x < SPRO_MIN_FFT_SIZE || x > SPRO_MAX_FFT_SIZE)
      throw Exception("invalid FFT length " + String::valueOf(x)
        + " (not in ["+String::valueOf(SPRO_MIN_FFT_SIZE)+","+String::valueOf(SPRO_MAX_FFT_SIZE)+"])",
        __FILE__, __LINE__);
    _nfilters = x;
  }

  // liftering value

  o = "featureComputerLifter";
  if (c.existsParam(o))
    _lifter = c.getIntegerParam(o);

  // add log-energy

  o = "featureComputerEnergy";
  if (c.existsParam(o))
    _flags.useE = c.getBooleanParam(o);
  
  // ----- basic argument sanity check -----

  if (_usemel && _alpha != 0.0)
    throw Exception("warning -- ignoring alpha option with mel specified", __FILE__, __LINE__);

  if (_f_max != 0.0 && _f_min != 0.0 && _f_max <= _f_min)
    throw Exception("invalid frequency range specification", __FILE__, __LINE__);

  if (_numceps > _nfilters)
    throw Exception("number of cepstral coefficients " + 
      String::valueOf(_numceps) + " > number of filters " +
      String::valueOf(_nfilters), __FILE__, __LINE__); 

  if (_fm_l < _fm_d)
    throw Exception("warning -- using non overlapping frames", __FILE__, __LINE__);

  _vectSize = (_flags.useE) ? _numceps + 1 : _numceps;

  _sampleRate = 1000.0/(real_t)_fm_d;

  _pBuf = sig_buf_alloc(_ibs, _pAudioInputStream->getSampleBytes());
  if (_pBuf == NULL)
    throw Exception("", __FILE__, __LINE__);
  _nread = 0;

  /* ----- initialize necessary stuff ----- */
  if ((fft_init(_fftnpts)) != 0)
    throw Exception("sfbcep error -- cannot initialize FFT with "
      + String::valueOf(_fftnpts) + " points", __FILE__, __LINE__);
  _fft_initialized = true;

  if ((dct_init(_nfilters, _numceps)) != 0)
  {
    throw Exception("sfbcep error -- cannot initialize "
      + String::valueOf(_nfilters) + " x "
      + String::valueOf(_numceps) + " DCT kernel", __FILE__, __LINE__);
  }
  _dct_initialized = true;

  /* ----- run cepstral analysis ----- */

  /* ----- initialize some more stuff ----- */
  _l = (unsigned long)(_fm_l * _pAudioInputStream->getFrameRate() / 1000.0); /* frame length in samples */
  _d = (unsigned long)(_fm_d * _pAudioInputStream->getFrameRate() / 1000.0); /* frame shift in samples */

  if ((_s = sig_alloc(_l)) == NULL) /* frame signal */
    throw Exception("Error in cepstral analysis", __FILE__, __LINE__);

  if (_win)
  {
    if ((_buf = (sample_t *)malloc(_l * sizeof(sample_t))) == NULL) /* frame buffer */
      throw Exception("Error in cepstral analysis", __FILE__, __LINE__);
    if ((_w = set_sig_win(_l, _win)) == NULL)
      throw Exception("Error in cepstral analysis", __FILE__, __LINE__);
  }
  else
    _buf = _s->s;

  if ((_e = (spf_t *)malloc(_nfilters * sizeof(spf_t))) == NULL) { /* filter-bank output */
    throw Exception("Error in cepstral analysis", __FILE__, __LINE__);
  }

  if ((_c = (spf_t *)malloc((_numceps+1) * sizeof(spf_t))) == NULL)
    throw Exception("Error in cepstral analysis", __FILE__, __LINE__);

  if (_lifter)
    if ((_r = set_lifter(_lifter, _numceps)) == NULL)
    throw Exception("Error in cepstral analysis", __FILE__, __LINE__);

  if (_usemel) {
    if ((_idx = set_mel_idx(_nfilters, _f_min / _pAudioInputStream->getFrameRate(), _f_max / _pAudioInputStream->getFrameRate(), _pAudioInputStream->getFrameRate())) == NULL)
      throw Exception("Error in cepstral analysis", __FILE__, __LINE__);
  }
  else if ((_idx = set_alpha_idx(_nfilters, _alpha, _f_min / _pAudioInputStream->getFrameRate(), _f_max / _pAudioInputStream->getFrameRate())) == NULL)
    throw Exception("Error in cepstral analysis", __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
bool R::readFeature(Feature& f, unsigned long step)
{
  if (step != 1)
    throw Exception("Argument 'step' must be 1", __FILE__, __LINE__);

  if (!get_next_sig_frame(1 /*channel*/, _l, _d, _emphco, _buf))
    return false; // EOF

  /* weight signal */
  if (_win)
    sig_weight(_s, _buf, _w);

  /* apply the filter bank */
  if ((log_filter_bank(_s, _nfilters, _idx, _e)) != 0)
    throw Exception("Error in cepstral analysis", __FILE__, __LINE__);

  /* DCT */

  if ((dct(_e, _c)) != 0)
    throw Exception("Error in cepstral analysis", __FILE__, __LINE__);

  /* liftering */
  if (_lifter)
    for (unsigned long j = 0; j<_numceps; j++)
	    *(_c+j) *= *(_r+j);

  /* energy */
  if (_flags.useE)
  {
    double energy;
    if ((energy = sig_normalize(_s, 0)) < SPRO_ENERGY_FLOOR)
	    energy = SPRO_ENERGY_FLOOR;
    *(_c+_numceps) = (spf_t)(2.0 * log(energy));
  }

  f.setVectSize(K::k, _vectSize);
  for (unsigned long i=0; i<_vectSize; i++)
    f[i] = _c[i];
  f.setValidity(true);
  _featureIndex++;
  return true;
  //cout << f.toString() << endl;
}
//-------------------------------------------------------------------------
void R::reset() { seekFeature(0); }
//-------------------------------------------------------------------------
void R::close()
{
  if (_pAudioInputStream != NULL)
    _pAudioInputStream->close(); //????
}
//-------------------------------------------------------------------------
void R::seekFeature(unsigned long featureNbr, const String& srcName)
{
  if (!srcName.isEmpty())
    throw Exception("cannot seek if argument 'srcName' is not empty",
    __FILE__, __LINE__);
  if (featureNbr == _featureIndex)
    return; 
  throw Exception("cannot seek if argument 'featureNbr' != 0",
    __FILE__, __LINE__);
}
//-------------------------------------------------------------------------
unsigned long R::getFeatureCount()
{
 real_t v = (_pAudioInputStream->getFrameCount()/_pAudioInputStream->getFrameRate()
 - _fm_l/1000.0)*_sampleRate;
 return static_cast<unsigned long>(v+0.5); // arrondi à l'entier inférieur
}
//-------------------------------------------------------------------------
real_t R::getSampleRate() { return _sampleRate; }
//-------------------------------------------------------------------------
const FeatureFlags& R::getFeatureFlags() { return _flags; }
//-------------------------------------------------------------------------
unsigned long R::getVectSize() { return _vectSize; }
//-------------------------------------------------------------------------
unsigned long R::getSourceCount()
{ return _pAudioInputStream->getSourceCount(); }
//-------------------------------------------------------------------------
unsigned long R::getFeatureCountOfASource(unsigned long srcIdx)
{
  if (srcIdx != 0)
    throw Exception("Only 1 file available", __FILE__, __LINE__);
  return getFeatureCount();
}
//-------------------------------------------------------------------------
unsigned long R::getFeatureCountOfASource(const FileName& f)
{
  throw Exception("Forbidden method for this kind of object",
                   __FILE__, __LINE__);
  return 0;
}
//-------------------------------------------------------------------------
unsigned long R::getFirstFeatureIndexOfASource(unsigned long srcIdx)
{
  if (srcIdx != 0)
    throw Exception("Only 1 file available", __FILE__, __LINE__);
  return 0;
}
//-------------------------------------------------------------------------
unsigned long R::getFirstFeatureIndexOfASource(const FileName& f)
{
  throw Exception("Forbidden method for this kind of object",
                   __FILE__, __LINE__);
  return 0;
}
//-------------------------------------------------------------------------
const String& R::getNameOfASource(unsigned long srcIdx)
{
  throw Exception("Forbidden method for this kind of object",
                   __FILE__, __LINE__);
#ifdef WIN32
  static const String x;
  return x;
#endif
}
//-------------------------------------------------------------------------
int R::get_next_sig_frame(int ch, int l, int d, float a, sample_t *s)
{
  unsigned long nread;          /* number of samples read in buffer         */
  unsigned short i, j;
  void *p;
  double v;

  if (_nread == 0) { /* first call ==> we have to read completely the first frame */
    _bp = 0;
    _prev = 0.0;
    j = 0;
  }
  else /* next calls ==> reuse l-d samples and add d new samples */
    for (i = d, j = 0; i < l; i++, j++)
      *(s+j) = *(s+i);

  nread = 1; /* ugly trick to get into the while loop ;) */
  p = (short*)(_pBuf->s) + (ch - 1) * _pAudioInputStream->getSampleBytes();

  while (j < l && nread) {

    if (_bp == _pBuf->n) { /* this means buffer is empty! */
      nread = sig_stream_read();
      _bp = 0;
    }
    
    if (nread)
      while (j < l && _bp < _pBuf->n)
      {
        v = getsample(p, _bp, _pAudioInputStream->getSampleBytes());
      	*(s+j) = (sample_t)(v - a * _prev);
        _prev = v;
        j++;
        _bp += 1;
      }
    else /* we failed to read additionnal samples */
      return(0);
  }

  return(1);
}
//-------------------------------------------------------------------------
unsigned long R::sig_stream_read()
{
  short* p = (short*)(_pBuf->s);
  unsigned long nread = 0, i;

  // m = sample count
  // nbps = number of bytes per sample
  // f = FILE object to read
  // s = buffer 
  //nread = fread(f->buf->s, f->nbps, f->buf->m, f->f);
  for (i=0; i<_pBuf->m && _pAudioInputStream->readFrame(_audioFrame); i++)
  {
    p[i] = _audioFrame.getData();
    nread++;
  }
  _pBuf->n = nread;
  _nread += nread;
  return nread;
}
//-------------------------------------------------------------------------
/* --------------------------------------- */
/* ----- int fft_init(unsigned long) ----- */
/* --------------------------------------- */
/*
 * Initialize and reset FFT kernel.
 */
int R::fft_init(unsigned long npts)
{
  int m;
  int i, j, ip, nb, lnb, llnb, n2, n4, n6, n8, n12, n16;
  double angd, c, s;
  float ang;
  if (npts) {
    if (frexp((double)npts, &m) != (double)0.5) {
      fprintf(stderr, "[SPro error %d] FFTInit(): FFT length (%lu) is not a power of two", SPRO_FFT_INIT_ERR, npts);
      return(SPRO_FFT_INIT_ERR);
    }
    m--;
    
    if ((_fftbuf = (float *)realloc(_fftbuf, npts * sizeof(float))) == NULL) {
      fprintf(stderr, "[SPro error %d] FFTInit(): cannot allocate FFT buffer", SPRO_ALLOC_ERR);
      return(SPRO_ALLOC_ERR);
    }

    n2 = npts / 2;
    n4 = npts / 4;
    n6 = npts / 6;
    n8 = npts / 8;
    n12 = npts / 12;
    n16 = npts / 16;
    
    if ((w1c = (float *)realloc(w1c, n4 * sizeof(float))) == NULL) {
      fprintf(stderr, "[SPro error %d] FFTInit(): cannot allocate FFT kernel", SPRO_ALLOC_ERR);
      free(_fftbuf);
      return(SPRO_ALLOC_ERR);
    }
    
    if ((w3c = (float *)realloc(w3c, n4 * sizeof(float))) == NULL) {
      fprintf(stderr, "[SPro error %d] FFTInit(): cannot allocate FFT kernel", SPRO_ALLOC_ERR);
      free(_fftbuf); free(w1c);
      return(SPRO_ALLOC_ERR);
    }
    
    if ((jx0 = (long *)realloc(jx0, npts * sizeof(long) / 3)) == NULL) {
      fprintf(stderr, "[SPro error %d] FFTInit(): cannot allocate FFT kernel", SPRO_ALLOC_ERR);
      free(_fftbuf); free(w1c); free(w3c);
      return(SPRO_ALLOC_ERR);
    }
    
    ang = PI2 / (float)npts;
    angd = PI2 / (double)npts;
    c = cos(angd);
    s = sin(angd);
    w1c[1] = c;
    w1c[n4-1] = s;
    w1c[npts/8] = 0.707106781186547;
    for (i = 2; i <= n16; i++) {
      w1c[i] = w1c[i-1] * c - w1c[n4-i+1] * s;
      w1c[n4-i] = w1c[n4-i+1] * c + w1c[i-1] * s;
      w1c[n8+i-1] = w1c[n8+i-2] * c - w1c[n8-i+2] * s;
      w1c[n8-i+1] = w1c[n8-i+2] * c + w1c[n8+i-2] * s;
    }
    for (i = 1; i <= n12; i++)
      w3c[i] = w1c[3*i];
    for (i = n12 + 1; i <= n6; i++)
      w3c[i] = -w1c[n2-3*i];
    for (i = n6 + 1; i <= n4 - 1; i++)
      w3c[i] = -w1c[3*i-n2];
    jx0[0] = 0;
    jx0[1] = 0;
    jx0[2] = 0;
    jx0[3] = n2;
    jx0[4] = 3 * n4;
    ip = 5;
    nb = 3;
    lnb = 1;
    for (i = 1; i <= m - 4; i++) {
      for (j = 0; j < nb; j++)
	jx0[ip+j] = jx0[ip-nb+j] / 2;
      ip = ip + nb;
      for (j = 0; j < lnb; j++) {
	jx0[ip+j] = jx0[ip-nb-nb-lnb+j] / 4 + n2;
	jx0[ip+j+lnb] = jx0[ip+j] + n4;
      }
      ip = ip+lnb+lnb;
      llnb = lnb;
      lnb = nb;
      nb = lnb + llnb + llnb;
    }

    _fftn = npts;
    _fftm = m;

  }
  else {
    if (_fftbuf) {
      free(_fftbuf);
      free(w1c);
      free(w3c);
      free(jx0);
    }
    _fftn = _fftm = 0;
  }

  return(0);
}
//-------------------------------------------------------------------------
/* ------------------------------------------------ */
/* ----- int fft(spsig_t *, float *, float *) ----- */
/* ------------------------------------------------ */
/*
 * Performs FFT on "signal" x -- either module or phase can be NULL 
 * if one is not interested in this result.
 */
int R::fft(spsig_t *s, float *m, float *ph)
{
  int i, j, n2;
  sample_t *p = s->s;
  float a, b;

  n2 = _fftn >> 1; /* _fftn / 2 */

  /* ----- sanity check ----- */
  if (_fftn == 0) {
    fprintf(stderr, "fft(): FFT kernel uninitialized");
    return(SPRO_KERNEL_INIT_ERR);
  }

  /* ----- copy signal to buffers and apply fft ----- */
  for (i = 0; i < (int)(s->n) && i < (int)_fftn; i++)
    *(_fftbuf+i) = (float)*(p+i);

  for (; i < (int)_fftn; i++)
    *(_fftbuf+i) = (float)0.0;
  
  _brx(_fftbuf, _fftm);
  _fft(_fftbuf, _fftm);

 
  /* ----- compute modulus and phase ----- */
  if (m || ph) {
    if (m)
      *m = (spf_t)fabs(*_fftbuf);
    if (ph)
      *ph = (spf_t)0.0;

    for (i = 1, j = (int)_fftn - 1; i < n2; i++, j--) {
      a = *(_fftbuf+i);
      b = *(_fftbuf+j);
      if (m)
      {
	*(m+i) = (float)sqrt(a * a + b * b);
      }
      if (ph)
	*(ph+i) = (float)atan(b / a);
    }
  }
  
  return(0);
}
//-------------------------------------------------------------------------
/* ----------------------------------- */
/* ----- void _brx(float *, int) ----- */
/* ----------------------------------- */
/*
 * Rearranges data in the FFT buffer - see _fft() for comments.
 */
void R::_brx(float *x, int m)
{
  int n, n1, m1, i, ipair, ibr, j, jbr, jbri, k, ia1, ia2, ia3, nh;
  int b;
  float xt;
  
  n = 1 << m;
  m1 = m / 2;
  n1 = 1 << m1;
  ia1 = n1 / 2;
  ia2 = n / n1;
  ia3 = ia1 + ia2;
  nh = n / 2;

  b = (m - m1 - m1) * n1;
  for (ipair = 0; ipair <= b; ipair += n1) {
    ibr = 0;
    xt = x[ipair+ia1];
    x[ipair+ia1] = x[ipair+ia2];
    x[ipair+ia2] = xt;
    for (i = 1 + ipair; i < ia1 + ipair; i++) {
      k = nh;
      if (k <= ibr)
	do {
	  ibr -= k;
	  k = k/2;
	}
	while (k <= ibr);
      ibr += k;
      xt = x[ibr+i+ia1];
      x[ibr+i+ia1] = x[ibr+i+ia2];
      x[ibr+i+ia2] = xt;
      jbr = 0;
      
      if (m < 4) 
	continue;
      
      for (j = ibr + ipair; j < ibr + i; j++) {
	jbri = jbr + i;
	xt = x[jbri];
	x[jbri] = x[j];
	x[j] = xt;
	xt = x[jbri+ia1];
	x[jbri+ia1] = x[j+ia2];
	x[j+ia2] = xt;
	xt = x[jbri+ia2];
	x[jbri+ia2] = x[j+ia1];
	x[j+ia1] = xt;
	xt = x[jbri+ia3];
	x[jbri+ia3] = x[j+ia3];
	x[j+ia3] = xt;
	k = nh;
	if(k <= jbr)
	  do {
	    jbr -= k;
	    k = k/2;
	  }
	  while (k <= jbr);
	jbr += k;
      }
    }
  }
}
//-------------------------------------------------------------------------
/* ----------------------------------- */
/* ----- void _fft(float *, int) ----- */
/* ----------------------------------- */
/*
 * perform FFT after rearrangments. 

 * This piece of code was kindly contributed by Pierre Duhamel and implements
 * the algorithm described in P. Duhamel and M. Vetterli, "Improved Fourier
 * and Hartley Transform Algorithms: Application to CycliC Convolution of Real
 * Data", IEEE Trans on ASSP, 35(6), June 1987.
 *
 * NOTE: this code could be subsequently speed up by using some simple
 * C tricks (*(p+i) instead of p[i], temp variable for storing p[i] values, etc).
 */
void R::_fft(float *x, int m)
{
  int i, i0, i1, i2, i3, i4, i5, i6, i7, ib, istep, ia0, ia1, ia2, ia3;
  int n, ib0, ib1, ib2, ib3, j, jstep, n2, n4 ,n8, nd4, nb, lnb, llnb, k, sgn;
  float c2, c3, d2, d3, r1, r2, r3, r4, t0, t1, t2;
  const float rac2s2 = 0.707106781186547;
  
  n = 1 << m;
  nd4 = n / 4;
  sgn = ((m%2) == 0) ? 1 : -1;
  nb = (n / 2 + sgn) / 3;
  lnb = (n - sgn) / 3;
  ib = n / 6;

  for (i = ib; i < ib + nb; i++) {
    i0 = jx0[i];
    i1 = i0 + 1;
    i2 = i1 + 1;
    i3 = i2 + 1;
    r1 = x[i0] + x[i1];
    t0 = x[i2] + x[i3];
    x[i3] = x[i3] - x[i2];
    x[i1] = x[i0] - x[i1];
    x[i2] = r1 - t0;
    x[i0] = r1 + t0;
    //printf("%1.18f %1.18f %1.18f\n", (double)r1, (double)t0, (double)(x[i0]- ( r1 + t0)));
  }
  llnb = lnb;
  lnb = nb;
  nb = (llnb - lnb) / 2;
  ib = ib - nb;
  
  for (i = ib; i < ib + nb; i++) {
    i0 = jx0[i];
    i4 = i0 + 4;
    i5 = i0 + 5;
    i6 = i0 + 6;
    i7 = i0 + 7;
    r1 = x[i4] - x[i5];
    r3 = x[i4] + x[i5];
    r2 = x[i7] - x[i6];
    r4 = x[i6] + x[i7];
    t0 = r3 + r4;
    x[i6] = r4 - r3;
    x[i4] = x[i0] - t0;
    x[i0] = x[i0] + t0;

    t1 = (r1 + r2) * rac2s2;
    t2 = (r2 - r1) * rac2s2;
    i3 = i0 + 3;
    x[i5] = t2 - x[i3];
    x[i7] = t2 + x[i3];
    i1 = i0 + 1;
    x[i3] = x[i1] - t1;
    x[i1] = x[i1] + t1;
  }

  istep = n / 16;
  n8 = 1;
  n4 = 2;
  n2 = 4;
  
  for (k = 4; k <= m; k++) {
    llnb = lnb;
    lnb = nb;
    nb = (llnb - lnb) / 2;
    ib = ib - nb;
    n8 = n4;
    n4 = n2;
    n2 = n2 + n2;
    
    for (i = ib; i < ib + nb; i++) {
      i0 = jx0[i];
      i1 = i0 + n4;
      i2 = i1 + n4;
      i3 = i2 + n4;
      t0 = x[i2] + x[i3];
      x[i3] = -x[i2] + x[i3];
      x[i2] = x[i0] - t0;
      x[i0] = x[i0] + t0;

      i0 = i0 + n8;
      i1 = i0 + n4;
      i2 = i1 + n4;
      i3 = i2 + n4;
      t1 = (x[i2] - x[i3]) * rac2s2;
      t2 = (x[i2] + x[i3]) * rac2s2;
      x[i2] = -t2 - x[i1];
      x[i3] = -t2 + x[i1];
      x[i1] = x[i0] - t1;
      x[i0] = x[i0] + t1;
    }
    
    if (n4 < 4) 
      continue;
    
    for (i = ib; i < ib + nb; i++) {
      jstep = 0;
      for (j = 1; j <= n8 - 1; j++) {
	jstep = jstep + istep;
	ia0 = jx0[i] + j;

	ia2 = ia0 + n2;
	ib2 = ia2 + n4 - j - j;
	c2 = x[ia2] * w1c[jstep] + x[ib2] * w1c[nd4-jstep];
	d2 = -x[ia2] * w1c[nd4-jstep] + x[ib2] * w1c[jstep];
	ia3 = ia2 + n4;
	ib3 = ib2 + n4;
	c3 = x[ia3] * w3c[jstep] - x[ib3] * w3c[nd4-jstep];
	d3 = x[ia3] * w3c[nd4-jstep] + x[ib3] * w3c[jstep];
	ib1 = ia0 + n4;
	t1 = c2 + c3;
	c3 = c2 - c3;
	x[ib2] = -x[ib1] - c3;
	x[ia3] = x[ib1] - c3;
	t2 = d2 - d3;
	ia1 = ib1 - j - j;
	x[ib1] = x[ia1] + t2;
	x[ia1] = x[ia1] - t2;
	d3 = d2 + d3;
	ib0 = ia1 + n4;
	x[ia2] = -x[ib0] + d3;
	x[ib3] = x[ib0] + d3;
	x[ib0] = x[ia0] - t1;
	x[ia0] = x[ia0] + t1;
      }
    }
    istep = istep / 2;
  }
}
//-------------------------------------------------------------------------
/* -------------------------------------------------------- */
/* ----- int dct_init(unsigned short, unsigned short) ----- */
/* -------------------------------------------------------- */
/*
 * Initialize or reset DCT kernel
 */
int R::dct_init(unsigned short nin, unsigned short nout)
{
  float *kp;
  unsigned short i, j;

  if (nin && nout) {
    float** dctk = _dctk;
    if ((_dctk = (float **)realloc(_dctk, nout * sizeof(float *))) == NULL) {
      fprintf(stderr, "[SPro erro r%d] DCTInit(): cannot allocate DCT kernel", SPRO_ALLOC_ERR);
      return(SPRO_ALLOC_ERR);
    }
    
    for (i = 0; i < nout; i++) {
      if (dctk == NULL)
        _dctk[i] = NULL;
      if ((_dctk[i] = (float *)realloc(_dctk[i], nin * sizeof(float))) == NULL) {
	fprintf(stderr, "[SPro error %d] DCTInit(): cannot allocate DCT kernel", SPRO_ALLOC_ERR);
	while(i)
	  free(_dctk[--i]);
	free(_dctk);
	return(SPRO_ALLOC_ERR);
      }
      
      kp = *(_dctk+i);
      for (j = 0; j < nin; j++)
      	*(kp+j) = (float)cos(XX_PI * (i + 1.0) * (j + 0.5) / nin);
      

    }
    
    _dctz = (float)sqrt(2.0 / nin);
    _dctnin = nin;
    _dctnout = nout;
  }
  else {
    if (_dctk) {
      for (i = 0; i < _dctnout; i++)
	if (*(_dctk+i))
	  free(*(_dctk+i));
      free(_dctk);
    }
    _dctnin = _dctnout = 0;
  }

  return(0);
}
//-------------------------------------------------------------------------
/* ------------------------------------- */
/* ----- int dct(spf_t *, spf_t *) ----- */
/* ------------------------------------- */
/*
 * Apply DCT according to 
 *
 *  c[i]=sqrt(2/N) * sum_{j=1}^{N}(m[j] * cos(XX_PI*i*(j-0.5)/N)     i=1,...,p
 */
int R::dct(spf_t *ip, spf_t *op)
{
  int i, j;
  double v;
  float *kp;
  
  if (! _dctnout) {
    fprintf(stderr, "fft(): DCT kernel uninitalized");
    return(SPRO_KERNEL_INIT_ERR);    
  }

  for (i = 0; i < _dctnout; i++) {
    kp = *(_dctk+i);
    v = 0.0;
    for (j = 0; j < _dctnin; j++)
    {
      v += ( (*(ip+j)) * (*(kp+j)) );
    *(op+i) = (spf_t)(v * _dctz);
    //if (j==6)
         //printf("%1.18f %1.18f\n", (*(ip+j)), (*(kp+j)) );

    }
  }

  return(0);
}
/* ------------------------------------------------------------------------------ */
/* ----- unsigned short *set_alpha_idx(unsigned short, float, float, float) ----- */
/* ------------------------------------------------------------------------------ */
/*
 * Set cut-off indexes on a alpha-transformed scale. Input buffer length is
 * taken from the initialized FFT kernel.
 */
unsigned short* R::set_alpha_idx(unsigned short n, float a, float fmin, float fmax)
{
  unsigned short *idx, i;
  float o, d, omin, omax, z;

  if ((idx = (unsigned short *)malloc((n + 2) * sizeof(unsigned short))) == NULL) {
    fprintf(stderr, "set_alpha_idx(): cannot allocate memory\n");
    return(NULL);
  }

  if (a <= -1.0 || a >= 1) {
    fprintf(stderr, "set_alpha_idx(): invalid resolution parameter value %f (not in [0,1])\n", a);
    return(NULL);
  }

  if (fmax <= fmin)
    fmax = 0.5;

  if (fmin < 0 || fmin > 0.5 || fmax < 0 || fmax > 0.5) {
    fprintf(stderr, "set_alpha_idx(): invalid frequency range [%f,%f]\n", fmin, fmax);
    return(NULL);
  }

  *idx = (unsigned short)round(2 * fmin * ((float)(_fftn / 2 - 1)));
  *(idx+n+1) = (unsigned short)round(2 * fmax * (float)(_fftn / 2 - 1));
  
  omin = (fmin) ? theta(2.0 * XX_PI * fmin, a) : 0.0; /* pulses in transform domain */
  omax = (fmax < 0.5) ? theta(2.0 * XX_PI * fmax, a) : XX_PI;

  d = (omax - omin) / (float)(n + 1);
  z = (float)((_fftn / 2) - 1) / XX_PI;
  o = omin;

  i = 0;
  for (i = 1; i <= n; i++) {
    o += d;
    *(idx+i) = (unsigned short)round(theta_inv(o, a) * z); /* index in the original domain */
  }

  return(idx);
}
//-------------------------------------------------------------------------
/* ------------------------------------- */
/* ----- float theta(float, float) ----- */
/* ------------------------------------- */
/*
 * The function determines the pulsation value on the transformed
 * axis (omega), assuming a spectral factor alpha.
 * 
 * For details on this, refer to:
 *       "Analyse spectrale a resolution variable", Ph.D. Thesis,
 *       Christian Chouzenoux, ENST
 * In this Ph.D., we are given omega'=theta(omega).
 */
float R::theta(float o, float a)
{
  double v, a2;

  if (a == 0.0)
    return(o);

  if (o == XX_PI)
    return(XX_PI);

  a2 = a * a;
  v = atan(((1.0 - a2) * sin(o)) / ((1.0 + a2) * cos(o) - 2.0 * a));

  return((float)(v < (double)0.0 ? v + XX_PI : v));
}
//-------------------------------------------------------------------------
/* ----------------------------------------- */
/* ----- float theta_inv(float, float) ----- */
/* ----------------------------------------- */
/*
 * The function determines the pulsation value on the original
 * axis (omega), assuming a spectral factor alpha.
 * 
 * For details on this, refer to:
 *       "Analyse spectrale a resolution variable", Ph.D. Thesis,
 *       Christian Chouzenoux, ENST
 *
 * In this Ph.D., we are given omega'=theta(omega). As I don't feel
 * like finding out an analytical solution to the inverse problem,
 * I simply look for a dichotomic solution to the problem by using
 * omega in the range [0,PI], as the transformation is monotonous.
 *
 * oop: original omega'
 * op:  omega'
 * opmem: omega' memory
 *
 * NOTE: If someone feels like programming an analytical solution
 * to the problem, he is welcome! Have fun ...
 *
 */
float R::theta_inv(float oop,float a)
{
  float o, op, oinf, osup;
  double v, a2, b;

  if (a == 0.0)
    return(oop);

  if (oop <= 0.0)
    return(0.0);

  if(oop >= XX_PI)
    return(XX_PI);

  oinf = 0.0;
  osup = XX_PI;
  a2 = a * a;
  b = 2.0 * a;
  do {
    o = oinf + (osup - oinf) / 2.0;
    v = atan(((1.0 - a2) * sin(o)) / ((1.0 + a2) * cos(o) - b));
    op = (v < 0) ? (float)v + XX_PI : (float)v;
    if(op > oop)
      osup = o;
    else
      oinf = o;
  }
  while ((float)fabs(oop - op) > OM_EPSILON);

  return(o);
}
/* ---------------------------------------------------------------------------- */
/* ----- unsigned short *set_mel_idx(unsigned short, float, float, float) ----- */
/* ---------------------------------------------------------------------------- */
/*
 * Set cut-off indexes on a MEL scale. Input buffer length is taken from the
 * initialized FFT kernel.
 */
unsigned short* R::set_mel_idx(unsigned short n, float fmin, float fmax, float srate)
{
  unsigned short *idx, i;
  float f, min, max, d, z;

  if ((idx = (unsigned short *)malloc((n + 2) * sizeof(unsigned short))) == NULL) {
    fprintf(stderr, "set_mel_idx(): cannot allocate memory\n");
    return(NULL);
  }

  if (fmax <= fmin)
    fmax = 0.5;

  if (fmin < 0 || fmin > 0.5 || fmax < 0 || fmax > 0.5) {
    fprintf(stderr, "set_mel_idx(): invalid frequency range [%f,%f]\n", fmin, fmax);
    return(NULL);
  }

  *idx = (unsigned short)round(2 * fmin * ((float)(_fftn / 2 - 1)));
  *(idx+n+1) = (unsigned short)round(2 * fmax * (float)(_fftn / 2 - 1));

  min = mel(fmin * srate); /* bounds and df in transform domain */
  max = mel(fmax * srate); 
  d = (max - min) / (float)(n + 1);
  z = (float)(_fftn / 2 - 1) * 2.0 / srate;
  f = min;

  for (i = 1; i <= n; i++) {
    f += d;
    *(idx+i) = (unsigned short)round(mel_inv(f) * z); /* index in the original domain */
  }

  return(idx);
}
//-------------------------------------------------------------------------
/* ---------------------------- */
/* ----- float mel(float) ----- */
/* ---------------------------- */
float R::mel(float f)
{
  return(2595.0 * log10(1 + f / 700.0));
}
//-------------------------------------------------------------------------
/* -------------------------------- */
/* ----- float mel_inv(float) ----- */
/* -------------------------------- */
/*
 * Return the value corresponding to Mel frequency f on a 
 * linear scale.
 *   Mel(f) = 2595 * log10(1 + f / 700)
 *   mel_inv(f) = 700 * (10 ^ (f / 2595) - 1)
 */
float R::mel_inv(float f)
{
  return((float)(700.0 * (pow(10, f / 2595.0) - 1)));
}
/* ------------------------------------------------------------------------------------- */
/* ----- int log_filter_bank(spsig_t *, unsigned short, unsigned short *, spf_t *) ----- */
/* ------------------------------------------------------------------------------------- */
/*
 * Apply triangular filter bank to module vector and return the log of
 * the energy in each band. Table p_index contains the indexes of the
 * cut-off frequencies. Looks like something like this:
 * 
 *                      filter 2   
 *                   <------------->
 *                filter 1           filter 3
 *             <----------->       <------------->
 *        | | | | | | | | | | | | | | | | | | | | | ..........
 *         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9  ..........
 *             ^     ^     ^       ^             ^
 *            |     |     |       |             |
 *          idx[0]  |  idx[2]    |           idx[4]
 *                idx[1]       idx[3]  
 *
 */
int R::log_filter_bank(spsig_t *x, unsigned short nfilt, unsigned short *idx, spf_t *e)
{
  int i, j, from, to, status;
  double a, s, m, re, im;

  if ((status = fft(x, NULL, NULL)) != 0) {
    fprintf(stderr, "log_filter_bank(): cannot run FFT\n");
    return(status);
  }

  for (i = 0; i < nfilt; i++) {
    s = 0.0;

    /* ascending step from=idx[i] to=idx[i+1]-1: a = 1 / (idx[i+1] - idx[i] + 1) */
    from = *(idx+i);
    to = *(idx+i+1);
    a = 1.0 / (float)(to - from + 1);

    for (j = from; j < to; j++) {
      if (j) {
	re = *(_fftbuf+j);
	im = *(_fftbuf+_fftn-j);
	m = sqrt(re * re + im * im);
      }
      else
	m = fabs(*_fftbuf);

      s += m * (1.0 - a * (to - j));
    }

    /* descending step from=idx[i+1] to=idx[i+2]: a = 1 / (idx[i+2] - idx[i+1] + 1) */
    from = to;
    to = *(idx+i+2);
    a = 1.0 / (float)(to - from + 1);

    for (j = from; j <= to; j++) {
      if (j) {
	re = *(_fftbuf+j);
	im = *(_fftbuf+_fftn-j);
	m = sqrt(re * re + im * im);
    
      }
      else 
	m = fabs(*_fftbuf);

      s += m * (1.0 - a * (j - from));
    }
    *(e+i) = (s < SPRO_ENERGY_FLOOR) ? (spf_t)log(SPRO_ENERGY_FLOOR) : (spf_t)log(s);
  }

  return(0);
}
//-------------------------------------------------------------------------
String R::getClassName() const { return "FeatureComputer"; }
//-------------------------------------------------------------------------
void R::freeMem()
{
  if (_fft_initialized)
    fft_reset();
  _fft_initialized = false;
  if (_dct_initialized)
    dct_reset();
  _dct_initialized = false;
  if (_pBuf != NULL)
    sig_buf_free(_pBuf);
  _pBuf = NULL;
  if (_buf != NULL)
    free(_buf);
  _buf = NULL;
  if (_w != NULL)
    free(_w);
  _w = NULL;
  if (_s != NULL)
    sig_free(_s);
  _s = NULL;
  if (_e != NULL)
    free(_e);
  _e = NULL;
  if (_c != NULL)
    free(_c);
  _c = NULL;
  if (_r != NULL)
    free(_r);
  _r = NULL;
  if (_idx != NULL)
    free(_idx);
  _idx = NULL;
}
//-------------------------------------------------------------------------
R::~FeatureComputer() { freeMem(); }
//-------------------------------------------------------------------------
#endif // !defined(ALIZE_FeatureComputer_cpp)
