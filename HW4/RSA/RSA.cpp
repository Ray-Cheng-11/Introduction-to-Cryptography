#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <iomanip>

#include "cryptopp/rsa.h"
#include "cryptopp/misc.h"
#include "cryptopp/osrng.h"

struct Encrypt
{
  int key_length;
  CryptoPP::Integer n;
  CryptoPP::Integer e;
  std::string plaintext;

  Encrypt(int _key_length,
          const CryptoPP::Integer &_n,
          const CryptoPP::Integer &_e,
          const std::string &_plaintext) : key_length(_key_length),
                                           n(_n),
                                           e(_e),
                                           plaintext(_plaintext) {}
};

struct Decrypt
{
  int key_length;
  CryptoPP::Integer n;
  CryptoPP::Integer d;
  std::string ciphertext;

  Decrypt(int _key_length,
          const CryptoPP::Integer &_n,
          const CryptoPP::Integer &_d,
          const std::string &_ciphertext) : key_length(_key_length),
                                            n(_n),
                                            d(_d),
                                            ciphertext(_ciphertext) {}
};

std::string integer_to_string(const CryptoPP::Integer &x)
{
  std::ostringstream oss;
  oss << std::uppercase << std::hex << x;
  std::string result = oss.str();

  // Remove the trailing 'h' character if present
  if (!result.empty() && result.back() == 'h')
  {
    result.pop_back();
  }

  return result;
}
std::string hex_string_conversion(const std::string &hex_string)
{
  std::string result;
  for (int i = 0; i < hex_string.length(); i += 2)
  {
    result += (char)stoul(hex_string.substr(i, 2), nullptr, 16);
  }
  return result;
}
std::string rsa_encrypt(const Encrypt &rsa_info)
{
  CryptoPP::RSA::PublicKey pubkey;
  pubkey.Initialize(rsa_info.n, rsa_info.e);

  CryptoPP::Integer plaintext(
      reinterpret_cast<const CryptoPP::byte *>(rsa_info.plaintext.data()),
      rsa_info.plaintext.size());

  assert(plaintext.BitCount() < rsa_info.n.BitCount());

  CryptoPP::Integer ciphertext = pubkey.ApplyFunction(plaintext);

  return integer_to_string(ciphertext);
}

/*std::string brute(const Decrypt &rsa_info)
{

  CryptoPP::Integer d = rsa_info.partial_d;
  CryptoPP::AutoSeededRandomPool prng;

  while (true)
  {
    try
    {
      CryptoPP::RSA::PrivateKey privkey;
      privkey.Initialize(rsa_info.n, rsa_info.e, d);

      CryptoPP::Integer plaintext = privkey.CalculateInverse(
          prng,
          rsa_info.ciphertext);

      assert(plaintext.BitCount() < rsa_info.n.BitCount());

      return {CryptoPP::IntToString(d, 16), integer_to_string(plaintext)};
    }
    catch (...)
    {
      d++;
    }
  }
}*/

std::string rsa_decrypt(const Decrypt &rsa_info)
{
  CryptoPP::RSA::PublicKey public_key;
  std::string message;
  std::string cipher = hex_string_conversion(rsa_info.ciphertext);
  CryptoPP::Integer n = CryptoPP::Integer(rsa_info.n);
  CryptoPP::Integer d = CryptoPP::Integer(rsa_info.d);
  public_key.Initialize(n, d);
  CryptoPP::Integer r = public_key.ApplyFunction(CryptoPP::Integer((const CryptoPP::byte *)cipher.c_str(), cipher.size()));
  message.resize(r.MinEncodedSize());
  r.Encode((CryptoPP::byte *)message.data(), message.size());
  return message;
}

int main()
{
  int bit;
  std::string op, modulus_string, exponent_string, message;
  // Read the entire input line
  std::string input_line;
  std::getline(std::cin, input_line);
  // Parse the input line
  std::istringstream iss(input_line);
  iss >> op >> bit >> modulus_string >> exponent_string;
  // Read the message which can contain spaces
  std::getline(iss >> std::ws, message);

  std::string modulus = "0x" + modulus_string;
  std::string exponent = "0x" + exponent_string;
  if (op == "enc")
  {
    Encrypt case_1(bit, CryptoPP::Integer(modulus.data()), CryptoPP::Integer(exponent.data()), message);
    std::string case_1_1 = rsa_encrypt(case_1);
    std::cout << case_1_1 << std::endl;
  }
  else if (op == "dec")
  {

    Decrypt case_2(bit, CryptoPP::Integer(modulus.c_str()), CryptoPP::Integer(exponent.c_str()), message);
    std::string case_2_1 = rsa_decrypt(case_2);
    std::cout << std::hex << case_2_1 << std::endl;
  }

  return 0;
}
