// Copyright [2021] <FLUORITE>
#include <iostream>
#include <vector>

#include "../includes/CUnit.h"
#include "../includes/CCircuit.h"

using namespace std; // NOLINT [build/namespaces]
/**
 * This is test for validity-checking. 
 * Unit accessibility from the feed 
 * Connectivity to both outlet streams of each unit
 * No self-cycle 
 * Different two destinations of each unit
**/
int main(int argc, char * argv[]) {
        // check every unit is accessible from the feed
        std::vector <int> valid1 {0, 4, 3, 2, 0, 5, 4, 4, 6, 2, 1};

        // check every unit forwards to both outlet streams
        std::vector <int> valid2 {6, 16, 7, 7, 13, 11, 12, 15, 5, 3, 6, 0, 2, 14, 12, 1, 12, 14, 11, 5, 16, 11, 9, 4, 1, 0, 8, 5, 10, 2, 6};

        // check it is a self-recycle
        std::vector <int> invalid1{0, 1, 2, 3, 2, 3, 5, 3, 4, 7, 5, 6, 5};

        // check two different outlets from each unit beforehand
        std::vector <int> invalid2{0, 1, 2, 3, 2, 5, 5, 4, 3, 0, 7, 6};

        CCircuit valid_test;
        CCircuit invalid_test;

        std::cout << "Check_Validity({0,4,3,2,0,5,4,4,6,2,1}):\n";
        if (valid_test.Check_Validity(valid1))
            std::cout  << "check every unit is accessible from the feed.\n pass\n";
        else
            std::cout << "fail\n";

        std::cout << "Check_Validity({6,16,7,7,13,11,12,15,5,3,6,0,2,14,12,1,12,14,11,5,16,11,9,4,1,0,8,5,10,2,6):\n";
        if (valid_test.Check_Validity(valid2))
            std::cout  << "check every unit forwards to both outlet streams.\n pass\n";
        else
            std::cout << "fail\n";

        std::cout << "Check_Validity({0,1,2,3,2,3,5,3,4,7,5,6,5}):\n";
        if (invalid_test.Check_Validity(invalid1))
            std::cout  << "fail\n";
        else
            std::cout << "check it is a self-cycle.\n pass\n";

        std::cout << "Check_Validity({0,1,2,3,2,5,5,4,3,0,7,6}):\n";
        if (invalid_test.Check_Validity(invalid2))
            std::cout << "fail\n";
        else
            std::cout << "check two different outlets from each unit beforehand.\n pass\n";
}
