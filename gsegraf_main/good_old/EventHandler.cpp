/*******************************************************************************
*
* EventHandler.c
*
* Handles events.
*
* Copyright © 2008, 2009, 2010, 2011 Spencer A. Buckner
* http://savannah.gnu.org/projects/gsegrafix
*
* This file is part of GSEGrafix, a scientific and engineering plotting program.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*******************************************************************************/


#include "gsegraf.h"


/* Event handler */
gboolean EventHandler ( GtkWidget *window, GdkEvent *event )
{
/* Declare variables */
   UINT32 zoom_fill_color, canvas_fg_color, canvas_bg_color;
   int flag_2d, flag_3d, flag_2d_rect, flag_polar, flag_linear, flag_logx;
   int flag_logy, flag_loglog, ncontours, nplots;
   int state, keyval,
       window_width_old, window_height_old,
       window_width_new, window_height_new;
   static int flag, flag_button1;
   static double x1_window, y1_window, x2_window, y2_window;
   double x1_box, x2_box, y1_box, y2_box, xmin, xmax, ymin, ymax, zmin, zmax;
   double xscale, yscale;
   double x1, x2, y1, y2, max_dimension, xtext1, xtext2, ytext1, ytext2;
   char label[21], string[51], axis_type[64];
   FILE *fptr;
   static GnomeCanvasItem *zoom_rectangle;
   GnomeCanvasItem *text, *rectangle;
   double tick_major_0, tick_minor_0, dash_0, space_dash_0, space_dot_0;

// Get current plot color values:
   GSEG_GetColorValues(&zoom_fill_color, &canvas_fg_color,
                       &canvas_bg_color);

// Get current plot values 
   GSEG_GetCurrentPlotSettings(&flag_2d, &flag_3d, &flag_2d_rect, &flag_polar,
                               &flag_linear, &flag_logx, &flag_logy,
                               &flag_loglog, &ncontours, &nplots);

   /* Get plot box minimum and maximum values */
   GSEG_GetBoxSettings(&x1_box, &x2_box, &y1_box, &y2_box,
                       &xmin, &xmax, &ymin, &ymax, &zmin, &zmax,
                       &xscale, &yscale, axis_type);

   /* Detect events */
   switch (event->type)
      {
      case GDK_DELETE:
         gtk_main_quit();
         break;

      case GDK_DESTROY:
         gtk_main_quit();
         break;

      case GDK_KEY_PRESS:
         state  = ((GdkEventKey*) event)->state;
         keyval = ((GdkEventKey*) event)->keyval;
         if ( state == 4 && keyval == 99 )
            {
            /* Ctrl-C */
            gtk_main_quit();
            }

         else if ( state == 0 && keyval == 65362 )
            {
            /* Up arrow */
            if ( strcmp(axis_type, "linear") == 0 )
               {
               if ( contour_labels_flag == 1 )
                  ViewLabelCallback(1);
               else if ( coords_display_flag == 1 )
                  ViewCoordsDisplayCallback(1);
               }
            }

         else if ( state == 0 && keyval == 65364 )
            {
            /* Down arrow */
            if ( strcmp(axis_type, "linear") == 0 )
               {
               if ( contour_labels_flag == 1 )
                  ViewLabelCallback(-1);
               else if ( coords_display_flag == 1 )
                  ViewCoordsDisplayCallback(-1);
               }
            }

         break;


      case GDK_BUTTON_PRESS:
         if ( flag_2d_rect == 1 )
            {
            /* Get window coordinates of mouse position 1 for zoom rectangle */
            if ( ((GdkEventButton*) event)->button == 1 &&
                 contour_labels_flag == 0 )
               {
               flag_button1 = 0;

               x1_window = ((GdkEventButton*) event)->x;
               y1_window = ((GdkEventButton*) event)->y;

               if ( x1_window < x1_box )
                  x1_window = x1_box;
               else if ( x1_window > x2_box )
                  x1_window = x2_box;

               if ( y1_window < y1_box )
                  y1_window = y1_box;
               else if ( y1_window > y2_box )
                  y1_window = y2_box;

               x2_window = x1_window;
               y2_window = y1_window;

               zoom_rectangle = gnome_canvas_item_new(group,
                                                      GNOME_TYPE_CANVAS_RECT,
                                                      "x1", x1_window,
                                                      "x2", x2_window,
                                                      "y1", y1_window,
                                                      "y2", y2_window,
                                                      "fill_color_rgba", 
                                                       zoom_fill_color,
                                                      "outline_color_rgba", 
                                                       canvas_fg_color,
                                                      "width_pixels", 1,
                                                      NULL);

               flag_button1 = 1;
               }

            /* Write 2d contour-plot labels */
            else if ( ((GdkEventButton*) event)->button == 1 &&
                      contour_labels_flag == 1 )
               {
               /* Get mouse coordinates */
               xmouse = ((GdkEventButton*) event)->x;
               ymouse = ((GdkEventButton*) event)->y;
               ViewLabelCallback(0);

               /* Write label */
               if ( contour_label != NULL )
                  {
                  memset(label, 0, sizeof(label));
                  snprintf(label, sizeof(label), "%g", *contour_label);
                  text = gnome_canvas_item_new(group,
                                               GNOME_TYPE_CANVAS_TEXT,
                                               "text", label,
                                               "x", xmouse,
                                               "y", ymouse,
                                               "anchor", GTK_ANCHOR_CENTER,
                                               "justification", GTK_JUSTIFY_CENTER,
                                               "font-desc", font_text_0,
                                               "fill_color_rgba", 
                                               canvas_fg_color,
                                               NULL);

                  gnome_canvas_item_get_bounds(text, &xtext1, &ytext1, &xtext2, &ytext2);

                  rectangle = gnome_canvas_item_new(group,
                                                    GNOME_TYPE_CANVAS_RECT,
                                                    "x1", xtext1,
                                                    "x2", xtext2,
                                                    "y1", ytext1,
                                                    "y2", ytext2,
                                                    "fill_color_rgba", 
                                                    canvas_bg_color,
                                                    "outline_color_rgba", 
                                                    canvas_bg_color,
                                                    "width_pixels", 1,
                                                    NULL);

                  gnome_canvas_item_raise_to_top(text);
                  }
               }
            }

         /* Zoom out */
         if ( ((GdkEventButton*) event)->button == 3 &&
              contour_labels_flag == 0 )
            {
            /* Stop coordinates display */
            flag = 0;
            if ( coords_display_flag == 1 )
               {
               ViewCoordsDisplayHandler();
               flag = 1;
               }

            /* Destroy old canvas group and zoom out */
            if ( gdk_pointer_is_grabbed() == TRUE )
               gdk_pointer_ungrab(GDK_CURRENT_TIME);
            gtk_object_destroy(GTK_OBJECT(group));
            gdk_window_set_cursor((GdkWindow *) window->window, NULL);
            group = GNOME_CANVAS_GROUP(gnome_canvas_item_new(group_root,
                                                             GNOME_TYPE_CANVAS_GROUP,
                                                             "x", 0.0,
                                                             "y", 0.0,
                                                             NULL));
/* Re-initialize plot canvas */
            jlp_gseg0->GSEG_gnome_InitializePlotCanvas(group, height_menu_bar);

            GSEG_ZoomOut();

            /* Restart coordinates display */
            if ( flag == 1 )
               ViewCoordsDisplayHandler();
            }

         break;

      case GDK_MOTION_NOTIFY:
         /* Get window coordinates of mouse position and draw zoom rectangle */
         if ( flag_2d_rect == 1 && flag_button1 == 1 
              && contour_labels_flag == 0 )
            {
            /* Grab mouse pointer */
            if ( gdk_pointer_is_grabbed() != TRUE )
               gdk_pointer_grab(window->window,
                                TRUE,
                                GDK_POINTER_MOTION_MASK,
                                window->window,
                                NULL,
                                GDK_CURRENT_TIME);

            /* Get mouse window coordinates */
            x2_window = ((GdkEventMotion*) event)->x;
            y2_window = ((GdkEventMotion*) event)->y;

            if ( x2_window < x1_box )
               x2_window = x1_box;
            else if ( x2_window > x2_box )
               x2_window = x2_box;

            if ( y2_window < y1_box )
               y2_window = y1_box;
            else if ( y2_window > y2_box )
               y2_window = y2_box;

            if ( x2_window >= x1_window )
               {
               x2 = x2_window;
               x1 = x1_window;
               }
            else
               {
               x2 = x1_window;
               x1 = x2_window;
               }

            if ( y2_window >= y1_window )
               {
               y2 = y2_window;
               y1 = y1_window;
               }
            else
               {
               y2 = y1_window;
               y1 = y2_window;
               }

            /* Draw rectangle */
            g_object_set(zoom_rectangle, "x1", x1, "x2", x2, "y1", y1, "y2", y2, NULL);
            }

         /* Write z values of 2d-contour data in upper left corner of plot window */
         if ( flag_2d_rect == 1 && contour_labels_flag == 1 )
            {
            /* Grab mouse pointer */
            gdk_pointer_grab(window->window,
                             TRUE,
                             GDK_POINTER_MOTION_MASK,
                             window->window,
                             NULL,
                             GDK_CURRENT_TIME);

            /* Get mouse window coordinates */
            xmouse = ((GdkEventMotion*) event)->x;
            ymouse = ((GdkEventMotion*) event)->y;

            /* Write values to plot window */
            ViewLabelCallback(0);
            memset(string, 0, sizeof(string));
            if ( zcontour_data != NULL )
               snprintf(string, sizeof(string), "contour plot %d\nz = %g",
                        icontour_plots, *zcontour_data);
            else
               snprintf(string, sizeof(string), "contour plot %d",
                        icontour_plots);
            g_object_set(text_contour_data, "text", string, NULL);
            }

         /* Write plot type in upper left corner of plot window;                    */
         /* write x and y coordinates of mouse in lower left corner of plot window; */
         /* write corresponding z values below cursor position                      */
         if ( flag_2d == 1 && coords_display_flag == 1 )
            {
            /* Grab mouse pointer */
            if ( gdk_pointer_is_grabbed() != TRUE )
               gdk_pointer_grab(window->window,
                                TRUE,
                                GDK_POINTER_MOTION_MASK,
                                window->window,
                                NULL,
                                GDK_CURRENT_TIME);

            /* Get mouse window coordinates */
            xmouse = ((GdkEventMotion*) event)->x;
            ymouse = ((GdkEventMotion*) event)->y;

            /* Write plot type to plot window */
            ViewCoordsDisplayCallback(0);
            memset(string, 0, sizeof(string));
            if ( plot_type_data[itotal_plots-1] > 0 )
               snprintf(string, sizeof(string), "color plot %d",
                        icolor_plots);
            else if ( plot_type_data[itotal_plots-1] < 0 )
               snprintf(string, sizeof(string), "contour plot %d",
                        icontour_plots);
            g_object_set(text_plot_data, "text", string, NULL);

            /* Write x and y values to plot window */
            memset(string, 0, sizeof(string));
            if ( xy_coords_data[0] != NULL && xy_coords_data[1] != NULL )
               {
               if ( flag_2d_rect == 1 )
                  snprintf(string, sizeof(string), "x = %g\ny = %g",
                           *xy_coords_data[0], *xy_coords_data[1]);
               else if ( flag_polar == 1 )
                  snprintf(string, sizeof(string), "\xCE\xB8 = %g\nr = %g",
                           *xy_coords_data[0], *xy_coords_data[1]);
               }
            g_object_set(text_xy_coords_data, "text", string, NULL);

            /* Write z value below cursor position */
            memset(string, 0, sizeof(string));
            if ( z_coords_data != NULL )
               {
               if ( flag_2d_rect == 1 )
                  snprintf(string, sizeof(string), "z = %g", *z_coords_data);
               }
            g_object_set(text_z_coords_data,
                         "x", xmouse, "y", ymouse+10.0, "text", string, NULL);
            }

         break;

      case GDK_BUTTON_RELEASE:
         /* Zoom in */
         if ( flag_2d_rect == 1 &&
              flag_button1 == 1 &&
              ((GdkEventButton*) event)->button == 1 &&
              contour_labels_flag == 0 )
            {
            flag_button1 = 0;

            /* Stop coordinates display */
            flag = 0;
            if ( coords_display_flag == 1 )
               {
               ViewCoordsDisplayHandler();
               flag = 1;
               }

            /* Ungrab mouse pointer and destroy last rectangle  */
            if ( gdk_pointer_is_grabbed() == TRUE )
               gdk_pointer_ungrab(GDK_CURRENT_TIME);
            gtk_object_destroy(GTK_OBJECT(zoom_rectangle));

            /* Check rectangle coordinates */
            if ( x2_window != x1_window && y2_window != y1_window )
               {
               /* Destroy old canvas group */
               gtk_object_destroy(GTK_OBJECT(group));

               /* Create new canvas group */
               group = GNOME_CANVAS_GROUP(gnome_canvas_item_new(group_root,
                                                                GNOME_TYPE_CANVAS_GROUP,
                                                                "x", 0.0,
                                                                "y", 0.0,
                                                                NULL));

/* Re-initialize plot canvas */
               jlp_gseg0->GSEG_gnome_InitializePlotCanvas(group, height_menu_bar);

               /* Redraw plot */
               GSEG_ZoomIn(x1_window, y1_window, x2_window, y2_window);
               }

            /* Restart coordinates display */
            if ( flag == 1 )
               ViewCoordsDisplayHandler();
            }

         break;

      case GDK_CONFIGURE:
         /* Draw graph first time */
         if ( p_window_data->flag == 1 )
            {
            /* Draw graph */
            GetAxisLabelPixbufs();
            DrawGraph();
            p_window_data->flag = 2;
            flag_button1 = 0;

            /* Save plot */
            if ( save_filename0 != NULL )
               FileOK(NULL);

            /* Signal plot complete */
            if ( p_tempfile != NULL )
               {
               if ( (fptr = fopen(p_tempfile, "w")) != NULL )
                  {
                  fprintf(fptr, "%s", "complete");
                  fclose(fptr);
                  }
               }
            /* printf("complete\n"); */

            /* Close plot window */
            if ( close_flag == 1 )
               {
               gtk_main_quit();
               }

            break;
            }

         /* Get old window size data */
         window_width_old  = p_window_data->width;
         window_height_old = p_window_data->height;

         /* Save new window size data */
         p_window_data->width  = ((GdkEventConfigure*) event)->width;
         p_window_data->height = ((GdkEventConfigure*) event)->height;

         /* Get new window size data */
         window_width_new  = p_window_data->width;
         window_height_new = p_window_data->height;

         /* Check window size change */
         if ( window_width_new  == window_width_old &&
              window_height_new == window_height_old )
            break;

         /* Calculate new tick-mark and dashed-line variables */
         if ( window_width_new >= window_height_new )
            max_dimension = window_width_new;
         else
            max_dimension = window_height_new;

// Calculate dash parameters:
         tick_major_0 = (8.0/768.0)*max_dimension;
         tick_minor_0 = 0.5*tick_major_0;
         dash_0 = tick_major_0;
         space_dash_0 = 0.5*dash_0;
         space_dot_0 = space_dash_0 - 1.0;
         GSEG_SetDashParameters(tick_major_0, tick_minor_0,    
                                dash_0, space_dash_0, space_dot_0);


         /* Stop coordinates display */
         flag = 0;
         if ( coords_display_flag == 1 )
            {
            ViewCoordsDisplayHandler();
            flag = 1;
            }

         /* Destroy old canvas group */
         gtk_object_destroy(GTK_OBJECT(group));

         /* Create new canvas group */
         group = GNOME_CANVAS_GROUP(gnome_canvas_item_new(group_root,
                                                          GNOME_TYPE_CANVAS_GROUP,
                                                          "x", 0.0,
                                                          "y", 0.0,
                                                          NULL));

/* Re-initialize plot canvas */
         jlp_gseg0->GSEG_gnome_InitializePlotCanvas(group, height_menu_bar);

         /* Draw graph */
         DrawGraph();

         /* Restart coordinates display */
         if ( flag == 1 )
            ViewCoordsDisplayHandler();

         break;

      default:
         break;
      }

   return FALSE;
   }
