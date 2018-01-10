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
 * $Id: CustomerRecord.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CustomerRecord.h>

void IMPLEMENT(CustomerRecord_setValue_name)(CustomerRecord * record, const char * value){
    copyStringWithLength(record->name, value, CUSTOMERRECORD_NAME_SIZE);
//  provided_CustomerRecord_setValue_name(record, value);
}

void IMPLEMENT(CustomerRecord_setValue_address)(CustomerRecord * record, const char * value){
    copyStringWithLength(record->address, value, CUSTOMERRECORD_ADDRESS_SIZE);
//  provided_CustomerRecord_setValue_address(record, value);
}

void IMPLEMENT(CustomerRecord_setValue_postalCode)(CustomerRecord * record, const char * value){
    copyStringWithLength(record->postalCode, value, CUSTOMERRECORD_POSTALCODE_SIZE);
//  provided_CustomerRecord_setValue_postalCode(record, value);
}

void IMPLEMENT(CustomerRecord_setValue_town)(CustomerRecord * record, const char * value){
    copyStringWithLength(record->town, value, CUSTOMERRECORD_TOWN_SIZE);
//  provided_CustomerRecord_setValue_town(record, value);
}

char * IMPLEMENT(CustomerRecord_getValue_name)(CustomerRecord * record){
    char *value = malloc(CUSTOMERRECORD_NAME_SIZE);
    copyStringWithLength(value, record->name, CUSTOMERRECORD_NAME_SIZE);
    return value;
//  return provided_CustIMPLEMENT(CustomerRecord_read)omerRecord_getValue_name(record);
}

char * IMPLEMENT(CustomerRecord_getValue_address)(CustomerRecord * record){
    char *value = malloc(CUSTOMERRECORD_ADDRESS_SIZE);
    copyStringWithLength(value, record->address, CUSTOMERRECORD_ADDRESS_SIZE);
    return value;
//  return provided_CustomerRecord_getValue_address(record);
}

char * IMPLEMENT(CustomerRecord_getValue_postalCode)(CustomerRecord * record){
    char *value = malloc(CUSTOMERRECORD_POSTALCODE_SIZE);
    copyStringWithLength(value, record->postalCode, CUSTOMERRECORD_POSTALCODE_SIZE);
    return value;
//  return provided_CustomerRecord_getValue_postalCode(record);
}

char * IMPLEMENT(CustomerRecord_getValue_town)(CustomerRecord * record){
    char *value = malloc(CUSTOMERRECORD_TOWN_SIZE);
    copyStringWithLength(value, record->town, CUSTOMERRECORD_TOWN_SIZE);
    return value;
//  return provided_CustomerRecord_getValue_town(record);
}

void IMPLEMENT(CustomerRecord_init)(CustomerRecord * record){
    memset(record->name, '\0',CUSTOMERRECORD_NAME_SIZE);
    memset(record->address, '\0',CUSTOMERRECORD_ADDRESS_SIZE);
    memset(record->postalCode, '\0',CUSTOMERRECORD_POSTALCODE_SIZE);
    memset(record->town, '\0',CUSTOMERRECORD_TOWN_SIZE);
//  provided_CustomerRecord_init(record);
}

void IMPLEMENT(CustomerRecord_finalize)(CustomerRecord * record){
    size_t len = stringLength(record->name);
    memset(record->name + len, '\0',CUSTOMERRECORD_NAME_SIZE - len);

    len = stringLength(record->address);
    memset(record->address + len, '\0',CUSTOMERRECORD_ADDRESS_SIZE - len);

    len = stringLength(record->postalCode);
    memset(record->postalCode + len, '\0',CUSTOMERRECORD_POSTALCODE_SIZE - len);

    len = stringLength(record->town);
    memset(record->town + len, '\0',CUSTOMERRECORD_TOWN_SIZE - len);

//  provided_CustomerRecord_finalize(record);
}

void IMPLEMENT(CustomerRecord_read)(CustomerRecord * record, FILE * file){
    char value[CUSTOMERRECORD_MAXSTRINGSIZE];

    if(!fread(value, 1, CUSTOMERRECORD_NAME_SIZE, file))
        fatalError("Read error\n");
    CustomerRecord_setValue_name(record, value);

    if(!fread(value, 1, CUSTOMERRECORD_ADDRESS_SIZE, file))
        fatalError("Read error\n");
    CustomerRecord_setValue_address(record, value);

    if(!fread(value, 1, CUSTOMERRECORD_POSTALCODE_SIZE, file))
        fatalError("Read error\n");
     CustomerRecord_setValue_postalCode(record, value);

    if(!fread(value, 1, CUSTOMERRECORD_TOWN_SIZE, file))
        fatalError("Read error\n");
    CustomerRecord_setValue_town(record, value);
//    provided_CustomerRecord_read(record, file);
}

void IMPLEMENT(CustomerRecord_write)(CustomerRecord * record, FILE * file){
    if(!fwrite(record->name, 1, CUSTOMERRECORD_NAME_SIZE, file))
        fatalError("Write error\n");

    if(!fwrite(record->address, 1, CUSTOMERRECORD_ADDRESS_SIZE, file))
        fatalError("Write error\n");

    if(!fwrite(record->postalCode, 1, CUSTOMERRECORD_POSTALCODE_SIZE, file))
        fatalError("Write error\n");

    if(!fwrite(record->town, 1, CUSTOMERRECORD_TOWN_SIZE, file))
        fatalError("Write error\n");

  //provided_CustomerRecord_write(record, file);
}

