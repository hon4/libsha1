/*
╔══════════════════╗
║ hon libSHA1      ║
╚══════════════════╝
│ Coded by: hon    │
│ Version: 0.0.0   │
│ Date: 2025-10-28 │
└──────────────────┘
*/
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
	return (X << n) | (X >> 32-n);
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
	return ((l + 63) / 64);
}

uint8_t* sha1(uint8_t* str) {
	uint64_t len = (uint64_t)strlen((char*)str);
	uint64_t bit_len = len * 8;
	uint64_t block_count = calc_pad_size(len + 9); //+9 = +1 for 0x80 and +8 for uint64
	uint32_t M[block_count][16];

	printf("%llu\n",len);
	return 0;
}
