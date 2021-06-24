// Copyright [2021] <FLUORITE>
#ifndef INCLUDES_CUNIT_H_
#define INCLUDES_CUNIT_H_

class CUnit {
 public:
  // compute the concentrate stream and tail stream of every unit
  void recoverStream();

  // index of the unit to which this unit’s concentrate stream is connected
  int conc_num;
  // index of the unit to which this unit’s concentrate stream is connected
  int tails_num;
  // A Boolean that is changed to true if the unit has been seen
  bool mark;

  // gormanium flow-in rate
  double gormanium_feed_rate;
  // waste flow-in rate
  double waste_feed_rate;

  // both stream have 2 component (valueble material and waste material)
  double concentrate_stream[2];
  double tail_stream[2];

  // gormanium convert rate

  static const double gormanium_convert_rate;
  // waste convert rate
  static const double waste_convert_rate;
};

#endif  // INCLUDES_CUNIT_H_
