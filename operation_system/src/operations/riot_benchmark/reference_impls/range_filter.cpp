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

struct range {
  range(double min_val, double max_val) : min(min_val), max(max_val) {}
  double min;
  double max;
};

bool filter(std::unordered_map<std::string, double> &input) {
  std::unordered_map<std::string, range> ranges = {
      {"acc_chest_x", {-13.931, 4.123}},  {"acc_chest_y", {-4.6376, 5.2361}},
      {"acc_chest_z", {-8.1881, 7.8786}}, {"ecg_lead_1", {-4.9314, 6.1371}},
      {"ecg_lead_2", {-6.786, 6.6604}},   {"acc_ankle_x", {-5.0006, 8.1472}},
      {"acc_ankle_y", {-14.303, 1.5909}}, {"acc_ankle_z", {-8.6234, 8.6958}},
      {"acc_arm_x", {-9.824, 5.5778}},    {"acc_arm_y", {-10.059, 8.506}},
      {"acc_arm_z", {-6.6739, 9.5725}}};

  for (auto &[key, valid_range] : ranges) {
    double val = input[key];
    if (val < valid_range.min || val > valid_range.max) {
      return false;
    }
  }

  return true;
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

  std::unordered_map<std::string, double> m = deserialize(serial_example);

  std::cout << filter(m) << std::endl;

  m["ecg_lead_2"] = 10;

  std::cout << filter(m) << std::endl;

  return 0;
}