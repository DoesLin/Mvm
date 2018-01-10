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
 * $Id: OperatorTable.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <OperatorTable.h>
#include <EncryptDecrypt.h>

/**
 * Create an empty table of operators.
 * @return the new table
 * @relates OperatorTable
 */
OperatorTable * IMPLEMENT(OperatorTable_create)(void) {
    OperatorTable * table = malloc(sizeof(OperatorTable));
	table->recordCount = 0;
	table->records = NULL;
	return table;
//    return provided_OperatorTable_create();
}

/** Free a table of operators.
 * @param table a pointer to the table to free
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_destroy)(OperatorTable * table) {
	for (int i = 0; i < table->recordCount; i++) {
		free(table->records[i][0]);
		free(table->records[i][1]);
		free(table->records[i]);
	}
	free(table->records);
	free(table);
//    provided_OperatorTable_destroy(table);
}

/** Load a table of operators from a file.
 * @param filename the file name
 * @return the new table
 * @relates OperatorTable
 */
OperatorTable * IMPLEMENT(OperatorTable_loadFromFile)(const char * filename) {
	FILE *pfile;

    if(!(pfile = fopen(filename, "rb+"))){
        pfile = fopen(filename, "wb+");
    }

	char nomb[10];
	int nbr;
	OperatorTable * table = OperatorTable_create();
	char name[OPERATORTABLE_MAXNAMESIZE], password[OPERATORTABLE_MAXPASSWORDSIZE];

	if (fgets(nomb, 10, pfile))
        nomb[stringLength(nomb) - 1] = '\0';

    sscanf(nomb, "%d", &nbr);

	for (int i = 0; i < nbr; i++) {
		if (fgets(name, OPERATORTABLE_MAXNAMESIZE, pfile)) {
			name[stringLength(name) - 1] = '\0';
			decrypt(OperatorCryptKey, name);
		}
		if (fgets(password, OPERATORTABLE_MAXPASSWORDSIZE, pfile)) {
			password[stringLength(password) - 1] = '\0';
			decrypt(OperatorCryptKey, password);
		}
		OperatorTable_setOperator(table, name, password);
	}
	fclose(pfile);
	return table;
//    return provided_OperatorTable_loadFromFile(filename);
}

/** Save a table of operators to a file.
 * @param table the table of operators
 * @param filename the file name
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_saveToFile)(OperatorTable * table, const char * filename) {
	FILE *pfile;
	pfile = fopen(filename, "wb+");
	char nomb[10];
	char name[OPERATORTABLE_MAXNAMESIZE], password[OPERATORTABLE_MAXPASSWORDSIZE];

    sprintf(nomb, "%d", table->recordCount);
	fputs(nomb, pfile);
	fputc('\n', pfile);

	for (int i = 0; i < table->recordCount; i++) {
        copyStringWithLength(name, table->records[i][0], OPERATORTABLE_MAXNAMESIZE + 1);
        copyStringWithLength(password, table->records[i][1], OPERATORTABLE_MAXPASSWORDSIZE + 1);

	    encrypt(OperatorCryptKey, name);
        encrypt(OperatorCryptKey, password);

		fputs(name, pfile);
		fputc('\n', pfile);
    	fputs(password, pfile);
    	fputc('\n', pfile);
	}
	fclose(pfile);
 //   provided_OperatorTable_saveToFile(table, filename);
}

/** Get the number of records of a table of operators
 * @param table the table of operators
 * @return the number of records
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_getRecordCount)(OperatorTable * table) {
	return table->recordCount;
//    return provided_OperatorTable_getRecordCount(table);
}

/** Get the name of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record number
 * @return the name of the operator
 * @relates OperatorTable
 */
const char * IMPLEMENT(OperatorTable_getName)(OperatorTable * table, int recordIndex) {
    return table->records[recordIndex][0];
 //   return provided_OperatorTable_getName(table, recordIndex);
}

/** Get the password of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record index
 * @return the paswword of the operator
 * @relates OperatorTable
 */
const char * IMPLEMENT(OperatorTable_getPassword)(OperatorTable * table, int recordIndex) {
    return table->records[recordIndex][1];
 //   return provided_OperatorTable_getPassword(table, recordIndex);
}

/** Get the record index associated with an operator name.
 * @param table the table of operators
 * @param name the name of the operator
 * @return the index of the operator or -1 if the operator is not in the table
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_findOperator)(OperatorTable * table, const char * name) {
	for (int i = 0; i < table->recordCount; i++) {
		if (!compareString(table->records[i][0], name))
			return i;
	}
	return -1;
//    return provided_OperatorTable_findOperator(table, name);
}

/** Define or change the password of an operator
 * @param table the table of operators
 * @param name the name of the operator
 * @param password the password of the operator
 * @return the index of the operator in the table
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_setOperator)(OperatorTable * table, const char * name, const char * password) {
    int i;
	for (i = 0; i < table->recordCount; i++) {
		if (!compareString(table->records[i][0], name)) {
			copyStringWithLength(table->records[i][1], password, OPERATORTABLE_MAXPASSWORDSIZE);
			return i;
		}
	}

	table->recordCount++;
	table->records = realloc(table->records, (sizeof(char ***) * (size_t)table->recordCount));

	table->records[i] = malloc(sizeof(char **) * 2);
	table->records[i][0] = malloc(OPERATORTABLE_MAXNAMESIZE);
	table->records[i][1] = malloc(OPERATORTABLE_MAXPASSWORDSIZE);
	copyStringWithLength(table->records[i][0], name, OPERATORTABLE_MAXNAMESIZE);
	copyStringWithLength(table->records[i][1], password, OPERATORTABLE_MAXPASSWORDSIZE);
	return i;
//    return provided_OperatorTable_setOperator(table, name, password);
}

/** Remove an operator from the table.
 * @param table the table of operators
 * @param recordIndex the index of the record to remove
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_removeRecord)(OperatorTable * table, int recordIndex) {
    if (recordIndex >= table->recordCount)
		return;
	int i;
	free(table->records[recordIndex][0]);
	free(table->records[recordIndex][1]);
	for (i = recordIndex; i < table->recordCount - 1; i++) {
		table->records[i][0] = table->records[i + 1][0];
		table->records[i][1] = table->records[i + 1][1];
	}
	free(table->records[i]);
	table->recordCount--;
	table->records = realloc(table->records, (sizeof(char ***) * (size_t)table->recordCount));
//	provided_OperatorTable_removeRecord(table, recordIndex);
}

