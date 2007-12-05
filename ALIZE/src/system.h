/******************************************************************************/
/*                                                                            */
/*                                system.h                                    */
/*                                                                            */
/*                               SPro Library                                 */
/*                                                                            */
/* Guig                                                             Sep. 2002 */
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
 * System dependent includes and defines.
 */

#ifndef _system_h_
# define _system_h_

#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <memory.h>
#include <string.h>
#define XX_PI 3.1415926535897932

/* limits */
# if HAVE_LIMITS_H
#  include <limits.h>
# endif
/* some systems (at least mine) do not define properly the following limits.... */
# ifndef FLT_MIN
#  define FLT_MIN 1.17549435E-38F
# endif
# ifndef FLT_MAX
#  define FLT_MAX 3.40282347e+38F
# endif
# ifndef DBL_MIN
#  define DBL_MIN 2.2250738585072014E-308
# endif
# ifndef DBL_MAX
#  define DBL_MAX 1.7976931348623157E+308
# endif

# if HAVE_SYS_TYPES_H
#  include <sys/types.h>
# endif

#endif /* _system_h_ */
