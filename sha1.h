#include <stdint.h>

uint32_t sha1_add(uint32_t x, uint32_t y) {
	return x + y;
}

uint32_t sha1_snx(uint8_t n, uint32_t x) {
	return (x << n) | (x >> 32-n);
}
