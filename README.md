# hon's libSHA1
An open source, indepedent and easy to use SHA1 for C/C++.

## Satus
Working - Partially Tested

## Requirements
- STD: C89 or newer

## Example Usage
At the beginning of the file:
```c
#include <sha1.h>
```

Then:
```c
unsigned char* text_str = "this text will be hashed";
unsigned char* sha1_hash = sha1(text_str);
```
For a complete example, see [example.c](example.c).

When compiling use `-lsha1` to include the library.
For example.c:
```
gcc example.c -o example -lsha1
```

## Tests
You can compile example.c to test the lib. If output is `A94A8FE5CCB19BA61C4C0873D391E987982FBBD3`, it means that the lib is working correctly.
For test info see [TESTS.md](TESTS.md)
