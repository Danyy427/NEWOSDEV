#ifndef _STRING_H_
#define _STRING_H_

#include "../Types/types.h"

char toLowercose(char chr);

char toUppercase(char chr);

uint32_t strlen(char *str);

uint32_t strcmp(char *dest, char *src);

uint32_t strncmp(char *s1, char *s2, size_t n);

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

void memcpy(unsigned char *dest, const unsigned char *src, uint64_t len);

void memset(unsigned char *dest, unsigned char val, uint64_t len);

void memmove(unsigned char *dest, unsigned char *src, uint64_t len);

int memcmp(const unsigned char *src1, const unsigned char *src2, uint64_t len);

char *itoa(uint64_t value, char *buffer, int base);

#endif