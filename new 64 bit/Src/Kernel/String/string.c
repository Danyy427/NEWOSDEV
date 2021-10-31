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

uint32_t strncmp(char *s1, char *s2, size_t n)
{
    unsigned char c1, c2;
    while (n)
    {
        c1 = *s1++;
        c2 = *s2++;
        if (c1 != c2)
            return c1 < c2 ? -1 : 1;
        if (!c1)
            break;
        n--;
    }
    return 0;
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
    char *ptr = dest + strlen(dest);

    // appends characters of the source to the destination string
    while (*src != '\0')
    {
        *ptr++ = *src++;
    }

    // null terminate destination string
    *ptr = '\0';

    // the destination is returned by standard `strcat()`
    return dest;
}

char *strncat(char *dest, char *src, size_t n)
{
    char *ptr = dest + strlen(dest);
    int c = 0;
    // appends characters of the source to the destination string
    while (*src != '\0' && c < n)
    {
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

void memcpy(unsigned char *dest, const unsigned char *src, uint64_t len)
{
    const unsigned char *sp = (const unsigned char *)src;
    unsigned char *dp = (unsigned char *)dest;
    while (len--)
    {
        *dp++ = *sp++;
    }
}

void memset(unsigned char *dest, unsigned char val, uint64_t len)
{
    unsigned char *temp = (unsigned char *)dest;
    while (len--)
    {
        *temp++ = val;
    }
}

void memmove(unsigned char *dest, unsigned char *src, uint64_t len)
{
    unsigned char *sp = (unsigned char *)src;
    unsigned char *dp = (unsigned char *)dest;
    if (dp < sp)
    {
        while (len--)
        {
            *dp++ = *sp++;
        }
    }
    else
    {
        unsigned char *lasts = sp + (len - 1);
        unsigned char *lastd = dp + (len - 1);
        while (len--)
        {
            *lastd-- = *lasts--;
        }
    }
}

int memcmp(const unsigned char *src1, const unsigned char *src2, uint64_t len)
{
    const unsigned char *s1 = (const unsigned char *)src1;
    const unsigned char *s2 = (const unsigned char *)src2;
    while (len--)
    {
        if (*s1++ != *s2++)
        {
            return s1[-1] < s2[-1] ? -1 : 1;
        }
    }
    return 0;
}

void swap(char *x, char *y)
{
    char t = *x;
    *x = *y;
    *y = t;
}

char *reverse(char *buffer, int i, int j)
{
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);

    return buffer;
}

// Iterative function to implement itoa() function in C
char *itoa(uint64_t value, char *buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32)
        return buffer;

    // consider absolute value of number
    uint64_t n = abs(value);

    uint64_t i = 0;
    while (n)
    {
        uint64_t r = n % base;

        if (r >= 10)
            buffer[i++] = 65 + (r - 10);
        else
            buffer[i++] = 48 + r;

        n = n / base;
    }

    // if number is 0
    if (i == 0)
        buffer[i++] = '0';

    // If base is 10 and value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
        buffer[i++] = '-';

    buffer[i] = '\0'; // null terminate string

    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}
