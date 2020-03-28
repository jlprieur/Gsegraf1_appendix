/*******************************************************************************
*
* gsegraf_prototypes.h
* declaration of the prototypes of the functions used by gsegraf 
*
* This file was modified from GSEGrafix (gsegrafix-1.0.6, sept. 2011)
* Copyright © 2008, 2009, 2010, 2011 Spencer A. Buckner
* http://savannah.gnu.org/projects/gsegrafix
*
* JLP
* Version 13/10/2016
*******************************************************************************/
#ifndef gsegraf_prototypes_h_
#define gsegraf_prototypes_h_

// Declaration of some structures used here
#include "jlp_gsegraf_defs.h"
 
/* Function prototypes */

/* CreateMenubar.c */
void CreateMenuBar();

/* Dialogs.c */
void     RequestDialog ( const char *prompt_str, const char *text_str, 
                        void (*callback) ( GtkEntry *entry_box) );
void     JLP_ErrorDialog ( const char *message_str );
void     MessageDialog ( const char *message_str );
void     QuestionDialog ( const char *question_str, void (*callback) );

/* EditMenu.c */
void     EditCopyHandler ( void );
void     GetWindowPixbuf ( void );

/* EventHandler.c */
gboolean EventHandler ( GtkWidget *window, GdkEvent *event );

/* FileMenu.c */
void     FileSaveAsHandler ( void );
void     FileOK ( GtkWidget *file_chooser );
void     FilePrintPreviewHandler ( void );
void     FilePrintHandler ( void );
void     PrintCalculate ( GnomePrintJob *job, GnomePrintContext *context, 
                          GdkPixbuf *pixbuf );
void     FileCloseHandler ( void );
void     FreeMemory ( void );

/* HelpMenu.c */
void     HelpHandler ( void );
void     HelpCallback ( GtkEntry *entry_box );
void     AboutHandler ( void );

/* ViewMenu.c */
void     ViewAxesHandler ( void );
void     ViewAxesCallback ( GtkEntry *entry_box );
void     ViewRotateHandler ( void );
void     ViewRotateCallback ( GtkEntry *entry_box );
void     ViewLabelHandler ( void );
void     ViewLabelCallback ( int inc );
void     ViewCoordsDisplayHandler ( void );
void     ViewCoordsDisplayCallback ( int inc );

#endif
