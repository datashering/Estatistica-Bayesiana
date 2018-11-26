#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <boost/math/distributions/beta.hpp>

double beta_density(double x, double alpha, double beta)
{
  double beta_func = (std::tgamma(alpha) * std::tgamma(beta))/std::tgamma(alpha + beta);
  return (std::pow(x, alpha - 1) * std::pow((1-x), beta - 1))/beta_func;
}

double bernoulli_density(std::vector<double> x, double p)
{
  double sum = 0;
  for (int i = 0; i < x.size(); i++)
  {
    sum += x[i];
  }

  return (pow(p, sum) * pow(1-p, (x.size()-sum)));
}

double SIR(double alpha, double beta, double J)
{
  std::vector<double> samples;
  samples.resize(J);

  std::mt19937 generator(clock());
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  boost::math::beta_distribution<> mybeta(alpha, beta);
  double randFromUnif = distribution(generator);

  for (int i = 0; i < J; i++)
  {
    samples[i] = quantile(mybeta, randFromUnif);
    randFromUnif = distribution(generator);
  }

}

double Rejection(std::vector<double> x, double alpha, double beta, double J)
{
  std::vector<double> samples;
  samples.resize(J);

  std::mt19937 generator(clock());
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  boost::math::beta_distribution<> mybeta(alpha, beta);

  //O estimador de max verossimilhança de uma bernoulli e sua media amostral
  double sum = 0;
  for (int i = 0; i < x.size(); i++)
  {
    sum+= x[i];
  }
  double M = pow(sum/x.size(),sum) * pow (1 - sum/x.size(), x.size() - sum);
  // std::cout << "Valor M e tamanho X: " << M << " " << x.size() << '\n';

  double cont = 0;
  while (cont < J)
  {
    double randFromUnif = distribution(generator);
    double candidate = quantile(mybeta, randFromUnif);
    randFromUnif = distribution(generator);

    //Considerando a g(O) como a priori temos que a parte direita da ineq sera a verossimilhança sobre o Estimador de MaxVer
    if (randFromUnif <= bernoulli_density(x, candidate)/M)
    {
      std::cout << "Valor: " << candidate << '\n';
      samples[cont] = candidate;
      cont++;
    }
  }
}

int main()
{
  double alpha = 2, beta = 5;
  std::vector<double> x (100, 0.5);
  //std::cout << "Valor: " << beta_density(x, alpha, beta) << '\n';
  std::mt19937 generator(clock());
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  double randFromUnif = distribution(generator);
  boost::math::beta_distribution<> dist(2,5);
  double randFromDist = quantile(dist, randFromUnif);
  std::cout << "Numero gerado: " << randFromDist << '\n';
  Rejection (x, alpha, beta, 10);

  return 0;
}
