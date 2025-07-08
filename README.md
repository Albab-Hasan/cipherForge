# CipherForge

A personal playground for understanding cryptography by building from scratch. Expect messy commits, learning curves, and probably a few spectacular bugs.

## Why Am I Doing This?

- I'm tired of copy-pasting crypto code without understanding it
- Academic papers are great, but nothing beats implementation
- Learning how things *actually* work under the hood

## Current State of Chaos 🚧

| Phase | Task | Status | Pain Level |
|-------|------|--------|------------|
| 1 | AES-256 (ECB) | done | 😅 Survived |
| 1 | CBC / CTR modes | mostly working | 🤯 Brain melting |
| 1 | ChaCha20 | staring at spec | 😱 Intimidating |
| 1 | SHA-256 | sketched out | 🤔 Thinking hard |
| 2 | RSA | dreaming about it | 💭 Wishful thinking |
| 3 | Secure messaging | distant future | 🌈 Optimistic |

## Build Instructions (Hopefully)

```bash
# Pray to the compiler gods
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run tests (aka see what broke)
ctest --output-on-failure
```

## Contributing / Sanity Checks

- Spot a bug? You're probably right.
- Want to improve something? Please do!
- Cryptography experts: Be gentle, I'm learning 🙏

## Disclaimer

**SERIOUSLY:** This is a learning project. Do NOT use this for anything serious.
I repeat: DO. NOT. USE. IN. PRODUCTION.

Seriously. I mean it. 🚨

## Learning Resources That Saved Me

- NIST FIPS 197 (AES Spec)
- "Understanding Cryptography" by Paar & Pelzl
- Countless StackOverflow rabbit holes
- Coffee ☕ and patience 🧘‍♀️
