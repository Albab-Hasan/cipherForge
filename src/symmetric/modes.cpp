#include "cf/modes.hpp"
#include "cf/aes.hpp"

#include <algorithm>
#include <cstring>

namespace cf
{

  namespace detail
  {

    std::size_t pkcs7_pad(std::vector<uint8_t> &data)
    {
      const std::size_t pad_len = BlockSize - (data.size() % BlockSize);
      data.insert(data.end(), pad_len, static_cast<uint8_t>(pad_len));
      return pad_len;
    }

    void pkcs7_unpad(std::vector<uint8_t> &data)
    {
      if (data.empty() || (data.size() % BlockSize) != 0)
      {
        throw std::runtime_error("Invalid PKCS#7 padding: data length");
      }
      const uint8_t pad_len = data.back();
      if (pad_len == 0 || pad_len > BlockSize)
      {
        throw std::runtime_error("Invalid PKCS#7 padding length");
      }
      // Verify all padding bytes
      for (std::size_t i = 0; i < pad_len; ++i)
      {
        if (data[data.size() - 1 - i] != pad_len)
        {
          throw std::runtime_error("Invalid PKCS#7 padding content");
        }
      }
      data.resize(data.size() - pad_len);
    }

  } // namespace detail

  // -------------------- ECB --------------------
  std::vector<uint8_t> ecb_encrypt(const AES256 &cipher, std::span<const uint8_t> plaintext)
  {
    std::vector<uint8_t> buffer(plaintext.begin(), plaintext.end());
    detail::pkcs7_pad(buffer);

    std::vector<uint8_t> out(buffer.size());

    for (std::size_t offset = 0; offset < buffer.size(); offset += AES256::BlockSizeBytes)
    {
      std::span<const uint8_t, AES256::BlockSizeBytes> in_block(&buffer[offset], AES256::BlockSizeBytes);
      std::span<uint8_t, AES256::BlockSizeBytes> out_block(&out[offset], AES256::BlockSizeBytes);
      cipher.encrypt_block(in_block, out_block);
    }
    return out;
  }

  std::vector<uint8_t> ecb_decrypt(const AES256 &cipher, std::span<const uint8_t> ciphertext)
  {
    if (ciphertext.size() % AES256::BlockSizeBytes != 0)
    {
      throw std::runtime_error("Ciphertext length must be multiple of block size for ECB");
    }

    std::vector<uint8_t> buffer(ciphertext.begin(), ciphertext.end());
    for (std::size_t offset = 0; offset < buffer.size(); offset += AES256::BlockSizeBytes)
    {
      std::span<const uint8_t, AES256::BlockSizeBytes> in_block(&ciphertext[offset], AES256::BlockSizeBytes);
      std::span<uint8_t, AES256::BlockSizeBytes> out_block(&buffer[offset], AES256::BlockSizeBytes);
      cipher.decrypt_block(in_block, out_block);
    }

    detail::pkcs7_unpad(buffer);
    return buffer;
  }

  // -------------------- CBC --------------------
  std::vector<uint8_t> cbc_encrypt(const AES256 &cipher,
                                   std::span<const uint8_t, detail::BlockSize> iv,
                                   std::span<const uint8_t> plaintext)
  {
    std::vector<uint8_t> buffer(plaintext.begin(), plaintext.end());
    detail::pkcs7_pad(buffer);

    std::vector<uint8_t> out(buffer.size());

    std::array<uint8_t, detail::BlockSize> prev;
    std::copy(iv.begin(), iv.end(), prev.begin());

    for (std::size_t offset = 0; offset < buffer.size(); offset += detail::BlockSize)
    {
      // XOR with previous ciphertext (or IV for first block)
      for (std::size_t i = 0; i < detail::BlockSize; ++i)
      {
        buffer[offset + i] ^= prev[i];
      }

      std::span<const uint8_t, AES256::BlockSizeBytes> in_block(&buffer[offset], AES256::BlockSizeBytes);
      std::span<uint8_t, AES256::BlockSizeBytes> out_block(&out[offset], AES256::BlockSizeBytes);
      cipher.encrypt_block(in_block, out_block);

      // Update prev to newly generated ciphertext block
      std::copy(out_block.begin(), out_block.end(), prev.begin());
    }

    return out;
  }

  std::vector<uint8_t> cbc_decrypt(const AES256 &cipher,
                                   std::span<const uint8_t, detail::BlockSize> iv,
                                   std::span<const uint8_t> ciphertext)
  {
    if (ciphertext.size() % detail::BlockSize != 0)
    {
      throw std::runtime_error("Ciphertext length must be multiple of block size for CBC");
    }

    std::vector<uint8_t> out(ciphertext.size());
    std::array<uint8_t, detail::BlockSize> prev;
    std::copy(iv.begin(), iv.end(), prev.begin());

    for (std::size_t offset = 0; offset < ciphertext.size(); offset += detail::BlockSize)
    {
      std::array<uint8_t, detail::BlockSize> decrypted_block;
      std::span<const uint8_t, AES256::BlockSizeBytes> in_block(&ciphertext[offset], AES256::BlockSizeBytes);
      std::span<uint8_t, AES256::BlockSizeBytes> temp_block(decrypted_block.data(), AES256::BlockSizeBytes);
      cipher.decrypt_block(in_block, temp_block);

      // XOR with prev
      for (std::size_t i = 0; i < detail::BlockSize; ++i)
      {
        out[offset + i] = decrypted_block[i] ^ prev[i];
      }
      // Update prev to current ciphertext block
      std::copy(in_block.begin(), in_block.end(), prev.begin());
    }

    detail::pkcs7_unpad(out);
    return out;
  }

  // -------------------- CTR --------------------
  std::vector<uint8_t> ctr_encrypt(const AES256 &cipher,
                                   std::span<const uint8_t, detail::BlockSize> iv,
                                   std::span<const uint8_t> plaintext)
  {
    std::vector<uint8_t> out(plaintext.size());
    std::array<uint8_t, detail::BlockSize> counter;
    std::copy(iv.begin(), iv.end(), counter.begin());

    for (std::size_t offset = 0; offset < plaintext.size();)
    {
      // Encrypt the counter
      std::array<uint8_t, detail::BlockSize> encrypted_counter;
      cipher.encrypt_block(counter, encrypted_counter);

      // XOR plaintext with encrypted counter
      std::size_t remaining = std::min(detail::BlockSize, plaintext.size() - offset);
      for (std::size_t i = 0; i < remaining; ++i)
      {
        out[offset + i] = plaintext[offset + i] ^ encrypted_counter[i];
      }

      offset += remaining;

      // Increment counter (big-endian)
      for (int i = detail::BlockSize - 1; i >= 0; --i)
      {
        if (++counter[i] != 0)
          break;
      }
    }

    return out;
  }

  // CTR mode decryption is identical to encryption due to XOR properties
  std::vector<uint8_t> ctr_decrypt(const AES256 &cipher,
                                   std::span<const uint8_t, detail::BlockSize> iv,
                                   std::span<const uint8_t> ciphertext)
  {
    // CTR mode decryption is the same as encryption
    return ctr_encrypt(cipher, iv, ciphertext);
  }

} // namespace cf
