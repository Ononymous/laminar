#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::string annotate(std::unordered_map<std::string, double> &m) {
  const auto now = std::chrono::system_clock::now();

  auto timestamp =
      std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch())
          .count();

  m["timestamp"] = static_cast<double>(timestamp);
}