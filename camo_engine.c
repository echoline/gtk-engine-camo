/* Camo - a GTK+ engine
 *
 * Copyright (C) 2011 Carlos Garnacho <carlosg@gnome.org>
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Authors: Carlos Garnacho <carlosg@gnome.org>
 *          Cosimo Cecchi <cosimoc@gnome.org>
 *          Eli Cohen <eli.neoturbine.net>
 */

#include <gtk/gtk.h>
#include <gmodule.h>
#include <math.h>
#include <cairo-gobject.h>
#include "blur.h"

#define CAMO_NAMESPACE "camo"

typedef struct _CamoEngine CamoEngine;
typedef struct _CamoEngineClass CamoEngineClass;

struct _CamoEngine
{
  GtkThemingEngine parent_object;
};

struct _CamoEngineClass
{
  GtkThemingEngineClass parent_class;
};

#define CAMO_TYPE_ENGINE		 (camo_engine_get_type ())
#define CAMO_ENGINE(object)		 (G_TYPE_CHECK_INSTANCE_CAST ((object), CAMO_TYPE_ENGINE, CamoEngine))
#define CAMO_ENGINE_CLASS(klass)	 (G_TYPE_CHECK_CLASS_CAST ((klass), CAMO_TYPE_ENGINE, CamoEngineClass))
#define CAMO_IS_ENGINE(object)	 (G_TYPE_CHECK_INSTANCE_TYPE ((object), CAMO_TYPE_ENGINE))
#define CAMO_IS_ENGINE_CLASS(klass)	 (G_TYPE_CHECK_CLASS_TYPE ((klass), CAMO_TYPE_ENGINE))
#define CAMO_ENGINE_GET_CLASS(obj)	 (G_TYPE_INSTANCE_GET_CLASS ((obj), CAMO_TYPE_ENGINE, CamoEngineClass))

GType camo_engine_get_type	    (void) G_GNUC_CONST;
void  camo_engine_register_types (GTypeModule *module);

G_DEFINE_DYNAMIC_TYPE (CamoEngine, camo_engine, GTK_TYPE_THEMING_ENGINE)

void
camo_engine_register_types (GTypeModule *module)
{
  camo_engine_register_type (module);
}

cairo_surface_t *pool = NULL;
gint pool_width = 0;
gint pool_height = 0;

static void
camo_engine_init (CamoEngine *self)
{
}

static void
pool_gen ()
{
  gint width = pool_width;
  gint height = pool_height;

  gdouble xs, ys;
  gint i;
  gdouble area = width * height;

  if (pool)
    cairo_surface_destroy(pool);

  pool = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
  cairo_t *cr = cairo_create(pool);

  cairo_set_source_rgba (cr, 0.5, 0.39, 0.35, 1.0);
  cairo_rectangle (cr, 0, 0, width, height);
  cairo_fill (cr);

  for (i = 0; i < area; i += 22500) {
    cairo_set_source_rgba (cr, 0.58, 0.55, 0.47, 1.0);
    xs = g_random_double_range(0, width-10);
    ys = g_random_double_range(0, height-15);

    while (g_random_double_range(0.0, 1.0) < 0.90) {
      cairo_rectangle (cr, xs, ys, 30, 20);
      cairo_fill (cr);

      xs += g_random_double_range(-10, 10);
      ys += g_random_double_range(-10, 10);
    }
  }

  for (i = 0; i < area; i += 2500) {
    cairo_set_source_rgba (cr, 0.31, 0.39, 0.35, 1.0);
    xs = g_random_double_range(0, width-10);
    ys = g_random_double_range(0, height-15);

    while (g_random_double_range(0.0, 1.0) < 0.90) {
      cairo_rectangle (cr, xs, ys, 30, 20);
      cairo_fill (cr);

      xs += g_random_double_range(-10, 10);
      ys += g_random_double_range(-10, 10);
    }
  }

  for (i = 0; i < area; i += 2500) {
    cairo_set_source_rgba (cr, 0.2, 0.2, 0.27, 1.0);
    xs = g_random_double_range(0, width-10);
    ys = g_random_double_range(0, height-15);

    while (g_random_double_range(0.0, 1.0) < 0.90) {
      cairo_rectangle (cr, xs, ys, 30, 20);
      cairo_fill (cr);

      xs += g_random_double_range(-10, 10);
      ys += g_random_double_range(-10, 10);
    }
  }

  cairo_destroy(cr);

  blur_image_surface (pool, 15, width, height);
}

static void
camo_engine_render_background (GtkThemingEngine *engine,
                                  cairo_t          *cr,
                                  gdouble           x,
                                  gdouble           y,
                                  gdouble           width,
                                  gdouble           height)
{
  GdkRGBA color;
  GtkStateFlags flags = gtk_theming_engine_get_state (engine);
  gboolean generate = FALSE;
  gint scr_width, scr_height;

  gtk_theming_engine_get_background_color (engine, flags, &color);

  cairo_save (cr);

  if ((color.red == 0) && (color.green == 0) && (color.blue == 0)) {
    if (width > pool_width) {
      generate = TRUE;
      pool_width = width;
    }

    if (height > pool_height) {
      generate = TRUE;
      pool_height = height;
    }

    if (generate) {
      scr_width = gdk_screen_get_width (gdk_window_get_screen (gdk_get_default_root_window ()));
      scr_height = gdk_screen_get_height (gdk_window_get_screen (gdk_get_default_root_window ()));

      if (scr_width > pool_width)
        pool_width = scr_width;
      if (scr_height > pool_height)
        pool_height = scr_height;

      pool_gen ();
    }

    cairo_set_source_surface (cr, pool, 0, 0);
  }
  else
    gdk_cairo_set_source_rgba (cr, &color);

  cairo_rectangle (cr, x, y, width, height);
  cairo_fill (cr);

  cairo_restore (cr);
}

static void
camo_engine_class_init (CamoEngineClass *klass)
{
  GtkThemingEngineClass *engine_class = GTK_THEMING_ENGINE_CLASS (klass);

  engine_class->render_background = camo_engine_render_background;
}

static void
camo_engine_class_finalize (CamoEngineClass *klass)
{
}

G_MODULE_EXPORT void
theme_init (GTypeModule *module)
{
  camo_engine_register_types (module);
}

G_MODULE_EXPORT void
theme_exit (void)
{
}

G_MODULE_EXPORT GtkThemingEngine *
create_engine (void)
{
  return GTK_THEMING_ENGINE (g_object_new (CAMO_TYPE_ENGINE,
                                           "name", "camo",
                                           NULL));
}
