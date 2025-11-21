# libSHA1 TESTS

| OS                    | ARCH    | ENDIAN | COMPILER     | STATUS | NOTES                            |
|-----------------------|---------|--------|--------------|--------|----------------------------------|
| AlpineLinux 3.21      | x86_64  | LITTLE | GCC 14.2.0   | Pass   |                                  |
| Termux (on Android 9) | aarch64 | LITTLE | clang 21.1.4 | Pass   |                                  |
| AlpineLinux 3.22      | x86     | LITTLE | GCC 14.2.0   | Pass   |                                  |
| AlpineLinux 3.22      | s390x   | BIG    | GCC 14.2.0   | Pass   | Updated To Autodetect Endianness |

## Arch Name Conflicts
+ x86_64 = amd64
+ x86 = IA-32
