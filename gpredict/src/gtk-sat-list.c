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
/** \brief Satellite List Widget.
 *
 * More info...
 */
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include "sgpsdp/sgp4sdp4.h"
#include "gtk-sat-list.h"
#include "sat-log.h"
#include "config-keys.h"
#include "sat-cfg.h"
#include "mod-cfg-get-param.h"
#include "gtk-sat-list-popup.h"
#include "gtk-sat-data.h"
#include "gpredict-utils.h"
#include "locator.h"
#include "sat-vis.h"
#include "sat-info.h"
#ifdef HAVE_CONFIG_H
#  include <build-config.h>
#endif



/** \brief Column titles indexed with column symb. refs. */
const gchar *SAT_LIST_COL_TITLE[SAT_LIST_COL_NUMBER] = {
    N_("Satellite"),
    N_("Catnum"),
    N_("Az"),
    N_("El"),
    N_("Dir"),
    N_("Ra"),
    N_("Dec"),
    N_("Range"),
    N_("Rate"),
    N_("Next Event"),
    N_("Next AOS"),
    N_("Next LOS"),
    N_("Lat"),
    N_("Lon"),
    N_("SSP"),
    N_("Footp"),
    N_("Alt"),
    N_("Vel"),
    N_("Dop"),
    N_("Loss"),
    N_("Del"),
    N_("MA"),
    N_("Phase"),
    N_("Orbit"),
    N_("Vis")
};


/** \brief Column title hints indexed with column symb. refs. */
const gchar *SAT_LIST_COL_HINT[SAT_LIST_COL_NUMBER] = {
    N_("Satellite Name"),
    N_("Catalogue Number"),
    N_("Azimuth"),
    N_("Elevation"),
    N_("Direction"),
    N_("Right Ascension"),
    N_("Declination"),
    N_("Slant Range"),
    N_("Range Rate"),
    N_("Next Event"),
    N_("Next AOS"),
    N_("Next LOS"),
    N_("Latitude"),
    N_("Longitude"),
    N_("Sub-Satellite Point"),
    N_("Footprint"),
    N_("Altitude"),
    N_("Velocity"),
    N_("Doppler Shift @ 100MHz"),
    N_("Signal Loss @ 100MHz"),
    N_("Signal Delay"),
    N_("Mean Anomaly"),
    N_("Orbit Phase"),
    N_("Orbit Number"),
    N_("Visibility")
};

const gfloat SAT_LIST_COL_XALIGN[SAT_LIST_COL_NUMBER] = {
    0.0, // name
    0.5, // catnum
    1.0, // az
    1.0, // el
    0.5, // direction
    0.0, // RA
    0.0, // dec
    1.0, // range
    1.0, // range rate
    0.5, // next event
    0.5, // AOS
    0.5, // LOS
    1.0, // lat
    1.0, // lon
    0.5, // SSP
    0.5, // footprint
    1.0, // alt
    0.0, // vel
    0.0, // doppler
    0.0, // loss
    0.0, // delay
    0.0, // MA
    0.0, // phase
    1.0, // orbit
    0.5, // visibility
};

static void          gtk_sat_list_class_init (GtkSatListClass *class);
static void          gtk_sat_list_init       (GtkSatList      *list);
static void          gtk_sat_list_destroy    (GtkObject       *object);
static GtkTreeModel *create_and_fill_model   (GHashTable      *sats);
static void          sat_list_add_satellites (gpointer key,
                                              gpointer value,
                                              gpointer user_data);
static gboolean      sat_list_update_sats    (GtkTreeModel *model,
                                              GtkTreePath  *path,
                                              GtkTreeIter  *iter,
                                              gpointer      data);

/* cell rendering related functions */
static void          check_and_set_cell_renderer (GtkTreeViewColumn *column,
                                                  GtkCellRenderer   *renderer,
                                                  gint               i);

static void          latlon_cell_data_function (GtkTreeViewColumn *col,
                                                GtkCellRenderer   *renderer,
                                                GtkTreeModel      *model,
                                                GtkTreeIter       *iter,
                                                gpointer           column);

static void          degree_cell_data_function (GtkTreeViewColumn *col,
                                                GtkCellRenderer   *renderer,
                                                GtkTreeModel      *model,
                                                GtkTreeIter       *iter,
                                                gpointer           column);

static void          distance_cell_data_function (GtkTreeViewColumn *col,
                                                  GtkCellRenderer   *renderer,
                                                  GtkTreeModel      *model,
                                                  GtkTreeIter       *iter,
                                                  gpointer           column);

static void          range_rate_cell_data_function (GtkTreeViewColumn *col,
                                                    GtkCellRenderer   *renderer,
                                                    GtkTreeModel      *model,
                                                    GtkTreeIter       *iter,
                                                    gpointer           column);

static void          float_to_int_cell_data_function (GtkTreeViewColumn *col,
                                                      GtkCellRenderer   *renderer,
                                                      GtkTreeModel      *model,
                                                      GtkTreeIter       *iter,
                                                      gpointer           column);

static void          two_dec_cell_data_function (GtkTreeViewColumn *col,
                                                 GtkCellRenderer   *renderer,
                                                 GtkTreeModel      *model,
                                                 GtkTreeIter       *iter,
                                                 gpointer           column);

static void          event_cell_data_function (GtkTreeViewColumn *col,
                                               GtkCellRenderer   *renderer,
                                               GtkTreeModel      *model,
                                               GtkTreeIter       *iter,
                                               gpointer           column);

static gint event_cell_compare_function (GtkTreeModel *model,
                                         GtkTreeIter  *a,
                                         GtkTreeIter  *b,
                                         gpointer user_data);


static gboolean   popup_menu_cb   (GtkWidget *treeview,
                                   gpointer list);

static gboolean   button_press_cb (GtkWidget *treeview,
                                   GdkEventButton *event,
                                   gpointer list);

static void row_activated_cb (GtkTreeView       *tree_view,
                              GtkTreePath       *path,
                              GtkTreeViewColumn *column,
                              gpointer           list);

static void       view_popup_menu (GtkWidget *treeview,
                                   GdkEventButton *event,
                                   gpointer list);

static void    Calculate_RADec (sat_t *sat, qth_t *qth, obs_astro_t *obs_set);




static GtkVBoxClass *parent_class = NULL;


GType
gtk_sat_list_get_type ()
{
    static GType gtk_sat_list_type = 0;

    if (!gtk_sat_list_type)
        {
            static const GTypeInfo gtk_sat_list_info =
                {
                    sizeof (GtkSatListClass),
                    NULL,  /* base_init */
                    NULL,  /* base_finalize */
                    (GClassInitFunc) gtk_sat_list_class_init,
                    NULL,  /* class_finalize */
                    NULL,  /* class_data */
                    sizeof (GtkSatList),
                    5,     /* n_preallocs */
                    (GInstanceInitFunc) gtk_sat_list_init,
                };

            gtk_sat_list_type = g_type_register_static (GTK_TYPE_VBOX,
                                                        "GtkSatList",
                                                        &gtk_sat_list_info,
                                                        0);
        }

    return gtk_sat_list_type;
}


static void
gtk_sat_list_class_init (GtkSatListClass *class)
{
    GObjectClass      *gobject_class;
    GtkObjectClass    *object_class;
    GtkWidgetClass    *widget_class;
    GtkContainerClass *container_class;

    gobject_class   = G_OBJECT_CLASS (class);
    object_class    = (GtkObjectClass*) class;
    widget_class    = (GtkWidgetClass*) class;
    container_class = (GtkContainerClass*) class;

    parent_class = g_type_class_peek_parent (class);

    object_class->destroy = gtk_sat_list_destroy;
 
}



static void
gtk_sat_list_init (GtkSatList *list)
{
    /*     GtkWidget *vbox,*hbox; */


    /*     hbox = gtk_hbox_new (TRUE, 5); */
    /*     gtk_box_pack_start_defaults (GTK_BOX (hbox), gtk_label_new ("POLAR")); */
    /*     gtk_box_pack_start_defaults (GTK_BOX (hbox), gtk_label_new ("LIST")); */

    /*     vbox = gtk_vbox_new (TRUE, 5); */
    /*     gtk_box_pack_start_defaults (GTK_BOX (vbox), gtk_label_new ("MAP")); */
    /*     gtk_box_pack_start_defaults (GTK_BOX (vbox), hbox); */

    /*     gtk_container_add (GTK_CONTAINER (module), vbox); */
    /*     gtk_widget_show_all (vbox); */

    /* initialise data structures */

}

static void
gtk_sat_list_destroy (GtkObject *object)
{
    (* GTK_OBJECT_CLASS (parent_class)->destroy) (object);
}




GtkWidget *
gtk_sat_list_new (GKeyFile *cfgdata, GHashTable *sats, qth_t *qth, guint32 columns)
{
    GtkWidget    *widget;
    GtkTreeModel *model;
    guint         i;

    GtkCellRenderer   *renderer;
    GtkTreeViewColumn *column;


    widget = g_object_new (GTK_TYPE_SAT_LIST, NULL);

    GTK_SAT_LIST (widget)->update = gtk_sat_list_update;

    /* Read configuration data. */
    /* ... */

    GTK_SAT_LIST (widget)->satellites = sats;
    GTK_SAT_LIST (widget)->qth = qth;


    /* initialise column flags */
    if (columns > 0)
        GTK_SAT_LIST (widget)->flags = columns;
    else
        GTK_SAT_LIST (widget)->flags = mod_cfg_get_int (cfgdata,
                                                        MOD_CFG_LIST_SECTION,
                                                        MOD_CFG_LIST_COLUMNS,
                                                        SAT_CFG_INT_LIST_COLUMNS);
    
    /* get refresh rate and cycle counter */
    GTK_SAT_LIST (widget)->refresh = mod_cfg_get_int (cfgdata,
                                                      MOD_CFG_LIST_SECTION,
                                                      MOD_CFG_LIST_REFRESH,
                                                      SAT_CFG_INT_LIST_REFRESH);

    GTK_SAT_LIST (widget)->counter = 1;

    /* create the tree view and add columns */
    GTK_SAT_LIST (widget)->treeview = gtk_tree_view_new ();

    gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (GTK_SAT_LIST (widget)->treeview),
                                  sat_cfg_get_bool (SAT_CFG_BOOL_RULES_HINT));

    /* create treeview columns */
    for (i = 0; i < SAT_LIST_COL_NUMBER; i++) {

        renderer = gtk_cell_renderer_text_new ();
        g_object_set (G_OBJECT (renderer), "xalign", SAT_LIST_COL_XALIGN[i], NULL);

        /* in win32 use special font for direction column because default font
           does not have arrow symbols.
        */
#ifdef G_OS_WIN32
        if (i == SAT_LIST_COL_DIR)
            g_object_set (G_OBJECT (renderer),
                          "font", "courier 12",
                          NULL);
#endif

        column = gtk_tree_view_column_new_with_attributes (_(SAT_LIST_COL_TITLE[i]),
                                                           renderer,
                                                           "text", i,
                                                           NULL);
        gtk_tree_view_insert_column (GTK_TREE_VIEW (GTK_SAT_LIST (widget)->treeview),
                                     column, -1);

        /* only aligns the headers */
        gtk_tree_view_column_set_alignment (column, 0.5);

        /* set sort id */
        gtk_tree_view_column_set_sort_column_id (column, i);

        /* set cell data function; allows to format data before rendering */
        check_and_set_cell_renderer (column, renderer, i);

        /* hide columns that have not been specified */
        if (!(GTK_SAT_LIST (widget)->flags & (1 << i))) {
            gtk_tree_view_column_set_visible (column, FALSE);
        }
        

    }

    /* create model and finalise treeview */
    model = create_and_fill_model (GTK_SAT_LIST (widget)->satellites);
    gtk_tree_view_set_model (GTK_TREE_VIEW (GTK_SAT_LIST (widget)->treeview), model);

    /* We need a special sort function for AOS/LOS events that works
       with all date and time formats (see bug #1861323)
    */
    gtk_tree_sortable_set_sort_func (GTK_TREE_SORTABLE (model),
                                     SAT_LIST_COL_AOS,
                                     event_cell_compare_function,
                                     NULL, NULL);
    gtk_tree_sortable_set_sort_func (GTK_TREE_SORTABLE (model),
                                     SAT_LIST_COL_LOS,
                                     event_cell_compare_function,
                                     NULL, NULL);

    /* satellite name should be initial sorting criteria */
    gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (model),
                                          SAT_LIST_COL_NAME,
                                          GTK_SORT_ASCENDING),

    g_object_unref (model);

    g_signal_connect (GTK_SAT_LIST (widget)->treeview, "button-press-event",
                      G_CALLBACK (button_press_cb), widget);
    g_signal_connect (GTK_SAT_LIST (widget)->treeview, "popup-menu",
                      G_CALLBACK (popup_menu_cb), widget);
    g_signal_connect (GTK_SAT_LIST (widget)->treeview, "row-activated",
                      G_CALLBACK (row_activated_cb), widget);

    GTK_SAT_LIST (widget)->swin = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (GTK_SAT_LIST (widget)->swin),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    gtk_container_add (GTK_CONTAINER (GTK_SAT_LIST (widget)->swin),
                       GTK_SAT_LIST (widget)->treeview);

    gtk_container_add (GTK_CONTAINER (widget), GTK_SAT_LIST (widget)->swin);
    gtk_widget_show_all (widget);

    return widget;
}




static GtkTreeModel *
create_and_fill_model   (GHashTable      *sats)
{
    GtkListStore *liststore;



    liststore = gtk_list_store_new (SAT_LIST_COL_NUMBER,
                                    G_TYPE_STRING,     // name
                                    G_TYPE_INT,        // catnum
                                    G_TYPE_DOUBLE,     // az
                                    G_TYPE_DOUBLE,     // el
                                    G_TYPE_STRING,     // direction
                                    G_TYPE_DOUBLE,     // RA
                                    G_TYPE_DOUBLE,     // Dec
                                    G_TYPE_DOUBLE,     // range
                                    G_TYPE_DOUBLE,     // range rate
                                    G_TYPE_STRING,     // next event
                                    G_TYPE_DOUBLE,     // next AOS
                                    G_TYPE_DOUBLE,     // next LOS
                                    G_TYPE_DOUBLE,     // ssp lat
                                    G_TYPE_DOUBLE,     // ssp lon
                                    G_TYPE_STRING,     // ssp qra
                                    G_TYPE_DOUBLE,     // footprint
                                    G_TYPE_DOUBLE,     // alt
                                    G_TYPE_DOUBLE,     // vel
                                    G_TYPE_DOUBLE,     // doppler
                                    G_TYPE_DOUBLE,     // path loss
                                    G_TYPE_DOUBLE,     // delay
                                    G_TYPE_DOUBLE,     // mean anomaly
                                    G_TYPE_DOUBLE,     // phase
                                    G_TYPE_ULONG,      // orbit
                                    G_TYPE_STRING);    // visibility


    g_hash_table_foreach (sats, sat_list_add_satellites, liststore);


    return GTK_TREE_MODEL (liststore);

}


static void
sat_list_add_satellites (gpointer key, gpointer value, gpointer user_data)
{
    GtkListStore *store = GTK_LIST_STORE (user_data);
    GtkTreeIter   item;
    sat_t        *sat = SAT (value);


    gtk_list_store_append (store, &item);
    gtk_list_store_set (store, &item,
                        SAT_LIST_COL_NAME, sat->nickname,
                        SAT_LIST_COL_CATNUM, sat->tle.catnr,
                        SAT_LIST_COL_AZ, sat->az,
                        SAT_LIST_COL_EL, sat->el,
                        SAT_LIST_COL_VISIBILITY, "-",
                        SAT_LIST_COL_RA, sat->ra,
                        SAT_LIST_COL_DEC, sat->dec,
                        SAT_LIST_COL_RANGE, sat->range,
                        SAT_LIST_COL_RANGE_RATE, sat->range_rate,
                        SAT_LIST_COL_DIR, "-",
                        SAT_LIST_COL_NEXT_EVENT, "--- N/A ---",
                        SAT_LIST_COL_AOS, sat->aos,
                        SAT_LIST_COL_LOS, sat->los,
                        SAT_LIST_COL_LAT, sat->ssplat,
                        SAT_LIST_COL_LON, sat->ssplon,
                        SAT_LIST_COL_SSP, "",
                        SAT_LIST_COL_FOOTPRINT, sat->footprint,
                        SAT_LIST_COL_ALT, sat->alt,
                        SAT_LIST_COL_VEL, sat->velo,
                        SAT_LIST_COL_DOPPLER, 0.0,
                        SAT_LIST_COL_LOSS, 0.0,
                        SAT_LIST_COL_DELAY, 0.0,
                        SAT_LIST_COL_MA, sat->ma,
                        SAT_LIST_COL_PHASE, sat->phase,
                        SAT_LIST_COL_ORBIT, sat->orbit,
                        -1);

    
}



/** \brief Update satellites */
void
gtk_sat_list_update          (GtkWidget *widget)
{
    GtkTreeModel *model;
    GtkSatList   *satlist = GTK_SAT_LIST (widget);


    /* first, do some sanity checks */
    if ((satlist == NULL) || !IS_GTK_SAT_LIST (satlist)) {
        
        sat_log_log (SAT_LOG_LEVEL_BUG,
                     _("%s: Invalid GtkSatList!"),
                     __FUNCTION__);
    }


    /* check refresh rate */
    if (satlist->counter < satlist->refresh) {
        satlist->counter++;
    }
    else {
        satlist->counter = 1;


        /* get and tranverse the model */
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (satlist->treeview));

        /* optimisation: detach model from view while updating */
        /* No, we do not do it, because it makes selections and scrolling
           impossible
        */
        /*         g_object_ref (model); */
        /*         gtk_tree_view_set_model (GTK_TREE_VIEW (satlist->treeview), NULL); */

        /* update */
        gtk_tree_model_foreach (model, sat_list_update_sats, satlist);

        /* re-attach model to view */
        /*         gtk_tree_view_set_model (GTK_TREE_VIEW (satlist->treeview), model); */

        /*         g_object_unref (model); */
    }

}


/** \brief Update data in each column in a given row */
static gboolean
sat_list_update_sats (GtkTreeModel *model,
                      GtkTreePath  *path,
                      GtkTreeIter  *iter,
                      gpointer      data)
{
    GtkSatList *satlist = GTK_SAT_LIST (data);
    guint      *catnum;
    sat_t      *sat;
    gchar      *buff;
    gdouble     doppler;
    gdouble     delay;
    gdouble     loss;
    gdouble     oldrate;
    gint        retcode;


    /* get the catalogue number for this row
       then look it up in the hash table
    */
    catnum = g_new0 (guint, 1);
    gtk_tree_model_get (model, iter, SAT_LIST_COL_CATNUM, catnum, -1);
    sat = SAT (g_hash_table_lookup (satlist->satellites, catnum));

    if (sat == NULL) {
        /* satellite not tracked anymore => remove */
        sat_log_log (SAT_LOG_LEVEL_MSG,
                     _("%s: Failed to get data for #%d."),
                     __FUNCTION__, *catnum);

        gtk_list_store_remove (GTK_LIST_STORE (model), iter);

        sat_log_log (SAT_LOG_LEVEL_BUG,
                     _("%s: Satellite #%d removed from list."),
                     __FUNCTION__, *catnum);
    }
    else {
        /* store new data */
        gtk_list_store_set (GTK_LIST_STORE (model), iter,
                            SAT_LIST_COL_AZ, sat->az,
                            SAT_LIST_COL_EL, sat->el,
                            SAT_LIST_COL_RANGE, sat->range,
                            SAT_LIST_COL_RANGE_RATE, sat->range_rate,
                            SAT_LIST_COL_LAT, sat->ssplat,
                            SAT_LIST_COL_LON, sat->ssplon,
                            SAT_LIST_COL_FOOTPRINT, sat->footprint,
                            SAT_LIST_COL_ALT, sat->alt,
                            SAT_LIST_COL_VEL, sat->velo,
                            SAT_LIST_COL_MA, sat->ma,
                            SAT_LIST_COL_PHASE, sat->phase,
                            SAT_LIST_COL_ORBIT, sat->orbit,
                            -1);

        /* doppler shift @ 100 MHz */
        if (satlist->flags & SAT_LIST_FLAG_DOPPLER) {
            doppler = -100.0e06 * (sat->range_rate / 299792.4580); // Hz
            gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                SAT_LIST_COL_DOPPLER, doppler,
                                -1);                        
        }

        /* delay */
        if (satlist->flags & SAT_LIST_FLAG_DELAY) {
            delay   = sat->range / 299.7924580;         // msec 
            gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                SAT_LIST_COL_DELAY, delay,
                                -1);                        
        }

        /* path loss */
        if (satlist->flags & SAT_LIST_FLAG_LOSS) {
            loss    = 72.4 + 20.0*log10(sat->range);               // dB
            gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                SAT_LIST_COL_LOSS, loss,
                                -1);                        
        }

        /* calculate direction */
        if (satlist->flags & SAT_LIST_FLAG_DIR) {

            if (sat->otype == ORBIT_TYPE_GEO) {
                buff = g_strdup ("G");
            }
            else if (sat->otype == ORBIT_TYPE_DECAYED) {
                buff = g_strdup ("D");
            }
            else if (sat->range_rate > 0.001) {
                /* going down */
                buff = g_strdup ("\342\206\223");
            }
            else if ((sat->range_rate <= 0.001) && (sat->range_rate >= -0.001)) {
                gtk_tree_model_get (model, iter,
                                    SAT_LIST_COL_RANGE_RATE, &oldrate,
                                    -1);
                /* turning around; don't know which way ? */
                if (sat->range_rate < oldrate) {
                    /* starting to approach */
                    buff = g_strdup ("\342\206\272");
                }
                else {
                    /* to receed */
                    buff = g_strdup ("\342\206\267");
                }
            }
            else if (sat->range_rate < -0.001) {
                /* coming up */
                buff = g_strdup ("\342\206\221");
            }
            else {
                buff = g_strdup ("-");
            }

            gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                SAT_LIST_COL_DIR, buff,
                                -1);

            /* free memory */
            g_free (buff);

        }
        
        /* SSP locator */
        if (satlist->flags & SAT_LIST_FLAG_SSP) {
            
            buff = g_try_malloc (7);

            retcode = longlat2locator (sat->ssplon, sat->ssplat, buff, 3);
            if (retcode == RIG_OK) {
                buff[6] = '\0';
                gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                    SAT_LIST_COL_SSP, buff,
                                    -1);
            }
            g_free (buff);
        }

        /* Ra and Dec */
        if (satlist->flags & (SAT_LIST_FLAG_RA | SAT_LIST_FLAG_DEC)) {
            obs_astro_t astro;

            Calculate_RADec (sat, satlist->qth, &astro);

            sat->ra = Degrees(astro.ra);
            sat->dec = Degrees(astro.dec);

            gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                SAT_LIST_COL_RA, sat->ra,
                                SAT_LIST_COL_DEC, sat->dec,
                                -1);
        }


        /* upcoming events */
        /*** FIXME: not necessary to update every time */
        if (satlist->flags & SAT_LIST_FLAG_AOS) {
            gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                SAT_LIST_COL_AOS, sat->aos,
                                -1);
        }
        if (satlist->flags & SAT_LIST_FLAG_LOS) {
            gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                SAT_LIST_COL_LOS, sat->los,
                                -1);

        }
        if (satlist->flags & SAT_LIST_FLAG_NEXT_EVENT) {
            gdouble    number;
            gchar      buff[TIME_FORMAT_MAX_LENGTH];
            gchar     *tfstr;
            gchar     *fmtstr;
            gchar     *alstr;
            time_t     t;
            guint      size;


            if (sat->aos > sat->los) {
                /* next event is LOS */
                number = sat->los;
                alstr = g_strdup ("LOS: ");
            }
            else {
                /* next event is AOS */
                number = sat->aos;
                alstr = g_strdup ("AOS: ");
            }
    
            if (number == 0.0) {
                gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                    SAT_LIST_COL_NEXT_EVENT, "--- N/A ---",
                                    -1);
            }
            else {

                /* convert julian date to struct tm */
                t = (number - 2440587.5)*86400.;

                /* format the number */
                tfstr = sat_cfg_get_str (SAT_CFG_STR_TIME_FORMAT);
                fmtstr = g_strconcat (alstr, tfstr, NULL);
                g_free (tfstr);

                /* format either local time or UTC depending on check box */
                if (sat_cfg_get_bool (SAT_CFG_BOOL_USE_LOCAL_TIME))
                    size = strftime (buff, TIME_FORMAT_MAX_LENGTH,
                                     fmtstr, localtime (&t));
                else
                    size = strftime (buff, TIME_FORMAT_MAX_LENGTH,
                                     fmtstr, gmtime (&t));
        
                if (size == 0)
                    /* size > MAX_LENGTH */
                    buff[TIME_FORMAT_MAX_LENGTH-1] = '\0';

                gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                    SAT_LIST_COL_NEXT_EVENT, buff,
                                    -1);


                g_free (fmtstr);
            }

            g_free (alstr);
            
        }

        if (satlist->flags & SAT_LIST_FLAG_VISIBILITY) {
            sat_vis_t  vis;

            vis = get_sat_vis (sat, satlist->qth, sat->jul_utc);
            buff = g_strdup_printf ("%c", vis_to_chr (vis));
            gtk_list_store_set (GTK_LIST_STORE (model), iter,
                                SAT_LIST_COL_VISIBILITY, buff,
                                -1);
            g_free (buff);
        }


    }

    g_free (catnum);

    /* Return value not documented what to return, but it seems that
       FALSE continues to next row while TRUE breaks
    */
    return FALSE;
}



/** \brief Set cell renderer function. */
static void
check_and_set_cell_renderer (GtkTreeViewColumn *column,
                             GtkCellRenderer   *renderer,
                             gint               i)
{

    switch (i) {

        /* general float with 2 dec. precision
           no extra format besides a degree char
        */
    case SAT_LIST_COL_AZ:
    case SAT_LIST_COL_EL:
    case SAT_LIST_COL_RA:
    case SAT_LIST_COL_DEC:
    case SAT_LIST_COL_MA:
    case SAT_LIST_COL_PHASE:
        gtk_tree_view_column_set_cell_data_func (column,
                                                 renderer,
                                                 degree_cell_data_function,
                                                 GUINT_TO_POINTER (i),
                                                 NULL);
        break;

        /* LAT/LON format */
    case SAT_LIST_COL_LAT:
    case SAT_LIST_COL_LON:
        gtk_tree_view_column_set_cell_data_func (column,
                                                 renderer,
                                                 latlon_cell_data_function,
                                                 GUINT_TO_POINTER (i),
                                                 NULL);
        break;

        /* distances and velocities */
    case SAT_LIST_COL_RANGE:
    case SAT_LIST_COL_ALT:
    case SAT_LIST_COL_FOOTPRINT:
        gtk_tree_view_column_set_cell_data_func (column,
                                                 renderer,
                                                 distance_cell_data_function,
                                                 GUINT_TO_POINTER (i),
                                                 NULL);
        break;

    case SAT_LIST_COL_VEL:
    case SAT_LIST_COL_RANGE_RATE:
        gtk_tree_view_column_set_cell_data_func (column,
                                                 renderer,
                                                 range_rate_cell_data_function,
                                                 GUINT_TO_POINTER (i),
                                                 NULL);
        break;

    case SAT_LIST_COL_DOPPLER:
        gtk_tree_view_column_set_cell_data_func (column,
                                                 renderer,
                                                 float_to_int_cell_data_function,
                                                 GUINT_TO_POINTER (i),
                                                 NULL);
        break;

    case SAT_LIST_COL_DELAY:
    case SAT_LIST_COL_LOSS:
        gtk_tree_view_column_set_cell_data_func (column,
                                                 renderer,
                                                 two_dec_cell_data_function,
                                                 GUINT_TO_POINTER (i),
                                                 NULL);
        break;

    case SAT_LIST_COL_AOS:
    case SAT_LIST_COL_LOS:
        gtk_tree_view_column_set_cell_data_func (column,
                                                 renderer,
                                                 event_cell_data_function,
                                                 GUINT_TO_POINTER (i),
                                                 NULL);
        break;

    default:
        break;

    }

}


/* render column containg lat/lon
   by using this instead of the default data function, we can
   control the number of decimals and display the coordinates in a
   fancy way, including degree sign and NWSE suffixes.

   Please note that this function only affects how the numbers are
   displayed (rendered), the tree_store will still contain the
   original flaoting point numbers. Very cool!
*/
static void
latlon_cell_data_function (GtkTreeViewColumn *col,
                           GtkCellRenderer   *renderer,
                           GtkTreeModel      *model,
                           GtkTreeIter       *iter,
                           gpointer           column)
{
    gdouble   number = 0.0;
    gchar   *buff;
    guint    coli = GPOINTER_TO_UINT (column);
    gchar    hmf = ' ';

    gtk_tree_model_get (model, iter, coli, &number, -1);

    /* check whether configuration requests the use
       of N, S, E and W instead of signs
    */
    if (sat_cfg_get_bool (SAT_CFG_BOOL_USE_NSEW)) {

        if (coli == SAT_LIST_COL_LAT) {
            if (number < 0.00) {
                number = -number;
                hmf = 'S';
            }
            else {
                hmf = 'N';
            }
        }
        else if (coli == SAT_LIST_COL_LON) {
            if (number < 0.00) {
                number = -number;
                hmf = 'W';
            }
            else {
                hmf = 'E';
            }
        }
        else {
            sat_log_log (SAT_LOG_LEVEL_BUG,
                         _("%s:%d: Invalid column: %d"),
                         __FILE__, __LINE__,
                         coli);
            hmf = '?';
        }
    }

    /* format the number */
    buff = g_strdup_printf ("%.2f\302\260%c", number, hmf);
    g_object_set (renderer,
                  "text", buff,
                  NULL);
    g_free (buff);
}


/* general floats with 2 digits + degree char */
static void
degree_cell_data_function (GtkTreeViewColumn *col,
                           GtkCellRenderer   *renderer,
                           GtkTreeModel      *model,
                           GtkTreeIter       *iter,
                           gpointer           column)
{
    gdouble    number;
    gchar     *buff;
    guint      coli = GPOINTER_TO_UINT (column);

    gtk_tree_model_get (model, iter, coli, &number, -1);

    /* format the number */
    buff = g_strdup_printf ("%.2f\302\260", number);
    g_object_set (renderer,
                  "text", buff,
                  NULL);
    g_free (buff);
}


/* distance and velocity, 0 decimal digits */
static void
distance_cell_data_function (GtkTreeViewColumn *col,
                             GtkCellRenderer   *renderer,
                             GtkTreeModel      *model,
                             GtkTreeIter       *iter,
                             gpointer           column)
{
    gdouble    number;
    gchar     *buff;
    guint      coli = GPOINTER_TO_UINT (column);

    gtk_tree_model_get (model, iter, coli, &number, -1);

    /* convert distance to miles? */
    if (sat_cfg_get_bool (SAT_CFG_BOOL_USE_IMPERIAL)) {
        number = KM_TO_MI(number);
    }

    /* format the number */
    buff = g_strdup_printf ("%.0f", number);
    g_object_set (renderer,
                  "text", buff,
                  NULL);
    g_free (buff);
}

/* range rate is special, because we may need to convert to miles
   and want 2-3 decimal digits.
*/
static void
range_rate_cell_data_function (GtkTreeViewColumn *col,
                               GtkCellRenderer   *renderer,
                               GtkTreeModel      *model,
                               GtkTreeIter       *iter,
                               gpointer           column)
{
    gdouble    number;
    gchar     *buff;
    guint      coli = GPOINTER_TO_UINT (column);

    gtk_tree_model_get (model, iter, coli, &number, -1);

    /* convert distance to miles? */
    if (sat_cfg_get_bool (SAT_CFG_BOOL_USE_IMPERIAL)) {
        number = KM_TO_MI(number);
    }

    /* format the number */
    buff = g_strdup_printf ("%.3f", number);
    g_object_set (renderer,
                  "text", buff,
                  NULL);
    g_free (buff);
}

/* 0 decimal digits */
static void
float_to_int_cell_data_function (GtkTreeViewColumn *col,
                                 GtkCellRenderer   *renderer,
                                 GtkTreeModel      *model,
                                 GtkTreeIter       *iter,
                                 gpointer           column)
{
    gdouble    number;
    gchar     *buff;
    guint      coli = GPOINTER_TO_UINT (column);

    gtk_tree_model_get (model, iter, coli, &number, -1);

    /* format the number */
    buff = g_strdup_printf ("%.0f", number);
    g_object_set (renderer,
                  "text", buff,
                  NULL);
    g_free (buff);
}

/* 2 decimal digits */
static void
two_dec_cell_data_function (GtkTreeViewColumn *col,
                            GtkCellRenderer   *renderer,
                            GtkTreeModel      *model,
                            GtkTreeIter       *iter,
                            gpointer           column)
{
    gdouble    number;
    gchar     *buff;
    guint      coli = GPOINTER_TO_UINT (column);

    gtk_tree_model_get (model, iter, coli, &number, -1);

    /* format the number */
    buff = g_strdup_printf ("%.2f", number);
    g_object_set (renderer,
                  "text", buff,
                  NULL);
    g_free (buff);
}


/* AOS/LOS; convert julian date to string */
static void
event_cell_data_function (GtkTreeViewColumn *col,
                          GtkCellRenderer   *renderer,
                          GtkTreeModel      *model,
                          GtkTreeIter       *iter,
                          gpointer           column)
{
    gdouble    number;
    gchar      buff[TIME_FORMAT_MAX_LENGTH];
    gchar     *fmtstr;
    guint      coli = GPOINTER_TO_UINT (column);
    time_t     t;
    guint size;


    gtk_tree_model_get (model, iter, coli, &number, -1);
    
    if (number == 0.0) {
        g_object_set (renderer,
                      "text", "--- N/A ---",
                      NULL);
    }
    else {

        /* convert julian date to struct tm */
        t = (number - 2440587.5)*86400.;

        /* format the number */
        fmtstr = sat_cfg_get_str (SAT_CFG_STR_TIME_FORMAT);

        /* format either local time or UTC depending on check box */
        if (sat_cfg_get_bool (SAT_CFG_BOOL_USE_LOCAL_TIME))
            size = strftime (buff, TIME_FORMAT_MAX_LENGTH, fmtstr, localtime (&t));
        else
            size = strftime (buff, TIME_FORMAT_MAX_LENGTH, fmtstr, gmtime (&t));
        
        if (size == 0)
            /* size > TIME_FORMAT_MAX_LENGTH */
            buff[TIME_FORMAT_MAX_LENGTH-1] = '\0';

        g_object_set (renderer,
                      "text", buff,
                      NULL);

        g_free (fmtstr);
    }

}


/** \brief Function to compare to Event cells.
  * \param model Pointer to the GtkTreeModel.
  * \param a Pointer to the first element.
  * \param b Pointer to the second element.
  * \param user_data Always NULL (TBC).
  * \return See detailed description.
  *
  * This function is used by the SatList sort function to determine whether
  * AOS/LOS cell a is greater than b or not. The cells a and b contain the
  * time of the event in Julian days, thus the result can be computed by a
  * simple comparison between the two numbers contained in the cells.
  *
  * The function returns -1 if a < b; +1 if a > b; 0 otherwise.
  */
static gint event_cell_compare_function (GtkTreeModel *model,
                                         GtkTreeIter  *a,
                                         GtkTreeIter  *b,
                                         gpointer user_data)
{
    gint result;
    gdouble ta,tb;
    gint sort_col;
    GtkSortType sort_type;


    /* Since this function is used for both AOS and LOS columns,
       we need to get the sort column */
    gtk_tree_sortable_get_sort_column_id (GTK_TREE_SORTABLE (model),
                                          &sort_col,
                                          &sort_type);

    /* get a and b */
    gtk_tree_model_get (model, a, sort_col, &ta, -1);
    gtk_tree_model_get (model, b, sort_col, &tb, -1);

    if (ta < tb) {
        result = -1;
    }
    else if (ta > tb) {
        result = 1;
    }
    else {
        result = 0;
    }

    return result;
}


/** \brief Reload configuration */
void
gtk_sat_list_reconf          (GtkWidget *widget, GKeyFile *cfgdat)
{
    sat_log_log (SAT_LOG_LEVEL_WARN, _("%s: FIXME I am not implemented"));
}



/** \brief Manage "popup-menu" events.
 *  \param treeview The tree view in the GtkSatList widget
 *  \param list Pointer to the GtkSatList widget.
 *
 * This function is called when the "popup-menu" signal is emitted. This
 * usually happens if the user presses SHJIFT-F10? It is used as a wrapper
 * for the function that actually creates the popup menu.
 */
static gboolean
popup_menu_cb (GtkWidget *treeview, gpointer list)
{

    /* if there is no selection, select the first row */


    view_popup_menu (treeview, NULL, list);

    return TRUE; /* we handled this */
}


/** \brief Manage button press events.
 *  \param treeview The tree view in the GtkSatList widget.
 *  \param event The event received.
 *  \param list Pointer to the GtkSatList widget.
 *
 */
static gboolean
button_press_cb (GtkWidget *treeview, GdkEventButton *event, gpointer list)
{

    /* single click with the right mouse button? */
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) {

        /* optional: select row if no row is selected or only
         *  one other row is selected (will only do something
         *  if you set a tree selection mode as described later
         *  in the tutorial) */
        if (1) {
            GtkTreeSelection *selection;

            selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));

            /* Note: gtk_tree_selection_count_selected_rows() does not
             *   exist in gtk+-2.0, only in gtk+ >= v2.2 ! */
            if (gtk_tree_selection_count_selected_rows (selection)  <= 1) {
                GtkTreePath *path;

                /* Get tree path for row that was clicked */
                if (gtk_tree_view_get_path_at_pos (GTK_TREE_VIEW (treeview),
                                                   (gint) event->x,
                                                   (gint) event->y,
                                                   &path, NULL, NULL, NULL)) {
                    gtk_tree_selection_unselect_all (selection);
                    gtk_tree_selection_select_path (selection, path);
                    gtk_tree_path_free (path);
                }
            }
        } /* end of optional bit */

        view_popup_menu (treeview, event, list);

        return TRUE; /* we handled this */
    }

    return FALSE; /* we did not handle this */
}

static void
row_activated_cb (GtkTreeView       *tree_view,
                  GtkTreePath       *path,
                  GtkTreeViewColumn *column,
                  gpointer           list)
{
    GtkTreeModel  *model;
    GtkTreeIter    iter;
    guint         *catnum;
    sat_t         *sat;
    
    catnum = g_new0 (guint, 1);

    model = gtk_tree_view_get_model(tree_view);
    gtk_tree_model_get_iter (model, &iter, path);
    gtk_tree_model_get (model, &iter,
                        SAT_LIST_COL_CATNUM, catnum,
                        -1);

    sat = SAT (g_hash_table_lookup (GTK_SAT_LIST (list)->satellites, catnum));

    if (sat == NULL) {
        sat_log_log (SAT_LOG_LEVEL_MSG,
                     _("%s:%d Failed to get data for %d."),
                     __FILE__, __LINE__, *catnum);
    }
    else {
        show_sat_info(sat, gtk_widget_get_toplevel (GTK_WIDGET (list)));
    }

    g_free (catnum);
}

static void
view_popup_menu (GtkWidget *treeview, GdkEventButton *event, gpointer list)
{
    GtkTreeSelection *selection;
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    guint            *catnum;
    sat_t            *sat;

    catnum = g_new0 (guint, 1);

    /* get selected satellite */
    selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
    if (gtk_tree_selection_get_selected (selection, &model, &iter))  {


        gtk_tree_model_get (model, &iter,
                            SAT_LIST_COL_CATNUM, catnum,
                            -1);

        sat = SAT (g_hash_table_lookup (GTK_SAT_LIST (list)->satellites, catnum));

        if (sat == NULL) {
            sat_log_log (SAT_LOG_LEVEL_MSG,
                         _("%s:%d Failed to get data for %d."),
                         __FILE__, __LINE__, *catnum);

        }
        else {
            gtk_sat_list_popup_exec (sat, GTK_SAT_LIST (list)->qth, event,
                                     GTK_SAT_LIST (list));
        }


    }
    else {
        sat_log_log (SAT_LOG_LEVEL_BUG,
                     _("%s:%d: There is no selection; skip popup."),
                     __FILE__, __LINE__);
    }

    g_free (catnum);
}


/*** FIXME: formalise with other copies, only need az,el and jul_utc */
static void
Calculate_RADec (sat_t *sat, qth_t *qth, obs_astro_t *obs_set)
{
    /* Reference:  Methods of Orbit Determination by  */
    /*                Pedro Ramon Escobal, pp. 401-402 */

    double phi,theta,sin_theta,cos_theta,sin_phi,cos_phi,
        az,el,Lxh,Lyh,Lzh,Sx,Ex,Zx,Sy,Ey,Zy,Sz,Ez,Zz,
        Lx,Ly,Lz,cos_delta,sin_alpha,cos_alpha;
    geodetic_t geodetic;


    geodetic.lon = qth->lon * de2ra;
    geodetic.lat = qth->lat * de2ra;
    geodetic.alt = qth->alt / 1000.0;
    geodetic.theta = 0;



    az = sat->az * de2ra;
    el = sat->el * de2ra;
    phi   = geodetic.lat;
    theta = FMod2p(ThetaG_JD(sat->jul_utc) + geodetic.lon);
    sin_theta = sin(theta);
    cos_theta = cos(theta);
    sin_phi = sin(phi);
    cos_phi = cos(phi);
    Lxh = -cos(az) * cos(el);
    Lyh =  sin(az) * cos(el);
    Lzh =  sin(el);
    Sx = sin_phi * cos_theta;
    Ex = -sin_theta;
    Zx = cos_theta * cos_phi;
    Sy = sin_phi * sin_theta;
    Ey = cos_theta;
    Zy = sin_theta*cos_phi;
    Sz = -cos_phi;
    Ez = 0;
    Zz = sin_phi;
    Lx = Sx*Lxh + Ex * Lyh + Zx*Lzh;
    Ly = Sy*Lxh + Ey * Lyh + Zy*Lzh;
    Lz = Sz*Lxh + Ez * Lyh + Zz*Lzh;
    obs_set->dec = ArcSin(Lz);  /* Declination (radians)*/
    cos_delta = sqrt(1 - Sqr(Lz));
    sin_alpha = Ly / cos_delta;
    cos_alpha = Lx / cos_delta;
    obs_set->ra = AcTan(sin_alpha,cos_alpha); /* Right Ascension (radians)*/
    obs_set->ra = FMod2p(obs_set->ra);

}




/** \brief Reload reference to satellites (e.g. after TLE update). */
void
gtk_sat_list_reload_sats (GtkWidget *satlist, GHashTable *sats)
{
    GTK_SAT_LIST (satlist)->satellites = sats;
}
