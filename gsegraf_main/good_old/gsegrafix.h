/*******************************************************************************
*
* gsegrafix.h
*
* Declares structures, external variables, and function prototypes for gsegrafix
* executable.
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
#include <gnome.h>

/* Formely external variable
GdkPixbuf *pixbuf_logo;

/* Function prototypes */
void     JLP_ErrorDialog   ( const char *message_str );

/* GSEGrafix.c */
void *   xmalloc       ( size_t size );
void     FileHandler   ( void );
void     FileOK        ( GtkWidget *file_chooser );
gboolean DeleteHandler ( void );
void     CloseHandler  ( void );
gboolean CtrlCHandler  ( GtkWindow *window, GdkEvent *event );

/* HelpMenu.c */
void     HelpHandler   ( void );
void     AboutHandler  ( void );

