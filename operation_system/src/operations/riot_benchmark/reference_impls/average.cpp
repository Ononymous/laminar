#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::unordered_map<std::string, double>> prev_inputs;
std::vector<std::unordered_map<std::string, double>> prev_avgs;

auto average(std::unordered_map<std::string, double> &input, int window_size) {
  std::vector<std::string> fields = {"test", "test2"};

  // Case: First input
  if (prev_avgs.size() == 0) {
    std::unordered_map<std::string, double> result;
    for (auto &field : fields) {
      result[field] = input[field];
    }

    return result;
  }

  std::unordered_map<std::string, double> prev_avg = prev_avgs.back();

  // Case: Window has not been filled
  if (prev_avgs.size() < window_size) {
    for (auto &field : fields) {
      double sum = prev_avg[field] * prev_avgs.size() + input[field];
      prev_avg[field] = sum / (prev_avgs.size() + 1);
    }

    return prev_avg;
  }

  // Case: Window is full
  std::unordered_map<std::string, double> oldest_input =
      prev_inputs[prev_inputs.size() - window_size];
  for (auto &field : fields) {
    prev_avg[field] += (input[field] - oldest_input[field]) / window_size;
  }

  return prev_avg;
}

int main() {
  std::vector<double> vals = {2, 4, 6, 8, 12, 14, 16, 18, 20};

  for (auto &val : vals) {
    std::unordered_map<std::string, double> m = {{"test", val},
                                                 {"test2", val * 2}};
    auto avg = average(m, 5);
    prev_avgs.push_back(avg);
    prev_inputs.push_back(m);
  }

  for (auto &avg : prev_avgs) {
    std::cout << avg["test"] << " " << avg["test2"] << std::endl;
  }
}