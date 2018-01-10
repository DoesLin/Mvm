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
 * $Id: CatalogRecord.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CatalogRecord.h>

/** Static function which test if a code only contains numbers and letters
 * @param  value the value to test
 * @return true if the code is valid, false otherwise
 */
int IMPLEMENT(CatalogRecord_isValueValid_code)(const char * value){
    char * p = (char *)value;
	while (*p) {
		if (*p >= 'a' && *p <= 'z'){
			p++;
			continue;
		}
		if (*p >= 'A' && *p <= 'Z') {
			p++;
			continue;
		}
		if (*p >= '0' && *p <= '9') {
			p++;
			continue;
		}
		return 0;
	}
	return 1;
//  return provided_CatalogRecord_isValueValid_code(value);
}

/** Static function which test if the value is a positive number
 * @param  value the value to test
 * @return true if the valie is valid, false otherwise
 */
int IMPLEMENT(CatalogRecord_isValueValid_positiveNumber)(const char * value){
    char* endptr;
	double tmp = strtod(value, &endptr);
	if (tmp > 0 && *endptr == '\0')
		return 1;
	return 0;
//  return provided_CatalogRecord_isValueValid_positiveNumber(value);
}

/** Static function to set the code field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_code)(CatalogRecord * record, const char * value){
    copyStringWithLength(record->code, value, CATALOGRECORD_CODE_SIZE);
//  provided_CatalogRecord_setValue_code(record, value);
}

/** Static function to set the designation field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_designation)(CatalogRecord * record, const char * value){
    copyStringWithLength(record->designation, value, CATALOGRECORD_DESIGNATION_SIZE);
//  provided_CatalogRecord_setValue_designation(record, value);
}

/** Static function to set the unity field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_unity)(CatalogRecord * record, const char * value){
    copyStringWithLength(record->unity, value, CATALOGRECORD_UNITY_SIZE);
//  provided_CatalogRecord_setValue_unity(record, value);
}

/** Static function to set the basePrice field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_basePrice)(CatalogRecord * record, const char * value){
   // if(CatalogRecord_isValueValid_positiveNumber(value))
    record->basePrice = atof(value);
//  provided_CatalogRecord_setValue_basePrice(record, value);
}

/** Static function to set the sellingPrice field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_sellingPrice)(CatalogRecord * record, const char * value){
    record->sellingPrice = atof(value);
//  provided_CatalogRecord_setValue_sellingPrice(record, value);
}

/** Static function to set the rateOfVAT field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void IMPLEMENT(CatalogRecord_setValue_rateOfVAT)(CatalogRecord * record, const char * value){
    record->rateOfVAT = atof(value);
//  provided_CatalogRecord_setValue_rateOfVAT(record, value);
}

/** Static function which create a copy string on the head of the code field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_code)(CatalogRecord * record){
    char *value = malloc(CATALOGRECORD_CODE_SIZE);
    copyStringWithLength(value, record->code, CATALOGRECORD_CODE_SIZE);
    return value;
//  return provided_CatalogRecord_getValue_code(record);
}

/** Static function which create a copy string on the head of the designation field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_designation)(CatalogRecord * record){
    char *value = malloc(CATALOGRECORD_DESIGNATION_SIZE);
    copyStringWithLength(value, record->designation, CATALOGRECORD_DESIGNATION_SIZE);
    return value;
//  return provided_CatalogRecord_getValue_designation(record);
}

/** Static function which create a copy string on the head of the unity field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_unity)(CatalogRecord * record){
    char *value = malloc(CATALOGRECORD_UNITY_SIZE);
    copyStringWithLength(value, record->unity, CATALOGRECORD_UNITY_SIZE);
    return value;
//  return provided_CatalogRecord_getValue_unity(record);
}

/** Static function which create a copy string on the head of the basePrice field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_basePrice)(CatalogRecord * record){
    char value[50];// = malloc(50);
    sprintf(value, "%.2lf", record->basePrice);
    return duplicateString(value);
//  return provided_CatalogRecord_getValue_basePrice(record);
}

/** Static function which create a copy string on the head of the sellingPrice field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_sellingPrice)(CatalogRecord * record){
    char value[50];// = malloc(50);
    sprintf(value, "%.2lf", record->sellingPrice);
    return duplicateString(value);
//  return provided_CatalogRecord_getValue_sellingPrice(record);
}

/** Static function which create a copy string on the head of the rateOfVAT field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(CatalogRecord_getValue_rateOfVAT)(CatalogRecord * record){
    char value[50];// = malloc(50);
    sprintf(value, "%.2lf", record->rateOfVAT);
    return duplicateString(value);
//  return provided_CatalogRecord_getValue_rateOfVAT(record);
}

/** Initialize a record
 * @param[in] record a pointer to a record to initialize
 * @warning every initialized record must be finalized with CatalogRecord_finalize()
 */
void IMPLEMENT(CatalogRecord_init)(CatalogRecord * record){
    record->code = malloc(CATALOGRECORD_CODE_SIZE);
    record->designation = malloc(CATALOGRECORD_DESIGNATION_SIZE);
    record->unity = malloc(CATALOGRECORD_UNITY_SIZE);

    memset(record->code, '\0',CATALOGRECORD_CODE_SIZE);
    memset(record->designation, '\0',CATALOGRECORD_DESIGNATION_SIZE);
    memset(record->unity, '\0',CATALOGRECORD_UNITY_SIZE);

    record->basePrice = 0;
	record->sellingPrice = 0;
	record->rateOfVAT = 0;
//  provided_CatalogRecord_init(record);
}

/** Finalize a record.
 * @param[in] record a pointer to a record to finalize
 */
void IMPLEMENT(CatalogRecord_finalize)(CatalogRecord * record){
    free(record->code);
	free(record->designation);
	free(record->unity);
//  provided_CatalogRecord_finalize(record);
}

/** Read a record from a file
 * @param record a pointer to an initialized record on which to store data
 * @param file the file from which the data are read
 */
void IMPLEMENT(CatalogRecord_read)(CatalogRecord * record, FILE * file){
    char value[CATALOGRECORD_MAXSTRING_SIZE];

    if(!fread(value, 1, CATALOGRECORD_CODE_SIZE, file))
        fatalError("Read error\n");
    CatalogRecord_setValue_code(record, value);

    if(!fread(value, 1, CATALOGRECORD_DESIGNATION_SIZE, file))
        fatalError("Read error\n");
    CatalogRecord_setValue_designation(record, value);

    if(!fread(value, 1, CATALOGRECORD_UNITY_SIZE, file))
        fatalError("Read error\n");
    CatalogRecord_setValue_unity(record, value);

	if (!fread(&(record->basePrice), 1, CATALOGRECORD_BASEPRICE_SIZE, file))
		fatalError("Read 'double' Error\n");
    if (!fread(&(record->sellingPrice), 1, CATALOGRECORD_SELLINGPRICE_SIZE, file))
        fatalError("Read 'double' Error\n");
    if (!fread(&(record->rateOfVAT), 1, CATALOGRECORD_RATEOFVAT_SIZE, file))
		fatalError("Read 'double' Error\n");

//  provided_CatalogRecord_read(record, file);
}

/** Write a record to a file
 * @param record a pointer to a record
 * @param file the file to which the data are written
 */
void IMPLEMENT(CatalogRecord_write)(CatalogRecord * record, FILE * file){
    if(!fwrite(record->code, 1, CATALOGRECORD_CODE_SIZE, file))
        fatalError("Write error\n");

    if(!fwrite(record->designation, 1, CATALOGRECORD_DESIGNATION_SIZE, file))
        fatalError("Write error\n");

    if(!fwrite(record->unity, 1, CATALOGRECORD_UNITY_SIZE, file))
        fatalError("Write error\n");

    if(!fwrite(&(record->basePrice), 1, CATALOGRECORD_BASEPRICE_SIZE, file))
        fatalError("Write error\n");

    if(!fwrite(&(record->sellingPrice), 1, CATALOGRECORD_SELLINGPRICE_SIZE, file))
        fatalError("Write error\n");

    if(!fwrite(&(record->rateOfVAT), 1, CATALOGRECORD_RATEOFVAT_SIZE, file))
        fatalError("Write error\n");
//  provided_CatalogRecord_write(record, file);
}

