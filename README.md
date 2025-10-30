# hon's libSHA1
An open source, indepedent and easy to use SHA1 for C/C++.

## Satus
Working - Partially Tested

## Installation
1. Place `sha1.h` into `/usr/include`, then use it with:
```
#include <sha1.h>
```

## Example Usage
```
unsigned char* text_str = "this text will be hashed";
unsigned char* sha1_hash = sha1(text_str);
```
Be sure that the `sha1.h` file is installed and that you include it with `#include <sha1.h>`.
