/*
╔
*/
#include <stdint.h>

//The SHA1 Addition Function (auto pad to uint32 no need for mod 2^32 or and bitmask to 32bit
uint32_t sha1_add(uint32_t x, uint32_t y) {
	return x + y;
}

//The SHA1 Circular BitShift SnX
uint32_t sha1_snx(uint8_t n, uint32_t x) {
	return (x << n) | (x >> 32-n);
}
