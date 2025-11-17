# hon's libSHA1
An open source, indepedent and easy to use SHA1 for C/C++.

## Satus
Working - Partially Tested

## Requirements
- STD: C89 or newer

## Installation
Place `sha1.h` into `/usr/include`, then use it with:
```c
#include <sha1.h>
```

## Example Usage
```c
unsigned char* text_str = "this text will be hashed";
unsigned char* sha1_hash = sha1(text_str);
```
For a complete example, see [example.c](example.c).

Be sure that the `sha1.h` file is installed and that you include it with `#include <sha1.h>`.

## Test Information
For test details, see the [TESTS.md](TESTS.md) file.

## Use without install
You can use it without installing sha1.h if you are compiling on a machine without root permissions by placing sha1.h in your project directory and including it with #include "sha1.h" instead of #include <sha1.h>. However, installation is generally recommended.
