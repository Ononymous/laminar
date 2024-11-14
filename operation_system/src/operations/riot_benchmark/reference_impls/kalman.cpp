#include <array>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

using SDMap = std::unordered_map<std::string, double>;

/**
 * p0: prior error covariance
 * p1: current error covariance
 * x0: previous estimation
 * x1: current estimation
 */

float kalman_filter(float z_measuredValue, double &p0, double &x0) {
  float q_processNoise = 0.125;
  float r_sensorNoise = 0.32;

  // Time update
  float p1 = p0 + q_processNoise;

  // Measurement update
  float k_kalmanGain = p1 / (p1 + r_sensorNoise);
  float x1 = x0 + k_kalmanGain * (z_measuredValue - x0);
  p1 = (1 - k_kalmanGain) * p1;

  // Update values
  x0 = x1;
  p0 = p1;

  return x1;
}

template <size_t N>
SDMap kalman_filter(SDMap &input, SDMap &p0s, SDMap &x0s,
                    const std::array<const char *, N> &fields) {
  SDMap estimates;

  // Get estimates, update p0s and x0s
  for (auto &field : fields) {
    float estimate = kalman_filter(input[field], p0s[field], x0s[field]);
    estimates[field] = estimate;
  }

  return estimates;
}

int main() {
  SDMap input = {{"test1", 3.0}, {"test2", 6.0}};

  //   constexpr const std::array fields = {
  //       "acc_chest_x", "acc_chest_y", "acc_chest_z", "ecg_lead_1",
  //       "ecg_lead_2",  "acc_ankle_x", "acc_ankle_y", "acc_ankle_z",
  //       "acc_arm_x",   "acc_arm_y",   "acc_arm_z",
  //   };

  constexpr const std::array fields = {"test1", "test2"};

  SDMap p0s, x0s;
  for (auto &field : fields) {
    p0s[field] = 30.0;
    x0s[field] = 0.0;
  }

  for (int i = 0; i < 5; i++) {
    SDMap estimates = kalman_filter(input, p0s, x0s, fields);
    input["test1"] += i * 0.1;
    input["test2"] += i * 0.1;
    std::cout << estimates["test1"] << ", " << estimates["test2"] << std::endl;
  }

  return 0;
}

/* Initial implementation for one value */

// float p0_priorErrorCovariance = 30;
// float x0_previousEstimation;

// float kalman_filter(std::unordered_map<std::string, double> &input,
//                     const std::string &field) {

//   float q_processNoise = 0.125;
//   float r_sensorNoise = 0.32;

//   // Get measure for current iteration
//   float z_measuredValue = input[field];

//   // Time update
//   float p1_currentErrorCovariance = p0_priorErrorCovariance + q_processNoise;

//   // Measurement update
//   float k_kalmanGain =
//       p1_currentErrorCovariance / (p1_currentErrorCovariance +
//       r_sensorNoise);
//   float x1_currentEstimation =
//       x0_previousEstimation +
//       k_kalmanGain * (z_measuredValue - x0_previousEstimation);
//   p1_currentErrorCovariance = (1 - k_kalmanGain) * p1_currentErrorCovariance;

//   x0_previousEstimation = x1_currentEstimation;
//   p0_priorErrorCovariance = p1_currentErrorCovariance;

//   return x1_currentEstimation;
// }

// int main() {
//   std::string field = "test";
//   std::unordered_map<std::string, double> input = {{"test", 3.0}};

//   std::vector<std::string> fields = {
//       "acc_chest_x", "acc_chest_y", "acc_chest_z", "ecg_lead_1",
//       "ecg_lead_2",  "acc_ankle_x", "acc_ankle_y", "acc_ankle_z",
//       "acc_arm_x",   "acc_arm_y",   "acc_arm_z"};

//   std::cout << kalman_filter(val) << std::endl;

//   return 0;
// }