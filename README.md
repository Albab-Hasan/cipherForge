# CipherForge

A personal playground for understanding cryptography by building from scratch. Expect messy commits, learning curves, and probably a few lot of bugs.

## Why Am I Doing This?

- I'm tired of copy-pasting crypto code without understanding it
- Academic papers are great, but nothing beats implementation
- Learning how things work under the hood

## Current State of the project

### Crypto Progress Log

| Phase | Task               | Status          | Notes                                                                 |
|-------|--------------------|-----------------|-----------------------------------------------------------------------|
| 1     | AES-256 (ECB)      | done            | Basic mode, got it working.          |
| 1     | CBC / CTR modes    | mostly working  | More complex, but making progress. Still debugging edge cases.       |
| 1     | ChaCha20           | reading spec    | Looks cool but kinda dense. Need more time.                          |
| 1     | SHA-256            | rough draft     | Started writing it out. Still wrapping my head around the padding.  |
| 2     | RSA                | thinking about it | Haven’t started, just reading how key gen works.                    |
| 3     | Secure messaging   | not started     | Eventually want to build something like Signal. Long way to go.      |


## Build Instructions

```bash
# Pray to the compiler gods
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run tests (aka see what broke)
ctest --output-on-failure
```

## Contributing

- Spot a bug? You're probably right.
- Want to improve something? Please do!
- Cryptography experts: Be kind pls.

## Disclaimer

This is a learning project. Do NOT use this for anything serious.

## Learning Resources That Helped Me

- NIST FIPS 197 (AES Spec)
- "Understanding Cryptography" by Paar & Pelzl
- Countless StackOverflow rabbit holes
