/******************************************************************************/
/*                                                                            */
/*                                  misc.c                                    */
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
 * General purpose signal processing tools and data basic operations. 
 */

#define _misc_c_

#include "spro.h"
#include <math.h> // for sqrt, cos
#include <malloc.h> // for malloc, free...
#include <string.h> // for memcopy


/* ------------------------------------------------------------ */
/* ----- unsigned short spf_tot_dim(unsigned short, long) ----- */
/* ------------------------------------------------------------ */
/*
 * Return total dimension given the base dimension and the flags.
 */
unsigned short spf_tot_dim(unsigned short bd, long flag)
{
  unsigned short dim, sd;
  
  sd = (flag & WITHE) ? (bd + 1) : bd;
  dim = sd;

  if (flag & WITHD) {
    dim += sd;
    if (flag & WITHA)
      dim += sd;
  }

  if (flag & WITHN)
    dim--;
  
  return(dim);
}

/* -------------------------------------------------------------------- */
/* ----- void spf_indexes(unsigned short *, long, unsigned short) ----- */
/* -------------------------------------------------------------------- */
/*
 * Compute start and end indexes the components of the feature vector.
 * Nine indexes are returned in the  idx array, where the index is set 
 * to zero for non-existing components. Here's what it looks like:
 *    <    static     ><E><      delta    ><dE>< delta delta  ><ddE>
 *    |   |  ...  |   |   |   |  ...  |   |   |   |  ...  |   |   |
 *      ^           ^   ^   ^           ^   ^   ^           ^   ^
 *      |           |   |   |           |   |   |           |   |
 *    idx[0]      idx[1]| idx[3]      idx[4]| idx[6]      idx[7]|
 *                    idx[2]              idx[5]              idx[8]
 */
void spf_indexes(unsigned short idx[9], unsigned short dim, long flag)
{
  unsigned short nextidx, d = dim;
  
  if (flag & WITHN)
    d++;
  if (flag & WITHD) {
    if (flag & WITHA)
      d /= 3;
    else
      d /= 2;
  }
  if (flag & WITHE)
    d--;
  
  /* d is now the static feature dimension, not counting the energy */
  idx[2] = 0; idx[3] = 0; idx[4] = 0; idx[5] = 0; idx[6] = 0; idx[7] = 0; idx[8] = 0;

  idx[0] = 0; idx[1] = d - 1;
  if ((flag & WITHE) && ! (flag & WITHN)) {
    idx[2] = d;
    nextidx = d + 1;
  }
  else
    nextidx = d;

  if (flag & WITHD) {

    idx[3] = nextidx;
    idx[4] = nextidx + d - 1;
    if (flag & WITHE) {
      idx[5] = nextidx + d;
      nextidx =  nextidx + d + 1;
    }
    else
      nextidx = nextidx + d;

    if (flag & WITHA) {
      idx[6] = nextidx;
      idx[7] = nextidx + d - 1;
      if (flag & WITHE)
	idx[8] = nextidx + d;
    }
  }
}

/* ------------------------------------------------------------------------------------------------- */
/* ----- int spf_buf_normalize(spfbuf_t *, unsigned short, unsigned short, unsigned long, int) ----- */
/* ------------------------------------------------------------------------------------------------- */
/*
 * Set mean of static coefficients to 0 using a sliding window. If
 * window size is null, CMS is done over the entire data vector. 
 * 
 * Return 0 if ok.
 */
int spf_buf_normalize(spfbuf_t *buf, unsigned short si, unsigned short ei, unsigned long ws, int vnorm)
{
  int dim = ei - si + 1;
  unsigned long i, ii, n;
  unsigned short j;
  double *m, *v = NULL;
  double a, z;
  spf_t *p;
  
  if (dim < 1 || si > buf->dim || ei > buf->dim) {
    fprintf(stderr, "spf_buf_normalize(): invalid bounds [%u,%u] (dim=%u)\n", si, ei, buf->dim);
    return(SPRO_BAD_PARAM_ERR);
  }

  if ((m = (double *)malloc(dim * sizeof(double))) == NULL) {
    fprintf(stderr, "spf_buf_normalize(): cannot allocate memory\n");
    return(SPRO_ALLOC_ERR);
  }
  
  if ((v = (double *)malloc(dim * sizeof(double))) == NULL) {
    fprintf(stderr, "spf_buf_normalize(): cannot allocate memory\n");
    free(m);
    return(1);
  }

  if ((ws == 0) || (ws > buf->n)) {
    for (j = 0; j < dim; j++) {
      *(m+j) = 0.0;
      *(v+j) = 0.0;
    }

    p = buf->s + si;
    for (i = 0; i < buf->n; i++) {
      for (j = 0; j < dim; j++) {
	a = *(p+j);
	*(m+j) += a;
	*(v+j) += a * a;
      }
      p += buf->adim;
    }

    z = 1.0 / (double)(buf->n);
    for (j = 0; j < dim; j++) {
      *(m+j) *= z;
      if (vnorm) {
	a = *(m+j);
	*(v+j) = 1.0 / sqrt(*(v+j) * z - a * a);
      }
    }

    p = buf->s + si;
    for (i = 0; i < buf->n; i++) {
      for (j = 0; j < dim; j++) {
	*(p+j) -= *(m+j);
	if (vnorm)
	  *(p+j) *= *(v+j);
      }
      p += buf->adim;
    }    
  }
  else {
    /* The memory buffer prev_mem is organized in a cyclic way,
       in order to avoid a lot of copying.
       Something like that: 

       prev_mem    |----------------------------------|
                                ^   ^
                                |   |
                             i_beg i_end
    */
    unsigned long w_next, w_prev, prev_mem_len, i_beg = 0, i_end = 1;
    spf_t *prev_mem;

    p = buf->s + si;

    w_next = ws >> 1;
    w_prev = ws - w_next;
    prev_mem_len = w_prev + 1;

    /* initialization and preliminary filling of each buffer */
    for (j = 0; j < dim; j++) {
      *(m+j) = 0.0;
      *(v+j) = 0.0;
      for (ii = 0; ii < ((w_next < buf->n) ?  (w_next) : (buf->n)); ii++) {
	a = *(p + j + ii*buf->adim);
	*(m+j) += a;
	*(v+j) += a * a;
      }
    }

    if ((prev_mem = (spf_t *)malloc(dim * prev_mem_len * sizeof(spf_t))) == NULL) {
      fprintf(stderr, "spf_buf_normalize(): cannot allocate memory\n");
      return(SPRO_ALLOC_ERR);
    }

    for (ii = 0; ii < dim * prev_mem_len; ii++)
      *(prev_mem+ii) = 0.0;

    for (i = 0; i < buf->n; i++) {
      spf_t *p_end, *p_et, *p_beg;

      /* update memory */
      i_beg++;
      if(i_beg >= prev_mem_len) i_beg -= prev_mem_len;

      i_end++;
      if(i_end >= prev_mem_len) i_end -= prev_mem_len;

      p_beg = prev_mem + i_beg*dim;
      for(ii = 0; ii < dim; ii++)
        *(p_beg+ii) = *(p+ii);

      /* calculate length of the current window */
      n = ((i + w_next < buf->n) ? (i + w_next) : (buf->n)) -
          ((i > w_prev - 1) ? (i - w_prev + 1) : 0) + 1;

      p_end = prev_mem + i_end*dim;
      p_et = (i + w_next < buf->n) ? (p + w_next * buf->adim) : NULL;

      /* update the sums calculated during the current window */
      for (j = 0; j < dim; j++) {
	a = *(p_end+j);
	*(m+j) -= a;
	*(v+j) -= a * a;
      	if(p_et) {
	  a = *(p_et+j);
	  *(m+j) += a;
	  *(v+j) += a * a;
	}
      }

      /* normalize calculated values */
      z = 1.0 / (double)n;
      for (j = 0; j < dim; j++) {
	a = *(m+j) * z;
	*(p+j) -= a;
	if (vnorm)
	  *(p+j) /= (spf_t)sqrt(*(v+j) * z - a * a);
      }

      p += buf->adim;
    }
    
    free(prev_mem);
  }

  free(m);
  free(v);

  return(0);
}

/* -------------------------------------------------------------------------------- */
/* ----- int scale_energy(spfbuf_t_t *, unsigned short, float, unsigned long) ----- */
/* -------------------------------------------------------------------------------- */
/*
 * Scale the specified coefficient according to c'[j] = s * (c[j] - max) + 1.0.
 */
int scale_energy(spfbuf_t *buf, unsigned short j, float s, unsigned long ws)
{
  spf_t m, *p;
  unsigned long i, ii, ii_max;
  
  if (j >= buf->dim) {
    fprintf(stderr, "scale_energy(): invalid energy feature index %u (dim=%u)\n", j, buf->dim);
    return(SPRO_BAD_PARAM_ERR);
  }

  if ((ws == 0) || (ws > buf->n)) {
    p = buf->s + j;
    m = *p;

    for (i = 1; i < buf->n; i++) {
      p += buf->adim;
      if (*p > m)
	m = *p;
    }
    
    p = buf->s + j;
    for (i = 0; i < buf->n; i++) {
      *p = (*p - m) * s + 1.0;
      p += buf->adim;
    }
  }
  else {
    /* The memory buffer prev_mem is organized in a cyclic way,
       in order to avoid a lot of copying.
       Something like that: 

       prev_mem    |----------------------------------|
                                ^
                                |
                             i_beg
    */
    unsigned long w_next, w_prev, i_beg = 0;
    spf_t *prev_mem;

    p = buf->s + j;

    w_next = ws >> 1;
    w_prev = ws - w_next;

    if ((prev_mem = (spf_t *)malloc(w_prev * sizeof(spf_t))) == NULL) {
      fprintf(stderr, "spf_buf_normalize(): cannot allocate memory\n");
      return(SPRO_ALLOC_ERR);
    }

    for (ii = 0; ii < w_prev; ii++)
      *(prev_mem+ii) = 0.0;

    for (i = 0; i < buf->n; i++) {
      long i_cur;

      /* update memory */
      i_beg++;
      if( i_beg >= w_prev ) i_beg -= w_prev;

      *(prev_mem + i_beg) = *(p);

      /* calculate maximum */
      i_cur = i_beg;
      m = *(prev_mem + i_cur);
      ii_max = (w_prev < i) ? (w_prev) : (i);
      for (ii = 1; ii < ii_max; ii++) {
	i_cur--;
	if( i_cur < 0 ) i_cur += w_prev;

	if (*(prev_mem + i_cur) > m)
	  m = *(prev_mem + i_cur);
      }

      ii_max = (w_next < (buf->n - i)) ? (w_next) : (buf->n - i);
      for (ii = 1; ii <= ii_max; ii++)
	if (*(p + ii*buf->adim) > m)
	  m = *(p + ii*buf->adim);

      /* scale energy */
      *p = (*p - m) * s + 1.0;

      p += buf->adim;
    }
    
    free(prev_mem);
  }

  return(0);
}

/* ------------------------------------------------ */
/* ----- void set_lifter(int, unsigned short) ----- */
/* ------------------------------------------------ */
/*
 * Allocate and initialize the lifer coefficients.
 *
 * h[i] = 1.0 + l * sin((i + 1) * M_PI / l) / 2.0
 */
float *set_lifter(int l, unsigned short n)
{
  float *h;
  unsigned short i;
 
  if ((h = (float *)malloc(n * sizeof(float))) != NULL)
    for (i = 0; i < n; i++)
      *(h+i) = (float)(1.0 + 0.5 * (double)l * sin((double)(i + 1) * XX_PI / (double)l));

  return(h);
}

/* ------------------------------------------------ */
/* ----- double sig_normalize(spsig_t *, int) ----- */
/* ------------------------------------------------ */
/*
 * Compute energy sqrt(sum(s[i]^2)) and eventually normalize
 * signal to unit energy if flag is true.
 */
double sig_normalize(spsig_t *s, int flag)
{
  unsigned long i;
  sample_t *p = s->s;
  double g = 0.0, v;

  for (i = 0; i < s->n; i++) {
    v = (double)(*(p+i));
    g += v * v;
  }
  
  g = sqrt(g);
  
  if (flag && g != 0.0) {
    v = 1.0 / g;
    for (i = 0; i < s->n; i++)
      *(p+i) *= v;
  }
  
  return(g);
}

/* -------------------------------------------------- */
/* ----- float *set_sig_win(unsigned long, int) ----- */
/* -------------------------------------------------- */
/*
 * Set window sample according to length and type. Return NULL
 * in case of error or a pointer to the (allocated) window.
 *
 * Thanks to Jacques Prado for pointing out the differences
 * between symetric and disymetric windows...
 */
float *set_sig_win(unsigned long n, int win)
{
  unsigned long i;
  float *w = NULL;
  double r, pixpi = 2.0 * XX_PI;

  
  switch(win) {

  case SPRO_HAMMING_WINDOW:
    if ((w = (float *)malloc(n * sizeof(double))) != NULL) {
      for (i = 0; i < n; i++)
	*(w+i) = (float)(0.54 - 0.46 * cos(pixpi * (double)i / (double)n));
    }
    break;
    
  case SPRO_HANNING_WINDOW:
    if ((w = (float *)malloc(n * sizeof(double))) != NULL) {
      for (i = 0; i < n; i++)
	*(w+i) = (float)(0.5 * (1 - cos(pixpi * (double)i / (double)n)));
    }
    break;
    
  case SPRO_BLACKMAN_WINDOW:
    if ((w = (float *)malloc(n * sizeof(double))) != NULL) {
      for (i = 0; i < n; i++) {
	r = (double)i / (double)n;
	*(w+i) = (float)(0.42 - 0.5 * cos(pixpi * r) + 0.08 * cos(2 * pixpi * r));
      }
    }
      
  default:
    break;
  }
  
  return(w);
}

/* -------------------------------------------------------------- */
/* ---- spsig_t *sig_weight(spsig_t *, sample_t *, float *) ----- */
/* -------------------------------------------------------------- */
/* 
 * Weight samples according to the given window, storing the result
 * as a signal. Return a pointer to the signal.
 */
spsig_t *sig_weight(spsig_t *s, sample_t *buf, float *w) 
{
  sample_t *p = s->s;
  unsigned long i, n = s->n;

  if (w)
    for (i = 0; i < n; i++) 
      *(p+i) = (*(buf+i)) * (*(w+i));
  else
    memcpy(p, buf, n * sizeof(sample_t));

  return(s);
}

#undef _misc_c_
