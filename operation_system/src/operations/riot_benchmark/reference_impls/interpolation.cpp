#include <functional>
#include <iostream>
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

void interpolate(std::unordered_map<std::string, double> &current) {
  int window_size = 5;

  if (window_size <= 0) {
    return;
  }

  // std::vector<std::string> fields = {
  //     "acc_chest_x", "acc_chest_y", "acc_chest_z", "ecg_lead_1",
  //     "ecg_lead_2",  "acc_ankle_x", "acc_ankle_y", "acc_ankle_z",
  //     "acc_arm_x",   "acc_arm_y",   "acc_arm_z"};

  std::vector<std::string> fields = {"acc_chest_x"};

  // std::vector<std::string> fields = {};

  std::vector<double> prev;

  for (std::string &field : fields) {
    // Only interpolate if value is null (and window size is valid)
    if (!current[field]) {
      // TODO: get last sequence of woof which holds value history
      int last_seq = 5;
      for (int i = last_seq; i > std::max(last_seq - window_size, 0); i--) {
        // TODO: Fill from history woof if empty
      }

      // Average previous values
      double sum = 0;
      for (double &val : prev) {
        sum += val;
      }

      // Update field with average
      current[field] = sum / prev.size();
    }

    // TODO: add current value to history
    // woof_put(...)
  }
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

  // TODO: get current value from input
  std::unordered_map<std::string, double> m = deserialize(serial_example);
  interpolate(m);
}

// Old (structs)

// struct mhealth_entry {
//   mhealth_entry(double x) : chest_accel_x(x) {}

//   double chest_accel_x;
//   double chest_accel_y;
//   double chest_accel_z;
//   double ecg_1;
//   double ecg_2;
//   double ankle_accel_x;
//   double ankle_accel_y;
//   double ankle_accel_z;
//   double ankle_gyro_x;
//   double ankle_gyro_y;
//   double ankle_gyro_z;
//   double ankle_mag_x;
//   double ankle_mag_y;
//   double ankle_mag_z;
//   double arm_accel_x;
//   double arm_accel_y;
//   double arm_accel_z;
//   double arm_gyro_x;
//   double arm_gyro_y;
//   double arm_gyro_z;
//   double arm_mag_x;
//   double arm_mag_y;
//   double arm_mag_z;
//   int label;
// };

// void interpolate() {
//   int window_size = 10;

//   // TODO: get current value from input
//   mhealth_entry *current = new mhealth_entry(0);
//   std::vector<mhealth_entry *> prev;

//   // Only interpolate if value is null (and window size is valid)
//   if (!current->chest_accel_x && window_size > 0) {
//     // TODO: get last sequence of woof which holds value history
//     int last_seq = 5;
//     for (int i = last_seq; i > std::max(last_seq - window_size, 0); i--) {
//       mhealth_entry *m = new mhealth_entry(i);
//       prev.push_back(m); // TODO: push value from woof to vector
//     }

//     double sum = 0;
//     for (auto &m : prev) {
//       sum += m->chest_accel_x;
//     }

//     current->chest_accel_x = sum / prev.size();
//   }

//   // TODO: add current value to history

//   std::cout << current->chest_accel_x;
// }

// int main() { interpolate(); }