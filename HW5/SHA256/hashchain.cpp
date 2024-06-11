#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <fstream>

using namespace std;
using namespace CryptoPP;

inline const string toHex(const string str)
{
  string hex;
  HexEncoder encoder;
  encoder.Attach(new StringSink(hex));
  encoder.Put((const CryptoPP::byte *)str.c_str(), str.size());
  encoder.MessageEnd();
  return hex;
}

int main()
{
  ofstream out;
  out.open("out2.txt", ios::out);
  string outputHash, nowHash(32, 0), msg = "111550012", prevHash, nonce(4, 0);
  SHA256 hash;
  hash.Update((const CryptoPP::byte *)msg.data(), msg.size());
  hash.Final((CryptoPP::byte *)&nowHash[0]);
  int i, j;
  unsigned int n = 0;
  for (i = 0; i < 64; i++)
  {
    n = 0;
    prevHash = nowHash;
    while (1)
    {
      for (j = 0; j < 4; j++)
        nonce[j] = (char)((n >> (8 * (3 - j))));
      msg = prevHash + nonce;
      hash.Update((const CryptoPP::byte *)msg.data(), 36);
      hash.Final((CryptoPP::byte *)&nowHash[0]);
      outputHash = toHex(nowHash);
      n++;
      if (n == 0)
        break;
      for (j = 0; outputHash.c_str()[j] == '0'; j++)
        ;
      if (i == j)
        break;
    }
    out << i << '\n'
        << toHex(prevHash) << '\n';
    out << toHex(nonce) << '\n'
        << outputHash << endl;
    if (n == 0)
      break;
  }
  out << "Fin";
  return 0;
}
