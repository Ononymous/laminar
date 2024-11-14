#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::vector<std::string> split(std::string &s, char sep) {
  std::vector<std::string> result;
  std::string token;
  size_t last = 0;
  size_t next = 0;

  while ((next = s.find(sep, last)) != std::string::npos) {
    token = s.substr(last, next - last);
    result.push_back(token);
    last = next + 1;
  }
  token = s.substr(last);
  result.push_back(token);

  return result;
}

void csv_to_senml(const std::string &filename_input,
                  const std::string &filename_output, char sep,
                  const std::vector<std::map<std::string, std::string>> &labels,
                  std::string_view &id) {
  int count = 0;
  std::ifstream input(filename_input);
  std::ofstream output(filename_output);
  std::string line;

  if (!input.is_open()) {
    std::cout << "Could not open input file" << std::endl;
    exit(1);
  }

  if (!output.is_open()) {
    std::cout << "Could not open output file" << std::endl;
    exit(1);
  }

  while (getline(input, line) && count++ < 10) {
    std::vector<std::string> tokens = split(line, sep);
    output << "[";
    output << "{\"n\": \"source_id\",\"t\": \"int\",\"v\": " << id << "},";
    for (int i = 0; i < tokens.size(); i++) {
      output << "{";
      for (auto &[k, v] : labels[i]) {
        output << "\"" << k << "\": \"" << v << "\",";
      }
      output << "\"v\": " << tokens[i] << "}";

      if (i != tokens.size() - 1) {
        output << ",";
      }
    }

    output << "]\n";
  }
}

int main() {
  std::string input_dir = "/home/snuc/Downloads/MHEALTHDATASET/";
  std::string output_dir = "/home/snuc/Downloads/MHEALTHDATASET/senml_outputs/";
  char sep = '\t';

  // name (n), unit (u), type (t)
  std::vector<std::map<std::string, std::string>> labels = {
    {{"n", "acc_chest_x"},  {"u", "m/s^2"},  {"t", "double"}}, // Chest acceleration (X axis)
    {{"n", "acc_chest_y"},  {"u", "m/s^2"},  {"t", "double"}}, // Chest acceleration (Y axis)
    {{"n", "acc_chest_z"},  {"u", "m/s^2"},  {"t", "double"}}, // Chest acceleration (Z axis)
    {{"n", "ecg_1"},        {"u", "mV"},     {"t", "double"}}, // Electrocardiogram (lead 1)
    {{"n", "ecg_2"},        {"u", "mV"},     {"t", "double"}}, // Electrocardiogram (lead 2)
    {{"n", "acc_ankle_x"},  {"u", "m/s^2"},  {"t", "double"}}, // Left ankle acceleration (X axis)
    {{"n", "acc_ankle_y"},  {"u", "m/s^2"},  {"t", "double"}}, // Left ankle acceleration (Y axis)
    {{"n", "acc_ankle_z"},  {"u", "m/s^2"},  {"t", "double"}}, // Left ankle acceleration (Z axis)
    {{"n", "gyro_ankle_x"}, {"u", "deg/s"},  {"t", "double"}}, // Left ankle gyro (X axis)
    {{"n", "gyro_ankle_y"}, {"u", "deg/s"},  {"t", "double"}}, // Left ankle gyro (Y axis)
    {{"n", "gyro_ankle_z"}, {"u", "deg/s"},  {"t", "double"}}, // Left ankle gyro (Z axis)
    {{"n", "mag_ankle_x"},  {"u", "tesla"},  {"t", "double"}}, // Left ankle magnetometer (X axis)
    {{"n", "mag_ankle_y"},  {"u", "tesla"},  {"t", "double"}}, // Left ankle magnetometer (Y axis)
    {{"n", "mag_ankle_z"},  {"u", "tesla"},  {"t", "double"}}, // Left ankle magnetometer (Z axis)
    {{"n", "acc_arm_x"},    {"u", "m/s^2"},  {"t", "double"}}, // Right arm acceleration (X axis)
    {{"n", "acc_arm_y"},    {"u", "m/s^2"},  {"t", "double"}}, // Right arm acceleration (Y axis)
    {{"n", "acc_arm_z"},    {"u", "m/s^2"},  {"t", "double"}}, // Right arm acceleration (Z axis)
    {{"n", "gyro_arm_x"},   {"u", "deg/s"},  {"t", "double"}}, // Right arm gyro (X axis)
    {{"n", "gyro_arm_y"},   {"u", "deg/s"},  {"t", "double"}}, // Right arm gyro (Y axis)
    {{"n", "gyro_arm_z"},   {"u", "deg/s"},  {"t", "double"}}, // Right arm gyro (Z axis)
    {{"n", "mag_arm_x"},    {"u", "tesla"},  {"t", "double"}}, // Right arm magnetometer (X axis)
    {{"n", "mag_arm_y"},    {"u", "tesla"},  {"t", "double"}}, // Right arm magnetometer (Y axis)
    {{"n", "mag_arm_z"},    {"u", "tesla"},  {"t", "double"}}, // Right arm magnetometer (Z axis)
    {{"n", "label"},                            {"t", "int"}}  // Label
  };

  std::cout << "Converting CSVs to SenML:" << std::endl;
  for (const auto &entry : std::filesystem::directory_iterator(input_dir)) {
    std::string input_path = entry.path();
    std::string output_path = output_dir + std::string(entry.path().filename());
    if (input_path.ends_with(".log")) {
      // Get sensor id from end of file (***[ID].log)
      std::string_view id = input_path;
      id.remove_suffix(4);
      id.remove_prefix(id.find_last_not_of("1234567890") + 1);
      
      csv_to_senml(input_path, output_path, sep, labels, id);
      std::cout << "\t" << input_path << std::endl;
    }
  }
}
