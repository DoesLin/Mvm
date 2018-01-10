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
 * $Id: DocumentUtil.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <DocumentUtil.h>

/** Create a new string on the heap which represents the parameter as a number in basis 36.
 * @param id the number to convert
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(computeDocumentNumber)(long id) {
    char const base36[36]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    /* log(2**64) / log(36) = 12.38 => max 13 char + '\0' */
    char buffer[14];
    unsigned int offset=14;

    buffer[--offset]='\0';
    do{
        buffer[--offset]=base36[id%36];
    }while(id/=36);
    return duplicateString(&buffer[offset]);
    //return provided_computeDocumentNumber(id);
}

/** Create a new string on the heap which represents the date in the format DD/MM/YYYY.
 * @param day the day
 * @param month the month
 * @param year the year
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(formatDate)(int day, int month, int year) {
    char const base10[10]="0123456789";
    char mydate[11];
    mydate[0]=base10[day/10];
    mydate[1]=base10[day%10];
    mydate[2]='/';
    mydate[3]=base10[month/10];
    mydate[4]=base10[month%10];
    mydate[5]='/';
    mydate[6]=base10[year/1000];
    mydate[7]=base10[year/100-year/1000*10];
    mydate[8]=base10[year/10-year/100*10];
    mydate[9]=base10[year%10];
    mydate[10]='\0';
    return duplicateString(mydate);
//    return provided_formatDate(day, month, year);
}


/** Write a string in a binary file
 * @param str the string
 * @param file the file
 */
void IMPLEMENT(writeString)(const char * str, FILE * file) {
    fputs(str,file);
	fputc('\0',file);
//    provided_writeString(str,file);
}

/** Read a string from a binary file
 * @param file the file
 * @return a new string created on the heap which contains the read string
 * @see writeString()
 */
char * IMPLEMENT(readString)(FILE * file) {
    char str[255];
    str[0]=(char)fgetc(file);
    unsigned int offset=0;
    while(str[offset++]!='\0'){
        str[offset]=(char)fgetc(file);
    }
    return duplicateString(str);
//    return provided_readString(file);
}
