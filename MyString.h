#ifndef MYSTRING_H_INCLUDED
#define MYSTRING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

char toLowerChar(char c);
char toUpperChar(char c);
void makeLowerCaseString(char * str);
void makeUpperCaseString(char * str);
size_t stringLength(const char * str);
int compareString(const char * str1, const char * str2);
int icaseCompareString(const char * str1, const char * str2);
const char *indexOfChar(const char *str, char c);
char *indexOfString(const char *meule_de_foin, const char *aiguille);
int icaseStartWith(const char * start, const char * str);
int icaseEndWith(const char * end, const char * str);
void copyStringWithLength(char * dest, const char * src, size_t destSize);

#endif // MYSTRING_H_INCLUDED
