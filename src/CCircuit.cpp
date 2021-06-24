// Copyright [2021] <FLUORITE>
#include <CCircuit.h>
#include <CUnit.h>
#include <Genetic_Algorithm.h>
#include <stdio.h>

#include <algorithm>
#include <vector>

std::vector<CUnit> units(num_units);

/** Check the validity of a graph
 * @param circuit_vector The circuit which needs to be verified 
 * @return True if the circuit is valid or False if it is not valid
 **/
bool CCircuit::Check_Validity(
    std::vector<int> &circuit_vector) {  // record tail and concentrate nodes.
  // this->final_tail =
  // *max_element(circuit_vector->begin(),circuit_vector->end());
  // this->final_con = final_tail-1;
  // this->num = final_tail+1;
  this->num = (circuit_vector.size() - 1) / 2 + 2;
  this->final_tail = num - 1;
  this->final_con = num - 2;
  // record feed node
  this->feed_node = circuit_vector[0];
  this->units.reserve(num);
  // load the vector of units, and set mark to false
  for (int i = 0; i < num; i++) {
    if (i == this->final_con || i == this->final_tail) {
      units[i].mark = false;
      continue;
    }
    units[i].conc_num = circuit_vector[2 * i + 1];
    units[i].tails_num = circuit_vector[2 * i + 2];
    // 3. There should be no self-recycle
    if (units[i].conc_num == i || units[i].tails_num == i) {
      return false;
    }
    // 4. The destination for both products from a unit should not be the same
    // unit.
    if (units[i].conc_num == units[i].tails_num) {
      return false;
    }
    units[i].mark = false;
  }
  // 1. Every unit must be accessible from the feed
  // check if all the units are been marked
  // after traversing from feed_node
  mark_units(this->feed_node);
  for (int i = 0; i < this->num; i++) {
    // if any units have not been marked, return false
    if (!units[i].mark) {
      return false;
    }
  }

  // 2. Every unit must have a route forward to both of the outlet streams.
  for (int i = 0; i < this->num; i++) {
    // set all units' mark to false
    for (int j = 0; j < this->num; j++) {
      units[j].mark = false;
    }
    // if traversed from this unit do not reach both outlet streams, return
    // false
    mark_units(i);
    if (units[this->final_con].mark == false ||
        units[this->final_tail].mark == false) {
      return false;
    }
  }
  return true;
}

/** Memorisation
 * @param unit_num The index of the unit#
 * */
void CCircuit::mark_units(int unit_num) {
  if (units[unit_num].mark) return;

  units[unit_num].mark = true;

  // If we have seen this unit already exit
  // Mark that we have now seen the unit
  // If conc_num does not point at a circuit outlet recursively call the
  // function

  if (units[unit_num].conc_num < this->num) {
    mark_units(units[unit_num].conc_num);
  } else {
    // ...Potentially do something to indicate that you have seen an exit
    // if we do not reach concentrate or tailings, or we return
    if (unit_num != final_con && unit_num != final_tail)
      mark_units(units[unit_num].conc_num);
    else
      return;
  }

  // If tails_num does not point at a circuit outlet recursively call the
  // function
  if (units[unit_num].tails_num < this->num) {
    mark_units(units[unit_num].tails_num);
  } else {
    // ...Potentially do something to indicate that you have seen an exit
    if (unit_num != final_con && unit_num != final_tail)
      mark_units(units[unit_num].tails_num);
    else
      return;
  }
}
