#pragma once

#include <array>
#include <cstdint>
#include <span>

namespace cf {

class AES256 final {
public:
    static constexpr std::size_t KeySizeBytes = 32;   // 256 bits
    static constexpr std::size_t BlockSizeBytes = 16; // 128 bits

    explicit AES256(std::span<const uint8_t, KeySizeBytes> key) noexcept;

    // Encrypt a single 16-byte block.
    void encrypt_block(std::span<const uint8_t, BlockSizeBytes> in,
                       std::span<uint8_t, BlockSizeBytes> out) const noexcept;

    // Decrypt a single 16-byte block.
    void decrypt_block(std::span<const uint8_t, BlockSizeBytes> in,
                       std::span<uint8_t, BlockSizeBytes> out) const noexcept;

    // Wipe sensitive material from memory.
    ~AES256();

private:
    std::array<uint32_t, 60> roundKeys_{}; // 4*(Nr+1) where Nr=14

    static uint8_t sub_byte(uint8_t x) noexcept;
    static uint8_t inv_sub_byte(uint8_t x) noexcept;
    static void sub_bytes(uint8_t state[4][4]) noexcept;
    static void inv_sub_bytes(uint8_t state[4][4]) noexcept;
    static void shift_rows(uint8_t state[4][4]) noexcept;
    static void inv_shift_rows(uint8_t state[4][4]) noexcept;
    static void mix_columns(uint8_t state[4][4]) noexcept;
    static void inv_mix_columns(uint8_t state[4][4]) noexcept;

    void add_round_key(uint8_t state[4][4], std::size_t round) const noexcept;

    void expand_key(std::span<const uint8_t, KeySizeBytes> key) noexcept;
};

} // namespace cf
