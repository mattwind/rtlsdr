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
#ifndef __GTK_SAT_LIST_H__
#define __GTK_SAT_LIST_H__ 1

#include <glib.h>
#include <glib/gi18n.h>
#include <gdk/gdk.h>
#include <gtk/gtkvbox.h>
#include "gtk-sat-data.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GTK_TYPE_SAT_LIST          (gtk_sat_list_get_type ())
#define GTK_SAT_LIST(obj)          GTK_CHECK_CAST (obj,\
                                 gtk_sat_list_get_type (),\
                                 GtkSatList)

#define GTK_SAT_LIST_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass,\
                                    gtk_sat_list_get_type (),\
                                    GtkSatListClass)

#define IS_GTK_SAT_LIST(obj)       GTK_CHECK_TYPE (obj, gtk_sat_list_get_type ())


typedef struct _gtk_sat_list      GtkSatList;
typedef struct _GtkSatListClass   GtkSatListClass;


struct _gtk_sat_list
{
     GtkVBox vbox;

     GtkWidget       *treeview;     /*!< the tree view itself */
     GtkWidget       *swin;         /*!< scrolled window */
     
     GHashTable      *satellites;   /*!< Satellites. */
     qth_t           *qth;          /*!< Pointer to current location. */

     guint32          flags;        /*!< Flags indicating which columns are visible */
     guint            refresh;      /*!< Refresh rate, ie. how many cycles should pass between updates */
     guint            counter;      /*!< cycle counter */

     gdouble          tstamp;       /*!< time stamp of calculations; set by GtkSatModule */
     
     void (* update) (GtkWidget *widget);  /*!< update function */
};

struct _GtkSatListClass
{
     GtkVBoxClass parent_class;
};


/** \brief Symbolic references to columns */
typedef enum {
     SAT_LIST_COL_NAME = 0,    /*!< Satellite name. */
     SAT_LIST_COL_CATNUM,      /*!< Catalogue number. */
     SAT_LIST_COL_AZ,          /*!< Azimuth. */
     SAT_LIST_COL_EL,          /*!< Elvation. */
     SAT_LIST_COL_DIR,         /*!< Direction, satellite on its way up or down. */
     SAT_LIST_COL_RA,          /*!< Right Ascension. */
     SAT_LIST_COL_DEC,         /*!< Declination. */
     SAT_LIST_COL_RANGE,       /*!< Range. */
     SAT_LIST_COL_RANGE_RATE,  /*!< Range rate. */
     SAT_LIST_COL_NEXT_EVENT,  /*!< Next event AOS or LOS depending on El. */
     SAT_LIST_COL_AOS,         /*!< Next AOS regardless of El. */
     SAT_LIST_COL_LOS,         /*!< Next LOS regardless of El. */
     SAT_LIST_COL_LAT,         /*!< Latitude. */
     SAT_LIST_COL_LON,         /*!< Longitude. */
     SAT_LIST_COL_SSP,         /*!< Sub satellite point grid square */
     SAT_LIST_COL_FOOTPRINT,   /*!< Footprint. */
     SAT_LIST_COL_ALT,         /*!< Altitude. */
     SAT_LIST_COL_VEL,         /*!< Velocity. */
     SAT_LIST_COL_DOPPLER,     /*!< Doppler shift at 100 MHz.*/
     SAT_LIST_COL_LOSS,        /*!< Path Loss at 100 MHz. */
     SAT_LIST_COL_DELAY,       /*!< Signal delay */
     SAT_LIST_COL_MA,          /*!< Mean Anomaly. */
     SAT_LIST_COL_PHASE,       /*!< Phase. */
     SAT_LIST_COL_ORBIT,       /*!< Orbit Number. */
     SAT_LIST_COL_VISIBILITY,  /*!< Visibility. */
     SAT_LIST_COL_NUMBER
} sat_list_col_t;



/** \brief Column Flags */
typedef enum {
     SAT_LIST_FLAG_NAME       = 1 << SAT_LIST_COL_NAME,        /*!< Satellite name. */
     SAT_LIST_FLAG_CATNUM     = 1 << SAT_LIST_COL_CATNUM,
     SAT_LIST_FLAG_AZ         = 1 << SAT_LIST_COL_AZ,          /*!< Azimuth. */
     SAT_LIST_FLAG_EL         = 1 << SAT_LIST_COL_EL,          /*!< Elvation. */
     SAT_LIST_FLAG_DIR        = 1 << SAT_LIST_COL_DIR,         /*!< Direction */
     SAT_LIST_FLAG_RA         = 1 << SAT_LIST_COL_RA,          /*!< Right Ascension. */
     SAT_LIST_FLAG_DEC        = 1 << SAT_LIST_COL_DEC,         /*!< Declination. */
     SAT_LIST_FLAG_RANGE      = 1 << SAT_LIST_COL_RANGE,       /*!< Range. */
     SAT_LIST_FLAG_RANGE_RATE = 1 << SAT_LIST_COL_RANGE_RATE,  /*!< Range rate. */
     SAT_LIST_FLAG_NEXT_EVENT = 1 << SAT_LIST_COL_NEXT_EVENT,  /*!< Next event. */
     SAT_LIST_FLAG_AOS        = 1 << SAT_LIST_COL_AOS,         /*!< Next AOS. */
     SAT_LIST_FLAG_LOS        = 1 << SAT_LIST_COL_LOS,         /*!< Next LOS. */
     SAT_LIST_FLAG_LAT        = 1 << SAT_LIST_COL_LAT,         /*!< Latitude. */
     SAT_LIST_FLAG_LON        = 1 << SAT_LIST_COL_LON,         /*!< Longitude. */
     SAT_LIST_FLAG_SSP        = 1 << SAT_LIST_COL_SSP,         /*!< SSP grid square */
     SAT_LIST_FLAG_FOOTPRINT  = 1 << SAT_LIST_COL_FOOTPRINT,   /*!< Footprint. */
     SAT_LIST_FLAG_ALT        = 1 << SAT_LIST_COL_ALT,         /*!< Altitude. */
     SAT_LIST_FLAG_VEL        = 1 << SAT_LIST_COL_VEL,         /*!< Velocity. */
     SAT_LIST_FLAG_DOPPLER    = 1 << SAT_LIST_COL_DOPPLER,     /*!< Doppler shift. */
     SAT_LIST_FLAG_LOSS       = 1 << SAT_LIST_COL_LOSS,        /*!< Path Loss. */
     SAT_LIST_FLAG_DELAY      = 1 << SAT_LIST_COL_DELAY,       /*!< Delay */
     SAT_LIST_FLAG_MA         = 1 << SAT_LIST_COL_MA,          /*!< Mean Anomaly. */
     SAT_LIST_FLAG_PHASE      = 1 << SAT_LIST_COL_PHASE,       /*!< Phase. */
     SAT_LIST_FLAG_ORBIT      = 1 << SAT_LIST_COL_ORBIT,       /*!< Orbit Number. */
     SAT_LIST_FLAG_VISIBILITY = 1 << SAT_LIST_COL_VISIBILITY   /*!< Visibility. */
} sat_list_flag_t;


GtkType        gtk_sat_list_get_type        (void);
GtkWidget*     gtk_sat_list_new             (GKeyFile   *cfgdata,
                                                        GHashTable *sats,
                                                        qth_t      *qth,
                                                        guint32     columns);
void           gtk_sat_list_update          (GtkWidget  *widget);
void           gtk_sat_list_reconf          (GtkWidget  *widget, GKeyFile *cfgdat);

void gtk_sat_list_reload_sats (GtkWidget *satlist, GHashTable *sats);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_SAT_MODULE_H__ */
