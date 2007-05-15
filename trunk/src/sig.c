/******************************************************************************/
/*                                                                            */
/*                                   sig.c                                    */
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
 * Signal I/O routines.
 *
 * Signals are input via a signal stream (sigstream_t) mechanisms in
 * order to be able to deal with very large files (I'm talking about 1
 * hour or more of 44.1 kHz stereo data). As we are mostly interested
 * in getting successive frames of signal, signal streams are accessed
 * sequentially and solely input functions are provided. Several input
 * format are supported (currently raw PCM, WAVE and SPHERE).  Small
 * chunks of signals, such as frames, are stored as an array of float
 * samples in a spsig_t structures.
 *
 * A signal stream consists of some stream information (format, sample
 * rate, number of channels, ...) and an input signal buffer
 * (sigbuf_t). Signal input streams must be opened with the
 * sig_stream_open() function. For streams whose format does not
 * permit to retrieve the sample rate, the latter is arbitrarily set
 * to 0 and must be changed by the user in higher level
 * functions. Some stream formats let's you know the number of samples
 * some don't. In the latter, the number of samples field of the
 * stream is arbitrarily set to zero (but this field is not used
 * anyway).  Stream samples are read by the sig_stream_read() function
 * which returns the number of samples per channel read.
 *
 * Note that at the stream level, the number of samples is counted as
 * the number of samples per channel, meaning that, for stereo files,
 * a sample is two actual samples. Therefore, the sig_stream_read()
 * functions returns the number of samples per channel. On the
 * contrary, at the buffer level, the number of samples is the actual
 * number of samples in the buffer as the number of channels is not
 * known. I admit this is somehow confusing but that's the way it is.
 */

#define _sig_c_

#include "spro.h"

/* --------------------------------------------- */
/* ----- spsig_t *sig_alloc(unsigned long) ----- */
/* --------------------------------------------- */
/*
 * Allocate memory for a n point signal. Return allocated address or NULL 
 * in case of error.
 */
spsig_t *sig_alloc(unsigned long n)
{
  spsig_t *p;

  if ((p = (spsig_t *)malloc(sizeof(spsig_t))) == NULL)
    return(NULL);
  
  if ((p->s = (sample_t *)malloc(n * sizeof(sample_t))) == NULL) {
    free(p);
    return(NULL);
  }
  
  p->n = n;

  return(p);
}

/* ------------------------------------ */
/* ----- void sig_free(spsig_t *) ----- */
/* ------------------------------------ */
/*
 * Free allocated signal
 */
void sig_free(spsig_t *p)
{
  if (p) {
    if (p->s)
      free(p->s);
    free(p);
  }
}

/* ----------------------------------------------------------- */
/* ----- sigbuf_t *sig_buf_alloc(size_t, unsigned short) ----- */
/* ----------------------------------------------------------- */
/*
 * Allocate memory for a signal I/O buffer with a maximum size of nbytes bytes. Try 
 * hard to have an even number of samples to avoid complications in stereo mode.
 */
sigbuf_t *sig_buf_alloc(size_t nbytes, unsigned short nbps)
{
  sigbuf_t *p;

  if ((p = (sigbuf_t *)malloc(sizeof(sigbuf_t))) == NULL)
    return(NULL);
  
  if ((p->s = (short *)malloc(nbytes)) == NULL) {
    free(p);
    return(NULL);
  }

  p->m = nbytes / nbps;
  if (p->m %2)
    (p->m)--;

  p->n = 0;
  
  return(p);
}

/* ----------------------------------------- */
/* ----- void sig_buf_free(sigbuf_t *) ----- */
/* ----------------------------------------- */
/*
 * Free memory allocated to a signal I/O buffer.
 */
void sig_buf_free(sigbuf_t *p)
{
  if (p) {
    if (p->s)
      free(p->s);
    free(p);
  }
}

/* ------------------------------------------------------------------------------- */
/* ----- sigstream_t *sig_stream_open(const char *, int, float, size_t, int) ----- */
/* ------------------------------------------------------------------------------- */
/* Open a signal input stream in a given format. Currently, the
 * following format are supported:
 *
 * SPRO_SIG_PCM16_FORMAT -- single channel raw 16 bits/sample PCM
 * (sample rate must be specified in this case)
 *
 * SPRO_SIG_WAVE_FORMAT -- WAVE COMP 360 file format (so popular in
 * the PC world) is supported
 *
 * SPRO_SIG_SPHERE_FORMAT -- NIST SPHERE file format (if compiled with
 * -DSPHERE) 
 */
sigstream_t *sig_stream_open(const char *fn, int format, float Fs, size_t nbytes, int swap)
{
  sigstream_t *p;
  char *name;                    /* just a buffer pointer for nice output   */
  int status;

  /* allocate output structure */
  if ((p = (sigstream_t *)malloc(sizeof(sigstream_t))) == NULL) {
    fprintf(stderr, "sig_stream_open(): cannot allocate memory\n");
    return(NULL);
  }

  p->name = NULL;
  p->f = NULL;
  p->buf = NULL;
  p->format = format;
  p->nsamples = 0;
  p->nread = 0;
  p->Fs = Fs;
  p->nchannels = 0;
  p->nbps = 0; // number of bytes per samples.channel
  p->swap = swap;
  
  /* set stream filename */
  if (fn && strcmp(fn, "-") != 0)
    name = (char *)fn;
  else
    name = NULL;

  /* initialize stream - open input file and read header info */
  switch (format) {

  case SPRO_SIG_PCM16_FORMAT:    
    status = sig_pcm16_stream_init(p, name);
    break;
    
  case SPRO_SIG_WAVE_FORMAT:
    status = sig_wave_stream_init(p, name);
    break;
    
#ifdef SPHERE
  case SPRO_SIG_SPHERE_FORMAT:
    status = sig_sphere_stream_init(p, name);
    break;
    
#endif /* SPHERE */
    
  default:
    fprintf(stderr, "sig_stream_open(): unknown stream format\n");
    sig_stream_close(p);
    return(NULL);
  }
  
  if (status) {
    fprintf(stderr, "sig_stream_open(): cannot open input signal stream %s\n", (name) ? (name) : "stdin");
    sig_stream_close(p);
    return(NULL);
  }
  
  /* allocate buffer */
  if ((p->buf = sig_buf_alloc(nbytes, (unsigned short)(p->nbps))) == NULL) {
    sig_stream_close(p);
    return(NULL);
  }

  return(p);
}

/* ------------------------------------------------ */
/* ----- void sig_stream_close(sigstream_t *) ----- */
/* ------------------------------------------------ */
/*
 * Close signal input stream.
 */
void sig_stream_close(sigstream_t *p)
{
  if (p) {

    switch(p->format) {
    case SPRO_SIG_PCM16_FORMAT:  
    case SPRO_SIG_WAVE_FORMAT:
      if (p->f && p->name) /* if p->name == NULL, input is from stdin and we don't wanna close stdin! */
	fclose(p->f);
      break;

#ifdef SPHERE
    case SPRO_SIG_SPHERE_FORMAT:
      sp_close(p->f);
      break;    
#endif /* SPHERE */
    }

    if (p->name)
      free(p->name);

    if (p->buf)
      sig_buf_free(p->buf);

    free(p);
  }
}

/* --------------------------------------------------------- */
/* ----- unsigned long sig_stream_read(sigstream_t *) ------ */
/* --------------------------------------------------------- */
/* 
 * Read samples from signal stream and transfer them to the
 * buffer. Return the number of samples per channel transfered into
 * the buffer.  
 */
unsigned long sig_stream_read(sigstream_t *f)
{
  unsigned long nread;

  switch(f->format) {
    case SPRO_SIG_PCM16_FORMAT:  
      nread = sig_pcm16_stream_read(f);
      break;

    case SPRO_SIG_WAVE_FORMAT:
      nread = sig_wave_stream_read(f);
      break;

#ifdef SPHERE
    case SPRO_SIG_SPHERE_FORMAT:
      nread = sig_sphere_stream_read(f);
      break;    
#endif /* SPHERE */
      
  default:
    f->buf->n = 0;
  }
  
  f->nread += nread;

  /* fprintf(stderr, "read %ld samples from stream (%ld samples / channel), total samples / channel = %ld\n", f->buf->n, nread, f->nread); */
  
  return(nread);
}

/* ------------------------------------------------------------------ */
/* ----- int sig_pcm16_stream_init(sigstream_t *, const char *) ----- */
/* ------------------------------------------------------------------ */
/*
 * Initialize stream for RAW PCM16 format (do *not* allocate the buffer)
 */
int sig_pcm16_stream_init(sigstream_t *f, const char *fn)
{
  if (fn) {
    if ((f->name = strdup(fn)) == NULL) {
      fprintf(stderr, "sig_pcm16_stream_init(): cannot set stream name %s\n", fn);
      return(SPRO_ALLOC_ERR);
    }
    
    if ((f->f = fopen(fn, "rb")) == NULL) {
      fprintf(stderr, "sig_pcm16_stream_init(): cannot open file %s\n", fn);
      return(SPRO_SIG_READ_ERR);
    }
  }
  else
    f->f = stdin;
  
  f->nchannels = 1;
  f->nbps = 2;

  return(0);
}

/* --------------------------------------------------------------- */
/* ----- unsigned long sig_pcm16_stream_read(sigstream_t *) ------ */
/* --------------------------------------------------------------- */
/* 
 * Read samples into the buffer from a raw 16 bits/sample signal
 * stream.  Return the number of samples per channel transfered into
 * the buffer.  
 */
unsigned long sig_pcm16_stream_read(sigstream_t *f)
{
  short *p = f->buf->s;
  unsigned long nread, i;

  nread = fread(f->buf->s, f->nbps, f->buf->m, f->f);
  
  if (f->swap)
    for (i = 0; i < nread; i++, p++)
      sp_swap(p+i, f->nbps);

  f->buf->n = nread;

  return(nread / f->nchannels);
}

/* ----------------------------------------------------------------- */
/* ----- int sig_wave_stream_init(sigstream_t *, const char *) ----- */
/* ----------------------------------------------------------------- */
/*
 * Initialize stream for WAVE COMP 360 format. 
 */
int sig_wave_stream_init(sigstream_t *f, const char *fn)
{
  struct {
    char riff[4];                 /* String 'RIFF' without \0                */
    long totsize;                 /* Total file size - 8                     */
    char wave[4];                 /* String 'WAVE' without \0                */
    char fmtstr[4];               /* String 'fmt_' without \0                */
    long dum1;                    /* Length of format chunk (0x10)           */
    short dum2;                   /* Always 0x01                             */
    short numchans;               /* Number of channels                      */
    long Fs;                      /* Sample rate (in Hz)                     */
    long nbytespersec;            /* number of bytes/seconde                 */
    short nbytespersample;        /* number of bytes/sample                  */
    /* (1=8 bit mono, 2=8 bit stereo or 16 bit mono, 4= 16 bit stereo)   */
    short nbitspersample;         /* number of bits/samples                  */
    char data[4];                 /* string 'data' without \0                */
    unsigned long datsize;        /* number of data bytes (not samples)      */
  } hdr;

  /* open input file */
  if (fn) {
    if ((f->name = strdup(fn)) == NULL) {
      fprintf(stderr, "sig_wave_stream_init(): cannot set stream name %s\n", fn);
      return(SPRO_ALLOC_ERR);
    }
    
    if ((f->f = fopen(fn, "rb")) == NULL) {
      fprintf(stderr, "sig_wave_stream_init(): cannot open file %s\n", fn);
      return(SPRO_SIG_READ_ERR);
    }
  }
  else
    f->f = stdin;
  
  /* read WAVE header */
  if (fread(&hdr, sizeof(hdr), 1, f->f) != 1) {
    fprintf(stderr, "sig_wave_stream_init(): cannot read WAVE header from %s\n", (fn) ? (fn) : "stdin");
    return(SPRO_SIG_READ_ERR);
  }
  
  if (f->swap) {
    sp_swap(&(hdr.numchans), sizeof(short));
    sp_swap(&(hdr.Fs), sizeof(long));
    sp_swap(&(hdr.nbytespersample), sizeof(short));
    sp_swap(&(hdr.datsize), sizeof(long));
  }

  if (strncmp(hdr.riff, "RIFF", 4) != 0 || strncmp(hdr.wave, "WAVE", 4)) {
    fprintf(stderr, "sig_wave_stream_init(): stream %s not in WAVE format\n", (fn) ? (fn) : "stdin");
    return(SPRO_SIG_READ_ERR);
  }

  /* set the value we are looking for */
  f->nchannels = (unsigned short)hdr.numchans;
  f->nbps = hdr.nbytespersample / hdr.numchans;
  f->Fs = (float)hdr.Fs;
  f->nsamples = (unsigned long)(hdr.datsize / hdr.nbytespersample); /* number of samples / channel */
  
  /* fprintf(stderr, "nchannels=%d  nbytespersample=%d  datasize=%ld\n", hdr.numchans, hdr.nbytespersample, hdr.datsize); */

  return(0);
}

/* -------------------------------------------------------------- */
/* ----- unsigned long sig_wave_stream_read(sigstream_t *) ------ */
/* -------------------------------------------------------------- */
/* 
 * Read samples into the buffer from a wave signal stream. Return the
 * number of samples per channel transfered into the buffer.  
 */
unsigned long sig_wave_stream_read(sigstream_t *f)
{
  void *p = f->buf->s;
  unsigned long nread, n, i;

  /* in wave format, the number of samples to read must be computed
     since there are some trailer (as opposed to header) data after
     the samples, so that we can't simply wait for the end of the
     input file! (though I suspect in most cases eof corresponds to
     the end of the input signal. */
  n = (f->nsamples - f->nread) * f->nchannels;
  if (n > f->buf->m)
    n = f->buf->m;

  /* Hi there! If you're reading this because you're trying to
     understand the warning below, here's an explanation! The function
     was supposed to read a total of nasked samples and actually did
     read less. This is probably because of a weird total number of
     samples read from the WAVE header and stored in
     f->nsamples. Check that! */
  if ((nread = fread(f->buf->s, f->nbps, n, f->f)) != n)
    fprintf(stderr, "[SPro warning] end of wave stream unexpected!\n");

  if (f->swap && f->nbps > 1)
  {
    short* x = (short*)p;
    for (i = 0; i < nread; i++, x += f->nbps)
      sp_swap(x, f->nbps);
    p = x;
  }

  f->buf->n = nread;;

  return(nread / f->nchannels);
}

#ifdef SPHERE
/* ------------------------------------------------------------------- */
/* ----- int sig_sphere_stream_init(sigstream_t *, const char *) ----- */
/* ------------------------------------------------------------------- */
/*
 * Initialize stream for WAVE COMP 360 format (do *not* allocate the buffer)
 */
int sig_sphere_stream_init(sigstream_t *f, const char *fn)
{
  long n, rate;
  
  /* open file */
  if (fn) {
    if ((f->name = strdup(fn)) == NULL) {
      fprintf(stderr, "sig_wave_stream_init(): cannot set stream name %s\n", fn);
      return(SPRO_ALLOC_ERR);
    }
  }
  
  if ((f->f = sp_open((fn) ? (char *)(fn) : "-", "r")) == NULL) {
    fprintf(stderr, "sig_wave_stream_init(): cannot open SPHERE stream %s\n", (fn) ? (fn) : "stdin");
    return(SPRO_SIG_READ_ERR);
  }
  
  /* set convertion to PCM16 */
  if (sp_set_data_mode(f->f, "SE-PCM-2:DF-RAW") != 0) {
    fprintf(stderr, "sig_sphere_stream_init(): cannot set convertion mode to 16 bits/sample PCM\n");
    return(SPRO_SIG_READ_ERR);
  }

  /* read header fields */
  if (sp_h_get_field(f->f, "channel_count", T_INTEGER, (void *)&n) != 0
      || sp_h_get_field(f->f, "sample_rate", T_INTEGER, (void *)&rate) != 0) {
    fprintf(stderr, "sig_sphere_stream_init(): cannot read SPHERE header from stream %s \n", (fn) ? (fn) : "stdin");    
    return(SPRO_SIG_READ_ERR);
  }

  f->Fs = (float)rate;
  f->nchannels = (unsigned short)n;
  f->nbps = 2;

  return(0);
}

/* ---------------------------------------------------------------- */
/* ----- unsigned long sig_sphere_stream_read(sigstream_t *) ------ */
/* ---------------------------------------------------------------- */
/*
 * Read samples into the buffer from a wave signal stream. Return the
 * number of samples per channel transfered into the buffer.  
 */
unsigned long sig_sphere_stream_read(sigstream_t *f)
{
  short *p = f->buf->s;
  unsigned long nread, i;

  nread = sp_read_data(p, f->buf->m / f->nchannels, f->f);

  if (f->swap) 
    for (i = 0; i < nread; i++, p++)
      sp_swap(p+i, sizeof(short));

  f->buf->n = nread * f->nchannels;
  
  return(nread);
}
#endif


/* ------------------------------------------------------------------- */
/* ----- double getsample(void *, unsigned long, unsigned short) ----- */
/* ------------------------------------------------------------------- */
/*
 * Return n'th sample value assuming m bytes samples.
 */
double getsample(void *p, unsigned long n, unsigned short m)
{
  double v;

  switch(m) {
  case 1:
    v = (double)(*((char *)p+n));
    break;
  case 2:
    v = (double)*((short *)p+n);
    break;
  case 4:
    v = (double)*((long *)p+n);
  default:
    v = 0.0;
  }
  return(v);
}

/* ---------------------------------------- */
/* ----- void sp_swap(void *, size_t) ----- */
/* ---------------------------------------- */
/*
 * Swap n bytes.
 */
void sp_swap(void *x, size_t n)
{
  char c, *p = (char *)x;
  size_t j, nn = n >> 1;
  
  for (j = 0; j < nn; j++) {
    c = *(p+j);
    *(p+j) = *(p+n-j-1);
    *(p+n-j-1) = c;
  }
}

#undef _sig_c_
