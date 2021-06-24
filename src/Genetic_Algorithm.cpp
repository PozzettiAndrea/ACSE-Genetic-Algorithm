// Copyright [2021] <Group fluorite>
#include "Genetic_Algorithm.h"

#include <stdio.h>

#include <algorithm>
#include <chrono> // NOLINT [build/c++11]
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "CCircuit.h"
#include "CUnit.h"


/*-------------------------------------------------------------------------------------------------*/

#ifdef _OPENMP
#include <omp.h>
// getting maximum number of threads available
static const int MAX_THREADS = omp_get_max_threads();
#endif

/*-------------------------------------------------------------------------------------------------*/

// global varibles

/*-------------------------------------------------------------------------------------------------*/
// Popsize can not be even since there is always 0 in first place
int PopSize = 101;
int gencounter = 0;
int randcounter = 0;

std::vector<std::vector<int>> population(PopSize, std::vector<int>(2 * num_units + 1));
std::vector<std::vector<int>> newpopulation(PopSize, std::vector<int>(2 * num_units + 1));
std::vector<double> fitness(PopSize);
std::vector<double> probabilities(PopSize);

std::vector<int> child1(2 * num_units + 1);
std::vector<int> child2(2 * num_units + 1);

/*-------------------------------------------------------------------------------------------------*/

CCircuit CChecker;
extern std::vector<CUnit> units;

/**************************************************************************
 * Function: setConstantCUnit
 * Description: set same initial state of every unit
 * Input: void
 * Return: void
 **************************************************************************/
static void setConstantUnits() {
  for (CUnit &unit : units) {
    unit.gormanium_feed_rate = 10;
    unit.waste_feed_rate = 100;
  }
}


/**
 * Function: Evaluate_Circuit
 * Description: evaluate the circuit described by circuit_vector
 **/

double Evaluate_Circuit(std::vector<int> &circuit_vector, double tolerance,
                        int max_iterations) {
  int vector_size = 2 * num_units + 1;
  bool converged = false;

  double Performance = 0.0;

  double concentrate_gormanium = 0.0;
  double concentrate_waste = 0.0;
  double tailing_gormanium = 0.0;
  double tailing_waste = 0.0;

  static const double gormanium_profit = 100.0;
  static const double waste_penalty = 500.0;

  int concentrate_idx = num_units;
  int tailing_idx = num_units + 1;

  double err1, err2;

  // Give an initial guess for the feed rate of both components to every cell in the circuit
  // The input is 10kg/s gormanium and 100kg/s waste
  setConstantUnits();

  // The first calculation of flowrate of each component
  for (CUnit &unit : units) {
    unit.recoverStream();
  }

  // This part is to modelling the circuit
  std::vector<double[2]> old_val(num_units);
  int it = 0;
  for (; it < max_iterations; it++) {
    // Store the current value of the feed to each cell as an old feed value
    // and set the current value for all components to zero
    for (int i = 0; i < num_units; i++) {
      old_val[i][0] = units[i].gormanium_feed_rate;
      old_val[i][1] = units[i].waste_feed_rate;
      units[i].gormanium_feed_rate = 0.0;
      units[i].waste_feed_rate = 0.0;
    }
    concentrate_gormanium = 0.0;
    concentrate_waste = 0.0;
    tailing_gormanium = 0.0;
    tailing_waste = 0.0;

    // Every iteration should give the feed units the same input component
    units[circuit_vector[0]].gormanium_feed_rate = 10;
    units[circuit_vector[0]].waste_feed_rate = 100;

    // update the estimate for the overall circuit concentrate and tailings streams's flow
    int src = 0;
    // except for the final concentrate and final tailing, the units[circuit_vector[i]] should get
    // concentrate stream, units[circuit_vector[i+1]] should get tailing stream
    for (int i = 1; i < vector_size; i += 2) {
      // If this units is the final concentrate, add all the concentrate stream
      // from the units which concentrate number is the final concentrate index
      if (circuit_vector[i] == concentrate_idx) {
        concentrate_gormanium += units[src].concentrate_stream[0];
        concentrate_waste += units[src].concentrate_stream[1];
      } else if (circuit_vector[i] == tailing_idx) {
        // If this units is the final tailing, add all the tailing stream
        // from the units which tailing number is the final tailing index
        tailing_gormanium += units[src].concentrate_stream[0];
        tailing_waste += units[src].concentrate_stream[1];
      } else {
        // otherwise just add the feed from every unit which will contribuite to it
        units[circuit_vector[i]].gormanium_feed_rate +=
            units[src].concentrate_stream[0];
        units[circuit_vector[i]].waste_feed_rate +=
            units[src].concentrate_stream[1];
      }

      // If this units is the final concentrate, add all the concentrate stream
      // from the units which concentrate number is the final concentrate index
      if (circuit_vector[i + 1] == concentrate_idx) {
        concentrate_gormanium += units[src].tail_stream[0];
        concentrate_waste += units[src].tail_stream[1];
      } else if (circuit_vector[i + 1] == tailing_idx) {
        // If this units is the final tailing, add all the tailing stream
        // from the units which tailing number is the final tailing index
        tailing_gormanium += units[src].tail_stream[0];
        tailing_waste += units[src].tail_stream[1];
      } else {
        // otherwise just add the feed from every unit which will contribuite to it
        units[circuit_vector[i + 1]].gormanium_feed_rate +=
            units[src].tail_stream[0];
        units[circuit_vector[i + 1]].waste_feed_rate +=
            units[src].tail_stream[1];
      }
      src++;
    }

    // do recovery once again
    for (CUnit &unit : units) {
      unit.recoverStream();
    }

    // check output flow rates
    // compare the final concentrate and final tailing with the feed input
    // which is 10kg/s gormanium and 100kg/s waste
    // the difference should less than 1e-3
    converged = false;
    err1 = std::abs(concentrate_gormanium + tailing_gormanium - 10) / 10;
    if (err1 > 1e-3) {
      continue;
    }
    err2 = std::abs(concentrate_waste + tailing_waste - 100) / 100;
    if (err2 > 1e-3) {
      continue;
    }

    // Check the difference between the newly calculated feed rate and the old feed rate for each cell
    // If any of them have a relative change that is above a given threshold (1.0e-6)
    // and then repeat this whole iteration.
    converged = true;
    for (int i = 0; i < num_units; i++) {
      err1 = abs(old_val[i][0] - units[i].gormanium_feed_rate) / old_val[i][0];
      err2 = abs(old_val[i][1] - units[i].waste_feed_rate) / old_val[i][1];
      if ((err1 >= tolerance) || (err2 >= tolerance)) {
        converged = false;
        break;
      }
    }
    if (converged) {
      break;
    }
  }

  // if this circuit become stable, compute the performance
  if (converged) {
    Performance = gormanium_profit * concentrate_gormanium -
                  waste_penalty * concentrate_waste;
  } else {
    // the worst performance is the flowrate of waste in the feed times the
    // value of the waste, which is a negative number
    Performance = -(waste_penalty * 100);
  }
  return Performance;
}

// initialise our popluation by using random generator
void initialisePopulation(std::vector<std::vector<int>> &population) {
  // Start with the vectors representing the initial random collection of valid
  // circuits.
  for (int i = 0; i < PopSize; i++) {
    population[i][0] = 0;

    // We iterate through each member of our population and we fill it with
    // random numbers
    for (int j = 0; j < num_units; j++) {
      population[i][2 * j + 1] =
          generate_random_int(0, num_units + 1, randcounter);  // Concentrate stream
      population[i][2 * j + 2] =
          generate_random_int(0, num_units + 1, j);  // Tailings stream
    }

    if ((!CChecker.Check_Validity(population[i]))) {
      i--;
    }
  }
}
/*-------------------------------------------------------------------------------------------------*/

// * random generator for different types
// * Description: set random generators for int and double number,
// * and use inline to reduce function call overhead
// * Return: int and double

/*-------------------------------------------------------------------------------------------------*/
int generate_random_int(int low, int high, int added = randcounter) {
  unsigned seed =
      randcounter *
      static_cast<unsigned>(
          std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::mt19937 rand_num(seed+added);
  std::uniform_int_distribution<int> dist(low, high);

  randcounter++;

  if (randcounter > 1000000) {
    randcounter = 0;
  }

  return dist(rand_num);
}

double random_0_1(double a, double b) {
  unsigned seed =
      randcounter *
      static_cast<unsigned>(
          std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::default_random_engine rand_num(seed);
  std::uniform_real_distribution<> dist(a, b);

  randcounter++;

  if (randcounter > 1000000) {
    randcounter = 0;
  }

  return dist(rand_num);
}

/*-------------------------------------------------------------------------------------------------*/

// * Description: this is the function for whether we should crossover, mutate
// or not
// * bool type function
// * Return: bool datatype

/*-------------------------------------------------------------------------------------------------*/
bool yesorno(double probability) {
  double x;
  x = random_0_1(0.0, 1.0);

  if (x > probability)
    // if x > probability, we don't want to crossover or mutate
    return false;
  else
    // if x < probability, we don't want to crossover or mutate
    return true;
}

/*-------------------------------------------------------------------------------------------------*/

// CROSS-OVER METHODS
// * Description: three different types of crossover: single point, two point,
// uniform
// * The UXO(uniform crossover method) is chosen for further optimisation

/*-------------------------------------------------------------------------------------------------*/
// one-point random cross-over function
// to generate parents index by using find_index func
// parent1_index = pickIndex(fitness)
// Crossoverfunction
void crossoverfunction(int index1, int index2) {
  // generate our crossover point
  int cpoint = generate_random_int(1, num_units - 1, randcounter);

  // Add the unchanged part to the children
  for (int i = 0; i < cpoint * 2 + 1; i++) {
    child1[i] = population[index1][i];
    child2[i] = population[index2][i];
  }

  // Add the swapped bit (after the crossover point)
  for (int i = cpoint * 2 + 1; i < num_units * 2 + 1; i++) {
    child1[i] = population[index2][i];
    child2[i] = population[index1][i];
  }
}

/*-------------------------------------------------------------------------------------------------*/
// two-point random cross-over function
void P2RX(int index1, int index2) {
  int cpoint1 = generate_random_int(1, num_units - 1, randcounter);
  int cpoint2 = generate_random_int(1, num_units - 1, randcounter);
  // order the two points
  int pos1 = std::min(cpoint1, cpoint2);
  int pos2 = std::max(cpoint1, cpoint2);
  // Add the unchanged part to the children
  for (int i = 0; i < pos1 * 2 + 1; i++) {
    child1[i] = population[index1][i];
    child2[i] = population[index2][i];
  }
  // add the changed part to the children

  for (int i = pos1 * 2 + 1; i < pos2 * 2 + 1; i++) {
    child1[i] = population[index2][i];
    child2[i] = population[index1][i];
  }

  for (int i = pos2 * 2 + 1; i < num_units; i++) {
    child1[i] = population[index1][i];
    child2[i] = population[index2][i];
  }
}

/*-------------------------------------------------------------------------------------------------*/
// discussion: it could also add multiple-point random cross-over, but it would
// break some good pattern
/*-------------------------------------------------------------------------------------------------*/
// uniform crossover of two child
void UXO(int index1, int index2) {
  for (int i = 0; i < num_units * 2 + 1; i++) {
    if (yesorno(0.5)) {
      child1[i] = population[index1][i];
      child2[i] = population[index2][i];
    } else {
      child1[i] = population[index2][i];
      child2[i] = population[index1][i];
    }
  }
}

/*-------------------------------------------------------------------------------------------------*/

// MUTATE METHODS
// * Description: two different types of mutation: single point, uniform
// * The single point mutation is chosen based on the effort on the observation
// and testing,
// * which provide better convergence

/*-------------------------------------------------------------------------------------------------*/

void mutate(double Pmutation) {
  // // mutating the first child
  // if (yesorno(Pmutation)) {
  //   child1[generate_random_int(1, 2 * num_units + 1,randcounter)] =
  //       generate_random_int(0, num_units + 1, 3);
  // }

  // // mutating the second child
  // if (yesorno(Pmutation)) {
  //   // CHECK THIS AGAIN
  //   child2[generate_random_int(1, 2 * num_units + 1,randcounter)] =
  //       generate_random_int(0, num_units + 1,randcounter);
  // }
  // mutating the first child
  for (int i = 1; i < 2*num_units+1; i++) {
    if (yesorno(Pmutation)) {
      child1[i] = generate_random_int(0, num_units + 1, i);
    }
  }

  for (int i = 1; i < 2*num_units+1; i++) {
    if (yesorno(Pmutation)) {
      child2[i] = generate_random_int(0, num_units + 1, i);
    }
  }
}

/*-------------------------------------------------------------------------------------------------*/

// Uniform mutation: replacing our child by new one
void UNM(double Pmutation) {
  if (yesorno(Pmutation)) {
    // loop over from the the second place in child since first place will
    // always be zero
    for (int i = 1; i < num_units * 2 + 1; i++) {
      child1[i] = generate_random_int(
          0, num_units + 1, i);  // replace child1[i] to random int number
      break;
    }
  }

  // for child2
  if (yesorno(Pmutation)) {
    for (int i = 1; i < num_units * 2 + 1; i++) {
      child2[i] = (child2[i] + generate_random_int(1, num_units - 1, randcounter)) %
                  num_units;  // replace child2[i] to random int number
      break;
    }
  }
}

/*-------------------------------------------------------------------------------------------------*/
// function to swap our genes
void swapgenes(double pswap) {
  // mutating the first child
  if (yesorno(pswap)) {
    int gene = generate_random_int(1, num_units, randcounter);
    int storage = child1[1+2*gene];
    child1[1+2*gene] = child1[2+2*gene];
    child1[2+2*gene] = storage;
  }
  // mutating the first child
  if (yesorno(pswap)) {
    int gene = generate_random_int(1, num_units, randcounter);
    int storage = child2[1+2*gene];
    child2[1+2*gene] = child2[2+2*gene];
    child2[2+2*gene] = storage;
  }
}

/*-------------------------------------------------------------------------------------------------*/

// SELECTION METHODS
// * three different types of selection methods: tourment, stochastic universal
// sampling,
// * and own-designed methods
// * The own-designed pickIndex method is chosen based on the effort on the
// observation and testing,
// * which optimise our results based on avaiable methods

/*-------------------------------------------------------------------------------------------------*/
// tourment pickIndex by using
int pickIndex_tour(std::vector<double> &fitnessesTCF) {
  int best = 0;                   // index to return
  int num = fitnessesTCF.size();  // number of fitnesses to consider
  for (int i = 0; i < num; i++) {   // loop to find best index
    int index = generate_random_int(1, num_units+1, randcounter);  // random select index
    if ((best == 0) || fitnessesTCF[index] > fitnessesTCF[best]) {
      best = index;  // update the best to the index
    }
  }
  return best;
}

// own-designed pickIndex function
int pickIndex(std::vector<double> &fitnessesTCF, double min, double max) {
  int index = 0;                  // index to return
  int num = fitnessesTCF.size();  // number of fitnesses to consider

  double sum_of_probability_differentials =
      0.00;  // sum of differentials, for later normalisation

  for (int i = 0; i < num; i++) {
    probabilities[i] =
        pow(((fitnessesTCF[i] - min) / (max - (min - 1))),
            1);  // we store the differentials in the probabilities vector
    sum_of_probability_differentials += probabilities[i];
  }

  double topick = random_0_1(0.0, sum_of_probability_differentials - 0.1);

  if (index == num - 1) {
    return index;
  }

  while (topick >= 0 && index < num - 1) {
    topick -= probabilities[index];
    index++;
  }
  return index - 1;  // return index for parents
}

// stochastic universal sampling selection
int pickIndex_SUS(std::vector<double> &fitnessesTCF) {
  int index_to_return = 0;        // index of index to return
  int num = fitnessesTCF.size();  // number of fitnesses to consider
  // sum of our fitness level in vector
  double sum_of_fitness =
      accumulate(fitnessesTCF.begin(), fitnessesTCF.end(), 0);
  double dist = sum_of_fitness / num;  // compute interval size
  double ptr = random_0_1(0.0, dist);  // this is pointer

  // select
  for (int i = 0; i < num; i++) {
    int index = 0;    // index to return
    int fit_sum = 0;  // initialise fit_sum is 0

    while (fit_sum <= ptr) {
      fit_sum += fitnessesTCF[index];
      index++;
    }
    ptr += dist;  // increase pointer
    index_to_return = index;
  }
  return index_to_return - 1;
}

/*-------------------------------------------------------------------------------------------------*/
// EVOLUTION FUNCTION
// evolution step for crossover and mutation, if verbose is true, it will print
// each step; if savetotxt is true, output data to txt file
// we also specific some conditions for the better performance
/*-------------------------------------------------------------------------------------------------*/
void Evolution1step(std::vector<std::vector<int>> &population, double tolerance,
                    int max_iterations, double PXover, double Pmutation,
                    bool verbose, bool savetotxt) {
  if (verbose) {
    std::cout << "Step 1: \n";
  }

  if (verbose) {
    std::cout << "Calculating the fitness value for each of the " << PopSize
         << " vectors \n";
  }

  double sum = 0;
  // We calculate the fitness value for each of the initialised vectors.
  for (int i = 0; i < PopSize; i++) {
    double eval = Evaluate_Circuit(population[i], 1e-6, 1000);
    fitness[i] = eval;
    sum += eval;
  }

  if (verbose) {
    std::cout << "Finding the minimum and maximum fitness values \n";
  }

  // define the max and min index
  int maxIndex = max_element(fitness.begin(), fitness.end()) - fitness.begin();
  int minIndex = min_element(fitness.begin(), fitness.end()) - fitness.begin();
  double max = fitness[maxIndex];
  double min = fitness[minIndex];

  if (verbose) {
    std::cout << "Maximum fitness is:" << max << "\n";
    std::cout << "Minimum fitness is:" << min << "\n";
  }


  // Take the best vector (the one with the highest fitness value) into the
  // child list unchanged (you want to keep the best solution).
  newpopulation[0] = population[maxIndex];

  // Crossover: Select a pair of the parent vectors with a probability that
  // depends on the fitness value. In this case you might want to start by using
  // a probability that either varies linearly between the minimum and maximum
  // finesses of the current population. This should be done “with replacement,”
  // which means that parents should be able to be selected more than once.
  int parent1_index;
  int parent2_index;

  // for time based randomness

  // You now wish to create nchild vectors
  int nchild = 1;  // We currently have 1 "child" which is our best vector

  while (true) {
    // We decide whether they cross over or not
    bool crossover = yesorno(PXover);

    // If they cross over, we cross them over and add their children to our next
    // population
    if (crossover) {
      while (true) {
        {
          // We pick our parents
          parent1_index = pickIndex(fitness, min, max);
          parent2_index = pickIndex(fitness, min, max);

          // to pickIndex for the index of parent2 again if index on parent1 and
          // 2 are equal, or elements on such index are the same
          while (parent1_index == parent2_index ||
                 population[parent1_index] == population[parent2_index]) {
            parent2_index = pickIndex(fitness, min, max);
          }

          if (verbose) {
            std::cout << "Parents are vectors " << parent1_index << " and "
                 << parent2_index << "\n";

            for (int i = 0; i < 2 * num_units + 1; i++) {
              std::cout << population[parent1_index][i] << " ";
            }
            std::cout << "\n";

            for (int i = 0; i < 2 * num_units + 1; i++) {
              std::cout << population[parent2_index][i] << " ";
            }
            std::cout << "\n";
          }

          if (verbose) {
            std::cout << "Crossing over parents " << parent1_index << " and "
                 << parent2_index << " \n";
          }

          // cross their genes to generate the children
          crossoverfunction(parent1_index, parent2_index);

          if (verbose) {
            std::cout << "Mutating: \n";
          }

          // mutate their children
          mutate(Pmutation);

          // if their children are valid, add them to the next generation, else
          // repeat
          if (CChecker.Check_Validity(child1) &&
              CChecker.Check_Validity(child2) &&
              (Evaluate_Circuit(child1, 1e-6, 1000) != -50000.0) &&
              (Evaluate_Circuit(child2, 1e-6, 1000) != -50000.0)) {
            if (verbose) {
              std::cout << "Children are valid.\n";
            }

            // MAKE THE DEEP COPYING A LITTLE BIT MORE ELEGANT
            if (verbose) {
              std::cout << "Adding two vecs to our result, positions " << nchild
                   << " and " << nchild + 1 << "\n";
              std::cout << "\n";
            }

            // update our child1 to newpoulation at position nchild and j
            for (int j = 0; j < num_units * 2 + 1; j++) {
              newpopulation[nchild][j] = child1[j];
            }

            // update our child2 to newpoulation at position nchild+1 and j
            for (int j = 0; j < num_units * 2 + 1; j++) {
              newpopulation[nchild + 1][j] = child2[j];
            }
            nchild += 2;
            break;
          }
        }
      }
    } else {
      // else we add the unchanged parent vectors to the next generation

      // We pick our parents
      parent1_index = pickIndex(fitness, min, max);
      parent2_index = pickIndex(fitness, min, max);

      while (parent1_index == parent2_index) {
        parent2_index = pickIndex(fitness, min, max);
      }

      if (verbose) {
        std::cout << "adding vectors " << parent1_index << " and " << parent2_index
             << " to the next generation\n";
      }
      // update our newpoplulation
      newpopulation[nchild] = population[parent1_index];
      newpopulation[nchild + 1] = population[parent2_index];
      nchild += 2;
    }

    // break when number of childs are equal the size of population
    if (nchild == PopSize) {
      break;
    }
  }

  /*----------------------------------------------------------------------------------*/
  // save the data if user specific it to 1
  if (savetotxt) {
    std::ofstream outfile;
    outfile.open("Generations.txt", std::ios_base::app);

    // If we are in the first generation, print the origin population
    if (gencounter == 0) {
      outfile << "\nGeneration:" << 0 << ". Maximum Fitness: " << max << "\n";
      std::cout << "Maximum Fitness: " << max << "\n";

      // save data of population to txt file
      for (int i = 0; i < PopSize; i++) {
        outfile << "Vector " << i << ": ";
        for (int j = 0; j < 2 * num_units + 1; j++) {
          outfile << population[i][j] << " ";
        }
        // save data of fitness and if the population are valid
        outfile << ". Fitness: " << Evaluate_Circuit(population[i], 1e-6, 1000);
        outfile << ". Validity: " << CChecker.Check_Validity(population[i]);
        outfile << "\n";
      }
    }

    // Add a generation indicator
    outfile << "\nGeneration:" << gencounter + 1 << ". Maximum Fitness: " << max
            << "\n";

    double average = 0;
    for (int i = 0; i < PopSize; i++) {
      outfile << "Vector " << i << ": ";
      for (int j = 0; j < 2 * num_units + 1; j++) {
        outfile << newpopulation[i][j] << " ";
      }

      double eval = Evaluate_Circuit(newpopulation[i], 1e-6, 1000);
      average += eval;
      outfile << ". Fitness: " << eval;
      outfile << ". Validity: " << CChecker.Check_Validity(population[i]);
      outfile << "\n";
    }
    std::cout << "Maximum Fitness: " << max << "Average Fitness" << average / PopSize
         << "\n";

    outfile << "Average Fitness is: " << average / PopSize << "\n";
  }
  /*----------------------------------------------------------------------------------*/
  // Replacing parent vectors with child vectors for the next step
  for (int i = 0; i < PopSize; i++) {
    // MAKE THE DEEP COPYING A LITTLE BIT MORE ELEGANT
    population[i] = newpopulation[i];
  }

  gencounter++;
}
/*-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------*/
// Genetic Algorithm FUNCTION
// step1: initialisation of population of solution vectors
// step2: Iterate through certain number of generations
/*-------------------------------------------------------------------------------------------------*/

std::vector<int> Genetic_Algorithm(int PopSize, double tolerance, int max_iterations,
                       int e_steps, double PXover, double Pmutation,
                       bool verbose, bool savetotxt) {

  // Step 1:
  initialisePopulation(population);

  // We have opted to make a function for each iteration because of tinkering
  // ease. We could easily change the probabilities of crossover or mutation or
  // stop within a certain tolerance
  // step 2:

  // #ifdef _OPENMP
  //  #pragma omp parallel for num_threads(MAX_THREADS)
  // #endif
  for (int i = 0; i < e_steps; i++) {
    std::cout << "Generation" << gencounter + 1 << "\n";
    Evolution1step(population, tolerance, max_iterations, PXover, Pmutation,
                   verbose, savetotxt);
    std::cout << "\n";
  }

  std::cout << "Best vector is: ";

  for (int i = 0; i < 2 * num_units + 1; i++) {
    std::cout << population[0][i] << " ";
  }
  std::cout << "\n";
  std::cout << "Fitness: ";
  std::cout << Evaluate_Circuit(population[0], 1e-6, 1000);
  std::cout << "\n";
  return population[0];
}

/*-------------------------------------------------------------------------------------------------*/

// function to print current time of every time we execute
void time() {
#define TIME_SIZE 40
  static char time_buffer[TIME_SIZE];
  struct tm *tm;

  time_t now;

  now = time(NULL);
  tm = localtime(&now);
  strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);

  std::cout << time_buffer << "\n";

  return;
#undef TIME_SIZE
}
