/*
╔══════════════════╗
║ hon libSHA1      ║
╚══════════════════╝
│ Coded by: hon    │
│ Version: 0.0.0   │
│ Date: 2025-10-29 │
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
	//Need benchmarks on x86, on aarch64/arm64 method 2 with inline +9 +63 gives the highest speed
	return ((l + 63) / 64);	//Method 2
	//return (l / 64) + 1;	//Method 1
}

uint32_t endian_le2be(uint32_t i) {
	return ((i >> 24) & 0x000000FF) |
			((i >> 8) & 0x0000FF00) |
			((i << 8) & 0x00FF0000) |
			((i << 24) & 0xFF000000);
}

uint8_t* sha1(uint8_t* str) {
	uint64_t len = (uint64_t)strlen((char*)str);
	uint64_t bit_len = len * 8;
	uint64_t block_count = calc_pad_size(len + 9); //+9 = +1 for 0x80 and +8 for uint64
	uint32_t M[block_count][16];
	memset(M, 0x00, block_count * 16 * sizeof(uint32_t));

	/* Add the str into the M */
	for (uint64_t i = 0; i < len; i++) {
		((uint8_t*)M)[i] = str[i];
	}

	((uint8_t*)M)[len] = 0x80; //Add the 0x80 = 0b10000000 at the end of the str

	((uint32_t*)M)[0] = endian_le2be(((uint32_t*)M)[0]);

	/* Add the uint64_t bitlen at the last two uint32_t words of the last block */
	// No endianess switch for the bit_len uint64(2 * uint32_t) because it's genned by the CPU and has the machines' arch already.
	M[block_count-1][14] = (uint32_t)(bit_len >> 32);
	M[block_count-1][15] = (uint32_t)(bit_len & 0xFFFFFFFF);


	/* SHA1 Calculation using RFC3174 Method 1 */
	uint32_t H0 = 0x67452301;
	uint32_t H1 = 0xEFCDAB89;
	uint32_t H2 = 0x98BADCFE;
	uint32_t H3 = 0x10325476;
	uint32_t H4 = 0xC3D2E1F0;

	//uint32_t *W = (uint32_t*)(M[0]); //W points to the First M block
	uint32_t W[80];
	memset(W, 0x00, 80 * sizeof(uint32_t));
	for (int i = 0; i < 16; i++) {
		W[i] = M[0][i];
	}

	for (uint8_t t = 16; t <= 79; t++) { //t<=79 in x86 asm is checked with jle (1 instruction) but for other trashchip architectures like arm will need more than 1 instruction for check.
		W[t] = sha1_snx(1, W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
	}

	uint32_t A = H0;
	uint32_t B = H1;
	uint32_t C = H2;
	uint32_t D = H3;
	uint32_t E = H4;

	for (uint8_t t = 0; t <= 79; t++) {
		uint32_t TEMP = sha1_add(sha1_add(sha1_add(sha1_add(sha1_snx(5, A), sha1_funct(t, B, C, D)), E), W[t]), sha1_const(t));
		E = D;
		D = C;
		C = sha1_snx(30, B);
		B = A;
		A = TEMP;
	}

	H0 = sha1_add(H0, A);
	H1 = sha1_add(H1, B);
	H2 = sha1_add(H2, C);
	H3 = sha1_add(H3, D);
	H4 = sha1_add(H4, E);
	/* End RFC3174 Method 1 */

	/* Convert H0, H1, H2, H3, H4 to a uint8_t* */
	static uint8_t ret[20]; //20bytes is the SHA1 giest length
	uint32_t* H = (uint32_t*)ret;
	H[0] = endian_le2be(H0);
	H[1] = endian_le2be(H1);
	H[2] = endian_le2be(H2);
	H[3] = endian_le2be(H3);
	H[4] = endian_le2be(H4);

	return ret;
}
