/**
 * mkcproj.c
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

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cutils/color.h"
#include "cmdline.h"
#include "mkcproj.h"

int opterr = 0;

char gszConfFileName[_MAX_PATH];
char gszLogFileName[_MAX_PATH];
DebugLevel giDebugLevel = 0;
bool gbColoredLogLevel = false;

bool gbVerbose = false;
char gszTemplatePathDir[2048];
char gszProjectsPathDir[2048];
char gszFullNewProjectPathDir[2048+2048];
char gszFullNewFileNamePath[2048+2048+2048];

const char *gkpszProgramName;
STRUCT_COMMAND_LINE gstCmdLine;

void vPrintErrorMessage(const char *kpszFmt, ...)
{
  va_list args;
  char szMsg[_MAX_PATH];
  
  memset(szMsg, 0, sizeof(szMsg));
   
  va_start(args, kpszFmt);
  
  /**
   * Check if the terminal suport colors
   */
  if(bTerminalSupportColors() == false)
  {
    snprintf(szMsg, sizeof(szMsg), _("E: %s\n"), kpszFmt);

    vfprintf(stderr, szMsg, args);
    return;
  }
  
  snprintf(szMsg, sizeof(szMsg), _("\033[1;31mE:\033[m %s\n"), kpszFmt);
  
  vfprintf(stderr, szMsg, args);

  va_end(args);
}

int iInitMkcproj(void)
{
  snprintf(gszTemplatePathDir, sizeof(gszTemplatePathDir), "%s/Template/%s", HOME,
                                                                             TEMPLATE_DIR);
  snprintf(gszProjectsPathDir, sizeof(gszProjectsPathDir), "%s/%s", HOME,
                                                                    PROJECTS_DIR);
  return 0;
}

int iGetProjInfo(void)
{
  char szAnswer[2048];

  memset(szAnswer, 0, sizeof(szAnswer));

  if(INFO_DETAILS) vTraceInfo(_("%s - begin"), __func__);
  
  do
  {
    printf(_("Developer name: "));
    vFgets(szAnswer, sizeof(szAnswer), stdin);

    if(bStrIsEmpty(szAnswer))
    {
      vPrintErrorMessage(_("You don't type a name!"));
      puts(_("Please, type your name below"));

      memset(szAnswer, 0, sizeof(szAnswer));
    }
  } while(bStrIsEmpty(szAnswer));
  
  strcpy(gstCmdLine.szDevName, szAnswer);

  memset(szAnswer, 0, sizeof(szAnswer));

  do
  {
    printf(_("Developer e-mail: "));
    vFgets(szAnswer, sizeof(szAnswer), stdin);

    if(bStrIsEmpty(szAnswer))
    {
      vPrintErrorMessage(_("You don't type your e-mail!"));
      puts(_("Please, type your e-mail below"));

      memset(szAnswer, 0, sizeof(szAnswer));
    }
  } while(bStrIsEmpty(szAnswer));
  
  strcpy(gstCmdLine.szDevMail, szAnswer);
  
  memset(szAnswer, 0, sizeof(szAnswer));
  
  printf(_("Template directory (default %s): "), gszTemplatePathDir);
  vFgets(szAnswer, sizeof(szAnswer), stdin);
  
  if(!bStrIsEmpty(szAnswer))
  {
    snprintf(gszTemplatePathDir, sizeof(gszTemplatePathDir), "%s", szAnswer);
  }

  memset(szAnswer, 0, sizeof(szAnswer));

  printf(_("Projects directory (default %s): "), gszProjectsPathDir);
  vFgets(szAnswer, sizeof(szAnswer), stdin);

  if(!bStrIsEmpty(szAnswer))
  {
    snprintf(gszProjectsPathDir, sizeof(gszProjectsPathDir), "%s", szAnswer);
  }

  do
  {
    printf(_("Project name: "));
    vFgets(szAnswer, sizeof(szAnswer), stdin);

    if(bStrIsEmpty(szAnswer))
    {
      vPrintErrorMessage(_("You don't type the name of your project!"));
      puts(_("Please, type the name of the project below"));

      memset(szAnswer, 0, sizeof(szAnswer));
    }
  } while(bStrIsEmpty(szAnswer));
  
  strcpy(gstCmdLine.szProjName, szAnswer);

  memset(szAnswer, 0, sizeof(szAnswer));

  do
  {
    printf(_("Description: "));
    vFgets(szAnswer, sizeof(szAnswer), stdin);

    if(bStrIsEmpty(szAnswer))
    {
      vPrintErrorMessage(_("You don't type the description of your software!"));
      puts(_("Please, type the description below"));
      
      memset(szAnswer, 0, sizeof(szAnswer));
    }
  } while(bStrIsEmpty(szAnswer));

  strcpy(gstCmdLine.szProjDescription, szAnswer);

  memset(szAnswer, 0, sizeof(szAnswer));

  printf(_("License (Default is GPLv2): "));
  vFgets(szAnswer, sizeof(szAnswer), stdin);

  bStrIsEmpty(szAnswer) ? strcpy(gstCmdLine.szLicense, "GPLv2") : 
                          strcpy(gstCmdLine.szLicense, szAnswer);

  if(INFO_DETAILS) vTraceInfo(_("%s - end"), __func__);

  return 0;
}

int iGetTemplateFileName(uint64_t ui64Flag, char *pszTemplateFileName)
{
  if(ui64Flag & HEADER_FILE)
  {
    sprintf(pszTemplateFileName, "template.h");
  }

  if(ui64Flag & SOURCE_FILE)
  {
    sprintf(pszTemplateFileName, "template.c");
  }

  if(ui64Flag & MAKEFILE_FILE)
  {
    sprintf(pszTemplateFileName, "Makefile");
  }

  if(ui64Flag & MK_FILE)
  {
    sprintf(pszTemplateFileName, "mk");
  }

  if(ui64Flag & MKALL_FILE)
  {
    sprintf(pszTemplateFileName, "mkall");
  }

  if(ui64Flag & MKD_FILE)
  {
    sprintf(pszTemplateFileName, "mkd");
  }

  if(ui64Flag & MKDALL_FILE)
  {
    sprintf(pszTemplateFileName, "mkdall");
  }

  if(ui64Flag & MKCLEAN_FILE)
  {
    sprintf(pszTemplateFileName, "mkclean");
  }

  if(ui64Flag & MKDISTCLEAN_FILE)
  {
    sprintf(pszTemplateFileName, "mkdistclean");
  }

  if(ui64Flag & MKINSTALL_FILE)
  {
    sprintf(pszTemplateFileName, "mkinstall");
  }

  if(ui64Flag & MKUNINSTALL_FILE)
  {
    sprintf(pszTemplateFileName, "mkuninstall");
  }

  if(ui64Flag & MKSTRIP_FILE)
  {
    sprintf(pszTemplateFileName, "mkstrip");
  }

  if(ui64Flag & INSTALL_FILE)
  {
    sprintf(pszTemplateFileName, "INSTALL");
  }

  if(ui64Flag & INSTALL_SCRIPT_FILE)
  {
    sprintf(pszTemplateFileName, "install.sh");
  }

  if(ui64Flag & UNINSTALL_SCRIPT_FILE)
  {
    sprintf(pszTemplateFileName, "uninstall.sh");
  }

  if(ui64Flag & AUTHORS_FILE)
  {
    sprintf(pszTemplateFileName, "AUTHORS");
  }

  if(ui64Flag & CHANGELOG_FILE)
  {
    sprintf(pszTemplateFileName, "ChangeLog");
  }

  if(ui64Flag & LICENSE_FILE)
  {
    sprintf(pszTemplateFileName, "COPYRIGHT");
  }

  if(ui64Flag & NEWS_FILE)
  {
    sprintf(pszTemplateFileName, "NEWS");
  }

  if(ui64Flag & README_FILE)
  {
    sprintf(pszTemplateFileName, "README_FILE");
  }

  if(ui64Flag & MARKDOWN_README_FILE)
  {
    sprintf(pszTemplateFileName, "README.md");
  }

  if(ui64Flag & TODO_FILE)
  {
    sprintf(pszTemplateFileName, "TODO");
  }

  if(ui64Flag & AUTOCOMPLETE_FILE)
  {
    sprintf(pszTemplateFileName, "_template_complete.sh");
  }

  if(ui64Flag & CONF_FILE)
  {
    sprintf(pszTemplateFileName, "template.conf");
  }

  if(ui64Flag & MAN_FILE)
  {
    sprintf(pszTemplateFileName, "template.1");
  }

  return 0;
}

int iGetFullTemplateFileNamePath(uint64_t ui64Flag, char *pszFullTemplateFileNamePath)
{
  if(ui64Flag & HEADER_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/include/template.h", gszTemplatePathDir);
  }

  if(ui64Flag & SOURCE_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/src/template.c",gszTemplatePathDir);
  }

  if(ui64Flag & MAKEFILE_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/Makefile", gszTemplatePathDir);
  }

  if(ui64Flag & MK_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/mk", gszTemplatePathDir);
  }

  if(ui64Flag & MKALL_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/mkall", gszTemplatePathDir);
  }

  if(ui64Flag & MKD_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/mkd", gszTemplatePathDir);
  }

  if(ui64Flag & MKDALL_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/mkdall", gszTemplatePathDir);
  }

  if(ui64Flag & MKCLEAN_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/mkclean", gszTemplatePathDir);
  }

  if(ui64Flag & MKDISTCLEAN_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/mkdistclean", gszTemplatePathDir);
  }

  if(ui64Flag & MKINSTALL_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/mkinstall", gszTemplatePathDir);
  }

  if(ui64Flag & MKUNINSTALL_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/mkuninstall", gszTemplatePathDir);
  }

  if(ui64Flag & MKSTRIP_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/mkstrip", gszTemplatePathDir);
  }

  if(ui64Flag & INSTALL_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/INSTALL", gszTemplatePathDir);
  }

  if(ui64Flag & INSTALL_SCRIPT_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/install.sh", gszTemplatePathDir);
  }

  if(ui64Flag & UNINSTALL_SCRIPT_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/uninstall.sh", gszTemplatePathDir);
  }

  if(ui64Flag & AUTHORS_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/AUTHORS", gszTemplatePathDir);
  }

  if(ui64Flag & CHANGELOG_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/ChangeLog", gszTemplatePathDir);
  }

  if(ui64Flag & LICENSE_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/COPYRIGHT", gszTemplatePathDir);
  }

  if(ui64Flag & NEWS_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/NEWS", gszTemplatePathDir);
  }

  if(ui64Flag & README_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/README_FILE", gszTemplatePathDir);
  }

  if(ui64Flag & MARKDOWN_README_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/README.md", gszTemplatePathDir);
  }

  if(ui64Flag & TODO_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/TODO", gszTemplatePathDir);
  }

  if(ui64Flag & AUTOCOMPLETE_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/_template_complete.sh", gszTemplatePathDir);
  }

  if(ui64Flag & CONF_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/template.conf", gszTemplatePathDir);
  }

  if(ui64Flag & MAN_FILE)
  {
    sprintf(pszFullTemplateFileNamePath, "%s/man/template.1", gszTemplatePathDir);
  }

  return 0;
}

int iGetNewFileName(uint64_t ui64Flag, char *pszNewFileName)
{
  if(ui64Flag & HEADER_FILE)
  {
    sprintf(pszNewFileName, "%s.h", gstCmdLine.szProjName);
  }

  if(ui64Flag & SOURCE_FILE)
  {
    sprintf(pszNewFileName, "%s.c", gstCmdLine.szProjName);
  }

  if(ui64Flag & MAKEFILE_FILE)
  {
    sprintf(pszNewFileName, "Makefile");
  }

  if(ui64Flag & MK_FILE)
  {
    sprintf(pszNewFileName, "mk");
  }

  if(ui64Flag & MKALL_FILE)
  {
    sprintf(pszNewFileName, "mkall");
  }

  if(ui64Flag & MKD_FILE)
  {
    sprintf(pszNewFileName, "mkd");
  }

  if(ui64Flag & MKDALL_FILE)
  {
    sprintf(pszNewFileName, "mkdall");
  }

  if(ui64Flag & MKCLEAN_FILE)
  {
    sprintf(pszNewFileName, "mkclean");
  }

  if(ui64Flag & MKDISTCLEAN_FILE)
  {
    sprintf(pszNewFileName, "mkdistclean");
  }

  if(ui64Flag & MKINSTALL_FILE)
  {
    sprintf(pszNewFileName, "mkinstall");
  }

  if(ui64Flag & MKUNINSTALL_FILE)
  {
    sprintf(pszNewFileName, "mkuninstall");
  }

  if(ui64Flag & MKSTRIP_FILE)
  {
    sprintf(pszNewFileName, "mkstrip");
  }

  if(ui64Flag & INSTALL_FILE)
  {
    sprintf(pszNewFileName, "INSTALL");
  }

  if(ui64Flag & INSTALL_SCRIPT_FILE)
  {
    sprintf(pszNewFileName, "install.sh");
  }

  if(ui64Flag & UNINSTALL_SCRIPT_FILE)
  {
    sprintf(pszNewFileName, "uninstall.sh");
  }

  if(ui64Flag & AUTHORS_FILE)
  {
    sprintf(pszNewFileName, "AUTHORS");
  }

  if(ui64Flag & CHANGELOG_FILE)
  {
    sprintf(pszNewFileName, "ChangeLog");
  }

  if(ui64Flag & LICENSE_FILE)
  {
    sprintf(pszNewFileName, "COPYRIGHT");
  }

  if(ui64Flag & NEWS_FILE)
  {
    sprintf(pszNewFileName, "NEWS");
  }

  if(ui64Flag & README_FILE)
  {
    sprintf(pszNewFileName, "README_FILE");
  }

  if(ui64Flag & MARKDOWN_README_FILE)
  {
    sprintf(pszNewFileName, "README.md");
  }

  if(ui64Flag & TODO_FILE)
  {
    sprintf(pszNewFileName, "TODO");
  }

  if(ui64Flag & AUTOCOMPLETE_FILE)
  {
    sprintf(pszNewFileName, "_%s_complete.sh", gstCmdLine.szProjName);
  }

  if(ui64Flag & CONF_FILE)
  {
    sprintf(pszNewFileName, "%s.conf", gstCmdLine.szProjName);
  }

  if(ui64Flag & MAN_FILE)
  {
    sprintf(pszNewFileName, "%s.1", gstCmdLine.szProjName);
  }

  return 0;
}

int iGetFullNewFileNamePath(uint64_t ui64Flag)
{
  if(ui64Flag & HEADER_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/include/%s.h", gszFullNewProjectPathDir,
                                                       gstCmdLine.szProjName);
  }

  if(ui64Flag & SOURCE_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/src/%s.c", gszFullNewProjectPathDir,
                                                   gstCmdLine.szProjName);
  }

  if(ui64Flag & MAKEFILE_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/Makefile", gszFullNewProjectPathDir);
  }

  if(ui64Flag & MK_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/mk", gszFullNewProjectPathDir);
  }

  if(ui64Flag & MKALL_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/mkall", gszFullNewProjectPathDir);
  }

  if(ui64Flag & MKD_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/mkd", gszFullNewProjectPathDir);
  }

  if(ui64Flag & MKDALL_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/mkdall", gszFullNewProjectPathDir);
  }

  if(ui64Flag & MKCLEAN_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/mkclean", gszFullNewProjectPathDir);
  }

  if(ui64Flag & MKDISTCLEAN_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/mkdistclean", gszFullNewProjectPathDir);
  }

  if(ui64Flag & MKINSTALL_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/mkinstall", gszFullNewProjectPathDir);
  }

  if(ui64Flag & MKUNINSTALL_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/mkuninstall", gszFullNewProjectPathDir);
  }

  if(ui64Flag & MKSTRIP_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/mkstrip", gszFullNewProjectPathDir);
  }

  if(ui64Flag & INSTALL_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/INSTALL", gszFullNewProjectPathDir);
  }

  if(ui64Flag & INSTALL_SCRIPT_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/install.sh", gszFullNewProjectPathDir);
  }

  if(ui64Flag & UNINSTALL_SCRIPT_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/uninstall.sh", gszFullNewProjectPathDir);
  }

  if(ui64Flag & AUTHORS_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/AUTHORS", gszFullNewProjectPathDir);
  }

  if(ui64Flag & CHANGELOG_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/ChangeLog", gszFullNewProjectPathDir);
  }

  if(ui64Flag & LICENSE_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/COPYRIGHT", gszFullNewProjectPathDir);
  }

  if(ui64Flag & NEWS_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/NEWS", gszFullNewProjectPathDir);
  }

  if(ui64Flag & README_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/README_FILE", gszFullNewProjectPathDir);
  }

  if(ui64Flag & MARKDOWN_README_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/README.md", gszFullNewProjectPathDir);
  }

  if(ui64Flag & TODO_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/TODO", gszFullNewProjectPathDir);
  }

  if(ui64Flag & AUTOCOMPLETE_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/_%s_complete.sh", gszFullNewProjectPathDir,
                                                          gstCmdLine.szProjName);
  }

  if(ui64Flag & CONF_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/%s.conf", gszFullNewProjectPathDir,
                                                  gstCmdLine.szProjName);
  }

  if(ui64Flag & MAN_FILE)
  {
    sprintf(gszFullNewFileNamePath, "%s/man/%s.1", gszFullNewProjectPathDir,
                                                   gstCmdLine.szProjName);
  }

  return 0;
}

/**
 * TODO: Implement it.
 */
int iCreateFile(uint64_t ui64Flag)
{
  FILE *fpFile = NULL;
  uint64_t u64FileType = 0;
  bool bFileType = false;
  char szTemplateFileName[_MAX_PATH];
  char szNewFileName[_MAX_PATH];
  char szFullTemplateFileNamePath[_MAX_PATH];
  
  UNUSED(fpFile);
  UNUSED(u64FileType);
  UNUSED(bFileType);

  memset(szNewFileName, 0, sizeof(szNewFileName));

  if(INFO_DETAILS) vTraceInfo(_("%s - begin"), __func__);
  
  iGetTemplateFileName(ui64Flag, szTemplateFileName);
  puts(szTemplateFileName);

  iGetFullTemplateFileNamePath(ui64Flag, szFullTemplateFileNamePath);
  puts(szFullTemplateFileNamePath);

  iGetNewFileName(ui64Flag, szNewFileName);
  puts(szNewFileName);
  
  iGetFullNewFileNamePath(ui64Flag);
  puts(gszFullNewFileNamePath);

  if(INFO_DETAILS) vTraceInfo(_("%s - end"), __func__);
  
  return 0;
}

/**
 * TODO: Implement it.
 */
int iCreateDirectories(uint64_t ui64Flag)
{
  bool bDirType = false;
  uint64_t u64DirType = 0;
  
  UNUSED(u64DirType);

  if(INFO_DETAILS) vTraceInfo(_("%s - begin"), __func__);
  
  if(ui64Flag & PROJ_DIR)
  {
    puts("PROJECTS_DIR");
  }

  if(ui64Flag & SRC_DIR)
  {
    puts("SRC_DIR");
  }

  if(ui64Flag & INC_DIR)
  {
    puts("INC_DIR");
  }

  if(ui64Flag & DOC_DIR)
  {
    puts("DOC_DIR");
  }
  
  if(ui64Flag & MAN_DIR)
  {
    puts("MAN_DIR");
  }

  if(ui64Flag & LIB_DIR)
  {
    puts("LIB_DIR");
  }
  
  if(bDirType == false)
  {
    if(INFO_DETAILS) vTraceInfo(_("Invalid directory type!"));
    
    return false;
  }
  
  if(INFO_DETAILS) vTraceInfo(_("%s - end"), __func__);
  
  return 0;
}

/**
 * NOTE: This is a prototype
 * TODO: Concat the PATH of file.
 */
bool bCreateHeaderComment(uint64_t ui64Flag)
{
  PSTRUCT_DATE pstDate = (PSTRUCT_DATE) malloc(sizeof(STRUCT_DATE));
  FILE *fpFile = NULL;
  bool bFileType = false;
  uint64_t u64FileType = 0;
  char szFileName[_MAX_PATH];

  memset(szFileName, 0, sizeof(szFileName));

  if(INFO_DETAILS) vTraceInfo(_("%s - begin"), __func__);

  if(ui64Flag & HEADER_FILE)
  {
    u64FileType = HEADER_FILE;
    strcat(szFileName, gstCmdLine.szProjName);
    strcat(szFileName, ".h");
  }

  if(ui64Flag & SOURCE_FILE)
  {
    u64FileType = SOURCE_FILE;
    strcat(szFileName, gstCmdLine.szProjName);
    strcat(szFileName, ".c");
  }

  if(ui64Flag & MAKEFILE_FILE)
  {
    u64FileType = MAKEFILE_FILE;
    snprintf(szFileName, sizeof(szFileName), "Makefile");
  }
  
  if(bFileType == false)
  {
    if(INFO_DETAILS) vTraceInfo(_("Invalid file type!"));
    
    free(pstDate);
    pstDate = NULL;

    return false;
  }
  
  if(INFO_DETAILS) vTraceInfo(_("Create Header Comment"));
  
  if(!bOpenFile(&fpFile, szFileName, "rw"))
  {
    vPrintErrorMessage(_("Impossible open the file %s"), szFileName);
    
    if(DEBUG_DETAILS) vTraceFatal(_("Impossible open the file %s"), szFileName);

    free(pstDate);
    pstDate = NULL;

    exit(EXIT_FAILURE);
  }
  
  vGetCurrentDate(&pstDate);

  if(u64FileType != MAKEFILE_FILE)
  {
    fprintf(fpFile,
        "/** \n"
        " * %s.%c\n" /* Filename and extension */
        " * \n"
        " * Description: %s\n"
        " * \n"
        " * Copyright (C) %d %s\n" /* Year and developer name */
        "%s"                       /* License text of the software */
        " * \n"
        " * Date: %02d/%02d/%04d\n" /* dd/mm/yyyy */
        " */", gstCmdLine.szProjName, u64FileType == HEADER_FILE ? 'h' : 'c',
               gstCmdLine.szProjDescription, pstDate->iYear, gstCmdLine.szDevName,
               bStrIsEmpty(gstCmdLine.szLicense) ? " * \n" : gstCmdLine.szLicense,
               pstDate->iDay, pstDate->iMonth, pstDate->iYear
    );
  }
  else
  {
    fprintf(fpFile,
        "# \n"
        "# %s.%c\n" /* Filename and extension */
        "# \n"
        "# Description: %s\n"
        "# \n"
        "# Copyright (C) %d %s\n" /* Year and developer name */
        "# %s"                       /* License text of the software */
        "# \n"
        "# Date: %02d/%02d/%04d\n" /* dd/mm/yyyy */
        "# ", gstCmdLine.szProjName, u64FileType == HEADER_FILE ? 'h' : 'c',
              gstCmdLine.szProjDescription, pstDate->iYear, gstCmdLine.szDevName,
              bStrIsEmpty(gstCmdLine.szLicense) ? " * \n" : gstCmdLine.szLicense,
              pstDate->iDay, pstDate->iMonth, pstDate->iYear
    );
  }
  
  if(!bCloseFile(&fpFile))
  {
    vPrintErrorMessage(_("Impossible close the file %s"), szFileName);
    
    if(DEBUG_DETAILS) vTraceFatal(_("Impossible close the file %s"), szFileName);

    free(pstDate);
    pstDate = NULL;

    exit(EXIT_FAILURE);
  }

  if(INFO_DETAILS) vTraceInfo(_("%s - end"), __func__);

  free(pstDate);
  pstDate = NULL;

  return true;
}

/**
 * TODO: Implement it.
 */
void vSkipTemplateHeaderComment(void)
{
  return;
}

/**
 * TODO: Implement it.
 */
int iMakeProject(void)
{
  /**
   * Creating the directories
   */
  if(iCreateDirectories(PROJ_DIR) != 0)
  {
    return -1;
  }

  if(iCreateDirectories(SRC_DIR) != 0)
  {
    return -2;
  }

  if(iCreateDirectories(INC_DIR) != 0)
  {
    return -3;
  }

  if(iCreateDirectories(LIB_DIR) != 0)
  {
    return -4;
  }
  
  if(iCreateDirectories(MAN_DIR) != 0)
  {
    return -5;
  }

  if(iCreateDirectories(DOC_DIR) != 0)
  {
    return -6;
  }
  
  /**
   * Creating the files
   */
  if(iCreateFile(HEADER_FILE) != 0)
  {
    return -7;
  }

  if(iCreateFile(SOURCE_FILE) != 0)
  {
    return -8;
  }

  if(iCreateFile(MAKEFILE_FILE) != 0)
  {
    return -9;
  }

  if(iCreateFile(MK_FILE) != 0)
  {
    return -10;
  }

  if(iCreateFile(MKALL_FILE) != 0)
  {
    return -11;
  }

  if(iCreateFile(MKD_FILE) != 0)
  {
    return -12;
  }

  if(iCreateFile(MKDALL_FILE) != 0)
  {
    return -13;
  }

  if(iCreateFile(MKCLEAN_FILE) != 0)
  {
    return -14;
  }

  if(iCreateFile(MKDISTCLEAN_FILE) != 0)
  {
    return -15;
  }

  if(iCreateFile(MKINSTALL_FILE) != 0)
  {
    return -16;
  }

  if(iCreateFile(MKUNINSTALL_FILE) != 0)
  {
    return -17;
  }

  if(iCreateFile(MKSTRIP_FILE) != 0)
  {
    return -18;
  }

  if(iCreateFile(INSTALL_FILE) != 0)
  {
    return -19;
  }

  if(iCreateFile(INSTALL_SCRIPT_FILE) != 0)
  {
    return -20;
  }

  if(iCreateFile(UNINSTALL_SCRIPT_FILE) != 0)
  {
    return -21;
  }

  if(iCreateFile(AUTHORS_FILE) != 0)
  {
    return -22;
  }

  if(iCreateFile(CHANGELOG_FILE) != 0)
  {
    return -23;
  }

  if(iCreateFile(LICENSE_FILE) != 0)
  {
    return -24;
  }

  if(iCreateFile(NEWS_FILE) != 0)
  {
    return -25;
  }

  if(iCreateFile(README_FILE) != 0)
  {
    return -26;
  }

  if(iCreateFile(MARKDOWN_README_FILE) != 0)
  {
    return -27;
  }

  if(iCreateFile(TODO_FILE) != 0)
  {
    return -28;
  }
  
  if(iCreateFile(AUTOCOMPLETE_FILE) != 0)
  {
    return -29;
  }

  if(iCreateFile(CONF_FILE) != 0)
  {
    return -30;
  }

  if(iCreateFile(MAN_FILE) != 0)
  {
    return -31;
  }

  return 0;
}

/******************************************************************************
 *                                                                            *
 *                                   main                                     *
 *                                                                            *
 ******************************************************************************/
#ifdef __linux__
int main(int argc, char **argv, char **envp)
#else
int main(int argc, char **argv)
#endif /* __linux__ */
{
  int iRsl = 0;
  
  memset(&gstCmdLine, 0, sizeof(gstCmdLine));

  /* Setting the name of program */
  gkpszProgramName = szGetProgramName(argv[0]);
  
  UNUSED(kszLogLevelColorInit);
  UNUSED(kszLogLevelColorEnd);
  UNUSED(kszLogLevel);
  UNUSED(kszBoldColors);
  UNUSED(kszNormalColors);
  
  setlocale(LC_ALL, NULL);
  
  bindtextdomain(gkpszProgramName, LOCALE_DIR);
  textdomain(gkpszProgramName);
  
  /* Checking and validate the command line arguments */
  if(!bCommandLineIsOK(argc, argv))
  {
    vPrintErrorMessage("Invalid option!");
    printf("Please, see %s --help\n", gkpszProgramName);

    exit(EXIT_FAILURE);
  }
  
  /* .conf file  */
  if(!bStrIsEmpty(gstCmdLine.szConfFileName))
  {
    vSetConfFileName(gstCmdLine.szConfFileName);
  }
  else
  {
    vSetConfFileName(CONF_FILE_NAME);
  }

  /* DebugLevel configurations */
  if(!bStrIsEmpty(gstCmdLine.szDebugLevel)) 
  {
    vSetLogLevel(atoi(gstCmdLine.szDebugLevel));
  }
  else 
  {
    vSetLogLevel(iGetLogLevel());
  }

  if(giDebugLevel < 0)
  {
    vPrintErrorMessage(_("Debug Level it's less than 0!"));

    exit(EXIT_FAILURE);
  }

  if(!bStrIsEmpty(gstCmdLine.szLogFileName))
  {
    vSetLogFileName(gstCmdLine.szLogFileName);
  }
  else
  {
    vSetLogFileName(LOG_FILE_NAME);
  }

  if(INFO_DETAILS)
  {
    vTraceInfo(_("%s - begin"), __func__);
  }

  if(TRACE_DETAILS)
  {
    vTraceCommandLine(argc, argv);
    vTraceProgramInfo();
    vTraceSystemInfo();
    vTraceEnvp(envp);
  }
  
  iInitMkcproj();

  if(argc == 1)
  {
    if((iRsl = iGetProjInfo()) != 0)
    {
      vPrintErrorMessage(_("Impossible get the information about the project!"));
      
      if(FATAL_DETAILS)
      {
        vTraceFatal("%s - end iRsl == %d", __func__, iRsl);
      }

      exit(EXIT_FAILURE);
    }

    if((iRsl = iMakeProject()) != 0)
    {
      vPrintErrorMessage(_("Impossible create the project!"));
      
      if(FATAL_DETAILS)
      {
        vTraceFatal("%s - end iRsl == %d", __func__, iRsl);
      }

      exit(EXIT_FAILURE);
    }
  }
  
  snprintf(gszFullNewProjectPathDir, sizeof(gszFullNewProjectPathDir), "%s/%s", gszProjectsPathDir, gstCmdLine.szProjName);

  if((iRsl = iMakeProject()) != 0)
  {
    vPrintErrorMessage(_("Impossible create the project!"));
    
    if(FATAL_DETAILS)
    {
      vTraceFatal("%s - end iRsl == %d", __func__, iRsl);
    }

    exit(EXIT_FAILURE);
  }

  if(INFO_DETAILS)
  {
    vInfoShowProjectInformations();
    vTraceInfo(_("%s - end iRsl == %d"), __func__, iRsl);
  }

  return iRsl;
}

