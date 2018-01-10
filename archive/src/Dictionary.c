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
 * $Id: Dictionary.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Dictionary.h>

/** Create an empty dictionary on the heap
 * @return a new dictionary
 * @warning the dictionary should be destroyed using Dictionary_destroy()
 */
Dictionary * IMPLEMENT(Dictionary_create)(void)
{
    Dictionary *dictionary=(Dictionary *)malloc(sizeof(Dictionary));

    if(dictionary==NULL){
        fatalError("Error creating a new Dictionary\n");
    }
    dictionary->count=0;
    dictionary->entries=NULL;
    return dictionary;
    //return provided_Dictionary_create();
}

/** Destroy a dictionary
 * @param dictionary the dictionary
 */
void IMPLEMENT(Dictionary_destroy)(Dictionary * dictionary)
{
    DictionaryEntry *freeEry=dictionary->entries;

    while(dictionary->count--!=0){
        free(freeEry->name);
        if(freeEry->type==STRING_ENTRY){
            free(freeEry->value.stringValue);
        }
        freeEry++;
    }
    free(dictionary->entries);
    free(dictionary);
    //Dictionary_destroy(dictionary);
}

/** Get a pointer on the entry associated with the given entry name
 * @param dictionary the dictionary
 * @param name the name of the entry
 * @return a pointer on the entry or NULL if the entry was not found
 */
DictionaryEntry * IMPLEMENT(Dictionary_getEntry)(Dictionary * dictionary, const char * name)
{
    DictionaryEntry *nameEry=dictionary->entries;

    int count=dictionary->count;
    while(count--!=0){
        if(0==icaseCompareString(name,nameEry->name)){
            return nameEry;
        }
        nameEry++;
    }
    return NULL;
    //return provided_Dictionary_getEntry(dictionary, name);
}

/** Define or change a dictionary entry as a string
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void IMPLEMENT(Dictionary_setStringEntry)(Dictionary * dictionary, const char * name, const char * value)
{
    Dictionary *dic=dictionary;
    DictionaryEntry *newEry=Dictionary_getEntry(dic,name);

    if(newEry==NULL){
        /// S'il est une entrée vide ou il n'y a pas une même entrée de ce nom
        dic->count++;
        newEry=(DictionaryEntry *)realloc(dictionary->entries,(size_t)(dic->count)*sizeof(DictionaryEntry));
        if(newEry==NULL){
            fatalError("Error creating a new entries\n");
        }
        dictionary->entries=newEry;
        newEry+=dic->count-1;
        newEry->name=duplicateString(name);
    }else{
        /// S'il y a une même entrée de ce nom
        if(newEry->type==STRING_ENTRY){
            free(newEry->value.stringValue);
        }
    }
    newEry->type=STRING_ENTRY;
    newEry->value.stringValue=duplicateString(value);
    //provided_Dictionary_setStringEntry(dictionary, name, value);
}

/** Define or change a dictionary entry as a number
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void IMPLEMENT(Dictionary_setNumberEntry)(Dictionary * dictionary, const char * name, double value)
{
    Dictionary *dic=dictionary;
    DictionaryEntry *newEry=Dictionary_getEntry(dic,name);

    if(newEry==NULL){
        /// S'il est une entrée vide ou il n'y a pas une même entrée de ce nom
        dic->count++;
        newEry=(DictionaryEntry *)realloc(dictionary->entries,(size_t)(dic->count)*sizeof(DictionaryEntry));
        if(newEry==NULL){
            fatalError("Error creating a new entries\n");
        }
        dictionary->entries=newEry;
        newEry+=dic->count-1;
        newEry->name=duplicateString(name);
    }
    newEry->type=NUMBER_ENTRY;
    newEry->value.numberValue=value;
    //provided_Dictionary_setNumberEntry(dictionary, name, value);
}

static char *Format_obtainName(const char * format){
    char curFmt=*format, nameFmt[255]={'\0'};
    int lenFmt=(int)stringLength(format), cpt=0;

    while(curFmt!='{'&&curFmt!='='&&curFmt!='}'&&curFmt!='%'&&curFmt!=','&&cpt<lenFmt){
        /// S'il est un nom encore
        nameFmt[cpt++]=curFmt;
        curFmt=toLowerChar(*(++format));
    }
    return duplicateString(nameFmt);
}

static char *Format_obtainPrecision(DictionaryEntry *entry, const char * format){
    char strDbl[256];

    switch(*format) {
        case '0':
            if(0==snprintf(strDbl,256,"%.f",entry->value.numberValue)){
                fatalError("Error print\n");}
            break;
        case '1':
            if(0==snprintf(strDbl,256,"%.1f",entry->value.numberValue)){
                fatalError("Error print\n");}
            break;
        case '2':
            if(0==snprintf(strDbl,256,"%.2f",entry->value.numberValue)){
                fatalError("Error print\n");}
            break;
        case '3':
            if(0==snprintf(strDbl,256,"%.3f",entry->value.numberValue)){
                fatalError("Error print\n");}
            break;
        case '4':
            if(0==snprintf(strDbl,256,"%.4f",entry->value.numberValue)){
                fatalError("Error print\n");}
            break;
        case '5':
            if(0==snprintf(strDbl,256,"%.5f",entry->value.numberValue)){
                fatalError("Error print\n");}
            break;
        case '6':
            if(0==snprintf(strDbl,256,"%f",entry->value.numberValue)){
                fatalError("Error print\n");}
            break;
        default : fatalError("Error format\n");
    }
    return duplicateString(strDbl);
}

/** Create a new string on the heap which is the result of the formatting of format according to the dictionary content
 * @param dictionary the dictionary
 * @param format the string to format
 * @return a new string created on the heap
 * @warning the user is responsible for freeing the returned string
 */
char * IMPLEMENT(Dictionary_format)(Dictionary * dictionary, const char * format)
{
    char *nameVar=NULL, *nameMdf=NULL, *nameNum=NULL;
    char *curFmt=NULL, *freeFmt=NULL, *endFmt=NULL;
    char *curResult=NULL, *result=NULL;
    DictionaryEntry *entry=NULL;

    if(format[0]=='%'&&(format[stringLength(format)-1]=='%')){
        curFmt=duplicateString(indexOfChar(format,'%')+1);/// Var... or %
        if(*curFmt=='%') {return duplicateString("%");}/// %% >> %
        nameVar=Format_obtainName(curFmt);
        freeFmt=curFmt;/// Var...
        curFmt+=stringLength(nameVar);/// {... or %

        while(*curFmt!='%'&&*curFmt){
            curFmt+=1;/// case...
            free(nameMdf);
            nameMdf=Format_obtainName(curFmt);
            entry=Dictionary_getEntry(dictionary,nameVar);
            curFmt+=stringLength(nameMdf)+1;

            if(0==compareString(nameMdf,"case")){
                if(entry->type==STRING_ENTRY){
                    result=duplicateString(entry->value.stringValue);
                    if(*curFmt>=97&&*curFmt<=122){
                        makeLowerCaseString(result);
                    }else if(*curFmt>=65&&*curFmt<=90){
                        makeUpperCaseString(result);
                    }else{fatalError("Error format\n");}
                }else{fatalError("Error entry\n");}
            }else if(0==compareString(nameMdf,"min")){
                size_t strLength=(size_t)strtol(curFmt,&endFmt,10);
                if(curFmt==endFmt){fatalError("Error format\n");}
                char *blankRes=NULL, *charRes=NULL;
                if(entry->type==NUMBER_ENTRY){
                    if(result!=NULL){
                        if(strLength<stringLength(result)){fatalError("Error format\n");}
                        strLength-=stringLength(result);
                        blankRes=malloc(strLength+1);
                        memset(blankRes,' ',strLength);
                        *(blankRes+strLength)='\0';
                        curResult=concatenateString(blankRes,result);
                        free(result);
                        result=curResult;
                    }else{
                        blankRes=malloc(strLength+1);
                        memset(blankRes,' ',strLength);
                        *(blankRes+strLength)='\0';
                        charRes=Format_obtainPrecision(entry,"6");
                        result=concatenateString(blankRes,charRes);
                    }
                }else if(entry->type==STRING_ENTRY){
                    if(strLength<stringLength(entry->value.stringValue)){fatalError("Error format\n");}
                    strLength-=stringLength(entry->value.stringValue);
                    blankRes=malloc(strLength+1);
                    memset(blankRes,' ',strLength);
                    *(blankRes+strLength)='\0';
                    if(result==NULL){
                        result=concatenateString(entry->value.stringValue,blankRes);
                    }else{fatalError("Error format\n");}
                }else{fatalError("Error entry\n");}
                free(blankRes);
                free(charRes);
            }else if(0==compareString(nameMdf,"max")){
                if(entry->type==STRING_ENTRY){
                    size_t strLength=(size_t)strtol(curFmt,&endFmt,10);
                    if(curFmt==endFmt) {fatalError("Error format\n");}
                    strLength=strLength>stringLength(entry->value.stringValue)? stringLength(entry->value.stringValue):strLength;
                    curResult=malloc(strLength+1);
                    if(result!=NULL){
                        copyStringWithLength(curResult,result,strLength+1);
                        free(result);
                    }else{
                        copyStringWithLength(curResult,entry->value.stringValue,strLength+1);
                    }
                    result=curResult;
                }else{fatalError("Error format\n");}
            }else if(0==compareString(nameMdf,"precision")){/// number...
                char *lastRes=result;
                curResult=Format_obtainPrecision(entry,curFmt); /// 2}%... or 2,min...
                if(lastRes!=NULL){
                    result=concatenateString(lastRes,curResult);
                    free(curResult);
                    free(lastRes);
                }else{result=curResult;}
            }else{fatalError("Error format\n");}
            nameNum=Format_obtainName(curFmt);
            curFmt+=stringLength(nameNum);/// }%... or ,min...
            free(nameNum);
            if(*curFmt=='}'&&*(curFmt+1)=='%'){
                curFmt+=2;/// \0 or ' '
                if(*curFmt){
                    char buffResult[256]={'\0'};
                    int cpt=0;
                    while(*curFmt!='%'&&*curFmt){
                        buffResult[cpt++]=*curFmt++;
                    }
                    curResult=concatenateString(result,buffResult);
                    free(result);
                    result=curResult;
                    free(nameVar);
                    nameVar=Format_obtainName(++curFmt);/// VAR1{precision=0}%
                    curFmt+=stringLength(nameVar);/// precision=0}%
                }
            }
        }
        free(freeFmt);
        if(result==NULL){/// %Var%
            entry=Dictionary_getEntry(dictionary,nameVar);
            if(entry->type==NUMBER_ENTRY){
                char strDbl[256];
                if(0==snprintf(strDbl,256,"%f",entry->value.numberValue)){
                    fatalError("Error print\n");}
                result=duplicateString(strDbl);
            }else if(entry->type==STRING_ENTRY){
                result=duplicateString(entry->value.stringValue);
            }else{fatalError("Error entry\n");}
        }
    }else{fatalError("Error format\n");}
    free(nameVar);
    free(nameMdf);
    return result;
    //return provided_Dictionary_format(dictionary, format);
}
