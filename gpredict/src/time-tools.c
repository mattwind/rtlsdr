/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
    Gpredict: Real-time satellite tracking and orbit prediction program

    Copyright (C)  2001-2009  Alexandru Csete, OZ9AEC.

    Authors: Alexandru Csete <oz9aec@gmail.com>

    Comments, questions and bugreports should be submitted via
    http://sourceforge.net/projects/gpredict/
    More details can be found at the project home page:

            http://gpredict.oz9aec.net/
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
  
    You should have received a copy of the GNU General Public License
    along with this program; if not, visit http://www.fsf.org/
*/

#include <glib.h>
#include <glib/gi18n.h>
//#include <sys/time.h>
#ifdef HAVE_CONFIG_H
#  include <build-config.h>
#endif
#include "sgpsdp/sgp4sdp4.h"
#include "time-tools.h"
//#ifdef G_OS_WIN32
//#  include "libc_internal.h"
//#  include "libc_interface.h"
//#endif



/** \brief Get the current time.
 *
 * Read the system clock and return the current Julian day.
 */
gdouble
get_current_daynum ()
{
    struct tm utc;
    //struct timeval tmval;
    GTimeVal tmval;
    double daynum;

    UTC_Calendar_Now (&utc);
    //gettimeofday (&tmval, NULL);
    g_get_current_time (&tmval);
    daynum = Julian_Date (&utc);
    daynum = daynum + (double)tmval.tv_usec/8.64e+10;
 
    return daynum;
}


/* This function calculates the day number from m/d/y. */
/* Legacy code no longer in use
long
get_daynum_from_dmy (int d, int m, int y)
{

    long dn;
    double mm, yy;

    if (m<3)
    { 
        y--; 
        m+=12; 
    }

    if (y<57)
        y+=100;

    yy=(double)y;
    mm=(double)m;
    dn=(long)(floor(365.25*(yy-80.0))-floor(19.0+yy/100.0)+floor(4.75+yy/400.0)-16.0);
    dn+=d+30*m+(long)floor(0.6*mm-0.3);

    return dn;
}
*/
