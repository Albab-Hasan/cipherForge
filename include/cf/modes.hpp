#pragma once

#include <vector>
#include <span>
#include <array>
#include <cstdint>
#include <stdexcept>

namespace cf
{

  // Forward declaration of AES256 (or any block cipher with 16-byte block) to avoid circular include.
  class AES256;

  // PKCS#7 padding helpers
  namespace detail
  {
    constexpr std::size_t BlockSize = 16;

    // Append PKCS#7 padding so that data size becomes multiple of BlockSize.
    // Returns number of padding bytes appended.
    std::size_t pkcs7_pad(std::vector<uint8_t> &data);

    // Remove PKCS#7 padding from data. Throws std::runtime_error if padding is invalid.
    void pkcs7_unpad(std::vector<uint8_t> &data);
  } // namespace detail

  // Electronic Codebook (ECB) mode with PKCS#7 padding.
  std::vector<uint8_t> ecb_encrypt(const AES256 &cipher, std::span<const uint8_t> plaintext);
  std::vector<uint8_t> ecb_decrypt(const AES256 &cipher, std::span<const uint8_t> ciphertext);

  // Cipher Block Chaining (CBC) mode with PKCS#7 padding.
  // iv must be exactly 16 bytes (AES block size).
  std::vector<uint8_t> cbc_encrypt(const AES256 &cipher,
                                   std::span<const uint8_t, detail::BlockSize> iv,
                                   std::span<const uint8_t> plaintext);

  std::vector<uint8_t> cbc_decrypt(const AES256 &cipher,
                                   std::span<const uint8_t, detail::BlockSize> iv,
                                   std::span<const uint8_t> ciphertext);

  // Counter (CTR) mode encryption/decryption
  // iv must be exactly 16 bytes (AES block size)
  std::vector<uint8_t> ctr_encrypt(const AES256 &cipher,
                                   std::span<const uint8_t, detail::BlockSize> iv,
                                   std::span<const uint8_t> plaintext);

  std::vector<uint8_t> ctr_decrypt(const AES256 &cipher,
                                   std::span<const uint8_t, detail::BlockSize> iv,
                                   std::span<const uint8_t> ciphertext);

} // namespace cf
