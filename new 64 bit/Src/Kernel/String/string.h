#ifndef _STRING_H_
#define _STRING_H_

#include "../Types/types.h"

char toLowercose(char chr);

char toUppercase(char chr);

uint32_t strlen(char *str);

uint32_t strcmp(char *dest, char *src);

uint32_t strncmp(char *dest, char *src, size_t n);

uint32_t strcasecmp(char *dest, char *src);

uint32_t strncasecmp(char *dest, char *src, size_t n);

char *strcat(char *dest, char *src);

char *strncat(char *dest, char *src, size_t n);

char *strchr(char *dest, char chr);

char *strnchr(char *dest, char chr, size_t n);

char *strrchr(char *dest, char chr);

char *strstr(char *dest, char *src);

char *strnstr(char *dest, char *src, size_t n);

char *strcpy(char *dest, const char *src);

char *strncpy(char *dest, const char *src, size_t n);

void memcpy(unsigned char *dest, const unsigned char *src, unsigned char len);

void memset(unsigned char *dest, unsigned char val, unsigned char len);

void memmove(unsigned char *dest, unsigned char *src, unsigned char len);

int memcmp(const unsigned char *src1, const unsigned char *src2, unsigned int len);

#endif