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

