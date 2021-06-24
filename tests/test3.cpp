// Copyright [2021] <FLUORITE>
#include <stdio.h>
#include <chrono>   // NOLINT [build/c++11]
#include <iostream>
#include <vector>

// #include "CCircuit.h"
// #include "CUnit.h"
// #include "Genetic_Algorithm.h"

#include "../includes/CCircuit.h"
#include "../includes/CUnit.h"
#include "../includes/Genetic_Algorithm.h"

int main(int argc, char* argv[]) {

        double fitnessarray1[6] = {4, 1, 1, 1, 1, 0};
        double fitnessarray2[4] = {5, 3, 2, 0};

        // ----------------- //
        // PickIndex Testing //
        // ----------------- //

        std::vector<double> fitnesses1 = {fitnessarray1, fitnessarray1+6};
        std::vector<double> fitnesses2 = {fitnessarray2, fitnessarray2+4};

        int occurrences1[6] = {0, 0, 0, 0, 0, 0};
        int occurrences2[4] = {0, 0, 0, 0};

        for (int i = 0; i < 800000; i++) {
            int index = pickIndex(fitnesses1, 0, 4);
            occurrences1[index]++;
        }

        if ((abs(occurrences1[0] - 400000) < 500) && (abs(occurrences1[1] - 100000) < 500) && (abs(occurrences1[2] - 100000) < 500) && (abs(occurrences1[3] - 100000) < 100) && (abs(occurrences1[4] - 100000) < 100)) {
                std::cout << "Test1 successful! :)\n";
            }

        for (int i = 0; i < 100000; i++) {
            int index = pickIndex(fitnesses2, 0, 5);
            occurrences2[index]++;
        }

        if ((abs(occurrences2[0] - 50000) < 100) && (abs(occurrences2[1] - 30000) < 100) && (abs(occurrences2[2] - 20000) < 100)) {
            std::cout << "Test2 successful! :)\n";
        }

        // ----------------- //
        // PickIndex Testing //
        // ----------------- //


        // -----------------//
        // generate_random_int Testing//
        // -----------------//


        double sum1 = 0;
        double sum2 = 0;

        int Occurences[5] = {0, 0, 0, 0, 0};

        for (int i = 0; i < 50000; i++) {
            sum1+= generate_random_int(0, 20, i);
            sum2+= generate_random_int(1, 20, i+3);
        }

        std::cout << "First average is: " << sum1/5000 <<"\n";
        std::cout << "Second average is: " << sum2/5000 <<"\n";

        if (abs((sum1/50000)-10) < 0.1 && abs((sum2/50000)-10.5) < 0.1) {
            std::cout << "Test1 for generate_random_int passed :)";
        }

        for (int i = 0; i < 1000000; i++) {
            Occurences[generate_random_int(0, 4, i)]++;
        }

        bool test2pass = true;

        for (int i = 0; i < 5; i++) {
            // cout << abs(Occurences[i]-200000) << "\n";
            if (abs(Occurences[i] - 200000) > 1000) {
                test2pass = false;
            }
        }

        if (test2pass) {
            std::cout << "Test2 for generate_random_int passed :)";
        }

        // --------------------------- //
        // generate_random_int Testing //
        // --------------------------- //

        // --------------- //
        // yesorno Testing //
        // --------------- //

        double probyes = 0.01;

        int total = 0;

        for (int i = 0; i < 100000; i++) {
            if (yesorno(probyes)) {
                total++;
            }
        }

        if (abs(total - 1000) < 50) {
            std::cout << "test passed\n";
        }

        std::cout << "\n" << "Yesorno: " << total;

        // ----------------- //
        // yesorno Testing //
        // ----------------- //

        // time();
}
