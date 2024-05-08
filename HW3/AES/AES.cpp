#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>

using namespace std;

string hexToString(const string &hex)
{
  string result;
  for (size_t i = 0; i < hex.length(); i += 2)
  {
    std::string byteString = hex.substr(i, 2);
    char byte = (char)strtol(byteString.c_str(), nullptr, 16);
    result += byte;
  }
  return result;
}

void print_text(string &str)
{
  for (int i = 0; i < (int)str.size(); i++)
  {
    printf("%02X", static_cast<unsigned char>(str[i]));
  }
  cout << endl;
}

bool isReadable(const string &str)
{
  for (char c : str)
  {
    if (!isdigit(c) && !isalpha(c) && !isspace(c))
    {
      return false;
    }
  }
  return true;
}

void stringTobyteArray(string &string, CryptoPP::byte arr[], int size = CryptoPP::AES::DEFAULT_KEYLENGTH)
{
  for (int i = 0; i < size; i++)
  {
    arr[i] = string[i];
  }
}

string ECB(string &plaintext, CryptoPP::byte key[])
{
  using namespace CryptoPP;

  string ciphertext;

  try
  {
    ECB_Mode<AES>::Encryption e;
    e.SetKey(key, AES::DEFAULT_KEYLENGTH);

    StringSource en(plaintext, true,
                    new StreamTransformationFilter(e, new StringSink(ciphertext), BlockPaddingSchemeDef::PKCS_PADDING));
  }
  catch (const Exception &e)
  {
    cerr << "Error: " << e.what() << endl;
    exit(1);
  }
  return ciphertext;
}

string CBC(string &plaintext, CryptoPP::byte key[], CryptoPP::byte IV[], string padding)
{
  using namespace CryptoPP;

  string ciphertext;

  try
  {
    CBC_Mode<AES>::Encryption e;
    e.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, IV);

    if (padding == "ONE_AND_ZEROS")
    {
      StringSource en(plaintext, true,
                      new StreamTransformationFilter(e, new StringSink(ciphertext), BlockPaddingSchemeDef::ONE_AND_ZEROS_PADDING));
    }
    else
    {
      StringSource en(plaintext, true,
                      new StreamTransformationFilter(e, new StringSink(ciphertext), BlockPaddingSchemeDef::PKCS_PADDING));
    }
  }
  catch (const Exception &e)
  {
    cerr << "Error: " << e.what() << endl;
    exit(1);
  }
  return ciphertext;
}

string CFB(string &plaintext, CryptoPP::byte key[], CryptoPP::byte IV[], int feedback)
{
  using namespace CryptoPP;

  string ciphertext;

  try
  {
    // const AlgorithmParameters parameter = MakeParameters(Name::FeedbackSize(), 2)(Name::IV(), ConstByteArrayParameter(IV, 16));
    CFB_Mode<AES>::Encryption e(key, AES::DEFAULT_KEYLENGTH, IV, feedback);

    StringSource en(plaintext, true,
                    new StreamTransformationFilter(e, new StringSink(ciphertext)));
  }
  catch (const Exception &e)
  {
    cerr << "Error: " << e.what() << endl;
    exit(1);
  }
  return ciphertext;
}

void test1(string &plaintext)
{
  // string plaintext = "Hello World!";
  string keytext = "2357111317192329";
  // string IVtext = "0000000000000000";
  string iv1 = "1234567812345678";
  string iv2 = "9999999999999999";
  CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
  CryptoPP::byte IV[CryptoPP::AES::DEFAULT_KEYLENGTH];
  CryptoPP::byte IV2[CryptoPP::AES::DEFAULT_KEYLENGTH];

  stringTobyteArray(keytext, key);
  stringTobyteArray(iv1, IV);
  stringTobyteArray(iv2, IV2);

  string ECB_result = ECB(plaintext, key);
  string CBC_result = CBC(plaintext, key, IV, "ONE_AND_ZEROS");
  string CFB_result = CFB(plaintext, key, IV2, 2);

  print_text(ECB_result);
  print_text(CBC_result);
  print_text(CFB_result);
}
void bruteForceKey(string interceptedCiphertext)
{
  for (int i = 0; i <= 99999; ++i)
  {
    // Create key in the required format
    ostringstream oss;
    oss << setw(5) << setfill('0') << i;
    string suffix = oss.str();
    string Key = "00000000000" + suffix;

    std::string decrypted;
    try
    {
      CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decrypt;
      decrypt.SetKey((CryptoPP::byte *)Key.data(), Key.size());

      // decrypt
      CryptoPP::StringSource de(interceptedCiphertext, true, new CryptoPP::StreamTransformationFilter(decrypt, new CryptoPP::StringSink(decrypted), CryptoPP::BlockPaddingSchemeDef::PKCS_PADDING));
      if (isReadable(decrypted))
      {
        cout << Key << "\n";
        cout << decrypted << "\n";
        break;
      }
    }
    catch (CryptoPP::Exception &e)
    {
      // Error occurred, continue to next key
    }
  }
}
int main()
{
  string str = "AES is the US block cipher standard.";
  string interceptedCiphertextHex = "104839DE2B34D9BA96F6E054F79F865890B827381D22FC3388690794F0D08EB3";

  string interceptedCiphertext = hexToString(interceptedCiphertextHex);
  test1(str);
  bruteForceKey(interceptedCiphertext);

  string message;
  getline(cin, message);
  test1(message);

  string text;
  getline(cin, text);
  string sEncData = hexToString(text);
  bruteForceKey(sEncData);

  return 0;
}
