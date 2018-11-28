#pragma once

double beta_density(double, double, double);

double bernoulli_verossimi(std::vector<double>, double);

void SIR(std::vector<double>, double, double, double);

void Rejection(std::vector<double>, double, double, double);

void Metropolis_Hastings(std::vector<double>, double, double, double);

void Gibbs_Sampling(std::vector<double>, double, double, double, double, double);
