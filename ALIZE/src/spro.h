/******************************************************************************/
/*                                                                            */
/*                                  spro.h                                    */
/*                                                                            */
/*                               SPro Library                                 */
/*                                                                            */
/* Guig                                                             Apr. 1997 */
/* -------------------------------------------------------------------------- */
/*  Copyright (C) 2002 Guillaume Gravier (ggravier@irisa.fr)                  */
/*                                                                            */
/*  This program is free software; you can redistribute it and/or             */
/*  modify it under the terms of the GNU General Public License               */
/*  as published by the Free Software Foundation; either version 2            */
/*  of the License, or (at your option) any later version.                    */
/*                                                                            */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU General Public License for more details.                              */
/*                                                                            */
/*  You should have received a copy of the GNU General Public License         */
/*  along with this program; if not, write to the Free Software               */
/*  Foundation, Inc., 59 Temple Place - Suite 330,                            */
/*  Boston, MA  02111-1307, USA.                                              */
/*                                                                            */
/******************************************************************************/
/*
 * CVS log:
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

/*
 * SPro library header 
 *
 * To avoid a huge header documenting the different structures defined
 * in SPro, the comments has been moved to the corresponding source
 * files. To know where to search for some information, here's a list
 * of the major structures with their corresponding source files:
 *
 *   - signal buffer and stream (sig.c)
 *   - feature buffer and stream (spf.c)
 *   - feature stream header (header.c)
 *
 * FFT (resp. LPC) related signal processing functions are found in
 * fft.c (resp. lpc.c). Convertion functions (adding deltas, modifying
 * stream descriptors, etc.) are in convert.c and whatever is not in
 * any of these files should be found in misc.c.
 *
 * Unexported functions are also defined here for sake of
 * documentation but there use is restricted by some defines.
 */

#ifndef _spro_h_
# define _spro_h_
 # include "system.h"

     /* ----------------------- */
     /* ----- error codes ----- */
     /* ----------------------- */

# define SPRO_ALLOC_ERR 10           /* allocation error                      */
# define SPRO_BUF_SIZE_ERR 20        /* I/O buffer size error                 */

# define SPRO_BAD_PARAM_ERR 55       /* invalid parameter value               */

# define SPRO_STREAM_OPEN_ERR 60     /* error opening stream                  */
# define SPRO_STREAM_SEEK_ERR 61     /* error seeking stream                  */
# define SPRO_SIG_READ_ERR 61        /* error reading signal                  */
# define SPRO_SIG_WRITE_ERR 62       /* error writing signal                  */
# define SPRO_CONVERT_ERR 70         /* error in data convertion              */
# define SPRO_FEATURE_READ_ERR 71    /* error reading features                */
# define SPRO_FEATURE_WRITE_ERR 72   /* error writing features                */
# define SPRO_DATA_KIND_ERR 73       /* invalid data kind                     */

# define SPRO_KERNEL_INIT_ERR 100    /* using uninitialized kernel error      */
# define SPRO_FFT_INIT_ERR 101       /* FFT initialization error              */
# define SPRO_DCT_INIT_ERR 102       /* DCT initialization error              */

     /* ---------------------------- */
     /* ----- some hard limits ----- */
     /* ---------------------------- */
# define SPRO_MIN_FFT_SIZE 64        /* minimum FFT size                      */
# define SPRO_MAX_FFT_SIZE 2048      /* maximum FFT size                      */
# define SPRO_MIN_FILTERS 3          /* miumium nb. of filters in a bank      */
# define SPRO_MAX_FILTERS 100        /* maximum nb. of filters in a bank      */
# define SPRO_ENERGY_FLOOR 1.0       /* floor energy below this threshold     */

     /* ----------------------------------------- */
     /* ----- formats and other identifiers ----- */ 
     /* ----------------------------------------- */
/*
 * data flag values
 */
# define SPRO_EMPTY_FLAG 0           /* empty flag                            */
# define WITHE 0x01                  /* log energy appended                   */
# define WITHZ 0x02                  /* mean normalization                    */
# define WITHN 0x04                  /* static energy suppressed              */
# define WITHD 0x08                  /* delta coefficients                    */
# define WITHA 0x10                  /* delta-delta coefficients              */
# define WITHR 0x20                  /* variance normalization                */

/*
 * Signal input formats
 */
# define SPRO_SIG_PCM16_FORMAT 0     /* RAW (linear 16) signal format         */
# define SPRO_SIG_WAVE_FORMAT 1      /* WAVE COMP360 signal format            */
# ifdef SPHERE
#  define SPRO_SIG_SPHERE_FORMAT 2   /* SHERE signal format                   */
# endif

/*
 * Weighting windows
 */
# define SPRO_NULL_WINDOW 0          /* no window                             */
# define SPRO_HAMMING_WINDOW 1       /* Hamming window                        */
# define SPRO_HANNING_WINDOW 2       /* Hanning window                        */
# define SPRO_BLACKMAN_WINDOW 3      /* Blackman window                       */

/*
 * Convertion modes (see convert.c for details)
 */
# define SPRO_CONV_DUPLICATE 0       /* convert with duplication              */
# define SPRO_CONV_REPLACE 1         /* convert and replace                   */
# define SPRO_CONV_UPDATE 2          /* convert in place                      */

/*
 * Stream I/O modes
 */
# define SPRO_STREAM_READ_MODE 1
# define SPRO_STREAM_WRITE_MODE 2

     /* ---------------------------- */
     /* ----- type definitions ----- */
     /* ---------------------------- */
/*
 * Feature level structures (see spf.c for details).
 */
typedef float spf_t;            /* feature type                               */

typedef struct {
  unsigned short adim;          /* allocated vector dimension                 */
  unsigned short dim;           /* actual vector dimension                    */
  unsigned long n;              /* number of vectors                          */
  unsigned long m;              /* maximum number of vectors                  */
  spf_t *s;                     /* pointer to features                        */
} spfbuf_t;

typedef struct {
  char *name;                   /* variable header field name                 */
  char *value;                  /* variable header field value                */
} spfield_t;                    /* header field                               */

typedef struct {
  unsigned short nfields;       /* number of variable header fields           */
  spfield_t *field;             /* array of attribute/value pairs             */
} spfheader_t;


typedef struct {
  char *name;                   /* stream file name (or NULL if stdio)        */
  FILE *f;                      /* I/O stream                                 */
  int iomode;                   /* I/O mode                                   */

  spfheader_t *header;          /* variable length header                     */
  float Fs;                     /* frame rate                                 */
  unsigned short idim;          /* input dimension                            */
  long iflag;                   /* input stream description                   */
  unsigned short odim;          /* output dimension                           */
  long oflag;                   /* output stream description                  */

  long cflag;                   /* additionnal stream description             */
  unsigned long winlen;         /* global processing window length            */
  float escale;                 /* energy scaling factor                      */

  spfbuf_t *buf;                /* feature I/O buffer                         */
  unsigned long start;          /* initial index of buffer                    */
  unsigned long idx;            /* current index in the buffer                */
} spfstream_t;                  /* feature stream                             */

/*
 * Signal level structures - SPro is only intended to *read* signals and a 
 * signal stream therefore implicitely refers to an *input* stream. See 
 * sig.c for details.
  */
typedef float sample_t;         /* a sample                                   */

typedef struct {
  unsigned long n;              /* number of samples                          */
  sample_t *s;                  /* array of samples                           */
} spsig_t;                      /* signal waveform                            */

typedef struct {
  unsigned long m;              /* maximum buffer size (num. samples)         */
  unsigned long n;              /* actual buffer size (num. samples)          */
  void *s;
} sigbuf_t;                     /* signal I/O buffer                          */

typedef struct {
  char *name;                   /* stream file name (or NULL if stdin)        */
  void *f;                      /* input stream (FILE or SP_FILE)             */
  int format;                   /* stream format                              */
  unsigned long nsamples;       /* total number of samples in stream          */
  unsigned long nread;          /* number of samples read from stream         */
  float Fs;                     /* sample rate                                */
  unsigned short nchannels;     /* number of channels                         */
  int nbps;                     /* number of bytes per samples.channel        */
  int swap;                     /* to swap or not to swap?                    */
  sigbuf_t *buf;                /* input buffer                               */
} sigstream_t;                  /* signal input stream                        */

     /* -------------------------------------------  */
     /* ----- signal stream related functions -----  */
     /* -------------------------------------------  */
/*
 * Signal allocation and I/O functions (see sig.c for details).
  */

/* create signal structure  */
spsig_t *sig_alloc(
  unsigned long                 /* number of samples                          */
);

/* free a signal  */
void sig_free(
  spsig_t *                     /* pointer to signal                          */
);

/* create signal I/O buffer  */
sigbuf_t *sig_buf_alloc(
  size_t,                       /* maximum buffer size (in bytes)             */
  unsigned short                /* number of bytes per sample                 */
);

/* free signal I/O buffer  */
void sig_buf_free(
  sigbuf_t *                    /* pointer to the buffer                      */
);

/* initialize signal input stream  */
sigstream_t *sig_stream_open(
  const char *,                 /* input stream name                          */
  int,                          /* format                                     */
  float,                        /* sample rate (needed for some formats)      */
  size_t,                       /* I/O buffer maximum size (in bytes)         */
  int                           /* swap the samples?                          */
);

/* close signal input stream  */
void sig_stream_close(
  sigstream_t *                 /* signal stream                              */
);

/* fill in buffer with new samples  */
unsigned long sig_stream_read(
  sigstream_t *                 /* signal stream                              */
);

/* get next frame from input stream  */
int get_next_sig_frame(
  sigstream_t *,                /* signal input stream                        */
  int,                          /* channel number (starts with 1)             */
  int,                          /* frame length (in samples)                  */
  int,                          /* frame shift (in samples)                   */
  float,                        /* pre-emphasis coefficient                   */
  sample_t *                    /* pointer to the output frame samples        */
);

# ifdef _sig_c_
void sp_swap(void *, size_t);
int sig_pcm16_stream_init(sigstream_t *, const char *);
unsigned long sig_pcm16_stream_read(sigstream_t *);
int sig_wave_stream_init(sigstream_t *, const char *);
unsigned long sig_wave_stream_read(sigstream_t *);
#  ifdef SPHERE
int sig_sphere_stream_init(sigstream_t *, const char *);
unsigned long sig_sphere_stream_read(sigstream_t *);
#  endif /* SPHERE  */
double getsample(void *, unsigned long, unsigned short);
# endif /* _sig_c_  */

     /* ---------------------------------------------------  */
     /* ----- feature stream header related functions -----  */
     /* ---------------------------------------------------  */
/*
 * Feature stream header initialization and basic handling
 * functions (see header.c for details).
  */

/* initalize feature header  */
spfheader_t *spf_header_init(
  const spfield_t *             /* field name/value array (NULL terminated)   */
);

/* free feature header  */
void spf_header_free(
  spfheader_t *                 /* stream header                              */
);

/* add field(s) to variable length header (do not check duplicate field
   names)  */
int spf_header_add(
  spfheader_t *,                /* stream header                              */
  const spfield_t *             /* field name/value array (NULL terminated)   */
);

/* get a variable header field from its name  */
char *spf_header_get(
  spfheader_t *,                /* stream header                              */
  const char *                  /* field name                                 */
);

/* read header from stream  */
spfheader_t *spf_header_read(
  FILE *                        /* Unix stream                                */
);

/* write header to stream  */
int spf_header_write(
  spfheader_t *,                /* stream header                              */
  FILE *                        /* Unix stream                                */
);

     /* ---------------------------------------------  */
     /* ----- feature buffer related functions ------  */
     /* ---------------------------------------------  */
/*
 * Feature buffer allocation and basic handling functions (see spf.c for 
 * details).
  */

/* allocate feature buffer  */
spfbuf_t *spf_buf_alloc(
  unsigned short,               /* feature vector dimension                   */
  size_t                        /* maximum buffer size (in bytes)             */
);

/* free feature buffer  */
void spf_buf_free(
  spfbuf_t *                    /* feature buffer                             */
);

/* resize feature buffer  */
spf_t *spf_buf_resize(
  spfbuf_t *,                   /* feature buffer                             */
  unsigned long                 /* new number of vectors                      */
);

/* append feature vector to buffer -- if buffer is full and resize block 
   size is not null, increment buffer size by block size, otherwise 
   return an error. Return a pointer to the appended vector in the 
   buffer or NULL in case of error.  */
spf_t *spf_buf_append(
  spfbuf_t *,                   /* feature buffer                             */
  spf_t *,                      /* feature vector                             */
  unsigned short,               /* feature vector dimension                   */
  unsigned long                 /* resize block size (or 0)                   */
);

/* return a pointer to the specified vector  */
spf_t *get_spf_buf_vec(
  spfbuf_t *,                   /* feature buffer                             */
  unsigned long                 /* relative vector index                      */
);

/* read data from stream into buffer  */
unsigned long spf_buf_read(
  spfbuf_t *,                   /* feature buffer                             */
  FILE *                        /* input stream                               */
);

/* write buffer data to stream  */
unsigned long spf_buf_write(
  spfbuf_t *,                   /* feature buffer                             */
  FILE *                        /* input stream                               */
);

# define spf_buf_dim(s)         ((s)->dim)
# define spf_buf_length(s)      ((s)->n)

# ifdef _spf_c_
extern void sp_swap(void *, size_t);
# endif /* _spf_c_  */

     /* ---------------------------------------------  */
     /* ----- feature stream related functions ------  */
     /* ---------------------------------------------  */
/*
 * Feature stream bulk I/O functions (see spf.c for details). More 
 * functions are defined in misc.c
  */

/* open feature stream in read mode  */
spfstream_t *spf_input_stream_open(
  const char *,                 /* stream name                                */
  long,                         /* feature descriptors to add                 */
  size_t                        /* I/O buffer maximum size (in bytes)         */
);

/* open feature stream in write mode  */
spfstream_t *spf_output_stream_open(
  const char *,                 /* stream name                                */
  unsigned short,               /* feature dimension                          */
  long,                         /* input feature description flag             */
  long,                         /* feature descriptors to add                 */
  float,                        /* frame sample rate in Hz                    */
  const spfield_t *,            /* field name/value array (NULL terminated)   */
  size_t                        /* I/O buffer maximum size (in bytes)         */
);

/* close feature stream  */
void spf_stream_close(
  spfstream_t *                 /* stream to close                            */
);

/* read new data into buffer and return the number of frames read  */
unsigned long spf_stream_read(
  spfstream_t *                 /* stream to read                             */
);

/* write data to the buffer and return the number of frames written  */
unsigned long spf_stream_write(
  spfstream_t *,                /* output stream                              */
  spf_t *,                      /* pointer to the data                        */
  unsigned long                 /* number of vectors to write                 */
);

/* flush content of (output) buffer and return the number of frames 
   written  */
unsigned long spf_stream_flush(
  spfstream_t *                 /* stream to flush                            */
);

/* seek into the stream to the specified position so that the next
   call to get_next_spf_frame() returns a pointer to the specified
   frame. The offset can be relative to the current position (mode is
   SEEK_CUR=1) or absolute from the start of file (mode
   SEEK_START=0). Note that the mode SEEK_END of fseek() is not
   supported by feature streams. Return 0 if ok.  */
int spf_stream_seek(
  spfstream_t *,                /* feature stream                             */
  long,                         /* seek offset (in number of frames)          */
  int                           /* seek mode as in fseek                      */
);

# define spf_stream_tell(s) ((s)->start + (s)->idx)
# define spf_stream_rewind(s) spf_stream_seek(s, 0, SEEK_SET)

/* get next frame  */
spf_t *get_next_spf_frame(
  spfstream_t *                 /* input stream                               */
);

/* convert a feature stream description string to binary flag  */
long sp_str_to_flag(
  const char *                  /* data description string                    */
);

/* convert a feature stream description flag to string  */
char *sp_flag_to_str(
  long,                         /* feature stream description                 */
  char [7]                      /* output string                              */
);

/* some macro accessors  */
# define spf_stream_name(s)     ((s)->name)
# define spf_stream_rate(s)     ((s)->Fs)
# define spf_stream_dim(s)      ((s)->odim)
# define spf_stream_flag(s)     ((s)->oflag)
# define spf_stream_buf(s)      ((s)->buf)
# define spf_stream_header(s)   ((s)->header)

# define set_stream_energy_scale(p, v) ((p)->escale = (v))
# define set_stream_seg_length(p, v) ((p)->winlen = (v))

# define set_flag_bits(s, b)    ((s) | (b))
# define get_flag_bits(s, b)    ((s) & (b))

     /* ------------------------------------------------  */
     /* ----- miscellaneous operations on features -----  */
     /* ------------------------------------------------  */
/*
 * The feature data handling functions are implemented in misc.c
  */

/* get total dimension of a vector given the base dimension  */
unsigned short spf_tot_dim(
  unsigned short,               /* base dimension (static without energy)     */
  long                          /* feature stream description                 */
);

/* return start/end bins in the feature vector of every component  */
void spf_indexes(
  unsigned short [9],           /* output indexes                             */
  unsigned short,               /* feature vector dimension                   */ 
  long                          /* feature stream description                 */
);

/* remove mean of static coefficients  */
int spf_buf_normalize(
  spfbuf_t *,                   /* feature buffer                             */
  unsigned short,               /* starting at coefficient from               */
  unsigned short,               /* up to coefficient to                       */
  unsigned long,                /* CMS window length (0 for whole data)       */
  int                           /* do variance normalization?                 */
);

/* normalize energy  */
int scale_energy(
  spfbuf_t *,                   /* feature buffer                             */
  unsigned short,               /* energy index                               */
  float,                        /* scaling factor                             */
  unsigned long                 /* scale window length (0 for whole data)     */
);

/* allocate a vector containing the lifter coefficients  */
float *set_lifter(
  int,                          /* lifter value                               */
  unsigned short                /* feature vector dimension                   */
);

      /* -----------------------------------------------  */
      /* ----- miscellaneous operations on signals -----  */
      /* -----------------------------------------------  */
/*
 * The signal handling functions are implemented in misc.c
  */


/* normalize a signal (return energy)  */
double sig_normalize(
  spsig_t *,                    /* input (and output) signal                  */
  int                           /* action flag                                */
);

/* set window  */
float *set_sig_win(
  unsigned long,                /* number of samples                          */
  int                           /* window type                                */
);

/* weight signal  */
spsig_t *sig_weight(
  spsig_t *,                    /* output signal                              */
  sample_t *,                   /* input samples                              */
  float *                       /* weighting window                           */
);

     /* ---------------------------------------------  */
     /* ----- feature data convertion functions -----  */
     /* ---------------------------------------------  */
/*
 * Convertion related functions are in spro_conv.c
  */

/* add deltas  */
int spf_add_delta(
  spfbuf_t *,                   /* feature buffer                             */
  unsigned short,               /* starting at coefficient index ...          */
  unsigned short,               /* up to coefficient index (included)         */
  unsigned short                /* writing result at index                    */
);

/* add/remove qualifiers  */
spfbuf_t *spf_buf_convert(
  spfbuf_t *,                   /* feature buffer                             */
  long,                         /* input feature stream description           */
  long,                         /* target feature stream description          */
  unsigned long,                /* normalization window length (0 if none)    */
  int                           /* convertion mode                            */
);

# ifdef _convert_c_
void spf_delta_set(spfbuf_t *, unsigned short, unsigned short, spfbuf_t *, unsigned short);
# endif /* _convert_c_ */

     /* ----------------------------------  */
     /* ----- LPC analysis functions -----  */
     /* ----------------------------------  */
/*
 * The LPC related functions are in lpc.c
  */

/* generalized correlation (variable spectral analysis)  */
int sig_correl(
  spsig_t *,                    /* pointer to the input signal                */
  float,                        /* spectral deformation parameter (alpha)     */
  float *,                      /* correlation                                */
  unsigned short                /* analysis order                             */
);

/* solve linear prediction equation  */
void lpc(
  float *,                      /* correlation                                */
  unsigned short,               /* analysis order                             */
  spf_t *,                      /* output prediction coefficients             */
  spf_t *,                      /* output reflexion coefficients              */
  float *                       /* prediction error                           */
);

/* linear prediction coefficients to cepstrum  */
void lpc_to_cep(
  spf_t *,                      /* pointer to input features                  */
  unsigned short,               /* LPC analysis order                         */
  unsigned short,               /* number of cepstral coefficients            */
  spf_t *                       /* pointer to output features                 */
);

/* reflexion coefficients to log area ratios  */
void refc_to_lar(
  spf_t *,                      /* pointer to input features                  */
  unsigned short,               /* analysis order                             */
  spf_t *                       /* pointer to output features                 */
);

/* linear prediction coefficients to line spectrum frequencies  */
int lpc_to_lsf(
  spf_t *,                      /* pointer to input features                  */
  unsigned short,               /* analysis order                             */
  spf_t *                       /* pointer to output features                 */
);

     /* ----------------------------------  */
     /* ----- FFT analysis functions -----  */
     /* ----------------------------------  */
/*
 * The FFT related functions are implemented in spro_fft.c
  */

/* initialize FFT kernel, return 0 if ok.  */
int fft_init(
  unsigned long                 /* number of FFT points                       */
);

/* perform FFT on a signal  */
int fft(
  spsig_t *,                    /* pointer to the input signal                */
  float *,                      /* pointer to the output module (or NULL)     */
  float *                       /* pointer to the output phase (or NULL)      */
);

# define fft_reset() fft_init(0)

/* set filter-bank indexes on a bilinear transformed frequency scale  */
unsigned short *set_alpha_idx(
  unsigned short,               /* number of filters                          */
  float,                        /* spectral resolution parameter (alpha)      */
  float,                        /* min normalized frequency (in [0,1/2])      */
  float                         /* max normalized frequency (in [0,1/2])      */
);

/* set filter-bank indexes on a MEL frequency scale  */
unsigned short *set_mel_idx(
  unsigned short,               /* number of filters                          */
  float,                        /* min normalized frequency (in [0,1/2])      */
  float,                        /* max normalized frequency (in [0,1/2])      */
  float                         /* sample rate                                */
);

/* log filter bank output on variable frequency scales  */
int log_filter_bank(
  spsig_t *,                    /* pointer to the input signal                */
  unsigned short,               /* number of filters                          */
  unsigned short *,             /* filter-bank indexes                        */
  spf_t *                       /* pointer to output features                 */
);

/* initialize DCT kernel  */
int dct_init(
  unsigned short,               /* number of input coefficients               */
  unsigned short                /* number of output coefficients              */
);

/* discrete cosine transform  */
int dct(
  spf_t *,                      /* pointer to input features                  */
  spf_t *                       /* pointer to output features                 */
);

# define dct_reset() dct_init(0, 0)

# ifdef _fft_c_
void _fft(float *, int);
void _brx(float *, int);
float theta(float, float);
float theta_inv(float, float);
float mel(float);
float mel_inv(float);
# endif /* _fft_c_ */

#endif /* _spro_h_ */
