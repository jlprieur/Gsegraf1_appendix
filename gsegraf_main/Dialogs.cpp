/*******************************************************************************
*
* Dialogs.c
*
* Contains functions:
*    RequestDialog
*    JLP_ErrorDialog
*    MessageDialog
*    QuestionDialog
*
* General-purpose dialogs.
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

void RequestDialog ( const char *prompt_str, const char *text_str, void (*callback) (GtkEntry *entry_box) )
   {
   /* Declare variables */
   GtkWindow *dialog_request;
   GtkVBox *vbox, *hbox;
   GtkHButtonBox *hbuttonbox;
   GtkWidget *image;
   GtkLabel *label;
   GtkEntry *entry_box;
   GtkAccelGroup *accel_group;
   GtkButton *cancel_button, *ok_button;


   /* Create dialog box */
   dialog_request = (GtkWindow*)g_object_new(GTK_TYPE_WINDOW,
                                 "title", "Question",
                                 "window-position", GTK_WIN_POS_CENTER,
                                 "resizable", FALSE,
                                 "border-width", 12,
                                 NULL);


   /* Create packing boxes */
   vbox  = (GtkVBox*)g_object_new(GTK_TYPE_VBOX, "spacing", 18, NULL);
   hbox  = (GtkVBox*)g_object_new(GTK_TYPE_HBOX, "spacing", 18, NULL);
   hbuttonbox = (GtkHButtonBox*)g_object_new(GTK_TYPE_HBUTTON_BOX,
                             "layout-style", GTK_BUTTONBOX_END,
                             "spacing", 12,
                             NULL);


   /* Create image */
   image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_QUESTION,
                                    GTK_ICON_SIZE_DIALOG);


   /* Create label */
   label = (GtkLabel*)g_object_new(GTK_TYPE_LABEL,
                        "label", prompt_str,
                        NULL);


   /* Add image and label to hbox */
   gtk_box_pack_start_defaults(GTK_BOX(hbox), image);
   gtk_box_pack_start_defaults(GTK_BOX(hbox), GTK_WIDGET(label));


   /* Create entry box */
   entry_box = (GtkEntry*)g_object_new(GTK_TYPE_ENTRY,
                            "text", text_str,
                            NULL);


   /* Create buttons */
   cancel_button = (GtkButton*)g_object_new(GTK_TYPE_BUTTON,
                                "label", GTK_STOCK_CANCEL,
                                "use-stock", TRUE,
                                NULL);

   ok_button = (GtkButton*)g_object_new(GTK_TYPE_BUTTON,
                            "label", GTK_STOCK_OK,
                            "use-stock", TRUE,
                            NULL);


   /* Add buttons to hbuttonbox */
   gtk_box_pack_start_defaults(GTK_BOX(hbuttonbox), GTK_WIDGET(cancel_button));
   gtk_box_pack_start_defaults(GTK_BOX(hbuttonbox), GTK_WIDGET(ok_button));


   /* Add hbox, entry box and hbutton box to vbox; add vbox to window */
   gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(hbox));
   if ( text_str != NULL )
      gtk_box_pack_start_defaults(GTK_BOX(vbox), GTK_WIDGET(entry_box));
   gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(hbuttonbox));
   gtk_container_add(GTK_CONTAINER(dialog_request), GTK_WIDGET(vbox));


   /* Add accelerator key */
   accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(dialog_request, accel_group);


   /* Connect signal handlers */
   g_signal_connect(cancel_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
   g_signal_connect_swapped(cancel_button, "clicked", G_CALLBACK(gtk_widget_destroy), dialog_request);

   g_signal_connect_swapped(ok_button, "clicked", G_CALLBACK(callback), entry_box);
   g_signal_connect(ok_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
   g_signal_connect_swapped(ok_button, "clicked", G_CALLBACK(gtk_widget_destroy), dialog_request);

   g_signal_connect(dialog_request, "delete-event", G_CALLBACK(gtk_main_quit), NULL);


   /* Show dialog */
   gtk_widget_grab_focus(GTK_WIDGET(entry_box));
   gtk_widget_show_all(GTK_WIDGET(dialog_request));
   gtk_window_present(dialog_request);

   gtk_main();

   return;
   }


void JLP_ErrorDialog ( const char *message_str )
   {
   /* Declare variables */
   GtkWindow *dialog_error;
   GtkVBox *vbox, *hbox;
   GtkHButtonBox *hbuttonbox;
   GtkWidget *image;
   GtkLabel *label;
   GtkAccelGroup *accel_group;
   GtkButton *ok_button;


   /* Create dialog box */
   dialog_error = (GtkWindow*)g_object_new(GTK_TYPE_WINDOW,
                               "title", "Error",
                               "window-position", GTK_WIN_POS_CENTER,
                               "resizable", FALSE,
                               "border-width", 12,
                               NULL);


   /* Create packing boxes */
   vbox  = (GtkVBox*)g_object_new(GTK_TYPE_VBOX, "spacing", 18, NULL);
   hbox  = (GtkVBox*)g_object_new(GTK_TYPE_HBOX, "spacing", 18, NULL);
   hbuttonbox = (GtkHButtonBox*)g_object_new(GTK_TYPE_HBUTTON_BOX,
                             "layout-style", GTK_BUTTONBOX_END,
                             NULL);


   /* Create image */
   image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_ERROR,
                                    GTK_ICON_SIZE_DIALOG);


   /* Create label */
   label = (GtkLabel*)g_object_new(GTK_TYPE_LABEL,
                        "label", message_str,
                        NULL);


   /* Add image and label to hbox */
   gtk_box_pack_start_defaults(GTK_BOX(hbox), image);
   gtk_box_pack_start_defaults(GTK_BOX(hbox), GTK_WIDGET(label));


   /* Create buttons */
   ok_button = (GtkButton*)g_object_new(GTK_TYPE_BUTTON,
                            "label", GTK_STOCK_OK,
                            "use-stock", TRUE,
                            NULL);


   /* Add buttons to hbuttonbox */
   gtk_box_pack_end(GTK_BOX(hbuttonbox), GTK_WIDGET(ok_button), FALSE, FALSE, 0);


   /* Add hbox and hbutton box to vbox; add vbox to window */
   gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(hbox));
   gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(hbuttonbox));
   gtk_container_add(GTK_CONTAINER(dialog_error), GTK_WIDGET(vbox));


   /* Add accelerator key */
   accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(dialog_error, accel_group);


   /* Connect signal handlers */
   g_signal_connect(ok_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
   g_signal_connect_swapped(ok_button, "clicked", G_CALLBACK(gtk_widget_destroy), dialog_error);

   g_signal_connect(dialog_error, "delete-event", G_CALLBACK(gtk_main_quit), NULL);


   /* Show dialog */
   gtk_widget_grab_focus(GTK_WIDGET(ok_button));
   gtk_widget_show_all(GTK_WIDGET(dialog_error));
   gtk_window_present(dialog_error);

   gtk_main();

   return;
   }


void MessageDialog ( const char *message_str )
   {
   /* Declare variables */
   GtkWindow *dialog_message;
   GtkVBox *vbox, *hbox;
   GtkHButtonBox *hbuttonbox;
   GtkWidget *image;
   GtkLabel *label;
   GtkAccelGroup *accel_group;
   GtkButton *ok_button;


   /* Create dialog box */
   dialog_message = (GtkWindow*)g_object_new(GTK_TYPE_WINDOW,
                                 "title", "Message",
                                 "window-position", GTK_WIN_POS_CENTER,
                                 "resizable", FALSE,
                                 "border-width", 12,
                                 NULL);


   /* Create packing boxes */
   vbox  = (GtkVBox*)g_object_new(GTK_TYPE_VBOX, "spacing", 18, NULL);
   hbox  = (GtkVBox*)g_object_new(GTK_TYPE_HBOX, "spacing", 18, NULL);
   hbuttonbox = (GtkHButtonBox*)g_object_new(GTK_TYPE_HBUTTON_BOX,
                             "layout-style", GTK_BUTTONBOX_END,
                             NULL);


   /* Create image */
   image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO,
                                    GTK_ICON_SIZE_DIALOG);


   /* Create label */
   label = (GtkLabel*)g_object_new(GTK_TYPE_LABEL,
                        "label", message_str,
                        NULL);


   /* Add image and label to hbox */
   gtk_box_pack_start_defaults(GTK_BOX(hbox), image);
   gtk_box_pack_start_defaults(GTK_BOX(hbox), GTK_WIDGET(label));


   /* Create buttons */
   ok_button = (GtkButton*)g_object_new(GTK_TYPE_BUTTON,
                            "label", GTK_STOCK_OK,
                            "use-stock", TRUE,
                            NULL);


   /* Add buttons to hbuttonbox */
   gtk_box_pack_end(GTK_BOX(hbuttonbox), GTK_WIDGET(ok_button), FALSE, FALSE, 0);


   /* Add hbox and hbutton box to vbox; add vbox to window */
   gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(hbox));
   gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(hbuttonbox));
   gtk_container_add(GTK_CONTAINER(dialog_message), GTK_WIDGET(vbox));


   /* Add accelerator key */
   accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(dialog_message, accel_group);


   /* Connect signal handlers */
   g_signal_connect(ok_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
   g_signal_connect_swapped(ok_button, "clicked", G_CALLBACK(gtk_widget_destroy), dialog_message);

   g_signal_connect(dialog_message, "delete-event", G_CALLBACK(gtk_main_quit), NULL);


   /* Show dialog */
   gtk_widget_grab_focus(GTK_WIDGET(ok_button));
   gtk_widget_show_all(GTK_WIDGET(dialog_message));
   gtk_window_present(dialog_message);

   gtk_main();

   return;
   }


void QuestionDialog ( const char *question_str, void (*callback) )
   {
   /* Declare variables */
   GtkDialog *dialog_question;
   GtkHBox *hbox;
   GtkImage *image;
   GtkLabel *label;


   /* Create dialog */
   dialog_question = GTK_DIALOG(gtk_dialog_new_with_buttons("Question",
                                                            NULL,
                                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                                            "_Cancel", GTK_RESPONSE_CANCEL,
                                                            "_OK", GTK_RESPONSE_OK,
                                                            NULL));


   /* Create packing box */
   hbox = (GtkHBox*)g_object_new(GTK_TYPE_HBOX,
                       "border-width", 8,
                       NULL);


   /* Create image */
   image = (GtkImage*)g_object_new(GTK_TYPE_IMAGE,
                        "stock", GTK_STOCK_DIALOG_QUESTION,
                        "icon-size", GTK_ICON_SIZE_DIALOG,
                        NULL);


   /* Create label */
   label = (GtkLabel*)g_object_new(GTK_TYPE_LABEL,
                        "label", question_str,
                        NULL);


   /* Add image and label to hbox and add hbox to dialog */
   gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(image), FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(label), TRUE, TRUE, 0);
   gtk_box_pack_start(GTK_BOX(dialog_question->vbox), GTK_WIDGET(hbox), FALSE, FALSE, 0);


   /* Connect signal handler */
   g_signal_connect(dialog_question, "response", G_CALLBACK(callback), NULL);
   g_signal_connect(dialog_question, "response", G_CALLBACK(gtk_main_quit), NULL);
   g_signal_connect_swapped(dialog_question, "response", G_CALLBACK(gtk_widget_destroy), dialog_question);
   g_signal_connect(dialog_question, "delete-event", G_CALLBACK(gtk_main_quit), NULL);


   /* Show dialog */
   gtk_widget_show_all(GTK_WIDGET(dialog_question));
   gtk_main();

   return;
   }

