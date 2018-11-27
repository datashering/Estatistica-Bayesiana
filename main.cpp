#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <random>
#include <boost/math/distributions/beta.hpp>
#include "Metodos.h"

int main()
{
  double alpha = 2, beta = 5;
  std::vector<double> x;
  x.resize(100);

  for (int i = 0; i < x.size(); i++)
  {
    if (i%2 == 0)
    {
      x[i] = 1;
    }
    else
    {
      x[i] = 0;
    }
  }

  SIR(x, alpha, beta, 100);
  Rejection(x, alpha, beta, 2000);
  Metropolis_Hastings(x, alpha, beta, 100);

  return 0;
}
