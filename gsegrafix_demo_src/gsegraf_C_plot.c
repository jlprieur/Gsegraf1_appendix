#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void gsegraf_C_plot ( char *fname, ... );

int main(int argc, char *argv[]){

gsegraf_C_plot("./sinc_param.txt",
                   "file_name     \"./sinc.txt\"",
                   "file_format   \"%lf %lf\"",
                   "plot_type     \"points\"",
                   "plot_style    lk",
                   "axis_type     \"linear\"",
                   "grid          ls",
                   "xlabel        \"x axis\"",
                   "ylabel        \"y axis\"",
                   "title         \"sin(x)/x\"",
                   NULL);
return;
}

void gsegraf_C_plot ( char *fname, ... )
   {
   /****************************************************************************
   *
   * Function writes a plot-parameter file, opens a temporary file for
   * communication with the plotting program, and then uses gsegraf to create
   * the plot. When the plot is completed, gsegraf writes the string "complete"
   * to the temporary file. The function then deletes the temporary file and
   * plot-parameter file.
   *
   * Input
   *    fname:           plot-parameter file-name (including path)
   *    keyword1   arg1: first keyword-argument pair
   *    keyword2   arg2: second keyword-argument pair
   *                               etc
   *
   * All inputs must be strings (enclosed in double quotes) and separated by
   * commas. If all or part of an argument needs to be quoted in the plot-
   * parameter file, the double-quote escape sequence (\") must be used for
   * these additional quotes. The list of keyword-argument pairs must be
   * terminated by a NULL pointer. An example of how to call gsegraf_C_plot is:
   *
   *    gsegraf_C_plot("./sinc_param.txt",
   *                   "file_name     \"./sinc.txt\"",
   *                   "file_format   \"%lf %lf\"",
   *                   "plot_type     \"points\"",
   *                   "plot_style    lk",
   *                   "axis_type     \"linear\"",
   *                   "grid          ls",
   *                   "xlabel        \"x axis\"",
   *                   "ylabel        \"y axis\"",
   *                   "title         \"sin(x)/x\"",
   *                   NULL);
   *
   ****************************************************************************/

   /* Declare variables */
   int fd;
   unsigned int size;
   char *str, *command, tempfile[] = "gsegraf_XXXXXX";
   va_list argp;
   FILE *fptr;

   /* Write plot-parameter file */
   fptr = fopen(fname, "w");
   va_start(argp, fname);
   while ( (str = va_arg(argp, char *)) != NULL )
      fprintf(fptr, "%s\n", str);
   fclose(fptr);
   va_end(argp);

   /* Plot data */
   fd = mkstemp(tempfile);
   close(fd);
   fptr = fopen(tempfile, "r");
   size = strlen(fname) + strlen(tempfile) + 12;
   command = malloc(size*sizeof(char));
   memset(command, 0, size*sizeof(char));
   sprintf(command, "%s %s %s %c", "gsegraf", fname, tempfile, '&');
   system(command);
   free(command);

   /* Wait for plot to be completed */
   while ( fgetc(fptr) == EOF )
      {
      system("sleep 0.1");
      rewind(fptr);
      }
   fclose(fptr);
   remove(tempfile);

   /* Delete plot-parameter file */
   remove(fname);

   return;
   }

