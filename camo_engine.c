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
 *          Eli Cohen
 */

#include <gtk/gtk.h>
#include <gmodule.h>
#include <math.h>
#include <cairo-gobject.h>

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

cairo_t *camo_pool = NULL;

static void
camo_engine_init (CamoEngine *self)
{
  gdouble xs, ys;
  gint i;
  gint width, height;
  gdouble area;

  GdkWindow *root = gdk_get_default_root_window ();
  gdk_window_get_geometry (root, NULL, NULL, &width, &height);
  area = width * height;

  cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
  camo_pool = cairo_create(surface);

  cairo_set_source_rgba (camo_pool, 0.5, 0.39, 0.35, 1.0);
  cairo_rectangle (camo_pool, 0, 0, width, height);
  cairo_fill (camo_pool);

  for (i = 0; i < area; i += 22500) {
    cairo_set_source_rgba (camo_pool, 0.58, 0.55, 0.47, 1.0);
    xs = g_random_double_range(0, width-5);
    ys = g_random_double_range(0, height-10);

    while (g_random_double_range(0.0, 1.0) < 0.99) {
      cairo_rectangle (camo_pool, xs, ys, 5, 10);
      cairo_fill (camo_pool);

      xs += g_random_double_range(-5, 5);
      ys += g_random_double_range(-5, 5);
    }
  }

  for (i = 0; i < area; i += 2500) {
    cairo_set_source_rgba (camo_pool, 0.31, 0.39, 0.35, 1.0);
    xs = g_random_double_range(0, width-5);
    ys = g_random_double_range(0, height-10);

    while (g_random_double_range(0.0, 1.0) < 0.99) {
      cairo_rectangle (camo_pool, xs, ys, 5, 10);
      cairo_fill (camo_pool);

      xs += g_random_double_range(-5, 5);
      ys += g_random_double_range(-5, 5);
    }
  }

  for (i = 0; i < area; i += 2500) {
    cairo_set_source_rgba (camo_pool, 0.2, 0.2, 0.27, 1.0);
    xs = g_random_double_range(0, width-5);
    ys = g_random_double_range(0, height-10);

    while (g_random_double_range(0.0, 1.0) < 0.99) {
      cairo_rectangle (camo_pool, xs, ys, 5, 10);
      cairo_fill (camo_pool);

      xs += g_random_double_range(-5, 5);
      ys += g_random_double_range(-5, 5);
    }
  }
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

  gtk_theming_engine_get_background_color (engine, flags, &color);

  cairo_save (cr);

  if (color.red == color.green == color.blue == 0)
    cairo_set_source_surface (cr, cairo_get_target(camo_pool), 0, 0);
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
