/*
╔══════════════════╗
║ hon libSHA1      ║
╚══════════════════╝
│ Coded by: hon    │
│ Version: 0.0.1   │
│ Date: 2025-11-20 │
└──────────────────┘
*/
/*
 * This file (sha1.h) is part of hon's libSHA1
 * Copyright (C) 2025 hon
 *
 * This program is free and open source software.
 * License: GPL-2.0
*/


// Define Endianness (1=Little-Endian, 0=Big-Endian)
// Now gets endianness from configure
#include "config.h"
#if defined(WORDS_BIGENDIAN) && WORDS_BIGENDIAN == 1
	//Big-Endian received from configure
	#define __LIBSHA1_USE_ENDIANNESS__ 0
#elif WORDS_BIGENDIAN == 0
	//Little-Endian received from configure
	#define __LIBSHA1_USE_ENDIANNESS__ 1
#else
	#error "Endianness not defined"
#endif
//Do NOT change the following
#define __LIBSHA1_LITTLE_ENDIAN__ 1
#define __LIBSHA1_BIG_ENDIAN__ 0

#include <stdint.h>
#include <string.h>

/*
General SHA1 to C info
==========================
This SHA1 library (libsha1) is based on information of RFC3174

SHA1 General Size Info
+------------+-------+-------+------------+
| What       | Bits  | Bytes | SHA1 Words |
+------------+-------+-------+------------+
| SHA1 Word  |    32 |     4 |          1 |
| SHA1 Block |   512 |    64 |         16 |
+------------+-------+-------+------------+

SHA1 to C for libsha1
+------------+--------------+---------+
| What       | libsha1 use  | Bits    |
+------------+--------------+---------+
| SHA1 Word  | uint32_t     |      32 |
| SHA1 Block | uint32_t[16] | 16 * 32 |
+------------+--------------+---------+

SHA1 Operators
Notes: 1. In addition: if the Z variable is uint32_t can get values 0 <= z < 2^32 that is the same as the SHA1 word so there is no need for mod 2^32
       2. In addition: in case the Z variable is not uint32_t or the calculation is not directly saved to a uint32_t variable,
          & 0xFFFFFFFF (bitmask to 32bits) is preferred to using mod 2^32 because BitMask method is mutch faster
+---------------------+------------+----------------------------------+---------------------------------------------------------------------------------+
| What                | Type       | SHA1                             | libsha1                                                                         |
+---------------------+------------+----------------------------------+---------------------------------------------------------------------------------+
| Logical AND         | Bitwise    | X AND Y                          | X & Y                                                                           |
| Logical OR          | Bitwise    | X OR Y                           | X | Y                                                                           |
| Logical NOT         | Bitwise    | NOT X                            | ~X                                                                              |
| Logical XOR         | Bitwise    | X XOR Y                          | X ^ Y                                                                           |
| Addition            | Arithmetic | Z = (X + Y) mod 2^32             | = X + Y(when Z is a uint32_t) or Z = (X + Y) & 0xFFFFFFFF (when Z not uint32_t) |
| Circular Left Shift | Bitwise    | S^n(X) = (X << n) OR (X >> 32-n) | = (X << n) | (X >> 32-n)                                                        |
+---------------------+------------+----------------------------------+---------------------------------------------------------------------------------+
*/

//The SHA1 Addition Function (auto pad to uint32 no need for mod 2^32 or and bitmask to 32bit
uint32_t sha1_add(uint32_t X, uint32_t Y) {
	return X + Y;
}

//The SHA1 Circular BitShift S^n(X)
uint32_t sha1_snx(uint8_t n, uint32_t X) {
	return (X << n) | (X >> (32-n)); //Ensure the - is executed before the BitShift
}

//The SHA1 Function (f) 0 <= t <= 79, so uint8_t. B,C,D words
uint32_t sha1_funct(uint8_t t, uint32_t B, uint32_t C, uint32_t D) {
	if (t <= 19) {			//  0 <= t <= 19
		return (B & C) | ((~B) & D);
	} else if (t <= 39) {	// 20 <= t <= 39
		return B ^ C ^ D;
	} else if (t <= 59) {	// 40 <= t <= 59
		return (B & C) | (B & D) | (C & D);
	} else {				// 60 <= t <= 79
		return B ^ C ^ D;
	}
}

//The SHA1 Constant (K). 0 <= t <= 79, so uint8_t
uint32_t sha1_const(uint8_t t) {
	if (t <= 19) {			//  0 <= t <= 19
		return 0x5A827999;
	} else if (t <= 39) {	// 20 <= t <= 39
		return 0x6ED9EBA1;
	} else if (t <= 59) {	// 40 <= t <= 59
		return 0x8F1BBCDC;
	} else {				// 60 <= t <= 79
		return 0xCA62C1D6;
	}
}

//Calculation Helper Function
uint64_t calc_pad_size(uint64_t l) {
	//Need benchmarks on x86, on aarch64/arm64 method 2 with inline +9 +63 gives the highest speed
	return ((l + 63 + 9) / 64);	//Method 2 (+9 moved from sha1 function to result less instructions and highest speed), a=(l + 63 + 9), 1 instruction (+63 +9 = +72 in compile time), b=(a / 64) 1 instruction, total 2 instructions (1x add, 1x div)
	//return (l + 9 / 64) + 1;	//Method 1 a=(l + 9) 1 instruction, b=(a / 64) 1 instruction, c=(b + 1) 1 instruction, total 3 instruction (2x add, 1x div)
}

uint32_t endian_le2be(uint32_t i) {
	#if defined(__LIBSHA1_USE_ENDIANNESS__) && (__LIBSHA1_USE_ENDIANNESS__ == __LIBSHA1_LITTLE_ENDIAN__)
	return ((i >> 24) & 0x000000FF) |
			((i >> 8) & 0x0000FF00) |
			((i << 8) & 0x00FF0000) |
			((i << 24) & 0xFF000000);
	#else
	return i; //Big-Endian: No need anything
	#endif
}

uint8_t* sha1(const uint8_t* str) {
	uint64_t len = (uint64_t)strlen((char*)str);
	uint64_t bit_len = len * 8;
	uint64_t block_count = calc_pad_size(len); //+9 = +1 for 0x80 and +8 for uint64 (+9 moved to calc_pad_size function for speed improvement)
	uint32_t M[block_count][16];
	memset(M, 0x00, block_count * 16 * sizeof(uint32_t));

	/* Add the str into the M */
	uint64_t __libsha1_tmp_i;
	for (__libsha1_tmp_i = 0; __libsha1_tmp_i < len; __libsha1_tmp_i++) {
		((uint8_t*)M)[__libsha1_tmp_i] = str[__libsha1_tmp_i];
	}

	((uint8_t*)M)[len] = 0x80; //Add the 0x80 = 0b10000000 at the end of the str

	#if defined(__LIBSHA1_USE_ENDIANNESS__) && (__LIBSHA1_USE_ENDIANNESS__ == __LIBSHA1_LITTLE_ENDIAN__)
	//Big-Endian to Little-Endian prevent useless for loop in Big-Endian, only used in Little-Endian
	uint64_t iword;
	for (iword = 0; iword < block_count * 16; iword++) {
		((uint32_t*)M)[iword] = endian_le2be(((uint32_t*)M)[iword]);
	}
	#endif

	/* Add the uint64_t bitlen at the last two uint32_t words of the last block */
	// No endianess switch for the bit_len uint64(2 * uint32_t) because it's genned by the CPU and has the machines' arch already.
	M[block_count-1][14] = (uint32_t)(bit_len >> 32);
	M[block_count-1][15] = (uint32_t)bit_len;


	/* SHA1 Calculation using RFC3174 Method 1 */
	/* Initial Hx Variable Values */
	uint32_t H0 = 0x67452301;
	uint32_t H1 = 0xEFCDAB89;
	uint32_t H2 = 0x98BADCFE;
	uint32_t H3 = 0x10325476;
	uint32_t H4 = 0xC3D2E1F0;

	uint32_t W[80];
	// Process all blocks
	uint64_t xblock;
	for (xblock = 0; xblock < block_count; xblock++) {
		memset(W, 0x00, 80 * sizeof(uint32_t)); //Clear the W
		//Move the M[xblock] at the beggining of the W
		int __libsha1_tmp_i2;
		for (__libsha1_tmp_i2 = 0; __libsha1_tmp_i2 < 16; __libsha1_tmp_i2++) {
			W[__libsha1_tmp_i2] = M[xblock][__libsha1_tmp_i2];
		}

		//Method 1 Step B
		uint8_t __libsha1_tmp_t1;
		for (__libsha1_tmp_t1 = 16; __libsha1_tmp_t1 <= 79; __libsha1_tmp_t1++) { //t<=79 in x86 asm is checked with jle (1 instruction) but for other trashchip architectures like arm will need more than 1 instruction for check.
			W[__libsha1_tmp_t1] = sha1_snx(1, W[__libsha1_tmp_t1-3] ^ W[__libsha1_tmp_t1-8] ^ W[__libsha1_tmp_t1-14] ^ W[__libsha1_tmp_t1-16]);
		}

		//Method 1 Step C
		uint32_t A = H0;
		uint32_t B = H1;
		uint32_t C = H2;
		uint32_t D = H3;
		uint32_t E = H4;

		//Method 1 Step D
		uint8_t __libsha1_tmp_t2;
		for (__libsha1_tmp_t2 = 0; __libsha1_tmp_t2 <= 79; __libsha1_tmp_t2++) {
			uint32_t TEMP = sha1_add(sha1_add(sha1_add(sha1_add(sha1_snx(5, A), sha1_funct(__libsha1_tmp_t2, B, C, D)), E), W[__libsha1_tmp_t2]), sha1_const(__libsha1_tmp_t2));
			E = D;
			D = C;
			C = sha1_snx(30, B);
			B = A;
			A = TEMP;
		}

		//Method 1 Step E
		H0 = sha1_add(H0, A);
		H1 = sha1_add(H1, B);
		H2 = sha1_add(H2, C);
		H3 = sha1_add(H3, D);
		H4 = sha1_add(H4, E);
	}
	/* End RFC3174 Method 1 */

	/* Convert H0, H1, H2, H3, H4 to a uint8_t* */
	static uint8_t ret[20]; //20bytes is the SHA1 giest length
	uint32_t* H = (uint32_t*)ret;
	#if defined(__LIBSHA1_USE_ENDIANNESS__) && (__LIBSHA1_USE_ENDIANNESS__ == __LIBSHA1_LITTLE_ENDIAN__)
	//Endianness change required only in Little-Endian
	H[0] = endian_le2be(H0);
	H[1] = endian_le2be(H1);
	H[2] = endian_le2be(H2);
	H[3] = endian_le2be(H3);
	H[4] = endian_le2be(H4);
	#else
	H[0] = H0;
	H[1] = H1;
	H[2] = H2;
	H[3] = H3;
	H[4] = H4;
	#endif

	return ret;
}
