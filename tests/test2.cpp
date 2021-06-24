// Copyright [2021] <FLUORITE>
#include <cmath>
#include <iostream>
#include <vector>

#include "../includes/CCircuit.h"
#include "../includes/CUnit.h"
#include "../includes/Genetic_Algorithm.h"

using namespace std; // NOLINT [build/namespaces]

int main(int argc, char* argv[]) {
  vector<int> vec1 = {0, 1, 2,  3,  4,  4,  5,  6,  7,  7, 8,
                      8, 9, 10, 11, 10, 11, 10, 11, 10, 11};

  vector<int> vec2 = {0, 1,  11, 2,  11, 3,  11, 4,  11, 5, 11,
                      6, 11, 7,  11, 8,  11, 9,  11, 10, 11};

  std::cout << Evaluate_Circuit(vec1, 1e-6, 1000) << "\n";
  std::cout << "Evaluate_Circuit(vec1, 1e-6, 1) close to 50000.0:\n";
  if (std::fabs(Evaluate_Circuit(vec1, 1e-6, 1) - (-50000)) < 1.0e-8) {
    std::cout << "pass\n";
  } else {
    std::cout << "fail\n";
  }

  std::cout << Evaluate_Circuit(vec2, 1e-6, 1000) << "\n";
  std::cout << "Evaluate_Circuit(vec2, 1e-6, 1) close to 50000.0:\n";
  if (std::fabs(Evaluate_Circuit(vec2, 1e-6, 1) - (-50000)) < 1.0e-8) {
    std::cout << "pass\n";
  } else {
    std::cout << "fail\n";
  }
  return EXIT_SUCCESS;
}
