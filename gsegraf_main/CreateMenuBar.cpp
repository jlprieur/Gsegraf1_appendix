/*******************************************************************************
*
* CreateMenuBar.c
*
* Create menu bar for plot window.
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

void CreateMenuBar ()
   {
   /* Declare variables */
   int flag_2d, flag_3d, flag_2d_rect, flag_polar, flag_linear, flag_logx;
   int flag_logy, flag_loglog, ncontours_plots, nplots;
   int icount, iplot;
   GnomeApp *window = p_window_data.window;
   GtkAccelGroup *accel_g;
   GtkMenu *file_menu, *edit_menu, *view_menu, *help_menu;
   GtkWidget *file, *edit, *view, *help,
             *save_as_menu_item, *print_preview_menu_item, 
             *print_menu_item, *close_menu_item, *copy_menu_item,
             *rotate_menu_item, *label_menu_item,
             *help_menu_item, *about_menu_item;

   /* Add accelerator group */
   accel_g = (GtkAccelGroup*)g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
   gtk_window_add_accel_group(GTK_WINDOW(window), accel_g);


   /* Create menu bar */
   menu_bar = (GtkMenuBar*)g_object_new(GTK_TYPE_MENU_BAR, NULL);


   /* Create menu items */
   file = gtk_menu_item_new_with_mnemonic("_File");
   edit = gtk_menu_item_new_with_mnemonic("_Edit");
   view = gtk_menu_item_new_with_mnemonic("_View");
   help = gtk_menu_item_new_with_mnemonic("_Help");
   gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);

   
   /* Create submenus */
   file_menu = (GtkMenu*)g_object_new(GTK_TYPE_MENU, NULL);
   edit_menu = (GtkMenu*)g_object_new(GTK_TYPE_MENU, NULL);
   view_menu = (GtkMenu*)g_object_new(GTK_TYPE_MENU, NULL);
   help_menu = (GtkMenu*)g_object_new(GTK_TYPE_MENU, NULL);
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), GTK_WIDGET(edit_menu));
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));


   /* Create file submenu items */
   save_as_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, accel_g);
   g_signal_connect(save_as_menu_item, "activate", G_CALLBACK(FileSaveAsHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_as_menu_item);

   print_preview_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_PRINT_PREVIEW, accel_g);
   g_signal_connect(print_preview_menu_item, "activate", G_CALLBACK(FilePrintPreviewHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), print_preview_menu_item);

   print_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_PRINT, accel_g);
   g_signal_connect(print_menu_item, "activate", G_CALLBACK(FilePrintHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), print_menu_item);

   close_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
   g_signal_connect(close_menu_item, "activate", G_CALLBACK(FileCloseHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), close_menu_item);


   /* Create copy submenu item */
   copy_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, NULL);
   g_signal_connect(copy_menu_item, "activate", G_CALLBACK(EditCopyHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), copy_menu_item);


   /* Create view submenu items */
   axes_menu_item = gtk_menu_item_new_with_mnemonic("_Axis Limits");
   g_signal_connect(axes_menu_item, "activate", G_CALLBACK(ViewAxesHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), axes_menu_item);

   rotate_menu_item = gtk_menu_item_new_with_mnemonic("_Rotate 3d Axes");
   g_signal_connect(rotate_menu_item, "activate", G_CALLBACK(ViewRotateHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), rotate_menu_item);

   label_menu_item = gtk_menu_item_new_with_mnemonic("_Label Contours");
   g_signal_connect(label_menu_item, "activate", G_CALLBACK(ViewLabelHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), label_menu_item);

   xy_coords_menu_item = gtk_menu_item_new_with_mnemonic("_Display Coordinates");
   g_signal_connect(xy_coords_menu_item, "activate", G_CALLBACK(ViewCoordsDisplayHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), xy_coords_menu_item);


   /* Create help submenu items */
   help_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, NULL);
   g_signal_connect(help_menu_item, "activate", G_CALLBACK(HelpHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_menu_item);

   about_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, accel_g);
   g_signal_connect(about_menu_item, "activate", G_CALLBACK(AboutHandler), NULL);
   gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about_menu_item);


   jlp_gsegraf0->GSEG_GetCurrentPlotSettings(&flag_2d, &flag_3d, &flag_2d_rect,
                                             &flag_polar, &flag_linear, 
                                             &flag_logx, &flag_logy, 
                                             &flag_loglog, &ncontours_plots, 
                                             &nplots);

   /* Set rotate menu-item sensitivity */
   gtk_widget_set_sensitive(rotate_menu_item, FALSE);
   if ( flag_3d == 1 )
      gtk_widget_set_sensitive(rotate_menu_item, TRUE);


   /* Set label menu-item sensitivity */
   gtk_widget_set_sensitive(label_menu_item, FALSE);
   icount = 0;
   if ( flag_linear == 1 )
      {
/** JLP2016 simplification
      for ( iplot=1; iplot<=nplots; iplot++ )
         if ( strcmp(&plot_types[(iplot-1)*10], "contour") == 0 )
            icount++;
      if ( icount >= 1 )
         gtk_widget_set_sensitive(label_menu_item, TRUE);
***/
    if(ncontours_plots > 0)
         gtk_widget_set_sensitive(label_menu_item, TRUE);
      }


   /* Set xy_coords menu-item sensitivity */
   gtk_widget_set_sensitive(xy_coords_menu_item, FALSE);
   if ( flag_2d == 1 )
      gtk_widget_set_sensitive(xy_coords_menu_item, TRUE);

   /* Add menu bar to window and show */
   gnome_app_set_menus(window, menu_bar);
   gtk_widget_show_all(GTK_WIDGET(menu_bar));

   return;
   }

