#include "../Types/types.h"
#include "string.h"

char toLowercose(char chr)
{
    return chr < 91 ? chr + 32 : chr;
}

char toUppercase(char chr)
{
    return chr < 91 ? chr : chr + 32;
}

uint32_t strlen(char *str)
{
    unsigned int count = 0;
    while (*str != '\0')
    {
        count++;
        str++;
    }
    return count;
}

uint32_t strcmp(char *dest, char *src)
{
    for (; *dest == *src; ++dest, ++src)
        if (*dest == 0)
            return 0;
    return *(const unsigned char *)dest - *(const unsigned char *)src;
}

uint32_t strncmp(char *dest, char *src, size_t n)
{
    for (int i = 0; *dest == *src && i < n; ++dest, ++src)
        if (*dest == 0)
            return 0;
    return *(const unsigned char *)dest - *(const unsigned char *)src;
}

uint32_t strcasecmp(char *dest, char *src)
{
    for (; toLowercose(*dest) == toLowercose(*src); ++dest, ++src)
        if (*dest == 0)
            return 0;
    return *(const unsigned char *)dest - *(const unsigned char *)src;
}

uint32_t strncasecmp(char *dest, char *src, size_t n)
{
    for (int i = 0; toLowercose(*dest) == toLowercose(*src) && i < n; ++dest, ++src)
        if (*dest == 0)
            return 0;
    return *(const unsigned char *)dest - *(const unsigned char *)src;
}

char *strcat(char *dest, char *src)
{
    char* ptr = dest + strlen(dest);
 
    // appends characters of the source to the destination string
    while (*src != '\0') {
        *ptr++ = *src++;
    }
 
    // null terminate destination string
    *ptr = '\0';
 
    // the destination is returned by standard `strcat()`
    return dest;
}

char *strncat(char *dest, char *src, size_t n)
{
    char* ptr = dest + strlen(dest);
	int c = 0;
    // appends characters of the source to the destination string
    while (*src != '\0' && c < n) {
        *ptr++ = *src++;
		c++;
    }
 
    // null terminate destination string
    *ptr = '\0';
 
    // the destination is returned by standard `strcat()`
    return dest;
}

char *strchr(char *dest, char chr)
{
    for (size_t i = 0; i < strlen(dest); i++)
    {
        if (dest[i] == chr)
        {
            return &dest[i];
        }
    }
    return 0;
}

char *strnchr(char *dest, char chr, size_t n)
{
    for (size_t i = 0; i < (strlen(dest) > n) ? n : strlen(dest); i++)
    {
        if (dest[i] == chr)
        {
            return &dest[i];
        }
    }
    return 0;
}

char *strrchr(char *dest, char chr)
{
    char *tmp = 0;
    for (size_t i = 0; i < strlen(dest); i++)
    {
        if (dest[i] == chr)
        {
            tmp = &dest[i];
        }
    }
    return tmp;
}

char *strstr(char *dest, char *src)
{
    for (size_t i = 0; i < strlen(dest); i++)
    {
        if (strcmp(dest + i, src) == 0)
            return dest + i;
    }
    return 0;
}

char *strnstr(char *dest, char *src, size_t n)
{
    for (size_t i = 0; i < (strlen(dest) > n) ? n : strlen(dest); i++)
    {
        if (strcmp(dest + i, src) == 0)
            return dest + i;
    }
    return 0;
}

char *strcpy(char *dest, const char *src)
{
    // return if no memory is allocated to the destination
    if (dest == 0)
    {
        return 0;
    }

    // take a pointer pointing to the beginning of the destination string
    char *ptr = dest;

    // copy the C-string pointed by source into the array
    // pointed by destination
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }

    // include the terminating null character
    *dest = '\0';

    // the destination is returned by standard `strcpy()`
    return ptr;
}

char *strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';

    return dest;
}
