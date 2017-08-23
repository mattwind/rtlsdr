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
/** \brief Utilities to read module configuration parameters.
 *
 * This file contains utility functions that can can be used by modules
 * to read configuration parameters from the GKeyFile of the module. If
 * a parameter does not exist in the GKeyFile the corrersponding value
 * from sat-cfg will be returned.
 *
 * The intended use of these functions is to read parameters when creating
 * new modules. A module may have it's own configuration for most settings
 * while for some settings the global/default values may be needed. To avoid
 * too much repetitive coding in the module implementations, the functions
 * warp this code into one convenient function call.
 */
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#ifdef HAVE_CONFIG_H
#  include <build-config.h>
#endif
#include "sat-log.h"
#include "config-keys.h"
#include "sat-cfg.h"


/** \brief Get boolean parameter.
 *  \param f The configuration data for the module.
 *  \param sec Configuration section in the cfg data (see config-keys.h).
 *  \param key Configuration key in the cfg data (see config-keys.h).
 *  \param p SatCfg index to use as fallback.
 */
gboolean
mod_cfg_get_bool (GKeyFile *f, const gchar *sec, const gchar *key, sat_cfg_bool_e p)
{
     GError  *error = NULL;
     gboolean param;

     /* check whether parameter is present in GKeyFile */
     if (g_key_file_has_key (f, sec, key, NULL)) {

          param = g_key_file_get_boolean (f, sec, key, &error);

          if (error != NULL) {

               sat_log_log (SAT_LOG_LEVEL_ERROR,
                         _("%s: Failed to read boolean (%s)"),
                         __FUNCTION__, error->message);

               g_clear_error (&error);

               /* get a timeout from global config */
               param = sat_cfg_get_bool (p);
          }
     }
     /* get value from sat-cfg */
     else {
          param = sat_cfg_get_bool (p);

/*           sat_log_log (SAT_LOG_LEVEL_DEBUG, */
/*                     _("%s: Boolean value not found, using default (%d)"), */
/*                     __FUNCTION__, param); */
     }

     return param;
}


gint
mod_cfg_get_int  (GKeyFile *f, const gchar *sec, const gchar *key, sat_cfg_int_e p)
{
     GError  *error = NULL;
     gint     param;

     /* check whether parameter is present in GKeyFile */
     if (g_key_file_has_key (f, sec, key, NULL)) {

          param = g_key_file_get_integer (f, sec, key, &error);

          if (error != NULL) {

               sat_log_log (SAT_LOG_LEVEL_ERROR,
                         _("%s: Failed to read integer (%s)"),
                         __FUNCTION__, error->message);

               g_clear_error (&error);

               /* get a timeout from global config */
               param = sat_cfg_get_int (p);
          }
     }
     /* get value from sat-cfg */
     else {
          param = sat_cfg_get_int (p);

/*           sat_log_log (SAT_LOG_LEVEL_DEBUG, */
/*                     _("%s: Integer value not found, using default (%d)"), */
/*                     __FUNCTION__, param); */
     }

     return param;
}


gchar   *
mod_cfg_get_str  (GKeyFile *f, const gchar *sec, const gchar *key, sat_cfg_str_e p)
{
     GError  *error = NULL;
     gchar   *param;

     /* check whether parameter is present in GKeyFile */
     if (g_key_file_has_key (f, sec, key, NULL)) {

          param = g_key_file_get_string (f, sec, key, &error);

          if (error != NULL) {

               sat_log_log (SAT_LOG_LEVEL_ERROR,
                         _("%s: Failed to read string (%s)"),
                         __FUNCTION__, error->message);

               g_clear_error (&error);

               /* get a timeout from global config */
               param = sat_cfg_get_str (p);
          }
     }
     /* get value from sat-cfg */
     else {
          param = sat_cfg_get_str (p);

/*           sat_log_log (SAT_LOG_LEVEL_DEBUG, */
/*                     _("%s: String not found, using default (%s)"), */
/*                     __FUNCTION__, param); */
     }

     return param;

}
