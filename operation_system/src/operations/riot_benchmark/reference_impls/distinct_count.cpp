#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <string>

const double magic_num = 0.79402; // Durand-Flajolet magic number
const int bucket_param = 10;
const int num_buckets = 1 << bucket_param;

int max_zeros[num_buckets];

int trailing_zeros(int n) {
  int bits = 0;

  if (n) {
    while ((n & 1) == 0) {
      ++bits;
      n >>= 1;
    }
  }

  return bits;
}

/**
 * Gives the appox count of the number of distinct items that have been seen
 * this far
 *
 * Based on the Durand-Flajolet Algorithm
 * [1] Loglog Counting of Large Cardinalities, Durand and Flajolet, Sec 2
 * http://algo.inria.fr/flajolet/Publications/DuFl03-LNCS.pdf
 * [2] Mining of Massive Datasets, Jure Leskovec, et al, 2014: Sec 4.4.2 The
 * Flajolet-Martin Algorithm
 *
 * @param item
 * @return E estimate of the cardinality of unique items in the stream, seen
 * thus far
 */
int unique_count(std::string_view item) {
  std::size_t hash_value = std::hash<std::string_view>{}(item);

  int bucket_id = hash_value & (num_buckets - 1);

  max_zeros[bucket_id] = std::max(max_zeros[bucket_id],
                                  trailing_zeros(hash_value >> bucket_param));

  double sum_max_zeros = 0;
  for (int i = 0; i < num_buckets; i++) {
    sum_max_zeros += max_zeros[i];
  }

  // E := α * m * 2^(1/m * Sum_j(M(j)))
  int E = (int)(magic_num * num_buckets * pow(2, sum_max_zeros / num_buckets));

  return E;
}

int main() {
  constexpr const std::array inputs = {"aaa", "aaa", "bbb",
                                       "aaa", "bbb", "ccc"};

  for (auto &input : inputs) {
    std::cout << unique_count(input) << std::endl;
  }

  return 0;
}