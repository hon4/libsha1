/*
+==================+
| hon libSHA1      |
+==================+
| Coded by: hon    |
| Version: 0.0.4   |
| Date: 2025-12-19 |
+------------------+
*/
/*
 * This file (sha1.h) is part of hon's libSHA1
 * Copyright (C) 2025 hon
 *
 * This program is free and open source software.
 * License: GPL-2.0
*/
// Thanks to mmxsrup (https://github.com/mmxsrup) for SHA1 Calculation in parts. Used ideas from https://github.com/mmxsrup/libsha1

#ifndef LIBSHA1_SHA1_H
#define LIBSHA1_SHA1_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

typedef struct {
	uint32_t H[5];		// hash value
	uint64_t total_len;	// Total SHA1 Size
	uint8_t buffer[64];	// a single 512 bit block buffer
} SHA1_CTX;

uint8_t* sha1(const uint8_t* str);

void SHA1_Init(SHA1_CTX* context);
void SHA1_Update(SHA1_CTX* context, const uint8_t* data);
void SHA1_Final(uint8_t digest[20], SHA1_CTX* context);


#ifdef __cplusplus
}
#endif

#endif
