/*******************************************************************************
*
* EditMenu.c
*
* Contains functions:
*    EditCopyHandler
*    GetWindowPixbuf
*
* Functions for Edit menu item.
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


void EditCopyHandler ( void )
   {
   /* Declare variables */
   GtkClipboard *clipboard;


   /* Load pixbuf */
   GetWindowPixbuf();


   /* Save pixbuf to clipboard */
   clipboard = gtk_clipboard_get(GDK_NONE);
   gtk_clipboard_set_image(clipboard, pixbuf_window_0);

   return;
   }


void GetWindowPixbuf ( void )
   {
   /* Declare variables */
   int width_image, height_image, xoffset_image, yoffset_image;
   double x1, y1, x2, y2;


   /* Wait for canvas update */
   gnome_canvas_get_scroll_region((GnomeCanvas*) p_window_data->canvas,
                                  &x1, &y1, &x2, &y2);
   gnome_canvas_request_redraw((GnomeCanvas*) p_window_data->canvas,
                               roundint(x1), roundint(y1),
                               roundint(x2), roundint(y2));
   while ( gtk_events_pending() )
      gtk_main_iteration();


   /* Unreference pixbuf_window */
   if ( G_IS_OBJECT(pixbuf_window_0) )
      {
      g_object_unref(G_OBJECT(pixbuf_window_0));
      pixbuf_window_0 = NULL;
      }


   /* Load window image into pixbuf */
   width_image   = p_window_data->width;
   height_image  = p_window_data->height - height_menu_bar;
   xoffset_image = 0;
   yoffset_image = 0;
   pixbuf_window_0 = gdk_pixbuf_get_from_drawable(NULL,
                                                (GdkDrawable *) p_window_data->canvas->window,
                                                NULL,
                                                xoffset_image,
                                                yoffset_image,
                                                0,
                                                0,
                                                width_image,
                                                height_image);

   return;
   }
