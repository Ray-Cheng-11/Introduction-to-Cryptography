#include <iostream>
#include <sstream>
#include <cassert>

#include "cryptopp/rsa.h"
#include "cryptopp/misc.h"
#include "cryptopp/osrng.h"
struct Encrypt
{
  int key_length;
  CryptoPP::Integer n;
  CryptoPP::Integer e;
  CryptoPP::Integer plaintext;

  Encrypt(int _key_length,
          const CryptoPP::Integer &_n,
          const CryptoPP::Integer &_e,
          const CryptoPP::Integer &_plaintext) : key_length(_key_length),
                                                 n(_n),
                                                 e(_e),
                                                 plaintext(_plaintext) {}
};
std::string integer_to_string(const CryptoPP::Integer &x)
{
  return x % 2 == 0 ? "0" : "1";
}
void rsa_encrypt(const Encrypt &rsa_info)
{
  CryptoPP::RSA::PublicKey pubkey;
  pubkey.Initialize(rsa_info.n, rsa_info.e);
  CryptoPP::Integer plaintext = rsa_info.plaintext;
  for (int i = 0; i < 32; i++)
  {
    if (i < 31)
    {
      CryptoPP::Integer ciphertext = pubkey.ApplyFunction(plaintext);
      std::cout << integer_to_string(ciphertext);
      plaintext = ciphertext;
    }
    else
    {
      CryptoPP::Integer ciphertext = pubkey.ApplyFunction(plaintext);
      std::cout << integer_to_string(ciphertext) << std::endl;
    }
  }
}

int main()
{
  int bit;
  std::string modulus_string, exponent_string, message_string;
  std::string input_line;
  std::getline(std::cin, input_line);
  std::istringstream iss(input_line);
  iss >> bit >> modulus_string >> exponent_string;
  std::getline(iss >> std::ws, message_string);

  std::string modulus = "0x" + modulus_string;
  std::string exponent = "0x" + exponent_string;
  std::string message = "0x" + message_string;

  Encrypt case_1(bit, CryptoPP::Integer(modulus.data()), CryptoPP::Integer(exponent.data()), CryptoPP::Integer(message.c_str()));
  rsa_encrypt(case_1);

  return 0;
}
