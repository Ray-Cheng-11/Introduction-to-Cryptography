#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
using namespace std;

class DES
{
private:
  int initial_permutation[64] = {58, 50, 42, 34, 26, 18, 10, 2,
                                 60, 52, 44, 36, 28, 20, 12, 4,
                                 62, 54, 46, 38, 30, 22, 14, 6,
                                 64, 56, 48, 40, 32, 24, 16, 8,
                                 57, 49, 41, 33, 25, 17, 9, 1,
                                 59, 51, 43, 35, 27, 19, 11, 3,
                                 61, 53, 45, 37, 29, 21, 13, 5,
                                 63, 55, 47, 39, 31, 23, 15, 7};

  int final_permutation[64] = {40, 8, 48, 16, 56, 24, 64, 32,
                               39, 7, 47, 15, 55, 23, 63, 31,
                               38, 6, 46, 14, 54, 22, 62, 30,
                               37, 5, 45, 13, 53, 21, 61, 29,
                               36, 4, 44, 12, 52, 20, 60, 28,
                               35, 3, 43, 11, 51, 19, 59, 27,
                               34, 2, 42, 10, 50, 18, 58, 26,
                               33, 1, 41, 9, 49, 17, 57, 25};

  int expansion_permutation[48] = {32, 1, 2, 3, 4, 5,
                                   4, 5, 6, 7, 8, 9,
                                   8, 9, 10, 11, 12, 13,
                                   12, 13, 14, 15, 16, 17,
                                   16, 17, 18, 19, 20, 21,
                                   20, 21, 22, 23, 24, 25,
                                   24, 25, 26, 27, 28, 29,
                                   28, 29, 30, 31, 32, 1};

  int s_boxes[8][4][16] = {{{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
                            {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
                            {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
                            {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}},

                           {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
                            {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
                            {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
                            {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},

                           {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
                            {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
                            {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
                            {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},

                           {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
                            {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
                            {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
                            {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},

                           {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
                            {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
                            {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
                            {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},

                           {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
                            {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
                            {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
                            {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},

                           {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
                            {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
                            {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
                            {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},

                           {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
                            {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
                            {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
                            {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}}};

  int permutation[32] = {16, 7, 20, 21, 29, 12, 28, 17,
                         1, 15, 23, 26, 5, 18, 31, 10,
                         2, 8, 24, 14, 32, 27, 3, 9,
                         19, 13, 30, 6, 22, 11, 4, 25};

  int key_permutation[56] = {57, 49, 41, 33, 25, 17, 9,
                             1, 58, 50, 42, 34, 26, 18,
                             10, 2, 59, 51, 43, 35, 27,
                             19, 11, 3, 60, 52, 44, 36,
                             63, 55, 47, 39, 31, 23, 15,
                             7, 62, 54, 46, 38, 30, 22,
                             14, 6, 61, 53, 45, 37, 29,
                             21, 13, 5, 28, 20, 12, 4};

  int key_compression[48] = {14, 17, 11, 24, 1, 5, 3, 28,
                             15, 6, 21, 10, 23, 19, 12, 4,
                             26, 8, 16, 7, 27, 20, 13, 2,
                             41, 52, 31, 37, 47, 55, 30, 40,
                             51, 45, 33, 48, 44, 49, 39, 56,
                             34, 53, 46, 42, 50, 36, 29, 32};

  int shift_table[16] = {2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1};

  // Initial permutation
  void initialPermutation(uint8_t *plaintext)
  {
    uint64_t result = 0;
    for (int i = 0; i < 64; i++)
    {
      int origin_pos = initial_permutation[i] - 1;
      int origin_idx = origin_pos / 8;
      int origin_bit = 7 - origin_pos % 8;
      uint64_t bit = (plaintext[origin_idx] >> origin_bit) & 0x01;
      result = result | (bit << (63 - i));
    }

    for (int i = 0; i < 8; i++)
    {
      plaintext[i] = (result >> (56 - i * 8)) & 0xFF;
    }
  }

  // Key Permutation
  void keyPermutation(uint8_t *key)
  {
    uint64_t result = 0;

    for (int i = 0; i < 56; i++)
    {
      int origin_pos = key_permutation[i] - 1;
      int origin_idx = origin_pos / 8;
      int origin_bit = 7 - origin_pos % 8;
      uint64_t bit = (key[origin_idx] >> origin_bit) & 0x01;
      result = result | (bit << (55 - i));
    }

    for (int i = 0; i < 8; i++)
    {
      key[i] = (result >> (49 - i * 7)) & 0x7F;
    }
  }
  uint32_t left_rotate(uint32_t value, int shift)
  {
    return ((value << shift) | (value >> (28 - shift))) & ((1 << 28) - 1);
  }
  // key scheduling
  void key_schedule(uint8_t *key, int round)
  {
    uint32_t leftkey = (key[0] << 21) | (key[1] << 14) | (key[2] << 7) | key[3];
    uint32_t rightkey = (key[4] << 21) | (key[5] << 14) | (key[6] << 7) | key[7];

    leftkey = left_rotate(leftkey, shift_table[round]);
    rightkey = left_rotate(rightkey, shift_table[round]);

    key[0] = (leftkey >> 21) & 0x7F;
    key[1] = (leftkey >> 14) & 0x7F;
    key[2] = (leftkey >> 7) & 0x7F;
    key[3] = leftkey & 0x7F;
    key[4] = (rightkey >> 21) & 0x7F;
    key[5] = (rightkey >> 14) & 0x7F;
    key[6] = (rightkey >> 7) & 0x7F;
    key[7] = rightkey & 0x7F;
  }
  // Key compression
  void keyCompression(uint8_t *key, uint8_t *subkey)
  {
    uint64_t result = 0;

    for (int i = 0; i < 48; i++)
    {
      int origin_pos = key_compression[i] - 1;
      int origin_idx = origin_pos / 7;
      int origin_bit = 6 - origin_pos % 7;
      uint64_t bit = (key[origin_idx] >> origin_bit) & 0x01;
      result = result | (bit << (47 - i));
    }

    for (int i = 0; i < 8; i++)
    {
      subkey[i] = (result >> (42 - i * 6)) & 0x3F;
    }
  }

  // Expansion permutation
  void expansion(uint32_t half_block, uint8_t *exp)
  {
    uint64_t result = 0;

    for (int i = 0; i < 48; i++)
    {
      int origin_pos = expansion_permutation[i] - 1;
      uint64_t bit = (half_block >> (31 - origin_pos)) & 0x01;
      result = result | (bit << (47 - i));
    }

    for (int i = 0; i < 8; i++)
    {
      exp[i] = (result >> (42 - i * 6)) & 0x3F;
    }
  }

  // S-box substitution
  void s_box(uint8_t *exp)
  {
    for (int i = 0; i < 8; i++)
    {
      int row_idx = ((exp[i] & 0x20) >> 4) | (exp[i] & 0x01);
      int col_idx = (exp[i] & 0x1E) >> 1;

      if (i == 1)
      {
        exp[i] = (2 * ((exp[i] >> 5) & 0x01) + 3 * ((exp[i] >> 4) & 0x01) + 5 * ((exp[i] >> 3) & 0x01) + 7 * ((exp[i] >> 2) & 0x01) + 11 * ((exp[i] >> 1) & 0x01) + 13 * (exp[i] & 0x01)) % 16;
      }
      else
      {
        exp[i] = s_boxes[i][row_idx][col_idx];
      }
    }
  }

  // P-box permutation

  uint32_t Permutation(uint8_t *box)
  {
    uint32_t result = 0;

    for (int i = 0; i < 32; i++)
    {
      int origin_pos = permutation[i] - 1;
      int origin_idx = origin_pos / 4;
      int origin_bit = 3 - origin_pos % 4;
      uint32_t bit = (box[origin_idx] >> origin_bit) & 0x01;
      result = result | (bit << (31 - i));
    }

    return result;
  }

  void finalPermutation(uint64_t swap, uint8_t *ciphertext)
  {
    uint64_t res = 0;
    for (int i = 0; i < 64; i++)
    {
      uint64_t bit = (swap >> (64 - final_permutation[i])) & 1;
      res = res | (bit << (63 - i));
    }

    for (int i = 0; i < 8; i++)
    {
      ciphertext[i] = (res >> (56 - i * 8)) & 0xFF;
    }
  }

  void XOR(uint8_t *e, uint8_t *subkey)
  {
    for (int i = 0; i < 8; i++)
    {
      e[i] = e[i] ^ subkey[i];
    }
  }

public:
  void Encrypt(uint8_t *plaintext, uint8_t *key, uint8_t *ciphertext)
  {
    initialPermutation(plaintext);
    keyPermutation(key);

    uint32_t L = (plaintext[0] << 24) | (plaintext[1] << 16) | (plaintext[2] << 8) | (plaintext[3]);
    uint32_t R = (plaintext[4] << 24) | (plaintext[5] << 16) | (plaintext[6] << 8) | (plaintext[7]);
    for (int round = 0; round < 16; round++)
    {

      uint8_t e[8];
      uint8_t subkey[8];
      key_schedule(key, round);
      keyCompression(key, subkey);
      expansion(R, e);
      XOR(e, subkey);
      s_box(e);

      uint32_t F_result = Permutation(e);
      uint32_t tmp = R;
      R = F_result ^ L;
      L = tmp;
    }

    finalPermutation(((uint64_t)R << 32) | (uint64_t)L, ciphertext);
  }

  void convertFormat(string &str, uint8_t *text)
  {
    for (int i = 0; i < 8; i++)
    {
      text[i] = static_cast<unsigned char>(str[i]);
    }
  }
};

int main()
{
  DES des;
  int size = 8;
  uint8_t plaintext[size];
  uint8_t key[size];
  uint8_t ciphertext[size];
  string str;
  while (getline(cin, str))
  {
    string sc1 = str.substr(0, 8);
    string sc2 = str.substr(9, 8);
    des.convertFormat(sc1, plaintext);
    des.convertFormat(sc2, key);

    des.Encrypt(plaintext, key, ciphertext);

    for (int i = 0; i < size; i++)
    {
      printf("%02X", ciphertext[i]);
    }
    printf("\n");
  }

  return 0;
}
