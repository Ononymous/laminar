#include <array>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

constexpr std::array<int, 7> seeds = {31,     131,     1313,    13131,
                                      131313, 1313131, 13131313};

// Source: https://www.partow.net/programming/hashfunctions/#BKDRHashFunction
unsigned int BKDRHash(const char *str, unsigned int length, unsigned int seed) {
  /* seeds: 31 131 1313 13131 131313 etc.. */
  unsigned int hash = 0;
  unsigned int i = 0;

  for (i = 0; i < length; ++str, ++i) {
    hash = (hash * seed) + (*str);
  }

  return hash;
}

void generate_filter() {
  for (int seed_idx = 0; seed_idx < seeds.size(); seed_idx++) {
    for (int id = 1; id <= 10; id++) {
      char source[20];
      sprintf(source, "%i_mHealth_subject", id);
      unsigned int hash = BKDRHash(source, strlen(source), seeds[seed_idx]);
      std::cout << std::hex << "0x" << hash << ",";
    }
    std::cout << std::endl;
  }
}

bool bloom_filter(std::unordered_map<std::string, double> &m) {
  const std::unordered_set<unsigned int> filter = {
      0xcd3cc008, 0x1de69e09, 0x6e907c0a, 0xbf3a5a0b, 0xfe4380c,  0x608e160d,
      0xb137f40e, 0x1e1d20f,  0x528bb010, 0x1c7f29f6, 0x602196bc, 0x2473c5fd,
      0xe8c5f53e, 0xad18247f, 0x716a53c0, 0x35bc8301, 0xfa0eb242, 0xbe60e183,
      0x82b310c4, 0x3486424e, 0xe620bac2, 0xb288ecc3, 0x7ef11ec4, 0x4b5950c5,
      0x17c182c6, 0xe429b4c7, 0xb091e6c8, 0x7cfa18c9, 0x49624aca, 0xefb75612,
      0xfff044e4, 0x77dfba5,  0xf0bb266,  0x16996927, 0x1e271fe8, 0x25b4d6a9,
      0x2d428d6a, 0x34d0442b, 0x3c5dfaec, 0xc6bf15be, 0xd079a012, 0xef8c2713,
      0xe9eae14,  0x2db13515, 0x4cc3bc16, 0x6bd64317, 0x8ae8ca18, 0xa9fb5119,
      0xc90dd81a, 0xb56eae32, 0x8be0a84,  0xba820745, 0x6c460406, 0x1e0a00c7,
      0xcfcdfd88, 0x8191fa49, 0x3355f70a, 0xe519f3cb, 0x96ddf08c, 0x41200b7e,
      0xfa6e4352, 0xddcb5e53, 0xc1287954, 0xa4859455, 0x87e2af56, 0x6b3fca57,
      0x4e9ce558, 0x31fa0059, 0x15571b5a, 0x6d6f6ab2};

  for (auto &seed : seeds) {
    char source[20];
    int id = static_cast<int>(m["source_id"]);
    sprintf(source, "%i_mHealth_subject", id);

    unsigned int hash = BKDRHash(source, strlen(source), seed);
    if (!filter.contains(hash)) {
      return false;
    }
  }

  return true;
}

int main() {
  std::unordered_map<std::string, double> m1 = {{"source_id", 3.0}};
  std::cout << bloom_filter(m1) << std::endl;

  std::unordered_map<std::string, double> m2 = {{"source_id", 100.0}};
  std::cout << bloom_filter(m2) << std::endl;

  int iters = 10000;

  bool x;
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iters; i++)
    x = bloom_filter(m1);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "avg time:" << elapsed_seconds.count() * 1e3 / iters << "ms"
            << std::endl;
}