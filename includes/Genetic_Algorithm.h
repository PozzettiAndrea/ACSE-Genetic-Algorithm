// Copyright [2021] <FLUORITE>
#ifndef INCLUDES_GENETIC_ALGORITHM_H_
#define INCLUDES_GENETIC_ALGORITHM_H_

#include <iostream>
#include <vector>

double Evaluate_Circuit(std::vector<int> &vector, double tolerance = 1e-2,
                        int max_iterations = 1000);

int generate_random_int(int low, int high, int added);

void initialisePopulation(std::vector<int*> population);

double random_0_1(double a, double b);

bool yesorno(double probability);

/*-------------------------------------------------------------------------------------------------*/

void crossoverfunction(int index1, int index2);

void P2RX(int index1, int index2);

void UXO(int index1, int index2);

/*-------------------------------------------------------------------------------------------------*/

void mutate(double Pmutation);

void UNM(double Pmutation);

/*-------------------------------------------------------------------------------------------------*/

// int pickIndex(std::vector<double> fitnessesTCF);

int pickIndex(std::vector<double>& fitnessesTCF, double min, double max);

int pickIndex_tour(std::vector<double>& fitnessesTCF);

int pickIndex_SUS(std::vector<double>& fitnessesTCF);

/*-------------------------------------------------------------------------------------------------*/

void Evolution1step(std::vector<std::vector<int>>& population, double tolerance, int max_iterations, double PXover, double Pmutation, bool verbose, bool savetotxt);

std::vector<int> Genetic_Algorithm(int PopSize, double tolerance, int max_iterations, int e_steps, double PXover, double Pmutation, bool verbose, bool savetotxt);

void time();

#endif  // INCLUDES_GENETIC_ALGORITHM_H_
