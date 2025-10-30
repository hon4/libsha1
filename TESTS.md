# hon's libSHA1 Tests

## Compiler Tests
### OS: Linux
| Compiler | Version | Status | Full OS               | CPU     | Notes |
|----------|---------|--------|-----------------------|---------|-------|
| GCC      |  14.2.0 | Pass   | AlpineLinux 3.21.2    | amd64   |       |
| clang    |  21.1.4 | Warn   | Termux (on Android 9) | aarch64 | >> bitshift |
| TCC      |  0.9.27 | Pass   | Termux (on Android 9) | aarch64 |       |

### OS: Windows
| Compiler | Version | Status | Full OS                  | CPU   | Notes |
|----------|---------|--------|--------------------------|-------|-------|
| Dev-C++  | 4.9.9.2 | Fail   | Windows XP Pro SP3 32bit | IA-32 | for loops used outside C99 |


## Notes
### CPU Architectures (also known as)
| Name    | a.k.a.                |
|---------|-----------------------|
| `amd64` | `x86_64`, `x86 64bit` |
| `IA-32` | `x86`, `x86 32bit` |
- `aarch64`: `arm64`
