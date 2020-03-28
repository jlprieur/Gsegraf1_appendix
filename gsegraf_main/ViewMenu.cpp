/*******************************************************************************
* ViewMenu.c
*
* Contains functions:
*    ViewAxesHandler
*    ViewAxesCallback
*    ViewRotateHandler
*    ViewRotateCallback
*    ViewLabelHandler
*    ViewLabelCallback
*    ViewCoordsDisplayHandler
*    ViewCoordsDisplayCallback
*
* Functions for View menu items.
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
* JLP
* Version 21/04/2017
*******************************************************************************/
#include <math.h>
#include "gsegraf.h"

/**************************************************************************
* Prompting for new axes limits
***************************************************************************/
void ViewAxesHandler ( void )
   {
/* Declare variables */
   char string[128], prompt[512];

// Create the string that will be displayed for prompting new axes limits:
   jlp_gsegraf0->GSEG_QuestionForChangingAxesLimits(prompt, string);

   /* Open dialog box */
   RequestDialog(prompt, string, ViewAxesCallback);

   return;
   }


/**************************************************************************
* Redrawing with new axes limits
***************************************************************************/
void ViewAxesCallback ( GtkEntry *entry_box )
{
/* Declare variables */
int status;
int i, disp_coord_flag, dcflag;
double xmin, xmax, ymin, ymax, zmin, zmax;
char *string = NULL;
char error_message[128];

/* Get entry-box string */
 g_object_get(entry_box, "text", &string, NULL);

// Decode the input string and change the axes limits
 status = jlp_gsegraf0->GSEG_DecodeStringForChangingAxesLimits(string, 
                                              &xmin, &xmax, 
                                              &ymin, &ymax, &zmin, &zmax, 
                                              error_message);
 if (status != 0) {
  JLP_ErrorDialog(error_message);
  return;
  }

// Change canvas, and redraw the plot
/* Destroy old canvas group */
 disp_coord_flag = 0;
 if ( coords_display_flag == 1 )
    {
    ViewCoordsDisplayHandler();
    disp_coord_flag = 1;
    }
 gtk_object_destroy(GTK_OBJECT (group));


/* Create new canvas group */
 group = GNOME_CANVAS_GROUP(gnome_canvas_item_new(group_root,
                                                  GNOME_TYPE_CANVAS_GROUP,
                                                  "x", 0.0,
                                                  "y", 0.0,
                                                  NULL));
/* Re-initialize plot canvas */
 jlp_gseg0->GSEG_gnome_InitializePlotCanvas(group, height_menu_bar);

/* Redraw plot */
 jlp_gsegraf0->GSEG_ReDrawPlot_Scaled(xmin, xmax, ymin, ymax, zmin, zmax,
                        &dcflag);

/* Restart coordinates display */
  if((disp_coord_flag == 1) && (dcflag == 1))
         ViewCoordsDisplayHandler();

 free(string);

 return;
}

/*************************************************************************
*
*************************************************************************/
void ViewRotateHandler ( void )
{
/* Declare variables */
int status;
char string[128], prompt[512];

   /* Create dialog box */
  status = jlp_gsegraf0->GSEG_QuestionForChangingAxesRotation(prompt, string);
   if ( status == 0 )
      {
/* Open dialog box */
      RequestDialog(prompt, string, ViewRotateCallback);
      }

return;
}

/*************************************************************************
*
*************************************************************************/
void ViewRotateCallback ( GtkEntry *entry_box )
   {
   /* Declare variables */
   double phi, theta;
   char *string = NULL;
   const char *error_str[] =
      { "Incorrect number of values;\ntwo expected.",
        "Elevation angle out of range." };


   /* Get entry-box string */
   g_object_get(entry_box, "text", &string, NULL);


   /* Get view-direction data */
   if ( string != NULL )
      {
      if ( sscanf(string, "%lf %lf", &phi, &theta) != 2 )
         {
         JLP_ErrorDialog(error_str[0]);
         free(string);
         return;
         }

      free(string);

      if ( theta >= 0.0 && theta <= 90.0 )
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

/* Draw plot */
         jlp_gsegraf0->GSEG_ReDrawPlot3D_Rotated(phi, theta);
         }
      else
         {
         JLP_ErrorDialog(error_str[1]);
         return;
         }
      }

   else
      return;
   }


void ViewLabelHandler ( void )
   {
   /* Declare variables */
   UINT32 zoom_fill_color, canvas_fg_color, canvas_bg_color;
   int iplot;
   GtkWidget *window;
   GdkCursor *cursor;

// Get current plot color values:
   jlp_gsegraf0->GetColorValues(&zoom_fill_color, &canvas_fg_color,
                                &canvas_bg_color);

   /* Reset flag */
   window = (GtkWidget *) p_window_data.window;
   if ( contour_labels_flag == 0 )
      {
      /* Turn xy_coords_display off */
      if ( coords_display_flag == 1 )
         {
         coords_display_flag = 0;
         if ( gdk_pointer_is_grabbed() == TRUE )
            gdk_pointer_ungrab(GDK_CURRENT_TIME);
         if ( G_IS_OBJECT(GTK_OBJECT(text_plot_data)) )
            gtk_object_destroy(GTK_OBJECT(text_plot_data));
         if ( G_IS_OBJECT(GTK_OBJECT(text_xy_coords_data)) )
            gtk_object_destroy(GTK_OBJECT(text_xy_coords_data));
         if ( G_IS_OBJECT(GTK_OBJECT(text_z_coords_data)) )
            gtk_object_destroy(GTK_OBJECT(text_z_coords_data));
         }

      /* Set cursor and initialize text canvas item */
      contour_labels_flag = 1;
      icontour_plots = 1;
      cursor = gdk_cursor_new(GDK_CROSSHAIR);
      gdk_window_set_cursor((GdkWindow *) window->window, cursor);
      text_contour_data = gnome_canvas_item_new(group,
                                                GNOME_TYPE_CANVAS_TEXT,
                                                "x", 10.0,
                                                "y", 10.0,
                                                "anchor", GTK_ANCHOR_NORTH_WEST,
                                                "justification", GTK_JUSTIFY_LEFT,
                                                "font-desc", font_text_0,
                                                "fill_color_rgba", 
                                                canvas_fg_color,
                                                NULL);
      /* Set axes and xy_coords menu-items sensitivity */
      gtk_widget_set_sensitive(axes_menu_item, FALSE);
      gtk_widget_set_sensitive(xy_coords_menu_item, FALSE);
      }

   else if ( contour_labels_flag == 1 )
      {
      icontour_plots = 1;
      contour_labels_flag = 0;
      if ( gdk_pointer_is_grabbed() == TRUE )
         gdk_pointer_ungrab(GDK_CURRENT_TIME);
      if ( G_IS_OBJECT(GTK_OBJECT(text_contour_data)) )
         gtk_object_destroy(GTK_OBJECT(text_contour_data));
      gdk_window_set_cursor((GdkWindow *) window->window, NULL);
      gtk_widget_set_sensitive(axes_menu_item, TRUE);
      gtk_widget_set_sensitive(xy_coords_menu_item, TRUE);
      }

   return;
   }


/***********************************************************************
*
***********************************************************************/
void ViewLabelCallback ( int inc )
{
int ncontour_plots0, status; 
static double z_data0, nearest_contour0;

   jlp_gsegraf0->GSEG_get_ncontour_plots(&ncontour_plots0);

/* Check icontour_plots value */
   icontour_plots = icontour_plots + inc;
   if ( icontour_plots < 1 )
      icontour_plots = 1;
   else if ( icontour_plots > ncontour_plots0)
      icontour_plots = ncontour_plots0;

status = jlp_gsegraf0->GSEG_CreateContourLabel(xmouse, ymouse, icontour_plots, 
                                         &z_data0, &nearest_contour0);
if(status == 0) {
  zcontour_data = &z_data0;
  contour_label = &nearest_contour0;
 }

return;
}


/***********************************************************************
*
***********************************************************************/
void ViewCoordsDisplayHandler ( void )
{
UINT32 zoom_fill_color, canvas_fg_color, canvas_bg_color;
int iplot, nplots, icount_color, icount_contour;
int gseg_plot_type0;
GtkWidget *window;
GdkCursor *cursor;

// Get current plot color values:
jlp_gsegraf0->GetColorValues(&zoom_fill_color, &canvas_fg_color,
                             &canvas_bg_color);

/* Reset flag */
window = (GtkWidget *) p_window_data.window;
if ( coords_display_flag == 0 )
   {
/* Get number of color or contour plots */
   jlp_gsegraf0->GSEG_get_nplots(&nplots);
   plot_type_data = new int[nplots];
   icount_color = 0;
   icount_contour = 0;
   for ( iplot=1; iplot<=nplots; iplot++ )
      {
// Get current plot types
   jlp_gsegraf0->GSEG_get_plot_type(iplot, &gseg_plot_type0);

// 4="color"
      if ( gseg_plot_type0 == 4)
         {
         icount_color++;
         plot_type_data[iplot-1] = icount_color;
         }
// 3="contour" 
      else if ( gseg_plot_type0 == 4)
         {
         icount_contour++;
         plot_type_data[iplot-1] = -icount_contour;
         }
      else
         plot_type_data[iplot-1] = 0;
      }

/* Set cursor and initialize text canvas item */
   coords_display_flag = 1;
   itotal_plots = 1;
   icolor_plots = 1;
   icontour_plots = 1;
   cursor = gdk_cursor_new(GDK_CROSSHAIR);
   gdk_window_set_cursor((GdkWindow *) window->window, cursor);
   text_plot_data = gnome_canvas_item_new(group,
                                          GNOME_TYPE_CANVAS_TEXT,
                                          "x", 10.0,
                                          "y", 10.0,
                                          "anchor", GTK_ANCHOR_NORTH_WEST,
                                          "justification", GTK_JUSTIFY_LEFT,
                                          "font-desc", font_text_0,
                                          "fill_color_rgba", 
                                          canvas_fg_color,
                                          NULL);
   text_xy_coords_data = gnome_canvas_item_new(group, GNOME_TYPE_CANVAS_TEXT,
                                               "x", 10.0,
                                               "y", p_window_data.height - height_menu_bar - 10.0,
                                               "anchor", GTK_ANCHOR_SOUTH_WEST,
                                               "justification", GTK_JUSTIFY_LEFT,
                                               "font-desc", font_text_0,
                                               "fill_color_rgba", 
                                               canvas_fg_color,
                                               NULL);
   text_z_coords_data = gnome_canvas_item_new(group, GNOME_TYPE_CANVAS_TEXT,
                                        "anchor", GTK_ANCHOR_NORTH,
                                           "justification", GTK_JUSTIFY_CENTER,
                                              "font-desc", font_text_0,
                                              "fill_color_rgba", 
                                               canvas_fg_color,
                                              NULL);

/* Set axes menu-item sensitivity */
   gtk_widget_set_sensitive(axes_menu_item, FALSE);
   }

else if ( coords_display_flag == 1 )
   {
   coords_display_flag = 0;
   itotal_plots = 1;
   icolor_plots = 1;
   icontour_plots = 1;
   delete[] plot_type_data;
   if ( gdk_pointer_is_grabbed() == TRUE )
      gdk_pointer_ungrab(GDK_CURRENT_TIME);
   if ( G_IS_OBJECT(GTK_OBJECT(text_plot_data)) )
      gtk_object_destroy(GTK_OBJECT(text_plot_data));
   if ( G_IS_OBJECT(GTK_OBJECT(text_xy_coords_data)) )
      gtk_object_destroy(GTK_OBJECT(text_xy_coords_data));
   if ( G_IS_OBJECT(GTK_OBJECT(text_z_coords_data)) )
      gtk_object_destroy(GTK_OBJECT(text_z_coords_data));
   gdk_window_set_cursor((GdkWindow *) window->window, NULL);
   gtk_widget_set_sensitive(axes_menu_item, TRUE);
   }

return;
}
/***********************************************************************
*
***********************************************************************/
void ViewCoordsDisplayCallback ( int inc )
{
int nplots, plot_type_data0;

/* Get number of color or contour plots */
 jlp_gsegraf0->GSEG_get_nplots(&nplots);

/* Check itotal_plots value */
 itotal_plots = itotal_plots + inc;
 if ( itotal_plots < 1 )
    itotal_plots = 1;
 else if ( itotal_plots > nplots )
    itotal_plots = nplots;

 plot_type_data0 = plot_type_data[itotal_plots-1];
// Color plots:
 if(plot_type_data0 > 0) {
   icolor_plots = plot_type_data0;
// Contour plots
 } else if ( plot_type_data0 < 0 ) {
   icontour_plots = -plot_type_data0;
 }

 jlp_gsegraf0->GSEG_UserFromDeviceCoord(xmouse, ymouse, plot_type_data0,
                          &(xy_coords_data[0]),
                          &(xy_coords_data[1]), &z_coords_data);
return;
}
