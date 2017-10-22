#include "MyString.h"

/**< niveau débutant */

/** \brief toLowerChar
 *  \brief toUpperChar
 *  Ecrire la fonction char toLowerChar(char c) qui retourne le caractère fournit en paramètre en minuscule s’il
 *  s’agit d’une lettre.
 * \param char c
 * \return char
 *
 */

char toLowerChar(char c){
    return c>='A'&& c<='Z' ? c-='A'-'a' : c;
}

char toUpperChar(char c){
    return c>='a'&& c<='z' ? c+='A'-'a' : c;
}

/** \brief makeLowerCaseString
 *  \brief makeUpperCaseString
 *  Ecrire la fonction void makeLowerCaseString(char ∗ str) qui transforme en minuscule la chaîne de caractères
 *  fournie en paramètre.
 * \param char * str
 *
 */

void makeLowerCaseString(char * str){
    while(*str){
        *str++ = toLowerChar((unsigned char) *str);
    }
}

void makeUpperCaseString(char * str){
    while(*str) {
        *str++ = toUpperChar((unsigned char) *str);
    }
}

/** \brief stringLength
 *  Ecrire la fonction size_t stringLength(const char ∗ str) qui retourne le nombre de caractères d’une chaîne.
 *  Donner au moins deux solutions utilisant des mécanismes différents.
 * \param const char * str
 * \return size_t
 *
 */

size_t stringLength(const char * str){
    size_t len=0;
    while(*str++){
        len++;
    }
    return len;
}

/** \brief compareString
 *  Ecrire la fonction int compareString(const char ∗ str1, const char ∗ str2) qui compare deux chaînes de ca-
 *  ractères selon l’ordre lexicographique. La fonction retourne, respectivement, un nombre négatif, 0 ou un nombre
 *  positif, si la première chaîne est, respectivement, avant, égale ou après la seconde chaîne dans l’ordre lexicographique.
 * \param const char * str1
 * \param const char * str2
 * \return int
 *
 */

int compareString(const char * str1, const char * str2){
    if(*str1 && *str2){
        return *str1==*str2 ? compareString(++str1,++str2) : (*str1<*str2 ? -1 : 1);
    }else if(!*str1 && *str2){
        return -1;
    }else if(*str1 && !*str2){
        return 1;
    }else{
        return 0;
    }
}

/** \brief icaseCompareString
 *  Ecrire la fonction int icaseCompareString(const char ∗ str1, const char ∗ str2) qui compare deux chaînes de
 *  caractères comme compareString mais sans tenir compte de la différence majsucule/minuscule.
 * \param const char * str1
 * \param const char * str2
 * \return int
 *
 */

int icaseCompareString(const char * str1, const char * str2){
    char c1,c2;
    c1=toLowerChar((unsigned char) *str1);
    c2=toLowerChar((unsigned char) *str2);
    if(*str1 && *str2){
        return c1==c2 ? compareString(++str1,++str2) : (c1<c2 ? -1 : 1);
    }else if(!*str1 && *str2){
        return -1;
    }else if(*str1 && !*str2){
        return 1;
    }else{
        return 0;
    }
}

/** \brief indexOfChar
 *  Ecrire la fonction const char ∗ indexOfChar(const char ∗str, char c) qui retourne un pointeur sur la première
 *  occurence du caractère c dans la chaîne ou NULL si la chaîne ne contient pas le caractère recherché.
 * \param const char *str
 * \param char c
 * \return const char *
 *
 */

const char *indexOfChar(const char *str, char c){
    if(!*str){
        return NULL;
    }else if(c==*str){
        return str;
    }else{
        return indexOfChar(++str,c);
    }
}

/** \brief indexOfString
 *  Ecrire la fonction char ∗indexOfString(const char ∗meule_de_foin, const char ∗aiguille) qui retourne un
 *  pointeur sur la première occurence de aiguille dans meule_de_foin ou NULL si la chaîne meule_de_foin
 *  ne contient pas la chaîne recherchée.
 * \param const char *meule_de_foin
 * \param const char *aiguille
 * \return char *
 *
 */

char *indexOfString(const char *meule_de_foin, const char *aiguille){
    if(!*meule_de_foin){
        return NULL;
    }else if(!*aiguille){
        return meule_de_foin;
    }else if((*meule_de_foin)!=(*aiguille)){
        return indexOfString(++meule_de_foin,aiguille);
    }else if((*meule_de_foin)==(*aiguille)){
        return indexOfString(++meule_de_foin,++aiguille)-1;
    }
}

/** \brief icaseStartWith
 *  \brief icaseEndWith
 *  Ecrire la fonction int icaseStartWith(const char ∗ start, const char ∗ str) qui retourne vrai si la chaîne de
 *  caractères str débute par la chaîne de caractères start et faux sinon. La comparaison est insensible à la casse.
 * \param
 * \param
 * \return
 *
 */

int icaseStartWith(const char * start, const char * str){
    if(!*start){
        return 1;
    }else if((!*str) || (*start)!=(*str)){
        return 0;
    }else if((*start)==(*str)){
        return icaseStartWith(++start,++str);
    }
}

int icaseEndWith(const char * end, const char * str){
    size_t lenend=stringLength(end);
    size_t lenstr=stringLength(str);
    return (0==compareString(end,str+lenstr-lenend));
}

/** \brief copyStringWithLength
 *  Ecrire la fonction void copyStringWithLength(char ∗ dest, const char ∗ src, size_t destSize) qui copie les ca-
 *  ractères de src dans la chaîne dest. Cette opération copie au maximum destSize caractères dans la chaîne. Il
 *  peut donc y avoir troncature lors de la copie. Dans tous les cas, dest est une chaîne de caractères valide au sens
 *  des convention du C après l’opération.
 * \param char * dest
 * \param const char * src
 * \param size_t destSize
 *
 */

void copyStringWithLength(char * dest, const char * src, size_t destSize){
    char *tmp=dest;
    while(destSize){
        if((*tmp++=*src)!=0){
            src++;
        }
        destSize--;
    }
}
