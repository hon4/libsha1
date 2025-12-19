#include <stdio.h>
#include <string.h>
#include <sha1.h>

int main() {
    const unsigned char* input = (unsigned char*)"test";
    SHA1_CTX context;
    unsigned char hash[20];

    SHA1_Init(&context);
    SHA1_Update(&context, input);  // Uses strlen() as in your library
    SHA1_Final(hash, &context);

    // Print the SHA-1 hash in hexadecimal format
    for (int i = 0; i < 20; i++) {
        printf("%02X", hash[i]);
    }
    printf("\n");

    return 0;
}
