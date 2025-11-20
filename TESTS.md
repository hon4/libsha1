# libSHA1 TESTS

| OS                    | ARCH    | STATUS | NOTES                                                                                                                                        |
|-----------------------|---------|--------|----------------------------------------------------------------------------------------------------------------------------------------------|
| AlpineLinux 3.21      | x86_64  | Pass   |                                                                                                                                              |
| Termux (on Android 9) | aarch64 | Pass   |                                                                                                                                              |
| AlpineLinux 3.22      | x86     | Pass   |                                                                                                                                              |
| AlpineLinux 3.22      | s390x   | Fail   | Endianess error. Works with macro change from `#define __LIBSHA1_USE_ENDIANNESS__ 1` to `#define __LIBSHA1_USE_ENDIANNESS__ 0` in src/sha1.c |

## Arch Name Conflicts
+ x86_64 = amd64
+ x86 = IA-32
