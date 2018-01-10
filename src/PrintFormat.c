/*
 * Copyright 2010 Sébastien Aupetit <sebtic@projectsforge.org>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id: PrintFormat.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <PrintFormat.h>
#include <Dictionary.h>

/** Initialize a print format
 * @param format a print format
 * @warning initialized print format should be finalized with PrintFormat_finalize()
 */
void IMPLEMENT(PrintFormat_init)(PrintFormat * format)
{
    format->name=duplicateString("");
    format->header=duplicateString("");
    format->row=duplicateString("");
    format->footer=duplicateString("");
//  provided_PrintFormat_init(format);
}

/** Finalize a print format
 * @param format the print format
 */
void IMPLEMENT(PrintFormat_finalize)(PrintFormat * format)
{
    free(format->name);
    free(format->header);
    free(format->row);
    free(format->footer);
//  provided_PrintFormat_finalize(format);
}

static char *Format_obtainName(const char * format){
    char curFmt=*format, nameFmt[255]={'\0'};
    int lenFmt=(int)stringLength(format), cpt=0;

    while(curFmt!=' '&&curFmt!='\n'&&cpt<lenFmt){
        /// S'il est un nom encore
        nameFmt[cpt++]=curFmt;
        curFmt=toUpperChar(*(++format));
    }
    return duplicateString(nameFmt);
}

static char *readLine(FILE *fichier){
    char str[1024];
    if(fgets(str, 1024, fichier)!=NULL){
        return duplicateString(str);
    }else{
        fatalError("Error line\n");
    }
}

static void loadLines(char **docMember, const char *nextMember, FILE *fichier, char **curLine){
    char *curMember=NULL;
    free(*curLine);
    *curLine=readLine(fichier);
    while(1!=icaseStartWith(nextMember,*curLine)){
        curMember=*docMember;
        *docMember=concatenateString(curMember,*curLine);
        free(curMember);
        free(*curLine);
        *curLine=readLine(fichier);
    }
    curMember=*docMember;
    *docMember=malloc(stringLength(curMember)+1);
    copyStringWithLength(*docMember,curMember,stringLength(curMember));
    free(curMember);
}

/** Load a print format from a file
 * @param format an initialized print format to fill
 * @param filename the file name
 */
void IMPLEMENT(PrintFormat_loadFromFile)(PrintFormat * format, const char * filename)
{
    FILE *file;
    if(!(file = fopen(filename, "rt"))){
        file = fopen(filename, "wt+");
    }
//    FILE *file=fopen(filename,"rt");
    char *curLine=NULL, *nameFmt=NULL;

    curLine=readLine(file);
    while(*curLine=='.'){
        nameFmt=Format_obtainName(curLine+1);
        if(0==compareString(nameFmt,"NAME")){
            size_t strLen=(size_t)(stringLength(curLine)-stringLength(nameFmt)-2);/// ' ' & '\n'
            free(format->name);
            format->name=malloc(strLen+1);
            copyStringWithLength(format->name,curLine+stringLength(nameFmt)+2,strLen);/// '.' & ' '
            free(curLine);
            curLine=readLine(file);
        }else if(0==compareString(nameFmt,"HEADER")){
            loadLines(&(format->header),".ROW",file,&curLine);
        }else if(0==compareString(nameFmt,"ROW")){
            loadLines(&(format->row),".FOOTER",file,&curLine);
        }else if(0==compareString(nameFmt,"FOOTER")){
            loadLines(&(format->footer),".END",file,&curLine);
        }else if(0==compareString(nameFmt,"END")){
            free(nameFmt);
            free(curLine);
            break;
        }else{fatalError("Error format\n");}
        free(nameFmt);
    }
    fclose(file);
//  provided_PrintFormat_loadFromFile(format, filename);
}

