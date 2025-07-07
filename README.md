# cipherForge

A place where I'm teaching myself real cryptography by writing everything from scratch. Just me, the specs, and a lot of "why doesn't this compile?" moments.

## Why bother?

1. I learn best by building stuff. Turning the AES spec into working C++ feels more useful than skimming blog posts.
2. Having a public repo forces me to keep the code readable (or at least not embarrassing).
3. It's nice to have a personal toolkit for experiments and demos.

## Roadmap / progress

| Phase | Task | Status |
|-------|------|--------|
| 1 | AES-256 (ECB) + test vector | done |
| 1 | CBC / CTR / GCM modes | in progress |
| 1 | ChaCha20 | todo |
| 1 | SHA-256, SHA-3, HMAC | todo |
| 2 | RSA (keygen, encrypt, sign) | todo |
| 2 | ECC (Curve25519, secp256k1) | todo |
| 3 | Custom secure messaging protocol | todo |
| 4 | Cryptanalysis tools | todo |

The table will grow (and hopefully turn greener) as I tick things off.

## Build instructions

```bash
# Linux / macOS
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
ctest --output-on-failure
```

CMake downloads GoogleTest automatically; you don't need anything else besides a C++17 compiler.

## Contributing / feedback

Spot a bug or something that looks suspiciously slow? File an issue or open a pull request. I'm still learning, so explanations are welcome.

## Disclaimer

This code is for study and experimentation. Don't rely on it for production security.
