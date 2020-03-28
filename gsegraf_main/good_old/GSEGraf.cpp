/*******************************************************************************
*
* GSEGraf.c
*
* Build window, specify plot-box position and size, create anti-aliased canvas,
* connect event handler, create child canvas groups, create window menu bar,
* get plot-parameter filename, and draw graph.
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
/* JLP 2016 to solve problem of numeric dot vs numeric comma*/
#include <stdio.h> 
#include <locale.h> 

#define MAIN_PROGRAM
#include "gsegraf.h"

/* Declare global variables for this file */
const char app_id[] = "gsegraf";
/* Declare external variables for other files */
char app_version[64];

int main ( int argc, char *argv[] )
{
/* Declare variables */
int i;
unsigned int size, rr, gg, bb;
char *filename_logo = NULL;
const char *error_str[] =
      { "Must specify plot-parameter filename.",
        "Too many arguments." };
GnomeProgram *program;
GnomeApp *window;
GtkWidget *canvas;
GtkStyle *style;
GdkColor color;

UINT32 zoom_fill_color, canvas_fg_color, canvas_bg_color;
int width_screen, height_screen;

/* Trick to allocate memory space to p_window_data pointer */
static window_data_type window_data;

strcpy(app_version, "1.0.6");

/* JLP 2016 to solve problem of numeric dot vs numeric comma */
/* In the "C" locale, a decimal_point is "." */
if (NULL == setlocale(LC_NUMERIC, "C")) {
  fprintf(stderr, " Error setting numeric dot in setlocale !\n");
  exit(-1);
}

/* Free memory at program termination */
   atexit(FreeMemory);


   /* Initialize GNOME */
   program = gnome_program_init(app_id, app_version,
                                LIBGNOMEUI_MODULE,
                                argc, argv,
                                GNOME_PROGRAM_STANDARD_PROPERTIES,
                                GNOME_PARAM_HUMAN_READABLE_NAME, "gsegraf",
                                NULL);


   /* Get logo pixbuf */
   filename_logo = gnome_program_locate_file(program,
                                             GNOME_FILE_DOMAIN_APP_PIXMAP,
                                             "gsegrafix-logo.png",
                                             TRUE,
                                             NULL);
   if ( filename_logo != NULL )
      {
      pixbuf_logo = gdk_pixbuf_new_from_file(filename_logo, NULL);
      gtk_window_set_default_icon(pixbuf_logo);
      free(filename_logo);
      }
// Testing another possibility:
   else if ( (pixbuf_logo = gdk_pixbuf_new_from_file("./figures/gsegrafix-logo.png", NULL)) != NULL )
      gtk_window_set_default_icon(pixbuf_logo);

   /* Read argument vector */
   if ( argc < 2 )
      {
      JLP_ErrorDialog(error_str[0]);
      exit(1);
      }

   else if ( argc <= 3 )
      {
      if ( strcmp(argv[1], "-h") == 0 )
         {
         HelpHandler();
         exit(0);
         }
      else
         parameter_file = argv[1];
      }

   else if ( argc > 3 )
      {
      JLP_ErrorDialog(error_str[1]);
      exit(1);
      }

   temporary_file = NULL;
   if ( argc == 3 )
      temporary_file = argv[2];

/* Initialize window data structure:
 width and height may be changed later by "EventHandler.c"
 */
   p_window_data = &window_data;
   p_window_data->x      = 0;
   p_window_data->y      = 0;
   p_window_data->width  = 768;
   p_window_data->height = 576;
   p_window_data->flag = 1;   /* flag used by EventHandler and GetAxisLabelPixbufs */

// Initialization of other private variables:
   coords_display_flag = 0;
   contour_labels_flag = 0;
   p_tempfile = temporary_file;

/* Initialize plot */
   GSEG_gnome_InitializePlot(p_window_data);
   GSEG_InitializePlot(p_window_data->width, p_window_data->height,
                       parameter_file, &save_filename0, &close_flag);

   GSEG_gnome_SetPointersFromPlot(&pixbuf_window_0, &pixbuf_xlabel_0, 
                                  &pixbuf_ylabel_0, &pixbuf_zlabel_0, 
                                  &pixbuf_title_0, &font_text_0);
   GSEG_GetColorValues(&zoom_fill_color, &canvas_fg_color, 
                       &canvas_bg_color);

   /* Create main window */
   size = strlen("GSEGrafix: ") + strlen(parameter_file);
   window_title = new char[size + 1];
   sprintf(window_title, "%s%s", "GSEGrafix: ", parameter_file);
   window = (GnomeApp*)g_object_new(GNOME_TYPE_APP,
                         "title", window_title,
                         "app-id", app_id,
                         "default-width", p_window_data->width,
                         "default-height", p_window_data->height,
                         NULL);


   /* gtk_widget_set_uposition(GTK_WIDGET(window), 0, 0); */
   p_window_data->window = window;


   /* Create menu bar */
   CreateMenuBar();

   /* Create antialiased canvas */
   canvas = gnome_canvas_new_aa();
   p_window_data->canvas = canvas;


   /* Set canvas background color */
   rr = (canvas_bg_color / 0x1000000);
   gg = (canvas_bg_color - rr*0x1000000)/0x10000;
   bb = (canvas_bg_color - rr*0x1000000 - gg*0x10000)/0x100;
   color.pixel = canvas_bg_color;
   color.red   = rr*0x100 + 0xFF;
   color.green = gg*0x100 + 0xFF;
   color.blue  = bb*0x100 + 0xFF;
   style = gtk_widget_get_style(canvas);
   for ( i=1; i<=5; i++ )
      style->bg[i-1] = color;
   gtk_widget_set_style(canvas, style);

   /* Set canvas scroll region */
   width_screen  = gdk_screen_width();
   height_screen = gdk_screen_height();

   gnome_canvas_set_scroll_region(GNOME_CANVAS(canvas),
                                  0.0,
                                  0.0,
                                  (double) width_screen,
                                  (double) height_screen);
   gnome_canvas_set_center_scroll_region(GNOME_CANVAS(canvas), 1);


   /* Connect EventHandler function */
   g_signal_connect(window, "event", G_CALLBACK(EventHandler), NULL);


   /* Retrieve canvas root group and create child canvas group */
   group_root = gnome_canvas_root(GNOME_CANVAS(canvas));
   group = GNOME_CANVAS_GROUP(gnome_canvas_item_new(group_root,
                                                    GNOME_TYPE_CANVAS_GROUP,
                                                    "x", 0.0,
                                                    "y", 0.0,
                                                    NULL));


   /* Add canvas to window */
   gnome_app_set_contents(window, canvas);


   /* Show window */
   gtk_widget_show_all(GTK_WIDGET(window));


   /* Get menu-bar height */
   /* height_menu_bar = 25 (Fedora 8) */
   height_menu_bar = GTK_WIDGET(menu_bar)->allocation.height;


/* Initialize plot canvas */
   jlp_gseg0->GSEG_gnome_InitializePlotCanvas(group, height_menu_bar);

   /* Start application */
   gtk_main();

   return 0;
   }
