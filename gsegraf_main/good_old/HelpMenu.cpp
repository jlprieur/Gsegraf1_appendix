/*******************************************************************************
*
* HelpMenu.c
*
* Contains functions:
*    HelpHandler
*    HelpCallback
*    AboutHandler
*
* Functions for Help menu items.
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
//#include <gnome.h>
#include "gsegraf.h"

// External variable
extern char app_version[64];

void   RequestDialog ( const char *prompt_str, const char *text_str,
                       void (*callback) (GtkEntry *entry_box) );
void   HelpCallback ( GtkEntry *entry_box );
void   JLP_ErrorDialog ( const char *message_str );

void HelpHandler ( void )
   {
   /* Declare variables */
   char string[] = "1";
   const char prompt[] = "Enter 1 or 2 to choose help file:\n"
                         "   1. gsegrafix.xhtml (displayed in web browser)\n"
                         "   2. gsegrafix.xml (displayed in gnome help browser";


   /* Open dialog box */
   RequestDialog(prompt, string, HelpCallback);

   return;
   }


void HelpCallback ( GtkEntry *entry_box )
   {
   /* Declare variables */
   int i, flag_help;
   unsigned int size;
   char *string = NULL;
   char *filename, *url, *path_help, *argv_child[3];
   const char *error_str[] =
      { "Enter only 1 or 2 to choose help file.",
        "Cannot find help file, gsegrafix.xhtml.",
        "Cannot open help file with gnome help browser.",
        "Cannot find help file, gsegrafix.xml." };
   GnomeProgram *program;
   FILE *fptr;


   /* Get entry-box string */
   g_object_get(entry_box, "text", &string, NULL);
   if ( string != NULL )
      {
      if ( strncmp(string, "1", 1) == 0 )
         flag_help = 1;
      else if ( strncmp(string, "2", 1) == 0 )
         flag_help = 2;
      else
         {
         JLP_ErrorDialog(error_str[0]);
         free(string);
         return;
         }
      }
   else
      return;
   free(string);


   /* Get help filename and display help file */
   filename = NULL;
   url = NULL;
   path_help = NULL;
   fptr = NULL;
   program = gnome_program_get();

   if ( flag_help == 1 )
      {
      if ( (filename = gnome_program_locate_file(program,
                                                 GNOME_FILE_DOMAIN_APP_HELP,
                                                 "gsegrafix/C/gsegrafix.xhtml",
                                                 TRUE,
                                                 NULL)) !=  NULL )
         {
         size = strlen(filename) + 8;
         url = new char[size];
         memset(url, 0, size*sizeof(char));
         sprintf(url, "%s%s", "file://", filename);
         gnome_url_show(url, NULL);
         free(url);
         free(filename);
         }
      else if ( (fptr = fopen("./gsegrafix.xhtml", "r")) != NULL )
         {
         fclose(fptr);
         path_help = g_get_current_dir();
         size = strlen(path_help) + 24;
         url = new char[size];
         sprintf(url, "%s%s%s", "file://", path_help, "/gsegrafix.xhtml");
         gnome_url_show(url, NULL);
         delete[] url;
         free(path_help);
         }
      else
         {
         JLP_ErrorDialog(error_str[1]);
         return;
         }
      }

   else if ( flag_help == 2 )
      {
      if ( (filename = gnome_program_locate_file(program,
                                                 GNOME_FILE_DOMAIN_APP_HELP,
                                                 "gsegrafix/C/gsegrafix.xml",
                                                 TRUE,
                                                 NULL)) !=  NULL )
         {
         size = strlen("yelp");
         argv_child[0] = new char[size + 1];
         sprintf(argv_child[0], "%s", "yelp");
         size = strlen(filename) + 7;
         argv_child[1] = new char[size + 1];
         sprintf(argv_child[1], "%s%s", "file://", filename);
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
            JLP_ErrorDialog(error_str[2]);
            free(filename);
            for ( i=1; i<=3; i++ )
               free(argv_child[i-1]);
            return;
            }
         }
      else if ( (fptr = fopen("./gsegrafix.xml", "r")) != NULL )
         {
         fclose(fptr);
         size = strlen("yelp");
         argv_child[0] = new char[size + 1];
         sprintf(argv_child[0], "%s", "yelp");
         path_help = g_get_current_dir();
         size = strlen(path_help) + 21;
         argv_child[1] = new char[size + 1];
         sprintf(argv_child[1], "%s%s%s", "file://", path_help, "/gsegrafix.xml");
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
            JLP_ErrorDialog(error_str[2]);
            free(path_help);
            for ( i=1; i<=3; i++ )
               free(argv_child[i-1]);
            return;
            }
         }
      else
         {
         JLP_ErrorDialog(error_str[3]);
         return;
         }
      }

   return;


   /* Display help file (doesn't work; help file is xhtml rather than html) */
   /*
   GError *error = NULL;
   gnome_help_display("gsegrafix", NULL, &error);
   if ( error != NULL )
      {
      g_printerr("   \nCan't open URL: %s\n\n", error->message);
      g_error_free(error);
      }
   */
   }


void AboutHandler ( void )
   {
   /* Declare variables */
   const char *authors[] = { "S. A. Buckner <http://savannah.gnu.org/projects/gsegrafix>", NULL };
   const char *documenters[] = { "S. A. Buckner <http://savannah.gnu.org/projects/gsegrafix>", NULL };
   const char license[] =
      "This program is free software: you can redistribute it and/or modify\n"
      "it under the terms of the GNU General Public License as published by\n"
      "the Free Software Foundation, either version 3 of the License, or\n"
      "(at your option) any later version.\n"
      "\n"
      "This program is distributed in the hope that it will be useful,\n"
      "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
      "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
      "GNU General Public License for more details.\n"
      "\n"
      "You should have received a copy of the GNU General Public License\n"
      "along with this program.  If not, see <http://www.gnu.org/licenses/>.";


   /* Create dialog box */
   gtk_show_about_dialog(NULL,
                         "program-name", "GSEGrafix",
                         "version", app_version,
                         "comments", "GSEGrafix is a GNOME application for creating\n"
                                     "scientific and engineering plots.",
                         "copyright", "Copyright \xC2\xA9 2008, 2009, 2010, 2011 Spencer A. Buckner",
                         "website", "http://savannah.gnu.org/projects/gsegrafix",
                         "authors", authors,
                         "documenters", documenters,
                         "license", license,
                         "logo", pixbuf_logo,
                         NULL);

   return;
   }

