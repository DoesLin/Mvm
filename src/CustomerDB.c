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
 * $Id: CustomerDB.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CustomerDB.h>
#include <CustomerRecord.h>
#include <CustomerRecordEditor.h>

const char * CUSTOMERDB_FILENAME = BASEPATH "/data/Customer.db";

CustomerDB * IMPLEMENT(CustomerDB_create)(const char * filename) {
    FILE *pfile;
    pfile = fopen(filename, "wb+");
    CustomerDB *cdb = malloc(sizeof(CustomerDB));
    cdb->file = pfile;
    cdb->recordCount = 0;
    fprintf(pfile, "%d", 0);
    return cdb;
//    return provided_CustomerDB_create(filename);
}

CustomerDB * IMPLEMENT(CustomerDB_open)(const char * filename) {
    FILE *pfile;
    if(!(pfile = fopen(filename, "rb+"))){
        return NULL;
    }
    CustomerDB *cdb = malloc(sizeof(CustomerDB));
    cdb->file = pfile;
    fscanf(pfile, "%d", &(cdb->recordCount));
    return cdb;
//    return provided_CustomerDB_open(filename);
}

CustomerDB * IMPLEMENT(CustomerDB_openOrCreate)(const char * filename) {
    FILE *pfile;
    CustomerDB *cdb = malloc(sizeof(CustomerDB));
    if(!(pfile = fopen(filename, "rb+"))){  //create
        pfile = fopen(filename, "wb+");
        cdb->recordCount = 0;
        fprintf(pfile, "%d", 0);
    }else{  //open
        fscanf(pfile, "%d", &(cdb->recordCount));
    }
    cdb->file = pfile;
    return cdb;
//    return provided_CustomerDB_openOrCreate(filename);
}

void IMPLEMENT(CustomerDB_close)(CustomerDB * customerDB) {
    fclose(customerDB->file);
    free(customerDB);
//    provided_CustomerDB_close(customerDB);
}

int IMPLEMENT(CustomerDB_getRecordCount)(CustomerDB * customerDB) {
    return customerDB->recordCount;
//    return provided_CustomerDB_getRecordCount(customerDB);
}

char * CustomerDB_getFieldValueAsString(CustomerDB * customerDB, int recordIndex, int field) {
    char * content = NULL;
    if (customerDB != NULL) {
        CustomerRecord_FieldProperties properties = CustomerRecord_getFieldProperties(field);
        CustomerRecord record;
        CustomerRecord_init(&record);
        CustomerDB_readRecord(customerDB, recordIndex, &record);
        content = (*properties.getValue)(&record);
        CustomerRecord_finalize(&record);
    }
    return content;
}

void IMPLEMENT(CustomerDB_appendRecord)(CustomerDB * customerDB, CustomerRecord *record) {
    CustomerDB_insertRecord(customerDB, customerDB->recordCount, record);
//    provided_CustomerDB_appendRecord(customerDB, record);
}

void IMPLEMENT(CustomerDB_insertRecord)(CustomerDB * customerDB, int recordIndex, CustomerRecord * record) {
    customerDB->recordCount++;
    fseek(customerDB->file, 0, SEEK_SET);
    fprintf(customerDB->file, "%d", customerDB->recordCount);

    CustomerRecord recordtmp;
    CustomerRecord_init(&recordtmp);
    for(int i = customerDB->recordCount - 2;i >= recordIndex;i--){
        CustomerDB_readRecord(customerDB, i, &recordtmp);
        CustomerDB_writeRecord(customerDB, i + 1, &recordtmp);
    }
    CustomerRecord_finalize(&recordtmp);

    CustomerDB_writeRecord(customerDB, recordIndex, record);
//    provided_CustomerDB_insertRecord(customerDB, recordIndex, record);
}

void IMPLEMENT(CustomerDB_removeRecord)(CustomerDB * customerDB, int recordIndex) {
    CustomerRecord recordtmp;
	CustomerRecord_init(&recordtmp);
	for (int i = recordIndex + 1; i < customerDB->recordCount; i++) {
		CustomerDB_readRecord(customerDB, i, &recordtmp);
		CustomerDB_writeRecord(customerDB, i - 1, &recordtmp);
	}
	CustomerRecord_finalize(&recordtmp);

	customerDB->recordCount--;
	fseek(customerDB->file, 0, SEEK_SET);
	fprintf(customerDB->file, "%d", customerDB->recordCount);
//    provided_CustomerDB_removeRecord(customerDB, recordIndex);
}

void IMPLEMENT(CustomerDB_readRecord)(CustomerDB * customerDB, int recordIndex, CustomerRecord * record) {
    int whence =(int)sizeof(int) + recordIndex * (int)CUSTOMERRECORD_SIZE; //4 + n * 308
    fseek(customerDB->file, whence, SEEK_SET);
    CustomerRecord_read(record, customerDB->file);
//    provided_CustomerDB_readRecord(customerDB, recordIndex, record);
}

void IMPLEMENT(CustomerDB_writeRecord)(CustomerDB * customerDB, int recordIndex, CustomerRecord * record) {
    if(customerDB->recordCount < recordIndex + 1){
        customerDB->recordCount++;
        fseek(customerDB->file, 0, SEEK_SET);
        fprintf(customerDB->file, "%d", customerDB->recordCount);
    }

    int whence =(int)sizeof(int) + recordIndex * (int)CUSTOMERRECORD_SIZE; //4 + n * 308
    fseek(customerDB->file, whence, SEEK_SET);
    CustomerRecord_write(record, customerDB->file);
//    provided_CustomerDB_writeRecord(customerDB, recordIndex, record);
}
