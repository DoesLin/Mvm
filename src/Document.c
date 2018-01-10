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
 * $Id: Document.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Document.h>
#include <DocumentUtil.h>
#include <DocumentRowList.h>

/** Initialize a document
 * @param document a pointer to a document
 * @warning An initialized document must be finalized by Document_finalize() to free all resources
 */
void IMPLEMENT(Document_init)(Document * document) {
    CustomerRecord_init(&(document->customer));
    document->editDate=duplicateString("");
    document->expiryDate=duplicateString("");
    document->object=duplicateString("");
    document->operator=duplicateString("");
    document->docNumber=duplicateString("");
    document->rows=NULL;
    document->typeDocument=QUOTATION;
    //provided_Document_init(document);
}

/** Finalize a document
 * @param document the document to finalize
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_finalize)(Document * document) {
    CustomerRecord_finalize(&(document->customer));
    free(document->editDate);
    free(document->expiryDate);
    free(document->docNumber);
    free(document->object);
    free(document->operator);
    DocumentRowList_finalize(&(document->rows));
    document->typeDocument=QUOTATION;
//    provided_Document_finalize(document);
}

/** Save the content of a document to a file
 * @param document the document
 * @param filename the file name
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_saveToFile)(Document * document, const char * filename) {
    FILE * file=fopen(filename,"w+b");
    int count=0;
    if(file==NULL){
        fatalError("Error file open\n");
    }
    CustomerRecord_write(&(document->customer), file);
    /// This order is read from document-unittest.db
    writeString(document->docNumber,file);
    writeString(document->editDate,file);
    writeString(document->object,file);
    writeString(document->operator,file);
    writeString(document->expiryDate,file);
    if(0==fwrite(&(document->typeDocument),sizeof(TypeDocument),1,file)){
        fatalError("Error typeDocument write\n");
    }
    DocumentRow *row=document->rows;
    count=DocumentRowList_getRowCount(row);
    if(0==(fwrite(&count,sizeof(int),1,file))){
        fatalError("Error rows'count write\n");
    }
    while(row!=NULL){
        DocumentRow_writeRow(row,file);
        row=row->next;
    }
    fclose(file);
//    provided_Document_saveToFile(document,filename);
}

/** Load the content of a document from a file
 * @param document the document to fill
 * @param filename the file name
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_loadFromFile)(Document * document, const char * filename) {
    FILE * file;
    int count=0;
    if(!(file = fopen(filename, "rb+"))){
        file = fopen(filename, "wb+");
    }
//    FILE * file=fopen(filename,"r+b");
//    if(file==NULL){
//        fatalError("Error file open\n");
//    }
    CustomerRecord_read(&(document->customer), file);
    free(document->docNumber);
    document->docNumber=readString(file);
    free(document->editDate);
    document->editDate=readString(file);
    free(document->object);
    document->object=readString(file);
    free(document->operator);
    document->operator=readString(file);
    free(document->expiryDate);
    document->expiryDate=readString(file);
    if(0==fread(&(document->typeDocument),sizeof(TypeDocument),1,file)){
        fatalError("Error typeDocument read\n");
    }
    DocumentRow *row=NULL;
    if(0==(fread(&count,sizeof(int),1,file))){
        fatalError("Error rows'count read\n");
    }
    document->rows=DocumentRow_readRow(file);
    row=document->rows;
    while(--count!=0){
        row->next=DocumentRow_readRow(file);
        row=row->next;
    }
    fclose(file);
//    provided_Document_loadFromFile(document,filename);
}

