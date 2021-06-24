// Copyright [2021] <Group fluorite>
#include "CCircuit.h"
#include "CUnit.h"
#include "Genetic_Algorithm.h"

/*-------------------------------------------------------------------------------------------------*/

// the interface for our software

/*-------------------------------------------------------------------------------------------------*/

std::vector<int> bestvector(2 * num_units + 1);

double bestval = -50000;

void introduction() {
  std::cout << "+-----------------------------------------------+\n";
  std::cout << "+       Welcome to our software :)              +\n";
  std::cout << "+-----------------------------------------------+\n";
  std::cout << "+  It will help you find the best arrangement   +\n";
  std::cout << "+  of units to get the best value out of your   +\n";
  std::cout << "+  Gormanium extraction center! :)              +\n\n";
  std::cout << "+  If you want to change the number of units    +\n";
  std::cout << "+  You can access it in CCircuit.h              +\n";
  std::cout << "+  If you want to change the population size    +\n";
  std::cout << "+  It's a global var in Genetic_Algorithm.cpp   +\n";
  std::cout << "+-----------------------------------------------+\n";
}

void user_guide() {
  std::cout << "\n\n";
  std::cout << "+-----------------------------------------------+\n";
  std::cout << "|               User guide:                     |\n";
  std::cout << "|                                               |\n";
  std::cout << "|                                               |\n";
  std::cout << "+-----------------------------------------------+\n";
  std::cout << "|  1. Input 1 for an introduction               |\n";
  std::cout << "|  2. Input 2 to run the Genetic Algorithm      |\n";
  std::cout << "|  for different parameters.                    |\n";
  std::cout << "|  The software will provide clear instruction  |\n";
  std::cout << "|  for each parameter.                          |\n";
  std::cout << "|  3. Input 3 to see the MIT License for this   |\n";
  std::cout << "|  software.                                    |\n";
  std::cout << "+-----------------------------------------------+\n";
}

void license() {
    std::cout <<"\n\n";
    std::cout <<" +-----------------------------------------------------------------------------------+\n";
    std::cout <<" |    MIT License                                                                    |\n";
    std::cout <<" |    Copyright (c) 2021 group fluorite                                              |\n";
    std::cout <<" |    Permission is hereby granted, free of charge, to any person obtaining a copy   |\n";
    std::cout <<" |   of this software and associated documentation files (the 'Software'), to deal   |\n";
    std::cout <<" |   in the Software without restriction, including without limitation the rights    |\n";
    std::cout <<" |   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       |\n";
    std::cout <<" |   copies of the Software, and to permit persons to whom the Software is           |\n";
    std::cout <<" |   furnished to do so, subject to the following conditions:                        |\n";
    std::cout <<" |                                                                                   |\n";
    std::cout <<" |   The above copyright notice and this permission notice shall be included in      |\n";
    std::cout <<" |   copies or substantial portions of the Software.                                 |\n";
    std::cout <<" |                                                                                   |\n";
    std::cout <<" |   THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      |\n";
    std::cout <<" |   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        |\n";
    std::cout <<" |   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     |\n";
    std::cout <<" |   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          |\n";
    std::cout <<" |   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   |\n";
    std::cout <<" |   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   |\n";
    std::cout <<" |   SOFTWARE.                                                                       |\n";
    std::cout <<" +-----------------------------------------------------------------------------------+\n";
}

void menu() {
  std::cout << "\n\n";
  std::cout << "+-----------------------------------------------+\n";
  std::cout << "|                     Test                      |\n";
  std::cout << "+-----------------------------------------------+\n";
  std::cout << "|   1.Introduction                              |\n";
  std::cout << "|   2.Genetic Algorithm                         |\n";
  std::cout << "|   3.License                                   |\n";
  std::cout << "+-----------------------------------------------+\n";
  std::cout << "|                    0.exit                     |\n";
  std::cout << "+-----------------------------------------------+\n";
  std::cout << "\n\n";
}

void cout_again() {
  std::cout << "Want to keep going?"
            << "\n";
  std::cout << "1.Introduction\n2.Genetic_Algorithm\n3.License.\n0.Exit\n";
}

void goodbye() {
  std::cout << "\n\n";
  std::cout << "+-----------------------------------------------+\n";
  std::cout << "|              Goodbye!                         |\n";
  std::cout << "|                                               |\n";
  std::cout << "|  If you have any suggestions or questions,    |\n";
  std::cout << "|  please don't hesitate to contact our Team    |\n";
  std::cout << "+-----------------------------------------------+\n";
}

/*-------------------------------------------------------------------------------------------------*/

// test our genetic_algorithm with loops determined in different situations
// which users specify

/*-------------------------------------------------------------------------------------------------*/
void test_GA() {
  int input;  // this is the input from user
  std::vector<double> print_fitness;
  do {
    menu();
    int choice[]{0, 1, 2, 3};  // store the choice of we have in the interface
    char str[20];
    std::cout << "Input your choice: ";
    while (1) {            // loop over
      std::cin >> input;   // user input
      while (!std::cin) {  // if the input is not valid
        std::cin.clear();
        std::cin >> str;
        std::cout << "Read your options again! :) " << std::endl;
        std::cin >> input;  // input again
      }

      if (choice[input] == 0) {  // if input is 0, then exit
        goodbye();
        break;
      }

      switch (choice[input]) {  // 3 different cases
        case 1:
          introduction();
          cout_again();
          break;

        case 2:  // test our GA with user-defined inputs
          int generation, print, write, loop_time;
          double crossover, mutation;
          std::cout << "FYI: the GA isn't guaranteed to converge to the optimal solution.\nMultiple runs increase your chance of convergence to the optimal solution\n";
          std::cout << "How many times do you want to run the algorithm?\n";
          std::cin >> loop_time;
          print_fitness.resize(loop_time);
          std::cout << "How many generations do you want each run to last?\n";
          std::cin >> generation;  // input user-defined number of generations

          std::cout
              << "What do you want the crossover probability to be? (e.g. type 0.8 for 80%) :";
          std::cin >> crossover;  // input user-defined mutation rate

          std::cout << "What do you want your mutation probability to be? (e.g. type 0.01 for 1%) : "
                       "0.01: ";
          std::cin >> mutation;  // input user-defined mutation rate

          std::cout << "Do you want it to run verbose? (0 for no, 1 for yes) \n";
          std::cin >> print;  // input if user want to print or not

          std::cout << "Do you want it to save to .txt? (0 for no, 1 for yes) \n";
          std::cin >> write;  // input if user want to write to txt file or not

          for (int i = 0; i < loop_time; i++) {
            std::vector<int> current(2 * num_units + 1);

            current = Genetic_Algorithm(10, 0.01, 20, generation, crossover,
                                        mutation, print, write);
            double currentfitness = Evaluate_Circuit(current, 1e-6, 1000);

            if (currentfitness > bestval) {
              bestval = currentfitness;
              bestvector = current;
            }
          }

          std::cout << "\n\nBest vector found: ";

          for (int i = 0; i < 2 * num_units + 1; i++) {
            std::cout << bestvector[i] << " ";
          }

          std::cout << "\nWith Fitness: " << bestval;
          std::cout << "\n";
          cout_again();
          break;

        case 3:
          license();
          cout_again();
          break;

        default:

          if (choice[input] == 0) {
            goodbye();
            break;
          } else if (choice[input] == 1 || choice[input] == 2 ||
                     choice[input] == 3) {
            break;
          } else {
            std::cout << "please input correct input!" << std::endl;
          }
          break;
      }
    }
  } while (input);
}

int main(int argc, char* argv[]) {
  int circuit_array[2 * num_units + 1] = {6, 11, 7,  7,  2, 11, 7, 3,  5, 3, 6,
                                          0, 2,  11, 10, 1, 11, 5, 11, 5, 11};
  std::vector<int> circuit_vector(circuit_array,
                                  circuit_array + 2 * num_units + 1);
  test_GA();
  return EXIT_SUCCESS;
}
