#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <hex.h>
#include "modes.h"
#include "aes.h"
#include "filters.h"

using namespace std;
using namespace CryptoPP;

bool isReadable(const string &str)
{
  for (char c : str)
  {
    if (!(isalnum(c) || isspace(c) || c == ',' || c == '.'))
    {
      return false;
    }
  }
  return true;
}

void decode(string ciphertext)
{
  const byte iv[] = "0000000000000000";
  byte key[] = "Our key is: XXXX";
  string plaintext;
  HexDecoder decoder;
  decoder.Attach(new StringSink(plaintext));
  decoder.Put((const byte *)ciphertext.data(), ciphertext.size());
  decoder.MessageEnd();

  for (int feedbackSize : {2, 4, 8})
  {
    for (int i = 0; i <= 0xFFFF; i++)
    {
      snprintf((char *)key + 12, 5, "%04X", i);

      const AlgorithmParameters cfbP = MakeParameters(Name::FeedbackSize(), feedbackSize)(Name::IV(), ConstByteArrayParameter(iv, 16));
      string output;
      CFB_Mode<AES>::Decryption decrypt;
      decrypt.SetKey(key, sizeof(key) - 1, cfbP);
      try
      {
        CryptoPP::StreamTransformationFilter decryptFilter(decrypt, new CryptoPP::StringSink(output));
        decryptFilter.Put(reinterpret_cast<const unsigned char *>(plaintext.c_str()), plaintext.length());
        decryptFilter.MessageEnd();

        if (isReadable(output))
        {
          cout << output << "\n";
          return;
        }
      }
      catch (const CryptoPP::Exception &e)
      {
        continue;
      }
    }
  }
}

int main()
{
  string ciphertext;
  getline(cin, ciphertext);
  decode(ciphertext);
  return 0;
}
