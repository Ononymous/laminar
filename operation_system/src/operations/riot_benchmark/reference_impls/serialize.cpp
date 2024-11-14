#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

void split(const std::string_view &s, const std::string &sep,
           std::function<void(std::string_view &)> &f) {
  std::string_view token;
  size_t last = 0;
  size_t next = 0;

  while ((next = s.find(sep, last)) != std::string_view::npos) {
    token = s.substr(last, next - last);
    f(token);
    last = next + sep.size();
  }
  token = s.substr(last);
  f(token);
}

std::string serialize(const std::unordered_map<std::string, double> &m) {
  std::string result = "";
  for (auto &[k, v] : m) {
    result += k;
    result += '~';
    result += std::to_string(v);
    result += '\t';
  }

  return result;
}

std::unordered_map<std::string, double> deserialize(const std::string_view &s) {
  std::unordered_map<std::string, double> m;

  std::function<void(std::string_view &)> add_to_map =
      [&](std::string_view &kv_pair) {
        std::string_view key = kv_pair.substr(0, kv_pair.find('~'));
        std::string_view value = kv_pair.substr(kv_pair.find('~') + 1);
        if (key != " ") {
          m[std::string(key)] = atof(std::string(value).c_str());
        }
      };

  split(s, "\t", add_to_map);

  return m;
}

int main() {
  std::string serial_example =
      "acc_chest_x~-9.8184	acc_chest_y~0.009971	"
      "acc_chest_z~0.29563	ecg_1~0.0041863	ecg_2~0.0041863	"
      "acc_ankle_x~2.1849	acc_ankle_y~-9.6967	"
      "acc_ankle_z~0.63077	gyro_ankle_x~0.1039	"
      "gyro_ankle_y~-0.84053	gyro_ankle_z~-0.68762	"
      "mag_ankle_x~-0.37	mag_ankle_y~-0.36327	"
      "mag_ankle_z~0.29963	acc_arm_x~-8.6499	"
      "acc_arm_y~-4.5781	acc_arm_z~0.18776	"
      "gyro_arm_x~-0.44902	gyro_arm_y~-1.0103	"
      "gyro_arm_z~0.034483	mag_arm_x~-2.35	mag_arm_y~-1.6102	"
      "mag_arm_z~-0.030899	label~0	 ";

  int runs = 10000;

  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < runs; i++)
    auto m = deserialize(serial_example);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "deserialize avg time:" << elapsed_seconds.count() * 1e3 / runs
            << "ms" << std::endl;

  auto m = deserialize(serial_example);
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < runs; i++)
    auto x = serialize(m);
  end = std::chrono::high_resolution_clock::now();

  elapsed_seconds = end - start;
  std::cout << "serialize avg time:" << elapsed_seconds.count() * 1e3 / runs
            << "ms" << std::endl;

  // auto m = deserialize(serial_example);
  // std::cout << serialize(m) << std::endl;
  // std::cout << serial_example;
}

// std::unordered_map<std::string, double> deserialize(const std::string_view
// &s) {
//   std::unordered_map<std::string, double> m;

//   std::vector<std::string_view> kv_pairs = split(s, "\t");

//   for (std::string_view &kv_pair : kv_pairs) {
//     std::string_view key = kv_pair.substr(0, kv_pair.find('~'));
//     std::string_view value = kv_pair.substr(kv_pair.find('~') + 1);
//     if (key != " ") {
//       m[std::string(key)] = atof(std::string(value).c_str());
//     }
//   }

//   return m;
// }