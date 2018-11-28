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

  std::normal_distribution<double> normal_dist(2,5);
  for (int i = 0; i < x_norm.size(); i++)
  {
    x_norm[i] = normal_dist(generato);
  }

  // SIR(x, alpha, beta, 100);
  // Rejection(x, alpha, beta, 2000);
  // Metropolis_Hastings(x, alpha, beta, 100);
  Gibbs_Sampling(x_norm, 10, 1, 2, 1, 100);

  return 0;
}
