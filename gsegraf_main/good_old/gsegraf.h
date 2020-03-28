/*****************************************************************************
*
* gsegraf.h
*
* Declares structures, variables and function prototypes of gsegraf
*
* This file was modified from GSEGrafix (gsegrafix-1.0.6, sept. 2011)
* Copyright © 2008, 2009, 2010, 2011 Spencer A. Buckner
* http://savannah.gnu.org/projects/gsegrafix
*
* JLP
* Version 13/10/2016
******************************************************************************/
#ifndef gsegraf_h_
#define  gsegraf_h_

#include "jlp_gsegraf_defs.h"       // in jlp_gsegraf
#include "jlp_gsegraf_prototypes.h" // in jlp_gsegraf

#include "jlp_gnome.h"              // in jlp_gnome
#include "jlp_gnome_defs.h"         // in jlp_gnome
#include "jlp_gnome1_prototypes.h"  // in jlp_gnome
#include "jlp_gnome_prototypes.h"   // in jlp_gnome

#include "gsegraf_prototypes.h"     // in this folder 

#ifndef PI
#define PI 3.14159
#endif
#define DEG2RAD (PI/180.0)

#ifdef MAIN_PROGRAM
#define GSEGRAF_EXTERN
#else
#define GSEGRAF_EXTERN extern
#endif

/************** gsegraf parameters *****************************************/
/* Window's parameters : */
GSEGRAF_EXTERN window_data_type   *p_window_data;

/* Declare gnome canvas group pointers */
GSEGRAF_EXTERN GnomeCanvasGroup *group_root, *group;

/* Declare menu bar variables */
GSEGRAF_EXTERN int height_menu_bar;
GSEGRAF_EXTERN GtkMenuBar *menu_bar;
GSEGRAF_EXTERN GdkPixbuf *pixbuf_logo;

// Set in "CreateMenuBar.c"  
GSEGRAF_EXTERN GtkWidget *axes_menu_item, *xy_coords_menu_item;
 
/* Declare variables for x-y coordinates display */
GSEGRAF_EXTERN int *plot_type_data, coords_display_flag, icolor_plots, itotal_plots;
GSEGRAF_EXTERN double xmouse, ymouse, *xy_coords_data[2], *z_coords_data;
GSEGRAF_EXTERN GnomeCanvasItem *text_plot_data, *text_xy_coords_data, *text_z_coords_data;

/* Declare variables for 2d-contour labeling */
GSEGRAF_EXTERN int contour_labels_flag, icontour_plots;
GSEGRAF_EXTERN GnomeCanvasItem *text_contour_data;
GSEGRAF_EXTERN double *zcontour_data, *contour_label;

// Set in "GSEGraf.c"  
GSEGRAF_EXTERN char *window_title, *parameter_file, *temporary_file;

GSEGRAF_EXTERN char *p_tempfile;
GSEGRAF_EXTERN int close_flag;

// From plot:
// Plot non-gnome variables:
GSEGRAF_EXTERN char *save_filename0;

// Plot gnome variables:
GSEGRAF_EXTERN PangoFontDescription *font_text_0;
/* Declare pixbufs */
GSEGRAF_EXTERN GdkPixbuf *pixbuf_window_0;
GSEGRAF_EXTERN GdkPixbuf *pixbuf_xlabel_0, *pixbuf_ylabel_0, *pixbuf_zlabel_0, *pixbuf_title_0;

#endif
