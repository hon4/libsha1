/*
╔═════════════╗
║ hon libSHA1 ║
╚═════════════╝
│             │
└─────────────┘
*/
#include <stdint.h>
#include <string.h>

//The SHA1 Addition Function (auto pad to uint32 no need for mod 2^32 or and bitmask to 32bit
uint32_t sha1_add(uint32_t x, uint32_t y) {
	return x + y;
}

//The SHA1 Circular BitShift SnX
uint32_t sha1_snx(uint8_t n, uint32_t x) {
	return (x << n) | (x >> 32-n);
}

uint64_t calc_pad_size(uint64_t l) {
	return ((l + 63) / 64) * 64;
}

uint8_t* sha1(uint8_t* str) {
	uint64_t len = (uint64_t)strlen((char*)str);
	uint64_t bit_len = len * 8;
	uint64_t fulllen = calc_pad_size(len + 9); //+9 = +1 for 0x80 and +8 for uint64

	//uint8_t padstr[fulllen];
	//memcpy(padstr, str, len);

	printf("%llu\n",len);
	return 0;
}
