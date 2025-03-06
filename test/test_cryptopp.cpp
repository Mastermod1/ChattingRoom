#include <gtest/gtest.h>
#include <crypt.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/secblock.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

TEST(Crypt, Encrypt)
{
    using namespace CryptoPP;

    std::string plaintext = "Hello World";
    std::string ciphertext, decryptedtext;

    // Generate a random key and IV
    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);

    AutoSeededRandomPool prng;
    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    // Encrypt
    try {
        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV(key, key.size(), iv);

        StringSource(plaintext, true,
            new StreamTransformationFilter(encryption,
                new StringSink(ciphertext)
            )
        );
    } catch (const Exception &e) {
        std::cerr << "Encryption error: " << e.what() << std::endl;
    }

    // Decrypt
    try {
        CBC_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV(key, key.size(), iv);

        StringSource(ciphertext, true,
            new StreamTransformationFilter(decryption,
                new StringSink(decryptedtext)
            )
        );
    } catch (const Exception &e) {
        std::cerr << "Decryption error: " << e.what() << std::endl;
    }

    EXPECT_NE(ciphertext, plaintext);
    EXPECT_EQ(decryptedtext, plaintext);
}
