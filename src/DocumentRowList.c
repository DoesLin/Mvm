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
 * $Id: DocumentRowList.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <DocumentRowList.h>
#include <DocumentUtil.h>

/** Initialize a row
 * @param row the row
 * @warning an initialized row must be finalized by DocumentRow_finalize() to free all resources
 */
void IMPLEMENT(DocumentRow_init)(DocumentRow * row) {
    row->code=duplicateString("");
    row->designation=duplicateString("");
    row->quantity=0;
    row->unity=duplicateString("");
    row->basePrice=0;
    row->sellingPrice=0;
    row->discount=0;
    row->rateOfVAT=0;
    row->next=NULL;
//    provided_DocumentRow_init(row);
}

/** Finalize a row
 * @param row the row
 * @warning document must have been initialized
 */
void IMPLEMENT(DocumentRow_finalize)(DocumentRow * row) {
    free(row->code);
    free(row->designation);
    free(row->unity);
//    provided_DocumentRow_finalize(row);
}

/** Create a new row on the heap and initialize it
 * @return the new row
 */
DocumentRow * IMPLEMENT(DocumentRow_create)(void) {
    DocumentRow *row=malloc(sizeof(DocumentRow));
    if(row==NULL){
        fatalError("Error creating a new DocumentRow\n");
    }
    DocumentRow_init(row);
    return row;
//    return provided_DocumentRow_create();
}

/** Finalize and destroy a row previously created on the heap
 * @param row the row
 */
void IMPLEMENT(DocumentRow_destroy)(DocumentRow * row) {
    DocumentRow_finalize(row);
    free(row);
//    provided_DocumentRow_destroy(row);
}

/** Initialize a list of rows
 * @param list the address of the pointer on the first cell of the list
 */
void IMPLEMENT(DocumentRowList_init)(DocumentRow ** list) {
    DocumentRow *head=NULL;
    *list=head;
//    provided_DocumentRowList_init(list);
}

/** Finalize a list of rows
 * @param list the address of the pointer on the first cell of the list
 * @note Each row of the list are destroyer using DocumentRow_destroy()
 */
void IMPLEMENT(DocumentRowList_finalize)(DocumentRow ** list) {
    /**< free():  If ptr is a null pointer, no action occurs. */
    if(*list==NULL) ;
    else{
        DocumentRowList_finalize(&((*list)->next));
        DocumentRow_destroy(*list);
    }
//    provided_DocumentRowList_finalize(list);
}

/** Get a pointer on the rowIndex-th row of the list
 * @param list the pointer on the first cell of the list
 * @param rowIndex the index of the requested row
 * @return a pointer on the rowIndex-th row of the list or NULL if the list contains less rows than the requested one
 */
DocumentRow * IMPLEMENT(DocumentRowList_get)(DocumentRow * list, int rowIndex) {
    while(rowIndex>0&&(list!=NULL)){
        rowIndex--;
        list=list->next;
    }
    return list;
//    return provided_DocumentRowList_get(list, rowIndex);
}

/**
 * Get the number of rows in the list
 * @param list the pointer on the first cell of the list
 */
int IMPLEMENT(DocumentRowList_getRowCount)(DocumentRow * list) {
    int count=0;
    while(list!=NULL){
        count++;
        list=list->next;
    }
    return count;
//    return provided_DocumentRowList_getRowCount(list);
}

/** Add a row at the end of the list
 * @param list the address of the pointer on the first cell of the list
 * @param row the row to add
 */
void IMPLEMENT(DocumentRowList_pushBack)(DocumentRow ** list, DocumentRow * row) {
    if(*list==NULL){
        *list=row;
        (*list)->next=NULL;
    }else{
        DocumentRowList_pushBack(&((*list)->next),row);
    }
//    provided_DocumentRowList_pushBack(list, row);
}

/** Insert a row before a given row
 * @param list the address of the pointer on the first cell of the list
 * @param position a pointer on the positioning row
 * @param row the row to insert
 */
void IMPLEMENT(DocumentRowList_insertBefore)(DocumentRow ** list, DocumentRow * position, DocumentRow * row) {
    if(*list==NULL||position==NULL){
        fatalError("Error list or error position\n");
    }else{
        if(*list==position){
            row->next=*list;
            *list=row;
        }else if((*list)->next==position){
            row->next=position;
            (*list)->next=row;
        }else{
            DocumentRowList_insertBefore(&((*list)->next),position,row);
        }
    }
//    provided_DocumentRowList_insertBefore(list, position, row);
}

/** Insert a row after a given row
 * @param list the address of the pointer on the first cell of the list
 * @param position a pointer on the positioning row
 * @param row the row to insert
 */
void IMPLEMENT(DocumentRowList_insertAfter)(DocumentRow ** list, DocumentRow * position, DocumentRow * row) {
    if(*list==NULL||position==NULL){
        fatalError("Error insertAfter\n");
    }else{
        if(*list==position){
            row->next=(*list)->next;
            (*list)->next=row;
        }else{
            DocumentRowList_insertAfter(&((*list)->next),position,row);
        }
    }
//    provided_DocumentRowList_insertAfter(list, position, row);
}

/** Remove a row from the list
 * @param list the address of the pointer on the first cell of the list
 * @param position the row to remove
 */
void IMPLEMENT(DocumentRowList_removeRow)(DocumentRow ** list, DocumentRow * position) {
    if(*list==NULL||position==NULL){
        fatalError("Error removeRow\n");
    }else{
        if(*list==position){
            *list=position->next;
            DocumentRow_destroy(position);
        }else if((*list)->next==position){
            (*list)->next=position->next;
            DocumentRow_destroy(position);
        }else{
            DocumentRowList_removeRow(&((*list)->next),position);
        }
    }
//    provided_DocumentRowList_removeRow(list, position);
}

/** Write a row in a binary file
 * @param row the row
 * @param file the opened file
 */
void IMPLEMENT(DocumentRow_writeRow)(DocumentRow * row, FILE * file) {
    writeString(row->code,file);
    writeString(row->designation,file);
    writeString(row->unity,file);
    fprintf(file,"%f",row->basePrice);
    fprintf(file,"%f",row->discount);
    fprintf(file,"%f",row->sellingPrice);
    fprintf(file,"%f",row->quantity);
    fprintf(file,"%f",row->rateOfVAT);
//    provided_DocumentRow_writeRow(row, file);
}

/** Read a row from a file
 * @param file the opened file
 * @return a new row created on the heap filled with the data
 */
DocumentRow * IMPLEMENT(DocumentRow_readRow)(FILE * file) {
    DocumentRow *row=DocumentRow_create();
    free(row->code);
    row->code=readString(file);
    free(row->designation);
    row->designation=readString(file);
    free(row->unity);
    row->unity=readString(file);
    fscanf(file,"%lf",&row->basePrice);
    fscanf(file,"%lf",&row->discount);
    fscanf(file,"%lf",&row->sellingPrice);
    fscanf(file,"%lf",&row->quantity);
    fscanf(file,"%lf",&row->rateOfVAT);
    return row;
//    return provided_DocumentRow_readRow(file);
}
