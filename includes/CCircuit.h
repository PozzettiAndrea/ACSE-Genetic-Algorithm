// Copyright [2021] <FLUORITE>
#ifndef INCLUDES_CCIRCUIT_H_
#define INCLUDES_CCIRCUIT_H_

#include <vector>

#include "CUnit.h"

const int num_units = 10;

class CCircuit {
 public:
  int num = num_units;
  int final_con = 0;
  int final_tail = 0;
  int feed_node = 0;
  std::vector<CUnit> units;
  bool Check_Validity(std::vector<int> &circuit_vector);
  void mark_units(int unit_num);
};

#endif  // INCLUDES_CCIRCUIT_H_
