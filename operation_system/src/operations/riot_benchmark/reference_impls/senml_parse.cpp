#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

std::vector<std::string_view> split(const std::string_view &s,
                                    const std::string &sep) {
  std::vector<std::string_view> result;
  std::string_view token;
  size_t last = 0;
  size_t next = 0;

  while ((next = s.find(sep, last)) != std::string_view::npos) {
    token = s.substr(last, next - last);
    result.push_back(token);
    last = next + sep.size();
  }
  token = s.substr(last);
  result.push_back(token);

  return result;
}

std::string serialize_senml(const std::string_view &senml) {
  std::string serialized = "";

  // Remove cruft
  std::string_view senml_clean = senml.substr(2, senml.size() - 4);

  // Extract values
  std::vector<std::string_view> entries = split(senml_clean, "},{");
  for (auto &e : entries) {
    std::vector<std::string_view> kv_pairs = split(e, ",");

    // Key
    std::string_view key_pair = kv_pairs[0];
    key_pair.remove_suffix(1);
    std::string_view key = key_pair.substr(kv_pairs[0].find(' ') + 2);
    serialized += key;
    serialized += '~';

    // Value
    std::string_view value_pair = kv_pairs.back();
    std::string_view value = value_pair.substr(value_pair.find(' ') + 1);
    serialized += value;
    serialized += '\t';
  }

  return serialized;
}

int main() {
    std::string senml = "[{\"n\": \"source_id\",\"t\": \"int\",\"v\": 1},{\"n\": \"acc_chest_x\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": -9.8184},{\"n\": \"acc_chest_y\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": 0.009971},{\"n\": \"acc_chest_z\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": 0.29563},{\"n\": \"ecg_1\",\"t\": \"double\",\"u\": \"mV\",\"v\": 0.0041863},{\"n\": \"ecg_2\",\"t\": \"double\",\"u\": \"mV\",\"v\": 0.0041863},{\"n\": \"acc_ankle_x\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": 2.1849},{\"n\": \"acc_ankle_y\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": -9.6967},{\"n\": \"acc_ankle_z\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": 0.63077},{\"n\": \"gyro_ankle_x\",\"t\": \"double\",\"u\": \"deg/s\",\"v\": 0.1039},{\"n\": \"gyro_ankle_y\",\"t\": \"double\",\"u\": \"deg/s\",\"v\": -0.84053},{\"n\": \"gyro_ankle_z\",\"t\": \"double\",\"u\": \"deg/s\",\"v\": -0.68762},{\"n\": \"mag_ankle_x\",\"t\": \"double\",\"u\": \"tesla\",\"v\": -0.37},{\"n\": \"mag_ankle_y\",\"t\": \"double\",\"u\": \"tesla\",\"v\": -0.36327},{\"n\": \"mag_ankle_z\",\"t\": \"double\",\"u\": \"tesla\",\"v\": 0.29963},{\"n\": \"acc_arm_x\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": -8.6499},{\"n\": \"acc_arm_y\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": -4.5781},{\"n\": \"acc_arm_z\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": 0.18776},{\"n\": \"gyro_arm_x\",\"t\": \"double\",\"u\": \"deg/s\",\"v\": -0.44902},{\"n\": \"gyro_arm_y\",\"t\": \"double\",\"u\": \"deg/s\",\"v\": -1.0103},{\"n\": \"gyro_arm_z\",\"t\": \"double\",\"u\": \"deg/s\",\"v\": 0.034483},{\"n\": \"mag_arm_x\",\"t\": \"double\",\"u\": \"tesla\",\"v\": -2.35},{\"n\": \"mag_arm_y\",\"t\": \"double\",\"u\": \"tesla\",\"v\": -1.6102},{\"n\": \"mag_arm_z\",\"t\": \"double\",\"u\": \"tesla\",\"v\": -0.030899},{\"n\": \"label\",\"t\": \"int\",\"v\": 0}]";

    // auto start = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < 1000; i++)
    //     std::string m = serialize_senml(senml);
    // auto end = std::chrono::high_resolution_clock::now();

    // std::chrono::duration<double> elapsed_seconds = end - start;
    // std::cout << "time:" << elapsed_seconds.count() * 1e3 << "ms" << std::endl;

    std::string m = serialize_senml(senml);
    std::cout << m;
}


// std::string_view implementation

// struct mhealth_entry {
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



// std::vector<std::string_view> split(const std::string_view& s, const std::string& sep) {
//   std::vector<std::string_view> result;
//   std::string_view token;
//   size_t last = 0;
//   size_t next = 0;

//   while ((next = s.find(sep, last)) != std::string_view::npos) {
//     token = s.substr(last, next - last);
//     result.push_back(token);
//     last = next + sep.size();
//   }
//   token = s.substr(last);
//   result.push_back(token);

//   return result;
// }

// mhealth_entry senml_to_struct(std::string_view senml) {
//   mhealth_entry m;

//   // Remove cruft
//   std::string_view senml_clean = senml.substr(2, senml.size() - 4);

//   // Extract values
//   std::vector<std::string_view> entries = split(senml_clean, "},{");
//   std::vector<std::string_view> values;
//   for (auto &e : entries) {
//     std::vector<std::string_view> kv_pairs = split(e, ",");
//     std::string_view value_pair = kv_pairs.back();
//     std::string_view value = value_pair.substr(value_pair.find(' ') + 1);
//     values.push_back(value);
//   }

//   // Assign values
//   m.chest_accel_x = atof(std::string(values[0]).c_str());
//   m.chest_accel_y = atof(std::string(values[1]).c_str());
//   m.chest_accel_z = atof(std::string(values[2]).c_str());
//   m.ecg_1 = atof(std::string(values[3]).c_str());
//   m.ecg_2 = atof(std::string(values[4]).c_str());
//   m.ankle_accel_x = atof(std::string(values[5]).c_str());
//   m.ankle_accel_y = atof(std::string(values[6]).c_str());
//   m.ankle_accel_z = atof(std::string(values[7]).c_str());
//   m.ankle_gyro_x = atof(std::string(values[8]).c_str());
//   m.ankle_gyro_y = atof(std::string(values[9]).c_str());
//   m.ankle_gyro_z = atof(std::string(values[10]).c_str());
//   m.ankle_mag_x = atof(std::string(values[11]).c_str());
//   m.ankle_mag_y = atof(std::string(values[12]).c_str());
//   m.ankle_mag_z = atof(std::string(values[13]).c_str());
//   m.arm_accel_x = atof(std::string(values[14]).c_str());
//   m.arm_accel_y = atof(std::string(values[15]).c_str());
//   m.arm_accel_z = atof(std::string(values[16]).c_str());
//   m.arm_gyro_x = atof(std::string(values[17]).c_str());
//   m.arm_gyro_y = atof(std::string(values[18]).c_str());
//   m.arm_gyro_z = atof(std::string(values[19]).c_str());
//   m.arm_mag_x = atof(std::string(values[20]).c_str());
//   m.arm_mag_y = atof(std::string(values[21]).c_str());
//   m.arm_mag_z = atof(std::string(values[22]).c_str());
//   m.label = atoi(std::string(values[23]).c_str());

//   return m;
// }


/////////////////

// Initial std::string implementation

// struct mhealth_entry {
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

// std::vector<std::string> split(std::string &s, std::string sep) {
//   std::vector<std::string> result;
//   std::string token;
//   size_t last = 0;
//   size_t next = 0;

//   while ((next = s.find(sep, last)) != std::string::npos) {
//     token = s.substr(last, next - last);
//     result.push_back(token);
//     last = next + sep.size();
//   }
//   token = s.substr(last);
//   result.push_back(token);

//   return result;
// }

// mhealth_entry senml_to_struct(std::string &senml) {
//   mhealth_entry m;

//   // Remove cruft
//   std::string senml_clean = senml.substr(2, senml.size() - 4);

//   // Extract values
//   std::vector<std::string> entries = split(senml_clean, "},{");
//   std::vector<std::string> values;
//   for (auto &e : entries) {
//     std::vector<std::string> kv_pairs = split(e, ",");
//     std::string &value_pair = kv_pairs.back();
//     std::string value = value_pair.substr(value_pair.find(' ') + 1);
//     values.push_back(value);
//   }

//   // Assign values
//   m.chest_accel_x = atof(values[0].c_str());
//   m.chest_accel_y = atof(values[1].c_str());
//   m.chest_accel_z = atof(values[2].c_str());
//   m.ecg_1 = atof(values[3].c_str());
//   m.ecg_2 = atof(values[4].c_str());
//   m.ankle_accel_x = atof(values[5].c_str());
//   m.ankle_accel_y = atof(values[6].c_str());
//   m.ankle_accel_z = atof(values[7].c_str());
//   m.ankle_gyro_x = atof(values[8].c_str());
//   m.ankle_gyro_y = atof(values[9].c_str());
//   m.ankle_gyro_z = atof(values[10].c_str());
//   m.ankle_mag_x = atof(values[11].c_str());
//   m.ankle_mag_y = atof(values[12].c_str());
//   m.ankle_mag_z = atof(values[13].c_str());
//   m.arm_accel_x = atof(values[14].c_str());
//   m.arm_accel_y = atof(values[15].c_str());
//   m.arm_accel_z = atof(values[16].c_str());
//   m.arm_gyro_x = atof(values[17].c_str());
//   m.arm_gyro_y = atof(values[18].c_str());
//   m.arm_gyro_z = atof(values[19].c_str());
//   m.arm_mag_x = atof(values[20].c_str());
//   m.arm_mag_y = atof(values[21].c_str());
//   m.arm_mag_z = atof(values[22].c_str());
//   m.label = atoi(values[23].c_str());

//   return m;
// }
