// Copyright [2021] <FLUORITE>
#include <CUnit.h>
#include <iostream>


double gormanium_recover_rate = 0.2;
double waste_recover_rate = 0.05;

/** Function: recoverStream
 * Description: compute the concentrate stream and tail stream, both of them
 *              have 2 component: gormanium and waste
**/
void CUnit::recoverStream() {
    // concentrate stream

    concentrate_stream[0] = gormanium_feed_rate * gormanium_recover_rate;   // the gormanium of concentrate stream
    concentrate_stream[1] = waste_feed_rate * waste_recover_rate;           // the waste of concentrate stream

    // tail stream
    tail_stream[0] = gormanium_feed_rate - concentrate_stream[0];           // the gormanium of tail stream
    tail_stream[1] = waste_feed_rate - concentrate_stream[1];               // the waste of tail stream
}
