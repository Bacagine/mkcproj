/**
 * mkcproj.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *  
 * Description: Generator of new C projects
 * 
 * Copyright (C) 2023 Gustavo Bacagine
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Date: 04/10/2023
 */

#ifndef _MKCPROJ_H_
#define _MKCPROJ_H_

/******************************************************************************
 *                                                                            *
 *                                 Includes                                   *
 *                                                                            *
 ******************************************************************************/
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>
#include <getopt.h>
#include <locale.h>
#include <libintl.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>
#include "trace/trace.h"
#include "cutils/cutils.h"
#include "cutils/str.h"
#include "cutils/date_time.h"
#include "cutils/file.h"
#include "cutils/io.h"

/******************************************************************************
 *                                                                            *
 *                             Defines and macros                             *
 *                                                                            *
 ******************************************************************************/

/**
 * Program information defines
 */
#define VERSION        "1.0"
#define DESCRIPTION    "This software is a Generator of new C projects"
#define COPYRIGHT      "Copyright (C) 2023"
#define DEVELOPER      "Gustavo Bacagine"
#define DEV_MAIL       "gustavo.bacagine@protonmail.com"
#define CONF_FILE_NAME "./mkcproj.conf"
#define LOG_FILE_NAME  "./mkcproj.log"
#define GITHUB_URL     "https://www.github.com/Bacagine/mkcproj"

/**
 * Program language defines
 */
#define _(STR) gettext(STR)
#define LOCALE_DIR ""

/**
 * This macro is used to remove
 * unused warnings during the
 * compilation.
 */
#define UNUSED(X) (void) X

#ifdef __linux__
  #define HOME getenv("HOME")
#endif /* __linux__ */

/**
 * Default files created in a new C project
 */
#define HEADER_FILE           0x0000001
#define SOURCE_FILE           0x0000002
#define MAKEFILE_FILE         0x0000004
#define MK_FILE               0x0000008
#define MKALL_FILE            0x0000010
#define MKD_FILE              0x0000020
#define MKDALL_FILE           0x0000040
#define MKCLEAN_FILE          0x0000080
#define MKDISTCLEAN_FILE      0x0000100
#define MKINSTALL_FILE        0x0000200
#define MKUNINSTALL_FILE      0x0000400
#define MKSTRIP_FILE          0x0000800
#define INSTALL_FILE          0x0001000
#define INSTALL_SCRIPT_FILE   0x0002000
#define UNINSTALL_SCRIPT_FILE 0x0004000
#define AUTHORS_FILE          0x0008000
#define CHANGELOG_FILE        0x0010000
#define LICENSE_FILE          0x0020000
#define NEWS_FILE             0x0040000
#define README_FILE           0x0080000
#define MARKDOWN_README_FILE  0x0100000
#define TODO_FILE             0x0200000
#define AUTOCOMPLETE_FILE     0x0400000
#define CONF_FILE             0x0800000
#define MAN_FILE              0x1000000

/**
 * libcutils files
 */
#define CUTILS_COLOR_HEADER_FILE     0x002000000
#define CUTILS_CONSTS_HEADER_FILE    0x004000000
#define CUTILS_HEADER_FILE           0x008000000
#define CUTILS_DATE_TIME_HEADER_FILE 0x010000000
#define CUTILS_DIR_HEADER_FILE       0x020000000
#define CUTILS_FILE_HEADER_FILE      0x040000000
#define CUTILS_IO_HEADER_FILE        0x080000000
#define CUTILS_STR_HEADER_FILE       0x100000000
#define CUTILS_LIB_FILE              0x200000000

/**
 * libtrace files
 */
#define LOG_HEADER_FILE 0x0400000000
#define LOG_LIB_FILE    0x0800000000

/**
 * Default directories created in new C project
 */
#define PROJ_DIR 0x001
#define SRC_DIR  0x002
#define INC_DIR  0x004
#define DOC_DIR  0x008
#define MAN_DIR  0x010
#define LIB_DIR  0x020

#define PROJECTS_DIR "Projects"
#define TEMPLATE_DIR "template"

/******************************************************************************
 *                                                                            *
 *                  Typedefs, structures, unions and enums                    *
 *                                                                            *
 ******************************************************************************/


/******************************************************************************
 *                                                                            *
 *                     Global variables and constants                         *
 *                                                                            *
 ******************************************************************************/

/* Receive the name of program */
extern const char *gkpszProgramName;

/**
 * Set a verbose exit of the program, default is false
 */
extern bool gbVerbose;

/**
 * Example: /home/user/Templates/template
 */
extern char gszTemplatePathDir[2048];

/**
 * Example: /home/user/Projects
 */
extern char gszProjectsPathDir[2048];

/**
 * Example: /home/user/Projects/MyProj
 */
extern char gszFullNewProjectPathDir[2048+2048];

/**
 * Receive the full new file name PATH
 *
 * Example: /home/user/Projects/MyProj/src/MyProj.c
 */
extern char gszFullNewFileNamePath[2048+2048+2048];


/******************************************************************************
 *                                                                            *
 *                            Prototype functions                             *
 *                                                                            *
 ******************************************************************************/

/**
 * Print the help message for the user
 */
void vPrintUsage(void);

/**
 * Print the version of the software
 */
void vPrintVersion(void);

/**
 * Print a formatted error message
 */
void vPrintErrorMessage(const char *kpszFmt, ...);

/**
 * Check if what wass passed on the command line is valid
 */
bool bCommandLineIsOK(int argc, char **argv);

/**
 * A detailed and excessive sample of what was passed on the command line
 */
void vTraceCommandLine(int argc, char **argv);

/**
 * \brief A detailed and excessive sampling of the system's environment variables
 */
void vTraceEnvp(char **envp);

/**
 * Show the information about new C project in the log
 * file with INFO log level
 */
void vInfoShowProjectInformations(void);

/**
 * This function is based in the examples of the book 
 * "Aprenda em 24 horas Programacao para Linux"
 *
 * This function return the name of argv[0] withouth the path,
 * for example, if argv[0] == "/usr/bin/program", the function returns
 * only the string "program".
 */
char *szGetBaseName(const char *szPathName);

/**
 * Set default values in global variables
 */
int iInitMkcproj(void);

/**
 * Get the informations to create a new C project
 */
int iGetProjInfo(void);

/**
 *
 */
int iGetTemplateFileName(uint64_t ui64Flag, char *pszTemplateFileName);

/**
 *
 */
int iGetTemplateFullFileNamePath(uint64_t ui64Flag, char *pszFullTemplateFileNamePath);

/**
 *
 */
int iGetNewFileName(uint64_t ui64Flag, char *pszNewFileName);

/**
 * Get the full path name of file will be created and save in variabel
 * gszFullNewFileNamePath.
 *
 * Example: /home/user/Projects/MyProj/src/MyProj.c
 */
int iGetNewFullFileNamePath(uint64_t ui64Flag);

/**
 * Create a file of the new C project
 */
int iCreateFile(uint64_t ui64Flag);

/**
 * Create the direcotories of the new C project
 */
int iCreateDirectories(uint64_t ui64Flag);

/**
 * Header comment of files
 */
bool bCreateHeaderComment(uint64_t ui64Flag);

/**
 * Skip the template header comment during the copy
 * of files (.c, .h and Makefile)
 */
void vSkipTemplateHeaderComment(void);

/**
 * Create a new C project using the functions
 * above.
 */
int iMakeProject(void);

#endif /* _MKCPROJ_H_ */

