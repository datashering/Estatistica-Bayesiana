#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <random>
#include <boost/math/distributions/beta.hpp>
#include <boost/math/distributions/inverse_gamma.hpp>
#include "Metodos.h"

std::mt19937 generato(clock());

int main()
{
  double alpha = 2, beta = 5;
  std::vector<double> x_ber, x_norm;
  x_ber.resize(100);
  x_norm.resize(50);

  for (int i = 0; i < x_ber.size(); i++)
  {
    if (i%2 == 0)
    {
      x_ber[i] = 1;
    }
    else
    {
      x_ber[i] = 0;
    }
  }

  std::ofstream newfile;
  newfile.open("bum.csv");
  std::normal_distribution<double> normal_dist(2,5);
  for (int i = 0; i < x_norm.size(); i++)
  {
    x_norm[i] = normal_dist(generato);
  }
  newfile << x_norm[0];
  for (int i = 1; i < x_norm.size(); i++)
  {
    newfile << ",";
    newfile << x_norm[i];
  }
  newfile.close();

  // SIR(x, alpha, beta, 100);
  // Rejection(x, alpha, beta, 2000);
  // Metropolis_Hastings(x, alpha, beta, 100);
  Gibbs_Sampling(x_norm, 0, 100, 0.01, 0.01, 100000);

  return 0;
}
