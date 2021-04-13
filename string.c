/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2020
 *
 * String Utilities
 */

#include "string.h"

/**
 * Sets the first n bytes pointed to by str to the value specified by c
 *
 * @param   dest - pointer the block of memory to set
 * @param   c    - value to set; passed as an integer but converted to unsigned
 * char when set
 * @param   n    - number of bytes to set
 * @return  pointer to the memory region being set; NULL on error
 */
void *sp_memset(void *dest, int c, size_t n) {
    /* !!! CODE NEEDED !!! */
    char *chardata;
    int i;
    //We taking the void pointer and changing it to a char to write 1 byte at a time to the block of memory
    chardata = (char *)dest;
    i=0;

    //Check to see if the pointer is valid or not
    if(*chardata == '\0' )
        return ;
    else
    {
        //Writes the variable c to everything in the memory block
        for(i=0; i < n; i++)
        {
            *chardata = (char)c;
            chardata= chardata + 1;
        }
    }
    return dest;
}

/**
 * Copies n bytes from the block of memory pointed to by src to dest
 *
 * @param   dest - pointer to the destination block of memory
 * @param   src  - pointer to the source block of memory
 * @param   n    - number of bytes to read/set
 * @return  pointer to the destination memory region; NULL on error
 */
void *sp_memcpy(void *dest, const void *src, size_t n) {
    /* !!! CODE NEEDED !!! */
    char *point, *write;
    int i;
    point = (char *) dest;
    write = (char *) src;


    if( n == 0)

    {
        return ;
    }
    else
    {

        for(i=0; i < (n) ; i++)
        {
            *point = *write;
            write++;
            point++;
        }
   }
    return dest;
}

/**
 * Compares the first n bytes of memory pointed to by str1 and str2
 *
 * @param  s1  - pointer to a block of memory
 * @param  s2  - pointer to a block of memory.
 * @param  n   - number of bytes to compare
 * @return  0 if every byte in s1 is equal to s2 or if n is 0
 *         <0 if the first non-matching byte of s1 is less than that of s2
 *         >0 if the first non-matching byte of s1 is greater than that of s2
 *         For a non-zero value the value will indicate the difference
 */
int sp_memcmp(const void *str1, const void *str2, size_t n) {
    /* !!! CODE NEEDED !!! */
    char * s1, * s2;
    int i, keep;
    keep = 0;
    s1 = (char *) str1;
    s2 = (char *) str2;

    if( n==0 )
        return 0;

    for(i=0; i < n; i++)
    {
        if (*s1 != *s2)
        {
            if (*s1 > *s2)
                keep = 1;
            else
                keep = -1;
            i = 20;
        }
        s1 = s1+1;
        s2 = s2+1;
    }

    if(keep == 1)
        return 1;
    else if(keep == 0)
        return 0;
    else
        return -1;
}

/**
 * Computes the length of the string str up to, but not including the null
 * terminating character
 *
 * @param  str - pointer to the string
 * @return length of the string
 */
size_t sp_strlen(const char *str) {
    /* !!! CODE NEEDED !!! */
    int i;
    i = 0;
    while( *str != '\0')
    {
        i++;
        str = str +1;
    }
    return i;
}

/**
 * Copies the string pointed to by src to the destination dest
 *
 * @param  dest - pointer to the destination string
 * @param  src  - pointer to the source string
 * @return pointer to the destination string
 */
char *sp_strcpy(char *dest, const char *src) {
    /* !!! CODE NEEDED !!! */
    char *copy, *change;
    copy = dest;
    change = src;

    while(*change != '\0')
    {
        *copy = *change;
        copy++;
        change++;
    }
    return dest;
}

/**
 * Copies up to n characters from the source string src to the destination
 * string dest. If the length of src is less than that of n, the remainder
 * of dest up to n will be filled with NULL characters.
 *
 * @param  dest - pointer to the destination string
 * @param  src  - pointer to the source string
 * @param  n    - maximum number of characters to be copied
 * @return pointer to the destination string
 */
char *sp_strncpy(char *dest, const char *src, size_t n) {
    /* !!! CODE NEEDED !!! */
    char *copy, *change;
    int i;

    copy = dest;
    change = src;
    i=0;

    for(i=0; i < n; i++)
    {
        if(*change != '\0')
        {
            *copy = *change;
            change++;
        }
        else
            *copy = '\0';
        copy++;
    }
    return dest;
}

/**
 * Compares the string pointed to by str1 to the string pointed to by str2
 *
 * @param  str1 - pointer to the string str1
 * @param  str2 - pointer to the string str2
 * @return  0 if every character in str1 is equal to str2
 *         <0 if the first non-matching character of str1 is less than that of
 * str2 >0 if the first non-matching character of str1 is greater than that of
 * str2 For a non-zero value the value will indicate the difference
 */
int sp_strcmp(const char *str1, const char *str2) {
    /* !!! CODE NEEDED !!! */
    int i;
    i = 0;

    while( ((*(str1+i)) != '\0') && ((*(str2+i)) != '\0') )
    {
        if( *(str1+i)  < *(str2+i) )
            return 1;
        else if ( *(str1+i) > *(str2+i) )
            return -1;
        else
            i++;
    }
    return 0;
}

/**
 * Compares the string pointed to by str1 to the string pointed to by str2 up to
 * the first n characters
 *
 * @param  str1 - pointer to the string str1
 * @param  str2 - pointer to the string str2
 * @param  n    - maximum number of characters to be compared
 * @return  0 if every character in str1 is equal to str2 or if n is 0
 *         <0 if the first non-matching character of str1 is less than that of
 * str2 >0 if the first non-matching character of str1 is greater than that of
 * str2 For a non-zero value the value will indicate the difference
 */
int sp_strncmp(const char *str1, const char *str2, size_t n) {
    /* !!! CODE NEEDED !!! */
    int i;
    i=0;

    for(i=0; i < n; i++)
    {
        if( *(str1+i)  < *(str2+i) )
            return 1;
        else if ( *(str1+i) > *(str2+i) )
            return -1;
    }
    return 0;
}
