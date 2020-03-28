/*******************************************************************************
*
* FileMenu.c
*
* Contains functions:
*    FileSaveAsHandler
*    FileOK
*    FilePrintHandler
*    FileCloseHandler
*    FreeMemory
*    
* Functions for File menu items.
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


/* Declare external variables */
extern char *window_title;


void FileSaveAsHandler ( void )
   {
   /* Declare variables */
   int result;
   GtkWidget *file_chooser;


   /* Create file-selection dialog */
   file_chooser = gtk_file_chooser_dialog_new("Specify image filename",
                                              NULL,
                                              GTK_FILE_CHOOSER_ACTION_SAVE,
                                              GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                              GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                              NULL);

   gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser), g_get_home_dir());
   gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER(file_chooser), "plot.png");
   gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(file_chooser), FALSE);


   /* Get filename */
   result = gtk_dialog_run(GTK_DIALOG(file_chooser));
   if ( result == GTK_RESPONSE_ACCEPT)
      FileOK(file_chooser);
   else
      gtk_widget_destroy(file_chooser);

   return;
   }


void FileOK ( GtkWidget *file_chooser )
   {
   /* Declare variables */
   char *type = NULL;
   const char *error_str[] =
      { "Legitimate filename not specified.",
        "Image filename must have extension specifying image format.",
        "Specified image format not supported;\n"
        "supported formats are: bmp, jpeg, and png." };
   FILE *fptr = NULL;


   /* Get filename */
   if ( file_chooser != NULL )
      {
      free(save_filename0);
      save_filename0 = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
      }


   /* Save plot to file */
   if ( (fptr = fopen(save_filename0, "w")) == NULL )
      {
      free(save_filename0);
      if ( file_chooser != NULL )
         gtk_widget_destroy(file_chooser);
      JLP_ErrorDialog(error_str[0]);
      return;
      }

   else if ( (type = strrchr(save_filename0, '.')) == NULL )
      {
      fclose(fptr);
      remove(save_filename0);
      free(save_filename0);
      if ( file_chooser != NULL )
         gtk_widget_destroy(file_chooser);
      JLP_ErrorDialog(error_str[1]);
      return;
      }

   else if ( strcmp(type,  ".bmp") != 0 &&
             strcmp(type, ".jpeg") != 0 &&
             strcmp(type,  ".png") != 0 )
      {
      fclose(fptr);
      remove(save_filename0);
      free(save_filename0);
      if ( file_chooser != NULL )
         gtk_widget_destroy(file_chooser);
      JLP_ErrorDialog(error_str[2]);
      return;
      }

   else
      {
      /* Close file */
      fclose(fptr);

      /* Destroy file_chooser widget */
      if ( file_chooser != NULL )
         gtk_widget_destroy(GTK_WIDGET(file_chooser));

      /* Load pixbuf */
      GetWindowPixbuf();

      /* Save window pixbuf to file */
      type++;
      gdk_pixbuf_save(pixbuf_window_0, save_filename0, type, NULL, NULL);
      return;
      }
   }


void FilePrintPreviewHandler ( void )
   {
   /**************************************************************
    *
    * Reference:
    *    http://cvs.gnome.org/viewcvs/libgnomeprintui/examples/
    *
    **************************************************************/

   /* Declare variables */
   GnomePrintJob *job;
   GnomePrintContext *context;
   GtkWidget *preview;


   /* Load pixbuf */
   GetWindowPixbuf();


   /* Create print job and context */
   job = gnome_print_job_new(NULL);
   context = gnome_print_job_get_context(job);


   /* Calculate print variables */
   PrintCalculate(job, context, pixbuf_window_0);


   /* Create print preview */
   preview = gnome_print_job_preview_new(job, (const guchar *) window_title);
   gtk_widget_show(preview);


   /* Destroy objects */
   g_object_unref(G_OBJECT(context));
   g_object_unref(G_OBJECT(job));
   g_object_unref(G_OBJECT(pixbuf_window_0));

   return;
   }


void FilePrintHandler ( void )
   {
   /**************************************************************
    *
    * Reference:
    *    http://cvs.gnome.org/viewcvs/libgnomeprintui/examples/
    *
    * If print is run more than once, or after print_preview, following warnings are printed:
    *
    *    GLib-GObject-WARNING **: invalid (NULL) pointer instance
    *
    *    GLib-GObject-CRITICAL **: g_signal_connect_data: assertion `G_TYPE_CHECK_INSTANCE (instance)' failed
    *
    **************************************************************/

   /* Declare variables */
   int response;
   GnomePrintJob *job;
   GnomePrintContext *context;
   GtkWidget *dialog, *preview;


   /* Load pixbuf */
   GetWindowPixbuf();


   /* Create print job and context */
   job = gnome_print_job_new(NULL);
   context = gnome_print_job_get_context(job);


   /* Run print dialog */
   /*    GNOME_PRINT_DIALOG_RESPONSE_PRINT   =  1 */
   /*    GNOME_PRINT_DIALOG_RESPONSE_PREVIEW =  2 */
   /*    GNOME_PRINT_DIALOG_RESPONSE_CANCEL  = -6 */
   dialog = gnome_print_dialog_new(job, (const guchar *) window_title, GNOME_PRINT_DIALOG_COPIES);
   response = gnome_print_dialog_run(GNOME_PRINT_DIALOG(dialog));


   /* Calculate print variables */
   PrintCalculate(job, context, pixbuf_window_0);


   /* Check dialog response */
   if ( response == GNOME_PRINT_DIALOG_RESPONSE_PREVIEW )
      {
      preview = gnome_print_job_preview_new(job, (const guchar *) window_title);
      gtk_widget_show(preview);
      }

   else if ( response == GNOME_PRINT_DIALOG_RESPONSE_PRINT )
      gnome_print_job_print(job);


   /* Destroy objects */
   gtk_widget_destroy(dialog);
   g_object_unref(G_OBJECT(context));
   g_object_unref(G_OBJECT(job));
   g_object_unref(G_OBJECT(pixbuf_window_0));

   return;
   }


void PrintCalculate ( GnomePrintJob *job, GnomePrintContext *context, GdkPixbuf *pixbuf )
   {
   /* Declare variables */
   gint rowstride, height, width;
   guchar *raw_image;
   gboolean has_alpha;
   double width_image, height_image, ratio_image,
          width_page, height_page, ratio_page,
          width_scale, height_scale, xtranslate, ytranslate;


   /* Get image size */
   width_image  = (double) p_window_data->width;
   height_image = (double) p_window_data->height - (double) height_menu_bar;
   ratio_image = height_image/width_image;


   /* Get page size */
   gnome_print_job_get_page_size(job, &width_page, &height_page);
   ratio_page = height_page/width_page;


   /* Calculate image size and offsets for page size */
   if ( ratio_page >= ratio_image )
      {
      width_scale = 0.90*width_page;
      height_scale = width_scale*ratio_image;
      }
   else
      {
      height_scale = 0.90*height_page;
      width_scale = height_scale/ratio_image;
      }
   xtranslate = (width_page - width_scale)/2.0;
   ytranslate = (height_page - height_scale)/2.0;


   /* Create page */
   gnome_print_beginpage(context, NULL);
   gnome_print_translate(context, xtranslate, ytranslate);
   gnome_print_scale(context, width_scale, height_scale);
   gnome_print_gsave(context);


   /* Print image from pixbuf */
   raw_image = gdk_pixbuf_get_pixels(pixbuf);
   has_alpha = gdk_pixbuf_get_has_alpha(pixbuf);
   rowstride = gdk_pixbuf_get_rowstride(pixbuf);
   height    = gdk_pixbuf_get_height(pixbuf);
   width     = gdk_pixbuf_get_width(pixbuf);
   if (has_alpha)
      gnome_print_rgbaimage(context, (guchar *)raw_image, width, height, rowstride);
   else
      gnome_print_rgbimage(context, (guchar *)raw_image, width, height, rowstride);


   /* Finish rendering page and close print job */
   gnome_print_grestore(context);
   gnome_print_showpage(context);
   gnome_print_job_close(job);

   return;
   }


void FileCloseHandler ( void )
   {
   gtk_main_quit();
   }


void FreeMemory ( void )
   {
   free(window_title);
   free(save_filename0);
   pango_font_description_free(font_text);
   if ( G_IS_OBJECT(pixbuf_window_0) )
      g_object_unref(pixbuf_window_0);
   if ( G_IS_OBJECT(pixbuf_logo) )
      g_object_unref(pixbuf_logo);
   if ( G_IS_OBJECT(pixbuf_xlabel_0) )
      g_object_unref(pixbuf_xlabel_0);
   if ( G_IS_OBJECT(pixbuf_ylabel_0) )
      g_object_unref(pixbuf_ylabel_0);
   if ( G_IS_OBJECT(pixbuf_zlabel_0) )
      g_object_unref(pixbuf_zlabel_0);
   if ( G_IS_OBJECT(pixbuf_title_0) )
      g_object_unref(pixbuf_title_0);
   }

