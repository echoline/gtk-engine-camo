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

#include "camo_utils.h"

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
  cairo_save (cr);

  cairo_set_source_surface(cr, cairo_get_target(camo_pool), 0, 0); // TODO
  cairo_rectangle(cr, x, y, width, height);
  cairo_fill(cr);

  cairo_restore (cr);
}

static void
camo_engine_render_arrow (GtkThemingEngine *engine,
                             cairo_t          *cr,
                             gdouble           angle,
                             gdouble           x,
                             gdouble           y,
                             gdouble           size)
{
  double line_width;
  GtkStateFlags state;
  GdkRGBA color;

  cairo_save (cr);

  line_width = size / 3.0 / sqrt (2);
  cairo_set_line_width (cr, line_width);
  cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);

  cairo_translate (cr, x + size / 2.0, y + size / 2.0);
  cairo_rotate (cr, angle - G_PI_2);
  cairo_translate (cr, size / 4.0, 0);
 
  cairo_scale (cr,
               (size / (size + line_width)),
               (size / (size + line_width)));

  cairo_move_to (cr, -size / 2.0, -size / 2.0);
  cairo_rel_line_to (cr, size / 2.0, size / 2.0);
  cairo_rel_line_to (cr, - size / 2.0, size / 2.0);

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get_color (engine, state, &color);
  gdk_cairo_set_source_rgba (cr, &color);
  cairo_stroke (cr);

  cairo_restore (cr);
}

static void
camo_engine_render_focus (GtkThemingEngine *engine,
                             cairo_t          *cr,
                             gdouble           x,
                             gdouble           y,
                             gdouble           width,
                             gdouble           height)
{
  GdkRGBA *border_color = NULL;
  GtkStateFlags state;
  gint line_width, focus_pad;
  gint border_radius;
  gboolean use_dashes;
  double dashes[2] = { 2.0, 0.2 };
  const GtkWidgetPath *path;

  path = gtk_theming_engine_get_path (engine);
  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "-camo-focus-border-color", &border_color,
                          "-camo-focus-border-radius", &border_radius,
                          "-camo-focus-border-dashes", &use_dashes,
                          NULL);

  gtk_theming_engine_get_style (engine,
                                "focus-line-width", &line_width,
                                "focus-padding", &focus_pad,
                                NULL);

  /* as we render the tab smaller than the whole allocation, we need
   * to recenter and resize the focus on the tab.
   */
  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_NOTEBOOK) &&
      gtk_theming_engine_has_region (engine, GTK_STYLE_REGION_TAB, NULL))
    {
      y += 3.0;
      height -= 3.0;
    }

  /* the treeview rows don't change allocation when modifying focus-padding,
   * so we have to move the focus ring inside the allocated area manually.
   */
  if (gtk_widget_path_is_type (path, GTK_TYPE_TREE_VIEW))
    {
      x += focus_pad;
      y += focus_pad;
      width -= 2 * focus_pad;
      height -= 2 * focus_pad;
    }

  cairo_save (cr);
  cairo_set_line_width (cr, line_width);

  if (line_width > 1)
    _cairo_round_rectangle_sides (cr, border_radius,
                                  x, y, width, height,
                                  SIDE_ALL, GTK_JUNCTION_NONE);
  else
    _cairo_round_rectangle_sides (cr, border_radius,
                                  x + 0.5, y + 0.5,
                                  width - 1, height - 1,
                                  SIDE_ALL, GTK_JUNCTION_NONE);

  if (use_dashes)
    cairo_set_dash (cr, dashes, 1, 0.0);

  if (border_color != NULL)
    gdk_cairo_set_source_rgba (cr, border_color);

  cairo_stroke (cr);
  cairo_restore (cr);

  if (border_color != NULL)
    gdk_rgba_free (border_color);
}

#define NOTEBOOK_TAB_TOP_MARGIN 3.0

static void
draw_tab_shape (cairo_t *cr,
                gdouble curve_width,
                gdouble x,
                gdouble y,
                gdouble width,
                gdouble height)
{
  cairo_move_to (cr, x, height);

  cairo_arc (cr, 
             x + curve_width, y + 3.0,
             2.5,
             G_PI, G_PI + G_PI_2);

  cairo_arc (cr,
             x + width - curve_width, y + 3.0,
             2.5,
             G_PI + G_PI_2, 2 * G_PI);

  cairo_line_to (cr, x + width, height);
}

static void
render_notebook_extension (GtkThemingEngine *engine,
                           cairo_t          *cr,
                           gdouble           x,
                           gdouble           y,
                           gdouble           width,
                           gdouble           height,
                           GtkPositionType   gap_side)
{
  gint tab_curvature, border_width;
  GdkRGBA border_color, background_color;
  GtkStateFlags state;
  gdouble angle = 0;
  cairo_pattern_t *pattern = NULL;
  gboolean is_active;

  gtk_theming_engine_get_style (engine,
                                "tab-curvature", &tab_curvature,
                                NULL);
  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get_background_color (engine, state, &background_color);
  gtk_theming_engine_get_border_color (engine, state, &border_color);
  gtk_theming_engine_get (engine, state,
                          "-camo-border-gradient", &pattern,
                          NULL);

  is_active = (state & GTK_STATE_FLAG_ACTIVE);
  border_width = 1.0;

  cairo_save (cr);
  cairo_set_line_width (cr, border_width);

  if (gap_side == GTK_POS_BOTTOM)
    x += border_width / 2;
  else if (gap_side == GTK_POS_TOP)
    x -= border_width / 2;

  if (gap_side == GTK_POS_TOP)
    {
      angle = G_PI;
      cairo_translate (cr, x + width, y + height - NOTEBOOK_TAB_TOP_MARGIN - border_width);
    }
  else
    {
      cairo_translate (cr, x, y + NOTEBOOK_TAB_TOP_MARGIN + border_width);
    }

  cairo_rotate (cr, angle);

  width -= border_width;
  height -= NOTEBOOK_TAB_TOP_MARGIN + border_width;

  /* draw the tab shape and clip the background inside it */
  cairo_save (cr);
  draw_tab_shape (cr, tab_curvature, 
                  0, 0.5,
                  width, is_active ? (height + 1.0) : (height));
  cairo_clip (cr);

  //GTK_THEMING_ENGINE_CLASS (camo_engine_parent_class)->render_background
  camo_engine_render_background
    (engine, cr, 0, 0.5,
     width, is_active ? (height + 1.0) : (height));

  cairo_restore (cr);

  /* now draw the border */
  draw_tab_shape (cr, tab_curvature,
                  0, 0,
                  width, height);

  if (pattern && (state & GTK_STATE_FLAG_ACTIVE))
    {
      cairo_scale (cr, width, height);
      cairo_set_source (cr, pattern);
      cairo_scale (cr, 1.0 / width, 1.0 / height);
    }
  else
    {
      gdk_cairo_set_source_rgba (cr, &border_color);
    }

  cairo_stroke (cr);

  if (pattern != NULL)
    cairo_pattern_destroy (pattern);

  cairo_restore (cr);
}

static void
camo_engine_render_extension (GtkThemingEngine *engine,
                                 cairo_t          *cr,
                                 gdouble           x,
                                 gdouble           y,
                                 gdouble           width,
                                 gdouble           height,
                                 GtkPositionType   gap_side)
{
  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_NOTEBOOK) &&
      ((gap_side == GTK_POS_TOP) || (gap_side == GTK_POS_BOTTOM)))
    {
      render_notebook_extension (engine, cr, x, y, width, height, gap_side);
      return;
    }

  GTK_THEMING_ENGINE_CLASS (camo_engine_parent_class)->render_extension
    (engine, cr,
     x, y, width, height,
     gap_side);
}

static void
camo_engine_render_expander (GtkThemingEngine *engine,
                                cairo_t          *cr,
                                gdouble           x,
                                gdouble           y,
                                gdouble           width,
                                gdouble           height)
{
  GdkRGBA fg;
  GtkStateFlags state;
  gdouble side, offset;
  gint line_width;
  GtkBorder border;
  const GtkWidgetPath *path = gtk_theming_engine_get_path (engine);

  side = floor (MIN (width, height));

  if (gtk_widget_path_is_type (path, GTK_TYPE_TREE_VIEW) &&
      (side == 17))
    {
      /* HACK: draw the expander as if it was 11px instead of the allocated 17px,
       * so that we can have a bit of padding between the view edge and the
       * expander itself.
       */
      x += 3;
      y += 3;
      width -= 6;
      height -= 6;
      side -= 6;
    }

  x += width / 2 - side / 2;
  y += height / 2 - side / 2;

  x = floor (x);
  y = floor (y);

  /* make sure the rendered side length is always odd */
  if (((gint) side % 2) == 0)
    side -= 1.0;

  //GTK_THEMING_ENGINE_CLASS (camo_engine_parent_class)->render_background 
  camo_engine_render_background
    (engine, cr, x, y, side, side);
  GTK_THEMING_ENGINE_CLASS (camo_engine_parent_class)->render_frame
    (engine, cr, x, y, side, side);

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get_color (engine, state, &fg);
  gtk_theming_engine_get_border (engine, state, &border);

  line_width = 1;
  offset = (1 + line_width / 2.0);

  cairo_save (cr);

  cairo_set_line_width (cr, line_width);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
  gdk_cairo_set_source_rgba (cr, &fg);

  cairo_move_to (cr,
                 x + border.left + offset,
                 y + side / 2);
  cairo_line_to (cr,
                 x + side - (border.right + offset),
                 y + side / 2);

  if ((state & GTK_STATE_FLAG_ACTIVE) == 0)
    {
      cairo_move_to (cr,
                     x + side / 2,
                     y + border.top + offset);
      cairo_line_to (cr,
                     x + side / 2,
                     y + side - (border.bottom + offset));
    }

  cairo_stroke (cr);

  cairo_restore (cr);
}

static void
camo_engine_class_init (CamoEngineClass *klass)
{
  GtkThemingEngineClass *engine_class = GTK_THEMING_ENGINE_CLASS (klass);

  engine_class->render_arrow = camo_engine_render_arrow;
  engine_class->render_focus = camo_engine_render_focus;
  engine_class->render_extension = camo_engine_render_extension;
  engine_class->render_expander = camo_engine_render_expander;
  engine_class->render_background = camo_engine_render_background;

  gtk_theming_engine_register_property (CAMO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("focus-border-color",
                                                            "Focus border color",
                                                            "Focus border color",
                                                            GDK_TYPE_RGBA, 0));
  gtk_theming_engine_register_property (CAMO_NAMESPACE, NULL,
                                        g_param_spec_int ("focus-border-radius",
                                                          "Focus border radius",
                                                          "Focus border radius",
                                                          0, G_MAXINT, 0,
                                                          0));
  gtk_theming_engine_register_property (CAMO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("border-gradient",
                                                            "Border gradient",
                                                            "Border gradient",
                                                            CAIRO_GOBJECT_TYPE_PATTERN, 0));
  gtk_theming_engine_register_property (CAMO_NAMESPACE, NULL,
                                        g_param_spec_boolean ("focus-border-dashes",
                                                              "Focus border uses dashes",
                                                              "Focus border uses dashes",
                                                              FALSE, 0));
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
