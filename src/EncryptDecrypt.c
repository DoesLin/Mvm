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
 * $Id: EncryptDecrypt.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <EncryptDecrypt.h>

void IMPLEMENT(encrypt)(const char * key, char * str) {
    char *keytmp = duplicateString(key);
    makeLowerCaseString(keytmp);
    char *keyc = keytmp;
    while(*str){
        *str = (char)(*str + (*keyc - 'a'));
        if(*str > 'z') *str = (char)(*str - 26);
        keyc++;
        if(!*keyc) keyc = keytmp;
        str++;
    }
    free(keytmp);
//  provided_encrypt(key,str);
}
void IMPLEMENT(decrypt)(const char * key, char * str) {
    char *keytmp = duplicateString(key);
    makeLowerCaseString(keytmp);
    char *keyc = keytmp;
    while(*str){
        *str = (char)(*str - (*keyc - 'a'));
        if(*str < 'a') *str = (char)(*str + 26);
        keyc++;
        if(!*keyc) keyc = keytmp;
        str++;
    }
    free(keytmp);
//  provided_decrypt(key,str);
}
