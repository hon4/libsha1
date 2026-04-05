#include <stdio.h>
#include <string.h>
#include <sha1.h>

int main() {
	/* Input string to hash */
	const unsigned char* input = (unsigned char*)"test";

	/* Calculate SHA-1 hash */
	unsigned char* hash = sha1(input, strlen(input));

	/* Print the SHA-1 hash in hexadecimal format */
	int i;
	for (i = 0; i < 20; i++) {
		printf("%02X", hash[i]);
	}
	printf("\n");

	return 0;
}
