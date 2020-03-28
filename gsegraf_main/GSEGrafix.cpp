/*******************************************************************************
*
* GSEGrafix.c
*
* Create GSEGrafix main dialog box with buttons connected to file-selection
* dialog box and help files.
*
* Contains functions:
*    FileHandler
*    FileOK
*    DeleteHandler
*    CloseHandler
*    CtrlCHandler
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

#include "gsegrafix.h"
#include "jlp_gsegraf.h"

/* Declare external variables */
const char app_id[] = "gsegrafix";

char app_version[64];
GdkPixbuf *pixbuf_logo;

int main ( int argc, char *argv[] )
   {
   /* Declare variables */
   int i;
   unsigned int size;
   char *filename_logo, *argv_child[3];
   const char *error_str[] =
      { "Program executable, gsegraf, not in PATH.",
        "Too many arguments." };
   GnomeProgram *program;
   GtkWindow *window;
   GtkVBox *vbox;
   GtkHButtonBox *hbuttonbox;
   GtkImage *image_logo;
   GtkWidget *image_file;
   GtkLabel *label1, *label2;
   GtkButton *file_button, *help_button, *about_button, *close_button;

   strcpy(app_version, "1.0.6");

   /* Set pointers to NULL */
   filename_logo = NULL;
   pixbuf_logo = NULL;
   image_logo = NULL;
   for ( i=1; i<=3; i++ )
      argv_child[i-1] = NULL;


   /* Initialize GNOME */
   program = gnome_program_init(app_id, app_version,
                                LIBGNOMEUI_MODULE,
                                argc, argv,
                                GNOME_PROGRAM_STANDARD_PROPERTIES,
                                GNOME_PARAM_HUMAN_READABLE_NAME, "gsegrafix",
                                NULL);


   /* Get logo pixbuf */
   if ( (filename_logo = gnome_program_locate_file(program,
                                                   GNOME_FILE_DOMAIN_APP_PIXMAP,
                                                   "gsegrafix-logo.png",
                                                   TRUE,
                                                   NULL)) != NULL )
      {
      pixbuf_logo = gdk_pixbuf_new_from_file(filename_logo, NULL);
      gtk_window_set_default_icon(pixbuf_logo);
      free(filename_logo);
      }
   else if ( (pixbuf_logo = gdk_pixbuf_new_from_file("./figures/gsegrafix-logo.png", NULL)) != NULL )
      gtk_window_set_default_icon(pixbuf_logo);


   /* Read argument vector */
   if ( argc == 2 )
      {
      if ( strcmp(argv[1], "-h") == 0 )
         {
         HelpHandler();
         if ( G_IS_OBJECT(pixbuf_logo) )
            g_object_unref(pixbuf_logo);
         exit(0);
         }
      else
         {
         size = strlen("gsegraf");
         argv_child[0] = new char[size + 1];
         sprintf(argv_child[0], "%s", "gsegraf");
         size = strlen(argv[1]);
         argv_child[1] = new char[size + 1];
         sprintf(argv_child[1], "%s", argv[1]);
         argv_child[2] = NULL;
         if ( g_spawn_async(NULL,
                            argv_child,
                            NULL,
                            G_SPAWN_SEARCH_PATH,
                            NULL,
                            NULL,
                            NULL,
                            NULL) == 0 )
            {
            JLP_ErrorDialog(error_str[0]);
            if ( G_IS_OBJECT(pixbuf_logo) )
               g_object_unref(pixbuf_logo);
            for ( i=1; i<=3; i++ )
               free(argv_child[i-1]);
            exit(1);
            }

         if ( G_IS_OBJECT(pixbuf_logo) )
            g_object_unref(pixbuf_logo);
         for ( i=1; i<=3; i++ )
            free(argv_child[i-1]);
         exit(0);
         }
      }

   else if ( argc > 2 )
      {
      JLP_ErrorDialog(error_str[1]);
      if ( G_IS_OBJECT(pixbuf_logo) )
         g_object_unref(pixbuf_logo);
      exit(1);
      }


   /* Create main window */
   window = (GtkWindow*)g_object_new(GTK_TYPE_WINDOW,
                         "title", "GSEGrafix",
                         "resizable", FALSE,
                         "border-width", 12,
                         NULL);
   if ( G_IS_OBJECT(pixbuf_logo) )
      g_object_unref(pixbuf_logo);


   /* Create vbox */
   vbox = (GtkVBox*)g_object_new(GTK_TYPE_VBOX, "spacing", 18, NULL);


   /* Create logo image */
   image_logo = NULL;
   if ( G_IS_OBJECT(pixbuf_logo) )
      image_logo = (GtkImage*)g_object_new(GTK_TYPE_IMAGE, "pixbuf", pixbuf_logo, NULL);


   /* Create labels */
   label1 = (GtkLabel*)g_object_new(GTK_TYPE_LABEL,
                         "use-markup", TRUE,
                         "label", "<b><big><big><big>Welcome to GSEGrafix</big></big></big></b>",
                          NULL);

   label2 = (GtkLabel*)g_object_new(GTK_TYPE_LABEL,
                         "use-markup", TRUE,
                         "label", "Press File button to choose plot-parameter file.",
                         NULL);


   /* Create hbuttonbox */
   hbuttonbox = (GtkHButtonBox*)g_object_new(GTK_TYPE_HBUTTON_BOX, "spacing", 12, NULL);


   /* Create buttons */
   /*
   Following does not work; GTK_STOCK_FILE has no "label" property.
   See /usr/bin/gtk-demo.
   file_button = (GtkButton*)g_object_new(GTK_TYPE_BUTTON,
                              "label", GTK_STOCK_FILE,
                              "use-stock", TRUE,
                              "use-underline", TRUE,
                              NULL);
   */

   image_file = gtk_image_new_from_stock("gtk-file", GTK_ICON_SIZE_BUTTON);
   file_button = (GtkButton*)g_object_new(GTK_TYPE_BUTTON,
                              "label", "_File",
                              "image", image_file,
                              "use-underline", TRUE,
                              NULL);

   help_button = (GtkButton*)g_object_new(GTK_TYPE_BUTTON,
                              "label", GTK_STOCK_HELP,
                              "use-stock", TRUE,
                              "use-underline", TRUE,
                              NULL);

   about_button = (GtkButton*)g_object_new(GTK_TYPE_BUTTON,
                               "label", GTK_STOCK_ABOUT,
                               "use-stock", TRUE,
                               "use-underline", TRUE,
                               NULL);

   close_button = (GtkButton*)g_object_new(GTK_TYPE_BUTTON,
                               "label", GTK_STOCK_CLOSE,
                               "use-stock", TRUE,
                               "use-underline", TRUE,
                               NULL);


   /* Add display widgets to vbox */
   if ( image_logo != NULL )
      gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(image_logo));
   gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(label1));
   gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(label2));


   /* Add buttons to hbuttonbox */
   gtk_box_pack_start_defaults(GTK_BOX(hbuttonbox), GTK_WIDGET(file_button));
   gtk_box_pack_start_defaults(GTK_BOX(hbuttonbox), GTK_WIDGET(help_button));
   gtk_box_pack_start_defaults(GTK_BOX(hbuttonbox), GTK_WIDGET(about_button));
   gtk_box_pack_start_defaults(GTK_BOX(hbuttonbox), GTK_WIDGET(close_button));


   /* Add hbuttonbox to vbox and add vbox to window */
   gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(hbuttonbox));
   gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(vbox));


   /* Connect signal handlers */
   g_signal_connect(file_button,  "clicked", G_CALLBACK(FileHandler), NULL);
   g_signal_connect(help_button,  "clicked", G_CALLBACK(HelpHandler), NULL);
   g_signal_connect(about_button, "clicked", G_CALLBACK(AboutHandler), NULL);
   g_signal_connect(close_button, "clicked", G_CALLBACK(CloseHandler), NULL);
   g_signal_connect(window, "delete-event", G_CALLBACK(DeleteHandler), NULL);
   g_signal_connect(window, "destroy", G_CALLBACK(CloseHandler), NULL);
   g_signal_connect(window, "event", G_CALLBACK(CtrlCHandler), window);


   /* Show window */
   gtk_widget_show_all(GTK_WIDGET(window));


   /* Start application */
   gtk_main();

   return 0;
   }


void FileHandler ( void )
   {
   /* Declare variables */
   int result;
   GtkWidget *file_chooser;
   GtkFileFilter *filter1, *filter2;


   /* Create file-selection dialog */
   file_chooser = gtk_file_chooser_dialog_new("Choose plot-parameter file",
                                              NULL,
                                              GTK_FILE_CHOOSER_ACTION_OPEN,
                                              GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                              GTK_STOCK_OK, GTK_RESPONSE_OK,
                                              NULL);

   filter1 = gtk_file_filter_new();
   filter2 = gtk_file_filter_new();
   gtk_file_filter_set_name(filter1, "All Files");
   gtk_file_filter_set_name(filter2, "All Text Files");
   gtk_file_filter_add_pattern(filter1, "*.*");
   gtk_file_filter_add_pattern(filter2, "*.txt");
   gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_chooser), filter1);
   gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_chooser), filter2);
   gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser), g_get_home_dir());
   gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(file_chooser), FALSE);
   gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(file_chooser), filter2);


   /* Get result */
   while ( (result = gtk_dialog_run(GTK_DIALOG(file_chooser))) == GTK_RESPONSE_OK )
      FileOK(file_chooser);


   /* Destroy file-chooser dialog */
   gtk_widget_destroy(file_chooser);

   return;
   }


void FileOK ( GtkWidget *file_chooser )
   {
   /* Declare variables */
   int i;
   unsigned int size;
   char *filename, *argv[3];
   const char *error_str[] =
      { "Program executable, gsegraf, not in PATH." };


   /* Set pointers to NULL */
   filename = NULL;
   for ( i=1; i<=3; i++ )
      argv[i-1] = NULL;


   /* Get filename */
   filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));


   /* Run gsegraf program */
   if ( filename != NULL )
      {
      size = strlen("gsegraf");
      argv[0] = new char[size + 1];
      sprintf(argv[0], "%s", "gsegraf");
      size = strlen(filename);
      argv[1] = new char[size + 1];
      sprintf(argv[1], "%s", filename);
      argv[2] = NULL;
      if ( g_spawn_async(NULL,
                         argv,
                         NULL,
                         G_SPAWN_SEARCH_PATH,
                         NULL,
                         NULL,
                         NULL,
                         NULL) == 0 )
         {
         gtk_widget_destroy(GTK_WIDGET(file_chooser));
         JLP_ErrorDialog(error_str[0]);
         free(filename);
         if ( G_IS_OBJECT(pixbuf_logo) )
            g_object_unref(pixbuf_logo);
         for ( i=1; i<=3; i++ )
            free(argv[i-1]);
         exit(1);
         }
      }

   free(filename);
   for ( i=1; i<=3; i++ )
      free(argv[i-1]);

   return;
   }


gboolean DeleteHandler ( void )
   {
   return FALSE;
   }


void CloseHandler ( void )
   {
   gtk_main_quit();
   }


gboolean CtrlCHandler ( GtkWindow *window, GdkEvent *event )
   {
   int state, keyval;

   if ( event->type == GDK_KEY_PRESS )
      {
      state  = ((GdkEventKey*) event)->state;
      keyval = ((GdkEventKey*) event)->keyval;
      if ( state == 4 && keyval == 99 )
         gtk_widget_destroy(GTK_WIDGET(window));
      }

   return FALSE;
   }
