#include "load.h"
#include <string>

using namespacce std;

void LoadData(const string& data_dir, float input[kNum][kInImSize][kInImSize],
              float weight[kNum][kNum][kKernel][kKernel], float bias[kNum]) {
  const char kInputFile[] = "input.bin";
  const char kWeightFile[] = "weight.bin";
  const char kBiasFile[] = "bias.bin";

  int input_fd = open((data_dir + kInputFile).c_str(), O_RDONLY);
  int weight_fd = open((data_dir + kWeightFile).c_str(), O_RDONLY);
  int bias_fd = open((data_dir + kBiasFile).c_str(), O_RDONLY);

  if (input_fd == -1) {
    clog << "Cannot find " << kInputFile << endl;
    exit(EXIT_FAILURE);
  }
  if (weight_fd == -1) {
    clog << "Cannot find " << kWeightFile << endl;
    exit(EXIT_FAILURE);
  }
  if (bias_fd == -1) {
    clog << "Cannot find " << kBiasFile << endl;
    exit(EXIT_FAILURE);
  }

  auto input_in = reinterpret_cast<float(*)[kInImSize][kInImSize]>(mmap(
      nullptr, sizeof(*input) * kNum, PROT_READ, MAP_SHARED, input_fd, 0));
  if (input_in == MAP_FAILED) {
    clog << "Incomplete " << kInputFile << endl;
    close(input_fd);
    exit(EXIT_FAILURE);
  }

  auto weight_in = reinterpret_cast<float(*)[kNum][kKernel][kKernel]>(mmap(
      nullptr, sizeof(*weight) * kNum, PROT_READ, MAP_SHARED, weight_fd, 0));
  if (weight_in == MAP_FAILED) {
    clog << "Incomplete " << kWeightFile << endl;
    close(weight_fd);
    exit(EXIT_FAILURE);
  }

  float* bias_in = reinterpret_cast<float*>(mmap(
      nullptr, sizeof(*bias) * kNum, PROT_READ, MAP_SHARED, bias_fd, 0));
  if (bias_in == MAP_FAILED) {
    clog << "Incomplete " << kBiasFile << endl;
    close(bias_fd);
    exit(EXIT_FAILURE);
  }

  memcpy(input, input_in, sizeof(*input) * kNum);
  memcpy(weight, weight_in, sizeof(*weight) * kNum);
  memcpy(bias, bias_in, sizeof(*bias) * kNum);
  munmap(input_in, sizeof(*input) * kNum);
  munmap(weight_in, sizeof(*weight) * kNum);
  munmap(bias_in, sizeof(*bias) * kNum);
  close(input_fd);
  close(weight_fd);
  close(bias_fd);
}